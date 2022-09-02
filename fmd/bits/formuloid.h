
namespace fmd {

/// \brief The main implementation for actual formuloids. 
///
/// T is the associated formuloid basal type.
///
/// Operations and functions that manipulate formuloids are typically
/// implemented using fmd::formuloid while end-user codes are more likely
/// to used derived classes such as fmd::func or fmd::cycle.
/// 
template <IsBasalFormuloid T>
class formuloid : public formuloid_root 
{
public:

  typedef T basal_type;
  
protected:
  
  mutable node<T> m_node;
  
  inline void assign( const fmd::formuloid<T> & in) {
    m_node = in.m_node;
  }
  
  inline void assign( fmd::formuloid<T> && in) {
    m_node = std::move(in.m_node);
  }
  
  inline void assign( const fmd::node<T> & in) {
    m_node = in;
  }
  
  inline void assign( fmd::node<T> && in) {
    m_node = std::move(in);
  }
  
  inline void assign( const T & value ) {
    m_node = value;
  }

public:

  /// \TBD  
  formuloid()
    : m_node()
  {
  }
  
  /// \TBD  
  explicit formuloid(const T &v)
    : m_node(v)
  {
  }

  /// \TBD
  explicit formuloid(const node<T> &op)
    : m_node(op)
  {
  }


  /// \TBD
  explicit formuloid(node<T> &&op)
    : m_node(std::move(op))
  {
  }


  /// \TBD
  template <IsNodeImpl  IMPL, 
            typename... Args
            >
  requires ( std::is_same_v< typename IMPL::basal_type , T > )
  static inline auto  
  emplace( Args&&... args )
  {
    return fmd::formuloid<T>( fmd::emplace_node<IMPL>(std::forward<Args>(args)...) ) ;
  }

  
  /// \brief \c f.apply(p) is an shortcut for fmd::apply(f,p)  
  template <IsFormuloid<double> T1>
  inline auto
  apply(const T1 &p) const
  {    
    using IMPL = fmd::compute::OperApply<T> ;
    return fmd::formuloid<T>::emplace<IMPL>( m_node, fmd::get_node(p)) ;   
  }
 
  /// \brief f(p) is a shortcut for fmd::apply(f,p)  
  template <IsFormuloid<double> T1>
  inline auto
  operator()(const T1 &p) const
  {
    return this->apply(p);
  }

  /// \brief Evaluate the formuloid for a given parameter p.
  inline T
  operator[](double p)
  {
    return fmd::evaluate(*this, p);
  }

  /// \brief Evaluate the formuloid for a given parameter p.
  inline T
  operator[](double p) const
  {
    return fmd::evaluate(*this, p);
  }

  /// \brief Evaluate the formuloid for a given parameter p.  
  inline T
  eval(double p)
  {
    return fmd::evaluate(*this, p);
  }

  /// \brief Evaluate the formuloid for a given parameter p.
  inline T
  eval(double p) const
  {
    return fmd::evaluate(*this, p);
  }
  
  /// \TBD
  void
  write_formula( std::ostream &out) const
  {    
    fmd::current_frame_id=0 ;       
    m_node.write_formula(out);   
  }
  
  /// Required by fmd::get_node() 
  const auto & get_node() const {
    return m_node;
  }
  
};

/// \ingroup grp_operators
///
/// operator<< to ostream is only provided for fmd::formuloid because
/// other actual formuloid implementations may have different requirements.
/// 
template <typename T> 
std::ostream & operator<<(std::ostream &out, const formuloid<T> &a)
{
  a.write_formula(out);
  return out;
}


/// \brief Create a fmd::formuloid<U> from a node implementation.
/// 
/// \tparam U is the basal type of the result. It is typically
///         inferred from the implementation argument.
///
/// \ingroup grp_support
template <IsBasalFormuloid U>
inline auto
to_formuloid( fmd::node_impl<U> *impl )
{
  return fmd::formuloid<U>( fmd::make_node(impl) ); 
}

/// \brief Convert any formuloid to a \c fmd::formuloid<U>.
/// \ingroup grp_support
template <IsFormuloid T>
inline auto
to_formuloid( const T & f )
{
  using U = typename fmd::basal_type_of<T> ; 
  return fmd::formuloid<U>(f) ;
}


/// \brief Construct a fmd::formuloid from a node implementation type.
///
/// \tparam  IMPL is a type that publically derive \c fmd::node_impl
/// \param   args are the arguments to IMPL constructor.
///
/// So
/// \code
///     auto result = emplace_formuloid<IMPL,TF>(arg1,...);
/// \endcode
/// is equivalent to
/// \code
///     using U = typename IMPL::basal_type;
///     fmd::node<U> n = fmd::emplace_node<IMPL>(arg1,...);
///     fmd::formuloid<U> result(n);
/// \endcode
///
/// \see fmd::emplace_node.
///
/// \ingroup grp_support
template <IsNodeImpl IMPL, 
          typename... Args
          >
inline auto
emplace_formuloid( Args&&... args )
{
  using U = typename IMPL::basal_type ;  
  return fmd::formuloid<U>::template emplace<IMPL>(std::forward<Args>(args)...) ;
}


///
/// Return true if the formuloid is obviously constant.
///
/// That property is used to optimized away some formuloids
/// when they can be replaced by a constant.
///
/// Basal formuloids are always obviously constant.
///
/// Actual formuloids are obviously constant when their formula
/// is composed of a single constant. For example, the
/// formula '3' is obviously constant while '(+ 1 2)' is not.
/// 
/// More complex cases may be detected in the future. 
///
/// \ingroup grp_support
template <IsFormuloid T>
inline bool
obviously_constant_formuloid(T x)
{
  if constexpr ( IsBasalFormuloid<T> ) {
    return true;
  } else {
    return fmd::get_node(x).is_pure_constant() ;     
  }
}


} // of namespace fmd 
