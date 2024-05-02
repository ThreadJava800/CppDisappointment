#pragma once

#include "future.hpp"
#include "../SharedPtr/shared_ptr.hpp"

template<class R>
class promise {
public:

    explicit promise()
      : value_state (new shared_state<R>)
    {}

    promise(promise&& other) noexcept {
        value_state = std::move(other.value_state);
    }

    promise& operator=(promise&& other) noexcept {
        if (this != &other) {
            value_state = std::move(other.value_state);
        }

        return *this;
    }


    promise           (const promise& other) = delete;
    promise& operator=(const promise& other) = delete;

    void swap(promise& other) noexcept {
        value_state.swap(other.value_state);
    }

    future<R> get_future() {
        return future(value_state);
    }

    void set_value(const R& value) {
        std::unique_lock lock(value_state->mutex);

        value_state->value          = std::optional<std::expected<R, std::exception_ptr>>(std::move(value));
        value_state->is_transferred = true;
        value_state->waiter.notify_all();
    }

    // void set_value(R&& value) {
    //     set_value(std::move(value));
    // }

    void set_exception(std::exception_ptr error) {
        std::unique_lock lock(value_state->mutex);

        value_state->value          = std::optional<std::expected<R, std::exception_ptr>>(std::unexpected(error));
        value_state->is_transferred = true;
        value_state->waiter.notify_all();
    }

private:
    SharedPtr<shared_state<R>, ClassicDeleter<shared_state<R>>> value_state;
};