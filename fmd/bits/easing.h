namespace fmd {

/////////// easing

/// \brief TO BE DOCUMENTED
// A fmd::easing object describes how to perform a transition between
// any two formuloid<double> around an arbitrary point t.
// 
// The 'ease_' variables below provide common easing methods. 
//
//
// Note: The sole reason to implement fmd::easing as a templated type 
//       is to delay its instanciation until the implementation of
//       fmd::formuloid.  This is a typical chicken & egg problem.
//       A fmd::func<double> (i.e. fmd::formuloid<double,true> ) is stored 
//       in 'fmd::easing' but the formuloid implementation is also
//       using fmd::easing.
//
template <typename T>
class easing_impl
{
public:
  fmd::formuloid<T>  function;  ///< A transition formuloid that must be defined in the range [0,1]  
  T             start;     ///< The relative start offset
  T             end;       ///< The relative end offset
public:

  /// \TBD
  easing_impl(const fmd::formuloid<T> &function, T start, T end ) :
    function(function) , start(start), end(end)
  {
  }


  /// \TBD
  easing_impl(const fmd::formuloid<T> &function ) :
    function(function) , start(-0.5), end(+0.5)
  {
  }

  /// \TBD
  inline T duration() const { return end-start ; } 
 
  /// \TBD
  ///
  /// Provide a copy with the [start,end] interval shifted to [0.0, end-start]
  ///
  /// For example, fmd::easing(F,-0.3,0.4).after() is equivalent to fmd::easing(F,0.0,0.7)   
  inline easing_impl after() const {
    return { this->function , T(0.0) , this->duration() } ;
  }

  
  /// \TBD
  ///
  /// Provide a copy with the [start,end] interval shifted to [end-start, 0.0]
  ///
  /// For example, fmd::easing_impl(F,-0.3,0.4).before() is equivalent to fmd::easing(F,-0.7,0.0)
  ///
  inline easing_impl before() const {
    return { this->function , -this->duration() , T(0.0) } ;
  }

  /// \TBD
  ///
  /// Provide a copy with the [start,end] interval centered at 0.0.
  ///
  /// For example, fmd::easing(F,-0.3,+0.4).center() is equivalent to fmd::transit(F,-0.35,+0.35)
  ///
  inline easing_impl center() const {
    double half = this->duration() * T(0.5) ; 
    return { this->function , -half , +half } ;
  }

  /// \TBD
  ///
  /// Provide a copy of this transit with the [start,end] interval scaled
  /// by the specified factor.  
  inline easing_impl scale(T factor) const {
    return { this->function , this->start*factor , this->end*factor } ;   
  }

  /// \TBD
  ///
  /// Provide a copy of this transit with the [start,end] interval scaled
  /// so that its duration matches the given length.    
  inline easing_impl resize(T length) const {
    double d = this->duration() ;
    if (d == T(0.0)) {
      // Special case of an empty initial interval.
      // Let's assume that the user wants a centered result.
      return { this->function , this->start-length/T(2.0) , this->end+length/T(2.0) } ;
    }
    return this->scale(length/d) ;
  }
  
} ;

/// \TBD
template<typename Tp>
inline easing_impl<Tp>
operator*( const easing_impl<Tp> &in, Tp factor) {
  return in.scale(factor);
}

/// \TBD
template<typename Tp>
inline easing_impl<Tp>
operator/( const easing_impl<Tp> &in, Tp factor) {
  return in.scale(1.0/factor);
}

/// \TBD
typedef fmd::easing_impl<double> easing ;

/// \TBD
extern const fmd::easing ease_none;  // The transition occurs instantly (no easing). 

// 
// Various standard 'easing' transitions found, for instance, in CSS.
// See https://easings.net/ for more details.
//
// Unless specified otherwise, they are all defined in the range [-0.5,+0.5] 
//

extern const fmd::easing ease_linear;  ///< \TBD

extern const fmd::easing ease_in_quad; ///< \TBD
extern const fmd::easing ease_out_quad; ///< \TBD
extern const fmd::easing ease_inout_quad; ///< \TBD

extern const fmd::easing ease_in_cubic; ///< \TBD
extern const fmd::easing ease_out_cubic; ///< \TBD
extern const fmd::easing ease_inout_cubic; ///< \TBD

extern const fmd::easing ease_in_quart; ///< \TBD
extern const fmd::easing ease_out_quart; ///< \TBD
extern const fmd::easing ease_inout_quart; ///< \TBD

extern const fmd::easing ease_in_quint; ///< \TBD
extern const fmd::easing ease_out_quint; ///< \TBD
extern const fmd::easing ease_inout_quint; ///< \TBD

extern const fmd::easing ease_in_sine; ///< \TBD
extern const fmd::easing ease_out_sine; ///< \TBD
extern const fmd::easing ease_inout_sine; ///< \TBD

extern const fmd::easing ease_out_bounce; ///< \TBD
extern const fmd::easing ease_in_bounce; ///< \TBD

extern const fmd::easing ease_in_back05; ///< \TBD
extern const fmd::easing ease_out_back05; ///< \TBD
extern const fmd::easing ease_inout_back05; ///< \TBD

extern const fmd::easing ease_in_back10; ///< \TBD
extern const fmd::easing ease_out_back10; ///< \TBD
extern const fmd::easing ease_inout_back10; ///< \TBD

extern const fmd::easing ease_in_back20; ///< \TBD
extern const fmd::easing ease_out_back20; ///< \TBD
extern const fmd::easing ease_inout_back20; ///< \TBD

extern const fmd::easing ease_in_back30; ///< \TBD
extern const fmd::easing ease_out_back30; ///< \TBD
extern const fmd::easing ease_inout_back30; ///< \TBD

// Some modifiable default transit methods 

extern fmd::easing ease_in;        ///< \brief Initially set to ease_in_quad
extern fmd::easing ease_out;       ///< \brief Initially set to ease_out_quad
extern fmd::easing ease_inout;     ///< \brief Initially set to ease_inout_quad

/// This is the recommanded default for all functions that require
/// a fmd::easing as input like, for instance, fmd::func<T>::morph
extern fmd::easing ease_default;

/// \TBD
template <typename T, typename U=double>
concept IsEasing = std::same_as<T,fmd::easing_impl<U>> ; 

} ; // of namespace fmd
