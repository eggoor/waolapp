#ifndef SUBNET_H
#define SUBNET_H

#include <IpAddress.h>
#include <MacAddress.h>

#include "Host.h"

namespace Wpp {
	class Subnet {
    public:
		Subnet(uint xIface, std::string&& ifaceName,
			IpAddress ip, uint mask, MacAddress macAddr);

		bool operator==(const Subnet& other) const;
        [[nodiscard]] uint hostCount() const;
        static uint calcSetBits(uint n);

		hosts_t extractSubnetHosts(hosts_t& hosts) const;
		[[nodiscard]] bool belongs(const HostPtr& host) const;

        const uint IfaceIndex;
        const std::string IfaceName;
		const IpAddress SelfIp;
        const uint Mask;
        const MacAddress MacAddr;
    };
} // namespace Wpp

#endif // SUBNET_H
