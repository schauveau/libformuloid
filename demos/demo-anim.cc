
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <memory>

#include "support-cairo-sdl.h"
#include <fmd/fmd.h>

#include "Color.h"
#include "Shape.h"


// Transparency values
#define HIDDEN  0.0
#define VISIBLE 1.0


double operator "" _t (long double v) {
  return v;
}
// _deg suffix is for 'double' litterals describing an angle
// expressed in degrees.
//
// For example,
//    30_deg is equivalent to M_PI/6
//    0.5_deg is equivalent to M_PI/360
//

constexpr double operator"" _deg( long double angle ) {
  return angle * (M_PI/180);
}
constexpr double operator"" _deg( unsigned long long int angle ) {
  return angle * (M_PI/180);
}

class MyDemo : public DemoPlayerBase
{
public:



  //  The screen is organized as follow:
  //
  //       DDHHHH
  //       DDHHHH
  //       IIHHHH
  //
  //  D is the demo area (always 100x100 in )
  //  H is the help area (text & source code)
  //  I is the info area   
  //  

  // The top left corner of size 100x100 contains the actual animation.
  // a 16/9 ratio should fit well on most screen
  // The rest is used to display C++ source and various information.
  
  // The 'virtual' size of the screen.
  enum { W=320, H=180 };  

  // The initial physical size of the initial window. Not too big!
  enum { window_W=W*3, window_H=H*3 }; 

  // Position and dimensions of the Demo area  (virtual coordinates)
  static const int Xd=0;
  static const int Yd=0;
  static const int Wd=100;
  static const int Hd=100;

  // position and dimension of the Help area    
  static const int Xh=Xd+Wd;
  static const int Yh=0;
  static const int Wh=W-Xh;
  static const int Hh=H;

  // position and dimension of the Info area    
  static const int Xi=0;
  static const int Yi=Hd;
  static const int Wi=Wd;
  static const int Hi=H-Hd;
  
  
  int counter = 0 ;

  // The full C++ source file (if available)
  std::vector<std::string> source_lines ; 

  // The help text (one line per entry)
  std::vector<std::string> help ; 
  double help_scroll=0.0 ;
  double help_scroll_target=0.0 ;

  // Each demo may use up to 8 shapes
  ShapeObject S[8]; 
  // and some aliases to make the code more readable 
  ShapeObject &s0 = S[0], &s1 = S[1], &s2 = S[2];
  ShapeObject &s3 = S[3], &s4 = S[4], &s5 = S[5];
  
  const fmd::func<double> at = fmd::identity() ;   

  int demo_number=0 ; 
  
public:
  
  MyDemo() : DemoPlayerBase(window_W,window_H)
  {
    const int begin = __LINE__ ;
    // aaaaaaaaaaaaaaaaa
    load_source_file();
    help_text("DEMO 1\n------------\nxxxxx");
    // zzzzzzzzzzzzzzzzz
    const int end = __LINE__ ;
    add_help_source(begin,end);
  }

  // Try to load the C++ source file in array source_lines.  
  void load_source_file() {
    const char *filename = BUILD_DIR "/" __FILE__ ;
    std::cout << filename << "\n"; 
    std::ifstream fin(filename) ;
    if (!fin.good()) {
      std::cerr << "WARNING: Source file " << filename << " not found! Code will not be displayed\n" ;
    }
    std::string line;
    source_lines.push_back(""); // Dummy first line
    while(getline(fin,line)){
      source_lines.push_back(line);
    }
  }
  
  virtual bool
  keypress(SDL_Keycode sym, SDL_Keymod mod) override
  {
    if (this->DemoPlayerBase::keypress(sym,mod)) {
      return true;
    }

    const uint64_t CTRL  = uint64_t(1) << 32;

    uint64_t key = sym & 0x7FFFFFFF; 
    if ( mod & KMOD_CTRL )
      key += CTRL;
    
    switch(key) {
      case CTRL+SDLK_a:
        std::cout << "CTRL-a\n";
        break;
      case SDLK_F1:
        std::cout << "F1\n";
        break;
      default:
        break;
    }
    return true;      
  }

