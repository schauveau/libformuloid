#include "tap.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>

static int tap_current_plan=0 ;
static bool atexit_ok = false ;

static std::vector<std::string>
split_string(const std::string& str,
             const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(str.substr(prev));
    return strings;
}


class tap_abort_exception : public std::exception
{
private:
  std::string msg ;
public:

  tap_abort_exception(const std::string m) : msg(m)
  {
  }

  const char* what() const noexcept override 
  {
    return msg.c_str() ;
  }
  
};

void
tap_plan(int count)
{
  tap_current_plan = count ;
  std::cout << "1.." << (tap_current_plan+1) << "\n";
}

void
tap_comment(const std::string comment,
                 const std::string prefix)
{
  if (comment.empty())
    return;
  for ( auto line : split_string(comment,"\n") ) {
    std::cout << "#" << prefix << line << "\n";
  }
}

static std::string loc_format(source_location loc)
{
  // This is recognized as the 'aix' error format in emacs compilation mode.
  // That format is one of the few that can be inserted anywhere.  
  return "in line " + std::to_string(loc.line()) + " of file " + loc.file_name() + " ";
}

void
tap_test(bool cond,
         std::string msg,
         const std::string &comment,
         source_location loc)
{  
  if (cond) {
    std::cout << "ok      : " << msg << std::endl ;
  } else {
    std::cout << "not ok  : " << msg << " " << loc_format(loc) << std::endl ;
    tap_comment(comment," --> ");
  } 
}

void
tap_pass(const std::string &msg, source_location loc)
{
  tap_test(true,msg,"",loc);
}

void
tap_fail(const std::string &msg, source_location loc)
{
  tap_test(false,msg,"",loc);
}

void tap_abort(const std::string &msg, source_location /*loc*/ )
{
  throw tap_abort_exception(msg) ;
}

static void atexit_handler() 
{
  if (!atexit_ok) {
    tap_fail("early exit detected");
  }
}

int
main()
{

  std::cout << std::boolalpha ;
  std::cout << std::scientific ;

  const int result = std::atexit(atexit_handler);
  
  if (result != 0) {
    tap_fail("atexit registration failed") ; 
    return 1;
  }

  try {
    tap_current_plan = 0;
    bool ok = tap_main() ;
    tap_test( ok, "tap_main return");
  } catch (const tap_abort_exception& e) {
    std::cout << "Bail out! *** " + std::string(e.what()) << std::endl ;
  } catch (const std::exception& e) {
    tap_fail("catched exception " + std::string(e.what()) );
  } catch (...) {
    tap_fail("catched exception");
  }
  atexit_ok = true;
  return 0;
}
