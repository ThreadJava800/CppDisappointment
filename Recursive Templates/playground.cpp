#include <iostream>

#include "playground.hpp"

constexpr int MAX_RAND_VALUE = 10;

int main() {
    srand((unsigned)time(NULL));

    std::cout << addNums(1, 2, 3, 4, 5) << '\n';

    // sum of arrays

    // Array arr1, arr2;
    // for (size_t i = 0; i < VALUE_COUNT; i++) {
    //     arr1.values[i] = rand() % MAX_RAND_VALUE;
    //     arr2.values[i] = rand() % MAX_RAND_VALUE;
    // }

    // int* result = proceedArrSum(arr1, arr2);
    // for (size_t i = 0; i < VALUE_COUNT; i++) {
    //     std::cout << result[i] << ' ';
    // }
    // std::cout << '\n';

    // for (size_t i = 0; i < VALUE_COUNT; i++) {
    //     std::cout << arr1.values[i] << ' ';
    // }
    // std::cout << '\n';

    // for (size_t i = 0; i < VALUE_COUNT; i++) {
    //     std::cout << arr2.values[i] << ' ';
    // }
    // std::cout << '\n';

    // sum of arr2

    Array2 arr1, arr2;
    for (size_t i = 0; i < VALUE_COUNT; i++) {
        for (size_t j = 0; j < COLUMN_COUNT; j++) {
            arr1.values[j][i] = rand() % MAX_RAND_VALUE;
            arr2.values[j][i] = rand() % MAX_RAND_VALUE;
        }
    }

    Array2 res_arr = proceedArr2Sum(arr1, arr2);

    for (size_t i = 0; i < VALUE_COUNT; i++) {
        for (size_t j = 0; j < COLUMN_COUNT; j++) {
            std::cout << arr1.values[j][i] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    for (size_t i = 0; i < VALUE_COUNT; i++) {
        for (size_t j = 0; j < COLUMN_COUNT; j++) {
            std::cout << arr2.values[j][i] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    for (size_t i = 0; i < VALUE_COUNT; i++) {
        for (size_t j = 0; j < COLUMN_COUNT; j++) {
            std::cout << res_arr.values[j][i] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}