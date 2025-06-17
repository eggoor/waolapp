#include <HostEditable.h>
#include <IpAddress.h>
#include <MacAddress.h>

using namespace Wpp;

// Test construction
TEST(HostEditableTest, Construction) {
    IpAddress ip("192.168.1.2");
    MacAddress mac("00:11:22:33:44:55");
    HostEditable host("Device", "host.local", ip, mac);
    EXPECT_EQ(host.DisplayName, "Device");
    EXPECT_EQ(host.Hostname, "host.local");
    EXPECT_EQ(host.IpAddress.toString(), "192.168.1.2");
    EXPECT_EQ(host.MacAddress.toString(), "00:11:22:33:44:55");
}
