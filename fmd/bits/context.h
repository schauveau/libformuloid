
namespace fmd {

/// \brief provide the context of a formuloid evaluation (e.g. parameter, ...)
class context {
private: 
  double  m_param;             
  bool    m_has_param;         
  size_t  m_mutable_counter;   // Number of mutable nodes encountered during evaluation.
  bool    m_param_was_used;    // True is m_param was accessed (via read_param())   
public:

  /// Create a new sub-context reusing the parameter of an existing context.
  context(context &parent) :
    m_param(parent.m_param),
    m_has_param(parent.m_has_param),
    m_mutable_counter(0),
    m_param_was_used(false)    
  {    
  }
  
  // Create a new context without a parameter (e.g. to evaluate an expr)
  context() :
    m_param(0),
    m_has_param(false),
    m_mutable_counter(0),
    m_param_was_used(false)
  {    
  }
  
  /// Create a new evaluation context using the specified parameter
  context(double p) :
    m_param(p),
    m_has_param(true),
    m_mutable_counter(0),
    m_param_was_used(false)
  {
  }

public:

  /// true if at least one mutable event was encountered
  bool is_mutable() { return m_mutable_counter; }

  /// record one mutable event.
  void record_mutable_event() { m_mutable_counter++ ; }

  /// discard some previously recorded mutable events.
  ///
  /// \throw fmd::eval_error if the current number of mutable events is lower
  /// than \c nb
  ///
  void discard_mutable_events(size_t nb) {
    // TODO should throw an eval_error
    if (m_mutable_counter < nb )
      throw fmd::eval_error("cannot discard enough mutable events");
    m_mutable_counter -= nb ;
  }

  /// Provides the number of recorded mutable events 
  size_t mutable_count() { return m_mutable_counter ; }  
  
  /// Return the parameter value associated to the context. 
  ///
  /// \throw false_formuloid_error if the context cannot provide a
  /// parameter (i.e. while evaluating a false formuloid)
  ///
  /// As a side effect, the parameter is marked as read. See also param_was_read()
  ///
  double
  read_param()
  {
    if (!m_has_param) {
      throw false_formuloid_error("true formuloid evaluated as a false formuloid so without a parameter");
    }
    m_param_was_used = true; 
    return m_param; 
  }

  /// Indicate if the parameter was read at least once.
  bool param_was_read() {
    return m_param_was_used;
  }
  
  /// \brief Merge a sub-context with the same parameter that this context. 
  ///
  /// Example:
  /// \code
  ///     context parent_ctxt ;
  ///     ...
  ///     {
  ///        // create a sub_ctxt with the same parameter
  ///        context sub_ctxt(parent_ctxt);
  ///        ...
  ///        ... use sub_ctxt to evaluate one or more nodes 
  ///        ...
  ///        parent_ctxt.merge_same_param(sub_ctxt); 
  ///     }
  /// \endcode
  ///
  void merge_same_param(context &sub)
  {
    this->m_mutable_counter += sub.m_mutable_counter; 
    if (sub.m_param_was_used)
      this->m_param_was_used = true; 
  }

  /// Merge a sub-context with a different parameter than this context.
  /// 
  /// param_ctxt is the context that was used exclusively to compute the sub-context parameter.
  /// param_ctxt shall use the same parameter that this context.
  ///  
  /// For example, the implementation of fmd::evaluate probably does something like 
  ///
  /// \code
  ///     T eval(context &ctxt)
  ///     {
  ///        // Create a new context to evaluate arg2 using the initial parameter.
  ///        fmd::context param_ctxt(ctxt);
  ///        fmd::param_t new_param = arg2.eval(param_ctxt);
  ///        // Create a sub context using that new parameter value.
  ///        fmd::context other_ctxt(new_param);
  ///        // Evaluate arg1 in that sub-context
  ///        T result = arg1.eval(other_ctxt); 
  ///        // And finally merge other_ctxt into the global context
  ///        ctxt.merge_other_param(other_ctxt, param_ctxt);
  ///        return result;
  ///     }
  /// \endcode
  ///
  void merge_other_param(context &other_ctxt, context &param_ctxt)
  {
    this->m_mutable_counter += other_ctxt.m_mutable_counter;
    if ( other_ctxt.m_param_was_used ) {
      merge_same_param(param_ctxt);
    }
  }

  /// Merge a sub-context but without considering its parameter use.
  ///
  /// This is typically used when the sub-context is known
  /// to have no parameter (e.g. an expr) or when the computation
  /// of the sub-context parameter was already merged in this context.
  ///
  void merge_without_param(context &sub)
  {
    this->m_mutable_counter += sub.m_mutable_counter;
  }
};

} // of namespace fmd
