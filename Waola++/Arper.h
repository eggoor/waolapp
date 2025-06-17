#ifndef ARPER_H
#define ARPER_H

#include <Host.h>
#include "ArperImpl.h"

namespace Wpp {
    class Arper {
    public:
        explicit Arper(FArpCb cb);
		void resolveAsync(SubnetGenerator&& subnetGen);
		void resolveAsync(SubnetGenerator&& subnetGen, hosts_t&& subnetHosts);
		void wait();
        void cancel() const;

    private:
        FArpCb cb;
		std::vector<ArperImplPtr> subnetArpers;
        std::mutex mtx;
    };
} // namespace Wpp

#endif // ARPER_H
