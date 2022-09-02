
#ifdef DO_COMPARE
#ifdef DO_BOOL
    std::cout << "[bool]\n";
    // bool
    eval_func_b( true OPER f_true );
#endif

#ifdef DO_INT
    std::cout << "[int]\n";
    // int
    eval_func_b( f_1 OPER 42 );
    eval_func_b( 42 OPER f_1 );
    eval_func_b( f_1 OPER f_1 );
#endif

#ifdef DO_DOUBLE
    std::cout << "[double]\n";
    // double
    eval_func_b( f_pi OPER 1.23 );
    eval_func_b( f_pi OPER f_pi );
#endif
#endif

#ifdef DO_UNOP

#ifdef DO_BOOL
    std::cout << "[bool]\n";
    eval_func_b( OPER f_false );
    eval_func_b( OPER f_true );
    eval_func_b( OPER (f_1 == 0) );
    eval_func_b( OPER (f_1 == 1) );
#endif

#ifdef DO_INT
    std::cout << "[int]\n";
    eval_func_i( OPER f_1 );
    eval_func_i( OPER fmd::to_formuloid(42) );
#endif

#ifdef DO_DOUBLE
    std::cout << "[double]\n";
    eval_func_d( OPER f_pi );
    eval_func_d( OPER fmd::to_formuloid(1.234) );
#endif

#endif

#ifdef DO_BINOP

#ifdef DO_BOOL
    std::cout << "[bool]\n";
    eval_func_b( true OPER f_true );
#endif

#ifdef DO_INT
    std::cout << "[int]\n";
    eval_func_i( f_1 OPER 42 );
    eval_func_i( 42 OPER f_1 );
    eval_func_i( f_1 OPER f_1 );
#endif

#ifdef DO_DOUBLE
    std::cout << "[double]\n";
    eval_func_d( f_pi OPER 1.23 );
#endif

#endif



#undef DO_COMPARE
#undef DO_UNOP
#undef DO_BINOP

#undef DO_BOOL
#undef DO_INT
#undef DO_DOUBLE

#undef OPER
