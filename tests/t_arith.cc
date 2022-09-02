#include "tap.h"
#include <fmd/fmd.h>

#define MORE_PV 3.141519 , 1.1234e33

#define PLAN 6

#include "tap_main_ptest.h"

void test_p(double p)
{
  fmd::func<double> at = fmd::id ;
  
  if (true) {
    fmd::func<double> f = (at+1.0) ;
    tap_same( f[p], (p+1.0)) ;
  }

  if (true) {
    fmd::func<double> f = (at/4.5) ;
    tap_same( f[p], (p/4.5)) ;
  }
  
  if (true) {
    fmd::func<double> f = (at*at+1.4)/2.5 ;
    tap_same( f[p], ((p*p+1.4)/2.5 )) ;
  }
  
  if (true) {
    // Remark: inf should be produced when p==PV[3].
    // That should be legal.
    fmd::func<double> f = ((at+1.4)/(p-PV[3])-3.4*at) ;
    tap_same( f[p], ((p+1.4)/(p-PV[3])-3.4*p)) ;
  }
  
  if (true) {
    fmd::func<double> f = (-at+1.4) ;
    tap_same( f[p], (-p+1.4) ) ;
  }
  
  if (true) {
    fmd::func<bool> f = (at==PV[4] || at==PV[7] ) ;
    tap_same( f[p], (p==PV[4])||(p==PV[7]) ) ;
  }
  
}

