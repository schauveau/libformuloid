
#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define FMD_USE_CXXABI
#endif

namespace fmd {
namespace internal {

extern std::string cxx_demangle(const char* name) ;

template <typename T>
auto get_typename = []() -> const std::string &
{
  static const std::string result( fmd::internal::cxx_demangle(typeid(T).name()) );
  return result;
};

} // of namespace internal
} // of namespace fmd


#define FMD_SET_TYPENAME(TYPE,NAME)                       \
template<>                                                \
auto fmd::internal::get_typename<TYPE> =                  \
  []() -> const std::string &                             \
  {                                                       \
    static const std::string result(NAME) ;               \
    return result;                                        \
  } 

#if 0
FMD_SET_TYPENAME(char*,  "char*");
FMD_SET_TYPENAME(char,   "char");
FMD_SET_TYPENAME(int,    "int");
FMD_SET_TYPENAME(int,    "double");
#endif