  void help_text(std::string str) {
#if 1
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find('\n', prev)) != std::string::npos)
    {
        help.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    help.push_back(str.substr(prev));
#else   
    help.push_back(str) ;
#endif
  }

  // Add the current source file lines ranging from begin to
  // end (both excluded) to the help text.
  void add_help_source(size_t begin, size_t end) {
    begin = std::max(begin, size_t(0) );
    end   = std::min(end, source_lines.size() );
    for ( size_t i = begin+1 ; i<end ; i++ ) {
      help.push_back(source_lines[i]) ; 
    }

  }

  double setup_0() {

    using fmd::id ;
    
    fmd::func<double> f1 = fmd::sine_wave(0.3, 4.5) ;
    std::cout << "id = " << id << "\n";
    std::cout << "f1 = " << f1 << "\n";
    fmd::func<double> f2 = fmd::sine_wave(0.3, 4.5, 3.4) ;
    std::cout << "f2 = " << f2 << "\n";
    fmd::func<std::string> a = std::string("ab\ncd sfsf sf sf s fs df sdf s f sdf sdfsdfs ");

    std::cout << "xxx = " << a << "\n";

    std::cout << "xxx = " << ((f1==f2)==true) << "\n";

    std::cout << "f1(id+1.0) = " << f1(id+1.0) << "\n";
    auto f3 = fmd::sine_wave(0.3, 4.5, fmd::func<double>(3.4)) ;
    std::cout << "f3 = " << f3 << "\n";
    std::cout << "f3 = " << f3.eval(0.0) << "\n";
    std::cout << "f3 = " << f3 << "\n";
    auto f4 = fmd::sine_wave(0.3,4.5,fmd::id+1.0) ;
    std::cout << "f4 = " << f4 << "\n";
    std::cout << "f4 = " << f4.eval(2.0) << "\n";
    std::cout << "f4 = " << f4 << "\n";    

    std::cout << "f1(f4) = " << fmd::pp(f1(f4)) << "\n";

    const int begin = __LINE__+1 ;
    {
      s0.init(10.0, 20.0, Shape::CIRCLE, 7.0, COL_YELLOW, VISIBLE );
      s1.init(50.0, 40.0, Shape::HOUSE,  7.0, COL_RED, VISIBLE );
      s2.init(10.0, 60.0, Shape::SQUARE, 4.0, COL_GREEN, VISIBLE );
      s3.init( 0.0,  0.0, Shape::STAR,   4.0, COL_BLUE, VISIBLE );
      s4.init( 0.0,  0.0, Shape::STAR,   4.0, COL_WHITE, VISIBLE );
      s5.init( 0.0,  0.0, Shape::CIRCLE, 4.0, COL_CYAN, VISIBLE );
      
      // The yellow circle:
      // At t=4s, move to right side while becoming purple
      s0.x.morph( 4.0, 90.0 );    
      s0.color_r.morph( 4.0 , COL_PURPLE.r );
      s0.color_g.morph( 4.0 , COL_PURPLE.g );
      s0.color_b.morph( 4.0 , COL_PURPLE.b );
      // At t=6s, fade away 
      s0.alpha.morph(6.0, 0.0);
      
      // The red house: 
      // At t=3s, initiate a full counter-clockwise rotation
      // that 'overshoot' a little in 4s.
      s1.rot.morph(3.0, -2*M_PI, fmd::ease_out_back20.after()*4.0);
      
      // The green square:
      // Its size oscillates between 3.0 and 13.0 every 2 seconds.
      //  s2.size = 3.0 + 10.0 * fmd::Abs(fmd::Sin(at)) ;
      s2.size = fmd::sine_wave_between(0.0, 3.0, 2.0, 13.0) ;
      
      // At t=2s, move gently to the bottom-center in 1s.
      s2.x.morph( 2.0 , 80.0 , fmd::ease_inout);  
      s2.y.morph( 2.0 , 80.0 , fmd::ease_inout);
      // At t=4s, move gently to the bottom-left in 1s.
      s2.x.morph( 4.0 , 20.0 , fmd::ease_inout);  
      s2.y.morph( 4.0 , 80.0 , fmd::ease_inout);
      // At t=6s, move back to the initial position but in 0.2s
      s2.x.morph( 6.0 , s2.x[0] , fmd::ease_inout.resize(0.2) );  
      s2.y.morph( 6.0 , s2.y[0] , fmd::ease_inout.resize(0.2) );
      
      // The blue and white stars:
      // Follow the green square but with a small delay
      s3.x = s2.x(at-0.1); s3.y = s2.y(at-0.1); // after 0.1s
      s4.x = s2.x(at-0.2); s4.y = s2.y(at-0.2); // after 0.2s

      // The cyan circle:
      // Orbit the green square in a non-regular manner
      s5.x = s2.x + sin(at*16.0)*10.0;
      s5.y = s2.y + cos(at*12.0)*10.0;

#if 0
      {
        fmd::context ctxt; 
        fmd::func<double> a = ctxt.add(s5.x*2.0);
        fmd::func<double> b = ctxt.add(s5.y*2.0);
        s5.x = ctxt.eval(a+b) ;
      }
      
#endif
      
      
    }
    const int end = __LINE__-1 ;
    add_help_source(begin,end);

    std::cout << fmd::lerp(3.0,4.5,at) << "\n"; 

    
    std::cout << fmd::internal::get_typename<void*>() << "\n";
    std::cout << fmd::internal::get_typename<char[4]>() << "\n";

    return 10 ;
  }

  virtual double setup() override
  {

    help.clear();

    for (int k=0;k<8;k++)
      S[k].init(50.0, 10+k*10, Shape::CIRCLE, 4.0, COL_WHITE, HIDDEN );
    
    while (true) {
      switch(demo_number) {
        case 0:  return setup_0() ;        
      }
      demo_number++ ;
      if (demo_number==100)
        demo_number=0;
    }
  }

