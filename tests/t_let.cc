#include "tap.h"
#include <fmd/fmd.h>
#include <string.h>
//
// Test fmd::let and fmd::frame_builder
//


//
// We create a custorm formuloid operations pass(x,n) that does two things:
//  - computes x 
//  - increases the static array element count[n] by one.
//
// The idea behind each test is to build some formuloids
// using multiple instances of pass(), evaluate those
// formuloids and check that the value obtained in each
// pass element matches the expectation. 
//

static int count[100];  

void clear_counters() {
  for (auto &c : count)
    c=0;
}

// Our magic pass operation
template <fmd::IsFormuloid T>
auto pass(int n, const T arg)
{
  char buf[10];
  sprintf(buf,"pass%d",n);  
  const char *name= strdup(buf);
  using U = fmd::basal_type_of<T> ;
  return fmd::make_formuloid_v(name,
                               [n](U a) -> U {
                                 count[n]++;
                                 return a;
                               },
                               arg);
}


// 
//  counter[0] = number of times 'arg1_' is evaluated in ref1
//                -> should be 3
//  counter[1] = number of times 'arg1_' is evaluated in test1
//                -> should be 1
//  counter[2] = number of times 'arg1' is evaluated in test1
//                -> should be 3
//

template <fmd::IsFormuloid T1>
auto ref1(T1 arg1_)
{
  auto arg1 = pass(0,arg1_);
  return arg1*(arg1+2.0)*(arg1*3.4) ;
}

template <fmd::IsFormuloid T1>
auto test1(T1 arg1_)
{
  fmd::frame_builder fb;
  auto arg1 = pass(2,fb.bind("arg1",pass(1,arg1_)));
  return fmd::let(fb,arg1*(arg1+2.0)*(arg1*3.4));
}


bool tap_main()
{
  //  tap_plan(4);

  {
    // Check that pass behaves as expected.
    clear_counters();
    fmd::func<double> x = pass(0,fmd::id*(fmd::id+1.0)) ;
    tap_same( x[2.3] , 2.3*(2.3+1.0) );
    tap_same( x[0.3] , 0.3*(0.3+1.0) );
    tap_same( count[0], 2 );
  }

  {
    clear_counters();

    auto r = ref1( pass(3,fmd::sin(fmd::id) )) ;
    auto t = test1( pass(4,fmd::sin(fmd::id) )) ;
    auto vr = r[2.3];
    auto vt = t[2.3]; 
    tap_same( vr, vt );  
    tap_same( count[0], 3 );
    tap_same( count[1], 1 );
    tap_same( count[2], 3 );
    tap_same( count[3], 3 ); // number of sin calls when using ref1
    tap_same( count[4], 1 ); // number of sin calls when using test1
  }
  
  {
    clear_counters();
    auto r = ref1( ref1( pass(3,fmd::sin(fmd::id))) ) ;
    auto t = test1( test1( pass(4,fmd::sin(fmd::id)) ) ) ;
    auto vr = r[2.3];
    auto vt = t[2.3]; 
    tap_same( vr, vt );  
    tap_same( count[0], 3+3*3 );
    tap_same( count[1], 1+1 );
    tap_same( count[2], 3+3 );
    tap_same( count[3], 3*3 ); // number of sin calls when using ref1
    tap_same( count[4], 1 );   // number of sin calls when using test1
  }
  
  return true;
}

