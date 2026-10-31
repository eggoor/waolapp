
#include "KnockerUpper.h"

namespace Wpp {
	KnockerUpperPtr IKnockerUpper::make()
	{
		return std::make_unique<KnockerUpper>();
	}

	KnockerUpper::KnockerUpper()
		: magicPacket{ byte{ 0xFF }, byte{ 0xFF }, byte{ 0xFF },
			byte{ 0xFF }, byte{ 0xFF }, byte{ 0xFF } }
	{
		platformStartUp();
	}

	Wpp::KnockerUpper::~KnockerUpper()
	{
		platformShutDown();
	}

	void KnockerUpper::wakeUp(const Subnet& subNet, const hosts_t& hosts)
	{
		const auto s = makeSocket(subNet);

		const auto broadcast = static_cast<uint>(subNet.SelfIp) | ~subNet.Mask;
		const auto destination = makeSin(broadcast, WolPort);

		try {
			for (const auto& host : hosts) {
				wakeUp(s, host, destination);
			}
		}
		catch (...) {
			close(s);
			throw;
		}

		close(s);
	}

	void KnockerUpper::wakeUp(const int sock, const HostPtr& host,
    	const sockaddr_in& destination)
	{
		bool success = wakeUp(sock, host->MacAddress, destination);
		host->OpResult = success ? orSuccess : orFail;

		if (!success) {
			throw std::system_error(getSocketError(), std::system_category(),
				"failed to send WoL magic packet");
		}
	}

	bool KnockerUpper::wakeUp(int sock, const MacAddress& macAddr,
    	const sockaddr_in& destination)
	{
		makeMagicPacket(macAddr);

		auto sentBytes = sendto(sock, magicPacket.data(), MagicPacketSize,
			0, std::bit_cast<sockaddr*>(&destination), sizeof(destination));

		return sentBytes == MagicPacketSize;
	}

	bool KnockerUpper::wakeUp(const subnets_t& subNets, const macs_t& macs)
	{
		bool success = true;

		for (const auto& subNet : subNets) {
			success &= wakeUp(subNet, macs);
		}

		return success;
	}

	bool KnockerUpper::wakeUp(const Subnet& subNet, const macs_t& macs)
	{
		bool success = true;

		const uint broadcast =
		static_cast<uint>(subNet.SelfIp) | ~subNet.Mask;
		const auto destination = makeSin(broadcast, WolPort);

		const auto s = makeSocket(subNet);

		for (const auto& mac : macs) {
			success &= wakeUp(s, mac, destination);
		}

		close(s);

		return success;
	}

	void KnockerUpper::makeMagicPacket(MacAddress macAddr)
	{
		for (int i = ETHER_ADDR_LEN; i < ETHER_ADDR_LEN * 2; ++i) {
			magicPacket[i] = byte{ (macAddr[i - ETHER_ADDR_LEN]) };
		}

		for (int i = ETHER_ADDR_LEN * 2; i < MagicPacketSize; ++i) {
			magicPacket[i] = byte{ 0xFF };
		}
	}

	int KnockerUpper::makeSocket(const Subnet& subNet)
	{
		int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (s == -1) {
			throw std::system_error(getSocketError(), std::system_category(),
				"failed to create WoL socket");
		}

		sockaddr_in sinBind = makeSin(static_cast<uint>(subNet.SelfIp), 0);

		int rc = bind(s, std::bit_cast<sockaddr*>(&sinBind), sizeof sinBind);

		if (rc < 0) {
			close(s);
			throw std::system_error(getSocketError(), std::system_category(),
				"failed to bind WoL socket");
		}

		constexpr int True = 1;

		rc = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &True, sizeof(int));

		if (rc) {
			close(s);
			throw std::system_error(getSocketError(), std::system_category(),
				"setsockopt(SO_BROADCAST) failed");
		}

		return s;
	}
} // namespace Wpp
