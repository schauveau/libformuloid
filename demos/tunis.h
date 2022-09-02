#ifndef TUNIS_H
#define TUNIS_H 1

#include "geometry.h"

/////
///// Tunis is a small set of support functions around the Cairo api.
/////
///// Why the name Tunis? Cairo ... Tunis ... that should be obvious.
/////

#ifdef TUNIS_DISABLE_INLINING
#define TUNIS_STATIC        [[maybe_unused]] [[ gnu::noinline ]] static
#define TUNIS_STATIC_INLINE [[maybe_unused]] [[ gnu::noinline ]] static 
#else
#define TUNIS_STATIC        [[maybe_unused]] static
#define TUNIS_STATIC_INLINE [[maybe_unused]] static inline
#endif

#define TUNIS_ATTR_PRINTF(fmt,arg) [[ gnu::__format__(__printf__,fmt,arg) ]]

#include <cairo/cairo.h>
#include <cstdio>
#include <cstdarg>

TUNIS_ATTR_PRINTF(1,2)
TUNIS_STATIC_INLINE
std::string tunis_sprintf(const char *fmt, ...) ;

TUNIS_ATTR_PRINTF(1,0)
TUNIS_STATIC_INLINE
std::string tunis_vsprintf(const char *fmt, va_list ap) ;

TUNIS_ATTR_PRINTF(4,5)
TUNIS_STATIC_INLINE
void tunis_printf(cairo_t *cr,
                  int x,
                  int y,
                  const char *fmt,
                  ... ) ;//TUNIS_PRINTF(4,5);


TUNIS_STATIC_INLINE
cairo_t *tunis_create_context_with_size(cairo_surface_t *surf,
                                        double x1, double y1,
                                        double x2,
                                        double y2);
TUNIS_STATIC_INLINE
void tunis_hsv_to_rgb(double h,
                      double s,
                      double v,
                      double &r,
                      double &g,
                      double &b);

TUNIS_STATIC_INLINE
void tunis_set_source_hsva(cairo_t *cr,
                           double h,
                           double s,
                           double v,
                           double a);

TUNIS_STATIC_INLINE
void tunis_set_source_hsv(cairo_t *cr,
                          double h,
                          double s,
                          double v);
TUNIS_STATIC_INLINE
void tunis_circle(cairo_t * cr,
                  double x,
                  double y,
                  double r );

TUNIS_STATIC_INLINE
void tunis_rectangle(cairo_t *cr,                                   
                     double x,
                     double y,
                     double w,
                     double h,
                     double rot=0.0);

TUNIS_STATIC_INLINE
void tunis_damier(cairo_t *cr,
                  double x,
                  double y,
                  double w,
                  double h,
                  double cw,
                  double ch,
                  bool   flip=false,
                  double rot=0.0);



TUNIS_STATIC_INLINE
void tunis_round_box(cairo_t *cr,
                     double x,
                     double y,
                     double w,
                     double h,
                     double r=0,  
                     double rot=0.0);

TUNIS_STATIC_INLINE
void
tunis_polygon(cairo_t *cr,
              double x,
              double y,
              double r,
              int    nb,
              double rot=0.0 );

TUNIS_STATIC_INLINE void tunis_star(cairo_t *cr,
                                    double x,
                                    double y,
                                    double r0,
                                    double r1,
                                    int    branches,
                                    double rot=0.0
                                    );

///////////////////////////////////////////////////////////////////////////

//
// Create a cairo context for the given surface using the specifid box.
// If necessary, vertical or horizontal borders may be added.
//
cairo_t *
tunis_create_context_with_size(cairo_surface_t *surf, double x1, double y1, double x2, double y2)
{
  double w = x2-x1 ;
  double h = y2-y1 ;
  cairo_t * cr = cairo_create(surf)  ;
  double ws = cairo_image_surface_get_width(surf);
  double hs = cairo_image_surface_get_height(surf);
  double scale; 
  if ( ws/w > hs/h ) {
    scale = hs/h ;
  } else {
    scale = ws/w ;
  }
  cairo_scale(cr,scale,scale) ;
  cairo_translate(cr, (ws/scale-w)/2 , (hs/scale-h)/2) ;
  return cr;
}

