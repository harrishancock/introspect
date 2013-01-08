#ifndef SPECTRE_HH
#define SPECTRE_HH

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

#include <utility>

#define SPECTRE_specialize_struct_element(R, STRUCT, INDEX, ATTRIBUTE)       \
    template <>                                                              \
    struct struct_element<INDEX, STRUCT> {                                   \
        using type = decltype(STRUCT::ATTRIBUTE);                            \
        static const unsigned int index = INDEX;                             \
                                                                             \
        static constexpr type& get (STRUCT& s) {                             \
            return s.ATTRIBUTE;                                              \
        }                                                                    \
                                                                             \
        static constexpr type&& get (STRUCT&& s) {                           \
            return std::move(s.ATTRIBUTE);                                   \
        }                                                                    \
                                                                             \
        static constexpr const type& get (const STRUCT& s) {                 \
            return s.ATTRIBUTE;                                              \
        }                                                                    \
                                                                             \
        static constexpr const char *name () {                               \
            return BOOST_PP_STRINGIZE(ATTRIBUTE);                            \
        }                                                                    \
    };                                                                       \

#define SPECTRE_ADAPT(STRUCT, ATTRIBUTE_SEQ)                                 \
    namespace spectre {                                                      \
                                                                             \
    template <>                                                              \
    struct struct_traits<STRUCT> {                                           \
        static const unsigned int size = BOOST_PP_SEQ_SIZE(ATTRIBUTE_SEQ);   \
    };                                                                       \
                                                                             \
    BOOST_PP_SEQ_FOR_EACH_I(                                                 \
            SPECTRE_specialize_struct_element,                               \
            STRUCT,                                                          \
            ATTRIBUTE_SEQ)                                                   \
                                                                             \
    } // namespace spectre

//////////////////////////////////////////////////////////////////////////////

namespace spectre {

template <typename Struct>
struct struct_traits;

template <unsigned int I, typename Struct>
struct struct_element;

template <unsigned int I, typename Struct>
constexpr typename struct_element<I, Struct>::type&
get (Struct& s) noexcept {
    return struct_element<I, Struct>::get(s);
}

template <unsigned int I, typename Struct>
constexpr typename struct_element<I, Struct>::type&&
get (Struct&& s) noexcept {
    return struct_element<I, Struct>::get(s);
}

template <unsigned int I, typename Struct>
constexpr const typename struct_element<I, Struct>::type&
get (const Struct& s) noexcept {
    return struct_element<I, Struct>::get(s);
}

template <unsigned int I, typename Struct>
constexpr const char *
name () noexcept {
    return struct_element<I, typename std::remove_const<Struct>::type>::name();
}

} // namespace spectre

#endif
