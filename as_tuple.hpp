#ifndef AS_TUPLE_HPP
#define AS_TUPLE_HPP

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define AS_TUPLE2_tuple_element(R, STRUCT, I, ATTRIBUTE)                     \
    template <>                                                              \
    struct tuple_element<I, STRUCT> {                                        \
        using type = decltype(STRUCT::ATTRIBUTE);                            \
    };

#define AS_TUPLE2_get_lref(R, STRUCT, I, ATTRIBUTE)                          \
    template <>                                                              \
    typename tuple_element<I, STRUCT>::type& get<I, STRUCT> (STRUCT& s) {              \
        return s.ATTRIBUTE;                                                  \
    }

#define AS_TUPLE2_get_rref(R, STRUCT, I, ATTRIBUTE)                          \
    template <>                                                              \
    typename tuple_element<I, STRUCT>::type&& get<I, STRUCT> (STRUCT&& s) {            \
        return std::move(s.ATTRIBUTE);                                       \
    }

#define AS_TUPLE2_get_clref(R, STRUCT, I, ATTRIBUTE)                         \
    template <>                                                              \
    typename tuple_element<I, STRUCT>::type const& get<I, STRUCT> (const STRUCT& s) {  \
        return s.ATTRIBUTE;                                                  \
    }

#define AS_TUPLE2_plus_sizeof(S, STATE, SIZEOF)                              \
    STATE + SIZEOF

#define AS_TUPLE2_sizeof_elem(R, STRUCT, ATTRIBUTE)                          \
    sizeof(STRUCT::ATTRIBUTE)

#define AS_TUPLE2_sizeof(STRUCT, ATTRIBUTE_SEQ)                              \
    (BOOST_PP_SEQ_FOLD_LEFT(AS_TUPLE2_plus_sizeof,                           \
            BOOST_PP_SEQ_HEAD(                                               \
                BOOST_PP_SEQ_TRANSFORM(                                      \
                    AS_TUPLE2_sizeof_elem, STRUCT, ATTRIBUTE_SEQ)),          \
            BOOST_PP_SEQ_TAIL(                                               \
                BOOST_PP_SEQ_TRANSFORM(                                      \
                    AS_TUPLE2_sizeof_elem, STRUCT, ATTRIBUTE_SEQ))))

#define AS_TUPLE2(STRUCT, ATTRIBUTE_SEQ)                                     \
    namespace std {                                                          \
                                                                             \
    template <>                                                              \
    struct tuple_size<STRUCT>                                                \
        : integral_constant<size_t, BOOST_PP_SEQ_SIZE(ATTRIBUTE_SEQ)> { };   \
                                                                             \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE2_tuple_element, STRUCT, ATTRIBUTE_SEQ)  \
                                                                             \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE2_get_lref, STRUCT, ATTRIBUTE_SEQ)       \
    }
#if 0
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE2_get_rref, STRUCT, ATTRIBUTE_SEQ)       \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE2_get_clref, STRUCT, ATTRIBUTE_SEQ)      \
                                                                             \
    }                                                                        \
                                                                             \
    static_assert(sizeof(STRUCT) <= AS_TUPLE2_sizeof(STRUCT, ATTRIBUTE_SEQ), \
            "struct/tuple size mismatch, did you forget to list all of "     \
            #STRUCT "'s attributes?");
#endif

//////////////////////////////////////////////////////////////////////////////

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
    AS_TUPLE_UNSAFE(STRUCT, ATTRIBUTE_SEQ) \
    static_assert(sizeof(STRUCT) <= sizeof(decltype(as_tuple(std::declval<STRUCT>()))), \
            "struct/tuple size mismatch, did you forget to list all of " \
            #STRUCT "'s attributes?");

//////////////////////////////////////////////////////////////////////////////

namespace supports {
    template <typename T> void as_tuple (T&&);

    template <typename T>
    struct supports_as_tuple : std::integral_constant<
            bool, !std::is_void<decltype(as_tuple(std::declval<T>()))>::value>
    { };
}

#endif
