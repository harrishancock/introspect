#include <cstdio>
#include <type_traits>
#include <typeinfo>

struct nil { };

template <template <typename...> class Container, typename... Types>
struct variadic {
private:
    template <typename Head, typename... Tail>
    static constexpr Head get_head (Head&&, Tail&&...) { return { }; }

    template <typename Head>
    static constexpr nil get_tail (Head&&) { return { }; }

    template <typename Head, typename Tail0, typename... Tail>
    static constexpr Container<Tail0, Tail...>
    get_tail (Head&&, Tail0&&, Tail&&...) { return { }; }
        
public:
    using head = decltype(get_head(std::declval<Types>()...));
    using tail = decltype(get_tail(std::declval<Types>()...));

    static constexpr unsigned int size = sizeof...(Types);
};

template <template <typename...> class Container>
struct variadic<Container> {
    using tail = nil;

    static constexpr unsigned int size = 0;
};

template <typename... List>
struct list {
    using head = typename variadic<list, List...>::head;
    using tail = typename variadic<list, List...>::tail;
};

template <typename Map, typename Key, unsigned int C = 0>
struct count : count<typename Map::tail, Key, C + std::is_same<Key, typename Map::key>::value> { };

template <typename Key, unsigned int C>
struct count<nil, Key, C> {
    static constexpr unsigned int value = C;
};

template <typename, typename...>
struct map;

template <template <typename, typename> class Pair, typename Key, typename Value, typename... Tail>
struct map<Pair<Key, Value>, Tail...> {
    using key = Key;
    using value = Value;
    using head = typename variadic<map, Pair<Key, Value>, Tail...>::head;
    using tail = typename variadic<map, Pair<Key, Value>, Tail...>::tail;

    static_assert(!count<tail, Key>::value, "duplicate key inserted");
};

template <typename Key, typename Value>
struct pair { };

template <typename Map, typename Key, typename Enable = void>
struct at;

template <typename Map, typename Key>
struct at<Map, Key, typename std::enable_if<std::is_same<Key, typename Map::key>::value>::type> {
    using type = typename Map::value;
};

template <typename Map, typename Key>
struct at<Map, Key, typename std::enable_if<!std::is_same<Key, typename Map::key>::value>::type> {
    static_assert(!std::is_same<nil, typename Map::tail>::value, "key does not exist");
    using type = typename at<typename Map::tail, Key>::type;
};

int main () {
    using v = list<int, char, float>;
    using h0 = v::head;
    using t0 = v::tail;
    using h1 = t0::head;
    using t1 = t0::tail;
    using h2 = t1::head;
    using t2 = t1::tail;

    printf("%s\n", typeid(v).name());
    printf("%s\n", typeid(h0).name());
    printf("%s\n", typeid(t0).name());
    printf("%s\n", typeid(h1).name());
    printf("%s\n", typeid(t1).name());
    printf("%s\n", typeid(h2).name());
    printf("%s\n", typeid(t2).name());

    using m = map<pair<int, char>, pair<double, float>, pair<unsigned, bool>>;
    using k0 = m::key;
    using v0 = typename at<m, k0>::type;
    using k1 = m::tail::key;
    using v1 = typename at<m, k1>::type;
    using k2 = m::tail::tail::key;
    using v2 = typename at<m, k2>::type;

    printf("%s\n", typeid(m).name());
    printf("%s\n", typeid(k0).name());
    printf("%s\n", typeid(v0).name());
    printf("%s\n", typeid(k1).name());
    printf("%s\n", typeid(v1).name());
    printf("%s\n", typeid(k2).name());
    printf("%s\n", typeid(v2).name());

    printf("count(int): %d\n", count<m, int>::value);
    printf("count(double): %d\n", count<m, double>::value);
    printf("count(unsigned): %d\n", count<m, unsigned>::value);

    printf("count(bool): %d\n", count<m, bool>::value);
}
