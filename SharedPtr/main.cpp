#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "shared_ptr.hpp"

void testThread(const char* thread_name, SharedPtr<int, DefaultDeleter<int>, SharedAllocator<int>> ptr) {
    sleep(1);   // stop thread for a second

    auto ptr_cpy = ptr;

    static std::mutex mutex;
    std::lock_guard<std::mutex> lk(mutex);

    printf("THREAD: %s, PTR: %p, value: %d, use_cnt: %lu\n", thread_name, ptr_cpy.get(), *ptr_cpy, ptr_cpy.useCount());
}

int main() {
    int test_val = 5;
    auto shared_ptr = makeShared<int>(5);

    std::thread t1{testThread, "t1", shared_ptr}, t2{testThread, "t2", shared_ptr}, t3{testThread, "t3", shared_ptr};

    // std::cout << *shared_ptr << '\n';
    // *shared_ptr = 4;
    // std::cout << *shared_ptr << '\n';

    t1.join(); t2.join(); t3.join();
    printf("PTR: %p, value: %d, use_cnt: %lu\n", shared_ptr.get(), *shared_ptr, shared_ptr.useCount());

    return 0;
}