#include "tap.h"
#include <fmd/fmd.h>

//
// Implement a simple operation using a scheme class.
// See also fmd/bits/scheme.h
//

//
// Reference implementation of the custom foobar operation
//
//
int foobar_ref(double p, int a, int b)
{
  return static_cast<int>(a*std::sin(p)+b*std::cos(p)) ;
}



struct foobar_scheme
{
  FMD_SCHEME_INFO("foobar", int,
                  fmd::scheme_prop::read_param | fmd::scheme_prop::provide_compute );

  // Tuple and array can be used for storing the arguments.
#if 0
  FMD_SCHEME_STORAGE_ARRAY(int,2);
#else
  FMD_SCHEME_STORAGE_TUPLE(int,int);
#endif

  static int compute( double p, // formuloid parameter 
                      int a, // 1st argument
                      int b  // 2nd argument
                    )
  {
    return static_cast<int>(a*std::sin(p)+b*std::cos(p)) ;
  } 
};

template <fmd::IsFormuloid<int> T1,
          fmd::IsFormuloid<int> T2>
auto
foobar( const T1 &a, const T2&b )
{
  // The scheme sets fmd::scheme_prop::read_param so the result
  // is necessarily fmd::func<int>.  
  return fmd::make_from_scheme<foobar_scheme>(a,b); 
}

//
// Foobar can also be expressed using standard formuloid operations
//
template <fmd::IsFormuloid<int> T1,
          fmd::IsFormuloid<int> T2>
auto
foobar_ref2( const T1 &a, const T2&b )
{
  return fmd::cast<int>( fmd::cast<double>(a)*fmd::sin(fmd::id)
                         + fmd::cast<double>(b)*fmd::cos(fmd::id) ) ;
}

 
bool tap_main()
{
  tap_plan(20);

  std::vector various_p = {2.3 , 4.5, 6.3 , -1.3 } ;
  
  fmd::func<double> at = fmd::id; 

  if (true)
  {
    fmd::func<int> f = foobar(42,37) ;
    fmd::func<int> f2 = foobar_ref2(42,37) ;
    for ( double p :various_p ) {
       tap_same( f[p] , foobar_ref(p, 42, 37) ); 
       tap_same( f[p] , f2[p]);
    }
  }

  if (true)
  {
    fmd::func<int> ati = fmd::cast<int>(at) ; 
    fmd::func<int> f  = foobar( 42+ati, 37*ati) ;
    fmd::func<int> f2 = foobar_ref2( 42+ati, 37*ati) ;

    for ( double p : various_p ) {
      int pi = int(p) ;

      // verify first that fmd::cast<int>() behaves as expected
      tap_same( ati[p] , pi );
      
      // compare f to the native reference 
      tap_same( f[p],  foobar_ref(p, 42+pi, 37*pi)) ;

      // compare f to the 'formuloid' expression reference 
      tap_same( f[p] , f2[p]);       
    }

  }

  
  return true;
}

