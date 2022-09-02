// The fmd::support namespace provides support functions
// used to implement some operators. 

namespace fmd {
namespace support {

#if 0

//  Call a lambda with a template integer argument in the range 0 to count-1
//
//  Example:
//   
//     auto A = std::make_tuple(3.4, 10, "Hello") ; 
//     constexpr_count<3>(
//        [&]<size_t n>() {
//          std::cout << "value " << n << " is " << std::get<n>(A) << "\n"; 
//        });
// --> 
//     value 0 is 3.4
//     value 1 is 10
//     value 2 is Hello
//
//  The implementation details can be hidden using macros.
//
//     auto A = std::make_tuple(3.4, 10, "Hello") ; 
//     FMD_CONSTEXPR_COUNT_BEGIN(3) {
//          std::cout << "value " << n << " is " << std::get<n>(A) << "\n"; 
//     } FMD_CONSTEXPR_COUNT_END; 
//
//
template <size_t count, size_t index=0, typename F_n>
requires (count < 100)
inline void constexpr_count(F_n &&f)
{
  if constexpr (index<count) {
    f.template operator()<index>();
    constexpr_count<count,index+1>(std::forward<F_n>(f));
  }
}

// Similar to constexpr_count but in reverse order from count-1 to 0. 
template <size_t count, size_t index=0, typename F_n>
requires (count < 100)
inline void constexpr_count_rev(F_n &&f)
{
  static_assert(count<100); 
  if constexpr (index<count) {
    f.template operator()<count-index-1>();
    constexpr_count_rev<count,index+1>(std::forward<F_n>(f));
  }
}

#define FMD_CONSTEXPR_COUNT_BEGIN(NAME,COUNT)     fmd::support::constexpr_count<COUNT>( [&]<size_t NAME>() {
#define FMD_CONSTEXPR_COUNT_REV_BEGIN(NAME,COUNT) fmd::support::constexpr_count_rev<COUNT>( [&]<size_t NAME>() {
#define FMD_CONSTEXPR_COUNT_END })

#endif

// Scalar implementation of the Quadratic Bezier Curve in the range 0..1
inline double bezier_quad_01(double t, double p0, double p1, double p2)
{  
  // return (1-t)*((1-t)*p0+t*p1)+t*((1-t)*p1+t*p2);
  // return (1-t)*((1-t)*p0 + t*(2*p1)) + t*t*p2;
  return (1-t)*(p0+t*(2*p1-p0))+t*t*p2;     
}

// Scalar implementation of the Quadratic Bezier Curve (full range)
inline double bezier_quad(double t, double p0, double p1, double p2)
{
  if (t<=0.0) {
    return p0;
  } else if (t<1.0) {
    return bezier_quad_01(t,p0,p1,p2);
  } else {
    return p2;
  }
}

// Scalar implementation of the Cubic Bezier Curve in the range 0..1
inline double bezier_cubic_01(double t, double p0, double p1, double p2, double p3)
{
  if(false) {
    // 22 operations
    return
      ((1-t)*((1-t)*p0+t*p1)+t*((1-t)*p1+t*p2))*(1-t) +
      ((1-t)*((1-t)*p1+t*p2)+t*((1-t)*p2+t*p3))*t ;
  } else if (false) {
    // 16 operations 
    return
      (1-t)*(1-t)*(1-t)*p0 +
      (1-t)*(1-t)*t*3*p1 +
      t*t*(1-t)*3*p2 +
      t*t*t*p3 ;                             
  } else {
    // 14 operations 
    return
      (1-t)*(1-t) * ( (1-t)*p0 + t*(3*p1) ) +
      t*t*( (1-t)*(3*p2) + t*p3 );
  }
}

// Scalar implementation of the Cubic Bezier Curve (full range)
inline double bezier_cubic(double t, double p0, double p1, double p2, double p3)
{
  if (t<=0.0) {
    return p0;
  } else if (t<1.0) {
    return fmd::support::bezier_cubic_01(t,p0,p1,p2,p3);
  } else {
    return p3;
  }
}

}; // of namespace support
}; // of namespace fmd
