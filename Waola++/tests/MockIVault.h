#ifndef MOCK_IVAULT_H
#define MOCK_IVAULT_H

#include <IVault.h>

namespace Wpp {
class MockIVault : public IVault {
public:
    MOCK_METHOD(void, init, (std::vector<Wpp::HostPtr>&&), (override));
    HostPtr operator[](size_t idx) const override { return getAt(idx); }
    MOCK_METHOD(HostPtr, getAt, (size_t), (const));
    MOCK_METHOD(HostPtr, at, (size_t), (const, override));
    MOCK_METHOD(size_t, hostCount, (), (const, override));
    MOCK_METHOD(bool, isEmpty, (), (const, override));
    MOCK_METHOD(VaultOpResult, tryAdd, (Host&&), (override));
    MOCK_METHOD(VaultOpResult, tryUpdateL7, (const L7Result&), (override));
    MOCK_METHOD(const std::vector<HostPtr>&, getHosts, (), (const, override));
    MOCK_METHOD(void, removeHosts, (size_t, size_t), (override));
};
} // namespace Wpp

#endif // MOCK_IVAULT_H
