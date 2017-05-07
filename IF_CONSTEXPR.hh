#include <type_traits>
#include <tuple>

#define IF_CONSTEXPR_TWO(var1, var2, cond, exp1)                                 \
    forward_as_tuple_for_if_constexpr(var1,var2)                                 \
    .if_constexpr(                                                               \
            std::integral_constant<bool, cond>{}                                 \
        ,   TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) { return exp1; }          \
        ,   TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) LAMBDA_BODY_WITH_CLOSING_BRACE

#define TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) [&](auto&&var1,auto&&var2)
#define LAMBDA_BODY_WITH_CLOSING_BRACE(...)  { return __VA_ARGS__; })

// Now to implement the variadic version, allowing any number of variables (including zero)
#define IF_CONSTEXPR(...)                                                                                      \
    forward_as_tuple_for_if_constexpr DROP_THE_LAST_TWO_MACRO_ARGS(__VA_ARGS__)                                \
    .if_constexpr(                                                                                             \
            std::integral_constant<bool, SECOND_LAST_MACRO_ARG(__VA_ARGS__)>{}                                 \
        ,   ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS(__VA_ARGS__) { return LAST_MACRO_ARG(__VA_ARGS__); }       \
        ,   ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS(__VA_ARGS__) LAMBDA_BODY_WITH_CLOSING_BRACE

#define                          LAST_MACRO_ARG(...)       EVAL(          FIRST_MACRO_ARG                REVERSE(__VA_ARGS__)  )
#define                   SECOND_LAST_MACRO_ARG(...)       EVAL(          SECOND_MACRO_ARG               REVERSE(__VA_ARGS__)  )
#define            DROP_THE_LAST_TWO_MACRO_ARGS(...)  EVAL(EVAL(  REVERSE DROP_THE_FIRST_TWO_MACRO_ARGS  REVERSE(__VA_ARGS__)  ))
#define ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS(...) PASTE(ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS_, COUNT_MACRO_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define EVAL(f, ...) f __VA_ARGS__

#define                         FIRST_MACRO_ARG(arg1      , ...)  arg1
#define                        SECOND_MACRO_ARG(arg1, arg2, ...)  arg2
#define           DROP_THE_FIRST_TWO_MACRO_ARGS(arg1, arg2, ...) (__VA_ARGS__)

#define COUNT_MACRO_ARGS(...) COUNT_MACRO_ARGS_(__VA_ARGS__, 5,4,3,2,1)
#define COUNT_MACRO_ARGS_(x1,x2,x3,x4,x5,THE_ANSWER,...)  THE_ANSWER

#define REVERSE(...) PASTE(REVERSE_, COUNT_MACRO_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define PASTE(funcname_base,n) PASTE_(funcname_base,n)
#define PASTE_(x,y) x ## y

#define REVERSE_1(        a) (a)
#define REVERSE_2(      b,a) (a,b)
#define REVERSE_3(    c,b,a) (a,b,c)
#define REVERSE_4(  d,c,b,a) (a,b,c,d)
#define REVERSE_5(e,d,c,b,a) (a,b,c,d,e)

#define ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS_2(                      ...) [&]()
#define ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS_3(a,                    ...) [&](auto &&a)
#define ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS_4(a,b                   ...) [&](auto &&a,auto &&b)
#define ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS_5(a,b,c                 ...) [&](auto &&a,auto &&b,auto&&c)
