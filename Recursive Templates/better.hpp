#ifndef _BETTER_H_
#define _BETTER_H_

#include <cstdio>

template<typename T, size_t col_cnt>
struct Array1Dim {
    T array[col_cnt];
};

template<typename T, size_t col_cnt, size_t row_cnt>
struct Array2Dim {
    Array1Dim<T, col_cnt> arrays[row_cnt];
};


template<typename T, size_t col_cnt, size_t index>
struct SumArray1Dim {
    void operator() (Array1Dim<T, col_cnt>& res_arr, Array1Dim<T, col_cnt>& arr1, Array1Dim<T, col_cnt>& arr2) {
        res_arr.array[index] = arr1.array[index] + arr2.array[index];

        SumArray1Dim<T, col_cnt, index + 1> next_iter;
        next_iter(res_arr, arr1, arr2);
    }
};

template<typename T, size_t col_cnt>
struct SumArray1Dim<T, col_cnt, col_cnt> {
    void operator() (Array1Dim<T, col_cnt>& res_arr, Array1Dim<T, col_cnt>& arr1, Array1Dim<T, col_cnt>& arr2) {
        return;
    }
};

template<typename T, size_t col_cnt, size_t row_cnt, size_t index>
struct SumArray2Dim {
    void operator() (Array2Dim<T, col_cnt, row_cnt>& res_arr, Array2Dim<T, col_cnt, row_cnt>& arr1, Array2Dim<T, col_cnt, row_cnt>& arr2) {
        SumArray1Dim<T, col_cnt, 0> array_sum;
        array_sum(res_arr.arrays[index], arr1.arrays[index], arr2.arrays[index]);

        SumArray2Dim<T, col_cnt, row_cnt, index + 1> arrays_sum;
        arrays_sum(res_arr, arr1, arr2);
    }
};

template<typename T, size_t col_cnt, size_t row_cnt>
struct SumArray2Dim<T, col_cnt, row_cnt, row_cnt> {
    void operator() (Array2Dim<T, col_cnt, row_cnt>& res_arr, Array2Dim<T, col_cnt, row_cnt>& arr1, Array2Dim<T, col_cnt, row_cnt>& arr2) {
        return;
    }
};

template<typename T, size_t col_cnt, size_t row_cnt>
Array2Dim<T, col_cnt, row_cnt>& proceedArr2Sum(Array2Dim<T, col_cnt, row_cnt>& arr1, Array2Dim<T, col_cnt, row_cnt>& arr2) {
    static Array2Dim<T, col_cnt, row_cnt> res_arr;

    SumArray2Dim<T, col_cnt, row_cnt, 0> dim2_sum;
    dim2_sum(res_arr, arr1, arr2);

    return res_arr;
}

#endif