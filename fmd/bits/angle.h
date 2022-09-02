
namespace fmd::compute {

enum class cyclic_direction {
  forward,
  backward,
  close,
  far
} ;

double cyclic_lerp(double a, double b, double t, bool reverse)
{
  double dummy ;
  // x = floor_dist(a);
  double x = modf(a, &dummy);
  if (a<0.0) x=1.0-x;
  // y = floor_dist(b);
  double y = modf(b, &dummy);
  if (b<0.0) y=1.0-y;

  if (reverse) {
    
  }
  double dist = y-x ;
  // Transform 'a' and 'b' so that standard lerp becomes applicable
  if (dir==cyclic_direction::close) {
    if (std::fabs(dist) < 0.5) ;

    if (dir==cyclic_direction::forward) {
    if (b<a) {
      b+=1.0;
    }    
  } else if (dir==cyclic_direction::backward) {
    if (a<b) {
      a+=1.0;
    }
  } 
  } else if (dir==cyclic_direction::far) {

  }
}

namespace fmd {
}

