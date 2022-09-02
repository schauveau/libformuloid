
namespace fmd {

namespace internal {

template <typename T> class OperFrameRef ;

struct binding_base {
  virtual ~binding_base() {};
  virtual bool write_ref(std::ostream &out) =0;
  virtual void release() =0;
};

} // of namespace internal

class frame_builder ;

/// \brief Hold a frame and its bindings.
///
/// That object is not directly constructible. Use \c fmd::frame_builder instead.
///
/// \see fmd::let 
///
class frame
{
private:

  friend class frame_builder;

private:
  fmd::context* m_context{0};
  size_t        m_mutable_counter{0};
  size_t        m_id{0}; // Used to uniquely identify the frame while printing  
  size_t        m_timestamp{0};

  // All references registered in the frame 
  std::vector<fmd::internal::binding_base *> m_bindings;

private:
  
  frame(frame &)=delete ;
  
  frame() {
  };

public:


  /// destructor
  virtual ~frame()
  {
    for ( auto &ref : m_bindings ) {
      ref->release(); 
      ref = nullptr;
    }
  }

  /// Provide the context that was used to open the frame.   
  fmd::context & context()
  {
    return *m_context;
  }

  /// Provide a timestamp that changes every time the frame is opened.
  size_t timestamp() {
    return m_timestamp;
  }

  /// While priting formuloid formulas, provide a value that uniquely
  /// match a frame to its bindings.  
  size_t print_id() {
    return m_id; 
  }
  
  /// Record a mutable event caused by accessing one of the frame bindings 
  void record_mutable_event() {
    m_mutable_counter++;
  }
  
  /// write the bindings separated by spaces.
  ///
  /// This is typically used when a frame in encountered while printing a formuloid formula.
  ///
  void write_bindings(std::ostream &out)
  {
    // Reminder: the frames are renumbered every time.
    m_id = ++fmd::current_frame_id;
    bool first = true;
    for (auto *ref : m_bindings ) {
      if (!first)
        out << " ";
      ref->write_ref(out);      
      first=false;
    }
  }

  /// Start using the frame in the given context  
  virtual void open(fmd::context &ctxt) {
    // TODO: throw exception if the frame is already opened
    this->m_timestamp++;
    this->m_context = &ctxt;
    this->m_mutable_counter=0;
  };

  /// Stop using the frame in the given context 
  virtual void close(fmd::context &ctxt) {
    // TODO: throw exception if the frame is not opened
    ctxt.discard_mutable_events(this->m_mutable_counter);
    this->m_context = nullptr;
    this->m_mutable_counter=0;
  };
  
};


/// \brief Build a single frame
///
/// This is typically used to build a frame for fmd::let().
///
/// Each instance of \c frame_builder can only build a single non-reusable frame.
///
class frame_builder
{
private:                            
  fmd::frame *m_frame;

  void check() {
     if (!m_frame) {
      throw std::invalid_argument("frame_builder is already grabbed");
    }
  }
  
public:

  /// constructor
  frame_builder() :
    m_frame(new fmd::frame())
  {
  } 

  frame_builder(const frame&) = delete;  

  /// destructor
  ~frame_builder() {
    if (m_frame) {
      delete m_frame ;
    }
  }

  /// Grab the constructed frame.
  ///
  /// Once this done, this \c frame_builder can no longer operate and should be discarded.
  /// 
  fmd::frame *
  grab()
  {
    if (!m_frame) {
      return nullptr;
    }
    auto ptr = m_frame ;
    m_frame = nullptr;
    return ptr;
  } 

  /// Indicates if the frame contains at least one active binding.      
  bool
  has_bindings()
  {
    this->check() ;
    return !m_frame->m_bindings.empty();
  }
  
  /// \brief bind a formuloid to the frame
  ///
  /// \param[in] name A proper name for the binding. It will appear 
  ///                 in the formula output and should not contain
  ///                 spaces, parenthesis or any other special
  ///                 character. 
  /// \param[in] in   Any formuloid.
  ///
  /// The formuloid \c in is added to the frame binding list.
  ///
  /// \result A \c fmd::formuloid of the same basal type than \c in
  /// and representing the evaluation of \c in the context of the frame. 
  ///
  template <IsFormuloid F>
  auto
  bind(const char *name, const F &in)
  {
    this->check() ;
    if ( name == NULL )
      throw std::invalid_argument("missing name for the framed formuloid");    
    if ( fmd::obviously_constant_formuloid(in) ) {
      return fmd::to_formuloid(fmd::evaluate(in)) ;
    } else {
      using U = fmd::basal_type_of<F>;
      auto * bind = new fmd::internal::OperFrameRef<U>( name, m_frame, fmd::get_node(in) ) ;
      bind->increase_refcount();
      m_frame->m_bindings.push_back(bind) ;
      return fmd::to_formuloid(bind) ;
    }   
  }
};


namespace internal {

template <typename T>
class OperFrameRef : public fmd::internal::binding_base , public fmd::node_impl<T> {
public:
  const char * m_name;
  fmd::frame * m_frame;
  fmd::node<T> m_sub;
  T            m_value;
  size_t       m_timestamp;
  
