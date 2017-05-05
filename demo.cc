#include <iostream>
#include "IF_CONSTEXPR.hh"

int main() {
    int a=3;
    int b=4;
    auto answer_if_true  =
        ((IF_CONSTEXPR (true ) (a,b)
            a*b )
            a+b );
    auto answer_if_false =
        ((IF_CONSTEXPR (false) (a,b)
            a*b)
            a+b);
    std::cout << answer_if_true << '\n';
    std::cout << answer_if_false << '\n';
}
