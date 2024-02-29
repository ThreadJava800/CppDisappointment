#include <iostream>

#include "shared_ptr.hpp"

int main() {
    int test_val = 5;
    auto shared_ptr = makeShared<int>(5);

    std::cout << *shared_ptr << '\n';
    *shared_ptr = 4;
    std::cout << *shared_ptr << '\n';

    return 0;
}