/*
 * This file isn't very interesting. It's just a demo.
 * Go look at IF_CONSTEXPR.hh to understand what this little
 * project is all about
 */
#include <iostream>
#include "IF_CONSTEXPR.hh"


int main() {
    int a=3;
    int b=4;

    std:: cout << ((IF_CONSTEXPR_BRACES(true)(a,b) a*b ) a+b)     << '\n';
    std:: cout << ((IF_CONSTEXPR_BRACES(false)(a,b) a*b ) a+b)     << '\n';

    std:: cout << IF_CONSTEXPR_SIMPLE(true ) ( a*b ) (a+b) << '\n';
    std:: cout << IF_CONSTEXPR_SIMPLE(false) ( a*b ) (a+b) << '\n';
}
