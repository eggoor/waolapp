#include "KnockerUpper.h"

namespace Wpp {

sockaddr_in KnockerUpper::makeSin(uint addr, ushort port)
{
	return {
        sizeof(sockaddr_in),			// sin_len
        AF_INET,						// sin_family
        htons(port),					// sin_port
		in_addr{htonl(addr)},	// sin_addr
		{0}							// sin_zero
	};
}

} // namespace Wpp
