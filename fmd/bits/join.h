namespace fmd {


/// \brief Join 2 or more formuloids at the given positions.
///
/// \param f1, f2, ... are formuloids of the same basal type.
/// \param pos1 ... are false formuloid of basal type \c double.
/// \param more  should be an alternating sequence of pairs
///              \c pos_i and \c fun_i.
///
/// \result A fmd::func of the same basal type than all \c fun_i 
///
/// It is assumed but not verified that all positions are given
/// in increasing order. Doing otherwise is not illegal but is
/// unlikely to produce the expected outcome.
///
/// \code
///    result = fmd::join(fun1,
///                       pos1, fun2,
///                       pos2, fun3,
///                       pos3, fun4) ;
/// \endcode
/// is equivalent to
/// \code
///    auto tmp3 = fmd::select(fmd::id < pos3, fun3, fun4);
///    auto tmp2 = fmd::select(fmd::id < pos2, fun2, tmp3);
///    result = fmd::select(fmd::id < pos1, fun1, tmp2);
/// \endcode
///
/// \ingroup grp_func
template <
  std::same_as<double> TP,
  IsFormuloid      T1,
  IsFormuloid<T1>  T2,
  typename...     TMORE
>
requires (sizeof...(TMORE)%2==0)
inline auto
join( const T1 & f1 , const TP & pos1 , const T2 & f2  , const TMORE & ...more)
{
  if constexpr ( sizeof...(TMORE) == 0 ) {
    return fmd::select( fmd::id < pos1 , f1, f2 );
  } else {
    return fmd::select( fmd::id < pos1 , f1, fmd::join(f2, more...) );
  }
}

};
