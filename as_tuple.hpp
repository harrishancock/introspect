#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#if 0
inline std::tuple<std::add_lvalue_reference<decltype(foo::i)>::type,
                  std::add_lvalue_reference<decltype(foo::d)>::type>
as_tuple (foo& s) {
    printf("as_tuple(foo&)\n");
    return std::make_tuple(std::ref(s.i), std::ref(s.d));
}

inline std::tuple<decltype(foo::i), decltype(foo::d)>
as_tuple (foo&& s) {
    printf("as_tuple(foo&&)\n");
    return std::make_tuple(std::move(s.i), std::move(s.d));
}

inline std::tuple<std::add_lvalue_reference<std::add_const<decltype(foo::i)>::type>::type,
                  std::add_lvalue_reference<std::add_const<decltype(foo::d)>::type>::type>
as_tuple (const foo& s) {
    printf("as_tuple(const foo&)\n");
    return std::make_tuple(std::cref(s.i), std::cref(s.d));
}
#endif

#define AS_TUPLE_ref_type(R, STRUCT, ATTRIBUTE) \
    typename std::add_lvalue_reference<decltype(STRUCT::ATTRIBUTE)>::type

#define AS_TUPLE_ref_value(R, STRUCT_VALUE, ATTRIBUTE) \
    std::ref(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_cref_type(R, STRUCT, ATTRIBUTE) \
    typename std::add_lvalue_reference< \
        typename std::add_const<decltype(STRUCT::ATTRIBUTE)>::type>::type

#define AS_TUPLE_cref_value(R, STRUCT_VALUE, ATTRIBUTE) \
    std::cref(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_move_type(R, STRUCT, ATTRIBUTE) \
    decltype(STRUCT::ATTRIBUTE)

#define AS_TUPLE_move_value(R, STRUCT_VALUE, ATTRIBUTE) \
    std::move(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_def(PRE, STRUCT, POST, ATTRIBUTE_SEQ, TYPE_OP, VALUE_OP) \
    inline std::tuple< \
        BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(TYPE_OP, STRUCT, ATTRIBUTE_SEQ)) \
        > \
    as_tuple (PRE STRUCT POST s) {  \
        return std::make_tuple( \
            BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(VALUE_OP, s, ATTRIBUTE_SEQ)) \
            );  \
    }

#define AS_TUPLE(STRUCT, ATTRIBUTE_SEQ) \
    AS_TUPLE_def(, STRUCT, &, ATTRIBUTE_SEQ, AS_TUPLE_ref_type, AS_TUPLE_ref_value) \
    AS_TUPLE_def(const, STRUCT, &, ATTRIBUTE_SEQ, AS_TUPLE_cref_type, AS_TUPLE_cref_value) \
    AS_TUPLE_def(, STRUCT, &&, ATTRIBUTE_SEQ, AS_TUPLE_move_type, AS_TUPLE_move_value)
