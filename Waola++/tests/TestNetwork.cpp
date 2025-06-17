#include <Network.h>

#include "MockIInterfaceProvider.h"
#include "MockIScanner.h"
#include "MockIKnockerUpper.h"
#include "NetworkTestFixture.h"

using namespace Wpp;

NetworkParams NetworkTestFixture::makeNetworkParams(FScannerFactory scannerFactory,
	InterfaceProviderPtr interfaceProvider, KnockerUpperPtr knockerUpper,
	FHostDetectedCb cbHostDetected, FHostChangedCb cbHostChanged,
	FScanStateChangedCb cbScanFinished)
{  
	NetworkParams params;  

	params.interfaceProvider = std::move(interfaceProvider);  
	params.knockerUpper = std::move(knockerUpper);  
	params.scannerFactory = std::move(scannerFactory);
	params.vaultFactory = &IVault::create;  
	params.cbHostDetected = cbHostDetected;
	params.cbHostChanged = cbHostChanged;
	params.cbScanFinished = cbScanFinished;

	return params;  
}

Network NetworkTestFixture::makeNetworkWithHosts(FScannerFactory scannerFactory,
	InterfaceProviderPtr interfaceProvider, KnockerUpperPtr knockerUpper,
	FHostDetectedCb cbHostDetected, FHostChangedCb cbHostChanged,
	FScanStateChangedCb cbScanFinished)
{
	Network net{makeNetworkParams(std::move(scannerFactory),
		std::move(interfaceProvider), std::move(knockerUpper),
		cbHostDetected, cbHostChanged,cbScanFinished)};

	std::vector<HostPtr> hosts;
	hosts.push_back(makeHost("192.168.1.1", "01:23:45:67:89:AB"));
	hosts.push_back(makeHost("192.168.1.2", "AA:78:45:67:89:AB"));
	hosts.push_back(makeHost("192.168.1.254", "AA:BB:CC:DD:EE:FF"));
	net.init(std::move(hosts));

	return net;
}

std::unique_ptr<MockIScanner> NetworkTestFixture::makeScanner(const ScannerParams& /*sp*/)
{
	return std::make_unique<MockIScanner>();
}

network_interfaces_t NetworkTestFixture::makeIfaces()
{
	const IpAddress ip("192.168.1.0");
	sockaddr_in sin{};
	sin.sin_family = AF_INET;

#ifdef __APPLE__
	sin.sin_len = sizeof(sin);
#endif

	sin.sin_addr.s_addr = ip.toNetworkByteOrder();
	constexpr Wpp::byte macAddr[ETHER_ADDR_LEN] = {};

	network_interfaces_t ifaces;
	const auto [it, _] = ifaces.emplace(0, std::vector<NetworkInterfacePtr>());
	it->second.emplace_back(std::make_unique<NetworkInterface>(0, "eth0",
		reinterpret_cast<const sockaddr*>(&sin), 0xFFFFFF00, macAddr));

	return ifaces;
}

HostPtr NetworkTestFixture::makeHost(const char *ip, const char *mac, const char *hostname)
{
	return std::make_shared<Host>("", hostname, IpAddress(ip), MacAddress(mac));
}

void NetworkTestFixture::onHostDetected(size_t /*xHost*/)
{
}

void NetworkTestFixture::onHostChanged(size_t /*xHost*/, HostField /*field*/)
{
}

void NetworkTestFixture::onIsIdleChanged(bool isIdle)
{
}

// Test basic construction
TEST_F(NetworkTestFixture, ConstructionThrowsOnEmptyParams)
{
	EXPECT_THROW(Network net{{}}, std::bad_function_call);
}

// Test Network::init with mock hosts
TEST_F(NetworkTestFixture, InitWithHosts)
{
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	auto net = makeNetworkWithHosts(snanerFactory);
	EXPECT_EQ(net.hostCount(), 3);
}

// Test Network::getHosts returns correct hosts vector
TEST_F(NetworkTestFixture, GetHostsReturnsCorrectVector)
{
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	Network net{std::move(makeNetworkParams(snanerFactory))};
	std::vector<HostPtr> hosts;
	hosts.push_back(makeHost("192.168.1.1", "01:23:45:67:89:AB"));
	hosts.push_back(makeHost("192.168.1.2", "AA:78:45:67:89:AB"));
	hosts.push_back(makeHost("192.168.1.254", "AA:BB:CC:DD:EE:FF"));
	net.init(std::vector(hosts));
	const auto& result = net.getHosts();

	EXPECT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], hosts[0]);
	EXPECT_EQ(result[1], hosts[1]);
	EXPECT_EQ(result[2], hosts[2]);
}

using namespace testing;


// Test Network::wakeUp with target indices
TEST_F(NetworkTestFixture, WakeUpTargetsThrowsLogicError)
{
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};
	auto net = makeNetworkWithHosts(snanerFactory);
	std::vector<int> targets = {0, 2};
	EXPECT_THROW(net.wakeUp(std::move(targets)), std::logic_error);
}

