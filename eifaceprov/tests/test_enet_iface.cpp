extern "C" {
#include "e_net_iface.h"
}

// Test e_net_iface_get_index (should handle NULL)
TEST(ENetIfaceTest, GetIndexNull) {
    ASSERT_EQ(e_net_iface_get_index(nullptr), 0u);
}

// Test e_net_iface_get_name (should handle NULL)
TEST(ENetIfaceTest, GetNameNull) {
    ASSERT_EQ(e_net_iface_get_name(nullptr), nullptr);
}

// Test e_net_iface_get_socket_address (should handle NULL)
TEST(ENetIfaceTest, GetSocketAddressNull) {
    ASSERT_EQ(e_net_iface_get_socket_address(nullptr), nullptr);
}

// Test e_net_iface_get_ipv4_address (should handle NULL)
TEST(ENetIfaceTest, GetIPv4AddressNull) {
    ASSERT_EQ(e_net_iface_get_ipv4_address(nullptr), 0u);
}

// Test e_net_iface_get_ipv4_mask (should handle NULL)
TEST(ENetIfaceTest, GetIPv4MaskNull) {
    ASSERT_EQ(e_net_iface_get_ipv4_mask(nullptr), 0u);
}

// Test e_net_iface_get_mac_address (should handle NULL)
TEST(ENetIfaceTest, GetMacAddressNull) {
    ASSERT_EQ(e_net_iface_get_mac_address(nullptr), nullptr);
}

// Test e_iface_prov_get_sockaddr_len (should handle NULL)
TEST(ENetIfaceTest, GetSockaddrLenNull) {
    ASSERT_EQ(e_iface_prov_get_sockaddr_len(nullptr), 0);
}

// No main() here; main() is only in test_iface_prov.cpp
