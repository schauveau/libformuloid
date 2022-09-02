
namespace fmd {


/// \TBD
template < IsFormuloid<double> T1,  
           IsFormuloid<double> T2
           >
inline auto
morph( const T1 & before,
       double   t,
       const T2 & after,
       const fmd::easing & ease = fmd::ease_default )
{
  if (ease.start == ease.end) {
    return fmd::select( fmd::id < t, before, after) ;
  } else {
    // TODO: Create a dedicated node to avoid the double evaluation of 't'?
    //       This is not critical as long as T2 remains a false formuloid.
    return fmd::transit( t+ease.start, t+ease.end , before, after, ease) ;
  }  
}


}; // of namespace fmd
