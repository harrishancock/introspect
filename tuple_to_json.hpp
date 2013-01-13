#ifndef TUPLE_TO_JSON_HPP
#define TUPLE_TO_JSON_HPP

#include "tuple_fold.hpp"

#include <iostream>

struct to_json_aux {
    explicit to_json_aux (std::ostream& os) : m_os(os) { }

    void start (int n) const {
        m_os << (n ? ", " : "[ ");
    }

    int operator() (int n, char c) const {
        start(n);
        m_os << '"' << c << '"';
        return n + 1;
    }

    int operator() (int n, int i) const {
        start(n);
        m_os << i;
        return n + 1;
    }

    int operator() (int n, float f) const {
        start(n);
        m_os << f;
        return n + 1;
    }

    int operator() (int n, double d) const {
        start(n);
        m_os << d;
        return n + 1;
    }

    template <typename... Args>
    int operator() (int n, const std::tuple<Args...>& value) const {
        start(n);
        foldl(to_json_aux(m_os), 0, value);
        m_os << " ]";
        return n + 1;
    }

    template <typename T>
    int operator() (int n, const T& value) const {
        return (*this)(n, as_tuple(value));
    }

private:
    std::ostream& m_os;
};

template <typename T>
void to_json (std::ostream& os, const T& t) {
    foldl(to_json_aux(os), 0, t);
    os << " ]\n";
}

#endif
