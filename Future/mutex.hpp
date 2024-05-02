#pragma once

#include <atomic>
#include <linux/futex.h>      /* Definition of FUTEX_* constants */
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>

template<typename T>
long futex_wait(std::atomic<T>* addr, T val, const struct timespec* time = NULL) {
    return syscall(SYS_futex, addr, FUTEX_WAIT, val, time, 0, 0);
}

class mutex {
 private:

  static const uint32_t kUnlocked    = 0u;
  static const uint32_t kLockedEmpty = 1u;
  static const uint32_t kLockedFull  = 2u;

 public:
  void Lock() {
    uint32_t cur_state = kUnlocked;
    occupied_.compare_exchange_strong(cur_state, kLockedEmpty);

    // mutex is not occupied
    if (cur_state != kUnlocked) {
      do {
        cur_state = kLockedEmpty;
        occupied_.compare_exchange_strong(cur_state, kLockedFull);

        if (cur_state != kUnlocked) {
          futex_wait(&occupied_, kLockedFull);
        }

        cur_state = kUnlocked;
        occupied_.compare_exchange_strong(cur_state, kLockedFull);

      } while (cur_state != kUnlocked);
    }
  }

  void Unlock() {
    uint32_t one = kLockedEmpty;
    if (occupied_.compare_exchange_strong(one, kUnlocked)) {
      return;
    }

    occupied_.store(kUnlocked);
    syscall(SYS_futex, (uint32_t*)&occupied_, FUTEX_WAKE, 1, 0, 0, 0);
  }

  void lock() { 
    Lock();
  }

  void unlock() {
    Unlock();
  }

 private:
  std::atomic<uint32_t> occupied_{kUnlocked};
};

template <typename Mutex>
class unique_lock {
 public:
  explicit unique_lock(Mutex& mutex)
      : kept_mutex_(&mutex) {
    mutex.lock();
  }

  void lock() {
    kept_mutex_->lock();
  }

  void unlock() {
    kept_mutex_->unlock();
  }

  ~unique_lock() {
    kept_mutex_->unlock();
  }

  // Non-copyable
  unique_lock(const unique_lock&) = delete;
  unique_lock& operator=(const unique_lock&) = delete;

  // Non-movable
  unique_lock(unique_lock&&) = delete;
  unique_lock& operator=(unique_lock&&) = delete;

 private:
  Mutex* kept_mutex_ = nullptr;
};

