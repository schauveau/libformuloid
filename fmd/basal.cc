#include "fmd.h"
#include <cctype>
#include <charconv>
#include <system_error>

namespace fmd {

///
/// Write a floating point value (while printing formulas).
/// This is basically the output of the printf format "%.6g"
/// with the following modifications:
///   - a trailing dot is added to differentiate floating
///     point values from integral values (e.g. print "42." instead of "42")
///   - a c-style suffixes may be added to identify the floating-point type: 'f' for float and 'L' for
///     'long double'
///
/// Of course, a precision of 6 digits is not always enough
/// to represent the value accurately. However, this is fine
/// if the goal is to debug the formulas. 
///
template<std::floating_point T>
void write_floating_point(std::ostream &out, T value)
{
  if ( !std::isfinite(value) ) {
    // A nan or inf
    out << value ;
    return;
  }

  // The problem with std::to_chars is that it may fail if the buffer is too small.
  // Hopefully, unlike 'fixed', the format 'general' should never produce very
  // long numbers. If I am not mistaken the worst case for 'general' should be 
  //   - the sign character if negative.
  //   - up to 'prec' digits
  //       --> no need to use more than std::numeric_limits<T>::max_digits10
  //           9 for float, 17 for double, ...
  //   - the '.' character
  //   - and if there is an exponent
  //      - the starting 'e' .
  //      - the exponent sign
  //      - the exponent digits (up to 2 for float, 3 for double and 4 for long double?)
  // So prec+8 should be good enough even for a 128 bit floating-point. 
  static constexpr int prec = 6;
  std::array<char,prec+8+10> buf;
  auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(),
                                 value,
                                 std::chars_format::general, prec) ;
  
  if (ec != std::errc()) {
    // Hoops! plan B! Should never happen ... hopefully
    auto saved = out.flags();   
    out << std::scientific << value ;
    out.flags(saved);
    return;
  }
  
  std::string_view str(buf.data(), ptr);
  out << str;

  // Append '.' if the output looks like an integral literal.
  // For instance, transform "42" into '42.' and "-123" into "-123.".
  bool digits_only=true;
  for ( auto c : str ) {
    if ( !( (c>='0' && c<='9') || c=='-' || c=='+') ) {
      digits_only=false;
      break;
    }
  }
  if (digits_only)
    out << '.' ; 

  // And add a c-style suffix according to the floating-point type.
  if constexpr (std::same_as<T,float>) {
    out << 'f'; 
  } else if constexpr (std::same_as<T,double>) {
    // no suffix 
  } else if constexpr (std::same_as<T,long double>) {
    out << 'L'; 
  } else {
    out << '?';         
  }
  
}

/// Write a simplified string.
///
/// This is used to display strings values while dumping formuloid. 
///
///  - all non-printing characters are replaced by '?'
///  - all spaces are replaced by '_'
///  - the characters  '"', '(' and ')' could be confusing and also replaced by '?'
///  - if the string is a significantly longer than \c cut then
///    it is truncated and the missing tail is replaced by '...'
///
/// Example:
///   - write_simplified_string(out,"\thello world\n");
///      -->  ?hello_world?
///   - write_simplified_string(out,"acdefghijklmnopqrstuvwxyz",10);
///      -->  abcdefghij...
///   - write_simplified_string(out,"acdefghijkl",10);
///      -->  abcdefghijkl         (a bit longer than 10 but not enough for truncation)
///
void
write_simplified_string(std::ostream &out, const std::string &str, size_t cut=16) {
  for (size_t i=0 ; i<str.size() ; i++) {
    char c = str[i];
    if (c==' ') {
      c='_';
    } else if (c=='"' || c=='(' || c==')') {
      c='?';
    } else if (!std::isprint(static_cast<unsigned char>(c))) {
      c='?';
    }
    out << c;
    if ( i >= cut && (str.size()-i)>8 ) {
      // More than 'cut' characters were printed and we are still
      // quite far from the end. 
      out << "...";
      break;
    }
  }
}

} // end of namespace fmd 

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,bool,value) {
  out << (value ? "true" : "false")  ; 
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,char,value) {
  out << int(value) ;
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,short,value) {
  out << value;
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,int,value) {
  out << value;
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,long,value) {
  out << value << "l";
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,long long,value) {
  out << value << "ll";
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,float,value) {
  fmd::write_floating_point<float>(out,value);
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,double,value) {
  fmd::write_floating_point<double>(out,value);
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,char *,value) {
  out << '"' ;
  fmd::write_simplified_string(out,value);
  out << '"' ;
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,const char *,value) {
  out << '"' ;
  fmd::write_simplified_string(out,value);
  out << '"' ;
}

FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(out,std::string,value) {
  out << '"' ;
  fmd::write_simplified_string(out,value);
  out << '"' ;
}

