namespace fmd {


/// \brief Compute the linear interpolation between \c a and \c b 
///
/// The formuloid version is optimized to avoid evaluating \c a and \c b when
/// possible. The actual semantic is given by the following pseudo code:
///
/// \code   
///   if (t==0.0) ;
///     return a;
///   else if (t==1.0)
///     return b;
///   else
///     return std::lerp(a,b,t);
/// \endcode
///
/// \ingroup grp_func
template  < IsFormuloid<double> T1,
            IsFormuloid<double> T2,
            IsFormuloid<double> T3>
inline auto
lerp( const T1 &a,
      const T2 &b,
      const T3 &t)
{
  if constexpr (fmd::all_basal_formuloid_v<T1,T2,T3>) {
    return std::lerp(a,b,t) ;
  } else {
    return make_formuloid_cnt( "lerp",
                              fmd::compute::cnt_lerp<double> ,
                              a,b,t) ;    
  }
}

/// \brief Compute the clamped linear interpolation between \c a and \c b
/// 
/// The formuloids \c a and \c b are only evaluated when needed.
///
/// The actual semantic is given by the following pseudo code:
///
/// \code  
///   if (t<=0.0)
///     return a;
///   else if (t>=1.0)
///     return b;
///   else
///     return std::lerp(a,b,t);
/// \endcode
///
/// \ingroup grp_func
template  < IsFormuloid<double> T1,
            IsFormuloid<double> T2,
            IsFormuloid<double> T3>
inline auto
clerp( const T1 &a,
       const T2 &b,
       const T3 &t)
{
  if constexpr (fmd::all_basal_formuloid_v<T1,T2,T3>)
  {
    if (t<=0.0)
      return a;
    else if (t>=1.0)
      return b;
    else
      return std::lerp(a,b,t);
  }
  else
  {  

    return make_formuloid_cnt( "lerp_clamp",
                              fmd::compute::cnt_lerp_clamp<double>,
                              a,b,t) ;

  }
}


} // of namespace fmd
