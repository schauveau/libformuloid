#include <iostream>
#include <iomanip>

#include <fmd/fmd.h>

static constexpr double default_at = 1.5 ;

template <typename V,
          fmd::IsActualFormuloid<V> T>
void eval_func( const T &f , double at=0.0 )
{
  std::cout << "func[" << at << "] = " << f[at] << "\n" ; 
}


template <typename V, fmd::IsActualFormuloid<V> T>
  void dump_func(T v)
{
  std::cout << ": func := " << v << "\n" ; 
}


template <typename T>
void eval_func_d(const T &v, double at=default_at ) {
  eval_func<double>(v,at) ;
}

template <typename T>
void eval_func_i(const T &v, double at=default_at ) {
  eval_func<int>(v,at) ;
}

template <typename T>
void eval_func_b(const T &v, double at=default_at ) {
  eval_func<bool>(v,at) ;
}

///

#define need_func_i  need_func<int>
#define need_func_b  need_func<bool>
#define need_func_d  need_func<double>

///

#define dump_func_i  dump_func<int>
#define dump_func_b  dump_func<bool>
#define dump_func_d  dump_func<double>



#define DEF_CONCEPTS_OP_1(NAME,OPERATION) \
template <typename Ta, typename R> \
concept check_operation_##NAME = requires (const Ta &a) { { OPERATION } -> std::same_as<R> ; }; \
template <typename Ta> \
concept exist_operation_##NAME = requires (const Ta &a) { OPERATION ; };\
template <typename Ta> \
concept unknown_operation_##NAME = (!exist_operation_##NAME<Ta>) ;


DEF_CONCEPTS_OP_1(neg,-a) ;
DEF_CONCEPTS_OP_1(not,!a) ;
DEF_CONCEPTS_OP_1(bitwise_not,~a) ;

#define DEF_CONCEPTS_OP_2(NAME,OPERATION) \
template <typename Ta, typename Tb, typename R> \
concept check_operation_##NAME = requires (const Ta &a, const Tb &b) { { OPERATION } -> std::same_as<R> ; }; \
template <typename Ta, typename Tb> \
concept exist_operation_##NAME = requires (const Ta &a, const Tb &b) { OPERATION ; }; \
template <typename Ta, typename Tb> \
concept unknown_operation_##NAME = (!exist_operation_##NAME<Ta,Tb>) ;


DEF_CONCEPTS_OP_2(add,a+b) ;
DEF_CONCEPTS_OP_2(sub,a-b) ;
DEF_CONCEPTS_OP_2(mul,a*b) ;
DEF_CONCEPTS_OP_2(div,a*b) ;
DEF_CONCEPTS_OP_2(mod,a*b) ;

DEF_CONCEPTS_OP_2(and,a && b) ;
DEF_CONCEPTS_OP_2(or,a || b) ;

DEF_CONCEPTS_OP_2(eq,a==b) ;
DEF_CONCEPTS_OP_2(ne,a!=b) ;
DEF_CONCEPTS_OP_2(gt,a>b) ;
DEF_CONCEPTS_OP_2(lt,a<b) ;
DEF_CONCEPTS_OP_2(ge,a>=b) ;
DEF_CONCEPTS_OP_2(le,a<=b) ;

DEF_CONCEPTS_OP_2(bitwise_and,a&b) ;
DEF_CONCEPTS_OP_2(bitwise_or,a|b) ;
DEF_CONCEPTS_OP_2(bitwise_xor,a^b) ;

#define check(what) do { if constexpr (!what) { std::cout << "FAIL: " #what  << "\n" ; } else { std::cout << "OK:   " #what  << "\n" ; } } while(0)

