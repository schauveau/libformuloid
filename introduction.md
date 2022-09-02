THIS FILE IS OBSOLETE! TO BE REMOVED 

# Introduction

libformuloid is a C++20 library that implement a special kind function named a formuloid.
Well! Strictly speaking, a formuloid is not really a function. This is a C++ object that 
holds a formula whole value depends of a single 'double' value called the formuloid 
parameter.

Unlike regular C++ functions and lambdas, arithmetic operators and functions can be 
applied to formuloids to create more complex formuloids (assuming that those operators
are implemented by libformuloid or by the user). The library is currently focussing on providing 
good support for formuloids of types \t bool, \t double and occasionnally \t int. 
Other can be used but with limited functionalities.

All features are defined in the fmd namespace. For a end-user, the most important type is fmd::func<T> that transforms a basal formuloid type (so basically any C++ type) into a formuloid.

For example, the following code creates a formuloid that holds the same value 3.1415 for any value of the formuloid parameter. The formula stored in the formuloid can be dumped using std::ostream. In that dump, '@' represents the formuloid parameter and '[v]{f}' represents the evaluation of the formula f using v as parameter. By definition, the dump of a 'true formuloid' is always of the form '[@]{...}' to indicate that the formula requires a parameter to be evaluated. 

    #define print(x) std::cout << #x << " := " << (x) << std::endl 

    fmd::func<double> f0 = 1234.5 ;
    print(f);  // --> f0 := [@]{1234.5}

A more interesting function is fmd::identity() also provided as a global
variable fmd::id. That formuloid provides the value of the parameter. 

    fmd::func<double> at = fmd::identity() ; // or fmd::id 
    print(at); // --> at := [@]{@}

It is possible to create more complex formuloids. An implementation for 'double' is provided for most of the mathematical functions from <cmath>.

    fmd::func<double>  f1 = fmd::sin(at+1.0)*3.0 
    print(f1);  // --> f1 is [@]{mul(sin(add(@,1.0)),3.0)}

It is possible to evaluate a true formuloid for a given parameter value using the [] operator. During evaluation, parts of the formula that were found to be always constant may be optimized to speed up further evaluations. 
  
    fmd::func<double> f2 = 66.0 ;
    f2 = f2+34.0;
    f2 = at*f2;
    print( f2 ) ;       // --> [@]{add(1.0,2.0)*@}
    print( f2[1.4] ) ;  // --> 140.0 
    print( f2 );        // --> [@]{@*100.0} 
    print( f2[2.3] ) ;  // --> 230.0 

Using () instead of [] provides a formuloid instead of the value.

    fmd::expr<double> e3 = f2(2.3);
    print( e3 ) ;       // --> [2.3]{add(1.0,2.0)*@}
    fmd::func<double> f4 = f2(at+4.5); 
    print( f4 ) ;       // --> [@]{[add(@,4.5)]{add(1.0,2.0)*@}}

In the previous example, the type of f2(2.3) is fmd::expr<double> instead of fmd::func<double>.  This indicates that this is a false formuloid that does not require an initial parameter value. The parameter @ that appears in its formula does not refer to the initial parameter but to the value of the parameter formula found in the enclosing '[]{}' construct. In that case, that is simply '2.3'.. 

f4 is constructed in a similar manner but is a true formuloid because 'at+4.5' is also a true formuloid. 

It should be noted that the true or false status of a formuloid is entirely decided at compile time and does not depend of the actual content of the formuloid. 

    fmd::expr<double> e5 = 5.4;   // 
    fmd::func<double> f6 = e5;    //  false formuloids can be converted to true formuloid
    // However, a true formuloid cannot be assigned to a false formuloid
    fmd::expr<double> e7 = f6;    // Compilation error!        
    
False formuloids can be evaluated using the unary * operator and doing so will usually
optimize their formula to a single constant.

    fmd::expr<int> e8 = fmd::abs(-10) ;
    print(e8);           //  -->  e8 := abs(-10)
    e8 = e8+32;
    print(e8);           //  -->  e8 := abs(-10)*32
    print(*e8);          //  -->  *e8 := 42 
    print(e8);           //  -->  e8 := 42
    


# Notations

- a formuloid is a C++ type that holds a formula using up to a single parameter. 

- a true formuloid is a formuloid with a formula that (potentially) requires
  a parameter value during evaluation.

- a false formuloid is a formuloid with a formula that cannot require any
  parameter value during evaluation.

- a basal formuloid is a type for which it is possible to implement formuloids.  

# Concepts and Traits

- concept `IsAnyBasalFormuloid<T>`

   true if type T is a basal formuloid type. As of today, this mean that
   T is neither void, const, reference, array or an actual formuloid.
    
   ``` 
   // The following types are basal formuloid
   static_assert( IsBasalFormuloid<int> );
   static_assert( IsBasalFormuloid<int*> );
   static_assert( IsBasalFormuloid<const int*> ); // the pointer is not const
   static_assert( IsBasalFormuloid<std::string> );
   // The following types are not basal formuloid
   static_assert( !IsBasalFormuloid<void>  );
   static_assert( !IsBasalFormuloid<const int> );
   static_assert( !IsBasalFormuloid<fmd::func<int>> ); // an actual formuloid
   static_assert( !IsBasalFormuloid<fmd::expr<int>> ); // an actual formuloid
   static_assert( IsBasalFormuloid<int *const> );      // the pointer is const
   ```

- concept `IsAnyActualFormuloid<T>` 

   true if type T is an actual implementation of a formuloid without any attribute.
    
   ```
   static_assert( IsActualFormuloid<fmd::func<int>> ); 
   static_assert( IsActualFormuloid<fmd::expr<int>> );
   static_assert( !IsActualFormuloid<fmd::func<int>*> );      // because of pointer
   static_assert( !IsActualFormuloid<const fmd::func<int>> ); // because of const qualifier
   static_assert( !IsActualFormuloid<int> );
   ```
 

- type `fmd::formuloid_value_t<T>`

   Provide the 'value type' of the formuloid T of `void` if T is not a formuloid.
   
   It should be noted that the 'value type' of a basal formuloid type is itself.
   
   ```
   static_assert( std::is_same_v< int,  fmd::formuloid_value_t<int>  >); // because int is basal 
   static_assert( std::is_same_v< bool, fmd::formuloid_value_t<fmd::expr<bool>> >);
   static_assert( std::is_same_v< char, fmd::formuloid_value_t<fmd::func<char>> >);   
   static_assert( std::is_same_v< void, fmd::formuloid_value_t<const fmd::func<char>> >); // because of 'const'
   static_assert( std::is_same_v< void, const int> );
   ```
    
- concept `IsBasalFormuloid<T,U>`

    true if T is a basal formuloid type and its value type is `U`.
        
    ```
    static_assert( fmd::IsBasalFormuloid<int,int> );
    static_assert( fmd::IsBasalFormuloid<double,double> );
    static_assert( !fmd::IsBasalFormuloid<char,int> );                 // fmd::formuloid_value_t<char> is not 'int' but 'char'
    static_assert( !fmd::IsBasalFormuloid<const int,const int> );      // because 'const int' is not basal
    static_assert( !fmd::IsBasalFormuloid<fmd::expr<double>,double> ); // because fmd::expr<double> is not basal
    static_assert( !fmd::IsBasalFormuloid<fmd::func<double>,double> ); // because fmd::func<double> is not basal
    ```



    



