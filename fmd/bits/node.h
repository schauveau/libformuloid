

namespace fmd {


class walker {
public:
  virtual ~walker() {} 
  virtual void start() =0 ;
  virtual void enter_operation( size_t type_hash, const char *name, size_t narg) =0 ;
  virtual void leave_operation( size_t type_hash, const char *name, size_t narg) =0 ;
  virtual void process_value( size_t type_hash, int rank, void *data) =0 ;

  virtual void process_frame( ... ) =0 ;
  virtual void process_frame_ref( ... ) =0 ;
    
  virtual void stop() =0 ;  
};

/// \brief Common base type for all \c node_impl<T>
struct node_impl_base : public non_basal_type
{
private:
  int  refcount;
public:
  
  /// \brief Constructor  
  node_impl_base() : refcount(0) {
  }

  /// \brief destructor
  virtual ~node_impl_base()
  {
    if (refcount != 0) {
      std::cerr << "Non-zero refcount in operation() destructor\n";
      exit(1);
    }
  }


  /// \brief increase the reference counter 
  void increase_refcount()
  {    
    refcount++;
    assert(refcount>0); // detect overflow
  }

  /// \brief decrease the reference counter
  ///
  /// This object is deleted when the reference counter reaches 0.  
  /// 
  bool decrease_refcount()
  {
    assert(refcount>=0);
    refcount--;
    if (refcount==0) {
      delete this;
      return true;
    }
    return false; 
  }
   
  /// \brief Write the formula stored in that node to a stream using a lisp-like format.
  /// 
  /// \result true if the written formula starts with a parenthesis and false otherwise.
  /// 
  virtual bool write_formula(std::ostream &out) =0;

  
};

/// \TBD
template<typename T> 
std::ostream &
operator<<(std::ostream &out, node_impl_base &a)
{
  a.write_formula(out);
  return out;
}

/// \brief Base class for formuloid operation nodes
template <IsBasalFormuloid T>
class node_impl : public fmd::node_impl_base
{
public:
  
  ///< The basal type produced by this implementation
  using basal_type = T; 

  /// brief constructor
  node_impl() {}
  
  node_impl(const node_impl &) =delete ;

  /// \brief destructor
  virtual ~node_impl() {
  }

  /// \brief evaluate the node using the specified context
  virtual T evaluate(context &ctxt)=0 ;
};


namespace internal {

/// \brief Base class of all \c fmd::node<T>
struct node_root : public non_basal_type
{
  node_root() = default;
  virtual ~node_root() = default;
  virtual void write_formula(std::ostream &out) const =0;
}; 

} // of namespace internal


/// \brief a smart reference to a \c node_impl<T>
/// 
/// It should be used exclusively to insure that all
/// \c fmd::node_impl are properly reference counted.
///
template <IsBasalFormuloid T>
class node : public fmd::internal::node_root {
public:
  using basal_type = T;  ///< The basal type for the formuloid represented by this node
private:
  // TODO: The node contains either a node_impl pointer or a value so we store
  //       m_ptr and m_value in a std::variant.
  //       --> also remove std::default_initializable<T> from IsBasalFormuloid.
  mutable node_impl<T> * m_ptr;
  mutable T m_value;
  // Control weither we want to reset m_value to the default_value when it is not in use.
  static constexpr bool reset_unused_value = true;
  // This is the default value when a node is not explicitly initialized
  // Should we let the user have control?
  // static constexpr T default_value = T() ;
  // Control if constant 'nodes' must be optimized
  // during evaluation
  //   -> can be turned off to debug internal problems.
  static constexpr bool optimize_by_default = true;

private:

  node(node_impl<T> *ptr) :
    m_ptr(ptr),
    m_value()
  {
    if (ptr)
      ptr->increase_refcount() ;
  }

  // the caller shall insure that new_ptr is not null
  void
  assign_ptr(node_impl<T> *new_ptr) const
  {
    new_ptr->increase_refcount();
    node_impl<T> * old_ptr = m_ptr;
    if ( old_ptr ) {
      old_ptr->decrease_refcount();
    } else if (reset_unused_value) {
      m_value = T();
    }
    m_ptr = new_ptr;    
  }
  
