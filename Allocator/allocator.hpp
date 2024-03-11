#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <cstddef>
#include <limits>
#include <new>
#include <utility>

using ptrdiff_t = long;

template<typename T>
class allocator {
public:

    using value_type      = T;
    using pointer_type    = T*;
    using reference_type  = T&;
    using size_type       = std::size_t;
    using difference_type = ptrdiff_t;

    explicit allocator() {}

    pointer_type address(reference_type value) {
        return static_cast<pointer_type>(&value);
    }

    [[nodiscard]] constexpr pointer_type allocate(size_type n) {
        return static_cast<pointer_type>(::operator new (sizeof(value_type) * n));
    }

    template<typename U, typename... Args>
    constexpr void construct(U* ptr, Args&&... args) {
        ::new (ptr) U(std::forward<Args>(args)...);
    }

    constexpr void deallocate(pointer_type ptr, size_type n) {
        ::operator delete (ptr, sizeof(value_type) * n);
    }

    template<typename U>
    constexpr void destroy(U* ptr) {
        if (ptr) ptr->~U();
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
};

template<class Alloc>
class allocator_traits {
public:

    using allocator_type  = Alloc;
    using value_type      = Alloc::value_type;
    using pointer_type    = Alloc::pointer_type;
    using difference_type = Alloc::difference_type;
    using size_type       = Alloc::size_type;

    explicit allocator_traits() {}

    [[nodiscard]] static constexpr pointer_type allocate(Alloc& allocator, size_type n) {
        return allocator.allocate(n);
    } 

    static constexpr void deallocate(Alloc& allocator, pointer_type ptr, size_type n) {
        allocator.deallocate(ptr, n);
    }

    template<typename U, typename... Args>
    static constexpr void construct(Alloc& allocator, U* ptr, Args&&... args) {
        allocator.construct(ptr, args...);
    }

    template<class U>
    static constexpr void destroy(Alloc& allocator, U* ptr) {
        allocator.destroy(ptr);
    }

    static constexpr size_type max_size(const Alloc& allocator) noexcept {
        return allocator.max_size();
    }
};

#endif