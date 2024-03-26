#include <iostream>

#include "function.hpp"

void testfun() {
    std::cout << "TEST FUN\n";
}

int hello(int a, int b) {
    return a + b;
}

int main() {
    int a  = 2;
    a = 10;
    Function<void()> test([=]{
        std::cout << "TEST " << a << " \n";
    });
    test();

    Function<int(int, int)> hello_fun(hello);
    int res = hello_fun(2, 3);
    std::cout << res << '\n';

    Function<int()> hello_bind(bind(&hello, 3, 4));
    std::cout << hello_bind();

    // Function<void()> test2(testfun);
    // test = std::move(test2);


    return 0;
}