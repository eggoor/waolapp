#ifndef MOCK_INETWORK_H
#define MOCK_INETWORK_H

#include <INetwork.h>

using INetwork = Wpp::INetwork;
using HostPtr = Wpp::HostPtr;
using args_t = Wpp::args_t;

class MockINetwork : public INetwork {
public:
    MOCK_METHOD(void, init, (std::vector<HostPtr>&&), (override));
	HostPtr operator[](size_t idx) const override { return getAt(idx); }
	MOCK_METHOD(HostPtr, getAt, (size_t), (const));
	MOCK_METHOD(HostPtr, at, (size_t), (const, override));
    MOCK_METHOD(size_t, hostCount, (), (const, override));
    MOCK_METHOD(bool, isEmpty, (), (const, override));
    MOCK_METHOD(bool, isIdle, (), (const, override));
    MOCK_METHOD(const std::vector<HostPtr>&, getHosts, (), (const, override));
    MOCK_METHOD(bool, addHost, (std::string&&, std::string&&,
        const std::string&, const std::string&), (override));
    MOCK_METHOD(void, removeHosts, (size_t, size_t), (override));
    MOCK_METHOD(void, wakeUp, (std::vector<int>&&), (override));
    MOCK_METHOD(bool, wakeUp, (const args_t&), (override));
    MOCK_METHOD(void, scanAsync, (), (override));
    MOCK_METHOD(void, refreshAsync, (), (override));
    MOCK_METHOD(void, finalizeAsyncOp, (bool), (override));
    MOCK_METHOD(void, cancel, (), (override));
    MOCK_METHOD(void, wait, (), (override));
};

#endif // MOCK_INETWORK_H
