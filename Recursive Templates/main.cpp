#include <iostream>

#include "matrix.hpp"

constexpr int MAX_RAND_VALUE = 10;

int main() {
    srand((unsigned)time(NULL));

    std::cout << addNums(1, 2, 3, 4, 5) << '\n';

    Array arr1, arr2;
    for (size_t i = 0; i < VALUE_COUNT; i++) {
        arr1.values[i] = rand() % MAX_RAND_VALUE;
        arr2.values[i] = rand() % MAX_RAND_VALUE;
    }

    int* result = proceedArrSum(arr1, arr2);
    for (size_t i = 0; i < VALUE_COUNT; i++) {
        std::cout << result[i] << ' ';
    }
    std::cout << '\n';

    for (size_t i = 0; i < VALUE_COUNT; i++) {
        std::cout << arr1.values[i] << ' ';
    }
    std::cout << '\n';

    for (size_t i = 0; i < VALUE_COUNT; i++) {
        std::cout << arr2.values[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}