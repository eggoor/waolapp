#include "L7Result.h"

#include "Scanner.h"

namespace Wpp {
	ScannerPtr IScanner::create(const ScannerParams& params)
	{
		return std::make_unique<Scanner>(params);
	}

	Scanner::Scanner(const ScannerParams& params)
		: arper([this](const ArpResult& res) { onArpResponse(res); })
		, hostDetectedCb(params.cbHostDetected)
		, cbL7Resolved(params.cbL7Resolved)
		, scanStateChangedCb(params.cbScanStateChanged)
	{
	}

	Scanner::~Scanner()
	{
		arper.cancel();
		doWatch();
		Scanner::finalizeAsyncOp(false);
	}

	void Scanner::scanAsync(subnets_t&& subNets)
	{
		prepareAsyncOrThrow();

		if (!cancelled) {
			workerThread = std::thread(&Scanner::doScan, this, std::move(subNets));
		}
	}

	void Scanner::refreshAsync(subnets_t&& subNets, hosts_t&& hosts)
	{
		prepareAsyncOrThrow();

		if (!cancelled) {
			workerThread = std::thread(&Scanner::doRefresh,
				this, std::move(subNets), std::move(hosts));
		}
	}

	void Scanner::doScan(subnets_t&& subNets)
	{
		try {
			for (auto subNet : subNets) {
				if (cancelled) {
					break;
				}
				arper.resolveAsync(std::move(subNet));
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}

		doWatch();
	}

	void Scanner::doRefresh(subnets_t&& subNets, hosts_t&& hosts)
	{
		try {
			for (auto subNet : subNets) {
				if (cancelled) {
					break;
				}

				auto subnetHosts = subNet.extractSubnetHosts(hosts);

				if (!subnetHosts.empty()) {
					arper.resolveAsync(std::move(subNet), std::move(subnetHosts));
				}
			}

			for (const auto& host : hosts) {
				if (!host->Hostname.empty()) {
					resolveIpAddrAsync(host->Hostname);
				}
				else if (!host->IpAddress.isZero()) {
					resolveHostnameAsync(host->IpAddress);
				}
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}

		doWatch();
	}

	void Scanner::doWatch()
	{
		try {
			arper.wait();
		}
		catch (...) {
			exPtr = std::current_exception();
		}

		for (auto& thread : l7Resolvers) {
			if (thread.joinable()) {
				thread.join();
			}
		}

		l7Resolvers.clear();

		if (!idle) {
			idle = true;
			scanStateChangedCb(true);
		}
	}

	void Scanner::finalizeAsyncOp(const bool rethrowException)
	{
		if (!idle) {
			cancel();
			Scanner::wait();
			idle = true;
			throw std::logic_error(std::string(__func__)
				+ " unexpected: scanner is in progress");
		}

		Scanner::wait();

		if (rethrowException && exPtr) {
			const auto exPtrCopy = exPtr;
			exPtr = nullptr;
			std::rethrow_exception(exPtrCopy);
		}
	}

	void Scanner::cancel()
	{
		cancelled = true;
		arper.cancel();
	}

	bool Scanner::isIdle() const
	{
		return idle;
	}

	void Scanner::wait()
	{
		if (workerThread.joinable()) {
			workerThread.join();
		}
	}

	void Scanner::prepareAsyncOrThrow()
	{
		if (!idle) {
			throw std::logic_error(std::string(__func__)
				+ " unexpected: previous async operation is in progress");
		}

		cancelled = false;
		idle = false;

		scanStateChangedCb(false);

	}

	void Scanner::onArpResponse(const ArpResult& res)
	{
		if (cancelled) {
			return;
		}

		resolveHostnameAsync(res.IpAddr);

		hostDetectedCb(res);
	}

	void Scanner::resolveHostnameAsync(const IpAddress& ipAddr)
	{
		std::lock_guard<std::mutex> lg(l7ResolversLock);
		l7Resolvers.emplace_back([this, ipAddr] { doResolveHostname(ipAddr); });
	}

	void Scanner::resolveIpAddrAsync(const std::string& hostname)
	{
		std::lock_guard<std::mutex> lg(l7ResolversLock);
		l7Resolvers.emplace_back([this, hostname] { doResolveIpAddr(hostname); });
	}

	void Scanner::doResolveHostname(const IpAddress& ipAddr) const
	{
		char buf[NI_MAXHOST];
		sockaddr_in sin{};
		sin.sin_family = AF_INET;
		*reinterpret_cast<uint*>(&sin.sin_addr) = ipAddr.toNetworkByteOrder();

		const int rc = getnameinfo(reinterpret_cast<const struct sockaddr*>(&sin),
			sizeof(struct sockaddr), buf, NI_MAXHOST, nullptr, 0, NI_NAMEREQD);

		if (rc == 0) {
			cbL7Resolved(L7Result(HostField::IpAddr, ipAddr, buf));
		}
	}

	void Scanner::doResolveIpAddr(const std::string& hostname) const
	{
		addrinfo* addrInfo = nullptr;
		const auto rc = getaddrinfo(hostname.c_str(), nullptr, nullptr, &addrInfo);

		if (rc == 0) {
			for (addrinfo* p = addrInfo; p; p = p->ai_next) {
				if (p->ai_addr && AF_INET == p->ai_addr->sa_family) {
					const auto nbo = reinterpret_cast<const sockaddr_in*>(p->ai_addr)->sin_addr.s_addr;
					cbL7Resolved(L7Result(HostField::Hostname, IpAddress(ntohl(nbo)), hostname));
					break;
				}
			}
		}

		freeaddrinfo(addrInfo);
	}
} // namespace Wpp
