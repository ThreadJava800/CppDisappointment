#include "vector.hpp"

struct BigStruct {
    double a, b, c, d, e, f, g, h;
};

int main() {
    // BigStruct big;
    // m_vector::vector<BigStruct> a1 = {{1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8}};
    // m_vector::vector<BigStruct> a2 = a1;

    m_vector::vector<int> a1 = {1, 2, 3, 4, 5};
    m_vector::vector<int> a2 = a1;

    return 0;
}