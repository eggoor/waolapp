#include <Arper.h>

using namespace Wpp;

// Test basic construction
TEST(ArperTest, Construction) {
    Arper arper([](const ArpResult&){});
    SUCCEED();
}
