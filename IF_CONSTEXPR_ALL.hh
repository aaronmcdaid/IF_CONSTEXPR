#include "IF_CONSTEXPR.hh"

#define IF_CONSTEXPR_ALL(...)                       \
        [&]() ->decltype(auto) {                    \
            MAKE_THE_CAPTURE_OBJECT(__VA_ARGS__);   \
            HANDLE_CONDITION_AND_THE_REST

#define MAKE_THE_CAPTURE_OBJECT(...)                        COUNT_THE_ARGS_AND_THEN_CALL_THE_APPROPRIATE_OVERLOAD(MAKE_THE_CAPTURE_OBJECT_ , __VA_ARGS__)
#define MAKE_THE_CAPTURE_OBJECT_2(var1, var2)       \
            using type_1 = decltype(var1);          \
            using type_2 = decltype(var2);          \
            struct local_copy_of_args_t {           \
                type_1             var1;            \
                type_2             var2;            \
            };                                      \
            struct local_copy_of_args_ref_t {           \
                type_1           & var1;            \
                type_2           & var2;            \
            };                                      \
            auto local_copy_of_args     = local_copy_of_args_t      { var1, var2 }; \
            auto local_copy_of_args_ref = local_copy_of_args_ref_t  { var1, var2 }; \
            (void) local_copy_of_args_ref;  \
            (void) local_copy_of_args

#define HANDLE_CONDITION_AND_THE_REST(...)                      \
            constexpr bool condition_variable = (__VA_ARGS__);  \
            using  first_expression_type = std::conditional_t<condition_variable, local_copy_of_args_ref_t, local_copy_of_args_t     >; \
            using second_expression_type = std::conditional_t<condition_variable, local_copy_of_args_t    , local_copy_of_args_ref_t >; \
            HANDLE_FIRST_EXPRESSION_AND_THE_REST
#define HANDLE_FIRST_EXPRESSION_AND_THE_REST(...)           HANDLE_FIRST_EXPRESSION (__VA_ARGS__)       HANDLE_SECOND_EXPRESSION_AND_THE_REST
#define HANDLE_SECOND_EXPRESSION_AND_THE_REST(...)          HANDLE_SECOND_EXPRESSION(__VA_ARGS__)       FINISH_WITH_THE_COMPUTATION

#define HANDLE_FIRST_EXPRESSION(...)                \
            auto first_expression_lambda    =   LAMBDA_TO_COMPUTE_AN_EXPRESSION(__VA_ARGS__);
#define HANDLE_SECOND_EXPRESSION(...)               \
            auto second_expression_lambda   =   LAMBDA_TO_COMPUTE_AN_EXPRESSION(__VA_ARGS__);

#define LAMBDA_TO_COMPUTE_AN_EXPRESSION(...) [](auto && local_copy_of_args, auto && dummy) ->decltype(auto) {   \
            struct one_expression_t : local_copy_of_args_t, std::remove_reference_t<decltype(dummy)> {          \
                one_expression_t(local_copy_of_args_t&& l) : local_copy_of_args_t( std::move(l) ) {}            \
                decltype(auto)  evaluate_the_expression() {                                                                          \
                    return __VA_ARGS__ ;                                                                        \
                }                                                                                               \
            };                                                                                                  \
            return one_expression_t( std::move(local_copy_of_args) ) . evaluate_the_expression();                                    \
        }

#define FINISH_WITH_THE_COMPUTATION                                 \
        return                                                      \
            return_the_first_arg_if_true                            \
            (   std::integral_constant<bool, condition_variable>{}  \
            ,    first_expression_lambda                            \
            ,   second_expression_lambda                            \
            )                                                       \
            ( std::move(local_copy_of_args), empty_t{} );           \
        }()


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
