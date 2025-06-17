#ifndef HOST_SAVED_H
#define HOST_SAVED_H

#include "HostEditable.h"

namespace Wpp {

typedef std::shared_ptr<class HostSaved> HostSavedPtr;

class HostSaved : public HostEditable
{
public:
    HostSaved(const class IpAddress& ipAddress, const class MacAddress& macAddress, const time_t lastSeenOnline = std::time(nullptr))
        : HostSaved(std::string(), std::string(), ipAddress, macAddress, lastSeenOnline)
    {
    }

    template<typename S1, typename S2>
    HostSaved(S1&& displayName, S2&& hostname, const class IpAddress& ipAddress,
		const class MacAddress& macAddress , const time_t lastSeenOnline = 0)
        : HostEditable(std::forward<S1>(displayName), std::forward<S2>(hostname), ipAddress, macAddress)
        , LastSeenOnline(lastSeenOnline)
    {
    }

    HostSaved(HostSaved&& host)
        : HostEditable(std::move(host.DisplayName), std::move(host.Hostname),
                       std::move(host.IpAddress), std::move(host.MacAddress))
        , LastSeenOnline(host.LastSeenOnline)
    {}

public:
    time_t LastSeenOnline;
};

} // namespace Wpp

#endif // HOST_SAVED_H
