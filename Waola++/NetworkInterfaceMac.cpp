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
        static constexpr auto nboIpV4Loopback = htonl(0x7F000000);
        static constexpr auto ipV4LoopbackMask = 0xFF000000;
        const auto& nboIpAddr = reinterpret_cast<const sockaddr_in*>(SockAddr.get())->sin_addr.s_addr;
        isLoopback = (Ipv4Mask == ipV4LoopbackMask) && ((nboIpAddr & nboIpV4Loopback) == nboIpV4Loopback);
        break;
    }
    case AF_IMPLINK: {
        break; }
    case AF_PUP: {
        break; }
    case AF_CHAOS: {
        break; }
    case AF_NS: {
        break; }
    case AF_ISO: {
        break; }
    case AF_ECMA: {
        break; }
    case AF_DATAKIT: {
        break; }
    case AF_CCITT: {
        break; }
    case AF_SNA: {
        break; }
    case AF_DECnet: {
        break; }
    case AF_DLI: {
        break; }
    case AF_LAT: {
        break; }
    case AF_HYLINK: {
        break; }
    case AF_APPLETALK: {
        break; }
    case AF_ROUTE: {
        break; }
    case AF_LINK: {
        break; }
    case pseudo_AF_XTP: {
        break; }
    case AF_COIP: {
        break; }
    case AF_CNT: {
        break; }
    case pseudo_AF_RTIP: {
        break; }
    case AF_IPX: {
        break; }
    case AF_SIP: {
        break; }
    case pseudo_AF_PIP: {
        break; }
    case AF_NDRV: {
        break; }
    case AF_ISDN: {
        break; }
    case pseudo_AF_KEY: {
        break; }
    case AF_INET6: {
        const auto& ipv6Addr = reinterpret_cast<const sockaddr_in6*>(SockAddr.get())->sin6_addr;
        isLoopback = IN6_ARE_ADDR_EQUAL(&in6addr_loopback, &ipv6Addr);
        break;
    }
    case AF_NATM: {
        break; }
    case AF_SYSTEM: {
        break; }
    case AF_NETBIOS: {
        break; }
    case AF_PPP: {
        break; }
    case pseudo_AF_HDRCMPLT: {
        break; }
    case AF_RESERVED_36: {
        break; }
    case AF_IEEE80211: {
        break; }
    case AF_UTUN: {
        break; }
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
        inet_ntop(AF_INET, &reinterpret_cast<const sockaddr_in&>(sa).sin_addr,
                  addrString, INET_ADDRSTRLEN);
        os << addrString;
        break;
    }
    case AF_IMPLINK: {
        os << "arpanet imp address";
        break; }
    case AF_PUP: {
        os << "pup protocol (e.g. BSP)";
        break; }
    case AF_CHAOS: {
        os << "mit CHAOS protocol";
        break; }
    case AF_NS: {
        os << "XEROX NS protocol";
        break; }
    case AF_ISO: {
        os << "ISO protocol";
        break; }
    case AF_ECMA: {
        os << "European computer manufacturers";
        break; }
    case AF_DATAKIT: {
        os << "datakit protocol";
        break; }
    case AF_CCITT: {
        os << "CCITT protocol (X.25 etc)";
        break; }
    case AF_SNA: {
        os << "IBM SNA";
        break; }
    case AF_DECnet: {
        os << "DECnet";
        break; }
    case AF_DLI: {
        os << "DEC Direct data link interface";
        break; }
    case AF_LAT: {
        os << "LAT";
        break; }
    case AF_HYLINK: {
        os << "NSC Hyperchannel";
        break; }
    case AF_APPLETALK: {
        os << "Apple Talk";
        break; }
    case AF_ROUTE: {
        os << "Internal Routing Protocol";
        break; }
    case AF_LINK: {
        os << "Link layer interface";
        break; }
    case pseudo_AF_XTP: {
        os << "eXpress Transfer Protocol (no AF)";
        break; }
    case AF_COIP: {
        os << "connection-oriented IP, aka ST II";
        break; }
    case AF_CNT: {
        os << "Computer Network Technology";
        break; }
    case pseudo_AF_RTIP: {
        os << "Help Identify RTIP packets";
        break; }
    case AF_IPX: {
        os << "Novell Internet Protocol";
        break; }
    case AF_SIP: {
        os << "Simple Internet Protocol";
        break; }
    case pseudo_AF_PIP: {
        os << "Help Identify PIP packets";
        break; }
    case AF_NDRV: {
        os << "Network Driver 'raw' access";
        break; }
    case AF_ISDN: {
        os << "Integrated Services Digital Network";
        break; }
    case pseudo_AF_KEY: {
        os << "Internal key-management function";
        break; }
    case AF_INET6: {
        char addrString[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &reinterpret_cast<const sockaddr_in6&>(sa).sin6_addr,
                  addrString, INET6_ADDRSTRLEN);
        os << addrString;
        break;
    }
    case AF_NATM: {
        os << "native ATM access";
        break; }
    case AF_SYSTEM: {
        os << "Kernel event messages";
        break; }
    case AF_NETBIOS: {
        os << "NetBIOS";
        break; }
    case AF_PPP: {
        os << "PPP communication protocol";
        break; }
    case pseudo_AF_HDRCMPLT: {
        os << "Used by BPF";
        break; }
    case AF_RESERVED_36: {
        os << "Reserved for internal usage";
        break; }
    case AF_IEEE80211: {
        os << "IEEE 802.11 protocol";
        break; }
    case AF_UTUN: {
        os << "UTUN";
        break; }
    case AF_VSOCK: {
        os << "VM Sockets";
        break; }
    default: {
        // Do nothing
    }
    }

    return os;
}
}
