#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include "EtherAddrLen.h"

namespace Wpp {
#ifdef constexpr_
#undef constexpr_
#endif

#if __cplusplus >= 202002L
#define constexpr_ constexpr
#else
#define constexpr_ inline
#endif

    class MacAddress {
    public:
        explicit constexpr MacAddress();
		explicit constexpr MacAddress(const unsigned char* macAddr);
		explicit MacAddress(const std::string& macAddr);
		[[nodiscard]] std::string toString() const;
        [[nodiscard]] constexpr_ bool isZero() const;
        explicit constexpr operator const unsigned char*() const;
        constexpr unsigned char operator[](size_t i) const;
        constexpr_ unsigned char& operator[](size_t i);

#if __cplusplus >= 201703L
        explicit constexpr MacAddress(const byte* macAddr);
        constexpr explicit operator const byte*() const;
#endif

#if __cplusplus >= 202002L
        auto operator<=>(const MacAddress& addr) const = default;
#else
        constexpr_ bool operator==(const MacAddress& addr) const;
        constexpr_ bool operator!=(const MacAddress& addr) const;
#endif

    private:
        unsigned char addr[ETHER_ADDR_LEN];
    };

	static constexpr unsigned char zeroAddr[ETHER_ADDR_LEN] = {'\0'};
} // namespace Wpp

#include "MacAddress.hpp"

#endif // MAC_ADDRESS_H
