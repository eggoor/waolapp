#ifndef I_NETWORK_H
#define I_NETWORK_H

#include "Host.h"
#include "NetworkParams.h"

namespace Wpp {

using NetworkPtr = std::unique_ptr<class INetwork>;
using args_t = std::vector<std::string>;

class INetwork {
public:
    static NetworkPtr create(NetworkParams&& params);

    virtual void init(std::vector<HostPtr>&& hosts) = 0;
	virtual HostPtr operator[](size_t x) const = 0;
	[[nodiscard]] virtual HostPtr at(size_t x) const = 0;
	[[nodiscard]] virtual size_t hostCount() const = 0;
    [[nodiscard]] virtual bool isEmpty() const = 0;
    [[nodiscard]] virtual bool isIdle() const = 0;
    [[nodiscard]] virtual const std::vector<HostPtr>& getHosts() const = 0;
	virtual bool addHost(std::string&& displayName, std::string&& hostname,
		const std::string& ipAddress, const std::string& macAddress) = 0;
	virtual void removeHosts(size_t xHost, size_t count) = 0;

	virtual void wakeUp(std::vector<int>&& hosts) = 0;
	virtual bool wakeUp(const args_t& macs) = 0;
	virtual void scanAsync() = 0;
	virtual void refreshAsync() = 0;
	virtual void finalizeAsyncOp(bool rethrowException = true) = 0;
    virtual void cancel() = 0;
    virtual void wait() = 0;
    virtual ~INetwork() = default;
};

} // namespace Wpp

#endif // I_NETWORK_H
