#include <iostream>

#include "MInt.h"

MInt mmax(MInt& val1, MInt& val2) {
    if (val1 > val2) return val1;
    return val2;
}

MInt sumOf2(MInt& val1, MInt& val2, MInt& val3) {
    return val1 + val2 + val3;
}

int main() {
    MInt val1(5), val2(6), val3(7);

    // printf("MAX_VAL:\n");
    // MInt max_val = mmax(val1, val2);
    // printf("MAX_VAL = %d\n", max_val.value);

    // printf("SUM_VAL:\n");
    MInt sum_val = sumOf2(val1, val2, val3);
    // printf("SUM_VAl = %d\n", sum_val.value);

    return 0;
}