#include "IpAddress.h"

namespace Wpp {
	IpAddress::IpAddress(const std::string& ipAddress)
	{
		in_addr in_addr;
		if (inet_pton(AF_INET, ipAddress.c_str(), &in_addr) != 1) {
			addr = 0;
		}
		else {
			addr = ntohl(in_addr.s_addr);

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
			strAddr = ipAddress;
#endif
		}
	}

	std::string IpAddress::toString() const
	{
		if (isZero()) {
			return {};
		}

		in_addr inAddr{};
		inAddr.s_addr = toNetworkByteOrder();

		char buf[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &inAddr, buf, INET_ADDRSTRLEN)) {
			return buf;
		}

		return {};
	}
} // namespace Wpp
