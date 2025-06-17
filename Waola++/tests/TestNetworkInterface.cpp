#include <NetworkInterface.h>

using namespace Wpp;

#if CMAKE_CXX_STANDARD <= 14 && __cplusplus == 201703L
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
} // namespace Wpp

#endif

// Test construction (dummy values)
TEST(NetworkInterfaceTest, Construction) {
    sockaddr sa{};
    Wpp::byte mac[ETHER_ADDR_LEN] = {};
    NetworkInterface iface(1, "eth42", &sa, 0xFFFFFF00, mac);
    EXPECT_EQ(iface.Index, 1u);
    EXPECT_EQ(iface.DisplayName, "eth42");
}