int main(void)
{
  std::cout << std::boolalpha ;
  fmd::func<double> at = fmd::id ;

  fmd::func<int> f_0 = 0 ;
  fmd::func<int> f_1 = 1 ;


  fmd::func<bool> f_true  = true;
  fmd::func<bool> f_false = false;

  fmd::func<double> f_pi = 3.14159;

#define TEST_OP1_FAIL(NAME,T1) \
  check(( unknown_operation_##NAME<  fmd::func<T1> > )); \

#define TEST_OP2_FAIL(NAME,T1,T2) \
  check(( unknown_operation_##NAME<  fmd::func<T1>,    fmd::func<T2> > )); \

#define TEST_OP1_OK(NAME,T1,RT) \
  check(( check_operation_##NAME<  fmd::func<T1>,    fmd::func<RT> > )); \

#define TEST_OP1_OK_S(NAME,T1,RT) \
  check(( check_operation_##NAME<  T1,               RT            > )); \
  check(( check_operation_##NAME<  fmd::func<T1>,    fmd::func<RT> > )); \

  
#define TEST_OP2_OK(NAME,T1,T2,RT) \
  check(( check_operation_##NAME<  fmd::func<T1>,    fmd::func<T2>,    fmd::func<RT> > )); \

#define TEST_OP2_OK_S(NAME,T1,T2,RT) \
  check(( check_operation_##NAME<  T1,               T2,               RT            > )); \
  check(( check_operation_##NAME<  fmd::func<T1>,    fmd::func<T2>,    fmd::func<RT> > )); \

  // Test the '' operators

  TEST_OP1_FAIL(neg,bool) ;
  TEST_OP2_FAIL(add,bool,bool) ;
  TEST_OP2_FAIL(sub,bool,bool) ;
  TEST_OP2_FAIL(mul,bool,bool) ;
  TEST_OP2_FAIL(div,bool,bool) ;
  TEST_OP2_FAIL(mod,bool,bool) ;
  TEST_OP1_OK_S(not,bool,bool) ;
  TEST_OP2_OK_S(and,bool,bool,bool) ;
  TEST_OP2_OK_S(or,bool,bool,bool) ;
  TEST_OP2_OK_S(eq,bool,bool,bool) ;
  TEST_OP2_OK_S(ne,bool,bool,bool) ;
  TEST_OP2_FAIL(lt,bool,bool) ;
  TEST_OP2_FAIL(gt,bool,bool) ;
  TEST_OP2_FAIL(le,bool,bool) ;
  TEST_OP2_FAIL(gt,bool,bool) ;
  TEST_OP1_FAIL(bitwise_not,bool) ;
  TEST_OP2_FAIL(bitwise_and,bool,bool) ;
  TEST_OP2_FAIL(bitwise_or,bool,bool) ;
  TEST_OP2_FAIL(bitwise_xor,bool,bool) ;

  // Test the 'int' operators

  TEST_OP1_OK_S(neg,int,int) ;
  TEST_OP2_OK_S(add,int,int,int) ;
  TEST_OP2_OK_S(sub,int,int,int) ;
  TEST_OP2_OK_S(mul,int,int,int) ;
  TEST_OP2_OK_S(div,int,int,int) ;
  TEST_OP2_OK_S(mod,int,int,int) ;
  TEST_OP1_FAIL(not,int) ;
  TEST_OP2_FAIL(and,int,int) ;
  TEST_OP2_FAIL(or,int,int) ;
  TEST_OP2_OK_S(eq,int,int,bool) ;
  TEST_OP2_OK_S(ne,int,int,bool) ;
  TEST_OP2_OK_S(lt,int,int,bool) ;
  TEST_OP2_OK_S(gt,int,int,bool) ;
  TEST_OP2_OK_S(le,int,int,bool) ;
  TEST_OP2_OK_S(gt,int,int,bool) ;
  TEST_OP1_OK_S(bitwise_not,int,int) ;
  TEST_OP2_OK_S(bitwise_and,int,int,int) ;
  TEST_OP2_OK_S(bitwise_or,int,int,int) ;
  TEST_OP2_OK_S(bitwise_xor,int,int,int) ;

  // Test the 'double' operators

  TEST_OP1_OK_S(neg,double,double) ;
  TEST_OP2_OK_S(add,double,double,double) ;
  TEST_OP2_OK_S(sub,double,double,double) ;
  TEST_OP2_OK_S(mul,double,double,double) ;
  TEST_OP2_OK_S(div,double,double,double) ;
  TEST_OP2_OK_S(mod,double,double,double) ;
  TEST_OP1_FAIL(not,double) ;
  TEST_OP2_FAIL(and,double,double) ;
  TEST_OP2_FAIL(or,double,double) ;
  TEST_OP2_OK_S(eq,double,double,bool) ;
  TEST_OP2_OK_S(ne,double,double,bool) ;
  TEST_OP2_OK_S(lt,double,double,bool) ;
  TEST_OP2_OK_S(gt,double,double,bool) ;
  TEST_OP2_OK_S(le,double,double,bool) ;
  TEST_OP2_OK_S(gt,double,double,bool) ;
  TEST_OP1_FAIL(bitwise_not,double) ;
  TEST_OP2_FAIL(bitwise_and,double,double) ;
  TEST_OP2_FAIL(bitwise_or,double,double) ;
  TEST_OP2_FAIL(bitwise_xor,double,double) ;




  
  // fmd::Identity()
  {
    std::cout << "### " << "Identity" << "\n";
    eval_func_d( fmd::identity() );        
    eval_func_d( fmd::id );        
    eval_func_b( (fmd::id == 1234.567) , 1234.567 );        
    eval_func_b( (fmd::id != 1234.567) , 33.0 )  ;        
  }

  
  {
    std::cout << "### " << "operator -a" << "\n";
#define DO_UNOP
#define DO_INT
#define OPER -
#include "demo-syntax-oper.h"
  }

  {
    std::cout << "### " << "operator ~a" << "\n";
#define DO_UNOP
#define DO_INT
#define OPER ~
#include "demo-syntax-oper.h"     
  }

  {
    std::cout << "### " << "operator !a" << "\n";
#define DO_UNOP
#define DO_BOOL
#define OPER !
#include "demo-syntax-oper.h"     
  }
  
  {
    std::cout << "### " << "operator a+b" << "\n";
#define DO_BINOP
#define DO_INT
#define DO_DOUBLE
#define OPER +
#include "demo-syntax-oper.h"
  }
  
  {
    std::cout << "### " << "operator a*b" << "\n";
#define DO_BINOP
#define DO_INT
#define DO_DOUBLE
#define OPER *
#include "demo-syntax-oper.h"
  }

  {
    std::cout << "### " << "operator a/b" << "\n";
#define DO_BINOP
#define DO_INT
#define DO_DOUBLE
#define OPER /
#include "demo-syntax-oper.h"
  }

  {
    std::cout << "### " << "operator a%b" << "\n";
#define DO_BINOP
#define DO_INT
#define OPER %
#include "demo-syntax-oper.h"          
  }

 
  {
    std::cout << "### " << "operator a & b" << "\n";
#define DO_BINOP
#define DO_INT
#define OPER &
#include "demo-syntax-oper.h"
  }

  {
    std::cout << "### " << "operator a | b" << "\n";
#define DO_BINOP
#define DO_INT
#define OPER |
#include "demo-syntax-oper.h"
  }

  {
    std::cout << "### " << "operator a ^ b" << "\n";
#define DO_BINOP
#define DO_INT
#define OPER ^
#include "demo-syntax-oper.h"  
  }
  
  {
    std::cout << "### " << "operator a && b" << "\n";
#define DO_BINOP
#define DO_BOOL
#define OPER &&
#include "demo-syntax-oper.h"  
  }
  
  {
    std::cout << "### " << "operator a || b" << "\n";
#define DO_BINOP
#define DO_BOOL
#define OPER &&
#include "demo-syntax-oper.h"  
  }

  {
    std::cout << "### " << "operator a == b" << "\n";
#define DO_COMPARE
#define DO_BOOL
#define DO_INT
#define DO_DOUBLE
#define OPER ==
#include "demo-syntax-oper.h"
  }
  
  {
    std::cout << "### " << "operator a != b" << "\n";
#define DO_COMPARE 1
#define DO_BOOL
#define DO_INT
#define DO_DOUBLE
#define OPER !=
#include "demo-syntax-oper.h"
  }
  
  {
    std::cout << "### " << "operator a < b" << "\n";
#define DO_COMPARE 1
//#define DO_BOOL
#define DO_INT
#define DO_DOUBLE
#define OPER <
#include "demo-syntax-oper.h"
  }

  {
    std::cout << "### " << "operator a > b" << "\n";
#define DO_COMPARE 1
//#define DO_BOOL
#define DO_INT
#define DO_DOUBLE
#define OPER >
#include "demo-syntax-oper.h"
  }
  
  {
    std::cout << "### " << "operator a <= b" << "\n";
#define DO_COMPARE 1
//#define DO_BOOL
#define DO_INT
#define DO_DOUBLE
#define OPER <=
#include "demo-syntax-oper.h"
  }
  
  {
    std::cout << "### " << "operator a >= b" << "\n";
#define DO_COMPARE 1
//#define DO_BOOL
#define DO_INT
#define DO_DOUBLE
#define OPER >=
#include "demo-syntax-oper.h"
  }
  
  // Abs
  {
    std::cout << "===== " << "Abs" << "\n";
    eval_func_i( fmd::abs(f_0) );
    eval_func_d( fmd::abs(f_pi) );
  }
  
 // Sum
  {
    std::cout << "===== " << "Sum" << "\n";
    eval_func_i( fmd::sum(f_0) );
    eval_func_i( fmd::sum(4, f_0 ) );
    eval_func_i( fmd::sum(4, 25, f_0) );
    eval_func_i( fmd::sum(4, 25, f_0, 44) );
    eval_func_i( fmd::sum(4, 25, f_0, 12, 33) );
    eval_func_d( fmd::sum( at, f_pi, 1.4) , 111.111) ;
    dump_func_d( fmd::sum( at, f_pi, 1.4 ) ) ;
  }

  // Mean
  {
    std::cout << "===== " << "mean" << "\n";
    eval_func_i( fmd::mean(f_0) );
    eval_func_i( fmd::mean(4, f_0 ) );
    eval_func_i( fmd::mean(4, 25, f_0) );
    eval_func_i( fmd::mean(4, 25, f_0, 44) );
    eval_func_i( fmd::mean(4, 25, f_0, 12, 33) );
    eval_func_d( fmd::mean( at, f_pi, 1.4) , 111.111) ;
    dump_func_d( fmd::mean( at, f_pi, 1.4 ) ) ;
  }


  // Product
  {
    std::cout << "===== " << "product" << "\n";
    eval_func_i( fmd::product(f_0) );
    eval_func_i( fmd::product(4, f_0 ) );
    eval_func_i( fmd::product(4, 25, f_0) );
    eval_func_i( fmd::product(4, 25, f_0, 44) );
    eval_func_i( fmd::product(4, 25, f_0, 12, 33) );
    eval_func_d( fmd::product( at, f_pi, 1.4) , 111.111) ;
    dump_func_d( fmd::product( at, f_pi, 1.4 ) ) ;
  }

  // min
  {
    std::cout << "===== " << "min" << "\n";
    eval_func_i( fmd::min(4, f_0 ) );
    eval_func_i( fmd::min(4, 25, f_0) );
    eval_func_i( fmd::min(4, 25, f_0, 44) );
    eval_func_i( fmd::min(4, 25, f_0, 12, 33) );
    eval_func_d( fmd::min( at, f_pi, 1.4) , 111.111) ;
    dump_func_d( fmd::min( at, f_pi, 1.4 ) ) ;
  }

  // Max
  {
    std::cout << "===== " << "max" << "\n";
    eval_func_i( fmd::max(4, f_0 ) );
    eval_func_i( fmd::max(4, 25, f_0) );
    eval_func_i( fmd::max(4, 25, f_0, 44) );
    eval_func_i( fmd::max(4, 25, f_0, 12, 33) );
    eval_func_d( fmd::max( at, f_pi, 1.4 ) , 111.111) ;
    dump_func_d( fmd::max( at, f_pi, 1.4 ) ) ;
  }

  // Clamp
  {
    std::cout << "===== " << "Clamp" << "\n";
    eval_func_i( fmd::clamp(4, -5, f_0) );
    eval_func_d( fmd::clamp(at, f_pi, 1.4 ) ); 
  }

  fmd::func<double> aa = 1.0 ;
  std::cout << floor_dist(aa) << "\n" ;
  std::cout << fmd::floor_dist(-1.3) << "\n" ;

//  titi(3.4);
//  toto(3.4);
}