void
tunis_hsv_to_rgb(double h, double s, double v, double &r, double &g, double &b)
{
  h = fmod(h,360.0);
  if (h<0) h = 360.0 - h ;
  s = std::max(0.0,std::min(s,1.0)) ;
  v = std::max(0.0,std::min(v,1.0)) ;
  double c = v*s ;
  double hh = h / 60.0 ;
  double x = c*(1-fabs(fmod(hh,2.0)-1.0));
  double m = v-c ;
  if      (hh<1.0) { r = c+m ; g = x+m ; b = 0+m ; }
  else if (hh<2.0) { r = x+m ; g = c+m ; b = 0+m ; }
  else if (hh<3.0) { r = 0+m ; g = c+m ; b = x+m ; }
  else if (hh<4.0) { r = 0+m ; g = x+m ; b = c+m ; }
  else if (hh<5.0) { r = x+m ; g = 0+m ; b = c+m ; }
  else             { r = c+m ; g = 0+m ; b = x+m ; }

}

// The HSV color format = Hue Saturation Value
//
// Hue        is circular in the range 0 to 360
// Saturation is in the range [0,1]
// Value      is in the range [0,1] 
// 

// A few predefined hue constants 

static const double HUE_RED     =   0.0;
static const double HUE_ORANGE  =  30.0;
static const double HUE_YELLOW  =  60.0;
static const double HUE_LIME    =  90.0;
static const double HUE_GREEN   = 120.0;
static const double HUE_AQUA    = 150.0;
static const double HUE_CYAN    = 180.0;
static const double HUE_SKY     = 210.0;
static const double HUE_BLUE    = 240.0; 
static const double HUE_PURPLE  = 270.0; 
static const double HUE_MAGENTA = 300.0; 
static const double HUE_RUBIS   = 330.0; 

void
tunis_set_source_hsva(cairo_t *cr, double h, double s, double v, double a){
  double r,g,b ;
  tunis_hsv_to_rgb(h,s,v,r,g,b);
  cairo_set_source_rgba(cr,r,g,b,a);
}

void
tunis_set_source_hsv(cairo_t *cr, double h, double s, double v){
  double r,g,b ;
  tunis_hsv_to_rgb(h,s,v,r,g,b);
  cairo_set_source_rgb(cr,r,g,b);
}

void
tunis_circle(cairo_t *cr,double x, double y, double r)
{
  cairo_new_sub_path(cr);
  cairo_arc(cr, x, y, r, 0, 2*M_PI);
}

void
tunis_rectangle(cairo_t *cr,
                double x, double y,
                double w, double h,
                double rot)
{
  cairo_save(cr);
  cairo_translate(cr,x,y);
  cairo_rotate(cr,rot);
  cairo_rectangle(cr,0,0,w,h);
  cairo_restore(cr);
}

void
tunis_round_box(cairo_t *cr,
                double x, double y,
                double w, double h,
                double r,  
                double rot)
{
  double sz = std::min(w,h);
  if (r<=0) {
    tunis_rectangle(cr,x,y,w,h,rot);
    return;
  }
  if (w<0) { x+=w ; w=-w ;} 
  if (h<0) { y+=h ; h=-h ;} 
  if (r>sz/2) {
     //    r = sz/2 ;
  }
    
  cairo_new_sub_path(cr);
  cairo_save(cr);
  cairo_translate(cr,x,y);
  cairo_rotate(cr,rot);
  cairo_arc(cr,  r,  r, r, -2*M_PI/2, -1*M_PI/2);
  cairo_arc(cr,w-r,  r, r, -1*M_PI/2,  0*M_PI/2);
  cairo_arc(cr,w-r,h-r, r, -0*M_PI/2,  1*M_PI/2);
  cairo_arc(cr,  r,h-r, r,  1*M_PI/2,  2*M_PI/2);
  cairo_close_path(cr);
  cairo_restore(cr);
}

