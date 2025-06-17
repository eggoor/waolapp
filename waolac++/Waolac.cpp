#include "Waolac.h"

Waolac::Waolac(args_t&& args, Wpp::NetworkPtr network)
	: network(network ? std::move(network) : Wpp::INetwork::create(NetworkParams {
		nullptr,
		&IScanner::create,
		&IVault::create,
		nullptr,
		[this](const size_t& x) { onHostDetected(x); },
		[this](const size_t& x, const HostField y) { onHostChanged(x, y); },
		[this](const bool isIdle) { onIsIdleChanged(isIdle); }
		}))
	, args(std::move(args))
 {}

int Waolac::Run()
{
	return args.empty() ? scan() : wakeUp();
}

int Waolac::wakeUp() const
{
	return network->wakeUp(args) ? 0 : 3;
}

int Waolac::scan()
{
	int rc = 0;

	try {
		network->scanAsync();

		std::unique_lock<std::mutex> ul(mtxCv);
		cv.wait(ul, [this]() { return scanFinished.load(); });

		network->finalizeAsyncOp();

		for (const auto& pair : hosts) {
			printHost(pair.second);
		}
	}
	catch (const std::system_error& ex) {
		std::cerr << "System error: " << ex.what()
				  << " (code " << ex.code().value() << ")\n";
	}
	catch (const std::exception& ex){
		std::cerr << ex.what() << '\n';
		rc = 1;
	}
	catch (...) {
		std::cerr << "Unknown exception\n";
		rc = 2;
	}

	return rc;
}

void Waolac::onHostDetected(const size_t& xHost)
{
	auto host = network->at(xHost);

	if (isReady(host)) {
		printHost(host);
	}
	else {
		std::lock_guard<std::mutex> lg(mtxHosts);
		hosts.emplace(xHost, std::move(host));
	}
}

void Waolac::onHostChanged(const size_t& xHost, const HostField /*field*/)
{
	std::unique_lock<std::mutex> ul(mtxHosts);

    if (hosts.find(xHost) != hosts.end()) {
		const auto& host = hosts[xHost];
		ul.unlock();

		if (host && isReady(host)) {
			printHost(host);
			ul.lock();
			hosts.erase(xHost);
		}
	}
}

void Waolac::onIsIdleChanged(const bool isIdle)
{
	if (isIdle) {
		scanFinished = true;
		cv.notify_one();
	}
}

bool Waolac::isReady(const HostPtr& host)
{
	return !host->Hostname.empty() && !host->IpAddress.isZero()
		&& !host->MacAddress.isZero();
}

void Waolac::printHost(const HostPtr& host)
{
	std::cout << host->MacAddress.toString() << '\t'
			  << host->IpAddress.toString() << '\t'
			  << host->Hostname << '\n';
}
