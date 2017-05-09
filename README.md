This started in response to this tweet by Eric Niebler
https://twitter.com/ericniebler/status/859985304068816896

Aaron McDaid aaron.mcdaid@gmail.com  2017-05-06

The tweet requested a macro which could transform

        IF_CONSTEXPR(var1,var2)(condition) (
            expression1
        )(
            expression2
        )

into

        if_constexpr<condition>(
                [&](auto&& var1, auto&& var2) -> decltype(auto) {
                    return expression1;
                }
            ,   [&](auto&& var1, auto&& var2) -> decltype(auto) {
                    return expression2;
                }
            )(var1,var2)


{ If you've stumbled across this file randomly, skip down to "Why is this useful?"
  to learn that this is about.  }



The file [IF_CONSTEXPR.hh] implements a macro with essentially the same semantics.
The only visible difference is that this macro requires that a comma (`,`) appears
just before and after `condition` instead of `)(`. i.e. it is used as follows:


        IF_CONSTEXPR (var1, var2, condition,
            expression1
        )(
            expression2
        )

This is slightly less aesthetically pleasing than the requested version, but this
appears to be the best possible with the C/C++ preprocessor.

(**Update May 9th** It is possible after all, to get exactly the requested macro.
I've implemented it as `IF_CONSTEXPR_ALL` within [IF_CONSTEXPR_ALL.hh].
The only other difference is that `IF_CONSTEXPR_ALL.hh` requires that the user
list every variable that appears in the expression, while `IF_CONSTEXPR` simply
requires only those that need protection from errors.
In this document, however, I'll focus on `IF_CONSTEXPR` as it's simpler.
)

You might find it pleasing to put extra, redundant, parentheses around the condition
to help it to stand out:

        IF_CONSTEXPR (var1, var2, (condition),
            expression1
        )(
            expression2
        )

A couple of notes before getting into more details:

    -   this is 'variadic', i.e. the number of variables that are 'protected' by this
        need not be exactly two ("var1" and "var2"). The number can range from
        zero to three. This limit can be made larger by adding extra macro "overloads"
        for "REVERSE_*" and "ALL_BUT_TWO_VARS_AS_GENERIC_LAMBDA_ARGS_*", and
        updating the COUNT_MACRO_ARGS macro.
    -   For clarity, it is not always necessary to list every variable that is used
        in either expression. A subset will usually suffice.  Only the variables that
        would cause an error in the 'wrong' branch need to be listed. Sometimes, this
        means the variable list can be empty:
                IF_CONSTEXPR (condition, expression1)( expression2)


# Why is this useful?

Consider this code:

    int a=3;
    int b=4;
    if(true) {
        std::cout << a*b << '\n';
    }
    else {
        std::cout << a(b) << '\n';
    }

This will fail to compile, as `a` is an integer and cannot be treated as a function
in `a(b)`. This is frustrating however in situations where it can be proven at
compile-time that the 'else' branch will never be taken.

C++17 has a feature called 'constexpr if' that solves this problem.
The purpose of this macro is to implement this in C++14.

# How does this IF_CONSTEXPR macro work?

The file [IF_CONSTEXPR.hh] is quite long, but the basic technology is straightforward.
Most of the complication is related to making it variadic. I'm just going
to discuss the preprocessor macros here, the C++ code at the bottom of IF_CONSTEXPR.hh
is not explained here.

If `a` and `b` are ints, then `a(b)` is an error as `a` is not a function.
But the following expression is *not* an error;

    auto first_lambda = [&](auto && a, auto && b) {
        return a(b);
    };

The above "generic lambda" can be passed around, not causing an error unless it's called
with two ints, e.g.

    first_lambda(3,4);                      // an error
    first_lambda(strlen, "hello world");    // not an error

Both expressions can therefore be wrapped up in generic lambdas, and then passed
to a function (or, in my case, a method) that calls the correct one based on
the value of the boolean `condition`.
Here is an example of the underlying C++ code corresponding to this macro:

        int a=3; int b=4;
        forward_as_tuple_for_if_constexpr(a,b)
            .if_constexpr(
                    std::integral_constant<bool, true>{}
                ,   [&](auto&&a,auto&&b){ return a*b;    }  // returns '12'
                ,   [&](auto&&a,auto&&b){ return a(b);   }  // branch not taken
               )

You can see the two expressions wrapped inside generic lambdas. Because the
condition is `true` (i.e. `std::integral_constant<bool, true>{}` instead of
`std::integral_constant<bool, false>{}`) the first expression will used.

The values of the two variables are passed into the generic lambdas by first
storing them in a special object type created by `forward_as_tuple_for_if_constexpr`.
This has a method called `.if_constexpr` that takes the boolean flag as argument,
and also the two generic lambdas, and performs that calculation. This is
a strange order, but this order is helpful as it makes it possible to write the
macro.

The macro is variadic, but for simplicity I'll just describe the two-argument
special-case, `IF_CONSTEXPR_TWO`, in more detail here. Notice how this is
a macro with four arguments `IF_CONSTEXPR_TWO(a,b,false, a(b) )` followed by
`( a+b )`. The challenge is to ensure that the second expression is handled
correctly even though it isn't actually in the four arguments.

    std:: cout <<    IF_CONSTEXPR_TWO  ( a,b,  false,
           a(b)                     // branch not taken
    )(
           a+b                      // returns 7
    )
    << '\n';                        // prints '7'

This two-variable case can be implemented relatively straightforwardly, mapping
to the C++ code above.

    #define IF_CONSTEXPR_TWO(var1, var2, cond, exp1)                                 \
        forward_as_tuple_for_if_constexpr(var1,var2)                                 \
        .if_constexpr(                                                               \
                std::integral_constant<bool, cond>{}                                 \
            ,   TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) ->decltype(auto) { return exp1; }          \
            ,   TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) ->decltype(auto) LAMBDA_BODY_WITH_CLOSING_BRACE

alongside this simple macro:

    #define TWO_VARS_AS_GENERIC_LAMBDA_ARGS(var1,var2) [&](auto&&var1,auto&&var2)

The only interesting thing is that this macro takes four arguments, not five. The second
expression, `exp2`, isn't directly visible in this. To understand how the second expression
is (indirectly) handled by this first macro, observe how this macro ends with the
token `LAMBDA_BODY_WITH_CLOSING_BRACE`. Immediately following this is the second expression
wrapped in parentheses, `( a+b )`. And because `LAMBDA_BODY_WITH_CLOSING_BRACE`
is a macro function, the text `LAMBDA_BODY_WITH_CLOSING_BRACE(a+b)` is then expanded via

    #define LAMBDA_BODY_WITH_CLOSING_BRACE(...)  { return __VA_ARGS__; })

to complete the two-variable macro.

To understand the variadic version (`IF_CONSTEXPR`, taking between zero and three variable names
before the condition), see the header file for the details. It's basically about writing
functions to extract (or drop) the last argument (or second-to-last argument) from a collection
of macro arguments.  This is needed to extract the condition (the second-to-last argument
to `IF_CONSTEXPR`) or the first expression (the last argument) from the macro.

We also need to drop the last two arguments. It's easy to drop the first two arguments:

    #define           DROP_THE_FIRST_TWO_MACRO_ARGS(arg1, arg2, ...) (__VA_ARGS__)

so to drop the last two arguments, we first reverse the list of arguments, then drop
the first two, then reverse the remaining arguments again:

    #define            DROP_THE_LAST_TWO_MACRO_ARGS(...)  \
        EVAL(EVAL(  REVERSE DROP_THE_FIRST_TWO_MACRO_ARGS  REVERSE(__VA_ARGS__)  ))
