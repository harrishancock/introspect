#include <cstdio>

struct [[gnu::reflection(1)]] A {
public:
    int x;
    bool hello () {
        i = 5;
        return true;
    }
private:
    int i;
    struct [[gnu::reflection(1)]] anus;
};

enum [[gnu::reflection(1)]] B {
    B_FOO = 0,
    B_FUZZ
};

int fart () {
    A a;
    a.hello();
    return 1;
}

int main () {
    printf("Hello, world!\n");
    return fart();
}
