
namespace fmd {


/// \brief Apply a parameter to a formuloid

/// \ingroup grp_func
template < IsFormuloid         T1,
           IsFormuloid<double> T2
          >
inline auto 
apply(const T1 &f, const T2 &p)
{
  if constexpr ( IsBasalFormuloid<T1> ) {
    // Basal formuloid do not use their parameter so this is basically a no-op.
    return f ;
  } else {    
    using RT = fmd::basal_type_of<T1> ;
    using IMPL = fmd::compute::OperApply<RT> ;
    return fmd::emplace_formuloid<IMPL>( fmd::get_node(f), fmd::get_node(p)) ;   
  }
};


}; // of namespace fmd
