#include "tap.h"
#include <fmd/fmd.h>

//
// Test fmd::id and fmd::identity()
//

bool tap_main()
{
  tap_plan(4);

  {
    fmd::func<double> at = fmd::id ;
    tap_assert( at[1.34] == 1.34 );     
    tap_assert( at[-1.111] == -1.111 );    
  }  

  {
    fmd::func<double> at = fmd::identity();
    tap_assert( at[4.34] == 4.34 );     
    tap_assert( at[-111.2] == -111.2 );     
  }  
  
  return true;
}

