#ifndef ANY_FUNCTION_HPP
#define ANY_FUNCTION_HPP

#include <boost/any.hpp>

class function_base {
public:
    virtual ~function_base () { }
    virtual boost::any operator() (archive&&) = 0;
};

template <typename>
class function;

template <typename Return, typename... Args>
class function<Return(Args...)> : public function_base {
public:
    function (Return (*func)(Args...)) : m_func(func) { }

    boost::any operator() (archive&& ar) override {
        return helper(std::forward<archive>(ar));
    }

    typename std::enable_if<std::is_void<Return>::value, boost::any>::type
    helper (archive&& ar) {
        std::tuple<Args...> t;
        ar >> t;
        return m_func(t);
    }

    typename std::enable_if<!std::is_void<Return>::value, boost::any>::type
    helper (archive&& ar) {
        std::tuple<Args...> t;
        ar >> t;
        m_func(t);
        return { };
    }

private:
    uncurry<Return(Args...)> m_func;
};

/* Could probably just get away with a ptr_container of function_bases, would
 * remove one level of indirection. */
class any_function {
public:
    /* function pointers */
    template <typename Return, typename... Args>
    any_function (Return (*func)(Args...))
            : m_func(new function<Return(Args...)> (func)) { }

    boost::any operator() (archive&& ar) {
        return (*m_func)(std::forward<archive>(ar));
    }

private:
    std::unique_ptr<function_base> m_func;
};

#endif
