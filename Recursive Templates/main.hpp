#ifndef _MAIN_H_
#define _MAIN_H_

#include <cstdio>

const size_t MAX_RAND_VALUE = 10;
const size_t COL_CNT        = 5;
const size_t ROW_CNT        = 3;
typedef int T;

struct Array1Dim {
    T columns[COL_CNT];
};

struct Array2Dim {
    Array1Dim rows[ROW_CNT];
};

template<size_t col_id>
void doValuesSum(Array1Dim& res_arr1, Array1Dim& arr1, Array1Dim& arr2) {
    res_arr1->columns[col_id] = arr1->columns[col_id] + arr2->columns[col_id];
    doValuesSum<col_id + 1>(res_arr1, arr1, arr2);
}

template<>
void doValuesSum<COL_CNT>(Array1Dim& res_arr1, Array1Dim& arr1, Array1Dim& arr2) {
    return;
}

template<size_t row_id>
void doRowSum(Array2Dim& res_arr2, Array2Dim& arr1, Array2Dim& arr2) {
    doValuesSum<0u>(&res_arr2->rows[row_id], &arr1->rows[row_id], &arr2->rows[row_id]);
    doRowSum<row_id + 1>(res_arr2, arr1, arr2);
}

template<>
void doRowSum<ROW_CNT>(Array2Dim& res_arr2, Array2Dim& arr1, Array2Dim& arr2) {
    return;
}

Array2Dim* proceedArr2Sum(Array2Dim& arr1, Array2Dim& arr2) {
    static Array2Dim res_arr;

    doRowSum<0u>(res_arr, arr1, arr2);

    return &res_arr;
}

#endif