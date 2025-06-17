#include "Subnet.h"

namespace Wpp {

Subnet::Subnet(const uint xIface, std::string&& ifaceName,
	IpAddress ip, const uint mask, MacAddress macAddr)
	: IfaceIndex(xIface)
	, IfaceName(std::move(ifaceName))
	, SelfIp(ip)
	, Mask(mask)
	, MacAddr(macAddr)
{}

bool Subnet::operator==(const Subnet& other) const
{
	return (this == &other)
		|| (IfaceIndex == other.IfaceIndex
			&& (SelfIp & Mask) == (other.SelfIp & other.Mask));
}

#define P2(n) (1U << (n))

uint Subnet::hostCount() const
{
	return P2(sizeof(Mask) * 8 - calcSetBits(Mask)) - 2;
}

uint Subnet::calcSetBits(uint n) {
	n = n - ((n >> 1) & 0x55555555);                // add pairs of bits
	n = (n & 0x33333333) + ((n >> 2) & 0x33333333); // quads
	n = (n + (n >> 4)) & 0x0F0F0F0F;                // groups of 8
	n *= 0x01010101;                                // horizontal sum of bytes
	// return just that top byte (after truncating to 32-bit even when int is wider than uint)
	return  n >> 24;
}

hosts_t Subnet::extractSubnetHosts(hosts_t& hosts) const
{
	hosts_t subnetHosts;

	const auto it = 
		
#if __cplusplus >= 202002L
		std::ranges::remove_if(hosts,
		[this, &subnetHosts]<typename THost>(THost&& host) {
			if (belongs(host)) {
				subnetHosts.emplace_back(std::forward<THost>(host));
				return true;
			}
			return false;
	}).begin();
#else
		std::remove_if(hosts.begin(), hosts.end(),
		[this, &subnetHosts](const HostPtr& host) {
			if (belongs(host)) {
				subnetHosts.push_back(host);
				return true;
			}
			return false;
			});
#endif
	hosts.erase(it, hosts.end());

	return subnetHosts;
}

bool Subnet::belongs(const HostPtr& host) const
{
	return !host->IpAddress.isZero() && (SelfIp & Mask) == (host->IpAddress & Mask);
}

} // namespace Wpp