#if 0
  void draw_shape( cairo_t *cr, Kind kind, double size,
                   double x, double y,
                   double rot )
  {
    cairo_save(cr);    
    cairo_translate(cr, x, y);
    cairo_rotate(cr, rot);

    switch( kind ) {            
      case CIRCLE:
      default:
        tunis_circle(cr, 0.0, 0.0, size/2.0 );
        break;
      case SQUARE:
        cairo_rectangle( cr, -size/2.0, -size/2.0, size , size ); 
        break;
      case TRIANGLE:
        tunis_polygon(cr, 0.0, 0.0, size/2.0, 3, 0.0); 
        break;
      case STAR:
        tunis_star( cr, 0.0, 0.0, size/2.0 , size/4.0, 6) ;
        break;
      case HOUSE:
        cairo_new_sub_path(cr);
        cairo_line_to(cr, -size/2.0, -size/2.0);
        cairo_line_to(cr,         0, -size*0.8);
        cairo_line_to(cr, +size/2.0, -size/2.0);
        cairo_line_to(cr, +size/2.0, +size/2.0);
        cairo_line_to(cr, +size/7.0, +size/2.0);        
        cairo_line_to(cr, +size/7.0, +size*0.1);        
        cairo_line_to(cr, -size/7.0, +size*0.1);        
        cairo_line_to(cr, -size/7.0, +size/2.0);        
        cairo_line_to(cr, -size/2.0, +size/2.0);
        cairo_close_path(cr);
    }
    cairo_fill(cr);
    cairo_restore(cr);
  }
