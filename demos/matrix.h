#ifndef FMD_MATRIX_H
#define FMD_MATRIX_H

namespace transform {

struct Matrix2D {
  double xx; double yx;
  double xy; double yy;
  double x0; double y0;

  // uninitialized matrix
  Matrix2D() noexcept = default;

  inline constexpr
  Matrix2D(const Matrix2D &src) noexcept = default;
  
  
  inline constexpr
  Matrix2D(double xx, double yx,
           double xy, double yy,
           double x0, double y0) noexcept
    : xx(xx), yx(yx),
      xy(xy), yy(yy),
      x0(x0), y0(y0)
  {}

  inline constexpr
  Matrix2D(double xx, double yx,
           double xy, double yy,
           double x0, double y0) noexcept
    : xx(xx), yx(yx),
      xy(xy), yy(yy),
      x0(x0), y0(y0)
  {}

  static constexpr
  Identity() noexcept
  {
    return Matrix2D( 1.0, 0.0
                     0.0, 1.0,
                     0.0, 0.0);
  }

  static constexpr
  Translation(double dx, double dy) noexcept
  {
    return Matrix2D( 1.0, 0.0
                     0.0, 1.0,
                     dx,  dy);
  }

  static constexpr
  Scaling(double s) {
    return Matrix2D(   s,  0.0
                     0.0,    s,
                     0.0,  0 0);    
  }

  
  static constexpr
  Scaling(double sx, double sy) {
    return Matrix2D(  sx,  0.0
                     0.0,   sy,
                     0.0,  0 0);    
  }

 static constexpr
 Rotation(double angle) {
   double cos_a = cos(angle);
   double sin_a = sin(angle);
   return Matrix2D(  cos_a,  sin_a
                    -sin_a,  cos_a,
                     0.0,    0 0);    
 }


  
  
};

}
;

