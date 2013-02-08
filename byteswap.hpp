#ifndef BYTESWAP_HPP
#define BYTESWAP_HPP

#include <type_traits>
#include <limits>

#include <climits>
#include <cstdlib>
#include <cmath>
#include <cstdint>

/**
 * Convert a non-boolean, integral value from network to host byte order, or
 * from host to network byte order.
 */
template <typename T>
T byteswap (const T value) {
    static_assert(!std::is_same<bool, T>::value,
            "byteswap called on bool");
    static_assert(std::is_integral<T>::value,
            "byteswap called on non-integral type");

    using unsigned_T = typename std::make_unsigned<T>::type;

    T result = 0;
    auto& u_result = reinterpret_cast<unsigned_T&>(result);
    auto uc_value = reinterpret_cast<const unsigned char *>(&value);

    for (std::size_t i = 0; i < sizeof(T); i++) {
        u_result <<= CHAR_BIT;
        u_result += uc_value[i] & UCHAR_MAX;
    }

    return result;
}

uint64_t canonize_fp (const double value) {
    const int bias = 1023;
    const int bits = 64;
    const int expbits = 11;

    int precision = std::numeric_limits<double>::digits;

    int iexp;
    uint64_t sign = std::signbit(value);
    uint64_t significand = std::ldexp(std::frexp(std::abs(value), &iexp), precision);
    uint64_t exponent = iexp + bias - 1;
    significand &= 0x000fffffffffffffLL;

    return sign << (bits - 1)
            | exponent << (bits - expbits - 1)
            | significand;
}

uint64_t byteswap (const double value) {
    return byteswap(canonize_fp(value));
}

#endif
