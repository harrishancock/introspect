#include "uncurry.hpp"
#include "any_function.hpp"
#include "tuple_from_json.hpp"
#include "as_tuple.hpp"

#include <cstdio>

#include <type_traits>
#include <functional>
#include <tuple>
#include <memory>
#include <sstream>
#include <string>

//////////////////////////////////////////////////////////////////////////////

void foo () {
    printf("Hello, world!\n");
}

void bar (int i, float j, char k) {
    printf("bar(%d, %f, %c)\n", i, j, k);
}

int main () {
    any_invoker foo_inv { foo };
    foo_inv.invoke("");

#if 0
    any_invoker bar_inv { bar };
    bar_inv.invoke("");

    uncurry<int(int&)> uncurried_bar { bar };

    int i = 0;
    printf("i == %d\n", i);
    auto ret = uncurried_bar(std::make_tuple(std::ref(i)));
    printf("i == %d\n", i);
    printf("uncurried function returned: %d\n", ret);
#endif

    any_invoker bar_inv { bar };
    bar_inv.invoke("[ 666, 3.14, c ]");
}
