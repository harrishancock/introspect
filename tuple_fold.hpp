#ifndef TUPLE_FOLD_HPP
#define TUPLE_FOLD_HPP

#include <tuple>

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) == N, Z>::type
foldr (Func func, const Z& z, std::tuple<Args...>& t) {
    return z;
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) != N, Z>::type
foldr (Func func, const Z& z, std::tuple<Args...>& t) {
    return func(std::get<N>(t), foldr<N+1>(func, z, t));
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) == N, Z>::type
foldr (Func func, const Z& z, const std::tuple<Args...>& t) {
    return z;
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) != N, Z>::type
foldr (Func func, const Z& z, const std::tuple<Args...>& t) {
    return func(std::get<N>(t), foldr<N+1>(func, z, t));
}

template <unsigned int N = 0, typename Func, typename Z, typename T>
typename std::enable_if<supports::fn_as_tuple<T>::value, Z>::type
foldr (Func func, Z&& z, T&& t) {
    return foldr(func, std::forward<Z>(z), std::forward<T>(t));
}

template <unsigned int N = 0, typename Func, typename Z, typename T>
typename std::enable_if<!supports::fn_as_tuple<T>::value, Z>::type
foldr (Func, Z&&, T&&) {
    static_assert(supports::fn_as_tuple<T>::value, "non-tuple object passed to foldr");
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) == N, Z>::type
foldl (Func func, const Z& z, std::tuple<Args...>& t) {
    return z;
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) != N, Z>::type
foldl (Func func, const Z& z, std::tuple<Args...>& t) {
    return foldl<N+1>(func, func(z, std::get<N>(t)), t);
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) == N, Z>::type
foldl (Func func, const Z& z, const std::tuple<Args...>& t) {
    return z;
}

template <unsigned int N = 0, typename Func, typename Z, typename... Args>
typename std::enable_if<sizeof...(Args) != N, Z>::type
foldl (Func func, const Z& z, const std::tuple<Args...>& t) {
    return foldl<N+1>(func, func(z, std::get<N>(t)), t);
}

template <unsigned int N = 0, typename Func, typename Z, typename T>
typename std::enable_if<supports::fn_as_tuple<T>::value, Z>::type
foldl (Func func, Z&& z, T&& t) {
    return foldl(func, std::forward<Z>(z), std::forward<T>(t));
}

template <unsigned int N = 0, typename Func, typename Z, typename T>
typename std::enable_if<!supports::fn_as_tuple<T>::value, Z>::type
foldl (Func, Z&&, T&&) {
    static_assert(supports::fn_as_tuple<T>::value, "non-tuple object passed to foldl");
}

#endif
