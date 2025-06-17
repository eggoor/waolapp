#include "MacAddress.h"

namespace Wpp {

bool isMacAddrDelimeter(const unsigned char ch);
bool isHexDigitChar(unsigned char ch);
unsigned char makeByte(unsigned char hi, unsigned char lo);

MacAddress::MacAddress(const std::string& macAddr)
{
	const char* str = macAddr.c_str();

	int bc = 0;

	while (*str && bc < ETHER_ADDR_LEN) {
		if (!isHexDigitChar(*str) || !isHexDigitChar(*(str + 1))) {
			break;
		}
		addr[bc++] = makeByte(*str, *(str + 1));
		str += 2;
		while (*str && isMacAddrDelimeter(*str)){
			++str;
		}
	}

	if (ETHER_ADDR_LEN != bc) {
		memset(addr, 0, ETHER_ADDR_LEN);
	}
}

std::string MacAddress::toString() const
{
	if (isZero()) {
		return {};
	}

	std::stringstream ss;
	ss.fill('0');

	for (int i = 0; i < ETHER_ADDR_LEN; ++i) {
		ss << std::hex << std::uppercase
		   << std::setw(2) << static_cast<int>(addr[i]);

		if (i < ETHER_ADDR_LEN - 1)
			ss << ':';
	}

	return ss.str();
}

#define arrsize(a) (sizeof(a)/sizeof(*a))

bool isMacAddrDelimeter(const unsigned char ch)
{
	static const unsigned char macAddressDelimeters[] = "-:";

	for (size_t i = arrsize(macAddressDelimeters); i --> 0;) {
		if (ch == macAddressDelimeters[i]) {
			return true;
		}
	}

	return false;
}

static const int hextable[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1, 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

bool isHexDigitChar(unsigned char ch)
{
	return hextable[ch] != -1;
}

unsigned char makeByte(unsigned char hi, unsigned char lo)
{
	return (hextable[hi] << 4) | hextable[lo];
}
} // namespace Wpp
