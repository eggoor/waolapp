
#include "KnockerUpper.h"

namespace Wpp {
	KnockerUpperPtr IKnockerUpper::make()
	{
		return std::make_unique<KnockerUpper>();
	}

	KnockerUpper::KnockerUpper()
		: magicPacket{ byte{ 0xFF }, byte{ 0xFF }, byte{ 0xFF },
			byte{ 0xFF }, byte{ 0xFF }, byte{ 0xFF } }
		, sinBroadcast(makeSin(INADDR_BROADCAST, WolPort))
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

		for (const auto& host : hosts) {
			wakeUp(s, host);
		}

		close(s);
	}

	void KnockerUpper::wakeUp(const int sock, const HostPtr& host)
	{
		bool success = wakeUp(sock, host->MacAddress);
		host->OpResult = success ? orSuccess : orFail;
	}

	bool KnockerUpper::wakeUp(int sock, const MacAddress& macAddr)
	{
		makeMagicPacket(macAddr);

		auto sentBytes = sendto(sock, reinterpret_cast<const char*>(magicPacket), MagicPacketSize,
			0, reinterpret_cast<sockaddr*>(&sinBroadcast), sizeof(sinBroadcast));

		return sentBytes == MagicPacketSize;
	}

	bool KnockerUpper::wakeUp(subnets_t&& subNets, const macs_t& macs)
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

		const auto s = makeSocket(subNet);

		for (const auto& mac : macs) {
			success &= wakeUp(s, mac);
		}

		close(s);

		return success;
	}

	void KnockerUpper::makeMagicPacket(MacAddress macAddr)
	{
		for (int i = ETHER_ADDR_LEN; i < MagicPacketSize; ++i) {
			magicPacket[i] = byte{ (macAddr[i % ETHER_ADDR_LEN]) };
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

		int rc = bind(s, reinterpret_cast<sockaddr*>(&sinBind), sizeof sinBind);

		if (rc < 0) {
			close(s);
			throw std::system_error(getSocketError(), std::system_category(),
				"failed to bind WoL socket");
		}

		constexpr char True = 1;

		rc = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &True, sizeof(int));

		if (rc) {
			close(s);
			throw std::system_error(getSocketError(), std::system_category(),
				"setsockopt(SO_BROADCAST) failed");
		}

		return s;
	}
} // namespace Wpp
