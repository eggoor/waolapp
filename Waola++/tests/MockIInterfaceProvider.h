#ifndef MOCK_IINTERFACEPROVIDER_H
#define MOCK_IINTERFACEPROVIDER_H

#include <gmock/gmock.h>

#include <IInterfaceProvider.h>
#include <NetworkInterface.h>

namespace Wpp {

class MockIInterfaceProvider : public IInterfaceProvider {
public:
    MockIInterfaceProvider() = default;
    ~MockIInterfaceProvider() override = default;

    MOCK_METHOD(Wpp::network_interfaces_t, getInterfaces, (), (const, override));
};

} // namespace Wpp

#endif // MOCK_IINTERFACEPROVIDER_H
