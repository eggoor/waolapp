#include "KnockerUpper.h"

namespace Wpp {
	static WSADATA wsaData;

	void KnockerUpper::platformStartUp() {
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			throw std::runtime_error("WSAStartup failed");
		}
	}

	void KnockerUpper::platformShutDown()
	{
		WSACleanup();
	}

    sockaddr_in KnockerUpper::makeSin(uint addr, ushort port)  
    {  
        in_addr address;  
        address.s_addr = htonl(addr);

        return {
			AF_INET,		// sin_family
			htons(port),	// sin_port
			address,		// sin_addr
			{0}				// sin_zero
        };  
    }

	int KnockerUpper::getSocketError() {
		return WSAGetLastError();
	}
} // namespace Wpp
