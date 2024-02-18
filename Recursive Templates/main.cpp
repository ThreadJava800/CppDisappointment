#include "main.hpp"

#include <ctime>
#include <cstdlib>
#include <cstdio>

int main() {
    srand((unsigned)time(NULL));

    Array2Dim arr1, arr2;
    for (size_t i = 0; i < ROW_CNT; i++) {
        for (size_t j = 0; j < COL_CNT; j++) {
            arr1.rows[i].columns[j] = rand() % MAX_RAND_VALUE;
            arr2.rows[i].columns[j] = rand() % MAX_RAND_VALUE;
        }
    }

    Array2Dim* res_arr = proceedArr2Sum(&arr1, &arr2);

    for (size_t i = 0; i < ROW_CNT; i++) {
        for (size_t j = 0; j < COL_CNT; j++) {
            printf("%.2d ", arr1.rows[i].columns[j]);
        }
        printf("\n");
    }
    printf("\n");

    for (size_t i = 0; i < ROW_CNT; i++) {
        for (size_t j = 0; j < COL_CNT; j++) {
            printf("%.2d ", arr2.rows[i].columns[j]);
        }
        printf("\n");
    }
    printf("\n");

    for (size_t i = 0; i < ROW_CNT; i++) {
        for (size_t j = 0; j < COL_CNT; j++) {
            printf("%.2d ", res_arr->rows[i].columns[j]);
        }
        printf("\n");
    }

    return 0;
}