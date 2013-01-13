#include "as_tuple.hpp"
#include "tuple_fold.hpp"
#include "tuple_to_json.hpp"

struct foo {
    int i;
    double d;
    std::tuple<char, char> t;
};

AS_TUPLE(foo, (i)(d)(t));

int main () {
    auto t = as_tuple(foo { 345, 2.5345, std::make_tuple('a', 'b') });

    printf("%d\n", std::get<0>(t));
    printf("%f\n", std::get<1>(t));

    to_json(t);

    return 0;
}
