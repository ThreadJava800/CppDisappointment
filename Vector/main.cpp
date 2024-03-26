#include <algorithm>
#include <iostream>
// #include <vector>

#include "vector.hpp"

int main() {
    m_vector::vector<int> test  = {1, 2, 3, 4, 5};
    m_vector::vector<int> test2 = {1000, 1001, 10002};
    m_vector::vector<int> test3 = {2000, 2001, 2002, 2003, 2004, 2005, 2006};
    auto print_func = [&]() {
        for (auto val : test) {
            std::cout << val << ' ';
        }
        std::cout << '\n';
    };
    print_func();

    // for_each test
    std::cout << "range_for print\n";
    auto one_val_printer = [](const int& val) {
        std::cout << val << " FOR_EACH ";
    };
    std::for_each(test.begin(), test.end(), one_val_printer);
    std::cout << '\n';


    // std::copy test
    std::cout << "std::copy\n";
    std::copy(test2.cbegin(), test2.cend(), test.begin());
    print_func();

    // std::sort test
    std::cout << "std::sort\n";
    std::sort(test.begin(), test.end());
    print_func();

    std::cout << "std::copy_if\n";
    std::copy_if(test3.begin(), test3.end(), test.begin(), [](const int& val){
        return val % 2 == 0;
    });
    print_func();


    // print_func();
    test[2] = 45;

    test.insert(test.begin() + 4, 42);
    std::cout << "AFTER INSERT\n";
    print_func();

    test.erase(test.begin() + 4);
    std::cout << "AFTER ERASE\n";
    print_func();

    test.erase(test.begin() + 1, test.begin() + 3);
    std::cout << "AFTER INT ERASE\n";
    print_func();

    std::cout << "BEFORE RESERVE " << test.capacity() << '\n';
    print_func();

    std::cout << "AFTER RESERVE " << test.capacity() << '\n';
    test.reserve(40);
    print_func();

    std::cout << "PUSH_BACK VAL\n";
    int res = test.emplace_back(456);
    print_func();

    std::cout << "RESIZE UP\n";
    test.resize(24, 42);
    print_func();

    std::cout << "RESIZE DOWN\n";
    test.resize(5);
    print_func();

    std::cout << "INSERT ONE VAL (11)\n";
    test.insert(test.begin() + 2, 11);
    print_func();

    std::cout << "INSERT RANGE (15, 16, 17)\n";
    test.insert(test.begin() + 4, {15, 16, 17});
    print_func();

    std::cout << "INSERT RANGE (100, 101, 102) x2\n";
    m_vector::vector<int> tmp = {100, 101, 102};
    test.insert(test.begin() + 6, tmp.begin(), tmp.end());
    print_func();

    return 0;
}