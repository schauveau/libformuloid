
namespace fmd {


/// A convenient alias for a std::tuple containing only fmd::node<T>.
/// 
/// For example, \c fmd::node_tuple<int,double,char>
/// is equivalent to \c std::tuple<fmd::node<int>,fmd::node<double>,fmd::node<char>>
///
template <typename... TARGS> using node_tuple = std::tuple<node<TARGS>...> ; 

/// \brief Write the formulas for an empty sequence of nodes
///
/// \tparam p if true then insure that at least one space is printed 
/// \param s is the stream to write to.
/// 
/// \ingroup grp_support
template <bool p=true>
std::ostream &
write_nodes(std::ostream &s) 
{
  if constexpr (p) {
    s << " ";
  } 
  return s;
}

/// \brief Write the formulas for a non-empty sequence of nodes
///
/// \tparam p if true then insure that at least one space is printed.
///         This is not used by this specific version of fmd::write_nodes. 
/// \param s is the stream to write to.
/// \param node is the first node to process
/// \param ...more are the other nodes to process
///
/// \ingroup grp_support
template <bool p=true, typename T, typename... MORE>
std::ostream &
write_nodes( std::ostream &s, fmd::node<T> &node, fmd::node<MORE> & ...more) 
{
  s << " " ;
  node.write_formula(s) ;
  write_nodes<false>(s, more...);
  return s;
}

/// \brief Write the formulas for a tuple of nodes
///
/// \param s is the stream to write to.
/// \param nt the tuple whose content shall be printed
/// \tparam p if true then insure that at least one space is printed
///           for an empty tuple. 
///
/// The formulas are separated by spaces. 
///
/// \ingroup grp_support
template <bool p=true, typename... TARGS>
std::ostream &
write_node_tuple( std::ostream &s, fmd::node_tuple<TARGS...> & nt)
{  
  std::apply(
    [&s](auto&&... nodes)  {
      fmd::write_nodes<p>(s, nodes...) ; 
    }
    , nt );
  return s;
}

namespace internal {

template <typename T, typename... TARGS> 
struct OperArgsTuple : public node_impl<T>
{  
  // static constexpr int narg = sizeof...(TARGS);
  OperArgsTuple(const node<TARGS> & ... a) : m_args(a...)
  {
  }

  using args_storage_type = fmd::node_tuple<TARGS...> ;
  args_storage_type  m_args ;
  
  // Remark: Make narg visible by default in the current scope. This is unfortunately not
  //         possible for all 'arg' members because they may not exist. 
  static constexpr size_t narg = sizeof...(TARGS);

  bool write_formula( std::ostream &out ) override {
    out << "(" << this->name() ;
    if constexpr ( sizeof...(TARGS) > 0 ) {
      out << " " ;
      fmd::write_node_tuple(out, m_args) ;      
    }
    out << ")";
    return true;
  }

  // Provide the operator name (for write_formula and debug)
  virtual const char *name()=0 ; 

  typedef std::function<T(const TARGS &...)>  eval_function_type ;

protected:

  // A wrapper around std::apply to call a 'compute' function
  // with arguments of types TARGS... and result type T. 
  //
  // The argument values are all evaluated using the same context ctxt.  
  //
  // TODO: make use of std::forward for performance? 
  //
  template <typename F>
  inline T eval_all_and_apply( context &ctxt, F &compute ) {
    return std::apply(
      [&compute,&ctxt]<typename... U>(node<U> & ...arg) -> T
      {
        return compute( arg.eval(ctxt) ... );
      },      
      this->args) ;
  }

  // Similar to eval_all_and_apply but also pass the context
  // parameter as argument.  
  template <typename F>
  inline T eval_all_and_apply_with_param( context &ctxt, F &compute ) {
    return std::apply(
      [&compute,&ctxt]<typename... U>(node<U> & ...arg) -> T
      {
        return compute( ctxt.read_param(), arg.eval(ctxt) ... );
      },      
      this->args) ;
  }
  
};

} // of namespace internal

} // of namespace fmd
