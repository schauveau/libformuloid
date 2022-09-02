#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>

#include "tunis.h"

class DemoPlayerBase {
public:
  SDL_Window   * sdl_window=0 ;
  SDL_Renderer * sdl_renderer=0 ;
  double         default_w;
  double         default_h;
  double         fps = 0.0;

private:
  bool           stop_demo = false;
  bool           restart_demo = true;
  double         demo_time; // The time within the demo
  double         demo_speed = 1.0;
  double         demo_duration;
  int            fps_count = 0; 
  double         fps_delay = 0.3;
  std::chrono::time_point<std::chrono::steady_clock> last_time; 
  std::chrono::time_point<std::chrono::steady_clock> fps_start; 
private:

  void updateTimers() {
    // Update the demo time and the fps 
    std::chrono::time_point<std::chrono::steady_clock> now;
    now = std::chrono::steady_clock::now();
    if (restart_demo) {
      restart_demo = false;
      this->demo_time  = 0.0;
      fps_start = now;
      fps_count = 0;
    } else {
      std::chrono::duration<double> elapsed = now - last_time;
      this->demo_time += elapsed.count() * this->demo_speed ;
    }
    std::chrono::duration<double> elapsed_fps = now - fps_start;
    if ( elapsed_fps.count() > fps_delay ) {
      fps = fps_count / elapsed_fps.count() ;
      fps_start = now;
      fps_count = 0;
    }    
    fps_count++;
    last_time=now;

    if ( this->demo_time > this->demo_duration ) {
      this->demo_time = 0 ;
    }
    
  }

public:
  
  DemoPlayerBase(int w, int h) :
    default_w(w),
    default_h(h)
  {
  }

  // (re)configure the demo.
  //
  // Return the demo duration in seconds.
  // A nul or negtive duration means infinite.
  // 
  virtual double setup() = 0 ;
  
  double getTime() {
    return demo_time; 
  }
  
  double getFps() {
    return fps; 
  }

  double getDuration() {
    return demo_duration; 
  }
  
  virtual ~DemoPlayerBase() {    
    if (sdl_renderer)
      SDL_DestroyRenderer(sdl_renderer);
    
    if (sdl_window) 
      SDL_DestroyWindow(sdl_window);
  }
  
  void draw_png(int w, int h)
  {
    cairo_surface_t *cr_surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h);
    this->paint(cr_surface);
    cairo_surface_write_to_png (cr_surface, "out.png");
    cairo_surface_destroy(cr_surface);
  }
  
  void refresh() {
    
    int width;
    int height;
    
    SDL_GetRendererOutputSize(sdl_renderer, &width, &height);
    
    // Reminder:
    //   The only 32bit pixel format without alpha supporte by Cairo is CAIRO_FORMAT_RGB24.
    //   From https://developer.gnome.org/cairo/stable/cairo-Image-Surfaces.html#cairo-format-t
    //     CAIRO_FORMAT_RGB24
    //	     each pixel is a 32-bit quantity, with the upper 8 bits unused. Red, Green,
    //         and Blue are stored in the remaining 24 bits in that order.
    //
    //   In practice, that means that in the SDL texture  the R, G and B masks must be set to 0x00FF000,
    //   0x0000F00 and 0x00000FF.
    //
    
    SDL_Surface *sdl_surface = SDL_CreateRGBSurface(0,
                                                    width,
                                                    height,
                                                    32,
                                                    0x00ff0000,
                                                    0x0000ff00,
                                                    0x000000ff,
                                                    0);
    
    
    cairo_surface_t *cr_surface;
    cr_surface = cairo_image_surface_create_for_data((unsigned char *)sdl_surface->pixels,
                                                     CAIRO_FORMAT_RGB24,
                                                     sdl_surface->w,
                                                     sdl_surface->h,
                                                     sdl_surface->pitch);
    
    
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    this->updateTimers();    
    this->paint(cr_surface);  
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer, sdl_surface);
    SDL_FreeSurface(sdl_surface);
    
    SDL_RenderCopy(sdl_renderer, texture, NULL, NULL);
    SDL_RenderPresent(sdl_renderer);
    
    cairo_surface_destroy(cr_surface);
    
    SDL_DestroyTexture(texture);
    
  }  

  //
  // Handle a SDL keypress. 
  // Return true if the key was known.
  //  
  virtual bool
  keypress(SDL_Keycode sym, SDL_Keymod mod)
  {
    // SDL_Keycode is 32 bit signed so we store the 
    // relevant modifiers in the upper bits 
    const uint64_t CTRL  = uint64_t(1) << 32;
    
    uint64_t key = sym & 0x7FFFFFFF; 
    
    if ( mod & KMOD_CTRL )
      key += CTRL;
    
    switch(key) {
      case CTRL+SDLK_q:
      case SDLK_ESCAPE:
      case CTRL+'c':
        stop_demo = true;
        break;
      case SDLK_SPACE:
        if (demo_speed==0.0)
          demo_speed = 1.0;
        else
          demo_speed = 0.0;                  
        break; 
      case CTRL+'s':
        draw_png(default_w,default_h);
        break;
      default:
        return false;
    }
    return true;
  }


  void run() {

    this->demo_duration = this->setup();
    if (demo_duration <= 0)
      demo_duration = std::numeric_limits<double>::infinity(); 
    
    sdl_window = SDL_CreateWindow("Vop demo",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  default_w,
                                  default_h,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE) ;
    if (!sdl_window) {
      std::cerr << "ERROR: Failed to create SDL window" << std::endl;
      goto done;
    }
    
    sdl_renderer = SDL_CreateRenderer(sdl_window,
                                      -1,
                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!sdl_window) {
      std::cerr << "ERROR: Failed to create SDL renderer" << std::endl;
      goto done;
    }
    
    refresh();

    while (!stop_demo) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          
          case SDL_QUIT:
            stop_demo = true;
            break;
            
          case SDL_KEYDOWN:
            this->keypress(event.key.keysym.sym, (SDL_Keymod)event.key.keysym.mod);
            break;            
            
          case SDL_WINDOWEVENT:
            // TODO?
            break;
            
          default:
            break;
        }
      }
      // SDL_Delay(1);
      refresh();
    }
    
    done:

    if (sdl_renderer)
      SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer=0 ;
    
    if (sdl_window) 
      SDL_DestroyWindow(sdl_window);
    sdl_window=0 ;
    
  }
  
  virtual void paint(cairo_surface_t *cr_surface) {

    const int W=600;
    const int H=400;
    cairo_t *cr = tunis_create_context_with_size(cr_surface, 0, 0, W, H) ;

    tunis_set_source_hsv(cr, HUE_BLUE, 0.70, 0.10);
    cairo_paint(cr);

    cairo_rectangle(cr, 0, 0, W, H);
    cairo_clip(cr);
    
    tunis_set_source_hsv(cr, HUE_GREEN, 0.34, 0.23);
    cairo_paint(cr);

    tunis_set_source_hsv(cr, HUE_RED, 1.0, 1.0);
    cairo_rectangle(cr, 0, 0, W, H);
    cairo_set_line_width(cr, 20);
    cairo_stroke(cr);
  
    static double hue = 30.0 ;
    tunis_set_source_hsva(cr, hue, 0.3, 0.4, 1.0);
    hue = hue + 1.0;
    cairo_rectangle(cr, W*0.2, H*0.2, W*0.6, H*0.6);
    cairo_fill(cr);    
    
    cairo_destroy(cr);
  }
  
};

