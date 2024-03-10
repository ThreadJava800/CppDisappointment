#include <vector>

#include "iterator.hpp"

int main() {
    istream_iterator<int> istream_start(std::cin);
    istream_iterator<int> istream_end;

    std::vector<int> test_vector;
    push_back_iterator<std::vector<int>, int> vector_iter(test_vector);

    std::copy(istream_start, istream_end, vector_iter);

    ostream_iterator<int> ostream_test(std::cout, " ");
    std::copy(test_vector.begin(), test_vector.end(), ostream_test);

    return 0;
}