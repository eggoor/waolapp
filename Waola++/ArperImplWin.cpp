#include "ArperImplWin.h"

namespace Wpp {

	ArperImplPtr ArperImpl::create(FArpCb cb, SubnetGenerator&& subnetGen)
	{
		return std::make_unique<ArperImplWin>(cb, std::move(subnetGen));
	}

	ArperImplWin::ArperImplWin(FArpCb cb, SubnetGenerator&& subnetGen)
		: ArperImpl(std::move(cb), std::move(subnetGen))
	{
	}

	ArperImplWin::~ArperImplWin()
	{
		cancel();
		ArperImplWin::wait();
	}

	void ArperImplWin::scanAsync()
	{
		startAsyncOp();
		threadReq = std::thread(&ArperImplWin::doScan, this);
	}

	void ArperImplWin::refreshAsync(hosts_t&& hosts)
	{
		startAsyncOp();
		threadReq = std::thread(&ArperImplWin::doRefresh, this, std::move(hosts));
	}

	void ArperImplWin::doScan()
	{
		try {
			for (auto& target : subnetGen) {
				if (target == subnetGen.SelfIp) {
					continue;
				}

				resolve(target);
				std::this_thread::sleep_for(RequestTimeout);

				if (cancelled) {
					break;
				}
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}
	}

	void ArperImplWin::doRefresh(hosts_t&& hosts)
	{
		for (auto& targetHost : hosts) {
			resolve(targetHost->IpAddress);
			std::this_thread::sleep_for(RequestTimeout);

			if (cancelled) {
				break;
			}
		}
	}

	void ArperImplWin::resolve(IpAddress ipAddr)
	{
		hostArpers.emplace_back(std::thread([this, ipAddr] {
			byte macAddr[ETHER_ADDR_LEN] = {};
			ULONG phyAddrLen = ETHER_ADDR_LEN;
			auto rc = SendARP(htonl(static_cast<uint>(ipAddr)), htonl(static_cast<uint>(subnetGen.SelfIp)),
				macAddr, &phyAddrLen);

			if (rc == NO_ERROR) {
				cb(ArpResult{ ipAddr, MacAddress(macAddr) });
			}
		}));
	}

	void ArperImplWin::wait()
	{
		ArperImpl::wait();

		for (auto& thread : hostArpers) {
			if (thread.joinable()) {
				thread.join();
			}
		}

		hostArpers.clear();
	}
} // namespace Wpp
