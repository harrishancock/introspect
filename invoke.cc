#include "uncurry.hpp"

#include <cstdio>

#include <type_traits>
#include <functional>
#include <tuple>
#include <memory>

//////////////////////////////////////////////////////////////////////////////

class invoker_base {
public:
    virtual ~invoker_base () { }
    virtual void invoke (const char *args) = 0;
};

template <typename>
class invoker;

template <typename... Args>
class invoker<void(Args...)> : public invoker_base {
public:
    invoker (void (*func)(Args...)) : m_func(func) {
        /* something? */
    }

    void invoke (const char *args) override {
        m_func();
    }

private:
    using args_tuple = std::tuple<Args...>;

    std::function<void(Args...)> m_func;
};

class any_invoker {
public:
    /* function pointers */
    template <typename... Args>
    any_invoker (void (*func)(Args...))
            : m_inv(new invoker<void(Args...)> (func)) { }

    void invoke (const char *args) {
        m_inv->invoke(args);
    }

#if 0
    template <typename Return>
    Return invoke (const char *args) {
        return m_inv->invoke(args);
    }
#endif

private:
    std::unique_ptr<invoker_base> m_inv;
};

void foo () {
    printf("Hello, world!\n");
}

int bar (int& i) {
    i++;
    return i - 1;
}

int main () {
    any_invoker foo_inv { foo };
    foo_inv.invoke("");

#if 0
    any_invoker bar_inv { bar };
    bar_inv.invoke("");
#endif

    uncurry<int(int&)> uncurried_bar { bar };

    int i = 0;
    printf("i == %d\n", i);
    auto ret = uncurried_bar(std::make_tuple(std::ref(i)));
    printf("i == %d\n", i);
    printf("uncurried function returned: %d\n", ret);
}
