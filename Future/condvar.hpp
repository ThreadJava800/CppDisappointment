#pragma once

#include "mutex.hpp"

class condition_variable {
 private:
 public:

  template <class Mutex>
  void wait(Mutex& mutex) {
    uint32_t wait_val = cur_.load();

    while (wait_val & 1) {
      if (cur_.compare_exchange_strong(wait_val, wait_val + 1)) {
        wait_val++;
      }
    }

    mutex.unlock();
    syscall(SYS_futex, (uint32_t*)&cur_, FUTEX_WAIT, 2, 0, 0, 0);
    mutex.lock();
  }

  void notify_one() {
    cur_.fetch_add(1);
    syscall(SYS_futex, (uint32_t*)&cur_, FUTEX_WAKE, 1, 0, 0, 0);
  }

  void notify_all() {
    cur_.fetch_add(1);
    syscall(SYS_futex, (uint32_t*)&cur_, FUTEX_WAKE, 0, 0, 0, 0);
  }

 private:
 std::atomic<uint32_t> cur_{0u};
};
