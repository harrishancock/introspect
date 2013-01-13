#ifndef TUPLE_FROM_JSON_HPP
#define TUPLE_FROM_JSON_HPP

#include "tuple_fold.hpp"

#include <cassert>

#include <iostream>

struct from_json_aux {
    explicit from_json_aux (std::istream& is) : m_is(is) { }

    void start (int n) const {
        char c;
        m_is >> c;
        assert(n ? ',' : '[' == c);
    }

    int operator() (int n, char& c) const {
        start(n);
        m_is >> c;
        return n + 1;
    }

    int operator() (int n, int& i) const {
        start(n);
        m_is >> i;
        return n + 1;
    }

    int operator() (int n, float& f) const {
        start(n);
        m_is >> f;
        return n + 1;
    }

    int operator() (int n, double& d) const {
        start(n);
        m_is >> d;
        return n + 1;
    }

private:
    std::istream& m_is;
};

template <typename T>
void from_json (std::istream& is, T& t) {
    foldl(from_json_aux(is), 0, t);
    char c;
    is >> c;
    assert(']' == c);
}

#endif
