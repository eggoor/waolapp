#ifndef ARPERIMPLX_H
#define ARPERIMPLX_H

#include "ArperImpl.h"
#include "ArpRequest.h"
#include "SubnetGenerator.h"

namespace Wpp {

class ArperImplX final : public ArperImpl
{
public:
	ArperImplX(FArpCb cb, SubnetGenerator&& subnetGen);
	~ArperImplX() override;

	void scanAsync() override;
	void refreshAsync(hosts_t&& hosts) override;
    void wait() override;

private:
	std::thread receiveResponsesAsync();
	void doScan();
	void doRefresh(hosts_t&& hosts);
	void doReceiveResponses();
	void sendRequest(IpAddress ipAddr);
    void processResponse(const ether_arp* etherArp) const;

    static int setupSocket(const Subnet& subNet);
    static int doSetupSocket(const Subnet& subNet, int& errNo);
    static size_t getResponseBufferSize(const std::string& ifName, int socket);

private:
    int socket;
    size_t responseBufSize;
    ArpRequest request;
	std::thread threadResp;

    static constexpr auto RequestTryCount = 3;
    static constexpr auto NoBuffersTimeoutMs  = 128;
    static constexpr auto ListenTimeoutMs = 1000;
    static constexpr ssize_t ArpPacketSize = sizeof(ether_header) + sizeof(ether_arp);
};

} // namespace Wpp

#endif // ARPERIMPLX_H
