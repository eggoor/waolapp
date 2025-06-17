#ifndef ARPERIMPLWINDOWS_H
#define ARPERIMPLWINDOWS_H

#include "ArperImpl.h"
#include "SubnetGenerator.h"

namespace Wpp {

	using namespace std::chrono_literals;

	class ArperImplWin final : public ArperImpl
    {
    public:
		ArperImplWin(FArpCb cb, SubnetGenerator&& subnetGen);
		~ArperImplWin() override;

        void scanAsync() override;
        void refreshAsync(hosts_t&& hosts) override;
        void wait() override;

    private:
        void doScan();
        void doRefresh(hosts_t&& hosts);
        void resolve(IpAddress ipAddr);

    private:
        std::vector<std::thread> hostArpers;

        static constexpr auto RequestTryCount = 3;
        static constexpr auto NoBuffersTimeoutMs = 128;
    };

} // namespace Wpp

#endif // ARPERIMPLWINDOWS_H
