#pragma once

#include "../SharedPtr/shared_ptr.hpp"
#include "shared_state.hpp"

enum class future_status {
    ready,
    timeout,
    deferred
};

template<typename T>
class future {

    template<class R>
    friend class promise;

private:

    explicit future(SharedPtr<shared_state<T>, ClassicDeleter<shared_state<T>>> _value_state) noexcept 
      : value_state (_value_state)
    {}

public:

    future() noexcept
      :  value_state (new shared_state<T>) {
    }

    future(future&& other) noexcept {
        value_state = std::move(other.value_state);
    }

    future& operator=(future&& other) noexcept {
        if (this != &other) {
            value_state = std::move(other.value_state);
        }

        return *this;
    }

    future           (const future& other) = delete;
    future& operator=(const future& other) = delete;

    // if valid() == false then behaviour is undefined
    T get() {
        unique_lock lock(value_state->condvar_mutex);

        if (!(value_state->is_transferred)) {
            lock.unlock();
            wait();
        }

        value_state->is_valid = false;
        
        if (!value_state->value.has_value()) {
            std::rethrow_exception(value_state->value.error());
        }

        return std::move(value_state->value.value());
    }

    bool valid() const noexcept {
        return value_state->is_valid;
    }

    void wait() const {
        if (!valid()) return;

        unique_lock lock(value_state->condvar_mutex);

        while (!(value_state->is_transferred)) {
            value_state->waiter.wait(lock);
        }
    }

    template<class Rep, class Period>
    future_status wait_for(const std::chrono::duration<Rep,Period>& timeout_duration) const {
        unique_lock lock(value_state->condvar_mutex);
        future_status result_code = future_status::deferred;

        if (!(value_state->is_transferred)) {
            // result_code = static_cast<future_status>(value_state->waiter.wait_for(lock, timeout_duration));
        }

        return result_code;
    }

    template<class Clock, class Duration>
    future_status wait_until(const std::chrono::time_point<Clock,Duration>& timeout_time) const {
        unique_lock lock(value_state->condvar_mutex);
        future_status result_code = future_status::deferred;

        if (!(value_state->is_transferred)) {
            // result_code = static_cast<future_status>(value_state->waiter.wait_until(lock, timeout_time));
        }

        return result_code;
    }

private:
    SharedPtr<shared_state<T>, ClassicDeleter<shared_state<T>>> value_state;
};