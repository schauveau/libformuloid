
namespace fmd {

/// \brief The default actual formuloid implementation
/// \ingroup grp_concept
/// 
/// \tparam T is the corresponding basal type
///
template <IsBasalFormuloid T>
class func : public formuloid<T>
{
public:

  /// \TBD
  func() : formuloid<T>()  {}

  /// \TBD  
  func(const T &v) : formuloid<T>(v)  {}

  /// \TBD
  func(const formuloid<T> &in) : formuloid<T>(in) {}

  /// \TBD
  func<T> &
  operator= (const formuloid<T> &in)
  {
    formuloid<T>::operator=(in);
    return *this; 
  }

  /// \TBD
  func<T> &
  operator= (const T &value)
  {    
    this->assign(value);
    return *this; 
  }
 
public:

  

  /// \brief Evaluate the formuloid for the given parameter p.
  inline T
  operator[](double p) const
  {
    return fmd::evaluate(*this, p);
  }

  /// allow f(p) as an alias for fmd::apply(f,p)  
  template <IsFormuloid<double> T1>
  inline auto
  operator()(const T1 &p) const
  {
    return fmd::apply(*this,p) ; 
  }


  /// allow f.apply(p) as an alias for fmd::apply(f,p)  
  template <IsFormuloid<double> T1>
  inline auto
  apply(const T1 &p) const
  {
    return fmd::apply(*this,p) ; 
  }

  /// \TBD
  void
  write_formula( std::ostream &out) const
  {    
    fmd::current_frame_id=0 ;       
    this->m_node.write_formula(out);   
  } 
  
public:

  /// Join to a new formuloid at a given position
  /// 
  /// Executing
  /// \code
  ///    f.join(t, new_f) ;
  /// \endcode
  /// is equivalent to
  /// \code
  ///    f = fmd::join(f,t,new_f) ;
  /// \endcode
  ///
  /// This member is only available for fmd::func<double>
  ///
  template < IsFormuloid<T> T1 >
  inline auto &
  join( double t, const T1 &new_f )
  {
    *this = fmd::join( *this, t, new_f ) ;
    return *this;
  }

  /// Morph this fmd::func<double> into new_func around t according to
  /// the specified fmd::easing or to fmd::ease_default if none is
  /// specified.
  ///
  /// The statement
  /// \code
  ///    f1.morph(t,f2,ease);
  /// \endcode
  /// is equivalent to
  /// \code
  ///    f1 = fmd::morph(f1,t,f2,ease) ;
  /// \endcode
  ///
  /// This member is only available for fmd::func<double> 
  ///
  template < IsFormuloid<double> T1>
  requires ( std::is_same_v<T,double> )
  inline auto &
  morph( double t,
         const T1 & new_func,
         const fmd::easing &ease = fmd::ease_default )
  {
    *this = fmd::morph( *this, t, new_func, ease ); 
    return *this;
  }

  
  /// Transit to new_f between start and end using the
  /// specified transition formuloid (or fmd:id by default).
  ///
  /// The statement
  ///    f.transit(start, end, new_f, transition);
  /// is equivalent to 
  ///    f = fmd::transit(start, end, f, new_f, transition) 
  ///
  /// Implementation note:
  ///   It seems that g++ cannot resolve T4  when 'transition'
  ///   is implicitly set to 'fmd::id'. This is why a default
  ///   type must be specified for T4.
  ///
  template < IsFormuloid<double> T1,
             IsFormuloid<double> T2 = fmd::func<double>
             >
  requires ( std::is_same_v<T,double> )
  inline auto &
  transit( double start,
           double end,
           const T1 & new_f,
           const T2 & transition = fmd::id)
  {
    *this = fmd::transit( start, end, *this, new_f, transition ); 
    return *this;
  }

  /// \TBD
  template < IsFormuloid<double> T1,
             IsEasing<double>    T2
             >
  requires ( std::is_same_v<T,double> )
  inline auto &
  transit( double start,
           double end,
           const T1 & new_f,
           const T2 & ease)
  {
    *this = fmd::transit( start, end, *this, new_f, ease.function ); 
    return *this;
  }
  
} ;

/// \ingroup grp_operators
///
/// operator<< to ostream is only provided for fmd::formuloid because
/// other actual formuloid implementations may have different requirements.
/// 
template <typename T> 
std::ostream & operator<<(std::ostream &out, const func<T> &a)
{
  a.write_formula(out);
  return out;
}


} // of namespace fmd 
