#ifndef KNOCKERUPPER_H
#define KNOCKERUPPER_H

#include <IKnockerUpper.h>

#include "SubnetGenerator.h"

namespace Wpp {

class KnockerUpper final : public IKnockerUpper {
public:
	KnockerUpper();
	~KnockerUpper() override;

	void wakeUp(const Subnet& subNet, const hosts_t& hosts) override;
	bool wakeUp(const subnets_t& subNets, const macs_t& macs) override;

private:
	void platformStartUp();
	void platformShutDown();
	void makeMagicPacket(MacAddress macAddr);
	static int makeSocket(const Subnet& subNet);
	bool wakeUp(const Subnet& subNet, const macs_t& macs);
	void wakeUp(int sock, const HostPtr& host, const sockaddr_in& destination);
	bool wakeUp(int sock, const MacAddress& host, const sockaddr_in& destination);
	static sockaddr_in makeSin(uint addr, ushort port);
	static int getSocketError();

private:
	static constexpr auto MagicPacketSize = 102;
	static constexpr short WolPort = 9;
	std::array<byte, MagicPacketSize> magicPacket;
};

} // Wpp

#endif //KNOCKERUPPER_H
