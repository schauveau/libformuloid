#define FMD_INSTANTIATE

#include "fmd.h"
#include <cctype>
#include <memory>

namespace fmd {

size_t current_frame_id = 0 ;

const formuloid<double> id = fmd::make_formuloid_pv("@",
                                                  [](double p) {
                                                    return p;
                                                  }) ;

static auto
ease_in_helper(double p) {
  p = std::max( 1.0, std::min(p, 5.0 ));
  return fmd::pow(fmd::id,p);
}

static auto
ease_out_helper(double p) {
  p = std::max( 1.0, std::min(p, 5.0 ));
  return  1.0 - fmd::pow(1.0-fmd::id,p) ; 
}

static auto
ease_inout_helper(double p) {
  p = std::max( 1.0, std::min(p, 5.0 ));
  double scale = std::pow(2.0,p-1) ;
  return fmd::select(fmd::id <0.5,
                     scale * fmd::pow(fmd::id,p),
                     1.0 - scale*fmd::pow(1.0-fmd::id,p) );
}

const fmd::easing ease_none       = { fmd::id , 0.0, 0.0 } ; 

const fmd::easing ease_linear     = { fmd::id , -0.5, +0.5 } ;

const fmd::easing ease_in_quad     = { ease_in_helper(2) } ; 
const fmd::easing ease_out_quad    = { ease_out_helper(2) } ; 
const fmd::easing ease_inout_quad  = { ease_inout_helper(2) } ; 

const fmd::easing ease_in_cubic    = { ease_in_helper(3) } ; 
const fmd::easing ease_out_cubic   = { ease_out_helper(3) } ; 
const fmd::easing ease_inout_cubic = { ease_inout_helper(3) } ; 

const fmd::easing ease_in_quart    = { ease_in_helper(4) } ; 
const fmd::easing ease_out_quart   = { ease_out_helper(4) } ; 
const fmd::easing ease_inout_quart = { ease_inout_helper(4) } ; 

const fmd::easing ease_in_quint    = { ease_in_helper(5) } ; 
const fmd::easing ease_out_quint   = { ease_out_helper(5) } ; 
const fmd::easing ease_inout_quint = { ease_inout_helper(5) } ; 

const fmd::easing ease_in_sine     =
{
  fmd::sin((fmd::id - 1.0) * (std::numbers::pi/2.0) ) + 1.0
} ; 

const fmd::easing ease_out_sine    =
{
  fmd::sin((fmd::id) * (std::numbers::pi/2.0) )
} ; 

const fmd::easing ease_inout_sine  = {
  0.5 * (1.0 - fmd::cos( fmd::id * std::numbers::pi ))
} ;

const fmd::easing ease_inout_sine2  = {

  fmd::make_formuloid_v( "ease_inout_sine",
                        [] (double x) -> double {
                          return 0.5 * (1.0 - std::cos( x * std::numbers::pi )) ;
                        },
                        fmd::id )
    
} ;

const fmd::easing ease_out_bounce  =
{ 
  fmd::join(
    (121.0 * fmd::id * fmd::id) / 16.0 ,
    4.0/11.0,
    (363.0/40.0 * fmd::id * fmd::id) - (99/10.0 * fmd::id) + 17/5.0 ,
    8.0/11.0,
    (4356.0/361.0 * fmd::id * fmd::id) - (35442.0/1805.0 * fmd::id) + 16061.0/1805.0 ,
    9.0/10.0,
    (54.0/5.0 * fmd::id * fmd::id) - (513.0/25.0 * fmd::id) + 268.0/25.0
    )
} ;

const fmd::easing ease_in_bounce  =
{
  1.0 - ease_out_bounce.function(1.0-fmd::id)
} ; 

const fmd::easing ease_in_back05 =
{
  fmd::id * fmd::id  - fmd::id * fmd::sin( fmd::id * std::numbers::pi) * 0.5
};

const fmd::easing ease_in_back10 =
{
  fmd::id * fmd::id  - fmd::id * fmd::sin( fmd::id * std::numbers::pi) * 0.7
};

const fmd::easing ease_in_back20 =
{
  fmd::id * fmd::id  - fmd::id * fmd::sin( fmd::id * std::numbers::pi) * 0.9
};

const fmd::easing ease_in_back30 =
{
  fmd::id * fmd::id  - fmd::id * fmd::sin( fmd::id * std::numbers::pi) * 1.1
};


const fmd::easing ease_out_back05 =
{
  1.0 - ease_in_back05.function(1.0-fmd::id)
};

const fmd::easing ease_out_back10 =
{
  1.0 - ease_in_back10.function(1.0-fmd::id)
};

const fmd::easing ease_out_back20 =
{
  1.0 - ease_in_back20.function(1.0-fmd::id)
};

const fmd::easing ease_out_back30 =
{
  1.0 - ease_in_back30.function(1.0-fmd::id)
};

const fmd::easing ease_inout_back05 =
{
  fmd::join( ease_in_back05.function(2.0*fmd::id) * 0.5,
             0.5,
             0.5 * (1.0 - ease_in_back05.function((1.0 - (2.0*fmd::id - 1.0)))) + 0.5
    ) 
};

const fmd::easing ease_inout_back10 =
{
  fmd::join( ease_in_back10.function(2.0*fmd::id) * 0.5,
             0.5,
             0.5 * (1.0 - ease_in_back10.function((1.0 - (2.0*fmd::id - 1.0)))) + 0.5
    ) 
};

const fmd::easing ease_inout_back20 =
{
  fmd::join( ease_in_back20.function(2.0*fmd::id) * 0.5,
             0.5,
             0.5 * (1.0 - ease_in_back20.function((1.0 - (2.0*fmd::id - 1.0)))) + 0.5
    ) 
};

const fmd::easing ease_inout_back30 =
{
  fmd::join( ease_in_back30.function(2.0*fmd::id) * 0.5,
             0.5,
             0.5 * (1.0 - ease_in_back30.function((1.0 - (2.0*fmd::id - 1.0)))) + 0.5
    ) 
};

fmd::easing ease_in       = fmd::ease_in_quad;
fmd::easing ease_out      = fmd::ease_out_quad;
fmd::easing ease_inout    = fmd::ease_inout_quad;
fmd::easing ease_default  = fmd::ease_inout_quad;


namespace internal
{

std::string cxx_demangle(const char* name)
{
#ifdef FMD_USE_CXXABI
  int status = -4; 
  std::unique_ptr<char, void(*)(void*)> res {
    abi::__cxa_demangle(name, NULL, NULL, &status),
    std::free
  };
  return (status==0) ? res.get() : name ;
#else
  return name; 
#endif
}

}


};
