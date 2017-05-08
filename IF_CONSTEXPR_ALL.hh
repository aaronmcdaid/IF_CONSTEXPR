#include "IF_CONSTEXPR.hh"

#define IF_CONSTEXPR_ALL(...)  MAKE_THE_CAPTURE_OBJECT(__VA_ARGS__); HANDLE_CONDITION_AND_THE_REST

#define MAKE_THE_CAPTURE_OBJECT(...)                        COUNT_THE_ARGS_AND_THEN_CALL_THE_APPROPRIATE_OVERLOAD(MAKE_THE_CAPTURE_OBJECT_ , __VA_ARGS__)
#define HANDLE_CONDITION_AND_THE_REST(...)      \
    constexpr bool condition_variable = (__VA_ARGS__);      HANDLE_FIRST_EXPRESSION_AND_THE_REST
#define HANDLE_FIRST_EXPRESSION_AND_THE_REST(...)           HANDLE_FIRST_EXPRESSION(__VA_ARGS__)        HANDLE_SECOND_EXPRESSION_AND_THE_REST
#define HANDLE_SECOND_EXPRESSION_AND_THE_REST(...)

#define MAKE_THE_CAPTURE_OBJECT_2(var1, var2)  \
        struct local_copy_of_args_t {          \
            decltype(var1)  &  var1;           \
            decltype(var2)  &  var2;           \
        };                                     \
        auto local_copy_of_args = local_copy_of_args_t  { var1, var2 }

#define HANDLE_FIRST_EXPRESSION(...)    auto first_expression_lambda = LAMBDA_TO_COMPUTE_AN_EXPRESSION(__VA_ARGS__);
#define HANDLE_SECOND_EXPRESSION(...)

#define LAMBDA_TO_COMPUTE_AN_EXPRESSION(...) [](auto && local_copy_of_args, auto && dummy) ->decltype(auto) {   \
            struct one_expression_t : local_copy_of_args_t, std::remove_reference_t<decltype(dummy)> {          \
                one_expression_t(local_copy_of_args_t&& l) : local_copy_of_args_t( std::move(l) ) {}            \
                decltype(auto)  go() {                                                                          \
                    return __VA_ARGS__ ;                                                                        \
                }                                                                                               \
            };                                                                                                  \
            return one_expression_t( std::move(local_copy_of_args) ) . go();                                    \
        }


struct empty_t {};

template<typename T, typename F>
decltype(auto)  return_the_first_arg_if_true
                ( std::true_type, T&& t, F&& )
{ return std::forward<T>(t); }

template<typename T, typename F>
decltype(auto)  return_the_first_arg_if_true
                ( std::false_type, T&& , F&& f)
{ return std::forward<F>(f); }

/*
    IF_CONSTEXPR_ALL    ( a,b,c )(  true )(
           a*b*c                    // returns 60
    )(
           a(b) + c + b(a)          // branch not taken
    )
    */
