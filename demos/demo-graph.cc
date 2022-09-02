
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <memory>
#include <optional>

#include "support-cairo-sdl.h"
#include <fmd/fmd.h>

#include "Color.h"
#include "Shape.h"
#include "Source.h"

// Transparency values
#define HIDDEN  0.0
#define VISIBLE 1.0

// 'double' litterals describing an angle expressed in degrees.
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

constexpr double operator"" _t( long double t ) {
  return t;
}
struct Page {

  std::string title ;
  std::optional<fmd::func<double>> F[10]; // Up to 10 functions may be displayed (F0..F9)
  std::vector<std::string> code ;  
    
  // The display area is a 10x10 grid of blocks.
  //   xc and yc are the center coordinates.
  //   dx and dy are the size of each block.
  //   
  double xc = 0.0 ;
  double yc = 0.0 ;
  double dx = 1.0 ;
  double dy = 1.0 ;

  Page() {    
  }

} ;
  
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
//  enum { W=320, H=180 };  
  enum { W=240, H=130 };  

  // The initial physical size of the initial window. Not too big!
  enum { window_W=W*4, window_H=H*4 }; 

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

  int current_page=0 ; // Current page number (0..99)
  Page pages[100];

  int toggle_page = 0 ;
  
  int counter = 0 ;

  // The full C++ source file (if available)
  //  std::vector<std::string> source_lines ; 

  // The help text (one line per entry)
  std::vector<std::string> help ; 
  double help_scroll=0.0 ;
  double help_scroll_target=0.0 ;

  const fmd::func<double> at = fmd::identity() ;   

  int demo_number=0 ; 

  bool show_fn[10] ; 
  
