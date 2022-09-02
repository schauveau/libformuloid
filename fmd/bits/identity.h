namespace fmd {

/// \brief The identity formuloid provides the formuloid parameter
///
/// The global variable fmd::id is equivalent to fmd::identity().
///
/// \ingroup grp_func
extern const formuloid<double> id;
  
/// \brief The identity formuloid provides the formuloid parameter.
///
/// The global variable fmd::id is equivalent to fmd::identity().
///
/// \ingroup grp_func
inline const formuloid<double> & identity() { return id; }

};

