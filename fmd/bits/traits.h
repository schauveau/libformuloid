
//
// Misc helper traits. Some may not be used at all. 
// 

namespace fmd {
 
/// \private
template <int RANK, int STEP, bool... VALUES> struct periodic_all_t;

/// \private
template <int RANK, int STEP, bool VALUE>
struct periodic_all_t<RANK,STEP,VALUE> {
  static constexpr bool value =  (RANK==0) ? VALUE : true ;
} ;

/// \private
template <int RANK, int STEP, bool VALUE, bool... MORE>
struct periodic_all_t<RANK, STEP, VALUE, MORE...> {
  static constexpr bool value = 
    periodic_all_t<RANK,STEP,VALUE>::value
    && periodic_all_t< (RANK+STEP-1)%STEP , STEP, MORE...>::value ;
} ;

/// \brief evaluates to true when a periodic subset of its arguments are all true.
///
/// \tparam RANK   is the starting rank of the periodic subset.
/// \tparam STEP   is the period of the subset. 
/// \tparam VALUES is the whole set of boolean values. 
///
/// \code
///    periodic_all_v<rank,step,v[0],v[1],v[2],...>
/// \endcode
/// is equivalent to
/// \code  
///    v[rank] && v[rank+step] && v[rank+2*step] && v[rank+3*step] ...
/// \endcode
/// 
/// Its primary purpose is to declare repeating conditions in a parameter pack. 
///
/// Example: Declaration of a parameter pack with repeating groups of 3 arguments:
///             - the 1st must be a double.
///             - the 2nd must be a int.  
///             - the 3rd must be either an integral type or a floating point type.
/// \code
///     template <typename... T>
///     requires ( (sizeof...(T)%3==0) &&
///                periodic_all_v<0,3, std::is_same_v<T,double> ...> &&
///                periodic_all_v<1,3, std::is_same_v<T,int> ...> &&
///                periodic_all_v<2,3, ( std::is_integral_v<T> || std::is_floating_point_v<T> ) ...> 
///                )
///     dump(std::ostream &out, T ...arg)
///     {
///        ... // do something 
///     } 
///
///     ...
/// 
///     dump(std::cout,
///         "pi", 1, std::numbers::pi,
///         "e",  1, std::numbers::e,
///         "INT_MIN", 4, std::numeric_limits<int>::min(),
///         "INT_MAX", 4, std::numeric_limits<int>::max()
///        ) ; 
/// \endcode
/// 
/// \ingroup grp_concept
template <int RANK,int STEP, bool... VALUES> constexpr bool periodic_all_v = periodic_all_t<RANK,STEP,VALUES...>::value ;


} // of namespace fmd


