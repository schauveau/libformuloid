//
// test join() as a member of func<T>
//

#include "tap.h"
#include <fmd/fmd.h>

#include "color_enum.h"

#define MORE_PV 1.5, 4.0, 10.0

#define PLAN 1

#include "tap_main_ptest.h"


bool is_between( double v, double a, double b )
{
  return (std::min(a,b) <= v ) && ( v <= std::max(a,b) ) ;
}

// Simulate a sequence of transitions and return if fv is a plausible
// value at p assuming that the transition functions do not overshot
// (i.e. strictly remain between 0 and 1) 
//
// - start_value is the initial formuloid value at p.
// - transits describes each transit applied to the formuloid also at p.  
// 
// TODO: add a way to simulate exactly using a transition function.
//
bool check_transit( double p,
                    double fp,
                    double start_value,
                    std::vector<std::array<double,3>> transits
                    )
{
  int nb = int(transits.size());
  for ( int k = nb-1 ; k>=0 ; k--) {
    double t0 = transits[k][0];
    double t1 = transits[k][1];
    double current_value = transits[k][2];
    double prev_value    = (k>0) ? transits[k-1][2] : start_value ;
    if ( p >= t1 ) {
      return fp==current_value ;
    } else if (p > t0) {
      // Inside the transit area, we can only make a guess      
      return is_between( fp, prev_value, current_value ) ;
    } 
  }
  return fp==start_value;
}

void test_p(double p)
{
  fmd::func<double> at = fmd::id ;
  
  if (true) {
    fmd::func<double> f = 4.0-at ;
    f.transit(-3.4,  +0.0, at+1.0 );
    f.transit(+1.5,  +5.4, 123.0 );
    f.transit(+8.0, +12.6, at+2.0 );

    std::vector<std::array<double,3>> transits = {
      {-3.4,   0.0, p+1.0},
      {+1.5,   5.4, 123.0},
      {+8.0, +12.6, p+2.0},      
    };
    
    tap_test( check_transit( p, f[p], 4.0-p, transits ) ) ;
  }

  // TODO: write some transit tests with easing.
  //   ==> Could use the easing formuloid to obtain the exact result.
  //       This is a bit circular but that should be fine
  //       since we are testing 'transit' and not the easing formuloids.     
}

