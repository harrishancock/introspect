#include "as_tuple/as_tuple.hpp"
#include "tuple_fold.hpp"
#include "tuple_to_json.hpp"
#include "tuple_from_json.hpp"

struct bar {
    float x;
    float y;
};

AS_TUPLE(bar, (x)(y));

struct foo {
    int i;
    double d;
    std::tuple<char, char> t;
    bar b;
};

AS_TUPLE(foo, (i)(d)(t)(b));

struct baz;

int main () {
    auto t = as_tuple(foo { 345, 2.5345, std::make_tuple('a', 'b'), { 9.0, 7.0 } });

    printf("%d\n", std::get<0>(t));
    printf("%f\n", std::get<1>(t));

    to_json(std::cout, t);

#if 0
    bar b;
    from_json(std::cin, as_tuple(b));
    to_json(std::cout, as_tuple(b));
#endif
    return 0;
}
