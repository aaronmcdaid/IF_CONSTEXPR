#include "IF_CONSTEXPR.hh"

struct empty_t {};

template<typename T, typename F>
decltype(auto)  return_the_first_arg_if_true
                ( std::true_type, T&& t, F&& )
{ return std::forward<T>(t); }

template<typename T, typename F>
decltype(auto)  return_the_first_arg_if_true
                ( std::false_type, T&& , F&& f)
{ return std::forward<F>(f); }
