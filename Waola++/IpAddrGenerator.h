#ifndef IPADDRGENERATOR_H
#define IPADDRGENERATOR_H

#include "IpAddrIterator.h"

namespace Wpp {

class IpAddrGenerator
{
public:
	using iterator = IpAddrIterator;
	using const_iterator = const iterator;

	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;
};

} // namespace Wpp

#endif // IPADDRGENERATOR_H
