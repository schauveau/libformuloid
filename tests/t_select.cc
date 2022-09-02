#include "tap.h"
#include <fmd/fmd.h>

#include "color_enum.h"


//template <> void fmd::basal_write(std::ostream &out, const color &value) {
//  out << "?my_enum" ;
//}


#define MORE_PV 3.141519 , 1.1234e33

// #define PLAN 2

#include "tap_main_ptest.h"

void test_p(double p)
{

  fmd::func<double> at = fmd::id ;
  
  if (true) {
    fmd::func<double> f = fmd::select(at>0.0, 111.0, 222.0 ) ;
    tap_same( f[p], (p>0.0)?111.0:222.0 ) ;
  }
  
  if (true) {
    fmd::func<int> f = fmd::select( at<=2.0 , 42, -7 ) ;
    tap_same( f[p], (p<=2.0)?42:-7 ) ;
  }

  if (true) {
    fmd::func<bool> f = fmd::select( at<1.0 , at>0.0, at<10.0 ) ;
    tap_same( f[p], (p<1.0)?(p>0):(p<10.0) ) ;
  }
  
  if (true) {
    std::string s1("hello");
    std::string s2("world");    
    fmd::func<std::string> f = fmd::select( at<1.0 , s1, s2 ) ;
    tap_same( f[p], (p<1.0)?s1:s2 ) ;
  }

  if (true) {
    fmd::func<color> f = fmd::select( at<-2.0 , color::red, color::blue ) ;
    tap_same( f[p], (p<-2.0) ? color::red : color::blue  ) ;
  }

}

