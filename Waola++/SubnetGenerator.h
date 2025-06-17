#ifndef SUBNETGENERATOR_H
#define SUBNETGENERATOR_H

#include "IpAddrGenerator.h"
#include "Subnet.h"

namespace Wpp {

class SubnetGenerator
	: public IpAddrGenerator
	, public Subnet
{
public:
	SubnetGenerator(uint xIface, std::string&& ifaceName,
		IpAddress selfIp, uint mask, MacAddress macAddr);

	virtual ~SubnetGenerator() = default;

	// IpAddressGenerator interface
public:
	[[nodiscard]] const_iterator begin() const override;
	[[nodiscard]] const_iterator end() const override;
};

} // namespace Wpp

template <>
struct std::hash<Wpp::SubnetGenerator> {
	std::size_t operator()(const Wpp::SubnetGenerator& obj) const noexcept
	{
		return std::hash<int>{}(obj.IfaceIndex) // NOLINT(*-narrowing-conversions)
			   ^ std::hash<uint>{}(static_cast<uint>(obj.SelfIp & obj.Mask)) << 1;
	}
};

#endif // SUBNETGENERATOR_H
