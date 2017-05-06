#include "if_constexpr.hh"

#define IF_CONSTEXPR_SIMPLE(cond) if_constexpr<cond> FIRST_EXPRESSION
#define FIRST_EXPRESSION(e1) ( [&] () { return e1; } , SECOND_EXPRESSION
#define SECOND_EXPRESSION(e2)   [&] () { return e2; } ) ()



#define IF_CONSTEXPR(cond) if_constexpr(cond) CAPTURE_TWO_VARS
#define CAPTURE_TWO_VARS(v1,v2) [&v1,&v2]() { TWO_EXPRESSIONS
#define TWO_EXPRESSIONS(e1,e2) return if_constexpr<cOnD>(  [&](){return e1;} , [&](){return e1;} )
