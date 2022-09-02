
namespace fmd {


// ============= F[@] = Transit(in[@], out[@], t0, t1, ease[@]) 
//
// All types are 'double'
//
// F[t] = 
//    if (t<t0) 
//       return in[t]
//    else if (t>t1)
//       return out[t]  
//    else
//       u = (t-t0)/(t1-t0) # so with 0 <= tmp <= 1
//       coef_out = ease[u]  
//       coef_in  = 1 - coef_out ;
//       return in[t]*coef_in + out[t]*coef_out  ;
//
/// \TBD
template < IsFormuloid<double>      T3,
           IsFormuloid<double>      T4,
           IsFormuloid<double>      T5 = fmd::formuloid<double>
          >
inline
fmd::formuloid<double>  
transit( double start,
         double end,
         const T3  &in,
         const T4  &out, 
         const T5  &transition = fmd::id)
{
  return fmd::make_formuloid_cnt("transit",
                                fmd::compute::cnt_transit<double> ,
                                start, end, in, out, transition) ;
}

/// \TBD
/// 
/// fmd::transit can also be specified using a fmd::easing  
/// for the transition. Though, the interval specified in
/// the easing object is ignored since the transition interval
/// is fully specified by [start:end]. 
///
template < IsFormuloid<double>   T3,
           IsFormuloid<double>   T4,
           IsEasing<double>     T5 
          >
inline auto
transit( double start,
         double end,
         const T3 & in,
         const T4 & out, 
         const T5 & ease)
{
  return fmd::transit(start,end,in,out,ease.function);
}

}; // end of namespace fmd
