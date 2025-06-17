#ifndef WAOLA_TESTS_NETWORKTESTFIXTURE_H
#define WAOLA_TESTS_NETWORKTESTFIXTURE_H

#include <Network.h>

using namespace Wpp;

class NetworkTestFixture : public ::testing::Test {
protected:
	static NetworkParams makeNetworkParams(FScannerFactory scannerFactory,
		InterfaceProviderPtr interfaceProvider = nullptr,
		KnockerUpperPtr knockerUpper = nullptr,
		FHostDetectedCb cbHostDetected = nullptr,
		FHostChangedCb cbHostChanged = nullptr,
		FScanStateChangedCb cbScanFinished = nullptr);

	static Network makeNetworkWithHosts(FScannerFactory scannerFactory,
		InterfaceProviderPtr interfaceProvider = nullptr,
		KnockerUpperPtr knockerUpper = nullptr,
		FHostDetectedCb cbHostDetected = nullptr,
		FHostChangedCb cbHostChanged = nullptr,
		FScanStateChangedCb cbScanFinished = nullptr);

 	static HostPtr makeHost(const char* ip, const char* mac, const char* hostname = "");
	static network_interfaces_t makeIfaces();
	std::unique_ptr<MockIScanner> makeScanner(const ScannerParams& sp);

	static void onHostDetected(size_t xHost);
	static void onHostChanged(size_t xHost, HostField field);
	static void onIsIdleChanged(bool isIdle);
};

#endif // WAOLA_TESTS_NETWORKTESTFIXTURE_H
