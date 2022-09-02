
namespace fmd {


#define __impl_simple_formuloid_v_1(NAME, TYPE)  \
template < IsFormuloid<TYPE> _FT1 >              \
inline auto NAME(const _FT1 & arg1) {           \
  return fmd::make_formuloid_v(#NAME, compute::v_##NAME<TYPE> , arg1) ;  \
}


#define __impl_simple_formuloid_v_2_same(NAME, TYPE)  \
template < IsFormuloid<TYPE> T1 ,                     \
           IsFormuloid<TYPE> T2 >                     \
inline auto NAME(const T1 & arg1,const T2 & arg2) {  \
  return fmd::make_formuloid_v(#NAME, compute::v_##NAME<TYPE>,arg1,arg2) ; \
}

#define __impl_simple_formuloid_v_3_same(NAME, TYPE)  \
template < IsFormuloid<TYPE> T1 ,                     \
           IsFormuloid<TYPE> T2 ,                     \
           IsFormuloid<TYPE> T3 >                     \
inline auto NAME(const T1 & arg1,const T2 & arg2,const T3 & arg3) {   \
  return fmd::make_formuloid_v(#NAME, compute::v_##NAME<TYPE>,arg1,arg2,arg3) ; \
}

/// \cond SEPARATE_DOC

// Remark: Those functions are documented separately in ../../doc/dummy.h

__impl_simple_formuloid_v_1(abs,  int);
__impl_simple_formuloid_v_1(abs,  long);
__impl_simple_formuloid_v_1(abs,  long long);
__impl_simple_formuloid_v_1(abs,  float);
__impl_simple_formuloid_v_1(abs,  double);

__impl_simple_formuloid_v_1(sin,  float);
__impl_simple_formuloid_v_1(cos,  float);
__impl_simple_formuloid_v_1(tan,  float);
__impl_simple_formuloid_v_1(asin, float);
__impl_simple_formuloid_v_1(acos, float);
__impl_simple_formuloid_v_1(atan, float);
__impl_simple_formuloid_v_1(sind,  float);
__impl_simple_formuloid_v_1(cosd,  float);
__impl_simple_formuloid_v_1(tand,  float);
__impl_simple_formuloid_v_1(asind, float);
__impl_simple_formuloid_v_1(acosd, float);
__impl_simple_formuloid_v_1(atand, float);
__impl_simple_formuloid_v_1(sinh, float);
__impl_simple_formuloid_v_1(cosh, float);
__impl_simple_formuloid_v_1(tanh, float);
__impl_simple_formuloid_v_1(asinh, float);
__impl_simple_formuloid_v_1(acosh, float);
__impl_simple_formuloid_v_1(atanh, float);
__impl_simple_formuloid_v_1(floor, float);
__impl_simple_formuloid_v_1(ceil,  float);
__impl_simple_formuloid_v_1(trunc, float);
__impl_simple_formuloid_v_1(round, float);
__impl_simple_formuloid_v_1(floor_dist, float);
__impl_simple_formuloid_v_1(ceil_dist,  float);
__impl_simple_formuloid_v_1(sqrt,  float);
__impl_simple_formuloid_v_1(cbrt,  float);
__impl_simple_formuloid_v_1(log,   float);
__impl_simple_formuloid_v_1(log2,  float);
__impl_simple_formuloid_v_1(log10, float);
__impl_simple_formuloid_v_1(exp,   float);
__impl_simple_formuloid_v_1(exp2,  float);
__impl_simple_formuloid_v_2_same(atan2,      float) ;
__impl_simple_formuloid_v_2_same(fmod,       float) ;
__impl_simple_formuloid_v_2_same(floor_fmod, float) ;
__impl_simple_formuloid_v_2_same(pow,        float) ;
__impl_simple_formuloid_v_2_same(remainder,  float) ;
__impl_simple_formuloid_v_2_same(hypot, float) ;
__impl_simple_formuloid_v_3_same(hypot, float);

__impl_simple_formuloid_v_1(sin,  double);
__impl_simple_formuloid_v_1(cos,  double);
__impl_simple_formuloid_v_1(tan,  double);
__impl_simple_formuloid_v_1(asin, double);
__impl_simple_formuloid_v_1(acos, double);
__impl_simple_formuloid_v_1(atan, double)

__impl_simple_formuloid_v_1(sind,  double);
__impl_simple_formuloid_v_1(cosd,  double);
__impl_simple_formuloid_v_1(tand,  double);
__impl_simple_formuloid_v_1(asind, double);
__impl_simple_formuloid_v_1(acosd, double);
__impl_simple_formuloid_v_1(atand, double);

__impl_simple_formuloid_v_1(sinh, double);
__impl_simple_formuloid_v_1(cosh, double);
__impl_simple_formuloid_v_1(tanh, double);
__impl_simple_formuloid_v_1(asinh, double);
__impl_simple_formuloid_v_1(acosh, double);
__impl_simple_formuloid_v_1(atanh, double);
__impl_simple_formuloid_v_1(floor, double);
__impl_simple_formuloid_v_1(ceil,  double);
__impl_simple_formuloid_v_1(trunc, double);
__impl_simple_formuloid_v_1(round, double);
__impl_simple_formuloid_v_1(floor_dist, double);
__impl_simple_formuloid_v_1(ceil_dist,  double);
__impl_simple_formuloid_v_1(sqrt,  double);
__impl_simple_formuloid_v_1(cbrt,  double);
__impl_simple_formuloid_v_1(log,   double);
__impl_simple_formuloid_v_1(log2,  double);
__impl_simple_formuloid_v_1(log10, double);
__impl_simple_formuloid_v_1(exp,   double);
__impl_simple_formuloid_v_1(exp2,  double);
__impl_simple_formuloid_v_2_same(atan2,      double) ;
__impl_simple_formuloid_v_2_same(fmod,       double) ;
__impl_simple_formuloid_v_2_same(floor_fmod, double) ;
__impl_simple_formuloid_v_2_same(pow,        double) ;
__impl_simple_formuloid_v_2_same(remainder,  double) ;
__impl_simple_formuloid_v_2_same(hypot, double) ;
__impl_simple_formuloid_v_3_same(hypot, double);

__impl_simple_formuloid_v_3_same(middle, int);
__impl_simple_formuloid_v_3_same(middle, long);
__impl_simple_formuloid_v_3_same(middle, long long);
__impl_simple_formuloid_v_3_same(middle, float);
__impl_simple_formuloid_v_3_same(middle, double);

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> ...   TMORE
>
inline auto
sum(const T1 & arg1, const TMORE & ...more)
{

  if constexpr ( sizeof...(TMORE) == 0 ) {
    return arg1 ;
  } else {
    using type = basal_type_of<T1> ;
    return fmd::make_formuloid_v("sum",
                                fmd::compute::v_sum<type,basal_type_of<TMORE>...>,
                                arg1,
                                more...) ;
  }
}


template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> ...   TMORE
>
inline auto 
product(const T1 & arg1, const TMORE & ...more)
{
  using type = basal_type_of<T1> ;
  if constexpr ( sizeof...(TMORE) == 0 ) {
    return arg1 ;
  } else {
    return fmd::make_formuloid_v("product",
                                fmd::compute::v_product<type,basal_type_of<TMORE>...>,
                                arg1,
                                more...) ;
  }
}

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> ...   TMORE
  >   
inline auto 
mean(const T1 & arg1, const TMORE & ...more)
{
  if constexpr ( sizeof...(TMORE) == 0 ) {
    return arg1 ;
  } else {
    using type = basal_type_of<T1> ;      
    return fmd::make_formuloid_v("mean",
                                fmd::compute::v_mean<type,basal_type_of<TMORE>...>,
                                arg1,
                                more...) ;
  }
}


template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1>       T2,
  IsFormuloid<T1> ...   TMORE
  >
inline auto
min(const T1 & arg1, const T2 & arg2, const TMORE & ...more)
{
  using type = basal_type_of<T1> ; 
  return fmd::make_formuloid_v( "min",
                               fmd::compute::v_min<type,basal_type_of<TMORE>...> ,
                               arg1,
                               arg2,
                               more...) ; 
}

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1>       T2,
  IsFormuloid<T1> ...   TMORE
>
inline auto
max(const T1 & arg1, const T2 & arg2, const TMORE & ...more)
{
  using type = basal_type_of<T1> ;  
  return fmd::make_formuloid_v( "max",
                               fmd::compute::v_max<type,basal_type_of<TMORE>...> ,
                               arg1,
                               arg2,
                               more...) ;
}



template <
  IsFormuloid<double,int> T1,
  IsFormuloid<T1>         T2,
  IsFormuloid<T1>         T3
>
inline auto
clamp( const T1 & v , const T2 & lo , const T3 & hi )
{
  using type = basal_type_of<T1> ;
  return fmd::make_formuloid_cnt( "clamp", fmd::compute::cnt_clamp<type>, v, lo, hi) ;
}



/// \endcond

} // of namespace fmd

