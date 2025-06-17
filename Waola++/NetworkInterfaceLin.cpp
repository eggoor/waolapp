#include "NetworkInterface.h"

namespace Wpp {

bool NetworkInterface::isLoopback() const {
    bool isLoopback(false);
    switch (SockAddr->sa_family) {
    case AF_UNSPEC: {
        break;
    }
    case AF_UNIX: {
        break;
    }
    case AF_INET: {
        static const auto nboIpV4Loopback = htonl(0x7F000000);
        static constexpr auto ipV4LoopbackMask = 0xFF000000;
        const auto& nboIpAddr = reinterpret_cast<const sockaddr_in*>(SockAddr.get())->sin_addr.s_addr;
        isLoopback = (Ipv4Mask == ipV4LoopbackMask) && ((nboIpAddr & nboIpV4Loopback) == nboIpV4Loopback);
        break;
    }
    case AF_SNA: {
        break; }
    case AF_DECnet: {
        break; }
    case AF_APPLETALK: {
        break; }
    case AF_ROUTE: {
        break; }
    case AF_IPX: {
        break; }
    case AF_ISDN: {
        break; }
    case AF_INET6: {
        const auto& ipv6Addr = reinterpret_cast<const sockaddr_in6*>(SockAddr.get())->sin6_addr;
        isLoopback = IN6_ARE_ADDR_EQUAL(&in6addr_loopback, &ipv6Addr);
        break;
    }
    case AF_VSOCK: {
        break; }
    default: {
        // Do nothing
    }
    }

    return isLoopback;
}

std::ostream& operator <<(std::ostream& os, const sockaddr& sa) {
    switch (sa.sa_family) {
    case AF_UNSPEC: {
        os << "unspecified";
        break;
    }
    case AF_UNIX: {
        os << "local to host (pipes)";
        break;
    }
    case AF_INET: {
        char addrString[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &reinterpret_cast<const struct sockaddr_in&>(sa).sin_addr,
                  addrString, INET_ADDRSTRLEN);
        os << addrString;
        break;
    }
    case AF_SNA: {
        os << "IBM SNA";
        break; }
    case AF_DECnet: {
        os << "DECnet";
        break; }
    case AF_APPLETALK: {
        os << "Apple Talk";
        break; }
    case AF_ROUTE: {
        os << "Internal Routing Protocol";
        break; }
    case AF_IPX: {
        os << "Novell Internet Protocol";
        break; }
    case AF_ISDN: {
        os << "Integrated Services Digital Network";
        break; }
    case AF_INET6: {
        char addrString[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &reinterpret_cast<const struct sockaddr_in6&>(sa).sin6_addr,
                  addrString, INET6_ADDRSTRLEN);
        os << addrString;
        break;
    }
    case AF_VSOCK: {
        os << "VM Sockets";
        break; }
    default: {
        // Do nothing
    }
    }

    return os;
}

} // namespace Wpp
