#include "ArpResult.h"
#include "SubnetGenerator.h"
#include "NetworkInterface.h"

#include "Network.h"

namespace Wpp {

	NetworkPtr INetwork::create(NetworkParams&& params)
	{
		return std::make_unique<Network>(std::move(params));
	}

	Network::Network(NetworkParams&& params)
		: interfaceProvider(params.interfaceProvider ? std::move(params.interfaceProvider) : IInterfaceProvider::make())
		, scanner(params.scannerFactory(ScannerParams{
			  [this](const ArpResult& res) { onHostDetected(res); }
			  ,[this](const L7Result& res) { onHostnameResolved(res); }
			  , std::move(params.cbScanFinished) }))
		, vault(params.vaultFactory({}))
		, knockerUpper(params.knockerUpper ? std::move(params.knockerUpper) : IKnockerUpper::make())
		, hostDetectedCb(std::move(params.cbHostDetected))
		, hostChangedCb(std::move(params.cbHostChanged))
	{
	}

	void Network::init(hosts_t&& hosts)
	{
		vault->init(std::move(hosts));
	}

	HostPtr Network::at(const size_t x) const
	{
		return vault->at(x);
	}

	HostPtr Network::operator[](const size_t x) const
	{
		return at(x);
	}

	const std::vector<HostPtr>& Network::getHosts() const
	{
		return vault->getHosts();
	}

	void Network::wakeUp(std::vector<int>&& xTargets)
	{
		if (!hostChangedCb) {
			throw std::logic_error(std::string(__func__) + " requires host changed callback");
		}

		const auto subNets = collectSubnets();
		const auto& allHosts = vault->getHosts();
			
#if __cplusplus >= 202302L
		auto targets = xTargets | std::ranges::views::filter([&](const int x) {
			return x >= 0 && x < allHosts.size();
		}) | std::ranges::views::transform([&](const int x) { return allHosts[x]; })
			| std::ranges::to<std::vector>();
#else
		std::vector<HostPtr> targets;
		targets.reserve(xTargets.size());
		for (const int x : xTargets) {
			if (x >= 0 && x < allHosts.size()) {
				targets.push_back(allHosts[x]);
			}
		}
#endif
		for (auto& subNet : subNets) {
			auto subnetHosts = subNet.extractSubnetHosts(targets);
			if (!subnetHosts.empty()) {
				knockerUpper->wakeUp(subNet, subnetHosts);
			}
			}

			for (const auto& host : targets) {
				host->OpResult = orFail;
			}

			for (const auto xHost : xTargets) {
				hostChangedCb(xHost, HostField::OpResult);
			}
	}

	bool Network::wakeUp(const args_t& macs)
	{
		macs_t macAddrs;
		macAddrs.reserve(macs.size());

		std::transform(macs.begin(), macs.end(), std::back_inserter(macAddrs),
			[](const std::string& mac) {
				return MacAddress(mac);
		});

		return knockerUpper->wakeUp(collectSubnets(), macAddrs);
	}

	size_t Network::hostCount() const
	{
		return vault->hostCount();
	}

	void Network::scanAsync()
	{
		scanner->scanAsync(collectSubnets());
	}

	void Network::refreshAsync()
	{
		scanner->refreshAsync(collectSubnets(), hosts_t(vault->getHosts()));
	}

	bool Network::isEmpty() const
	{
		return vault->isEmpty();
	}

	bool Network::isIdle() const
	{
		return scanner->isIdle();
	}

	void Network::finalizeAsyncOp(const bool rethrowException)
	{
		scanner->finalizeAsyncOp(rethrowException);
	}

	void Network::cancel()
	{
		scanner->cancel();
	}

	void Network::wait()
	{
		scanner->wait();
	}

	bool Network::addHost(std::string&& displayName, std::string&& hostname,
		const std::string& ipAddress, const std::string& macAddress)
	{
		auto res = vault->tryAdd(Host{ std::move(displayName), std::move(hostname),
			IpAddress(ipAddress), MacAddress(macAddress)});

		if (res.xHost != -1) {
			scanner->refreshAsync(collectSubnets(), { vault->at(res.xHost) });
		}

		return res.isAdded;
	}

	void Network::removeHosts(const size_t xHost, const size_t count)
	{
		vault->removeHosts(xHost, count);
	}

	void Network::onHostDetected(const ArpResult& res) const
	{
		processVaultResult(vault->tryAdd(Host{ res.IpAddr, res.MacAddr }));
	}

	void Network::onHostnameResolved(const L7Result& res) const
	{
		processVaultResult(vault->tryUpdateL7(res));
	}

	void Network::processVaultResult(const VaultOpResult& res) const
	{
		if (res.isAdded) {
			hostDetectedCb(res.xHost);
		}
		else if (hostChangedCb) {
			if (res.changedFiled != HostField::None) {
				hostChangedCb(res.xHost, res.changedFiled);
			}
			hostChangedCb(res.xHost, HostField::LastSeenOnline);
		}
	}

	subnets_t Network::collectSubnets() const
	{
		subnets_t subNets;

		const auto ifaces = interfaceProvider->getInterfaces();

#if __cplusplus >= 202302L
		for (const auto& val : ifaces | std::views::values) {
			for (const auto& iface : val) {
#else
		for (const auto& pair: ifaces) {
			for (const auto& iface : pair.second) {
#endif
				if (iface->isIpV4() && !iface->isLoopback()) {
					const auto xIface = iface->Index;
					auto ifaceName = iface->DisplayName;
					const auto selfIp = iface->GetIpV4Addr();
					const auto mask = iface->GetIpV4Mask();
					const auto macAddr = iface->MacAddr;

					const SubnetGenerator subnetGen(xIface, std::move(ifaceName),
						IpAddress(selfIp), mask, MacAddress(macAddr));

					if (subNets.find(subnetGen) != subNets.end()) {  
						continue;
					}

					subNets.insert(subnetGen);
				}
			}
		}

		return subNets;
	}
} // namespace Wpp
