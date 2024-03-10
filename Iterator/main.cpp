#include <vector>

#include "iterator.hpp"

int main() {
    std::vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.push_back(i);
    }

    ostream_iterator<int> ostream_test(std::cout, ",");
    std::copy(values.begin(), values.end(), ostream_test);

    return 0;
}