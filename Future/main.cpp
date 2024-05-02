#include "future.hpp"

int main() {
    future<int> test;
    test.get();

    return 0;
}