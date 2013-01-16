#include "as_tuple.hpp"
#include "tuple_fold.hpp"
#include "tuple_to_json.hpp"
#include "tuple_from_json.hpp"

struct bar {
    float x;
    float y;
};

AS_TUPLE(bar, (x)(y));
//AS_TUPLE2(bar, (x)(y));

struct foo {
    int i;
    double d;
    std::tuple<char, char> t;
    bar b;
};

AS_TUPLE(foo, (i)(d)(t)(b));
//AS_TUPLE2(foo, (i)(d)(t));

struct baz;

int main () {
    auto t = as_tuple(foo { 345, 2.5345, std::make_tuple('a', 'b'), { 9.0, 7.0 } });

    printf("%d\n", std::get<0>(t));
    printf("%f\n", std::get<1>(t));

    to_json(std::cout, t);

    bar b;
    from_json(std::cin, as_tuple(b));
    to_json(std::cout, as_tuple(b));

    static_assert(supports::supports_as_tuple<foo>::value, "foo does not support as_tuple");
    static_assert(supports::supports_as_tuple<baz>::value, "baz does not support as_tuple");
    return 0;
}
