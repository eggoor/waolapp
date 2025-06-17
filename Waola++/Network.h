#ifndef NETWORK_H
#define NETWORK_H

#include <INetwork.h>
#include <IScanner.h>

#include "KnockerUpper.h"

namespace Wpp {

class Network final : public INetwork
{
public:
    explicit Network(NetworkParams&& params);

    // INetwork implementation
public:
    void init(std::vector<Wpp::HostPtr>&& hosts) override;
    [[nodiscard]] size_t hostCount() const override;
	HostPtr operator[](size_t x) const override;
	[[nodiscard]] HostPtr at(size_t x) const override;
	[[nodiscard]] const std::vector<HostPtr>& getHosts() const override;
	void wakeUp(std::vector<int>&& xTargets) override;
	bool wakeUp(const args_t &macs) override;
	void scanAsync() override;
	void refreshAsync() override;
	[[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] bool isIdle() const override;
    void finalizeAsyncOp(bool rethrowException) override;
    void cancel() override;
    void wait() override;
	bool addHost(std::string&& displayName, std::string&& hostname,
		const std::string& ipAddress, const std::string& macAddress) override;
	void removeHosts(size_t xHost, size_t count) override;

private:
    void onHostDetected(const ArpResult& res) const;
    void onHostnameResolved(const L7Result& res) const;
    void processVaultResult(const VaultOpResult& res) const;

	subnets_t collectSubnets() const;

private:
	InterfaceProviderPtr interfaceProvider;
    ScannerPtr scanner;
    VaultPtr vault;
	KnockerUpperPtr knockerUpper;
    FHostDetectedCb hostDetectedCb;
    FHostChangedCb hostChangedCb;
};

} // namespace Wpp

#endif // NETWORK_H
