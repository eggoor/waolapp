#include "ArperImplX.h"

namespace Wpp {

	int ArperImplX::setupSocket(const Subnet& subNet)
	{
		int errNo = 0;
		return doSetupSocket(subNet, errNo);
	}

	int ArperImplX::doSetupSocket(const Subnet& subNet, int& /*errNo*/)
	{
		auto s = ::socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));

		if (s < 0) {
			throw std::system_error(errno, std::system_category(),
				"failed to create ARP socket");
		}

		sockaddr_ll sll{ AF_PACKET, 0, static_cast<int>(subNet.IfaceIndex) };

		if (::bind(s, reinterpret_cast<sockaddr*>(&sll), sizeof(sockaddr_ll)) < 0) {
			throw std::system_error(errno, std::system_category(),
				"failed to bind ARP socket");
		}

		return s;
	}

	size_t ArperImplX::getResponseBufferSize(const std::string& ifName, int socket)
	{
		return 0;
	}

	void ArperImplX::doReceiveResponses()
	{
		try {
			while (!reqFinished && !cancelled) {
				timeval timeout = {
					.tv_sec = ListenTimeoutMs / 1000,
					.tv_usec = ListenTimeoutMs % 1000 * 1000
				};

				fd_set socks{};
				FD_SET(socket, &socks);

				int rc = select(socket + 1, &socks, NULL, NULL, &timeout);

				if (rc < 0) {
					throw std::system_error(errno, std::system_category(),
						"select on ARP socket failed");
				}

				if (rc == 0) {
					continue;
				}

				if (FD_ISSET(socket, &socks)) {
					byte buf[ArpPacketSize];
					auto respLen = recv(socket, buf, ArpPacketSize, 0);

					if (-1 == respLen) {
						throw std::system_error(errno, std::system_category(),
												"recvfrom ARP socket failed");
					}

					const auto response = reinterpret_cast<struct ethhdr*>(buf);
					if (response->h_proto != htons(ETHERTYPE_ARP)) {
						continue;
					}

					const auto etherArp = reinterpret_cast<struct ether_arp*>(buf + sizeof(ethhdr));
					if (etherArp->ea_hdr.ar_op != htons(ARPOP_REPLY)) {
						continue;
					}

					processResponse(etherArp);
				}
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}
	}
} // namespace Wpp
