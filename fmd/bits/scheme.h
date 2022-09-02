#if 0
namespace fmd {

// 
// This is an API to implement formuloid operations from
// a scheme (i.e. a class that provides all the required
// informations).
// 
// The body of a scheme class must start by a call to the macro
// FMD_SCHEME_MEMBERS to specify the operation name, its
// output type, and a bitmask that provides additional
// information (see 
//
// The scheme should then specify a storage type for the
// operation arguments. This is typically done using
// FMD_SCHEME_STORAGE_TUPLE or FMD_SCHEME_STORAGE_ARRAY
// (when all arguments have the same type). 
// 
// Each scheme_enum semantic also imposes its own requirements
// on the members required in the scheme class (see below).
//
// With a propoer scheme class, new operation nodes can be
// allocated by calling fmd::make_from_scheme<SCHEME>(...).
//
// The result is a fmd::node so a proper wrapper still need
// to be provided to transform it into a fmd::expr or a
// fmd::func.
//

// Properties to use in the PROPS bitmask of scheme_base

namespace scheme_prop
{
  // Indicate that the node reads the parameter value.
  // An obvious side effect is that this scheme can only be used to produce std::func. 
  static constexpr  unsigned read_param           = 1u<<0;

  // Indicate that the scheme provides a member 'compute'.
  // Its result type should be the type.
  // If read_param is also set then the first argument
  // is the parameter value and so shall be of type
  // param_t (i.e. double).
  // The remaining arguments are the values of the
  // operation arguments. 
  static constexpr  unsigned provide_compute      = 1u<<1;

  // Indicate that the scheme provides a member 'evaluate'.
  // The first argument is a reference to the current context.
  // The second argument is a reference to a storage_type  
  // providing the operation arguments (typically a std::tuple
  // or a std::array of nodes).
  static constexpr  unsigned provide_evaluate     = 1u<<2;
} ;


#define FMD_SCHEME_INFO(NAME, RESULT, PROPS) \
  typedef RESULT result_type; \
  static constexpr bool read_param       = (PROPS) & fmd::scheme_prop::read_param; \
  static constexpr bool provide_compute  = (PROPS) & fmd::scheme_prop::provide_compute; \
  static constexpr bool provide_evaluate = (PROPS) & fmd::scheme_prop::provide_evaluate; \
  static constexpr const char * name = NAME;

#define FMD_SCHEME_STORAGE_TUPLE(...) \
  typedef fmd::node_tuple<__VA_ARGS__> storage_type; \
  static constexpr size_t narg = std::tuple_size<storage_type>::value 

#define FMD_SCHEME_STORAGE_ARRAY(TYPE,NB) \
  typedef std::array<TYPE,NB> storage_type; \
  static constexpr size_t narg = NB


namespace internal {

template <typename SCHEME>
class OperFromScheme : public node_impl<typename SCHEME::result_type> {
public:

  static constexpr int narg = SCHEME::narg ;
  
  using result_type      = typename SCHEME::result_type ;
  using storage_type     = typename SCHEME::storage_type ;
    
  storage_type m_args ;

  // Is there a way to verify that the node types are matching the scheme?
  // Hopefully, the initialization of m_args will fail. 
  template <typename... ARGS>
  OperFromScheme(const node<ARGS> & ...a) :
    m_args(a...)
  {
  }  
  
  virtual result_type evaluate(context &ctxt) override
  {
    static_assert(SCHEME::provide_evaluate || SCHEME::provide_compute ,
                  "Scheme should provide either 'evaluate' or 'compute'");

    if constexpr ( SCHEME::provide_evaluate ) {
      // Always privilegiate 'evaluate' over 'compute' since it may 
      // provide some optimizations (e.g. not evaluating all arguments) 
      return SCHEME::evaluate(ctxt, m_args) ;
    } else if constexpr ( SCHEME::read_param ) {
      // Call 'compute' with the parameter as first argument  
      fmd::param_t param = ctxt.read_param(); 
      return std::apply(
        [&]<typename... T>(node<T> & ...arg) -> result_type
        {
          return SCHEME::compute( param, arg.eval(ctxt) ... );
        },      
        m_args) ;
    } else {
      // Call 'compute' without the parameter as first argument
      return std::apply(
        [&]<typename... T>(node<T> & ...arg) -> result_type
        {
          return SCHEME::compute(arg.eval(ctxt) ... );
        },      
        m_args) ;
    }
  }

  bool write_formula( std::ostream &out ) override
  {
    out << "(";
    if constexpr ( std::is_function_v<decltype(SCHEME::name)> ) {
      out << SCHEME::name() ;
    } else {
      out << SCHEME::name;
    }
    write_node_tuple(out, m_args);
    out << ")";
    return true;
  }

//  const char * name() final { return SCHEME::name ; } 
};

} // of namespace internal

template <typename SCHEME,
          typename... TARGS>
requires ( sizeof...(TARGS) > 0 )
inline auto
make_from_scheme( const TARGS & ...arg )
{
  if constexpr ( fmd::all_basal_formuloid_v<TARGS...> 
                 && SCHEME::provide_compute 
                 && !SCHEME::read_param ) {
    return SCHEME::compute(arg...) ;
  } else {
    using IMPL = OperFromScheme<SCHEME>;
    constexpr bool tf = SCHEME::read_param || fmd::any_true_formuloid_v<TARGS...> ;
    return fmd::emplace_func<IMPL>( fmd::get_node(arg)... ) ;   
  } 
}


} // of namespace fmd

#endif
