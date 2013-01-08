#ifndef INTROSPECT_HH
#define INTROSPECT_HH

#define BOOST_RESULT_OF_USE_DECLTYPE

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/fusion/include/distance.hpp>
#include <boost/fusion/include/begin.hpp>

#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/stringize.hpp>

//////////////////////////////////////////////////////////////////////////////

/*
 * We'll make a couple of modifications to Boost.Fusion's default struct
 * adapter macros.
 *
 * First, although Boost.Fusion comes with an extension to provide access to
 * the names of the members of the struct in string form, at run-time, it does
 * not (from what I can tell) provide the same facility for the name of the
 * actual struct itself. RTTI might tell us that name, or it might not. We can
 * make sure it's available.
 *
 * Second, with C++11's decltype, Boost.Fusion's struct adapter macros can be
 * made easier to use. Instead of
 *
 * BOOST_FUSION_ADAPT_STRUCT(foo, (type0, member0)(type1, member1)...)
 *
 * we can say
 *
 * INTROSPECT_ADAPT(foo, (member0)(member1)...)
 *
 * In addition to removing the requirement of specifying the type of each
 * member, this makes templated types with multiple template parameters
 * simpler to deal with. That is,
 *
 * BOOST_FUSION_ADAPT_STRUCT(foo, (type0<param0, param1>, member0))
 *
 * contains a parse error, as type0<param0, param1> would be interpreted as two
 * distinct macro arguments.
 */

#define INTROSPECT_add_type(s, struct_name, member) \
    decltype(struct_name::member), member

#define INTROSPECT(struct_name, members) \
    BOOST_FUSION_ADAPT_STRUCT(struct_name, \
            BOOST_PP_SEQ_TRANSFORM(INTROSPECT_add_type, struct_name, members))

//////////////////////////////////////////////////////////////////////////////

namespace introspect {
namespace detail {

/**
 */
template <typename Func>
struct fold_aux {
    Func f;
    fold_aux (Func func) : f(func) { }

    template <typename Z, typename Iter>
    Z operator() (const Z& z, const Iter& it) const {
        using boost::fusion::extension::struct_member_name;

        /* Assuming Iter is a basic_iterator. This code likely relies on
         * implementation details. */
        typedef typename Iter::seq_type seq_type;
        typedef typename Iter::index index;

        auto name = struct_member_name<seq_type, index::value>::call();

        return f(index::value, name, z, deref(it));
    }
};

template <typename Func>
struct for_each_aux {
    Func f;
    for_each_aux (Func func) : f(func) { }

    /* We have to return something, since this will be called by fold. */
    template <typename T>
    int operator() (size_t index, const char* name, int z, T& t) const {
        f(index, name, t);
        return 0;
    }
};

} // namespace detail

//////////////////////////////////////////////////////////////////////////////

template <typename Seq, typename Z, typename Func>
Z fold (Seq& s, const Z& z, Func f) {
    return boost::fusion::iter_fold(s, z, detail::fold_aux<Func>(f));
}

template <typename Seq, typename Func>
void for_each (Seq& s, Func f) {
    fold(s, 0, detail::for_each_aux<Func>(f));
}

} // namespace introspect

#endif
