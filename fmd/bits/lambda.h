
namespace fmd::compute {

enum mf_scheme {
  mf_scheme_v,
  mf_scheme_pv,
  mf_scheme_cnt 
} ;

// Implement a formuloid node from a compute object.
template <mf_scheme SCHEME,
          typename T,
          typename PRINTER,
          typename COMPUTE,
          typename... ARGS>
class OperLambda : public node_impl<T>
{ 
public:

  typedef fmd::node_tuple<ARGS...> storage_type ;
  storage_type m_args;
  PRINTER      m_printer;
  COMPUTE      m_compute;
  
  OperLambda( PRINTER printer,
              COMPUTE compute,
              const node<ARGS> & ...a ) :
    m_args(a...),
    m_printer(printer),
    m_compute(compute)
  {    
  }
  
  virtual T
  evaluate(context &ctxt) override
  {
    if constexpr ( SCHEME==mf_scheme_v ) {
      return std::apply(
        [this,&ctxt]<typename... U>(node<U> & ...arg) -> T {
          return m_compute( arg.eval(ctxt) ... );
        },      
        m_args) ;
    } 
    if constexpr ( SCHEME==mf_scheme_pv ) {
      return std::apply(
        [this,&ctxt]<typename... U>(node<U> & ...arg) -> T {          
          return m_compute( ctxt.read_param(), arg.eval(ctxt) ... );
        },      
        m_args) ;
    }
    if constexpr ( SCHEME==mf_scheme_cnt ) {      
      return m_compute(ctxt, m_args) ;
    }
  }

  
 bool write_formula( std::ostream &out ) override
  {
    if constexpr ( std::same_as<PRINTER, const char*> ||
                   std::same_as<PRINTER, std::string> )      
    {
      return this->write_formula_with_name(out,m_printer);
    }

    if constexpr ( std::is_invocable_r_v<const char*, PRINTER> ||
                   std::is_invocable_r_v<std::string, PRINTER> ||
                   std::is_invocable_r_v<std::string&, PRINTER> ||
                   std::is_invocable_r_v<const std::string&, PRINTER>
      )
    {
      return this->write_formula_with_name(out,m_printer());
    }    

    if constexpr ( std::invocable<PRINTER, std::ostream &, storage_type &> ) {
      return m_printer(out, m_args);
    }
    
    if constexpr ( std::invocable<PRINTER, std::ostream &> ) {
      out << "(" ;
      m_printer(out);
      write_node_tuple(out, m_args);
      out << ")" ;
      return true;
    }
    
  }

private:

  // Here, NAME is either a char pointer or a 'std::string'
  template <typename NAME>
  inline bool write_formula_with_name(std::ostream &out, const NAME &name)
  {    
    if (sizeof...(ARGS)==0 && name[0]=='@' ) { 
      // It is recommanded that active formuloids have a name 
      // beginning with @. If they do not have arguments,
      // then we can drop the parenthesis.
      out << name ;
      return false;
    } else {
      out << "(" << name;
      write_node_tuple(out, m_args);
      out << ")";
      return true;
    }      
  }

};

} // of namespace fmd::compute


