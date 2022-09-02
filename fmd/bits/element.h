//
// This header file provides features related to printing formuloid  
//
//
//


namespace fmd {

/// \brief Helper class to print formuloid basal values.
///
/// When dumping a formuloid a potential issue is that basal elements
/// can be of almost any type T. The printed values must be formated. 
///
/// The class basal_simple_writer provides a mean to 
///
/// @ingroup grp_support
template <typename T>
struct basal_simple_writer {
  
  /// \brief write a simplified representation of basal type T
  ///
  /// This is the default fallback version. In specialized
  /// instanciations, that member takes an additional
  /// argument of a type T.
  ///
  static void apply(std::ostream &out) {
    static int counter=0;
    counter++;
    out << "?" ;
  };
};

} // of namespace fmd

/// \brief Helper macro to provide the declaration of a basal_simple_writer specialisation.
///
/// See \c fmd::basal_simple_writer for an example.
/// 
/// @ingroup grp_support
#define FMD_DECLARE_BASAL_SIMPLE_WRITER(TYPE)         \
  template <> struct fmd::basal_simple_writer<TYPE> { \
    static void apply(std::ostream &, TYPE const &); \
  }

/// \brief Helper macro to provide the implementation of a basal_simple_writer specialisation
///
/// See \c fmd::basal_simple_writer for an example.
///
/// @ingroup grp_support
#define FMD_IMPLEMENT_BASAL_SIMPLE_WRITER(STREAM, TYPE, NAME) \
  void fmd::basal_simple_writer<TYPE>::apply(std::ostream &STREAM, TYPE const &NAME)

FMD_DECLARE_BASAL_SIMPLE_WRITER(bool);
FMD_DECLARE_BASAL_SIMPLE_WRITER(char);
FMD_DECLARE_BASAL_SIMPLE_WRITER(short);
FMD_DECLARE_BASAL_SIMPLE_WRITER(int);
FMD_DECLARE_BASAL_SIMPLE_WRITER(long);
FMD_DECLARE_BASAL_SIMPLE_WRITER(long long);
FMD_DECLARE_BASAL_SIMPLE_WRITER(float);
FMD_DECLARE_BASAL_SIMPLE_WRITER(double);
FMD_DECLARE_BASAL_SIMPLE_WRITER(char*);
FMD_DECLARE_BASAL_SIMPLE_WRITER(const char*);
FMD_DECLARE_BASAL_SIMPLE_WRITER(std::string);

