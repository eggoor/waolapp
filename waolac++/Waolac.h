#ifndef WAOLAC_H
#define WAOLAC_H

#include <INetwork.h>

using namespace Wpp;

class Waolac
{
public:
	Waolac(args_t&& args, Wpp::NetworkPtr network = nullptr);

	int Run();

private:
	void onHostDetected(const size_t& xHost);

	void onHostChanged(const size_t& xHost, HostField field);

	void onIsIdleChanged(bool isIdle);

	static bool isReady(const HostPtr &host);

	static void printHost(const HostPtr &host);

private:
	int wakeUp() const;
	int scan();

private:
	args_t args;
	Wpp::NetworkPtr network;
	using hosts_t = std::map<size_t, Wpp::HostPtr>;
	std::mutex mtxHosts;
	hosts_t hosts;
	std::mutex mtxCv;
	std::condition_variable cv;
	std::atomic_bool scanFinished;
};

#endif // WAOLAC_H
