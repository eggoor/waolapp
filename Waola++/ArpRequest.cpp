#include "ArpRequest.h"

namespace Wpp {

ArpRequest::ArpRequest(const IpAddress& ipAddr, const MacAddress& macAddr)
{
    memset(&ether.ether_dhost, 0xff, ETHER_ADDR_LEN);

    memcpy(&ether.ether_shost, static_cast<const unsigned char*>(macAddr), ETHER_ADDR_LEN);
    ether.ether_type = htons(ETHERTYPE_ARP);

    arp.arp_hrd = htons(ARPHRD_ETHER);
    arp.arp_pro = htons(ETHERTYPE_IP);
    arp.arp_hln = ETHER_ADDR_LEN;
    arp.arp_pln = 4;
    arp.arp_op = htons(ARPOP_REQUEST);
    memcpy(arp.arp_sha, ether.ether_shost, ETHER_ADDR_LEN);
    memset(arp.arp_tha, 0, ETHER_ADDR_LEN);

    const auto selfIp = ipAddr.toNetworkByteOrder();
    memcpy(arp.arp_spa, &selfIp, sizeof(selfIp));

    static_assert(sizeof(*this) == sizeof(ether) + sizeof(arp),
        "sizeof(ArpRequest) != sizeof(ether_header) + sizeof(ether_arp)");
}

void ArpRequest::setTarget(IpAddress ip)
{
	auto nbo = htonl(static_cast<uint>(ip));
    memcpy(arp.arp_tpa, &nbo, sizeof(nbo));
}

ArpRequest::operator const unsigned char* () const
{
    return reinterpret_cast<const unsigned char*>(this);
}

} // namespace Wpp
