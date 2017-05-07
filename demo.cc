#include "IF_CONSTEXPR.hh"

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
