
namespace fmd {

/// \brief A formuloid implementation for cyclic domains such as angles. 
/// \ingroup grp_concept
///
/// \tparam P is the cyclic period.
///
/// 
///
///
template <double P>
class cyclic : public fmd::formuloid<double>
{
private:

  static constexpr double PERIOD = P ;
  static constexpr double HALF_PERIOD = P/2 ;
  
public:

  /// destructor
  ~cyclic() {
  }

  /// \TBD
  cyclic() = default ;
  cyclic( const cyclic &a ) = default ;
  
  /// \TBD  
  cyclic( double &v ) :
    fmd::formuloid<double>(v) 
  {
  }

  /// \TBD
  cyclic(const fmd::formuloid<double> &in)
    : fmd::formuloid<double>(in)
  {
  }
  
  /// \TBD
  auto operator= (const fmd::formuloid<double> &in)
  {    
    this->assign(in);
    return *this; 
  }
  
  /// \TBD
  auto operator= (const fmd::cyclic<P> &in)
  {    
    this->assign(in);
    return *this; 
  }

  /// \TBD
  auto operator= (const double &value)
  {    
    this->assign(value);
    return *this; 
  }
 
public:

  
  /// Evaluate the formuloid value for the given parameter
  inline double
  eval(double p)
  {
    return fmd::evaluate(*this, p);
  }

  /// Evaluate the formuloid value for the given parameter
  inline double
  eval(double p) const
  {
    return fmd::evaluate(*this, p);
  }

  /// Evaluate the formuloid value for the given parameter
  inline double
  operator[](double p) const
  {
    return fmd::evaluate(*this, p);
  }

  /// Evaluate the formuloid value for the given parameter
  inline double
  operator[](double p)
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
    m_node.write_formula(out);   
  } 

public:

  /// Return the formuloid normalized in the range <tt>[-PERIOD/2,+PERIOD/2(</tt> 
  /// TODO: TEST
  fmd::formuloid<double> centered() const
  {
    if constexpr (PERIOD==1.0) {
      return (fmd::floor_dist((*this)+0.5)-0.5);
    } else {
      return (fmd::floor_dist(((*this)/PERIOD)+0.5)-0.5) * PERIOD;
    }
  }

  /// Return the formuloid normalized in the range <tt>[0:PERIOD(</tt>
  /// TODO: TEST
  fmd::formuloid<double> positive() const
  {
    if constexpr (PERIOD==1.0) {
      return fmd::floor_dist(*this);
    } else {
      return fmd::floor_dist((*this)/PERIOD) * PERIOD;
    }
  }

  /// Return the formuloid normalized in the range <tt>[-PERIOD:0(</tt> 
  /// TODO: TEST
  fmd::formuloid<double> negative() const {
    if constexpr (PERIOD==1.0) {
      return fmd::floor_dist(*this)-1.0;
    } else {
      return (fmd::floor_dist((*this)/PERIOD)-1.0) * PERIOD;
    }
  }

public:

  /// Join to a new formuloid  
  /// 
  /// Executing
  /// \code
  ///   f.join(t, new_f) ;
  /// \endcode
  /// is equivalent to
  /// \code
  ///   f = fmd::join(f,t,new_f) ;
  /// \endcode
  template < IsFormuloid<double> T1 >
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
  ///   f1.morph(t,f2,ease);
  /// is equivalent to
  ///   f1 = morph(f1,t,f2,ease) ;
  /// 
  /// This member is only available for fmd::func<double> 
  ///
  /// TODO: Remove? Using Lerp is ambiguous for cyclics  
  ///
  ///
  template < IsFormuloid<double> T1>
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
  /// TODO: Remove? Using Lerp is ambiguous for cyclics
  ///
  template < IsFormuloid<double>       T1,
             IsFormuloid<double>       T2 = fmd::func<double>
             >
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
  /// TODO: Remove? Using Lerp is ambiguous for cyclics
  template < IsFormuloid<double>       T1,
             IsEasing<double>         T2
             >
  inline auto &
  transit( double start,
           double end,
           const T1 & new_f,
           const T2 & ease)
  {
    *this = fmd::transit( start, end, *this, new_f, ease.function ); 
    return *this;
  }
  
public:

  /// Required by fmd::get_node() 
  const auto & get_node() const {
    return m_node;
  }

} ;

/// Cyclic type to hold angles expressed in radians
typedef fmd::cyclic<2*std::numbers::pi> radians ;

/// Cyclic type to hold angles expressed in degrees
typedef fmd::cyclic<360.0> degrees;

/// Unit cyclic type 
typedef fmd::cyclic<1.0> units;


/// \ingroup grp_operators
///
/// operator<< to ostream is only provided for fmd::formuloid because
/// other actual formuloid implementations may have different requirements.
/// 
template <double PERIOD> 
std::ostream & operator<<(std::ostream &out, const fmd::cyclic<PERIOD> &a)
{
  a.write_formula(out);
  return out;
}

} // of namespace fmd 
