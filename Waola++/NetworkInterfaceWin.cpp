#include "NetworkInterface.h"

#include <cassert>

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
        break;
    }
    case AF_DECnet: {
        break; }
    case AF_APPLETALK: {
        break;
    }
    case AF_IPX: {
        break;
    }
    case AF_INET6: {
        const auto& ipv6Addr = reinterpret_cast<const sockaddr_in6*>(SockAddr.get())->sin6_addr;
        isLoopback = IN6_ARE_ADDR_EQUAL(&in6addr_loopback, &ipv6Addr);
        break;
    }
    default: {
        assert(false);
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
    case AF_IMPLINK: {
        os << "arpanet imp addresses";
        break;
    }
    case AF_PUP: {
        os << "pup protocols: e.g. BSP";
        break;
    }
    case AF_CHAOS: {
        os << "mit CHAOS protocols";
        break;
    }
    case AF_NS: {
        os << "XEROX NS protocols/IPX protocols: IPX, SPX, etc.";
        break;
    }
    case AF_ISO: {
        os << "ISO protocols (OSI is ISO)";
        break;
    }
    case AF_ECMA: {
        os << "european computer manufacturers";
        break;
    }
    case AF_DATAKIT: {
        os << "datakit protocols";
        break;
    }
    case AF_CCITT: {
        os << "CCITT protocols, X.25 etc";
        break;
    }
    case AF_SNA: {
        os << "IBM SNA";
        break;
    }
    case AF_DECnet: {
        os << "DECnet";
        break;
    }
    case AF_DLI: {
        os << "Direct data link interface";
        break;
    }
    case AF_LAT: {
        os << "LAT";
        break;
    }
    case AF_HYLINK: {
        os << "NSC Hyperchannel";
        break;
    }
    case AF_APPLETALK: {
        os << "AppleTalk";
        break;
    }
    case AF_NETBIOS: {
        os << "NetBios-style addresses";
        break;
    }
    case AF_VOICEVIEW: {
        os << "VoiceView";
        break;
    }
    case AF_FIREFOX: {
        os << "Protocols from Firefox";
        break;
    }
    case AF_UNKNOWN1: {
        os << "Somebody is using this!";
        break;
    }
    case AF_BAN: {
        os << "Banyan";
        break;
    }
    case AF_ATM: {
        os << "Native ATM Services";
        break;
    }
    case AF_INET6: {
        char addrString[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &reinterpret_cast<const struct sockaddr_in6&>(sa).sin6_addr,
            addrString, INET6_ADDRSTRLEN);
        os << addrString;
        break;
    }
    case AF_CLUSTER: {
        os << "Microsoft Wolfpack";
        break;
    }
    case AF_12844: {
        os << "IEEE 1284.4 WG AF";
        break;
    }
    case AF_IRDA: {
        os << "IrDA";
        break;
    }
    case AF_NETDES: {
        os << "Network Designers OSI & gateway";
        break;
    }
    default: {
        // Do nothing
    }
    }

    return os;
}

} // namespace Wpp
