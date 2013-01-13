#include "as_tuple.hpp"

#include <cstdio>

struct foo {
    int i;
    double d;
};

AS_TUPLE(foo, (i)(d));

//////////////////////////////////////////////////////////////////////////////

constexpr foo f { 666, 3.14 };

int main () {
    //auto t = as_tuple(foo{345, 2.5345});
    auto t = as_tuple(f);

    printf("%d\n", std::get<0>(t));
    printf("%f\n", std::get<1>(t));

    //std::get<0>(t) = 897;

    printf("%d\n", std::get<0>(t));
    printf("%f\n", std::get<1>(t));

    return 0;
}
