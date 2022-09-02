namespace fmd {

///
/// Base class for objects to be thrown as exception during the
/// evaluation of formuloids.
/// 
class eval_error: public std::runtime_error {
public:
  /// \brief constructs a new eval_error object with the given message 
  explicit eval_error( const std::string & msg ) : runtime_error(msg) {}
  /// \brief constructs a new eval_error object with the given message 
  explicit eval_error( const char *msg ) : runtime_error(msg) {};
};

/// \brief TO BE DOCUMENTED
///
/// This class is thrown during the evaluation of a false formuloid if 
/// the formuloid parameter is accessed.
/// 
/// Simply speaking, this indicates that a true formuloid (e.g. a std::func)
/// was somehow assigned to a false formuloid (e.g. a std::expr).
///
class false_formuloid_error : public eval_error {
public:
  /// \brief constructs a new operation_error object with the given message 
  explicit false_formuloid_error( const std::string & msg ) : eval_error(msg) {}
  /// \brief constructs a new operation_error object with the given message 
  explicit false_formuloid_error( const char *msg )   : eval_error(msg) {};
};


} // of namespace fmd
