
namespace fmd {

/// \brief Conditionnal formuloid selection
///
/// Eventually, 'fmd::select' could be extended to accept an integer index as first argument
/// followed by a parameter pack of formuloids of the same basal type. The new semantic would
/// would be that 'fmd::select(i,v0,v1,...,vn)' returns the element 'vi' if i is between 0 and n
/// or 'vn' otherwise.  
///
/// \ingroup grp_func
template <
  IsConvertibleFormuloid<bool> T1,
  IsFormuloid     T2,
  IsFormuloid<T2> T3
>
inline auto
select( const T1 & cond, const T2 & true_value , const T3 & false_value )
{
  using C = fmd::basal_type_of<T1> ;
  using U = fmd::basal_type_of<T2> ;

  if constexpr ( fmd::all_basal_formuloid_v<T1,T2,T3> ) {
    return cond ? true_value : false_value ;
  } else if constexpr ( IsBasalFormuloid<T1> ) {
    return fmd::to_formuloid( cond ? true_value : false_value ) ;
  } else {
    return make_formuloid_cnt( "select",
                              fmd::compute::cnt_select<C,U>,
                              cond, true_value, false_value );

  }
} 


};
