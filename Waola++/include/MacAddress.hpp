#ifndef MACADDRESS_HPP
#define MACADDRESS_HPP

namespace Wpp {
    constexpr MacAddress::MacAddress()
        : MacAddress(zeroAddr)
    {
    }

    constexpr MacAddress::MacAddress(const unsigned char* macAddr)
        : addr{macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]}
    {
    }

    constexpr_ bool MacAddress::isZero() const
    {
		return std::equal(addr, addr + ETHER_ADDR_LEN, zeroAddr);
    }

    constexpr MacAddress::operator const unsigned char*() const
    {
        return addr;
    }

    constexpr unsigned char MacAddress::operator[](const size_t i) const
    {
        if (i >= ETHER_ADDR_LEN) {
            throw std::out_of_range("'" + std::to_string(i) + "' exceeds MAC address size");
        }

        return addr[i];
    }

    constexpr_ unsigned char& MacAddress::operator[](const size_t i)
    {
        if (i >= ETHER_ADDR_LEN) {
            throw std::out_of_range("'" + std::to_string(i) + "' exceeds MAC address size");
        }

        return addr[i];
    }

#if __cplusplus >= 201703L
    constexpr MacAddress::MacAddress(const byte* macAddr)
        : MacAddress(static_cast<const unsigned char*>(static_cast<const void*>(macAddr)))
    {
    }
    
    constexpr MacAddress::operator const byte* () const
    {
        return static_cast<const byte*>(static_cast<const void*>(addr));
    }

#endif

#if __cplusplus < 202002L
    constexpr_ bool MacAddress::operator==(const MacAddress& addr) const
    {
        return memcmp(this->addr, addr.addr, ETHER_ADDR_LEN) == 0;
    }

    constexpr_ bool MacAddress::operator!=(const MacAddress& addr) const
    {
        return !(*this == addr);
    }
#endif
} // namespace Wpp

template <>
struct std::hash<Wpp::MacAddress> {
    std::size_t operator()(const Wpp::MacAddress& obj) const noexcept
    {
        std::size_t hash = 0;
        const auto addr = static_cast<const unsigned char*>(obj);

        for (int i = 0; i < ETHER_ADDR_LEN; ++i) {
            hash ^= static_cast<std::size_t>(addr[i]) << (i * 8);
        }

        return hash;
    }
};

#endif //MACADDRESS_HPP
