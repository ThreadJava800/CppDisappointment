#pragma once

#include <condition_variable>
#include <expected>
#include <mutex>
#include <stdexcept>
#include <optional>

// TODO:
template<typename T>
struct shared_state {
public:

    explicit shared_state(std::nullptr_t = nullptr) noexcept 
      :  value          (),
         is_valid       (true),
         is_transferred (false),
         waiter         (),
         mutex          ()
    {}

    shared_state(shared_state&& other) {
        mutex          = std::move(other.mutex);
        is_valid       = std::move(other.is_valid);
        is_transferred = std::move(other.is_transferred);
        value          = std::move(other.value);
        waiter         = std::move(other.waiter);
    }

    shared_state& operator=(shared_state&& other) {
      if (this != &other) {
        mutex          = std::move(other.mutex);
        is_valid       = std::move(other.is_valid);
        is_transferred = std::move(other.is_transferred);
        value          = std::move(other.value);
        waiter         = std::move(other.waiter);
      }

      return *this;
    }



public:
    std::optional<std::expected<T, std::exception_ptr>> value;
    bool                                                is_valid;
    bool                                                is_transferred;

    std::condition_variable waiter;
    std::mutex              mutex;
};