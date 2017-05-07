#include "if_constexpr.hh"

#define IF_CONSTEXPR_SIMPLE(cond) if_constexpr<cond> FIRST_EXPRESSION
#define FIRST_EXPRESSION(e1) ( [&] () { return e1; } , SECOND_EXPRESSION
#define SECOND_EXPRESSION(e2)   [&] () { return e2; } ) ()

