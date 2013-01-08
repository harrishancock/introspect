#include "spectre.hh"

#include <cstdio>
#include <cstdlib>

#include <typeinfo>

struct foo {
    int i;
    double d;
};

SPECTRE_ADAPT(foo, (i)(d));

constexpr foo f { 666, 3.14 };

constexpr int getf () {
    return spectre::get<0>(f);
}

template <int I>
struct barf {
    static const int value = I;
};

namespace spectre {

template <typename T>
bool is_a (unsigned int i, const foo& f) {
    auto code = typeid(T).hash_code();

    switch (i) {
        case 0:
            return code = typeid(f.i).hash_code();
        case 1:
            return code = typeid(f.d).hash_code();
    }

    abort();
}

} // namespace spectre

int main () {
    printf("%s: %d\n", spectre::name<0, decltype(f)>(), spectre::get<0>(f));
    printf("%s: %f\n", spectre::name<1, decltype(f)>(), spectre::get<1>(f));
    //auto x = spectre::get<2>(f);
    
    printf("%d\n", barf<getf()>::value);

    if (is_a<int>(0, f)) {
    }

    return 0;
}
