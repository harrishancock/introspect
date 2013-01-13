#ifndef TUPLE_TO_JSON_HPP
#define TUPLE_TO_JSON_HPP

#include "tuple_fold.hpp"

#include <cstdio>

struct to_json_aux {
    void start (int n) const {
        printf("%c ", n ? ',' : '[');
    }

    int operator() (int n, char c) const {
        start(n);
        printf("\"%c\"", c);
        return n + 1;
    }

    int operator() (int n, int i) const {
        start(n);
        printf("%i", i);
        return n + 1;
    }

    int operator() (int n, double d) const {
        start(n);
        printf("%f", d);
        return n + 1;
    }

    template <typename T>
    int operator() (int n, const T& value) const {
        start(n);
        foldl(to_json_aux(), 0, value);
        printf(" ]");
        return n + 1;
    }
};

template <typename T>
void to_json (const T& t) {
    foldl(to_json_aux(), 0, t);
    printf(" ]\n");
}

#endif
