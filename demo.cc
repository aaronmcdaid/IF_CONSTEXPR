/*
 * This file isn't very interesting. It's just a demo.
 * Go look at IF_CONSTEXPR.hh to understand what this little
 * project is all about
 */
#include <iostream>
#include <type_traits>
#include "IF_CONSTEXPR.hh"

template<typename T, typename F, typename ...Args>
auto if_c(std:: true_type, T&& t, F&&, Args&&... args)
{
    return std::forward<T>(t)( std::forward<Args>(args) ... );
}
template<typename T, typename F, typename ...Args>
auto if_c(std:: false_type, T&&, F&&f, Args&&... args)
{
    return std::forward<F>(f)( std::forward<Args>(args) ... );
}



int main() {
    int a=3;
    int b=4;

    std:: cout << IF_CONSTEXPR_SIMPLE(true ) ( a*b ) (a+b) << '\n';
    std:: cout << IF_CONSTEXPR_SIMPLE(false) ( a*b ) (a+b) << '\n';

    // IF_CONSTEXPR( true )( a, b,
    //       a*b
    // )(
    //       a+b
    // )

    auto x =
    if_c( std::integral_constant<bool, false>{}
           ,[&](auto&&a,auto&&b){ return a*b; }
           ,[&](auto&&a,auto&&b){ return a+b; }
           ,a,b
           );
    std::cout << x <<'\n';
}
