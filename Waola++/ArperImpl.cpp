#include "ArperImpl.h"

namespace Wpp {


	ArperImpl::ArperImpl(FArpCb cb, SubnetGenerator&& subnetGen)
		: cb(std::move(cb))
		, subnetGen(std::move(subnetGen))
	{
	}

	ArperImpl::~ArperImpl()
	{
		cancel();
		ArperImpl::wait();
	}

	void ArperImpl::startAsyncOp()
	{
		exPtr = nullptr;
		reqFinished = false;
	}

	void ArperImpl::wait()
	{
		if (threadReq.joinable()) {
			threadReq.join();
			
			if (exPtr) {
				std::rethrow_exception(exPtr);
			}
		}
	}

	void ArperImpl::cancel()
	{
		cancelled = true;
	}
} // namespace Wpp
