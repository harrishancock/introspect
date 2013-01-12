#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

/**
 * Convert an object with function semantics taking an arbitrary parameter
 * list into a function object taking those same parameters in tuple form.
 *
 * Inspired by, and and very similar to, Christopher Allen Ogden's
 * call-with-tuple:
 * https://github.com/Sydius/call-with-tuple
 */
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
    /* General case: unpack the (N-1)th argument from the arguments tuple,
     * placing it in front of the previously unpacked arguments. */
    template <unsigned int N, typename... Unpacked>
    typename std::enable_if<0 != N, Return>::type
    call (const std::tuple<Args...>& args, Unpacked&&... unpacked) {
        return call<N-1>(args, std::get<N-1>(args),
                std::forward<Unpacked>(unpacked)...);
    }

    /* Base case: all arguments unpacked, can finally call the wrapped
     * function. */
    template <unsigned int N, typename... Unpacked>
    /* Use enable_if to simulate function template partial specialization. */
    typename std::enable_if<0 == N, Return>::type
    call (const std::tuple<Args...>&, Unpacked&&... unpacked) {
        return m_func(std::forward<Unpacked>(unpacked)...);
    }

    std::function<Return(Args...)> m_func;
};

/**
 * Void return specialization of uncurry.
 */
template <typename... Args>
struct uncurry<void(Args...)> {
    template <typename Func>
    uncurry (const Func& func) : m_func(func) {
    }

    void operator() (const std::tuple<Args...>& args) {
        call<sizeof...(Args)>(args);
    }

private:
    template <unsigned int N, typename... Unpacked>
    typename std::enable_if<0 != N, void>::type
    call (const std::tuple<Args...>& args, Unpacked&&... unpacked) {
        call<N-1>(args, std::get<N-1>(args),
                std::forward<Unpacked>(unpacked)...);
    }

    template <unsigned int N, typename... Unpacked>
    typename std::enable_if<0 == N, void>::type
    call (const std::tuple<Args...>&, Unpacked&&... unpacked) {
        m_func(std::forward<Unpacked>(unpacked)...);
    }

    std::function<void(Args...)> m_func;
};
