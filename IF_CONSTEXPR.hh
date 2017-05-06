/*
 * This started in response to this tweet by Eric Niebler
 * https://twitter.com/ericniebler/status/859985304068816896
 *
 * Aaron McDaid aaron.mcdaid@gmail.com  2017-05-06
 *
 * This file is the MACRO magic. In if_constexpr.hh, I have my
 * own implementation of if_constexpr.
 *
 * Basically, you need at least one trailing parenthesis in order
 * for the preprocessor to be able to see far enough ahead. i.e.
 * this is possible:
          IF_CONSTEXPR (true ) (a,b)
            a*b )
            a+b );
 * or even this to go to the extreme:
          IF_CONSTEXPR (true
            a   )
            b   )
            a*b )
            a+b );
 * However, text editors would be unhappy with the unbalanced
 * parentheses. Hence, I hacked it a bit more so that I
 * could type two parentheses just before IF_CONSTEXPR.
 * Anyway, this works:
        ((IF_CONSTEXPR_BRACES (true ) (a,b)
            a*b )
            a+b );
 */
#include "if_constexpr.hh"

#define IF_CONSTEXPR_SIMPLE(cond) if_constexpr<cond> FIRST_EXPRESSION
#define FIRST_EXPRESSION(e1) ( [&] () { return e1; } , SECOND_EXPRESSION
#define SECOND_EXPRESSION(e2)   [&] () { return e2; } ) ()


#define IF_CONSTEXPR_BRACES thing_with_if_constexpr_method{})).OUTER_MACRO(( DEAL_WITH_THE_CONDITIONAL

#define OUTER_MACRO(x) FIVE_TOGETHER(EXPAND_WITH_COMMA x)
#define DEAL_WITH_THE_CONDITIONAL(conditional) conditional , EXPAND_WITH_COMMA
#define EXPAND_WITH_COMMA(...) __VA_ARGS__ ,
#define FIVE_TOGETHER(a)  IF_CONSTEXPR_IMPL(a)
#define IF_CONSTEXPR_IMPL(conditional, v1, v2, e1, e2)  if_constexpr_for_the_macro<conditional>( [](auto && v1, auto && v2) { return e1; }, [](auto &&v1, auto &&v2){return e2;} ) (v1,v2)
