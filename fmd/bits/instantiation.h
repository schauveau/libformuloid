//
// Define the templates that are explicitly instanciated in libformuloid.
//
#if 0

namespace fmd {

#ifdef FMD_INSTANTIATE 
#define FMD_EXTERN
#else
#define FMD_EXTERN extern
#endif

FMD_EXTERN template class fmd::node<double>;
FMD_EXTERN template class fmd::formuloid<double,false>;
FMD_EXTERN template class fmd::formuloid<double,true>;

FMD_EXTERN template class fmd::node<int>;
FMD_EXTERN template class fmd::formuloid<int,false>;
FMD_EXTERN template class fmd::formuloid<int,true>;

FMD_EXTERN template class fmd::node<bool>;
FMD_EXTERN template class fmd::formuloid<bool,false>;
FMD_EXTERN template class fmd::formuloid<bool,true>;

#undef FMD_EXTERN
}
#endif
