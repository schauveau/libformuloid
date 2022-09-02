
enum class Shape { CIRCLE, SQUARE, TRIANGLE, STAR, ARROW , HOUSE } ;



struct ShapeObject {
  
  void init(double x, double y, Shape kind, double size, Color col, double alpha)
  {
    this->x       = x;
    this->y       = y;
    this->size    = size;
    this->rot     = 0.0;
    this->alpha   = alpha;
    this->color_r = col.r;
    this->color_g = col.g;
    this->color_b = col.b;
    this->kind    = kind;
  }

  fmd::func<Shape>  kind ;   // The kind of shape to draw
  
  fmd::func<double> x,y ;   // Coordinates 
  fmd::func<double> rot ;   // Rotation
  fmd::func<double> size ;  // The size (interpretation varies with the kinds) 
  fmd::func<double> alpha ; // Transparency

  fmd::func<double> color_r; 
  fmd::func<double> color_g; 
  fmd::func<double> color_b; 
  
  void draw(cairo_t *cr, double t) {
    double a = alpha[t];
    if (a <= 0.0)
      return;            
    cairo_set_source_rgba(cr, color_r[t], color_g[t], color_b[t], a);
    ShapeObject::render(cr, kind[t], size[t], x[t], y[t], rot[t]);
  }

  inline static void
  render(cairo_t *cr,
         Shape kind,
         double size,
         double x, double y,
         double rot )
  {
    cairo_save(cr);    
    cairo_translate(cr, x, y);
    cairo_rotate(cr, rot);
    
    switch( kind ) {            
      case Shape::CIRCLE:
      default:
        tunis_circle(cr, 0.0, 0.0, size/2.0 );
        break;
      case Shape::SQUARE:
        cairo_rectangle( cr, -size/2.0, -size/2.0, size , size ); 
        break;
      case Shape::TRIANGLE:
        tunis_polygon(cr, 0.0, 0.0, size/2.0, 3, 0.0); 
        break;
      case Shape::STAR:
        tunis_star( cr, 0.0, 0.0, size/2.0 , size/4.0, 6) ;
        break;
      case Shape::HOUSE:
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
  
} ;


