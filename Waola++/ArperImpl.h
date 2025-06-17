#ifndef ARPERIMPL_H
#define ARPERIMPL_H

#include <Host.h>

#include "SubnetGenerator.h"
#include "ArpResult.h"

namespace Wpp {

using FArpCb = std::function<void(const ArpResult&)>;
using ArperImplPtr = std::unique_ptr<class ArperImpl>;

typedef std::unique_ptr<ArperImpl> ArperImplPtr;

class ArperImpl
{
public:
	static ArperImplPtr create(FArpCb cb, SubnetGenerator&& subnetGen);

	ArperImpl(FArpCb cb, SubnetGenerator&& subnetGen);

	virtual void scanAsync() = 0;
	virtual void refreshAsync(hosts_t&& hosts) = 0;
	virtual void wait();
    void cancel();

    virtual ~ArperImpl();

protected:
	void startAsyncOp();

	FArpCb cb;
	SubnetGenerator subnetGen;

	std::atomic_bool reqFinished;
	std::thread threadReq;
	std::exception_ptr exPtr;
	std::atomic_bool cancelled;
};

static constexpr auto RequestTimeout = std::chrono::milliseconds(16);

} // namespace Wpp

#endif // ARPERIMPL_H
