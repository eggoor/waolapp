#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "EtherAddrLen.h"

namespace Wpp {

class NetworkInterface
{
public:
    NetworkInterface(uint index, std::string&& displayName, const sockaddr* sa,
        uint ipv4Mask, const byte* macAddr);
    ~NetworkInterface() = default;
    NetworkInterface() = delete;
    NetworkInterface(const NetworkInterface&) = delete;
    NetworkInterface(NetworkInterface&&) = delete;

    friend std::ostream& operator <<(std::ostream& os, const NetworkInterface& iface);

public:
    const uint Index;
    const std::string DisplayName;
    const uint Ipv4Mask;
    const byte MacAddr[ETHER_ADDR_LEN];
    const std::unique_ptr<sockaddr> SockAddr;

public:
    bool isIpV4() const;
    bool isIpV6() const;
    bool isLoopback() const;
    uint GetIpV4Addr() const;
    uint GetIpV4Mask() const;

    static std::unique_ptr<sockaddr> SockaddrDup(const sockaddr* const sa);
};

std::ostream& operator <<(std::ostream& os, const sockaddr& sa);

} // namespace Wpp

#endif // NETWORK_INTERFACE_H