  OperFrameRef(const char *name, fmd::frame *f, const fmd::node<T> & sub) :
    m_name(name),
    m_frame(f),
    m_sub(sub),
    m_timestamp{0}
  {
  }

  T evaluate(fmd::context &ctxt) override
  {
    // Update the value if the frame was changed
    if ( this->m_timestamp != m_frame->timestamp())
    {
      this->m_value = m_sub.eval(m_frame->context()) ;
      this->m_timestamp = m_frame->timestamp() ;      
    }
    
    // If the node is not a pure constant, make a mutable
    // event to prevent any optimisation in the current context.
    // Also, increase the frame m_mutable_counter to allow
    // frame::close() to discard that mutable event.
    if ( ! m_sub.is_pure_constant() ) {
      ctxt.record_mutable_event();
      this->m_frame->record_mutable_event();
    }
    return m_value ;
  }

  bool write_formula(std::ostream &out) override {
    out << m_name << "_" << m_frame->print_id();
    return false;
  }
  
  bool write_ref(std::ostream &out) override {
    out << "(";
    this->write_formula(out);
    out << " ";
    m_sub.write_formula(out);
    out << ")";
    return true;
  }

  // Called when the frame is destructed
  void release() override
  {
    m_frame=nullptr; 
    this->decrease_refcount();
  }

};

template <typename T>  
class OperLet : public fmd::node_impl<T>
{
private:
  fmd::frame *m_frame;
  fmd::node<T> m_sub ;
public:

  OperLet(fmd::frame *frame, const fmd::node<T> &sub) :
    m_frame(frame),
    m_sub(sub)
  {
  }
  
  ~OperLet() {
    // Todo: Clear m_sub and m_frame.nodes then check that
    //       Frame is not anymore referenced by any OperFrameRef
    //         --> need a refcount in Frame.
  }

  
  T evaluate(fmd::context &ctxt) override
  {
    m_frame->open(ctxt);
    T result = m_sub.eval(ctxt);
    m_frame->close(ctxt);    
    return result;
  }
  
  bool write_formula(std::ostream &out) override {
    out << "(let (";
    m_frame->write_bindings(out); 
    out << ") " ;
    m_sub.write_formula(out) ;
    out << ")";    
    return true;
  }

} ;

} // of namespace internal

/// @brief Reuse the result of one or more formuloid evaluations.
///
/// \c fmd::let(frame,sub) creates a new formuloid that evaluates
/// the formuloid \c sub within the specified \c frame. The 
/// frame bindings can be used multiple times within \c sub.
///
/// \tparam BUILDER is either fmd::frame_builder<true> or fmd::frame_builder<false>.
/// \tparam T is the basal type of the result. It should be infered from the type of \c sub.
///
/// It should be noted that the binding is only evaluated when it is first
/// encountered in the \c sub expression.
///
/// Note: If the frame does not contain any binding then it is discarded
///       and the result is \c sub potentially promoted to a true
///       formuloid.  
///
/// Example: Provide an efficient formuloid implementation of <tt>G(x) = F(t(x)-d(x))*0.25+F(t(x))*0.5+F(t(x)+d(x))</tt>
///          where \c t(x) and \c d(x) are evaluated only once.
///           
/// \code
///   fmd::func<T> F = ... ;  
///   fmd::func<double> t = ... ;
///   fmd::func<double> d = ... ;
///   ...
///   fmd::frame_builder<true> fb;
///   fmd::func<double> bind_t = fb.bind(t);
///   fmd::func<double> bind_d = fb.bind(d);
///   fmd::func<T> sub = F(bind_t-bind_d)*0.25
///                    + F(bind_t)*0.5
///                    + F(bind_t+bind_d)*0.25
///                    ;
///   fmd::func<T> result = fmd::let(fb,sub);
/// \endcode
///
/// \ingroup grp_func
///
template <IsFormuloid T>
formuloid<basal_type_of<T>>
let( frame_builder &fb, const T &sub)
{
  if ( fb.has_bindings() ) {
    using U = fmd::basal_type_of<T>; 
    using IMPL = fmd::internal::OperLet<U>;    
    return fmd::emplace_formuloid<IMPL>( fb.grab(), fmd::get_node(sub) ) ;
  } else {
    return sub;
  }
}

} // of namespace fmd
