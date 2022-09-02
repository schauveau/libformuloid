
//
// Overloaded C++ operators.
//
// Unless specified otherwise, the operands and the result of an overloaded
// operator have the same basic type.
// Types are not implicitly promoted as in C++ native operators.
// For example,  fmd::func<int> + fmd::func<double> is not legal.
//
// Instead, fmd::Cast can be used to convert between types.
//
//    fmd::func<int>    f1 = fmd::Select(fmd::ID<0.0, 0, 42) ;
//    fmd::func<double> f2 = fmd::Sin(fmd::ID);
//    fmd::func<double> f3 = f1 + f2 ;                     // Illegal
//    fmd::func<double> f4 = fmd::Cast<double>(f1) + f2 ;  // Legal
//
// Most arithmetic operators are supported for all integral and
// floating point types. The exception is the modulo operator %
// that is only supported for integral types.
//
//     - a      
//   a + b    
//   a - b    
//   a * b    
//   a / b    
//   a % b  
//
// Bitwise operations are supported for all integral types.
//
//     ~ a        
//   a & b
//   a | b      
//   a ^ b      
//
// Comparison operations are implemented for all supported types.
// The operators == and < must be available for that type.
//
//   a == b
//   a != b     --> implemented as !(a==b)
//   a <  b   
//   a >  b     --> implemented as b < a
//   a <= b     --> implemented as !(b<a)
//   a >= b     --> implemented as !(a<b)
//
// Logical operations are supported for bool arguments only. 
//
//      ! a
//   a && b
//   a || b
//
// Shift operations are not yet implemented.
// 


namespace fmd::compute {

enum class op1 {
  logical_not,
  neg,
  bitwise_not
} ;

enum class op2 {
  eq,ne,lt,gt,le,ge,
  add,sub,mul,div,mod,
  logical_and,
  logical_or,
  bitwise_and,
  bitwise_or,
  bitwise_xor
} ;

template <op1 OPER, typename T, typename T1>
class OperUnary : public node_impl<T>   {
public:

  node<T1> arg1 ;
  
  OperUnary( const node<T1> &a) :  arg1(a) {}

  T evaluate(context &ctxt) override {    
    T1 a = this->arg1.eval(ctxt);    
    if constexpr (OPER==op1::logical_not)  return !a ;
    if constexpr (OPER==op1::neg)          return -a ;
    if constexpr (OPER==op1::bitwise_not)  return ~a ;
  }

  const char * name() {    
    if constexpr (OPER==op1::logical_not)  return "not" ;
    if constexpr (OPER==op1::neg)          return "-" ;
    if constexpr (OPER==op1::bitwise_not)  return "~" ;          
  }

  bool write_formula( std::ostream &out ) override {
    out << "(" << this->name() ;
    write_nodes(out, this->arg1);
    out << ")";
    return true;
  }
  
};

template <op2 OPER, typename T, typename T1 , typename T2>
class OperBinary : public node_impl<T> {
public:

  node<T1> arg1 ;
  node<T2> arg2 ;
  
  OperBinary( const node<T1> &a, const node<T2> &b) :  arg1(a), arg2(b) {}
  
  T evaluate(context &ctxt) override
  {
    
    if constexpr (OPER==op2::logical_and) {
      return this->arg1.eval(ctxt) && this->arg2.eval(ctxt) ;
    }

    if constexpr (OPER==op2::logical_or) {
      return this->arg1.eval(ctxt) || this->arg2.eval(ctxt) ;
    }
    
    T1 a = this->arg1.eval(ctxt);
    T1 b = this->arg2.eval(ctxt);
    
    if constexpr (OPER==op2::eq)  return   a == b ;
    if constexpr (OPER==op2::ne)  return !(a == b);
    if constexpr (OPER==op2::lt)  return   a < b;
    if constexpr (OPER==op2::le)  return !(b < a);
    if constexpr (OPER==op2::gt)  return   b < a;
    if constexpr (OPER==op2::ge)  return !(a < b);
    if constexpr (OPER==op2::add) return (a + b);
    if constexpr (OPER==op2::sub) return (a - b);
    if constexpr (OPER==op2::mul) return (a * b);
    if constexpr (OPER==op2::div) return (a / b);
    if constexpr (OPER==op2::mod) return (a % b);
    if constexpr (OPER==op2::bitwise_and) return (a & b);
    if constexpr (OPER==op2::bitwise_or ) return (a | b);
    if constexpr (OPER==op2::bitwise_xor) return (a ^ b);        
  }

