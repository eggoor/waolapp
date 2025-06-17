#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

namespace Wpp {

#ifdef constexpr_
#undef constexpr_
#endif

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
	#define constexpr_ inline
#else
	#define constexpr_ constexpr
#endif

class IpAddress {
public:
	explicit constexpr_ IpAddress(uint addr);
	explicit IpAddress(const std::string& ipAddress);
	[[nodiscard]] std::string toString() const;
	[[nodiscard]] uint toNetworkByteOrder() const;
	explicit constexpr_ operator uint() const;
	constexpr_ IpAddress operator &(uint n) const;
	constexpr_ IpAddress operator |(uint n) const;
	constexpr_ IpAddress operator +(uint n) const;
	constexpr_ IpAddress& operator++();
	[[nodiscard]] constexpr_ bool isZero() const;

#if __cplusplus >= 202002L
	auto operator<=>(const IpAddress& addr) const = default;
#else
	constexpr_ bool operator==(const IpAddress& addr) const;
	constexpr_ bool operator!=(const IpAddress& addr) const;
#endif

private:
	uint addr;

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
	std::string strAddr;
#endif
};

} // namespace Wpp

#include "IpAddress.hpp"

#endif // IP_ADDRESS_H