void
tunis_polygon(cairo_t *cr,
              double x,
              double y,
              double r,
              int    nb,
              double rot )
{
  cairo_new_sub_path(cr);
  const double da = (2*M_PI)/nb;
  double a = rot;
  for (int k=0;k<nb;k++) {
    cairo_line_to(cr, x+r*sin(a), y+r*cos(a));    
    a += da ;    
  } 
  cairo_close_path(cr);
}

void
tunis_star(cairo_t *cr,
           double x,
           double y,
           double r0,
           double r1,
           int    branches,
           double rot)
{
  cairo_new_sub_path(cr);
  const double da = M_PI/branches;
  for (int k=0;k<branches;k++) {
    cairo_line_to(cr, x+r0*sin(rot), y+r0*cos(rot));    
    rot += da ;
    cairo_line_to(cr, x+r1*sin(rot), y+r1*cos(rot));
    rot += da ;    
  }
  cairo_close_path(cr);    
}


// Bezier Quadratic path  
void
tunis_quad_to(cairo_t *cr,                 
              double x1, double y1,
              double x2, double y2 )
{
  // cairo_curve_to is Bezier Cubic and so can be used 
  // to implement a Bezier Quadratic
  double x0,y0;
  cairo_get_current_point (cr, &x0, &y0);
  return cairo_curve_to(cr,
                        (x0+2*x1)/3, (y0+2*y1)/3,
                        (2*x1+x2)/3, (2*y1+y2)/3,
                        x2,y2 );
}


std::string tunis_vsprintf(const char *fmt, va_list va)
{
  va_list va2;  
  va_copy(va2, va);

  const int len = 50; 
  char str[len+1] ;
  int size = vsnprintf(str, len+1, fmt, va);
  
  if (size < 0) {
    va_end(va2);
    return "[ERROR]" ;
  }

  if (size < len) {
    va_end(va2);
    return std::string(str,str+size);
  }

  std::unique_ptr<char[]> buf( new char[size+1] ); 
  size = vsnprintf(buf.get(), size+1, fmt, va2);
  va_end(va2);
  return std::string( buf.get(), buf.get()+size );
}

std::string tunis_sprintf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  auto out = tunis_vsprintf(fmt,ap);
  va_end(ap);
  return out;
}

void tunis_printf(cairo_t *cr, int x, int y, const char *fmt, ...)  
{
  va_list ap;
  va_start(ap, fmt);
  std::string text = tunis_vsprintf(fmt,ap);
  va_end(ap);

  cairo_move_to(cr,x,y);
  cairo_show_text(cr,text.c_str());
}

//
// Draw multiple square forming a damier pattern in the rectangle (x,y,w,h).
// The squares have dimension (cw,ch).
// The argument flip specifies which half of the squares are drawn.
//
void tunis_damier(cairo_t *cr,                                   
                  double x,
                  double y,
                  double w,
                  double h,
                  double cw,
                  double ch,
                  bool   flip,
                  double rot)
{
  cairo_save(cr);
  cairo_move_to(cr,x,y);
  cairo_rotate(cr,rot);
  int nx = int(w/cw);
  int ny = int(h/ch);
  for ( int kx=0 ; kx<nx ;kx++) 
    for ( int ky=0 ; ky<ny ;ky++)
    {
      if ( ((kx^ky)&1) == flip ) {
        cairo_rectangle(cr,kx*cw,ky*ch,cw,ch);
      }
    }      
  cairo_restore(cr);
}

static inline
void tunis_arrow(cairo_t *cr,
                 double x0, double y0,
                 double x1, double y1,
                 double size,
                 double angle)
{
  double a = trigo::polar_angle(x1-x0,y1-y0) + M_PI;

  cairo_new_sub_path(cr);
  cairo_move_to(cr,x0,y0);
  cairo_line_to(cr,x1,y1);
  cairo_close_path(cr);

  cairo_new_sub_path(cr);
  cairo_move_to(cr,x1,y1);
  cairo_line_to(cr,x1+size*cos(a+angle),y1+size*sin(a+angle));
  cairo_close_path(cr);
  
  cairo_new_sub_path(cr); 
  cairo_line_to(cr,x1,y1);
  cairo_line_to(cr,x1+size*cos(a-angle),y1+size*sin(a-angle));
  cairo_close_path(cr);
}

#endif