// Test Network::wakeUp with target indices and check MockIKnockerUpper expectations
TEST_F(NetworkTestFixture, WakeUpTargetsCallsKnockerUpper)
{
	auto interfaceProvider = std::make_unique<MockIInterfaceProvider>();
	auto ifaces = makeIfaces();
	EXPECT_CALL(*interfaceProvider, getInterfaces())
		.WillOnce(testing::Return(std::move(ifaces)));

	auto knockerUpper = std::make_unique<MockIKnockerUpper>();
	EXPECT_CALL(*knockerUpper, wakeUp(Matcher<const Subnet&>(_), _));

	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	//auto cbHostDetected = [](const size_t x) { onHostDetected(x); };
	auto cbHostChanged = [](const size_t x, HostField y) { onHostChanged(x, y); };
	//auto cbScanFinished = [](const bool isIdle) { onIsIdleChanged(isIdle); };

	auto net = makeNetworkWithHosts(snanerFactory, std::move(interfaceProvider),
		std::move(knockerUpper), nullptr, cbHostChanged);

	std::vector<int> targets = {0, 2};
	EXPECT_NO_THROW(net.wakeUp(std::move(targets)));
}

// Test Network::wakeUp(const args_t&) calls knockerUpper and returns its result
TEST_F(NetworkTestFixture, WakeUpWithMacs_CallsKnockerUpperAndReturnsTrue) {
    auto knockerUpper = std::make_unique<MockIKnockerUpper>();
    EXPECT_CALL(*knockerUpper, wakeUp(Matcher<subnets_t&&>(_), _))
        .WillOnce(testing::Return(true));

	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	auto net = makeNetworkWithHosts(snanerFactory, nullptr, std::move(knockerUpper));
    const args_t macs = {"00:11:22:33:44:55", "AA:BB:CC:DD:EE:FF"};
    const bool result = net.wakeUp(macs);
    EXPECT_TRUE(result);
}

// Test operator[] and at
TEST_F(NetworkTestFixture, OperatorIndexAndAt) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	const auto net = makeNetworkWithHosts(snanerFactory);
    EXPECT_EQ(net[0], net.at(0));
    EXPECT_EQ(net[1], net.at(1));
}

// Test isEmpty
TEST_F(NetworkTestFixture, IsEmpty) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	const auto net = makeNetworkWithHosts(snanerFactory);
    EXPECT_FALSE(net.isEmpty());
}

// Test isIdle
TEST_F(NetworkTestFixture, IsIdle) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, isIdle()).WillOnce(testing::Return(true));
		return scanner;
	};

	auto scanner = std::make_unique<MockIScanner>();

	auto net = makeNetworkWithHosts(snanerFactory,
		nullptr, nullptr);

    EXPECT_TRUE(net.isIdle());
}

// Test addHost
TEST_F(NetworkTestFixture, AddHost) {
	auto interfaceProvider = std::make_unique<MockIInterfaceProvider>();
	EXPECT_CALL(*interfaceProvider, getInterfaces())
		.WillOnce(testing::Return(makeIfaces()));

	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, refreshAsync(_, _));
		return scanner;
	};

	auto net = makeNetworkWithHosts(std::move(snanerFactory),
		std::move(interfaceProvider));

	std::vector<HostPtr> hosts;
    net.init(std::move(hosts));
    const bool added = net.addHost("TestHost", "test.local",
    	"10.0.0.1", "00:11:22:33:44:55");
    EXPECT_TRUE(added);
    EXPECT_EQ(net.hostCount(), 1);
}

// Test removeHosts
TEST_F(NetworkTestFixture, RemoveHosts) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		return makeScanner(sp);
	};

	auto net = makeNetworkWithHosts(snanerFactory);
    const size_t before = net.hostCount();
    net.removeHosts(0, 1);
    EXPECT_EQ(net.hostCount(), before - 1);
}

// Test scanAsync
TEST_F(NetworkTestFixture, ScanAsyncNoThrow) {
	auto interfaceProvider = std::make_unique<MockIInterfaceProvider>();
	EXPECT_CALL(*interfaceProvider, getInterfaces())
		.WillOnce(testing::Return(makeIfaces()));

	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, scanAsync(_));
		return scanner;
	};

	auto net = makeNetworkWithHosts(snanerFactory,
		std::move(interfaceProvider));

	EXPECT_NO_THROW(net.scanAsync());
}

// Test refreshAsync
TEST_F(NetworkTestFixture, RefreshAsyncNoThrow) {
	auto interfaceProvider = std::make_unique<MockIInterfaceProvider>();
	EXPECT_CALL(*interfaceProvider, getInterfaces())
		.WillOnce(testing::Return(makeIfaces()));

	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, refreshAsync(_, _));
		return scanner;
	};

	auto net = makeNetworkWithHosts(snanerFactory,
		std::move(interfaceProvider));

	EXPECT_NO_THROW(net.refreshAsync());
}

// Test finalizeAsyncOp
TEST_F(NetworkTestFixture, FinalizeAsyncOpNoThrow) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, finalizeAsyncOp(_));
		return scanner;
	};

	auto net = makeNetworkWithHosts(snanerFactory);

	EXPECT_NO_THROW(net.finalizeAsyncOp(true));
}

// Test cancel
TEST_F(NetworkTestFixture, CancelNoThrow) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, cancel());
		return scanner;
	};

	auto net = makeNetworkWithHosts(snanerFactory);

	EXPECT_NO_THROW(net.cancel());
}

// Test wait
TEST_F(NetworkTestFixture, WaitNoThrow) {
	auto snanerFactory = [this](const ScannerParams& sp) -> ScannerPtr {
		auto scanner = makeScanner(sp);
		EXPECT_CALL(*scanner, wait());
		return scanner;
	};

	auto net = makeNetworkWithHosts(snanerFactory);

	EXPECT_NO_THROW(net.wait());
}
