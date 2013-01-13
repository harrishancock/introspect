#include "uncurry.hpp"
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
        auto t = std::make_tuple(Args()...);
        if (sizeof...(Args)) {
            std::stringstream ss (args, std::stringstream::in);
            from_json(ss, t);
        }
        m_func(t);
    }

private:
    uncurry<void(Args...)> m_func;
};

class any_invoker {
public:
    /* function pointers */
    template <typename Return, typename... Args>
    any_invoker (Return (*func)(Args...))
            : m_inv(new invoker<Return(Args...)> (func)) { }

    //typename std::enable_if<void_return, Return>::type
    void invoke (const char *args) {
        m_inv->invoke(args);
    }

#if 0
    /* Need to use boost::any for function's return type so this can overload
     * the base class's */
    typename std::enable_if<!void_return, Return>::type
    invoke (const char *args) {
        return m_inv->invoke(args);
    }
#endif

private:
    std::unique_ptr<invoker_base> m_inv;
};

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
