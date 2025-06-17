#ifndef HOST_EDITABLE_H
#define HOST_EDITABLE_H

//#include <memory>

#include "IpAddress.h"
#include "MacAddress.h"

namespace Wpp {

typedef std::shared_ptr<class HostEditable> HostEditablePtr;

class HostEditable: std::enable_shared_from_this<HostEditable>
{
public:
	HostEditable(const IpAddress& ipAddress, const MacAddress& macAddress)
		: HostEditable(std::string(), std::string(), ipAddress, macAddress)
	{}

	template<typename S1, typename S2>
	HostEditable(S1&& displayName, S2&& hostname,
		const Wpp::IpAddress& ipAddress,
		const Wpp::MacAddress& macAddress)
		: DisplayName(std::forward<S1>(displayName))
		, Hostname(std::forward<S2>(hostname))
		, IpAddress(ipAddress)
		, MacAddress(macAddress)
	{}

	std::string getDisplayNameSmart() {
		return !DisplayName.empty() ? DisplayName
				: (!Hostname.empty() ? Hostname
				: (!IpAddress.isZero() ? IpAddress.toString()
				: (!MacAddress.isZero() ? MacAddress.toString()
				: std::string())));
	}

	bool isEmpty() const;
	bool setDisplayName(std::string&& displayName);
	bool setHostname(std::string&& hostname);
	bool setIpAddress(const std::string& ipAddress);
	bool setMacAddress(const std::string& macAddress);

public:
	std::string DisplayName;
	std::string Hostname;
	Wpp::IpAddress IpAddress;
	Wpp::MacAddress MacAddress;
};

} // namespace Wpp

#endif // HOST_EDITABLE_H
