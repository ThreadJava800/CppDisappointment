#include <iostream>

#include "function.hpp"

void testfun() {
    std::cout << "TEST FUN\n";
}

int main() {
    Function<void()> test([]{
        std::cout << "TEST\n";
    });

    Function<void()> test2(testfun);

    test = std::move(test2);

    test();

    return 0;
}