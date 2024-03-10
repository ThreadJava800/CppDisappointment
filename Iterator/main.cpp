#include <vector>

#include "iterator.hpp"

int main() {
    istream_iterator<int> istream_start(std::cin);
    istream_iterator<int> istream_end;

    ostream_iterator<int> ostream_test(std::cout, ",");
    std::copy(istream_start, istream_end, ostream_test);

    return 0;
}