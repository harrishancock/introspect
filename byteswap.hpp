#ifndef BYTESWAP_HPP
#define BYTESWAP_HPP

#include <type_traits>

#include <climits>

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

    for (size_t i = 0; i < sizeof(T); i++) {
        u_result <<= CHAR_BIT;
        u_result += uc_value[i] & UCHAR_MAX;
    }

    return result;
}

#endif
