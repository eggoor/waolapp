#ifndef MOCK_ISCANNER_H
#define MOCK_ISCANNER_H

#include <IScanner.h>

namespace Wpp {
    
class MockIScanner : public IScanner {
public:
    MOCK_METHOD(void, scanAsync, (subnets_t&&), (override));
    MOCK_METHOD(void, refreshAsync, (subnets_t&&, hosts_t&&), (override));
    MOCK_METHOD(void, finalizeAsyncOp, (bool), (override));
    MOCK_METHOD(void, cancel, (), (override));
    MOCK_METHOD(bool, isIdle, (), (const, override));
    MOCK_METHOD(void, wait, (), (override));
};
} // namespace Wpp

#endif // MOCK_ISCANNER_H
