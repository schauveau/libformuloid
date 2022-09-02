//
// Provide an implementation of tap_main() with the
// following scheme: 
//
// - The function test_p (see below) is called multiple
//   times with a different value of p.
//
// - test_p is expected to perform one or more tests
//   using the specified p value.
//
// - A comment describing the p value is emited before calling 
//   test_p. 
//
// - The global array containing all tested p values is called PV.
//   and its size is insured to be at least 10.
//     --> PV[0] to PV[9] can be safely used in test_p. 
//
// - the default PV is insured to contain
//     * the values 0.0, +1.0, -1.0  in PV[0], PV[1] and PV[2]
//     * a few other values in the range [-1.0,+1.0].
//     * some values above and below that range.
//     * no absolute value greater than 1000 
// 
// - The macro CUSTOM_PV can be defined as an alternative
//   expression for PV.  
//
// - The macro MORE_PV can be used to specify one or more
//   additional values in the default PV. 
//
// - The macro FILTER_PV, when set, can be used to select
//   which p-values 
//   
//    
//
// - If the number of tests is constant in test_p then
//   the macro PLAN can be set to that number to generate
//   a tap plan.
// 

#ifndef FILTER_PV
#define FILTER_PV(p) true
#endif

#ifndef CUSTOM_PV
#define CUSTOM_PV PV
#endif

std::vector<double> PV =
{
    0.0,   -1.0,    1.0,
  -123.7, -33.4,  -12.3,
    -0.7,  -0.2,   +0.3,
    +0.5, +0.88,  +13.0,
   +42.3, +44.5, +111.2
#ifdef MORE_PV
   , MORE_PV
#endif
} ;



void test_p(double p) ;
   
bool tap_main()
{
  std::vector<double> pvalues_ ;
  
  for (auto p : CUSTOM_PV) {
    if (FILTER_PV(p)) {
      pvalues_.push_back(p) ;
    }
  }
  
#ifdef PLAN
  tap_plan( pvalues_.size() * (PLAN) ) ;  
#endif
  
  for (auto p : pvalues_)
  {
    std::ostringstream s ;
    s.copyfmt(std::cout);
    s << "===== start testing with p = " << p << " ========";
    tap_comment(s.str());         

    test_p(p) ;    
  }
  
  return true;
}
