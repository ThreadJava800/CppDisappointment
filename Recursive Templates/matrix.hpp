#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#include <cstdio>

#define VALUE_COUNT 5

// add values

template<typename... Other>
int addNums(Other... args);

template<typename T, typename... Other>
int addNums(T cur_val, Other... to_add) {
    return cur_val + addNums(to_add...);
}

template<> 
int addNums() {
    return 0;
}

// add simple arrays
struct Array {
    int values[VALUE_COUNT] ;
};

template<size_t cur_id>
void doArraySum(int* res_arr, Array& arr1, Array& arr2) {
    res_arr[cur_id] = arr1.values[cur_id] + arr2.values[cur_id];
    doArraySum<cur_id + 1>(res_arr, arr1, arr2);
}

template<>
void doArraySum<VALUE_COUNT>(int* res_arr, Array& arr1, Array& arr2) {
    return;
}

int* proceedArrSum(Array& arr1, Array& arr2) {
    static int values[VALUE_COUNT];

    doArraySum<0u>(values, arr1, arr2);

    return values;
}

#endif