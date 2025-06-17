#include "SubnetGenerator.h"

namespace Wpp {

SubnetGenerator::SubnetGenerator(uint xIface, std::string&& ifaceName,
	IpAddress selfIp, uint mask, MacAddress macAddr)
	: Subnet(xIface, std::move(ifaceName), selfIp, mask, macAddr)
{}

IpAddrGenerator::const_iterator Wpp::SubnetGenerator::begin() const
{
	return {(SelfIp & Mask) + 1};
}

IpAddrGenerator::const_iterator Wpp::SubnetGenerator::end() const
{
	return {SelfIp | ~Mask};
}

} // namespace Wpp
