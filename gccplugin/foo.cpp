#include <cstdio>

namespace foo {

struct [[gnu::reflection(1)]] A {
public:
    int x;
    bool hello () {
        i = 5;
        return true;
    }
private:
    int i;
    struct anus;
};

enum [[gnu::reflection(1)]] B {
    B_FOO,
    B_FUZZ
};

}

int fart () {
    foo::A a;
    a.hello();
    return 1;
}

int main () {
    printf("Hello, world!\n");
    return fart();
}
