#ifndef WAOLA_TESTS_NETWORKTESTFIXTURE_H
#define WAOLA_TESTS_NETWORKTESTFIXTURE_H

#include <ENetworkModel.h>

class NetworkModelTestFixture : public ::testing::Test {
protected:

	void SetUp() override;

	static void makeModelWithHosts();

	static Wpp::HostPtr makeHost(const char *ip,
		const char *mac, const char *hostname = nullptr);

protected:
	std::unique_ptr<MockINetwork> net;
	std::unique_ptr<MockIPersistor> persistor;
};

#endif // WAOLA_TESTS_NETWORKTESTFIXTURE_H
