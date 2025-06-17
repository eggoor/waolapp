#ifndef IPADDRITERATOR_H
#define IPADDRITERATOR_H

#include <IpAddress.h>

namespace Wpp {

class IpAddrIterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = IpAddress;
	using const_pointer = const IpAddress*;
	using const_reference = const IpAddress&;

	IpAddrIterator(IpAddress ip)
		: ip(ip)
	{}

	const_reference operator*() const
	{
		return ip;
	}

	const_pointer operator->() const
	{
		return &ip;
	}

	IpAddrIterator& operator++()
	{
		++ip;
		return *this;
	}

	bool operator!=(const IpAddrIterator& that) const
	{
		return this != &that && ip != that.ip;
	}

private:
	IpAddress ip;
};

} // namespace Wpp

#endif // IPADDRITERATOR_H
