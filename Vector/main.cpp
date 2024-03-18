#include <iostream>
// #include <vector>

#include "vector.hpp"

int main() {
    m_vector::vector<int> test = {1, 2, 3, 4, 5};
    // m_vector::vector<int> test(5, 0);
    // for (size_t i = 0; i < 5; i++) test[i] = i;
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    test[2] = 45;

    test.insert(test.begin() + 4, 42);
    std::cout << "AFTER INSERT\n";
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    test.erase(test.begin() + 4);
    std::cout << "AFTER ERASE\n";
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    test.erase(test.begin() + 1, test.begin() + 3);
    std::cout << "AFTER INT ERASE\n";
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "BEFORE RESERVE " << test.capacity() << '\n';
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';    
    test.reserve(40);
    std::cout << "AFTER RESERVE " << test.capacity() << '\n';
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "PUSH_BACK VAL\n";
    int res = test.emplace_back(456);
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "RESIZE UP\n";
    test.resize(24, 42);
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "RESIZE DOWN\n";
    test.resize(5);
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "INSERT ONE VAL (11)\n";
    test.insert(test.begin() + 2, 11);
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "INSERT RANGE (15, 16, 17)\n";
    test.insert(test.begin() + 4, {15, 16, 17});
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "INSERT RANGE (100, 101, 102) x2\n";
    m_vector::vector<int> tmp = {100, 101, 102};
    test.insert(test.begin() + 6, tmp.begin(), tmp.end());
    for (size_t i = 0; i < test.size(); i++) {
        std::cout << test[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}