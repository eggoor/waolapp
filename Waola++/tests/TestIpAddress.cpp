#include <IpAddress.h>

using namespace Wpp;

// Test default construction (should be zero address)
TEST(IpAddressTest, DefaultZero) {
    const IpAddress ip(0);
    EXPECT_TRUE(ip.isZero());
}

// Test construction from string
TEST(IpAddressTest, ConstructFromString) {
    const IpAddress ip("192.168.1.1");
    EXPECT_FALSE(ip.isZero());
    EXPECT_EQ(ip.toString(), "192.168.1.1");
}

// Test invalid string
TEST(IpAddressTest, InvalidString) {
    const IpAddress ip("not.an.ip");
    EXPECT_TRUE(ip.isZero());
}

// Test increment
TEST(IpAddressTest, Increment) {
    IpAddress ip("10.0.0.1");
    ++ip;
    EXPECT_EQ(ip.toString(), "10.0.0.2");
}