#endif
  
  void paint_info_area(cairo_t *cr,double t)
  {
    cairo_save(cr);
    cairo_translate(cr, Xi, Yi) ;
    cairo_rectangle(cr, 0, 0, Wi, Hi);
    cairo_clip(cr);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.5);
    cairo_rectangle(cr, 0, 0, Wi, Hi);
    cairo_fill(cr);

    
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,  CAIRO_FONT_WEIGHT_BOLD);
    double fs = 4.0;
    cairo_set_font_size(cr, fs);
    
    tunis_printf(cr, 10, fs, "        x      y  rot alpha" );
    
    for (int i=0;i<8;i++) {      
      auto &s = S[i];
      double y = (i+2)*fs ;
      cairo_set_source_rgb(cr, s.color_r[t], s.color_g[t], s.color_b[t]);
      ShapeObject::render(cr, s.kind[t], fs*0.7, fs, y-fs*0.35, s.rot[t]) ;
      int rot_deg = round(s.rot[t]*180/M_PI);
      int alpha_pc =  int(s.alpha[t]*100) ; 
      tunis_printf(cr, 10, y,
                   "s%d %6.2f %6.2f %+4d° %3d%%",
                   i+1,
                   s.x[t], s.y[t],
                   rot_deg,
                   alpha_pc);      
      cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
      
#if 0
          this->x       = x;
          this->y       = y;
          this->size    = size;
          this->rot     = 0.0;
          this->alpha   = alpha;
          this->color_r = col.r;
          this->color_g = col.g;
          this->color_b = col.b;
          this->kind    = kind;
          this->effect  = effect;
#endif
    }
    
    cairo_restore(cr) ;
  }

  // Paint the demo area
  //
  // Reminder: the demo area is of size 100x100
  //
  void paint_demo_area(cairo_t *cr,double t) 
  {
    assert( Wd==100.0 );
    assert( Hd==100.0 );
    cairo_save(cr);
    cairo_translate(cr, Xd, Yd) ;
    cairo_rectangle(cr, 0, 0, Wd, Hd);
    cairo_clip(cr);

    // Fill with a 10x10 checkboard of 10x10 squares
    cairo_set_source_rgb(cr, 0.2, 0.3, 0.2);
    cairo_rectangle(cr, 0, 0, 100, 100);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.3);
    tunis_damier(cr, 0, 0, 100, 100, 10, 10);
    cairo_fill(cr);

    // And draw the 8 shapes as requested
    for (int i=0;i<8;i++) {
      S[i].draw(cr,t);
    }
    
    cairo_restore(cr) ;
  }

  void paint_help_area(cairo_t *cr)
  {
    cairo_save(cr);
    size_t n = help.size();
    double fs = 4.0;
    double x0 = Xh ;
    double y0 = Yh ;
    cairo_set_source_rgb(cr, 1.0, 1.0, 0.5); 
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,  CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, fs);
    for (size_t i=0; i<n; i++) {
      cairo_move_to(cr, x0, y0+(i+1)*fs);      
      cairo_show_text(cr, help[i].c_str());      
    }    
    cairo_restore(cr);
  }
  
  virtual void paint(cairo_surface_t *cr_surface) override
  {
    double t = this->getTime(); 
    cairo_t *cr = tunis_create_context_with_size(cr_surface, 0, 0, W, H );        

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);
           
    paint_help_area(cr);
    paint_demo_area(cr,t);
    paint_info_area(cr,t);
    
    if (true) {
      double fs; // the font size
      fs = 4.5;
      cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); 
      cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,  CAIRO_FONT_WEIGHT_BOLD);
      cairo_set_font_size(cr, fs);
      tunis_printf(cr, 20, 2*fs, "Time %6.1f/%-.1f  FPS %3.1f",
                   this->getTime(),
                   this->getDuration(),
                   this->getFps() ) ;
    }
        
    cairo_destroy(cr);
  }

};


int main()
{
  SDL_Init(SDL_INIT_VIDEO);
  MyDemo().run();  
  SDL_Quit();
  return 0;
}
