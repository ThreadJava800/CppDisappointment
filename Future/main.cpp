#include <chrono>
#include <iostream>
#include <thread>

#include "future.hpp"
#include "promise.hpp"

void do_work(promise<int> barrier) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value(10);
}

int main() {
    promise<int> prom;
    future<int>  future_res = prom.get_future();
    std::thread work_thread(do_work, std::move(prom));
    
    using namespace std::literals::chrono_literals;
    auto res = future_res.wait_for(2s);

    std::cout << static_cast<int>(res) << '\n';
    work_thread.join();

    return 0;
}