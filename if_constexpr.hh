/*
 * This file isn't very interesting. Go look at IF_CONSTEXPR.hh
 * to understand what this little project is all about
 */
#include<utility>
namespace helper {
    template<bool conditional>
    struct if_constexpr_helper;

    template<>
    struct if_constexpr_helper<true> {
        template<typename T, typename F>
        T operator() (T && t, F && ) { return std::forward<T>(t); }
    };

    template<>
    struct if_constexpr_helper<false> {
        template<typename T, typename F>
        F operator() (T &&, F && f) { return std::forward<F>(f); }
    };
}

template< bool conditional
        , typename T
        , typename F >
std::conditional_t<conditional,T,F>
if_constexpr (T && t, F && f) {
    return helper:: if_constexpr_helper<conditional>{} ( std::forward<T>(t), std::forward<F>(f) );
}

struct thing_with_if_constexpr_method {
    template<bool conditional, typename ...Ts>
    decltype(auto) if_constexpr_for_the_macro(Ts &&...ts) const {
        return if_constexpr<conditional>( std::forward<Ts>(ts)... );
    }
};
