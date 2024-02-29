#include <iostream>

#include "function.hpp"

void testfun() {
    std::cout << "TEST FUN\n";
}

int main() {
    Function<void()> test([]{
        std::cout << "TEST\n";
    });

    auto yyy = []{
        std::cout << "TEST\n";
    };

    // Function<void()> test(testfun);

    test();

    return 0;
}