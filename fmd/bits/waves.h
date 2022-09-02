namespace fmd {


/// \brief The Sine waverform functoid
///
/// Provides the sine waveform functoid F[t] of
/// half-period p=t2-t1 with values in range [0,1],
/// and phased to
///  - return 0.0 at t==t1
///  - return 1.0 at t==t2
///
/// The result is undefined if t1 and t2 are equal. 
///
/// See also https://en.wikipedia.org/wiki/Waveform
///
template < IsFormuloid<double> T >
inline fmd::formuloid<double>
sine_wave( double t1, double t2, const T& t )
{
  return 0.5*(fmd::cos((t-t2)*(std::numbers::pi/(t2-t1)))+1.0)  ;
}

/// \brief The Sine waverform functoid
///
/// This is a shortcut for \c fmd::sine_wave(t1,t2,fmd::id)
///
inline fmd::formuloid<double>
sine_wave( double t1, double t2)
{
  return fmd::sine_wave(t1, t2, fmd::id);
}

/// \brief Interpolation two functoid using a sinewave
///
/// Provide a SineWave interpolation between f1 and f2.
///
/// This is an alias for fmd::Lerp(f1, f3, fmd::sine_wave(t1,t2)).
///
/// An error occurs if t1 and t2 are equal.  
///
template < IsFormuloid<double> T1,
           IsFormuloid<double> T2 >
inline fmd::formuloid<double>
sine_wave_between( double t1, const T1 &f1, double t2, const T2 &f2  )
{  
  return fmd::lerp(f1, f2, fmd::sine_wave(t1,t2) );
}

/// \brief The square waveform functoid
///
/// Provides the square waveform function F[t] with
/// an half-period p=abs(t2-t1) and phased to
///  - return 0.0 if t in [t1,t1+p(
///  - return 1.0 it t in [t1-p,t1(  
///
/// An error occurs if t1 and t2 are equal.
///
/// See also https://en.wikipedia.org/wiki/Waveform
///
template <
  IsFormuloid<double> T 
>
inline fmd::functoid<double>
square_wave( double t1, double t2, const T& t )
{
  double p = std::abs(t2-t1);
  return fmd::select(floor_fmod((t-t1)/p,2.0)<1.0,0.0,1.0); 
}

/// \brief The square waveform functoid
///
/// This is a shortcut for \c fmd::square_wave(t1,t2,fmd::id)
/// 
inline fmd::functoid<double>
square_wave( double t1, double t2)
{
  return fmd::square_wave(t1, t2, fmd::id);
}

/// \brief The sawtooth waveform functoid
///
/// Provides the sawtooth waveform function F[x] with
/// period p=t2-t1 and phased to return 0.0 at t=t1
/// up to 1.0 near t=t2.
///
/// Remark: Because it is cyclic, the sawtooth function
/// may never actually returns 1.0.  
/// 
/// The result is undefine if t1 and t2 are equal (i.e.
/// a null period).
///
/// See also https://en.wikipedia.org/wiki/Waveform
///
template <
  IsFormuloid<double> T
>
inline fmd::functoid<double>
sawtooth_wave(double t1, double t2, const T& t )
{
  return fmd::floor_dist( (x-t1) / std::abs(t2-t1) );
}


/// \brief The sawtooth waveform functoid
///
/// This is a shortcut for \c fmd::sawtooth_wave(t1,t2,fmd::id)
///
inline fmd::functoid<double>
sawtooth_wave(double t1, double t2)
{
  return sawtooth_wave(t1, t2, fmd::id);
}

}; // of namespace fmd
