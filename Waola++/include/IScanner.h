#ifndef ISCANNER_H
#define ISCANNER_H

// #include <unordered_set>

#include "ScannerParams.h"
#include "Host.h"

namespace Wpp {

using ScannerPtr = std::unique_ptr<class IScanner>;
using subnets_t = std::unordered_set<class SubnetGenerator>;

typedef std::function<ScannerPtr(const ScannerParams&)> FScannerFactory;

class IScanner {
public:
    static ScannerPtr create(const ScannerParams& params);

    virtual void scanAsync(subnets_t&& subNets) = 0;
	virtual void refreshAsync(subnets_t&& subNets, hosts_t&& hosts) = 0;
	virtual void finalizeAsyncOp(bool rethrow) = 0;
	virtual void cancel() = 0;
    [[nodiscard]] virtual bool isIdle() const = 0;
    virtual void wait() = 0;

    virtual ~IScanner() = default;
};

} // namespace Wpp

#endif // ISCANNER_H
