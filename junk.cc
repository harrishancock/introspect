#include <typeinfo>
#include <type_traits>
                     
#include <cstdio>
#include <cstdint>

struct nocast {
    using from_bool = char;
    using from_char = char;
    using from_wchar_t = wchar_t;
    using from_char16_t = char16_t;
    using from_char32_t = char32_t;
    using from_signed_char = signed char;
    using from_unsigned_char = unsigned char;
    using from_short_int = short int;
    using from_unsigned_short_int = unsigned short int;
    using from_int = int;
    using from_unsigned_int = unsigned int;
    using from_long_int = long int;
    using from_unsigned_long_int = unsigned long int;
    using from_long_long_int = long long int;
    using from_unsigned_long_long_int = unsigned long long int;
    using from_float = float;
    using from_double = double;
    using from_long_double = long double;
};

template <typename Policy>
struct holder {
#define DEF(T, F) \
    void print (T t) { \
        printf(#T " -> "); \
        if (!std::is_same<typename Policy::F, T>::value) { \
            print(static_cast<typename Policy::F>(t)); \
        } \
        printf("\n"); \
    }

    DEF(bool, from_bool)
    DEF(char, from_char)
    DEF(wchar_t, from_wchar_t)
    DEF(char16_t, from_char16_t)
    DEF(char32_t, from_char32_t)
    DEF(signed char, from_signed_char)
    DEF(unsigned char, from_unsigned_char)
    DEF(short int, from_short_int)
    DEF(unsigned short int, from_unsigned_short_int)
    DEF(int, from_int)
    DEF(unsigned int, from_unsigned_int)
    DEF(long int, from_long_int)
    DEF(unsigned long int, from_unsigned_long_int)
    DEF(long long int, from_long_long_int)
    DEF(unsigned long long int, from_unsigned_long_long_int)
    DEF(float, from_float)
    DEF(double, from_double)
    DEF(long double, from_long_double)
};

int main () {
    holder<nocast> h;

    h.print(true);
    h.print('c');
    h.print(1L);
    h.print(1LL);
}
