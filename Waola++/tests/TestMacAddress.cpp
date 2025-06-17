#include <MacAddress.h>

using namespace Wpp;

// Test default construction (should be zero address)
TEST(MacAddressTest, DefaultZero) {
    MacAddress mac;
    EXPECT_TRUE(mac.isZero());
}

// Test construction from string
TEST(MacAddressTest, ConstructFromString) {
    MacAddress mac("00:11:22:33:44:55");
    EXPECT_FALSE(mac.isZero());
    EXPECT_EQ(mac[0], 0x00);
    EXPECT_EQ(mac[1], 0x11);
    EXPECT_EQ(mac[2], 0x22);
    EXPECT_EQ(mac[3], 0x33);
    EXPECT_EQ(mac[4], 0x44);
    EXPECT_EQ(mac[5], 0x55);
}

// Test invalid string
TEST(MacAddressTest, InvalidString) {
    MacAddress mac("not:a:mac");
    EXPECT_TRUE(mac.isZero());
}

// Test toString
TEST(MacAddressTest, ToString) {
    MacAddress mac("00:11:22:33:44:55");
    EXPECT_EQ(mac.toString(), "00:11:22:33:44:55");
}

// Test different delimiters
TEST(MacAddressTest, DifferentDelimiters) {
    MacAddress mac1("00-11-22-33-44-55");
    MacAddress mac2("00:11:22:33:44:55");
    MacAddress mac3("001122334455");
    EXPECT_EQ(mac1[0], mac2[0]);
    EXPECT_EQ(mac1[5], mac2[5]);
    EXPECT_EQ(mac1[0], mac3[0]);
    EXPECT_EQ(mac1[5], mac3[5]);
}

// Test out-of-range access
TEST(MacAddressTest, OutOfRangeAccess) {
    MacAddress mac("00:11:22:33:44:55");
    EXPECT_THROW(mac[6], std::out_of_range);
}
