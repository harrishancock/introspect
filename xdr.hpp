#ifndef XDR_HPP
#define XDR_HPP

#include <boost/range/adaptor/reversed.hpp>

#include <type_traits>
#include <limits>

#include <climits>
#include <cstdlib>
#include <cmath>
#include <cstdint>

static_assert(8 == CHAR_BIT, "xdr.hpp only supports POSIX environments");

namespace xdr {

template <typename T>
using uchar_array = typename std::conditional<sizeof(T) <= 4,
      std::array<unsigned char, 4>,
      std::array<unsigned char, 8>>::type;

template <typename T>
using enable_if_unsigned = typename std::enable_if<std::is_unsigned<T>::value, uchar_array<T>>::type;

template <typename T>
using enable_if_signed = typename std::enable_if<std::is_signed<T>::value, uchar_array<T>>::type;

//////////////////////////////////////////////////////////////////////////////

template <typename Unsigned>
enable_if_unsigned<Unsigned>
pack (Unsigned value) {
    uchar_array<Unsigned> result;

    /* Serialize into the unsigned char array in big-endian byte order. */
    for (auto& uc : result | boost::adaptors::reversed) {
        uc = value & UCHAR_MAX;
        value >>= CHAR_BIT;
    }

    return result;
}

template <typename Signed>
enable_if_signed<Signed>
pack (Signed value) {
    using unsigned_type = typename std::make_unsigned<Signed>::type;

    /* static_cast to the equivalent unsigned type should, I believe, ensure
     * two's complement. */
    return pack(static_cast<unsigned_type>(value));
}

uchar_array<uint32_t> pack (bool value) {
    return pack(static_cast<uint32_t>(value));
}

template <typename T>
struct pack_traits;

template <>
struct pack_traits<float> {
    static constexpr unsigned int bytes = 4;
    static constexpr unsigned int bits = 32;
    static constexpr unsigned int expbits = 8;
    static constexpr unsigned int bias = 127;
};

template <>
struct pack_traits<double> {
    static constexpr unsigned int bytes = 8;
    static constexpr unsigned int bits = 64;
    static constexpr unsigned int expbits = 11;
    static constexpr unsigned int bias = 1023;
};

template <>
struct pack_traits<long double> {
    static constexpr unsigned int bytes = 16;
    static constexpr unsigned int bits = 128;
    static constexpr unsigned int expbits = ;
    static constexpr unsigned int bias = ;
};

std::array<unsigned char, pack_traits<double>::bytes> pack (double value) {
    static_assert(2 == std::numeric_limits<double>::radix,
            "xdr.hpp only supports base-2 floating point types");

    int precision = std::numeric_limits<double>::digits;

    int iexp;
    uint64_t sign = std::signbit(value);
    uint64_t significand = std::ldexp(std::frexp(std::abs(value), &iexp), precision);
    uint64_t exponent = iexp + pack_traits<double>::bias - 1;
    significand -= 1LL << (precision - 1);

    return pack(sign << (pack_traits<double>::bits - 1)
            | exponent << (pack_traits<double>::bits - pack_traits<double>::expbits - 1)
            | significand);
}

} // namespace xdr

#endif
