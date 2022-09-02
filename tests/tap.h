#ifndef TAP_API_H
#define TAP_API_H

//
// A simple C++ API for the Test Anything Protocol (TAP)
// 
// See http://testanything.org/tap-specification.html for more details
// about that protocol. 
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>

#if __has_include(<source_location>)
#include <source_location>
using std::source_location ;
#else 
#include <experimental/source_location>
using std::experimental::source_location ;
#endif


#define TAP_HERE  source_location::current() 

// Indicate how many tests are going to be performed.
//
// This is not strictly needed but, ideally, tap_main
// should call that function once.
// 
extern void tap_plan(int count) ;

// Insert a safe comment in the output. 
// The comment may be multiline.
// The prefix is inserted in front of each line (e.g. for a nice indentation) 
extern void tap_comment(const std::string comment, const std::string prefix=" ") ;

// Record the result of a test.
//  - 'cond' indicates if the test was successful 
//  - 'msg' is the name of the test (optional)
//  - 'comment' is an additional comment that will
//     only be produced if the test fails.
//  - 'loc' is the caller source location. 
//
extern void tap_test(bool cond,
                     std::string name="",
                     const std::string &comment = "",
                     source_location loc = TAP_HERE) ; 


// Record a successful test 
//
// This is an alias for tap_test(true, msg)
//
extern void tap_pass(const std::string &msg,
                     source_location loc = TAP_HERE);

// Record a failed test 
//
// This is an alias for tap_test(false, msg)
//
extern void tap_fail(const std::string &msg,
                     source_location loc = TAP_HERE);

//
// Internal macros used in tap_assert to accept 1, 2 or 3 arguments
//
#define tap_assert_1(cond)              tap_test(cond,"",#cond) 
#define tap_assert_2(cond,msg)          tap_test(cond,msg,#cond)  
#define TAP_GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define tap_assert_chooser(...) \
  TAP_GET_4TH_ARG(__VA_ARGS__, tap_test, tap_assert_2, tap_assert_1)

// tap_assert is macro wrapper for tap_test.
//
// The macro accepts 1, 2 or 3 arguments.
//
//   tap_assert(cond)  
//     -->  tap_test(cond, "", #cond) 
//
//   tap_assert(cond, msg)
//     -->  tap_test(cond, msg, #cond) 
//
//   tap_assert(cond, msg, comment)
//     -->  tap_test(cond, msg, comment) 
//
// Macros can be confused by commas appearing in template arguments
// as shown in 
//   tap_assert( !std::is_same_v<int,double> , "int vs double");
// Instead, write
//   tap_assert( (!std::is_same_v<int,double>) , "int vs double");
//
#define tap_assert(...) tap_assert_chooser(__VA_ARGS__)(__VA_ARGS__)

//
// Abort testing. 
//
extern void tap_abort(const std::string &msg, source_location here = TAP_HERE) ;


//
// test that a and b are identical and, if not, print both values
//
template <typename T>
void
tap_same( T a, T b, const std::string &comment="", source_location loc = TAP_HERE)
{
  bool equal;

  if constexpr ( std::is_floating_point_v<T> ) {
    // Because floating point assumes than NaN is not equal to itself  
    equal = (a==b) || (std::isnan(a) && std::isnan(b)) ;
  } else {
    equal = (a==b);
  }
  
  if (equal) {
    tap_pass("",loc);
  } else {
    std::ostringstream s ;
    s.copyfmt(std::cout);
    s << "  a=" << a ;
    s << "  b=" << b ;
    s << comment ;
    tap_test(false, "", s.str(), loc);
  }
}

// 
// Each test source file should defined that function instead of main().  
//
// Its result indicates if the end of the test sequence was reached. 
//
extern bool tap_main();

#endif
