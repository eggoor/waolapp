#include "ArpResult.h"

#include "ArperImplX.h"

#include <utility>

namespace Wpp {

	ArperImplPtr ArperImpl::create(FArpCb cb, SubnetGenerator&& subnetGen)
	{
		return std::make_unique<ArperImplX>(cb, std::move(subnetGen));
	}

	ArperImplX::ArperImplX(FArpCb cb, SubnetGenerator&& subnetGen)
		: ArperImpl(cb, std::move(subnetGen))
		, socket(setupSocket(subnetGen))
		, responseBufSize(getResponseBufferSize(subnetGen.IfaceName, socket))
		, request(subnetGen.SelfIp, subnetGen.MacAddr)
	{
	}

	ArperImplX::~ArperImplX()
	{
		cancel();
		ArperImplX::wait();
	}

	void ArperImplX::scanAsync()
	{
		startAsyncOp();
		threadResp = receiveResponsesAsync();
		threadReq = std::thread(&ArperImplX::doScan, this);
	}

	void ArperImplX::refreshAsync(hosts_t&& hosts)
	{
		startAsyncOp();
		threadResp = receiveResponsesAsync();
		threadReq = std::thread(&ArperImplX::doRefresh, this, std::move(hosts));
	}

	void ArperImplX::doScan()
	{
		try {
			for (auto& target : subnetGen) {
				if (target == subnetGen.SelfIp) {
					continue;
				}
				sendRequest(target);
				std::this_thread::sleep_for(RequestTimeout);

				if (cancelled) {
					break;
				}
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}

		reqFinished = true;
	}

	void ArperImplX::doRefresh(hosts_t&& hosts)
	{
		try {
			for (const auto& host : hosts) {
				sendRequest(host->IpAddress);
				std::this_thread::sleep_for(RequestTimeout);

				if (cancelled) {
					break;
				}
			}
		}
		catch (...) {
			exPtr = std::current_exception();
		}

		reqFinished = true;
	}

	void ArperImplX::sendRequest(IpAddress ipAddr)
	{
		request.setTarget(ipAddr);

		ssize_t sentBytes = 0;

		for (int i = 0; i < RequestTryCount; ++i) {
			while (sentBytes < ArpPacketSize) {
				auto addLen = write(socket, static_cast<const unsigned char*>(request) + sentBytes,
					ArpPacketSize - sentBytes);

				if (addLen < 0) {
					break;
				}

				sentBytes += addLen;
			}

			if (ArpPacketSize == sentBytes) {
				break;
			}

			if (ENOBUFS == errno) {
				std::this_thread::sleep_for(std::chrono::milliseconds(NoBuffersTimeoutMs << i));
			}
		}
	}

	std::thread ArperImplX::receiveResponsesAsync()
	{
		return std::thread(&ArperImplX::doReceiveResponses, this);
	}

	void ArperImplX::processResponse(const ether_arp* const etherArp) const
	{
		if (!cancelled) {
			auto ipAddr = ntohl(*(uint*)etherArp->arp_spa);
			cb(ArpResult{ IpAddress(ipAddr), MacAddress(etherArp->arp_sha) });
		}
	}

	void ArperImplX::wait()
	{
		if (threadResp.joinable()) {
			threadResp.join();
		}
	}
} // namespace Wpp
