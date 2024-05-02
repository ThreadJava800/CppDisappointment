#pragma once

#include <chrono>

#include "../Functor/function.hpp"
#include "mutex.hpp"

enum class cv_status {
    no_timeout,
    timeout  
};

class condition_variable {
 private:

  template <class Mutex>
  void wait_wrapper(Mutex& mutex, Function<void(uint32_t)> wait_func) {
    uint32_t wait_val = cur_.load();

    while (wait_val & 1) {
      if (cur_.compare_exchange_strong(wait_val, wait_val + 1)) {
        wait_val++;
      }
    }

    mutex.unlock();
    wait_func(wait_val);
    mutex.lock();
  }

public:

  template <class Mutex>
  void wait(Mutex& mutex) {
    wait_wrapper(mutex, Function<void(uint32_t)>([this](uint32_t wait_val) {
      futex_wait(&cur_, wait_val);
    }));
  }

  template<class Rep, class Period, class Mutex>
  cv_status wait_for(Mutex& mutex, const std::chrono::duration<Rep, Period>& rel_time) {
    cv_status result = cv_status::timeout;
    
    wait_wrapper(mutex, Function<void(uint32_t)>([&, this](uint32_t wait_val) {

        auto seconds     = std::chrono::duration_cast<std::chrono::seconds>    (rel_time);
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(rel_time);
        nanoseconds -= seconds;

        struct timespec c_time{seconds.count(), nanoseconds.count()};
        auto futex_code = futex_wait(&cur_, wait_val, &c_time);

        if (futex_code == -1) {
            result = cv_status::timeout;
        } else {
            result = cv_status::no_timeout;
        }
    }));

    return result;
  }

  template<class Clock, class Duration, class Mutex>
  cv_status wait_until(Mutex& mutex, const std::chrono::time_point<Clock, Duration>& abs_time) {
    cv_status result = cv_status::timeout;
    
    wait_wrapper(mutex, Function<void(uint32_t)>([&, this](uint32_t wait_val) {

        auto now         = std::chrono::system_clock::now();
        auto seconds     = std::chrono::duration_cast<std::chrono::seconds>    (abs_time - now);
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(abs_time - now);
        nanoseconds -= seconds;

        struct timespec c_time{seconds.count(), nanoseconds.count()};
        long futex_code = futex_wait(&cur_, wait_val, &c_time);

        if (futex_code == -1) {
            result = cv_status::timeout;
        } else {
            result = cv_status::no_timeout;
        }
    }));

    return result;
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
