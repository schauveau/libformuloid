
namespace fmd::compute {

/// \addtogroup grp_compute
/// \{

/// \brief coefficient to convert from radians to degrees. 
template <typename T>
requires ( std::is_same_v<T,float> || std::is_same_v<T,double> ) 
static constexpr T rad_to_deg = static_cast<T>(180)*std::numbers::inv_pi_v<T>;

/// \brief coefficient to convert from degrees to radians. 
template <typename T> 
requires ( std::is_same_v<T,float> || std::is_same_v<T,double> ) 
static constexpr T deg_to_rad = std::numbers::pi_v<T>/static_cast<T>(180);

/// \brief Functor used to implement fmd::abs
template <typename T> 
requires requires (T a) { { std::abs(a) } -> std::same_as<T> ; }
auto v_abs =
  [](T a) -> T { 
    return std::abs(a) ; 
  };

/// \brief Functor used to implement fmd::sin
template <typename T> 
requires requires (T a) { { std::sin(a) } -> std::same_as<T> ; }
auto v_sin =
  [](T a) -> T { 
    return std::sin(a) ; 
  };

/// \brief Functor used to implement fmd::sind
template <typename T> 
requires requires (T a) { { std::sin(a) } -> std::same_as<T> ; }
auto v_sind =
  [](T a) -> T { 
    return std::sin(deg_to_rad<T>*a); 
  };

/// \brief Functor used to implement fmd::cos
template <typename T>
requires requires (T a) { { std::cos(a) } -> std::same_as<T> ; }
auto v_cos =
  [](T a) -> T { 
    return std::cos(a) ; 
  };

/// \brief Functor used to implement fmd::cosd
template <typename T>
requires requires (T a) { { std::cos(a) } -> std::same_as<T> ; }
auto v_cosd =
  [](T a) -> T { 
    return std::cos(deg_to_rad<T>*a);
  };

/// \brief Functor used to implement fmd::tan
template <typename T> 
requires requires (T a) { { std::tan(a) } -> std::same_as<T> ; }
auto v_tan   =
  [](T a) -> T { 
    return std::tan(a) ; 
  };

/// \brief Functor used to implement fmd::tand
template <typename T> 
requires requires (T a) { { std::tan(a) } -> std::same_as<T> ; }
auto v_tand   =
  [](T a) -> T { 
    return std::tan(deg_to_rad<T>*a);
  };

/// \brief Functor used to implement fmd::asin
template <typename T> 
requires requires (T a) { { std::asin(a) } -> std::same_as<T> ; }
auto v_asin  =
  [](T a) -> T { 
    return std::asin(a) ; 
  };

/// \brief Functor used to implement fmd::asind
template <typename T> 
requires requires (T a) { { std::asin(a) } -> std::same_as<T> ; }
auto v_asind  =
  [](T a) -> T { 
    return rad_to_deg<T>*std::asin(a);
  };

/// \brief Functor used to implement fmd::acos
template <typename T> 
requires requires (T a) { { std::acos(a) } -> std::same_as<T> ; }
auto v_acos  =
  [](T a) -> T { 
    return std::acos(a) ; 
  };

/// \brief Functor used to implement fmd::acosd
template <typename T> 
requires requires (T a) { { std::acos(a) } -> std::same_as<T> ; }
auto v_acosd  =
  [](T a) -> T { 
    return rad_to_deg<T>*std::acos(a);
  };

/// \brief Functor used to implement fmd::atan
template <typename T> 
requires requires (T a) { { std::atan(a) } -> std::same_as<T> ; }
auto v_atan  =
  [](T a) -> T { 
    return std::atan(a) ; 
  };

/// \brief Functor used to implement fmd::atand
template <typename T> 
requires requires (T a) { { std::atan(a) } -> std::same_as<T> ; }
auto v_atand  =
  [](T a) -> T { 
    return rad_to_deg<T>*std::atan(a) ; 
  };

/// \brief Functor used to implement fmd::sinh
template <typename T> 
requires requires (T a) { { std::sinh(a) } -> std::same_as<T> ; }
auto v_sinh  =
  [](T a) -> T { 
    return std::sinh(a) ; 
  };

/// \brief Functor used to implement fmd::cosh
template <typename T> 
requires requires (T a) { { std::cosh(a) } -> std::same_as<T> ; }
auto v_cosh  =
  [](T a) -> T { 
    return std::cosh(a) ; 
  };

/// \brief Functor used to implement fmd::tanh
template <typename T> 
  requires requires (T a) { { std::atanh(a) } -> std::same_as<T> ; }
auto v_tanh  =
  [](T a) -> T { 
    return std::tanh(a) ; 
  };

/// \brief Functor used to implement fmd::asinh
template <typename T> 
requires requires (T a) { { std::asinh(a) } -> std::same_as<T> ; }
auto v_asinh =
  [](T a) -> T { 
    return std::asinh(a) ; 
  };

/// \brief Functor used to implement fmd::acosh
template <typename T> 
requires requires (T a) { { std::acosh(a) } -> std::same_as<T> ; }
auto v_acosh =
  [](T a) -> T { 
    return std::acosh(a) ; 
  };

/// \brief Functor used to implement fmd::atanh
template <typename T> 
requires requires (T a) { { std::acos(a) } -> std::same_as<T> ; }
auto v_atanh =
  [](T a) -> T { 
    return std::atanh(a) ; 
  };

/// \brief Functor used to implement fmd::floor
template <typename T> 
requires requires (T a) { { std::floor(a) } -> std::same_as<T> ; }
auto v_floor =
  [](T a) -> T { 
    return std::floor(a) ; 
  };

/// \brief Functor used to implement fmd::ceil
template <typename T> 
requires requires (T a) { { std::ceil(a) } -> std::same_as<T> ; }
auto v_ceil  =
  [](T a) -> T { 
    return std::ceil(a) ; 
  };

/// \brief Functor used to implement fmd::trunc
template <typename T> 
requires requires (T a) { { std::trunc(a) } -> std::same_as<T> ; }
auto v_trunc =
  [](T a) -> T { 
    return std::trunc(a) ; 
  };

/// \brief Functor used to implement fmd::round
template <typename T>
requires requires (T a) { { std::round(a) } -> std::same_as<T> ; }
auto v_round =
  [](T a) -> T { 
    return std::round(a) ; 
  };

/// \brief Functor used to implement fmd::floor_dist
template <typename T>
requires requires (T a) { { std::floor(a) } -> std::same_as<T> ; }
auto v_floor_dist =
  [](T a) -> T {
    return a - std::floor(a);
  };

/// \brief Functor used to implement fmd::ceil_dist
template <typename T>
requires requires (T a) { { std::ceil(a) } -> std::same_as<T> ; }
auto v_ceil_dist =
  [](T a) -> T {
    return std::ceil(a) - a;
  };

/// \brief Functor used to implement fmd::sqrt
template <typename T>
requires requires (T a) { { std::sqrt(a) } -> std::same_as<T> ; }
auto v_sqrt =
  [](T a) -> T {
    return std::sqrt(a);
  };

/// \brief Functor used to implement fmd::cbrt
template <typename T> 
requires requires (T a) { { std::cbrt(a) } -> std::same_as<T> ; }
auto v_cbrt =
  [](T a) -> T { 
    return std::cbrt(a) ; 
  };

/// \brief Functor used to implement fmd::log
template <typename T> 
requires requires (T a) { { std::log(a) } -> std::same_as<T> ; }
auto v_log =
  [](T a) -> T { 
    return std::log(a) ; 
  };

/// \brief Functor used to implement fmd::log2
template <typename T> 
requires requires (T a) { { std::log2(a) } -> std::same_as<T> ; }
auto v_log2 =
  [](T a) -> T { 
    return std::log2(a) ; 
  };

/// \brief Functor used to implement fmd::log10
template <typename T> 
requires requires (T a) { { std::log10(a) } -> std::same_as<T> ; }
auto v_log10 =
  [](T a) -> T { 
    return std::log10(a) ; 
  };

/// \brief Functor used to implement fmd::exp
template <typename T> 
requires requires (T a) { { std::exp(a) } -> std::same_as<T> ; }
auto v_exp =
  [](T a) -> T { 
    return std::exp(a) ; 
  };

/// \brief Functor used to implement fmd::exp2
template <typename T> 
requires requires (T a) { { std::exp2(a) } -> std::same_as<T> ; }
auto v_exp2 =
  [](T a) -> T { 
    return std::exp2(a) ; 
  };

/// \brief functor class used to implement v_hypot
///
/// The \c std::hypot exists with 2 and 3 arguments. That cannot
/// be implemented using a single lambda so a custom functor type
/// is needed
/// 
/// \brief Functor used to implement fmd::
template <typename T>
struct v_hypot__impl
{
  /// \brief call hypoth with 2 arguments
  inline auto operator()(T a, T b) const
    requires requires (T x) { { std::hypot(x,x) } -> std::same_as<T> ; }
  {
    return std::hypot(a,b) ;
  }  

