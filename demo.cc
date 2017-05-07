#include "IF_CONSTEXPR.hh"

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

#include <iostream>

int main() {
    int a=3;
    int b=4;

    auto if_true = IF_CONSTEXPR    (       true,
           a*b
     )(
           a+b
    );
    auto if_false = IF_CONSTEXPR    ( a,    false,
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
