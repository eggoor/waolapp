#ifndef L7_RESULT_H
#define L7_RESULT_H

#include "HostField.h"
#include "IpAddress.h"

namespace Wpp {

class L7Result
{
public:
    template<typename TIpAddr, typename S>
	L7Result(const HostField sourceOfTruth, TIpAddr&& ipAddr, S&& hostname)
		: sourceOfTruth(sourceOfTruth)
		, IpAddr(std::forward<TIpAddr>(ipAddr))
        , Hostname(std::forward<S>(hostname))
    {
    }

	const HostField sourceOfTruth;
    const IpAddress IpAddr;
    const std::string Hostname;
};

} // namespace Wpp

#endif // L7_RESULT_H
