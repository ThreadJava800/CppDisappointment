#pragma once

#include "../SharedPtr/shared_ptr.hpp"
#include "shared_state.hpp"

template<typename T>
class future {
public:

    future() noexcept
      :  value_state (new shared_state<T>) {

    }

    future(future&& other) noexcept {

    }

    future(const future& other) = delete;

private:
    SharedPtr<shared_state<T>, ClassicDeleter<shared_state<T>>> value_state;
};