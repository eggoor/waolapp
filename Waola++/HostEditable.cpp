#include "HostEditable.h"

namespace Wpp {

bool HostEditable::isEmpty() const
{
return DisplayName.empty() && Hostname.empty()
	&& IpAddress.isZero() && MacAddress.isZero();
}

bool HostEditable::setDisplayName(std::string&& displayName)
{
	bool hasSet(false);

	if (displayName != DisplayName) {
		std::swap(displayName, DisplayName);
		hasSet = true;
	}

	return hasSet;
}

bool HostEditable::setHostname(std::string&& hostname)
{
	bool hasSet(false);

	if (hostname != Hostname) {
		std::swap(hostname, Hostname);
		hasSet = true;
	}

	return hasSet;
}

bool HostEditable::setIpAddress(const std::string& ipAddress)
{
	bool hasSet(false);

	if (!ipAddress.empty()) {
		auto newIpAddress = Wpp::IpAddress(ipAddress);
		if (!newIpAddress.isZero() && IpAddress != newIpAddress){
			IpAddress = newIpAddress;
			hasSet = true;
		}
	}

	return hasSet;
}

bool HostEditable::setMacAddress(const std::string& macAddress)
{
	bool hasSet(false);

	if (!macAddress.empty()) {
		auto newMacAddress = Wpp::MacAddress(macAddress);
		if (!newMacAddress.isZero() && MacAddress != newMacAddress){
			MacAddress = newMacAddress;
			hasSet = true;
		}
	}

	return hasSet;
}

} // namespace Wpp
