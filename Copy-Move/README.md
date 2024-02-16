# Cpp Disappointment

It is a small educational project aimed to understand how many unnecessary object copies does 'C++' creates. 

I created an own implementation of Integer class and tested it on different basic functions.

Here's example of calculation of sum of 3 values (a + b + c):

![naive_sum_2](https://github.com/ThreadJava800/CppDisappointment/blob/master/img_naive/sum_of_3.svg)

As we can see two copy constructors were called and also too many constructor calls. We are not happy with this!

Let's override move constructor (as well as move operator=):

![move_sum_2](https://github.com/ThreadJava800/CppDisappointment/blob/master/img_move_ctor/sum_of_3.svg)

![no_+_sum_2](https://github.com/ThreadJava800/CppDisappointment/blob/master/img_w_flags/sum_of_3.svg)

![no_w_sum_2](https://github.com/ThreadJava800/CppDisappointment/blob/master/img/sum_of_3.svg)