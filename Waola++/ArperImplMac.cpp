#include "Waola++.h"

#include "ArperImplX.h"

namespace Wpp {

	int ArperImplX::setupSocket(const Subnet& subNet)
	{
		int s = 0;
		for (int i = 0; i < DefTryCount; ++i) {
			int errNo = 0;
			s = doSetupSocket(subNet, errNo);

			if (s >= 0 || errNo != EBUSY) {
				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(DefWaitTimeMs << i));
		}

		if (s < 0) {
			throw std::system_error(errno, std::system_category(),
				"failed to create ARP socket");
		}

		return s;
	}

	int ArperImplX::doSetupSocket(const Subnet& subNet, int& errNo)
	{
		static bpf_insn bpfInstr[] = {
			// Load word at octet 12
			BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 12),
			// If not ETHERTYPE_ARP, skip next 3 (and return nothing)
			BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ETHERTYPE_ARP, 0, 3),
			// Load word at octet 20
			BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 20),
			// If not ARPOP_REPLY, skip next 1 (and return nothing)
			BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ARPOP_REPLY, 0, 1),
			// Valid ARP reply received, return message
			BPF_STMT(BPF_RET | BPF_K, ArpPacketSize),
			// Return nothing
			BPF_STMT(BPF_RET | BPF_K, 0),
		};

		static constexpr bpf_program filter = {
			arrsiz(bpfInstr),
			bpfInstr
		};

		int fd = -1;
		char device[] = "/dev/bpf000";

		for (int i = 0; i < 1000; ++i) {
			snprintf(device, sizeof(device), "/dev/bpf%d", i);

			fd = open(device, O_RDWR);

			if (fd >= 0 || EBUSY != errno) {
				errNo = errno;
				break;
			}
		}

		if (fd < 0) {
			return fd;
		}

		ifreq ifreq{};
		strncpy(ifreq.ifr_name, subNet.IfaceName.c_str(), IFNAMSIZ);

		if (ioctl(fd, BIOCSETIF, &ifreq) == -1) {
			close(fd);
			return -1;
		}

		if (ioctl(fd, BIOCSETF, &filter) == -1) {
			close(fd);
			return -1;
		}

		uint yes = 1;

		if (ioctl(fd, BIOCIMMEDIATE, &yes) == -1) {
			close(fd);
			return -1;
		}

		return fd;
	}

	size_t ArperImplX::getResponseBufferSize(const std::string& ifName, int socket)
	{
		ifreq ifreq{};
		strncpy(ifreq.ifr_name, ifName.c_str(), IFNAMSIZ);
		u_int bufSize;

		if (ioctl(socket, BIOCGBLEN, &bufSize) == -1) {
			close(socket);
			throw std::system_error(errno, std::system_category(),
				"BIOCGBLEN failed");
		}

		return bufSize;
	}

	void ArperImplX::doReceiveResponses()
	{
		try {
			timeval timeout = {
				.tv_sec = ListenTimeoutMs / 1000,
				.tv_usec = ListenTimeoutMs % 1000 * 1000
			};

			auto buffer = std::make_unique<byte[]>(responseBufSize);

			if (!buffer) {
				std::stringstream ss;
				ss << "no " << responseBufSize << "B of free memory for ARP response buffer";
				throw std::system_error(errno, std::system_category(), ss.str());
			}

			if (ioctl(socket, BIOCSRTIMEOUT, &timeout) == -1) {
				throw std::system_error(errno, std::system_category(),
					"BIOCSRTIMEOUT failed");
			}

			while (!reqFinished && !cancelled) {
				ssize_t respLen = 0;

				while ((respLen = read(socket, buffer.get(), responseBufSize)) > 0) {
					if (respLen >= sizeof(bpf_hdr)) {
						const byte* ptr = buffer.get();
						auto bpfHdr = reinterpret_cast<const bpf_hdr*>(ptr);
						const auto replyLen = bpfHdr->bh_hdrlen + ArpPacketSize;

						while (respLen >= replyLen) {
							const auto etherArpOffset = bpfHdr->bh_hdrlen + sizeof(ether_header);
							const auto etherArp = reinterpret_cast<const ether_arp*>(ptr + etherArpOffset);

							processResponse(etherArp);

							ptr += replyLen;
							bpfHdr = reinterpret_cast<const struct bpf_hdr*>(ptr);
							respLen -= replyLen;
						}
					}
				}
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}
	}
} // namespace Wpp
