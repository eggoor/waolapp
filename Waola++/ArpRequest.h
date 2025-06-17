#ifndef ARPREQUESTU_H
#define ARPREQUESTU_H

#include <IpAddress.h>
#include <MacAddress.h>

namespace Wpp {

class ArpRequest
{
public:
    ArpRequest(const IpAddress& ipAddr, const MacAddress& macAddr);

	void setTarget(IpAddress ip);

    explicit operator const unsigned char* () const;

private:
    ether_header ether{};
    ether_arp arp{};
};

} // namespace Wpp

#endif // ARPREQUESTU_H