  void
  assign_value(const T &v) const
  {
    if ( m_ptr ) {
      m_ptr->decrease_refcount() ;
      m_ptr=nullptr;
    }
    m_value = v ;
  }
  
public:

  /// \brief default constructor
  node() : m_ptr(nullptr) , m_value()
  {
  } 

  ~node()  {
    if (m_ptr) {
      m_ptr->decrease_refcount();
      m_ptr = nullptr;
    }
  }

  /// \brief construct from a basal value
  node(const T & value) :
    m_ptr(nullptr),
    m_value(value)
  {
  }

  /// \brief construct from another node
  node(const node<T> &from) :
    m_ptr(from.m_ptr),
    m_value(from.m_value)
  {
    if (m_ptr)
      m_ptr->increase_refcount();
  }

  // TODO: implement the move constructor : node(node<T> &&from) 
  
public:

  /// \TBD
  node<T> &
  operator=(const node<T> &from)
  {
    if ( &from != this ) {
      if (from.m_ptr) {
        this->assign_ptr(from.m_ptr);
      } else {
        this->assign_value(from.m_value);
      }
    }
    return *this;
  }

  // TODO: implement move assignement : node<T> & operator=(node<T> &&from)

  /// \TBD
  /// TODO: Should only use emplace_node
  static node<T> make(node_impl<T> *from)
  {
    return node(from);
  }
  
  /// \TBD
  T eval(context &ctxt, bool optimize=optimize_by_default) const
  {
    if (!m_ptr)
      return m_value ;

    if (!optimize) {
      return m_ptr->evaluate(ctxt);
    }
      
    context subctxt(ctxt) ;
    T result = m_ptr->evaluate(subctxt);
    ctxt.merge_same_param(subctxt);

    if ( !subctxt.is_mutable() && !subctxt.param_was_read() ) {
      // The result will never change so optimize to a pure constant. 
      this->assign_value(result);
    }
    return result;
  }
  
  /// \TBD
  void write_formula(std::ostream &out) const override
  {    
    if (m_ptr) {
      m_ptr->write_formula(out);
    } else {      
      if constexpr ( requires (std::ostream &out) { fmd::basal_simple_writer<T>::apply(out); } ) {
        // This is the default fallback version
        fmd::basal_simple_writer<T>::apply(out);
      } else {
        // This is a specialization
        fmd::basal_simple_writer<T>::apply(out,m_value);
      }
    }
  }

  /// \brief true if and only if the node represents a pure constant.
  bool is_pure_constant() {
    return !m_ptr ;
  }
};




/// \private
template<class T>
struct is_node : std::false_type {};

/// \private
template<IsBasalFormuloid T>
struct is_node<fmd::node<T>> : std::true_type {};

/// \brief true if type T is a \c fmd::node<U>.
/// \ingroup grp_concept
template<class T> constexpr bool is_node_v = is_node<T>::value ;


/// Helper to create a node<T> from an implementation 
///
/// This is typically used immediately after 'new' to start
/// 'ref-counting' the pointer.
///  
/// TODO: Should only use node_emplace
/// 
template <IsBasalFormuloid T>
node<T>
make_node( node_impl<T> *ptr)  {
  return node<T>::make(ptr) ;
}

/// Evaluate to true when type T is a proper node implementation.
///
/// That is any type publicly derived from a fmd::node_impl<U>.
///
/// \ingroup grp_concept
template <typename T>
concept IsNodeImpl = std::derived_from<T,fmd::node_impl_base> ;


/// \brief Construct a fmd::node from a node implementation type.
///
/// \tparam  IMPL is a type that publically derive \c fmd::node_impl.
/// \param   args are the arguments for the IMPL constructor.
///
/// So,
/// \code
///     auto result = emplace_node<IMPL>(arg1,...);
/// \endcode
/// is equivalent to
/// \code
///     using U = typename IMPL::value_type;
///     IMPL * impl_ptr = new IMPL(arg1,...);
///     fmd::node<U> result = fmd::make_node(impl_ptr);
/// \endcode
///
/// \see fmd::emplace_func
///
/// \ingroup grp_support
template< IsNodeImpl IMPL,
          typename... Args>
auto
emplace_node(Args&&... args)
{
  return make_node( new IMPL(std::forward<Args>(args)...) );
}

} // of namespace fmd
