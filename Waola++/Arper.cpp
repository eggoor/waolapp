#include "Arper.h"

namespace Wpp {
    Arper::Arper(FArpCb cb)
        : cb(std::move(cb))
    {
    }

	void Arper::resolveAsync(SubnetGenerator&& subnetGen)
	{
		const std::lock_guard<std::mutex> lg(mtx);
#if __cplusplus >= 201703L
        const auto& arper = subnetArpers.emplace_back(ArperImpl::create(cb, std::move(subnetGen)));
		arper->scanAsync();
#else
        auto arper = ArperImpl::create(cb, std::move(subnetGen));
		arper->scanAsync();
        subnetArpers.push_back(std::move(arper));
#endif
	}

	void Arper::resolveAsync(SubnetGenerator&& subnetGen, hosts_t&& subnetHosts)
	{
        const std::lock_guard<std::mutex> lg(mtx);
#if __cplusplus >= 201703L
		const auto& arper = subnetArpers.emplace_back(ArperImpl::create(cb, std::move(subnetGen)));
        arper->refreshAsync(std::move(subnetHosts));
#else
        auto arper = ArperImpl::create(cb, std::move(subnetGen));
        arper->refreshAsync(std::move(subnetHosts));
        subnetArpers.push_back(std::move(arper));
#endif
	}

	void Arper::wait()
    {
        const std::lock_guard<std::mutex> lg(mtx);
        for (const auto& arper : subnetArpers) {
            arper->wait();
        }

        subnetArpers.clear();
    }

    void Arper::cancel() const
    {
        for (auto& arper : subnetArpers) {
            arper->cancel();
        }
    }
} // namespace Wpp
