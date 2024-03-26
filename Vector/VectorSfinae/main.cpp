#include "vector.hpp"

struct BigStruct {
    double a, b, c, d, e, f, g, h;
    // BigStruct() {
    // }
    // BigStruct(double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h)
    //     : a(_a), b(_b), c(_c), d(_d), e(_e), f(_f), g(_g), h(_h) {  
    // }
    // BigStruct(BigStruct& other) {
    // }
};

int main() {
    m_vector::vector<BigStruct> a1 = {{1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8}};
    m_vector::vector<BigStruct> a2 = a1;

    // m_vector::vector<int> a1 = {1, 2, 3, 4, 5};
    // m_vector::vector<int> a2 = a1;

    return 0;
}