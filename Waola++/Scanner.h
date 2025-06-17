#ifndef SCANNER_H
#define SCANNER_H

#include <IScanner.h>

#include "ArpResult.h"
#include "Arper.h"

namespace Wpp {
    class Scanner final : public IScanner {
    public:
        explicit Scanner(const ScannerParams& params);
        ~Scanner() override;
		void scanAsync(subnets_t&& subNets) override;
		void refreshAsync(subnets_t&& subNets, hosts_t&& hosts) override;
		void finalizeAsyncOp(bool rethrowException) override;
		void cancel() override;
        bool isIdle() const override;
        void wait() override;

    private:
		void prepareAsyncOrThrow();
		void doScan(subnets_t&& subNets);
		void doRefresh(subnets_t&& subNets, hosts_t&& hosts);
		void doWatch();
        void onArpResponse(const ArpResult& res);
		void resolveHostnameAsync(const IpAddress& ipAddr);
		void resolveIpAddrAsync(const std::string& hostname);
		void doResolveHostname(const IpAddress& ipAddr) const;
		void doResolveIpAddr(const std::string& ipAddr) const;

    private:
        Arper arper;
        FArpResultCb hostDetectedCb;
		FL7ResultCb cbL7Resolved;
		FScanStateChangedCb scanStateChangedCb;
		std::thread workerThread;
        std::exception_ptr exPtr = nullptr;
		std::mutex l7ResolversLock;
		std::vector<std::thread> l7Resolvers;
        std::atomic_bool idle = { true };
		std::atomic_bool cancelled = { false };
	};
} // namespace Wpp

#endif // SCANNER_H
