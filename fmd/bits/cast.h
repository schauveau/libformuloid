
namespace fmd::compute {


/// \brief Functor used to implement fmd::cast
/// \ingroup grp_compute
template <typename TOUT, typename TIN>
auto v_cast =
  [](TIN a) -> TOUT {
    return TOUT(a);
  };

/// \brief Provide the name used to implement fmd::cast
/// \ingroup grp_compute
template <typename TOUT>
inline auto printer_cast =
  []() {
    return std::string("cast<") + fmd::internal::get_typename<TOUT>() + ">" ;
  };

} // of namespace fmd::compute

namespace fmd {


/// \cond SEPARATE_DOC

// This function is documented separately in doc/function.dox
template < IsBasalFormuloid TOUT,
           IsFormuloid      TIN
           >
requires (std::is_convertible_v< fmd::basal_type_of<TIN> ,TOUT>) 
auto
cast(const TIN &arg ) {
  if constexpr ( IsBasalFormuloid<TIN> ) {
     return TOUT(arg) ;
  } else {
    using U = typename fmd::basal_type_of<TIN> ;
    return fmd::make_formuloid_v( fmd::compute::printer_cast<TOUT>,      
                                 fmd::compute::v_cast<TOUT,U>,
                                 arg );
  }
}

/// \endcond

} // of namespace fmd