  /// \brief call hypoth with 3 arguments
  inline auto operator()(T a, T b, T c) const
    requires requires (T x) { { std::hypot(x,x,x) } -> std::same_as<T> ; }
  {
    return std::hypot(a,b,c) ;
  }

};

/// \brief Functor used to implement fmd::hypot
template <typename T> auto v_hypot = v_hypot__impl<T>{} ;

/// \brief Functor used to implement fmd::atan2
template <typename T>
requires requires (T x) { { std::atan2(x,x) } -> std::same_as<T> ; }
auto v_atan2 =
  [](T a, T b) -> T { 
    return std::atan2(a,b) ; 
  };

/// \brief Functor used to implement fmd::modf
template <typename T>
requires requires (T x) { { std::fmod(x,x) } -> std::same_as<T> ; }
auto v_fmod =
  [](T a, T b) -> T { 
    return std::fmod(a,b) ; 
  };

/// \brief Functor used to implement fmd::floor_fmod
template <typename T>
requires requires (T x) { { std::floor(x) } -> std::same_as<T> ; }
auto v_floor_fmod =
  [](T a, T b) -> T { 
    return a-std::floor(a/b)*b ; 
  };


/// \brief Functor used to implement fmd::pow
template <typename T>
requires requires (T x) { { std::pow(x,x) } -> std::same_as<T> ; }
auto v_pow =
  [](T a, T b) -> T { 
    return std::pow(a,b) ; 
  };


/// \brief Functor used to implement fmd::remainder
template <typename T>
requires requires (T x) { { std::remainder(x,x) } -> std::same_as<T> ; }
auto v_remainder =
  [](T a, T b) -> T { 
    return std::remainder(a,b) ; 
  };


/// \brief Functor used to implement fmd::middle
template <typename T>
requires requires (T x) {
  { std::min(x,x) } -> std::common_reference_with<T> ;
  { std::max(x,x) } -> std::common_reference_with<T> ;
}
auto v_middle =  
  [](T a, T b, T c) -> T  { 
    T min_ab = std::min(a,b) ;
    T max_ab = std::max(a,b) ;
    if (c<min_ab)
      return min_ab;   // order [c,a,b] or [c,b,a]  -> result is min(a,b)
    else if (max_ab<c)
      return max_ab;   // order [a,b,c] or [b,a,c]  -> result is max(a,b)
    else
      return c;        // order [a,c,b] or [b,c,a]  -> result is c
  };


/// \brief Functor used to implement fmd::sum
template <typename T, typename... TMORE>
requires ( std::is_same_v<T,TMORE> && ... )
auto v_sum =
  [](T a, TMORE ...more ) -> T
  { 
    T sum = a;
    ((sum+=more),...) ;
    return sum ;
  };
  
/// \brief Functor used to implement fmd::product
template <typename T, typename... TMORE>
requires ( std::is_same_v<T,TMORE> && ... )
auto v_product =
  [](T a, TMORE ...more ) -> T
  { 
    T p = a;
    ((p*=more),...) ;
    return p ;
  };

/// \brief Functor used to implement fmd::mean
template <typename T, typename... TMORE>
requires ( std::is_same_v<T,TMORE> && ... )
auto v_mean =
  [](T a, TMORE ...more ) -> T
  { 
    T sum = a;
    ((sum+=more),...) ;
    return sum / (1+sizeof...(TMORE)) ;
  };

/// \brief Functor used to implement fmd::min
template <typename T, typename... TMORE>
requires ( std::is_same_v<T,TMORE> && ... )
auto v_min =
  [](T a, T b, TMORE ...more ) -> T
  {
    T val = std::min(a,b) ;  
    ( (val=std::min(val,more)) , ... ) ;
    return val;
  };
  
/// \brief Functor used to implement fmd::max
template <typename T, typename... TMORE>
requires ( std::is_same_v<T,TMORE> && ... )
auto v_max =
  [](T a, T b, TMORE ...more ) -> T
  {
    T val = std::max(a,b) ;  
    ( (val=std::max(val,more)) , ... ) ;
    return val;
  };


/// \brief Functor used to implement fmd::clamp
template <typename T> 
auto v_clamp = [](T v, T lo, T hi) -> T
{
  if (v<lo)
    return lo;
  else if (hi<v)
    return hi;
  else
    return v; 
};

/// \brief Functor used to implement fmd::clamp. 
template <typename U>
auto cnt_clamp = [](fmd::context &ctxt,
                    fmd::node_tuple<U,U,U> &args) -> U
{
  auto & [ v_node, lo_node, hi_node  ] = args ;

  auto v = v_node.eval(ctxt) ;
  auto lo = lo_node.eval(ctxt) ;
  
  if ( v <= lo  ) {
    return lo ;
  }

  auto hi = hi_node.eval(ctxt) ;
  return std::min(v,hi) ; 
};



/// \brief Functor used to implement fmd::select. 
template <typename C, typename U> 
auto cnt_select = [](fmd::context &ctxt,
                     fmd::node_tuple<C,U,U> &args) -> U
{
  auto & [ cond, true_val, false_val ] = args ;
  if ( cond.eval(ctxt) ) {
    return true_val.eval(ctxt) ;
  } else {
    return false_val.eval(ctxt) ;
  }
};

/// \brief Functor used to implement fmd::clamp. 
template <typename T>
auto cnt_lerp= [](fmd::context &ctxt,
                  fmd::node_tuple<T,T,T> &args) -> T
{
  auto & [arg_a, arg_b, arg_t] = args ;
  T t = arg_t.eval(ctxt) ; 
  if (t==0.0) {
    return arg_a.eval(ctxt) ;
  } else if (t==1.0) {
    return arg_b.eval(ctxt) ;      
  } else {
    T a = arg_a.eval(ctxt); 
    T b = arg_b.eval(ctxt); 
    return std::lerp(a,b,t);
  }
};

/// \brief Functor used to implement fmd::lerp_clamp. 
template <typename T>
T cnt_lerp_clamp= [](fmd::context &ctxt,
                     fmd::node_tuple<T,T,T> &args) 
{
  auto & [arg_a, arg_b, arg_t] = args ;
  
  double t = arg_t.eval(ctxt) ; 
  
  if (t<=0.0) {
    return arg_a.eval(ctxt) ;
  } else if (t>=1.0) {
    return arg_a.eval(ctxt) ;      
  } else {
    T a = arg_a.eval(ctxt); 
    T b = arg_b.eval(ctxt); 
    return std::lerp(a,b,t);
  }      
};


/// \brief Functor used to implement fmd::lerp_clamp. 
template <typename T>
auto cnt_transit =
  []( context &ctxt, fmd::node_tuple<T,T,T,T,T> & args)
  {
    double t = ctxt.read_param();
    
    auto & [ arg_t0,
             arg_t1,
             arg_in,
             arg_out,
             arg_ease ] = args ; 
    
    
    double t0 = arg_t0.eval(ctxt) ;     
    double t1 = arg_t1.eval(ctxt) ;
    
    // What to do if the interval is malformed?
    // Multiple strategies are possible:
    //   - fatal error or exception.
    //   - use [t0,t0] 
    //   - use [t1,t1] 
    //   - use the average: t0 = t1 = (t0+t1)/2
    //   - Invert the transition:
    //        - use [t1,t0]
    //        - and also invert the ease function as '1-ease'
    //
    // The most user-friendly is to invert the transition.
    //
    bool inv = false ;
    if (t0 > t1) {
      std::swap(t0,t1) ;
      inv = true;
    }
    
    if ( t < t0 ) {      
      return arg_in.eval(ctxt);
    } 
    
    double dt = t1-t0;
    
    if ( t >= t1 ) {      
      return arg_out.eval(ctxt);
    } 
    
    // At that point, we know that t0 < t < t1
    //   
    // The case dt==0 is probably not possible but let's just be safe
    // in case a very mall t1-t0 would be rounded to 0 (thus causing an
    // Infinite when dividing by dt below)  
    // TODO: Figure out if underflow to 0 can occur during a floating-point
    //       substract operation. I suspect that this is not possible
    //       if denormals are allowed but an external confirmation would be
    //       welcome.
    // Anyways, if that happens then that means that interval [t0,t1]
    // is very very very small. 
    
    if (dt==0) {
      return arg_out.eval(ctxt);
    }
    
    double u = (dt==0) ? 1.0 : (t-t0)/dt ;   
    
    // Make sure that u is in [0,1]
    // I am a bit paranoid again but I cannot fully rule out the possibility that
    // dividing by a very small dt would create an Inf value.
    // PS: that should be fine because t-t0 should be even smaller than dt by
    // construction thus making u < 1.
    u = std::max(0.0, std::min(u,1.0) )  ;
    
    double coef_in, coef_out ;
    
    if (inv) {
      u = 1.0 - u ;
    }
    
    // Remark: u was obtained from t, t0 and t1 and since those are
    //         already accounted in ctxt, we can ignore the parameter
    //         usage of 'u'.  
    context ctxt_param_u(u);
    double e = arg_ease.eval(ctxt_param_u);
    ctxt.merge_without_param(ctxt_param_u);
    
    if (inv) {
      coef_in  = e;
      coef_out = 1.0-e;
    } else {
      coef_out = e;
      coef_in  = 1.0-e ;
    }
    
    // Get the final value. For best performance, only
    // evaluate in[t] and out[t] when needed.  
    double v = 0.0 ;
    if ( coef_in != 0 ) {
      v += coef_in * arg_in.eval(ctxt) ;
    }
    if ( coef_out != 0 ) {
      v += coef_out * arg_out.eval(ctxt) ;
    }
    return v;    
  } ;


template <typename T>
class OperApply final : public node_impl<T> {  
public:

  fmd::node_tuple<T,double> m_args ;
  
  OperApply(const node<T> &a, const node<double> &b)
    : m_args(a,b) 
  {
  }
  
  virtual T evaluate(context &ctxt) override
  {
    auto & [ a, b ] = m_args ;
    context param_ctxt(ctxt);
    double  param = b.eval(param_ctxt);
    context other_ctxt(param);
    T result = a.eval(other_ctxt);
    ctxt.merge_other_param(other_ctxt,param_ctxt) ;
    return result;
  }
  
   
  bool write_formula( std::ostream &out ) override {
    auto & [ a, b ] = m_args ;    
    out << "(-> ";
    a.write_formula(out) ;
    out << " ";
    b.write_formula(out) ;
    out << ")";
    return false;
  }

};




/// \}

} // of namespace fmd::compute
