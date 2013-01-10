#include <cstdio>

#include <type_traits>
#include <functional>
#include <memory>

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
    template <typename Func>
    invoker (Func func) : m_func(func) { }

    void invoke (const char *args) override {
        m_func();
    }

private:
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
}