  const char * name() {
    if constexpr (OPER==op2::eq) return "==" ;
    if constexpr (OPER==op2::ne) return "!=" ;
    if constexpr (OPER==op2::lt) return "<" ;
    if constexpr (OPER==op2::le) return "<=" ;
    if constexpr (OPER==op2::gt) return ">" ;
    if constexpr (OPER==op2::ge) return ">=" ;
    if constexpr (OPER==op2::add) return "+";
    if constexpr (OPER==op2::sub) return "-";
    if constexpr (OPER==op2::mul) return "*";
    if constexpr (OPER==op2::div) return "/";
    if constexpr (OPER==op2::mod) return "%";
    if constexpr (OPER==op2::logical_and) return "and";
    if constexpr (OPER==op2::logical_or ) return "or";
    if constexpr (OPER==op2::bitwise_and) return "&";
    if constexpr (OPER==op2::bitwise_or ) return "|";
    if constexpr (OPER==op2::bitwise_xor) return "^";
  }

  bool write_formula( std::ostream &out ) override {
    out << "(" << this->name() ;
    write_nodes(out, this->arg1, this->arg2);
    out << ")";
    return false;
  }
  
};

template <op1 OPER, typename RT, typename T1>
inline auto unary_op( const T1 & arg1)
{
  using namespace fmd::internal;
  using namespace fmd::compute;
  typedef basal_type_of<T1> V1; 
  using IMPL = OperUnary<OPER,RT,V1> ;
  return fmd::emplace_formuloid<IMPL>( fmd::get_node(arg1) ) ;   
}

template <op2 OPER, typename RT, typename T1, typename T2>
inline auto binary_op( const T1 & arg1, const T2 & arg2)
{
  using namespace internal;
  typedef basal_type_of<T1> V1; 
  typedef basal_type_of<T2> V2;
  using IMPL = OperBinary<OPER,RT,V1,V2> ;
  return fmd::emplace_formuloid<IMPL>( fmd::get_node(arg1) , fmd::get_node(arg2) ) ;   
}

} // of namespace fmd::compute


namespace fmd {

// Reminder: formuloid operators are documented separately in the doc/ directory

/// \cond SEPARATE_DOC

template <
  IsFormuloid<int,long,float,double> T1,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1> )
inline auto operator-( const T1 & arg1)
{
  using namespace fmd::compute;
  return unary_op<op1::neg,RT>(arg1) ;
}

template <
  IsFormuloid<int,long> T1,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1> )
inline auto operator~( const T1 & arg1)
{
  using namespace fmd::compute;
  return unary_op<op1::bitwise_not,RT>(arg1) ;
}

template <
  IsFormuloid<bool> T1,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1> )
inline auto operator!( const T1 & arg1)
{
  using namespace fmd::compute;
  return unary_op<op1::logical_not,RT>(arg1) ;
}


template <
  IsFormuloid<int,double> T1,
  IsFormuloid<T1>         T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator+( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::add,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1>         T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator-( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::sub,RT>(arg1,arg2) ;
}


template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator*( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::mul,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator/( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::div,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long> T1,
  IsFormuloid<T1> T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator%( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::mod,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long> T1,
  IsFormuloid<T1> T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator&( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::bitwise_and,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long> T1,
  IsFormuloid<T1> T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator|( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::bitwise_or,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long> T1,
  IsFormuloid<T1> T2,
  typename RT = fmd::basal_type_of<T1>
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator^( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::bitwise_xor,RT>(arg1,arg2) ;
}

template <
  IsFormuloid<bool> T1,
  IsFormuloid<bool> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator||( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::logical_or,bool>(arg1,arg2) ;
}

template <
  IsFormuloid<bool> T1,
  IsFormuloid<bool> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator&&( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::logical_and,bool>(arg1,arg2) ;
}

template <
  IsFormuloid<bool,int,long,float,double> T1,
  IsFormuloid<T1> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator==( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::eq,bool>(arg1,arg2) ;
}

template <
  IsFormuloid<bool,int,long,float,double> T1,
  IsFormuloid<T1> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator!=( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::ne,bool>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator<( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::lt,bool>(arg1,arg2) ;
}


template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator<=( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::le,bool>(arg1,arg2) ;
}

template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator>( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::gt,bool>(arg1,arg2) ;
}


template <
  IsFormuloid<int,long,float,double> T1,
  IsFormuloid<T1> T2
  >
requires ( !fmd::all_basal_formuloid_v<T1,T2> )
inline auto operator>=( const T1 & arg1, const T2 & arg2)
{
  using namespace fmd::compute;
  return binary_op<op2::ge,bool>(arg1,arg2) ;
}

/// \endcond

} // of namespace fmd

