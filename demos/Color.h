
struct Color {

  Color(unsigned rgb) {
    r = ( ( rgb >> 16 ) & 0xFF ) / 255.0 ;
    g = ( ( rgb >> 8  ) & 0xFF ) / 255.0 ;
    b = ( ( rgb       ) & 0xFF ) / 255.0 ;
  }

  Color(unsigned r, unsigned g, unsigned b) :
    r(r/255.0),
    g(g/255.0),
    b(b/255.0)
  {    
  }

  Color(double r, double g, double b) : r(r) , g(g), b(b) { 
  }
  
  double r,g,b ;  
} ;

Color COL_RED(1.0, 0.2, 0.2) ;
Color COL_GREEN(0.2, 0.8, 0.2) ;
Color COL_BLUE(0.2, 0.2, 1.0) ;
Color COL_YELLOW(1.0, 1.0, 0.2) ;
Color COL_BLACK(0.2, 0.2, 0.2) ;
Color COL_WHITE(1.0, 1.0, 1.0) ;
Color COL_PURPLE(1.0, 0.2, 1.0) ;
Color COL_CYAN(0.2, 1.0, 1.0) ;
