This started in response to this tweet by Eric Niebler
https://twitter.com/ericniebler/status/859985304068816896

Aaron McDaid aaron.mcdaid@gmail.com  2017-05-06

This file is the MACRO magic. In if_constexpr.hh, I have my
own implementation of if_constexpr.

Basically, you need at least one trailing parenthesis in order
for the preprocessor to be able to see far enough ahead. i.e.
this is possible:
       IF_CONSTEXPR (true ) (a,b)
         a*b )
         a+b );
or even this to go to the extreme:
       IF_CONSTEXPR (true
         a   )
         b   )
         a*b )
         a+b );
However, text editors would be unhappy with the unbalanced
parentheses. Hence, I hacked it a bit more so that I
could type two parentheses just before IF_CONSTEXPR.
Anyway, this works:
     ((IF_CONSTEXPR (true ) (a,b)
         a*b )
         a+b );
