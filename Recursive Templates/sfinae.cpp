#include <ctime>
#include <iostream>
#include <string>

#include "sfinae_impl.hpp"

const size_t ROW_CNT        = 3;
const size_t COL_CNT        = 5;
const size_t MAX_RAND_VALUE = 10;

// struct Test {
//     Test()          {}
//     Test(int value) {}

//     Test operator+(const Test&) {
//         return Test();
//     }

//     // Test* operator+(const Test&) {
//     //     return 0;
//     // }

//     friend std::ostream& operator<<(std::ostream& stream, const Test& Test) {
//         stream << "IT'S TEST";
//         return stream;
//     }
// };

using CurrentType = size_t;

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    // initialize arrays
    Array2Dim<CurrentType, COL_CNT, ROW_CNT> arr1, arr2;
    for (size_t i = 0; i < COL_CNT; i++) {
        for (size_t j = 0; j < ROW_CNT; j++) {
            arr1.arrays[j].array[i] = rand() % MAX_RAND_VALUE;
            arr2.arrays[j].array[i] = rand() % MAX_RAND_VALUE;
        }
    }

    Array2Dim<CurrentType, COL_CNT, ROW_CNT> sum_arr = 
        proceedArr2Sum<CurrentType, COL_CNT, ROW_CNT>(arr1, arr2);

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