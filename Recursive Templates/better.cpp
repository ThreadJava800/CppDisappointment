#include <ctime>
#include <iostream>

#include "better.hpp"

const size_t ROW_CNT        = 3;
const size_t COL_CNT        = 5;
const size_t MAX_RAND_VALUE = 10;

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    // initialize arrays
    Array2Dim<int, COL_CNT, ROW_CNT> arr1, arr2;
    for (size_t i = 0; i < COL_CNT; i++) {
        for (size_t j = 0; j < ROW_CNT; j++) {
            arr1.arrays[j].array[i] = rand() % MAX_RAND_VALUE;
            arr2.arrays[j].array[i] = rand() % MAX_RAND_VALUE;
        }
    }

    Array2Dim<int, COL_CNT, ROW_CNT> sum_arr = 
        proceedArr2Sum<int, COL_CNT, ROW_CNT>(arr1, arr2);

    for (size_t i = 0; i < COL_CNT; i++) {
        for (size_t j = 0; j < ROW_CNT; j++)
            std::cout << arr1.arrays[j].array[i] << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';

    for (size_t i = 0; i < COL_CNT; i++) {
        for (size_t j = 0; j < ROW_CNT; j++)
            std::cout << arr2.arrays[j].array[i] << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';

    for (size_t i = 0; i < COL_CNT; i++) {
        for (size_t j = 0; j < ROW_CNT; j++)
            std::cout << sum_arr.arrays[j].array[i] << ' ';
        std::cout << '\n';
    }

    return 0;
}