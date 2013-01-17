#ifndef AS_TUPLE_STD_HPP
#define AS_TUPLE_STD_HPP

#include "sizeof.hpp"

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

#define AS_TUPLE_STD_tuple_element(R, STRUCT, I, ATTRIBUTE)                  \
    template <>                                                              \
    struct tuple_element<I, STRUCT> {                                        \
        using type = decltype(STRUCT::ATTRIBUTE);                            \
    };

#define AS_TUPLE_STD_get_lref(R, STRUCT, I, ATTRIBUTE)                       \
    template <>                                                              \
    typename tuple_element<I, STRUCT>::type&                                 \
    get<I, STRUCT> (STRUCT& s) {                                             \
        return s.ATTRIBUTE;                                                  \
    }

#define AS_TUPLE_STD_get_rref(R, STRUCT, I, ATTRIBUTE)                       \
    template <>                                                              \
    typename tuple_element<I, STRUCT>::type&&                                \
    get<I, STRUCT> (STRUCT&& s) {                                            \
        return std::move(s.ATTRIBUTE);                                       \
    }

#define AS_TUPLE_STD_get_clref(R, STRUCT, I, ATTRIBUTE)                      \
    template <>                                                              \
    typename tuple_element<I, STRUCT>::type const&                           \
    get<I, STRUCT> (const STRUCT& s) {                                       \
        return s.ATTRIBUTE;                                                  \
    }

/* TODO put the get definitions in the struct's namespace */
#define AS_TUPLE_STD_UNSAFE(STRUCT, ATTRIBUTE_SEQ)                           \
    namespace std {                                                          \
                                                                             \
    template <>                                                              \
    struct tuple_size<STRUCT>                                                \
        : integral_constant<size_t, BOOST_PP_SEQ_SIZE(ATTRIBUTE_SEQ)> { };   \
                                                                             \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE_STD_tuple_element,STRUCT,ATTRIBUTE_SEQ) \
                                                                             \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE_STD_get_lref, STRUCT, ATTRIBUTE_SEQ)    \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE_STD_get_rref, STRUCT, ATTRIBUTE_SEQ)    \
    BOOST_PP_SEQ_FOR_EACH_I(AS_TUPLE_STD_get_clref, STRUCT, ATTRIBUTE_SEQ)   \
                                                                             \
    }

#define AS_TUPLE_STD(STRUCT, ATTRIBUTE_SEQ)                                  \
    AS_TUPLE_SIZEOF_CHECK(STRUCT, ATTRIBUTE_SEQ)                             \
    AS_TUPLE_STD_UNSAFE(STRUCT, ATTRIBUTE_SEQ)

#endif
