#include "tap.h"
#include <unistd.h>
#include <fmd/fmd.h>

bool tap_main()
{
  tap_plan(7);

  fmd::func<int>    i1 = 42 ;
  fmd::func<double> f1 = 3.4 ;

  tap_assert( i1[0.0]  == 42 ); 
  tap_assert( i1[10.0] == 42 ); 
  tap_assert( f1[0.0]  == 3.4 ); 
  tap_assert( f1[10.0] == 3.4 );

  fmd::func<double> f2  = fmd::id + 2.3 ;
  tap_assert( f2[0.0]   == ( 0.0+2.3) ); 
  tap_assert( f2[1.0]   == ( 1.0+2.3) ); 
  tap_assert( f2[-1.4]  == (-1.4+2.3) ); 

  return true;
}

