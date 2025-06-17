#include "Vault.h"
#include "VaultOpResult.h"

namespace Wpp {

VaultPtr IVault::create(const VaultParams& params)
{
	return std::make_unique<Vault>(params);
}

Vault::Vault(const VaultParams& params)
{
}

void Vault::init(hosts_t&& hosts)
{
	std::swap(this->hosts, hosts);
}

HostPtr Vault::operator[](size_t x) const
{
	return at(x);
}

HostPtr Vault::at(const size_t x) const
{
	std::lock_guard<std::mutex> lg(mtx);

	if (hosts.size() <= x)
		throw std::out_of_range("Requested host #" + std::to_string(x)
			+ " exceeds host count (" + std::to_string(hosts.size()) + ")");

	return hosts[x];
}

size_t Vault::hostCount() const
{
	std::lock_guard<std::mutex> lg(mtx);
	return hosts.size();
}

bool Vault::isEmpty() const
{
	return hostCount() == 0;
}

VaultOpResult Vault::tryAdd(Host&& newHost)
{
	VaultOpResult res;

	std::lock_guard<std::mutex> lg(mtx);

	if (!newHost.isEmpty()) {
		const auto itHost = std::find_if(hosts.begin(), hosts.end(),
			[&newHost](const HostPtr& knownHost) {
				return newHost.MacAddress == knownHost->MacAddress
					|| newHost.IpAddress == knownHost->IpAddress;
		});

		if (itHost == hosts.end()) {
			res.xHost = hosts.size();
			hosts.emplace_back(std::make_unique<Host>(std::move(newHost)));
			res.isAdded = true;
		}
		else {
			res.xHost = itHost - hosts.begin();
			auto& knownHost = *itHost;
			knownHost->LastSeenOnline = newHost.LastSeenOnline;

			if (knownHost->IpAddress != newHost.IpAddress) {
				knownHost->IpAddress = newHost.IpAddress;
				res.changedFiled = HostField::IpAddr;
			}
			else if (knownHost->MacAddress != newHost.MacAddress) {
				knownHost->MacAddress = newHost.MacAddress;
				res.changedFiled = HostField::MacAddr;
			}
		}
	}

	return res;
}

VaultOpResult Vault::tryUpdateL7(const L7Result& l7Res)
{
	VaultOpResult res;

	std::lock_guard<std::mutex> lg(mtx);

	const auto itHost = std::find_if(hosts.begin(), hosts.end(),
		[&l7Res](const HostPtr& knownHost) {
			return l7Res.IpAddr == knownHost->IpAddress
				|| l7Res.Hostname == knownHost->Hostname;
	});

	if (itHost == hosts.end()) {
		res.xHost = hosts.size();
		hosts.emplace_back(std::make_shared<Host>(Host{std::string(),
			l7Res.Hostname, l7Res.IpAddr, MacAddress(),
			std::time(nullptr)}));
		res.isAdded = true;
	}
	else {
		res.xHost = itHost - hosts.begin();
		auto& knownHost = *itHost;
		knownHost->LastSeenOnline = std::time(nullptr);

		switch (l7Res.sourceOfTruth) {
		case HostField::Hostname:
		{
			if (knownHost->IpAddress != l7Res.IpAddr) {
				knownHost->IpAddress = l7Res.IpAddr;
				res.changedFiled = HostField::IpAddr;
			}
			break;
		}
		case HostField::IpAddr:
		{
			if (knownHost->Hostname != l7Res.Hostname) {
				knownHost->Hostname = l7Res.Hostname;
				res.changedFiled = HostField::Hostname;
			}
			break;
		}
		default:
			throw std::invalid_argument('"' + std::to_string(static_cast<int>(l7Res.sourceOfTruth))
				+ "\" is invalid source of thruth in " + std::string(__func__));
			break;
		}
	}

	return res;
}

const std::vector<HostPtr> &Vault::getHosts() const
{
	return hosts;
}

void Vault::removeHosts(const size_t xHost, const size_t count)
{
	std::lock_guard<std::mutex> lg(mtx);

	if (xHost + count > hosts.size()) {
		throw std::out_of_range("Requested host count #" + std::to_string(xHost + count)
			+ " exceeds existing host count (" + std::to_string(hosts.size()) + ")");
	}

	hosts.erase(hosts.begin() + xHost, hosts.begin() + xHost + count);
}

} // namespace Wpp
