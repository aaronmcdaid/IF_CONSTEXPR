/*
 * This file isn't very interesting. It's just a demo.
 * Go look at IF_CONSTEXPR.hh to understand what this little
 * project is all about
 */
#include <iostream>
#include <type_traits>
#include <tuple>
#include "IF_CONSTEXPR.hh"

#define IF_CONSTEXPR_TWO(var1, var2, cond, exp1)                                 \
    forward_as_tuple_for_if_constexpr(var1,var2)                                       \
    .if_constexpr(                                                               \
            std::integral_constant<bool, cond>{}                                 \
        ,   TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) { return exp1; }                \
        ,   TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) LAMBDA_BODY_WITH_CLOSING_BRACE

#define TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) [&](auto&&var1,auto&&var2)
#define LAMBDA_BODY_WITH_CLOSING_BRACE(...)  { return __VA_ARGS__; })

template<size_t ... I, typename F>
decltype(auto) apply_integer_sequence(F &&f, std:: index_sequence<I...>) {
    return std::forward<F>(f) ( std::integral_constant<size_t, I>{} ... );
}

template<size_t count, typename F>
decltype(auto) apply_integer_sequence(F&&f) {
    return apply_integer_sequence
        (   std::forward<F>(f)
        ,   std::make_index_sequence<count>{}
        );
}

template<typename ... Args>
struct forward_as_tuple_for_if_constexpr_helper {
    std::tuple< Args ...> m_args;
    forward_as_tuple_for_if_constexpr_helper(Args&& ...args) : m_args( std::forward<Args>(args)... ){}

    template<typename T, typename F>
    decltype(auto) if_constexpr(std:: false_type, T&&, F&&f)
    {
        return apply_integer_sequence<sizeof...(Args)>( [&](auto &&... idxs){
                return std::forward<F>(f)( std::get< std::remove_reference_t<decltype(idxs)>::value >(m_args) ...);
        });
    }
    template<typename T, typename F>
    decltype(auto) if_constexpr(std::  true_type, T&&t, F&&)
    {
        return apply_integer_sequence<sizeof...(Args)>( [&](auto &&... idxs){
                return std::forward<T>(t)( std::get< std::remove_reference_t<decltype(idxs)>::value >(m_args) ...);
        });
    }
};

template<typename ... Args>
auto forward_as_tuple_for_if_constexpr(Args&& ... args) {
    return forward_as_tuple_for_if_constexpr_helper<Args...>( std::forward<Args>(args)... );
}

int main() {
    int a=3;
    int b=4;

    std:: cout << IF_CONSTEXPR_SIMPLE(true ) ( a*b ) (a+b) << '\n';
    std:: cout << IF_CONSTEXPR_SIMPLE(false) ( a*b ) (a+b) << '\n';

    auto if_true =
    IF_CONSTEXPR_TWO( a, b, true,
           a*b
     )(
           a(b)
    );
    auto if_false =
    IF_CONSTEXPR_TWO( a, b, false,
           a(b)
     )(
           a+b
    );
    std:: cout << if_true  << "\n";
    std:: cout << if_false << "\n";

    std::cout
        <<  forward_as_tuple_for_if_constexpr(a,b)
            .if_constexpr(
                    std::integral_constant<bool, true>{}
                ,   [&](auto&&a,auto&&b){ return a*b; }
                ,   [&](auto&&a,auto&&b){ return a(b); }
               )
        <<'\n';
    std::cout
        <<  forward_as_tuple_for_if_constexpr(a,b)
            .if_constexpr(
                    std::integral_constant<bool, false>{}
                ,   [&](auto&&a,auto&&b){ return a(b); }
                ,   [&](auto&&a,auto&&b){ return a+b; }
               )
        <<'\n';
}
