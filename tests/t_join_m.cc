//
// test join() as a member of func<T>
//

#include "tap.h"
#include <fmd/fmd.h>

#include "color_enum.h"


//template <> void fmd::basal_write(std::ostream &out, const color &value) {
//  out << "?color" ;
//}

#define MORE_PV -2.0, 3.0, 5.0

#define PLAN 3

#include "tap_main_ptest.h"

void test_p(double p)
{
  fmd::func<double> at = fmd::id ;
  
  if (true) {
    fmd::func<double> f = 4.0-at ;
    f.join(0.0,  at+1.0 );
    f.join(3.0,  123.0 );
    f.join(5.0,  at*at );

    double v;  
    if (p>=5.0)
      v = p*p ;
    else if (p>=3.0)
      v = 123.0 ;
    else if (p>=0.0)
      v = p+1.0 ;
    else
      v = 4.0-p ;
    
    tap_same( f[p], v ) ;
  }
  
  if (true) {
    fmd::func<color> f = color::red ;
    f.join(-1.0, color::blue );
    f.join(1.3, color::white );
    f.join(4.5, color::green );

    color v;  
    if (p>=4.5)
      v = color::green ;
    else if (p>=1.3)
      v = color::white ;
    else if (p>=-1.0)
      v = color::blue ;
    else
      v = color::red ;
    
    tap_same( f[p], v ) ;
  }

  if (true) {
    fmd::func<int> f = 11 ;
    f.join(-6.4, 22 );
    f.join(+4.3, 33 );
    f.join(-2.0, 44 ); 

    // Joining at '-2.0' hides the 2 previous joins.
    // They are probably still present in the formula since
    // removing them would require more optimizations than
    // currently performed. 
    // Not impossible but unlikely to ever happen.
    // So this is inefficient but not incorrect. 
    
    int v;  
    if (p>=-2.0)
      v = 44 ;
    // else if (p>=4.3)
    //   v = 33; 
    // else if (p>=-6.4)
    //   v = 22;
    else
      v = 11 ;
    
    tap_same( f[p], v ) ;
  }
  
}

