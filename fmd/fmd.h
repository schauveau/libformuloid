#ifndef FMD_H
#define FMD_H

#include <iostream>
#include <iomanip>
#include <functional>
#include <cassert>
#include <tuple>
#include <cmath>
#include <numbers>
#include <bitset>

namespace fmd {

/// @brief Prevent derived classes from becoming basal formuloid types.
struct non_basal_type {};

/// @brief The non-templated base class of all actual formuloid implementations.
struct formuloid_root {};


/// \brief The concept fmd::IsBasalFormuloid<T> is satisfied if and only
/// if type T qualifies as a basal formuloid.
///
/// \ingroup grp_concept
template <typename T>
concept IsBasalFormuloid =
     std::copyable<T>
  && std::default_initializable<T> // TODO: is that really needed?
  && !std::derived_from<T, fmd::formuloid_root>
  // In order to avoid some annoying ambiguities in the implementation
  // we need to exclude classes derived from fmd::non_basal_type
  // but also pointers to those classes. 
  && !std::derived_from<std::remove_cv_t<std::remove_pointer_t<T>>,
                        fmd::non_basal_type> 
  ;

namespace internal {

template <typename T>
struct basal_type_of_helper {
};

template <typename T>
requires IsBasalFormuloid<T> 
struct basal_type_of_helper<T> {
  typedef T type ;
};

//
// Here, we are testing for formuloid_root but we
// really expect actual_formuloid<U>.
//
template <typename T>
requires ( std::derived_from<T,fmd::formuloid_root> )
struct basal_type_of_helper<T> {
  typedef typename T::basal_type type ;
};

} // of namespace internal

// documented in doc/concepts.dox
template <typename T> using basal_type_of = typename fmd::internal::basal_type_of_helper<T>::type ;

/// \brief Match the basal type of a formuloid type with any of the specified type.
///
/// This is true if and only if
///  - LIST is empty 
///  - or fmd::basal_type_of<T> is the same as the basal type of any element of LIST. 
///
/// Examples:
/// \code
///   static_assert( fmd::MatchAnyBasalType< int > );
///   static_assert( fmd::MatchAnyBasalType< fmd::func<int> >  );
///   static_assert( fmd::MatchAnyBasalType< fmd::func<int>,  int>  );
///   static_assert( fmd::MatchAnyBasalType< fmd::func<int>,  bool, double, int>  );
///   static_assert( fmd::MatchAnyBasalType< fmd::func<int>,  fmd::functoid<int> >  );
///   static_assert( ! fmd::MatchAnyBasalType< fmd::func<int>,  bool, double  >  );
///   static_assert( ! fmd::MatchAnyBasalType< fmd::func<int>,  fmd::functoid<bool> >  );
/// \endcode
/// 
///
/// \ingroup grp_concept
template <typename T, typename... LIST>
concept MatchAnyBasalType = 
  (sizeof...(LIST)==0)
  || (std::is_same_v<fmd::basal_type_of<T>,fmd::basal_type_of<LIST>> || ...)
  ;


template <IsBasalFormuloid T> class node ;


/// A global counter used to re-number local variables while
/// printing formuloid formulas. It is supposed to be set to 0   
/// before printing a formula.
extern size_t current_frame_id; 


/// \brief The concept \c fmd::IsActualFormuloid<T> is satisfied if and only if type T
///        implements an actual formuloid. 
///
/// \tparam T    the tested type.
///
/// \tparam U... when set, an additionnal constraint is
///              that the basal type of T should match the basal
///              type of at least one U.
///
/// By default, \c fmd::IsActualFormuloid is satisfied for all \c fmd::func.
///
/// Examples:
/// \code
///   // Without basal constraint 
///   static_assert( !fmd::IsActualFormuloid< int > );
///   static_assert( fmd::IsActualFormuloid< fmd::formuloid<int> > );
///   static_assert( fmd::IsActualFormuloid< fmd::func<int> > );
///   // With basal constraint
///   static_assert( fmd::IsActualFormuloid< fmd::func<int>, int > );
///   static_assert( fmd::IsActualFormuloid< fmd::func<int>, bool, int> );
///   static_assert( ! fmd::IsActualFormuloid< fmd::func<int>, bool, char> );
/// \endcode
/// 
/// \ingroup grp_concept
template <typename T,typename... U>
concept IsActualFormuloid  =
  std::derived_from<T, formuloid_root> && MatchAnyBasalType<T,U...> ;

} // of namespace fmd

#include "bits/support.h"

//#include "bits/locinfo.h"
#include "bits/traits.h"
#include "bits/element.h"
#include "bits/exceptions.h"
#include "bits/context.h"
#include "bits/node.h"
#include "bits/args.h"
#include "bits/typename.h"
#include "bits/compute.h"

