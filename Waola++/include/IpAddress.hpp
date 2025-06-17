#ifndef IPADDRESS_HPP
#define IPADDRESS_HPP

namespace Wpp {

    constexpr_ IpAddress::IpAddress(const uint addr)
        : addr(addr)
#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
		, strAddr(toString())
#endif
    {
    }

    inline uint IpAddress::toNetworkByteOrder() const
    {
        return htonl(addr);
    }

    constexpr_ IpAddress::operator uint() const
    {
        return addr;
    }

    constexpr_ IpAddress IpAddress::operator &(const uint n) const
    {
        return IpAddress(addr & n);
    }

	constexpr_ IpAddress IpAddress::operator |(const uint n) const
	{
		return IpAddress(addr | n);
	}

	constexpr_ IpAddress IpAddress::operator +(const uint n) const
	{
		return IpAddress(addr + n);
	}

	constexpr_ IpAddress& IpAddress::operator++()
	{
		++addr;

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
        strAddr = toString();
#endif

		return *this;
	}

	constexpr_ bool IpAddress::isZero() const
    {
        return addr == 0;
    }

#if __cplusplus < 202002L
    bool IpAddress::operator==(const IpAddress& addr) const
    {
		return this->addr == addr.addr;
    }

    bool IpAddress::operator!=(const IpAddress& addr) const
	{
		return !(*this == addr);
	}
#endif

} // namespace Wpp

template <>
struct std::hash<Wpp::IpAddress> {
    std::size_t operator()(const Wpp::IpAddress& obj) const noexcept
    {
        return std::hash<uint>{}(static_cast<uint>(obj));
    }
};

#endif //IPADDRESS_HPP