public:
  
  MyDemo() : DemoPlayerBase(window_W,window_H)
  {   
    int begin_line = -1 ;
    int end_line = -1 ;
 
    load_source_file(BUILD_DIR "/" __FILE__);
 
    double NOT_USED = -9999 ;
    

    // Source lines containing those keywords will not be
    // displayed by the demo
    std::vector<std::string> discard_keywords = {
      "///",
      "page.",
      "begin_line"
    } ;
    
    for (int pn=0 ; pn<100 ; pn++)
    {
      Page &page = pages[pn] ;

      
      fmd::func<double> F[10] ;

      for (int i=1;i<9;i++) {
        F[i] = NOT_USED ;
      }
      
      fmd::func<double> &F1 = F[1];
      fmd::func<double> &F2 = F[2];
      fmd::func<double> &F3 = F[3];
      fmd::func<double> &F4 = F[4];
      fmd::func<double> &F5 = F[5];
      fmd::func<double> &F6 = F[6];
      fmd::func<double> &F7 = F[7];
      fmd::func<double> &F8 = F[8];
      fmd::func<double> &F9 = F[9];
      
      begin_line = -1;
      end_line   = -1;

      enum {
        pg_transitions_1,
        pg_transitions_2,
        pg_foo1,
        pg_foo2,
      };
      
#define PAGE(n, t) case n: { pages[pn].title = t ; begin_line=__LINE__+1 ; 
#define END_PAGE   end_line=__LINE__-1 ; } break 

      switch(pn) {
        
        PAGE(pg_transitions_1,"Transitions 1") {
          double to = 3.0;

          F1 = F2 = F3 = F4 = F5 = F6 = F7 = F8 = F9 = 2.0 ;

          // join without any transition 
          F1.join(1.0_t, to) ;
          
          // morph with default easing 
          F2.morph(2.0_t, to) ;
          
          // Overshoot by about 5%. 
          F3.morph(3.0_t, to, fmd::ease_inout_back05) ;
          
          // Overshoot by about 10%. 
          F4.morph(4.0_t, to, fmd::ease_inout_back10) ; 

          // Overshoot by about 20%. 
          F5.morph(5.0_t, to, fmd::ease_inout_back20) ;

          // Overshoot by about 30%. 
          F6.morph(6.0_t, to, fmd::ease_inout_back30) ;

          F7.morph(7.0_t, to, fmd::ease_inout_back30) ;

          // Transit is linear by default
          F8.transit(8.0_t, 9.0_t, to) ;

          // Transit with easing
          F9.transit(9.0_t, 10.0_t, to, fmd::ease_inout_back30) ;

          page.dx = page.dy =1.0;
          page.xc = 5.0;
          page.yc = 3.0;          
        } END_PAGE;

        
        PAGE(pg_transitions_2,"Transitions 2") {

          F1 = 2.0 + fmd::cos(fmd::id*3.0) ;
          F2 = 5.0 + fmd::cos(fmd::id*8.0) ;

          // F3 starts as F1
          // then transits to F2 in 0.5s
          // then transits back to F1 in 1.5s           
          F3 = F1;
          F3.transit(3.0_t, 4.0_t, F2, fmd::ease_default) ;
          F3.transit(6.0_t, 7.0_t, F1, fmd::ease_default) ;
          
          // A similar effect can be achieved with morphing by only specifying
          // when the transition should occur. The default ease method is
          // of length 1.0 and centered around the specified position.           
          F4 = F1;
          F4.morph(3.5_t, F2, fmd::ease_default);
          F4.morph(6.5_t, F1, fmd::ease_default);

          // Shift the F3 and F4 curves down for clarity
          F3 = F3-0.3;
          F4 = F4-0.6;
         
          page.dx = page.dy =1.0;
          page.xc = 5.0;
          page.yc = 3.0;          
        } END_PAGE;
        
        PAGE(pg_foo1,"paglop") {
          F1 = fmd::floor_dist(fmd::id);

          F2 = 6.0;

          F3 = (F1+F2);
          F3.eval(0);

          
          F5 = clamp(sin(at), -0.2, 0.5);
          F5 = (F5(at-0.5) + 2.0*F5(at) + F5(at+0.5))/4.0;

          F7 = fmd::bezier_cubic(0.0, 1.0, 0.0, 2.0, -1.0, 1.0, fmd::id) ;          
          F8 =
            fmd::lerp(2.4,3.4,F2)
            + fmd::lerp(F2,3.4,1.2)
            + fmd::lerp(2.4,F2,3.0)
            + fmd::lerp(2.4,F2,F2)
            + fmd::lerp(F2,F2,3.2)
            + fmd::lerp(F2,F2,F2);

        } END_PAGE;
        
        PAGE(pg_foo2,"paglop") {
          page.xc = page.yc = 5.0 ;
          F1 = abs(sin(fmd::id)*6.0);
        } END_PAGE;        

        default:
          break;
      }

      for (int i=1;i<=9;i++) {
        if (F[i][0.0]!=NOT_USED)
          page.F[i] = F[i] ;
      } 

      if (begin_line != -1) { 
        append_from_source(page.code, begin_line, end_line, discard_keywords, 9);
      }
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
      case SDLK_F2:
      case SDLK_F3:
      case SDLK_F4:
      case SDLK_F5:
      case SDLK_F6:
      case SDLK_F7:
      case SDLK_F8:
      case SDLK_F9:
        {
          int i = key-SDLK_F1+1 ;         
          show_fn[i] = !show_fn[i];
        }
        break;
      case SDLK_h: // Hide all       
        {
          for (int i=0;i<10;i++)
            show_fn[i] = false ;          
        }
        break;
      case SDLK_s: // Show all       
        {
          for (int i=0;i<10;i++)
            show_fn[i] = true ; 
        }
        break;
       case SDLK_r: // Reset       
        setup();
        break;
      case SDLK_RIGHT:
        toggle_page = current_page;
        do {
          current_page=(current_page+1)%100;
        } while( pages[current_page].title.empty() ) ;
        setup();
        break;
      case SDLK_LEFT:
        toggle_page = current_page;
        do {
          current_page=(current_page+99)%100;
        } while( pages[current_page].title.empty() ); 
        setup();
        break;
      case SDLK_0:
      case SDLK_1:
      case SDLK_2:
      case SDLK_3:
      case SDLK_4:
      case SDLK_5:
      case SDLK_6:
      case SDLK_7:
      case SDLK_8:
      case SDLK_9:
        { 
          toggle_page = (toggle_page % 10)*10 + (key-SDLK_0);
        }
        break ;
      case SDLK_RETURN:
      {
        std::swap(current_page,toggle_page) ;
        setup();
      }
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
 
  virtual double setup() override
  {
    help.clear();
    help = pages[current_page].code;
    
    for (int i=0;i<10;i++)
      show_fn[i] = true;
    
    return 10;
  }
  
  void paint_info_area(cairo_t *cr, double /*t*/ )
  {
    Page &page = pages[current_page]; 
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
    
    tunis_printf(cr, 0, 1*fs, "[%02d] %s" , current_page, page.title.c_str() );
    tunis_printf(cr, 0, 2*fs, "-> toggle to [%02d]" , toggle_page );
    
    
    cairo_restore(cr) ;
  }

  // Paint the demo area
  //
  // Reminder: the demo area is of size 100x100
  //
  void paint_demo_area(cairo_t *cr, double /*t*/ ) 
  {
    assert( Wd==100.0 );
    assert( Hd==100.0 );
    cairo_save(cr);
    cairo_translate(cr, Xd, Yd) ;
    cairo_rectangle(cr, 0, 0, Wd, Hd);
    cairo_clip(cr);

    double blocksize = 10.0 ;
    // Fill with a 10x10 checkboard of 10x10 squares
    cairo_set_source_rgb(cr, 0.2, 0.3, 0.2);
    cairo_rectangle(cr, 0, 0, 100, 100);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.3);
    tunis_damier(cr, 0, 0, 100, 100, blocksize, blocksize);
    cairo_fill(cr);

    Page &page = pages[current_page];

    double rgb[10][3] {
      {1.0,1.0,1.0},
      {1.0,0.0,0.0},
      {0.0,1.0,0.0},
      {0.0,0.0,1.0},
      {0.0,1.0,1.0},
      {1.0,0.0,1.0},
      {1.0,1.0,0.0},
      {1.0,0.6,0.3},
      {0.6,0.3,1.0},
      {0.3,1.0,0.6}
    };    

    double fs = 4.0;
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,  CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, fs);
    
    // The X horizontal axis
    cairo_set_source_rgba(cr, 1.0, 0.7, 0.3, 0.8);
    cairo_set_line_width(cr,0.3);
    cairo_move_to(cr,   0.0, 95.0);
    cairo_line_to(cr, 100.0, 95.0);
    cairo_stroke(cr);
    for (int k=-4; k<=4 ; k+=2 ) {
      char text[20];
      double value = page.xc+k*page.dx ;
      if (value==0)
        sprintf(text,"0");
      else
        sprintf(text,"%+3g",value);
      // Center alignment
      cairo_text_extents_t extents;
      cairo_text_extents (cr, text, &extents);
      double align_x = -(extents.width/2 + extents.x_bearing);
      double align_y = -(extents.height/2 + extents.y_bearing);
      cairo_move_to(cr, Xd+(k+5)*blocksize+align_x , 97.0+align_y );      
      cairo_show_text(cr, text);
    }
    cairo_stroke(cr);
            

    // The Y horizontal axis
    cairo_set_source_rgba(cr, 1.0, 0.7, 0.3, 0.8);
    cairo_set_line_width(cr,0.3);
    cairo_move_to(cr,  5.0, 100.0);
    cairo_line_to(cr,  5.0,   0.0);
    cairo_stroke(cr);

    for (int k=-4; k<=4 ; k+=2 ) {
      char text[20];
      double value = page.yc-k*page.dy; 
      if (value==0)
        sprintf(text,"0");
      else
        sprintf(text,"%+3g", value);
      // Center alignment
      cairo_text_extents_t extents;
      cairo_text_extents (cr, text, &extents);
      double align_x = -(extents.width/2 + extents.x_bearing);
      double align_y = -(extents.height/2 + extents.y_bearing);
      cairo_move_to(cr, 5.0+align_x , Yd+(k+5)*blocksize+align_y );      
      cairo_show_text(cr, text);
    }
    cairo_stroke(cr);
    
    // Macro to convert logical coordinates to screen coordinates
    // 
#define X(x) (Wd/2 + 10.0*((x)-page.xc)/page.dx )
#define Y(y) (Hd/2 - 10.0*((y)-page.yc)/page.dy )
    
    for (int i=0;i<10;i++) {
      if ( page.F[i].has_value() && show_fn[i]) {

        cairo_set_source_rgb(cr, rgb[i][0], rgb[i][1], rgb[i][2] ) ;
        cairo_set_line_width(cr,0.5);
        fmd::func<double> f = page.F[i].value();

        for ( double s = -Wd/2; s <= Wd/2 ; s+=0.5) {
          double x = page.xc + (s*page.dx)/blocksize ;
          cairo_line_to(cr, X(x), Y(f[x]));
        }

        cairo_stroke(cr);
      }
    }
    
    cairo_restore(cr) ;
  }

  void paint_help_area(cairo_t *cr)
  {
    cairo_save(cr);
    size_t n = help.size();
    double fs = 3.0;
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


int
main()
{
  SDL_Init(SDL_INIT_VIDEO);
  MyDemo().run();  
  SDL_Quit();
  return 0;
}
