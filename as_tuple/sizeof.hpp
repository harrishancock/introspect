#ifndef AS_TUPLE_SIZEOF_HPP
#define AS_TUPLE_SIZEOF_HPP

#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define AS_TUPLE_SIZEOF_plus(S, STATE, SIZEOF)                               \
    STATE + SIZEOF

#define AS_TUPLE_SIZEOF_elem(R, STRUCT, ATTRIBUTE)                           \
    sizeof(STRUCT::ATTRIBUTE)

#define AS_TUPLE_SIZEOF(STRUCT, ATTRIBUTE_SEQ)                               \
    (BOOST_PP_SEQ_FOLD_LEFT(AS_TUPLE_SIZEOF_plus,                            \
            BOOST_PP_SEQ_HEAD(                                               \
                BOOST_PP_SEQ_TRANSFORM(                                      \
                    AS_TUPLE_SIZEOF_elem, STRUCT, ATTRIBUTE_SEQ)),           \
            BOOST_PP_SEQ_TAIL(                                               \
                BOOST_PP_SEQ_TRANSFORM(                                      \
                    AS_TUPLE_SIZEOF_elem, STRUCT, ATTRIBUTE_SEQ))))

#define AS_TUPLE_SIZEOF_CHECK(STRUCT, ATTRIBUTE_SEQ)                         \
    static_assert(sizeof(STRUCT) <= AS_TUPLE_SIZEOF(STRUCT, ATTRIBUTE_SEQ),  \
            "struct/tuple size mismatch, did you forget to list all of "     \
            #STRUCT "'s attributes?");

#endif
