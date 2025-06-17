#include <e_iface_prov.h>

#include "Subnet.h"

#include "NetworkInterface.h"

namespace Wpp {

NetworkInterface::NetworkInterface(const uint index, std::string&& displayName,
    const sockaddr* const sa, const uint ipv4Mask, const byte* const macAddr)
    : Index(index)
    , DisplayName(std::move(displayName))
    , Ipv4Mask(ipv4Mask)
    , MacAddr{macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]}
    , SockAddr(SockaddrDup(sa))
{
}

std::unique_ptr<sockaddr> NetworkInterface::SockaddrDup(const sockaddr* const sa) {
    sockaddr* dup = nullptr;
    const auto saLen = e_iface_prov_get_sockaddr_len(sa);

    if (saLen > 0) {
        // ReSharper disable once CppDFAMemoryLeak
        dup = static_cast<sockaddr*>(malloc(saLen));

        if (!dup) {
            throw std::bad_alloc();
        }

        memcpy(dup, sa, saLen);
    }

    return std::unique_ptr<sockaddr>(dup);
}

bool NetworkInterface::isIpV4() const {
    return SockAddr->sa_family == AF_INET;
}

bool NetworkInterface::isIpV6() const {
    return SockAddr->sa_family == AF_INET6;
}

uint NetworkInterface::GetIpV4Addr() const {
    return ntohl(reinterpret_cast<const struct sockaddr_in*>(SockAddr.get())->sin_addr.s_addr);
}

uint NetworkInterface::GetIpV4Mask() const {
    return Ipv4Mask;
}

std::ostream& operator <<(std::ostream& os, const NetworkInterface& iface) {
    auto prevFill = os.fill('0');
    os << std::setw(2) << iface.Index;
    os.fill(prevFill);

    os << ": \"" << iface.DisplayName << "\"\t"
       << *iface.SockAddr;

    if (iface.isIpV4())
		os << '/' << Subnet::calcSetBits(iface.Ipv4Mask);

    return os;
}

} // namespace Wpp
