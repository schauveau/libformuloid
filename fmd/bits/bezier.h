#ifndef FMD_BITS_BEZIER_H
#define FMD_BITS_BEZIER_H

namespace fmd {

namespace internal {

// OperBezierQuad(tin,tout,p0,p1,p2,t) 
// 
// Bezier Quadradic interpolation between p0 at tin to p2 at tout using
// p1 as control point. 
//
template <std::same_as<double> T>
class OperBezierQuad: public OperArgsTuple<T,T,T,T,T,T,T> {
public:
  OperBezierQuad(const node<T> &t0,
                 const node<T> &t2,
                 const node<T> &p0,
                 const node<T> &p1,
                 const node<T> &p2,
                 const node<T> &t) :
    OperArgsTuple<T,T,T,T,T,T,T>(t0,t2,p0,p1,p2,t)
    {
    }
  
  virtual T evaluate(context &ctxt) override
  {
 
    auto & [ arg_t0,
             arg_t2,
             arg_p0,
             arg_p1,
             arg_p2,
             arg_t ] = this->m_args ;
                 
    T t  = arg_t.eval(ctxt) ;
    T result;

    // The context used to evaluate p0[t], p1[t] and p2[t].
    context ctxt_param_t(t);
    
    T t0 = arg_t0.eval(ctxt) ;
    if (t<=t0) {
      result = arg_p0.eval(ctxt_param_t) ;     
    } else {
      T t2 = arg_t2.eval(ctxt) ;
      if (t >= t2) {
        result = arg_p2.eval(ctxt_param_t) ;
      } else {
        result = fmd::support::bezier_quad_01( (t-t0)/(t2-t0),
                                               arg_p0.eval(ctxt_param_t),
                                               arg_p1.eval(ctxt_param_t),
                                               arg_p2.eval(ctxt_param_t));
      }
    }

    // Remark: 't' was already evaluated in 'ctxt' so we do not care
    //         of its potential use in p0[t], p1[t] and p2[t]
    ctxt.merge_without_param(ctxt_param_t); 
    return result;
  }
      
  const char * name() final { return "bezier_quad" ; } 
};


template <std::same_as<double> T>
class OperBezierCubic: public OperArgsTuple<T,T,T,T,T,T,T,T> {
public:
  OperBezierCubic(const node<T> &t0,
                  const node<T> &t3,
                  const node<T> &p0,
                  const node<T> &p1,
                  const node<T> &p2,
                  const node<T> &p3,
                  const node<T> &t) :
    OperArgsTuple<T,T,T,T,T,T,T,T>(t0,t3,p0,p1,p2,p3,t)
    {
    }
 

  virtual T evaluate(context &ctxt) override
  {
 
    auto & [ arg_t0,
             arg_t3,
             arg_p0,
             arg_p1,
             arg_p2,
             arg_p3,
             arg_t  ] = this->m_args ;
      
    T t  = arg_t.eval(ctxt) ;
    T result;

    // The context used to evaluate p0[t], p1[t] and p2[t].
    context ctxt_param_t(t);
    
    T t0 = arg_t0.eval(ctxt);
    if (t<=t0) {
      result = arg_p0.eval(ctxt_param_t);
    } else {
      T t3 = arg_t3.eval(ctxt);
      if (t>=t3) {
        result = arg_p3.eval(ctxt_param_t);
      } else {
        result = fmd::support::bezier_cubic_01( (t-t0)/(t3-t0),                    
                                                arg_p0.eval(ctxt_param_t),
                                                arg_p1.eval(ctxt_param_t),
                                                arg_p2.eval(ctxt_param_t),
                                                arg_p3.eval(ctxt_param_t) );
      }
    }

    // Remark: 't' was already evaluated in 'ctxt' so we do not care
    //         if it was used in p0[t], p1[t], p2[t] or p3[t]. 
    ctxt.merge_without_param(ctxt_param_t); 
    return result;

  }
  
  const char * name() final { return "bezier_cubic" ; } 
};


} // of namespace internal

/// \TBD
/// \ingroup grp_func
template < IsFormuloid<double> T1,
           IsFormuloid<double> T2,
           IsFormuloid<double> T3,
           IsFormuloid<double> T4,
           IsFormuloid<double> T5,
           IsFormuloid<double> T6
           >
inline auto
bezier_quad( const T1 &t0,
             const T2 &t2,
             const T3 &p0,
             const T4 &p1,
             const T5 &p2,
             const T6 &t
  )
{
  using namespace fmd::internal; 
  auto op = new OperBezierQuad( fmd::get_node(t0),
                                fmd::get_node(t2),
                                fmd::get_node(p0),
                                fmd::get_node(p1),
                                fmd::get_node(p2),
                                fmd::get_node(t)
                                ); 
  return fmd::to_formuloid(op);
}

/// \TBD
/// \ingroup grp_func
template < IsFormuloid<double> T1,
           IsFormuloid<T1>     T2,
           IsFormuloid<T1>     T3,
           IsFormuloid<T1>     T4,
           IsFormuloid<T1>     T5,
           IsFormuloid<T1>     T6,
           IsFormuloid<T1>     T7
           >
inline auto
bezier_cubic( const T1 &t0,
              const T2 &t3,
              const T3 &p0,
              const T4 &p1,
              const T5 &p2,
              const T6 &p3,
              const T7 &t  )
{
  using namespace fmd::internal; 
  auto op = new OperBezierCubic( fmd::get_node(t0),
                                 fmd::get_node(t3),
                                 fmd::get_node(p0),
                                 fmd::get_node(p1),
                                 fmd::get_node(p2),
                                 fmd::get_node(p3),
                                 fmd::get_node(t)
                                ); 
  return fmd::to_formuloid(op);
}



}; // of namespace fmd

#endif
