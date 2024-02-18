#include "matrix.hpp"

#include <iostream>

template<typename T>
Matrix<T>::Matrix() :
    values (nullptr),
    rows   (0),
    columns(0)       {
        std::cout << "IN\n";
    }

template<typename T>
Matrix<T>::Matrix(size_t _rows, size_t _columns) :
    rows   (_rows),
    columns(_columns) {

    }