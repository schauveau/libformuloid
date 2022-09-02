#ifndef COLOR_ENUM_H
#define COLOR_ENUM_H

enum class color {
  red=3,
  green=11,
  blue=23,
  yellow=34,
  white=45,
  black=67
} ;

inline std::ostream &
operator<<( std::ostream &s , color v)
{
  switch(v) {
    case color::red:    s<<"red" ; break;
    case color::green:  s<<"green" ; break;
    case color::blue:   s<<"blue" ; break;
    case color::yellow: s<<"yellow" ; break;
    case color::white:  s<<"write" ; break;
    case color::black:  s<<"black" ; break;
    default:
      s << "***UNKNOWN***" ; break;
  }
  return s;
}

#endif
