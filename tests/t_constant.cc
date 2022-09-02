#include "tap.h"
#include <fmd/fmd.h>

enum class my_enum { foo=44, bar=22 } ;

//
// Test various simple constant formuloids.
//

bool tap_main()
{
  tap_plan(
    3    // int
    +4   // double
    +4   // bool 
    +2   // string
    +1   // enum
    ); 

  // int
  {
    const int value = 123;
    fmd::func<int>  cst = value ;
    tap_assert( cst[0.0]  == value ); 
    tap_assert( cst[10.0] == value ); 
    tap_assert( cst.eval(44.0) == value ); 
  }

  // double

  {
    const double value = -111.43;
    fmd::func<double>  cst = value ;
    tap_assert( cst[0.0]  == value ); 
    tap_assert( cst[10.0] == value ); 
    tap_assert( cst.eval(-3.4) == value ); 
    tap_assert( cst[6.7]  != 123.44 ); 
  }

  // bool

  {
    fmd::func<bool> t = true ;
    fmd::func<bool> f = false ;
    tap_assert( t[5.6]  == true ); 
    tap_assert( t[-3.4] == true ); 
    tap_assert( f[1.3]  == false ); 
    tap_assert( f[12.3] == false ); 
  }

  // std::string
  

  {
    fmd::func<std::string> cst = std::string("joe") ;
    tap_assert( cst[5.6]  == "joe" ); 
    tap_assert( cst.eval(1.2) == "joe" ); 
  }

  // an enum class
  {

    my_enum value = my_enum::foo ;
    fmd::func<my_enum> cst = value ;
    tap_assert( cst[12.3]  ==  value ); 
  }
  
  return true;
}