namespace fmd {
  

/// \brief The concept \c fmd::IsFormuloid<T> is satisfied if and only if type T
///        represents a formuloid. 
///
/// \tparam T    the tested type.
///
/// \tparam U... when set, an additionnal constraint is
///              that the basal type of T should match the basal
///              type of at least one U.
///
/// Example: Typical use as a type trait
/// \code
///   // Without basal constraint
///   static_assert( fmd::IsFormuloid< int > );
///   static_assert( fmd::IsFormuloid< fmd::expr<int> > );
///   static_assert( fmd::IsFormuloid< fmd::func<int> > );
///   static_assert( !fmd::IsFormuloid< const int > );
///   static_assert( !fmd::IsFormuloid< int[4] > );
///   static_assert( !fmd::IsFormuloid< int& > );
///   // With basal constraint
///   static_assert( fmd::IsNonBasalFormuloid< fmd::func<int>, int > );
///   static_assert( fmd::IsNonBasalFormuloid< fmd::expr<int>, int, double > );
///   static_assert( fmd::IsNonBasalFormuloid< fmd::func<int>, double, fmd::expr<int> > );
///   static_assert( !fmd::IsNonBasalFormuloid< int, char > );
///   static_assert( !fmd::IsNonBasalFormuloid< fmd::func<int>, double, char > );
/// \endcode
///
/// Example: Typical use in template definition
/// \code
///  template<
///     fmd::IsFormuloid            T1,  // T1 can be any formuloid 
///     fmd::IsFormuloid<int>       T2,  // T2 is a formuloid of basal type 'int' 
///     fmd::IsFormuloid<bool,char> T3,  // T3 is a formuloid of basal type 'bool' or 'char' 
///     fmd::IsFormuloid<T3>        T4,  // T4 is a formuloid with same basal type than T3.
///     fmd::IsFormuloid<T3,double> T5   // T5 is a formuloid with same basal type than T3 or double
///  >
///  void foo( const T1 &, const T2 &, const T3 &, const T4 &, const T5 &) ;
/// \endcode
///
/// \ingroup grp_concept
template<typename T, typename... U>
concept IsFormuloid =
  ( IsActualFormuloid<T> || IsBasalFormuloid<T> )
  && MatchAnyBasalType<T,U...> 
  ;

/// \brief The concept \c fmd::IsConvertibleFormuloid<T> is satisfied if and only if
///        type T is a formuloid whose basal type is convertible to the basal formuloid U.
///
/// \tparam T    the tested type.
/// \tparam U    the destination type.
///
/// TODO: provide a few examples.
///
/// \ingroup grp_concept
template <typename T, typename U>
concept IsConvertibleFormuloid
=  IsFormuloid<T> && IsBasalFormuloid<U> && std::convertible_to<fmd::basal_type_of<T>,U>
  ;


//
// A few convenient traits to test multiple formuloids at once.
//
// Some of them are probably never going to be useful but they are all given
// in order to remain consistent. 
//

template<typename... Ti> static constexpr bool all_formuloid_v       = ( ... && fmd::IsFormuloid<Ti> ) ;
template<typename... Ti> static constexpr bool all_basal_formuloid_v = ( ... && fmd::IsBasalFormuloid<Ti> ) ;
template<typename... Ti> static constexpr bool all_actual_formuloid_v = ( ... && fmd::IsActualFormuloid<Ti> ) ;

template<typename... Ti> static constexpr bool any_formuloid_v        = ( ... || fmd::IsFormuloid<Ti> ) ;
template<typename... Ti> static constexpr bool any_basal_formuloid_v  = ( ... || fmd::IsBasalFormuloid<Ti> ) ;
template<typename... Ti> static constexpr bool any_actual_formuloid_v = ( ... || fmd::IsActualFormuloid<Ti> ) ;

///
/// fmd::get_node(f) provides the fmd::node<U> associated to the formuloid f.
///
/// \ingroup grp_formuloid_api
template <IsFormuloid T>
inline auto get_node(const T &f)
{
  if constexpr ( IsBasalFormuloid<T> ) {
    return fmd::node<T>(f);
  } else {
    return f.get_node();
  }
}


/// Evaluate the content of any formuloid.
///
/// In practice, this is equivalent to operator [] and the eval() member
/// of the fmd::formuloid class except that fmd::evaluate can be safely
/// applied to all formuloids, including basal formuloids.
///
/// \ingroup grp_actual
template <IsFormuloid T>
inline auto
evaluate(const T &f, double p=0.0)
{
  if constexpr ( IsBasalFormuloid<T> ) {
    return f;
  } else {
    // an actual formuloid
    fmd::context ctxt(p);
    return fmd::get_node(f).eval(ctxt);
  } 
}


/////////////

} // of namespace fmd


// Features declared before "bits/formuloid.h" cannot
// instantiate any fmd::expr<T> and fmd::func<T> which 
// in practice means that T should be an unknown 
// type (i.e. not yet instanciated).
//
#include "bits/formuloid.h"
#include "bits/easing.h"
#include "bits/make.h"
#include "bits/scheme.h"
#include "bits/operators.h"
#include "bits/math.h"
#include "bits/apply.h"
#include "bits/identity.h"
#include "bits/select.h"
#include "bits/join.h"
#include "bits/transit.h"
#include "bits/morph.h"
#include "bits/lerp.h"

#include "bits/cast.h"

#include "bits/algorithm.h"

#include "bits/let.h"

// Starting from now, fmd::expr and fmd::func are fully defined

#include "bits/bezier.h"
#include "bits/waves.h"

#include "bits/pretty.h"

#include "bits/func.h"
#include "bits/cyclic.h"
#include "bits/instantiation.h"

#endif
