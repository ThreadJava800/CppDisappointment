#pragma once

#include <expected>
#include <stdexcept>

#include "condvar.hpp"
#include "mutex.hpp"

// TODO:
template<typename T>
struct shared_state {
public:

    explicit shared_state(std::nullptr_t = nullptr) noexcept 
      :  value          (),
         is_valid       (true),
         is_transferred (false),
         waiter         (),
         condvar_mutex  ()
    {}

    shared_state(shared_state&& other) {
        condvar_mutex  = std::move(other.condvar_mutex);
        is_valid       = std::move(other.is_valid);
        is_transferred = std::move(other.is_transferred);
        value          = std::move(other.value);
        waiter         = std::move(other.waiter);
    }

    shared_state& operator=(shared_state&& other) {
      if (this != &other) {
        condvar_mutex  = std::move(other.condvar_mutex);
        is_valid       = std::move(other.is_valid);
        is_transferred = std::move(other.is_transferred);
        value          = std::move(other.value);
        waiter         = std::move(other.waiter);
      }

      return *this;
    }



public:
    std::expected<T, std::exception_ptr> value;
    bool                                 is_valid;
    bool                                 is_transferred;

    condition_variable waiter;
    mutex              condvar_mutex;
};