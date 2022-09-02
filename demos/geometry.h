#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

// Some trigonometry functions 
//
// A nice source for the formuas is 
//   http://www.ambrsoft.com/TrigoCalc/menu/menuTrigoCalc.htm
//
// Notations and conventions:
//
//  - Polar coordinates (r,α) and cartesian coordinates (x,y)
//    are defined as
//          x = r*cos(α) 
//          y = r*sin(α) 
//  
//  - When using the computer convention with the x-axis pointing to the right
//    and the y-axis pointing downward, polar angles are growing clockwise.
//
//  - Using the computer convention with the x-axis pointing to the right
//    and the y-axis pointing upward, polar angles are growing counter-clockwise.
//   
//  - In all cases,
//      - points on the positive x-axis have a polar angle of 0° 
//      - points on the positive y-axis have a polar angle of 90° 
//      - points on the negative x-axis have a polar angle of 180° (or -180°)
//      - points on the negative y-axis have a polar angle of 270° (or -90°) 
//
//
//
//

namespace trigo {

static inline
double
dist2(double x, double y)
{
  return (x*x)+(y*y);
}

static inline
double
dist(double x, double y)
{
  return std::sqrt((x*x)+(y*y));
}


// Compute the square of the Euclidian distance
// between two points A=(xa,ya) and B=(xb,yb)
static inline
double
dist2(double xa, double ya,
      double xb, double yb)
{
  return dist2(xb-xa,yb-ya);
}

// Compute the Euclidian distance between
// two points A=(xa,ya) and B=(xb,yb)
static inline
double
dist(double xa, double ya,
     double xb, double yb)
{
  return std::sqrt(dist2(xa,ya,xb,yb)); 
}

// Provide the polar angle at the point (x,y)
static inline 
double
polar_angle(double x, double y)
{
  return std::atan2(y,x) ;
}

// Compute the polar angle between the vector AB and the x-axis.
static inline 
double
angle_ab(double xa, double ya,
         double xb, double yb)
{
  return std::atan2(yb-ya,xb-xa) ;
}


// Compute the angle ABC formed by the vector BA and BC
static inline 
double
angle_abc(double xa, double ya,
          double xb, double yb,
          double xc, double yc)
{
  double abc = std::atan2(yc-yb,xc-xb) - std::atan2(ya-yb,xa-xb) ;
  if (abc <= -M_PI)
    abc += 2*M_PI;
  else if ( abc > M_PI)
    abc -= 2*M_PI;
  return abc;
}



// Return t1 scaled by the given factor relative to t0.
//
//  scale(t0,t1,factor) = t0 + (t1-t0)*factor 
//   
// For example, assuming 2 points A and B, the point C such
// that AC = 5*AB is given by
//
//  C.x = scale( A.x, B.x, 5.0 );
//  C.y = scale( A.y, B.y, 5.0 );
//
inline static
double
scale(double t0, double t1, double factor)
{
  return t0 + (t1-t0)*factor ;
}

// Compute the center (x,y) and the radius r of the circle inscribed
// in the 3 points (x1,y1), (x2,y2) and (x3,y3).
//
// Return true in case of success (or false if the 3 points are aligned).
//
inline static
bool
find_inscribed_circle(double xa, double ya,
                      double xb, double yb,
                      double xc, double yc,
                      double &x, double &y,
                      double &r )
{
  // See http://www.ambrsoft.com/TrigoCalc/Circle3D.htm
  // The circle formula is Ax² + Ay² + Bx + Cy + D = 0 

  double da = xa*xa+ya*ya ;
  double db = xb*xb+yb*yb ;
  double dc = xc*xc+yc*yc ;
  
  double A = xa*(yb-yc) - ya*(xb-xc) + xb*yc - xc*yb ;

  // Detect the special case of aligned or equal points.
  if (A==0)
    return false ; 
  
  double B = da*(yc-yb) + db*(ya-yc) + dc*(yb-ya);
  double C = da*(xb-xc) + db*(xc-xa) + dc*(xa-xb);
  double D = da*(xc*yb-xb*yc) +  db*(xa*yc-xc*ya) + dc*(xb*ya-xa*yb);

  x = -B/(2*A) ;
  y = -C/(2*A) ;
  r = sqrt((B*B+C*C-4*A*D)/(4*A*A));
  return true ;
}

}; // of namespace trigo

#endif

