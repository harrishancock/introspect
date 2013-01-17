#ifndef AS_TUPLE_HPP
#define AS_TUPLE_HPP

#include "sizeof.hpp"

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define AS_TUPLE_ref_type(R, STRUCT, ATTRIBUTE)                              \
    typename std::add_lvalue_reference<decltype(STRUCT::ATTRIBUTE)>::type

#define AS_TUPLE_ref_value(R, STRUCT_VALUE, ATTRIBUTE)                       \
    std::ref(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_cref_type(R, STRUCT, ATTRIBUTE)                             \
    typename std::add_lvalue_reference<                                      \
        typename std::add_const<decltype(STRUCT::ATTRIBUTE)>::type>::type

#define AS_TUPLE_cref_value(R, STRUCT_VALUE, ATTRIBUTE)                      \
    std::cref(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_move_type(R, STRUCT, ATTRIBUTE)                             \
    decltype(STRUCT::ATTRIBUTE)

#define AS_TUPLE_move_value(R, STRUCT_VALUE, ATTRIBUTE)                      \
    std::move(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_enumerate(OP, DATA, SEQ)                                    \
    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(OP, DATA, SEQ))

#define AS_TUPLE_def(PRE, STRUCT, POST, ATTRIBUTE_SEQ, TYPE_OP, VALUE_OP)    \
    inline std::tuple<AS_TUPLE_enumerate(TYPE_OP, STRUCT, ATTRIBUTE_SEQ)>    \
    as_tuple (PRE STRUCT POST s) {                                           \
        return std::make_tuple(                                              \
                AS_TUPLE_enumerate(VALUE_OP, s, ATTRIBUTE_SEQ));             \
    }

#define AS_TUPLE_UNSAFE(STRUCT, ATTRIBUTE_SEQ) \
    AS_TUPLE_def(, STRUCT, &, ATTRIBUTE_SEQ, AS_TUPLE_ref_type, AS_TUPLE_ref_value) \
    AS_TUPLE_def(const, STRUCT, &, ATTRIBUTE_SEQ, AS_TUPLE_cref_type, AS_TUPLE_cref_value) \
    AS_TUPLE_def(, STRUCT, &&, ATTRIBUTE_SEQ, AS_TUPLE_move_type, AS_TUPLE_move_value)

#define AS_TUPLE(STRUCT, ATTRIBUTE_SEQ) \
    AS_TUPLE_SIZEOF_CHECK(STRUCT, ATTRIBUTE_SEQ) \
    AS_TUPLE_UNSAFE(STRUCT, ATTRIBUTE_SEQ)

//////////////////////////////////////////////////////////////////////////////

namespace supports {

template <typename T> void as_tuple (T&&);

template <typename T>
struct fn_as_tuple : std::integral_constant<
        bool, !std::is_void<decltype(as_tuple(std::declval<T>()))>::value>
{ };

}

#endif
