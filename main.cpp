#include <iostream>

#include "MInt.h"

typedef MInt (*TwoValFunc  )(MInt& val1, MInt& val2);
typedef MInt (*ThreeValFunc)(MInt& val1, MInt& val2, MInt& val3);

static const int M_RAND_MAX = 10;

MInt mmax(MInt& val1, MInt& val2) {
    if (val1 > val2) return val1;
    return val2;
}

MInt sumOf2(MInt& val1, MInt& val2) {
    return val1 + val2;
}

MInt sumOf3(MInt& val1, MInt& val2, MInt& val3) {
    return val1 + val2 + val3;
}

MInt sameValFunc(MInt& val) {
    return val;
}

MInt sumOfFuncs(MInt& val1, MInt& val2) {
    return sameValFunc(val1) + sameValFunc(val2);
}

void initDotFile() {
    DOT_FILE = fopen(GRAPHVIZ_FILE_NAME, "w");
    mprintf(DOT_FILE, "digraph structs {\n");
    mprintf(DOT_FILE, "\trankdir=LR;\n");

    VARIABLES_CNT = 0;
    OPER_CNT      = 0;
    INFO_STR      = new char[MAX_INFO_LEN];
    INFO_STR_POS  = 0;
}

void createGraphFromDot(const char* image_name) {
    mprintf(DOT_FILE, "}");
    fclose(DOT_FILE);

    char command[MAX_NAME_LENGTH] = {};
    sprintf(command, "dot -Tsvg %s > %s", GRAPHVIZ_FILE_NAME, image_name);
    system(command);

    delete[] INFO_STR;
}

void twoValDriver(const TwoValFunc call_func, const char* image_name) {
    printf("\n=================\n");
    initDotFile();

    int rand_val1 = rand() % M_RAND_MAX;
    int rand_val2 = rand() % M_RAND_MAX;

    MInt val1(rand_val1), val2(rand_val2);
    call_func(val1, val2);

    createGraphFromDot(image_name);
}

void threeValDriver(const ThreeValFunc call_func, const char* image_name) {
    printf("\n=================\n");
    initDotFile();

    int rand_val1 = rand() % M_RAND_MAX;
    int rand_val2 = rand() % M_RAND_MAX;
    int rand_val3 = rand() % M_RAND_MAX;

    MInt val1(rand_val1), val2(rand_val2), val3(rand_val3);
    call_func(val1, val2, val3);

    createGraphFromDot(image_name);
}

int main() {
    srand(time(NULL));

    twoValDriver  (mmax,       "img/max.svg");
    twoValDriver  (sumOf2,     "img/sum_of_2.svg");
    twoValDriver  (sumOfFuncs, "img/func_sum.svg");
    threeValDriver(sumOf3,     "img/sum_of_3.svg");

    return 0;
}