namespace fmd {

//
// fmd::make_formuloid_v(name, compute, [arg, ...] )
// fmd::make_formuloid_pv(name, compute, [arg, ...] )
// fmd::make_formuloid_cnt(name, compute, [arg, ...] )
//
// Those helper functions simplify the creation of formuloid
// operations with an arbitrary number of arguments (also
// formuloids).
//
// Their 'compute' argument is a callable (of templated
// type COMPUTE) that specifies how to evaluate the operation.
// 
// The suffix indicates how the 'compute' object will be invoked,
// and so how it should be declared.
// 
// Let's assume that T1, ...Tn are the basal types of the arguments.
// Those arguments are stored by the operation node in a tuple
// of fmd::node:
// 
//   std::tuple<fmd::node<T1>,...,fmd::node<Tn>>  args ;
// or 
//   fmd::node_tuple<T1,T2,...,Tn> args ;
//
// The suffix indicates various schemes for 'compute'. 
// 
// - '_v' stands for 'values'.
//
//     'COMPUTE' should respect
//
//         std::invocable< compute, T1, T2, ... , Tn > 
//
//     in order to be called as    
//
//         result = compute( std::get<0>(args).eval(ctxt),
//                           std::get<1>(args).eval(ctxt),
//                           ...
//                           std::get<n-1>(args).eval(ctxt));
//
//     See also make_formuloid_v for additional information
//
//
// - '_pv' stands for 'parameter, values'.
//
//     This is similar to the 'v' scheme except that the
//     current parameter is passed first:
//    
//     'COMPUTE' should respect
//
//         std::invocable< COMPUTE,
//                         fmd::param_t,
//                         T1, T2, ... , Tn
//                         > 
//
//     in order to be called as    
//
//       result = compute( ctxt.read_param(),
//                         std::get<0>(args).eval(ctxt),
//                         std::get<1>(args).eval(ctxt),
//                         ...
//                         std::get<n-1>(args).eval(ctxt));
//
//     See also make_formuloid_pv for additional information
//
// - '_cnt' stands for 'context, node_tuple'.
//
//      'COMPUTE' should respect
//
//          std::invocable< COMPUTE,
//                          fmd::context &,
//                          fmd::node_tuple<T1,T2,...,Tn> &
//                          >
//
//      in order to be called as
//     
//          result = compute(ctxt, args);
//
//     See make_formuloid_cnt for additional information
//

/// \brief Create a formuloid using the 'v' scheme.
/// \ingroup grp_support
///
/// When all arguments are basal formuloid (i.e. values) then the
/// compute object is immediately invoked to produce a basal value.
/// If ALLOW_BASAL is true then that basal value is returned otherwise
/// an actual formuloid is returned.            
///
///
template < bool          ALLOW_BASAL=true,
           typename      PRINTER,
           typename      COMPUTE,
           IsFormuloid... TARGS >
requires ( std::invocable<COMPUTE, fmd::basal_type_of<TARGS> ...> ) 
inline auto
make_formuloid_v( PRINTER printer,
                 COMPUTE compute,
                 const TARGS & ...arg )
{
  using namespace fmd::compute;
  using TOUT = std::invoke_result_t<COMPUTE, fmd::basal_type_of<TARGS> ...> ;
  static_assert( IsBasalFormuloid<TOUT> ) ;
  
  if constexpr ( fmd::all_basal_formuloid_v<TARGS...> ) {
    if constexpr (ALLOW_BASAL)
      return compute(arg...) ;
    else
      return to_expr(compute(arg...)); 
  } else {
    using IMPL = OperLambda<mf_scheme_v,
                            TOUT,
                            PRINTER,
                            COMPUTE,
                            fmd::basal_type_of<TARGS>...
                            > ;
    return fmd::emplace_formuloid<IMPL>( printer, compute, fmd::get_node(arg)... );
  } 
}


/// \brief Create a formuloid using the 'pv' scheme.
/// \ingroup grp_support
///
/// In that scheme, the formuloid parameter is always evaluated 
/// so the result is necessarily an actual formuloid
///
template < typename      PRINTER,
           typename      COMPUTE,
           IsFormuloid... TARGS >
requires ( std::invocable<COMPUTE, double, fmd::basal_type_of<TARGS>  ...> ) 
inline auto
make_formuloid_pv( PRINTER printer,
                  COMPUTE compute,
                  const TARGS & ...arg )
{
  using namespace fmd::compute;
  using TOUT = std::invoke_result_t<COMPUTE, double, fmd::basal_type_of<TARGS> ...> ;
  static_assert( IsBasalFormuloid<TOUT> ) ;  
  using IMPL = OperLambda<mf_scheme_pv,
                          TOUT,
                          PRINTER,
                          COMPUTE,
                          fmd::basal_type_of<TARGS>... > ;
  return fmd::emplace_formuloid<IMPL>( printer, compute, fmd::get_node(arg)... );
}

/// \brief Create a formuloid using the 'cnt' scheme.
/// \ingroup grp_support
///
/// This is the most low-level scheme. It should be used with
/// great care since an incorrect handling of the 'fmd::context'
/// could cause some incorrect optimisations of the formuloid
/// formula (e.g. such as replacing a non-constant sub-formula
/// by a constant value).
///
template < typename       PRINTER,
           typename       COMPUTE,
           IsFormuloid...  TARGS >
requires ( std::invocable<COMPUTE,
                          fmd::context &,
                          fmd::node_tuple<fmd::basal_type_of<TARGS>...> &
                         > )
inline auto
make_formuloid_cnt( PRINTER printer,
                   COMPUTE compute,
                   const TARGS & ...arg)
{
  using namespace fmd::compute;
  using TOUT = std::invoke_result_t<COMPUTE, fmd::context&, fmd::node_tuple<fmd::basal_type_of<TARGS>...>&> ;
  static_assert( IsBasalFormuloid<TOUT> ) ;  
  using IMPL = OperLambda<mf_scheme_cnt,
                          TOUT,
                          PRINTER,
                          COMPUTE,
                          fmd::basal_type_of<TARGS>...
                          > ;
  return fmd::emplace_formuloid<IMPL>( printer, compute, fmd::get_node(arg)... );
}


} /// of namespace fmd
