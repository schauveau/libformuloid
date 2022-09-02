// various helpers for tap based tests 

#include <vector>

// A set of generic p-values inclusing 0.0


// A set of generic p-values excluding 0.0 
std::vector<double> pvalues_nz = { -33.4 , 12.3 , 0.1 , 0.5, 1.0, 444.5 } ;

// some p-values in the interval [0,1]
std::vector<double> pvalues_0_1 = { 0.0, 0.123, 0.222, 0.5, 0.755, 1.0 } ;

// some p-values in the interval [-1,1]
std::vector<double> pvalues_1 = { -1.0, -0.32, 0.0, +0.2, +0.45, +1.0 };

// some positive and negative sets of p-values
std::vector<double> pvalues_pos = { +0.0, +1.0, +3.4, +42.0, +111.3, +666.6 };
std::vector<double> pvalues_neg = { -0.0, -1.0, -3.4, -42.0, -111.3, -666.6 };
