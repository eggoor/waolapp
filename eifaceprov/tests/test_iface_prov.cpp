#ifndef arrsiz
    #define arrsiz(a) (sizeof(a) / sizeof((*a)))
#endif

extern "C" {
#include "e_iface_prov.h"
#include "net_iface.h"
}

#include "mock_e_iface_prov.h"

using ::testing::Return;

// Example test for e_iface_prov_get_first
TEST(EifaceProvTest, GetFirstNotNull) {
    struct e_net_iface* iface = e_iface_prov_get_first();
    e_iface_prov_free(iface);
    ASSERT_NE(iface, nullptr);
}

// Test e_iface_prov_get_first
TEST(EifaceProvTest, GetFirst) {
    e_net_iface_t* const iface = e_iface_prov_get_first();
    if (iface) e_iface_prov_free(iface);
    SUCCEED();
}

// Test e_iface_prov_get_next
TEST(EifaceProvTest, GetNextNull) {
    e_net_iface_t* const next = e_iface_prov_get_next(nullptr);
    ASSERT_EQ(next, nullptr);
}

// Test e_iface_prov_free (no-op for NULL)
TEST(EifaceProvTest, FreeNull) {
    e_iface_prov_free(nullptr);
    SUCCEED();
}

TEST(MockEIfaceProvTest, GetFirstAndNextStructChain) {
    e_net_iface ifaces[3];

    auto i = arrsiz(ifaces);
    ifaces[i - 1].next = nullptr;

    while (i --> 1) {
        ifaces[i - 1].next = &ifaces[i];
    }

    MockEIfaceProv mock;
    EXPECT_CALL(mock, get_first()).WillOnce(Return(reinterpret_cast<e_net_iface_t*>(&ifaces[0])));

    e_net_iface_t* first = mock.get_first();
    ASSERT_EQ(first, reinterpret_cast<e_net_iface_t*>(&ifaces[0]));
    
    e_net_iface_t* next = first;

    while (next) {
        ASSERT_EQ(next, reinterpret_cast<e_net_iface_t*>(&ifaces[i++]));
        next = e_iface_prov_get_next(next);
    }

    ASSERT_EQ(next, nullptr);
    ASSERT_EQ(i, arrsiz(ifaces));
}

// No main() here; main() is only in main.cpp
