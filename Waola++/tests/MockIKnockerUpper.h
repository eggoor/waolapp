#ifndef MOCK_IKNOCKERUPPER_H
#define MOCK_IKNOCKERUPPER_H

#include <gmock/gmock.h>
#include <IKnockerUpper.h>

namespace Wpp {
class MockIKnockerUpper : public IKnockerUpper {
public:
    MOCK_METHOD(void, wakeUp, (const Subnet&, const hosts_t&), (override));
    MOCK_METHOD(bool, wakeUp, (subnets_t&&, const macs_t&), (override));
};
} // namespace Wpp

#endif // MOCK_IKNOCKERUPPER_H
