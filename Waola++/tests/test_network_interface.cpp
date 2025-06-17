#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <cstring>
#include "Waola++/NetworkInterface.h"

using namespace Wpp;

namespace {

// Helper to create a dummy sockaddr_in
sockaddr_in make_sockaddr_in(uint32_t addr = 0x7F000001 /*127.0.0.1*/, uint16_t port = 0) {
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(addr);
    sa.sin_port = htons(port);
    return sa;
}

// Test construction and field access
TEST(NetworkInterfaceTest, ConstructAndFields) {
    auto sa = make_sockaddr_in();
    std::byte mac[ETHER_ADDR_LEN] = {std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}, std::byte{0x00}, std::byte{0x01}};
    NetworkInterface iface(1, "eth0", reinterpret_cast<sockaddr*>(&sa), 0xFFFFFF00, mac);
    EXPECT_EQ(iface.Index, 1u);
    EXPECT_EQ(iface.DisplayName, "eth0");
    EXPECT_EQ(iface.Ipv4Mask, 0xFFFFFF00u);
    for (int i = 0; i < ETHER_ADDR_LEN; ++i) {
        EXPECT_EQ(iface.MacAddr[i], mac[i]);
    }
    ASSERT_NE(iface.SockAddr, nullptr);
}

// Test isIpV4 and isIpV6
TEST(NetworkInterfaceTest, IpVersionChecks) {
    auto sa = make_sockaddr_in();
    std::byte mac[ETHER_ADDR_LEN] = {};
    NetworkInterface iface(2, "eth1", reinterpret_cast<sockaddr*>(&sa), 0xFFFFFF00, mac);
    EXPECT_TRUE(iface.isIpV4());
    EXPECT_FALSE(iface.isIpV6());
}

// Test isLoopback
TEST(NetworkInterfaceTest, LoopbackCheck) {
    auto sa = make_sockaddr_in(0x7F000001); // 127.0.0.1
    std::byte mac[ETHER_ADDR_LEN] = {};
    NetworkInterface iface(3, "lo", reinterpret_cast<sockaddr*>(&sa), 0xFF000000, mac);
    EXPECT_TRUE(iface.isLoopback());
}

// Test GetIpV4Addr and GetIpV4Mask
TEST(NetworkInterfaceTest, GetIpV4AddrAndMask) {
    auto sa = make_sockaddr_in(0xC0A80001); // 192.168.0.1
    std::byte mac[ETHER_ADDR_LEN] = {};
    NetworkInterface iface(4, "eth2", reinterpret_cast<sockaddr*>(&sa), 0xFFFFFF00, mac);
    EXPECT_EQ(iface.GetIpV4Addr(), 0xC0A80001u);
    EXPECT_EQ(iface.GetIpV4Mask(), 0xFFFFFF00u);
}

// Test static SockaddrDup
TEST(NetworkInterfaceTest, SockaddrDup) {
    auto sa = make_sockaddr_in(0x08080808); // 8.8.8.8
    auto dup = NetworkInterface::SockaddrDup(reinterpret_cast<sockaddr*>(&sa));
    ASSERT_NE(dup, nullptr);
    auto* sa_dup = reinterpret_cast<sockaddr_in*>(dup.get());
    EXPECT_EQ(sa_dup->sin_addr.s_addr, sa.sin_addr.s_addr);
    EXPECT_EQ(sa_dup->sin_family, sa.sin_family);
}

// Test handling of nullptr sockaddr
TEST(NetworkInterfaceTest, NullSockaddr) {
    std::byte mac[ETHER_ADDR_LEN] = {};
    // Should not crash, but SockAddr will be nullptr
    NetworkInterface iface(5, "null", nullptr, 0, mac);
    EXPECT_EQ(iface.SockAddr, nullptr);
}

} // namespace
