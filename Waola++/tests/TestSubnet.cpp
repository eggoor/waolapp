#include <Subnet.h>

using namespace Wpp;

// Test default construction
TEST(SubnetTest, Default) {
    Subnet subnet{1, "eth0", IpAddress(0),
        0, MacAddress()};
}

// Test construction from strings
TEST(SubnetTest, ConstructFromStrings) {
    Subnet subnet(1, "eth0", IpAddress("192.168.1.0"),
        24, MacAddress("00:11:22:33:44:55"));
}

// Test contains
TEST(SubnetTest, Contains) {
    Subnet subnet(1, "eth0", IpAddress("192.168.1.0"),
        24, MacAddress("00:11:22:33:44:55"));
}
