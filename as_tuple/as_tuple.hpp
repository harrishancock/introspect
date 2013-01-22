#ifndef AS_TUPLE_HPP
#define AS_TUPLE_HPP

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define AS_TUPLE_ref_value(R, STRUCT_VALUE, ATTRIBUTE)                       \
    std::ref(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_cref_value(R, STRUCT_VALUE, ATTRIBUTE)                      \
    std::cref(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_move_value(R, STRUCT_VALUE, ATTRIBUTE)                      \
    std::move(STRUCT_VALUE.ATTRIBUTE)

#define AS_TUPLE_enumerate(OP, DATA, SEQ)                                    \
    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(OP, DATA, SEQ))

#define AS_TUPLE_def(STRUCT, ATTRIBUTE_SEQ, VALUE_OP) \
    inline auto as_tuple (STRUCT s) \
        -> decltype(std::make_tuple(    \
                    AS_TUPLE_enumerate(VALUE_OP, s, ATTRIBUTE_SEQ))) { \
        return std::make_tuple( \
                    AS_TUPLE_enumerate(VALUE_OP, s, ATTRIBUTE_SEQ)); \
    }

#define AS_TUPLE_UNSAFE(STRUCT, ATTRIBUTE_SEQ) \
    AS_TUPLE_def(STRUCT&, ATTRIBUTE_SEQ, AS_TUPLE_ref_value) \
    AS_TUPLE_def(const STRUCT&, ATTRIBUTE_SEQ, AS_TUPLE_cref_value) \
    AS_TUPLE_def(STRUCT&&, ATTRIBUTE_SEQ, AS_TUPLE_move_value)

#define AS_TUPLE(STRUCT, ATTRIBUTE_SEQ) \
    AS_TUPLE_UNSAFE(STRUCT, ATTRIBUTE_SEQ)  \
    static_assert(sizeof(STRUCT) <= sizeof(decltype(as_tuple(std::declval<STRUCT>()))), \
            "struct/tuple size mismatch, did you forget to list all of "    \
            #STRUCT "'s attributes?");

//////////////////////////////////////////////////////////////////////////////

namespace supports {

template <typename T> void as_tuple (T&&);

template <typename T>
struct fn_as_tuple : std::integral_constant<
        bool, !std::is_void<decltype(as_tuple(std::declval<T>()))>::value>
{ };

}

#endif
