#include <cstdio>

#include <type_traits>
#include <functional>
#include <tuple>
#include <memory>

template <typename Return, typename... Args>
struct uncurry;

template <typename Return, typename... Args>
struct uncurry<Return(Args...)> {
    template <typename Func>
    uncurry (const Func& func) : m_func(func) {
    }

    Return operator() (const std::tuple<Args...>& args) {
        return call<sizeof...(Args)>(args);
    }

private:
    /* Use enable_if magic to simulate partial specialization of a function
     * template. */
    template <unsigned int N, typename... Unpacked>
    typename std::enable_if<0 != N, Return>::type
    call (const std::tuple<Args...>& args, Unpacked&&... unpacked) {
        return call<N-1>(args, std::get<N-1>(args),
                std::forward<Unpacked>(unpacked)...);
    }

    template <unsigned int N, typename... Unpacked>
    typename std::enable_if<0 == N, Return>::type
    call (const std::tuple<Args...>&, Unpacked&&... unpacked) {
        return m_func(std::forward<Unpacked>(unpacked)...);
    }

    std::function<Return(Args...)> m_func;
};

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

int bar () {
    return 3;
}

int main () {
    any_invoker foo_inv { foo };
    foo_inv.invoke("");

#if 0
    any_invoker bar_inv { bar };
    bar_inv.invoke("");
#endif

    uncurry<int()> uncurried_bar { bar };
    auto i = uncurried_bar(std::tuple<>());
    printf("uncurried function returned: %d\n", i);
}
