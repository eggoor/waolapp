#ifndef ARP_RESULT_H
#define ARP_RESULT_H

#include <IpAddress.h>
#include <MacAddress.h>

namespace Wpp {

class ArpResult
{
public:
    template<typename TIpAddr, typename TMacAddr>
    ArpResult(TIpAddr&& ipAddr, TMacAddr&& macAddr)
        : IpAddr(std::forward<TIpAddr>(ipAddr))
        , MacAddr(std::forward<TMacAddr>(macAddr))
    {
    }

    const IpAddress IpAddr;
    const MacAddress MacAddr;
};

} // namespace Wpp

#endif // ARP_RESULT_H
