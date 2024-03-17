#pragma once

#include <cstdio>
#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "../Allocator/allocator.hpp"
#include "../Iterator/iterator.hpp"

namespace m_vector{

template<
        typename T,
        class Allocator = allocator<T>
        >
class vector {
public:

    //=============MEMBER TYPES=============//
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = Allocator::pointer_type;
    using const_pointer   = const Allocator::pointer_type;

private:
    class iterator : public input_iterator<T> {
    public:
        constexpr explicit iterator(pointer ptr)
            : ptr_(ptr) {
        }
        constexpr reference operator*() const {
            if (!ptr_) throw std::invalid_argument("Iterator ptr is NULL!");
            return *ptr_;
        }
        constexpr iterator& operator++() {
            ptr_++;
            return *this;
        }
        constexpr iterator operator+(size_type n) const {
            return iterator(ptr_ + n);
        }
        constexpr size_type operator-(const iterator& another) const {
            return ptr_ - another.ptr_;
        }
        constexpr iterator operator-(size_type n) const {
            return iterator(ptr_ - n);
        }
        constexpr iterator& operator--() {
            ptr_--;
            return *this;
        }
        constexpr bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }
        constexpr bool operator!=(const iterator& other) const {
            return ptr_ != other.ptr_;
        }
        constexpr const_pointer getPtr() const {
            return ptr_;
        }
    private:
        pointer ptr_;
    };

    void fill(iterator start, iterator end, const_reference value) {
        for (; start != end; ++start) {
            *start = value;
        }
    }
    void copy(iterator start, iterator end, iterator output) {
        for (; start != end; ++start, ++output) {
            *output = *start;
        }
    }
    void copy(iterator start, iterator end, iterator output, size_type max_count) {
        iterator beginning = start;
        for (; (start != end) && (start - beginning < max_count); ++start, ++output) {
            *output = *start;
        }
    }

public:

    using const_iterator  = const iterator;

    //=============CONSTRUCTORS=============//
    constexpr vector() noexcept(noexcept(Allocator()))
        : allocator_   (Allocator()),
          start_value  (nullptr),
          end_value    (nullptr),
          end_capacity (nullptr) {
    }
    constexpr explicit vector(const Allocator& allocator) noexcept
        : allocator_   (allocator),
          start_value  (nullptr),
          end_value    (nullptr),
          end_capacity (nullptr) {
    }
    constexpr vector(size_type count, const_reference value, const Allocator& allocator = Allocator())
        : allocator_   (allocator),
          start_value  (allocator_.allocate(count)),
          end_value    (start_value + count),
          end_capacity (end_value) {
        fill(start_value, end_value, value);
    }
    constexpr explicit vector(size_type count, const Allocator& allocator = Allocator())
        : allocator_   (allocator),
          start_value  (allocator_.allocate(count)),
          end_value    (start_value + count),
          end_capacity (end_value) {
    }
    constexpr vector(const vector& another)
        : allocator_   (another.allocator_),
          start_value  (allocator_.allocate(another.capacity())),
          end_value    (start_value + another.size()),
          end_capacity (start_value + another.capacity()) {
        copy(another.begin(), another.end(), begin());
    }
    constexpr vector(const vector& another, const Allocator& allocator)
        : allocator_   (allocator),
          start_value  (allocator_.allocate(another.capacity())),
          end_value    (start_value + another.size()),
          end_capacity (start_value + another.capacity()) {
        copy(another.begin(), another.end(), begin());
    }
    constexpr vector(vector&& another) noexcept
        : allocator_   (std::move(another.allocator_)),
          start_value  (nullptr),
          end_value    (nullptr),
          end_capacity (nullptr) {
        if (this != &another) {
            start_value  = another.start_value;
            end_value    = another.end_value;
            end_capacity = another.end_capacity;

            another.start_value  = nullptr;
            another.end_value    = nullptr;
            another.end_capacity = nullptr;
        }
    }
    constexpr vector(vector&& another, const Allocator& allocator) 
        : allocator_   (allocator),
          start_value  (nullptr),
          end_value    (nullptr),
          end_capacity (nullptr) {
        if (this != &another) {
            start_value  = allocator_.allocate(another.capacity());
            end_value    = start_value + another.size();
            end_capacity = start_value + another.capacity();

            copy(another.begin(), another.end(), begin());
        }
    }
    ~vector() {
        allocator_.deallocate(start_value.getPtr(), capacity());
    }

    //=============OPERATORS=============//
    constexpr vector& operator=(const vector& another) {
        allocator_   = another.allocator_;
        start_value  = allocator_.allocate(another.capacity());
        end_value    = start_value + another.size();
        end_capacity = start_value + another.capacity();

        copy(another.begin(), another.end(), begin());

        return *this;
    }
    constexpr vector& operator=(vector&& another) noexcept {
        if (this != &another) {
            allocator_   = std::move(another.allocator_);
            start_value  = another.start_value;
            end_value    = another.end_value;
            end_capacity = another.end_capacity;

            another.start_value  = nullptr;
            another.end_value    = nullptr;
            another.end_capacity = nullptr;
        }

        return *this;
    }
    constexpr reference operator[](size_type pos) {
        if (pos >= capacity()) throw std::out_of_range("Position was greater capacity!"); 
        return static_cast<reference>(*(start_value + pos));
    }
    constexpr const_reference operator[](size_type pos) const {
        if (pos >= capacity()) throw std::out_of_range("Position was greater capacity!"); 
        return *(start_value + pos);
    }

    //=============ITERATORS=============//
    constexpr iterator begin() noexcept {
        return start_value;
    }
    constexpr const_iterator begin() const noexcept {
        return start_value;
    }
    constexpr const_iterator cbegin() const noexcept {
        return start_value;
    }
    constexpr iterator end() noexcept {
        return end_value;
    }
    constexpr const_iterator end() const noexcept {
        return end_value;
    }
    constexpr const_iterator cend() const noexcept {
        return end_value;
    }

    //=============ELEMENT ACCESS=============//
    constexpr reference at(size_type pos) {
        return operator[](pos);
    }
    constexpr const_reference at(size_type pos) const {
        return operator[](pos);
    }
    constexpr reference front() {
        if (!size()) throw std::out_of_range("Container is empty!");
        return *start_value;
    }
    constexpr const_reference front() const {
        if (!size()) throw std::out_of_range("Container is empty!");
        return *start_value;
    }
    constexpr reference back() {
        if (!size()) throw std::out_of_range("Container is empty!");
        return *end_value;
    }
    constexpr const_reference back() const {
        if (!size()) throw std::out_of_range("Container is empty!");
        return *end_value;
    }
    constexpr pointer data() noexcept {
        return start_value;
    }
    constexpr const_pointer data() const {
        return start_value;
    }

    //=============CAPACITY ACCESS=============//
    [[nodiscard]] constexpr bool empty() const noexcept {
        return size() == 0;
    }
    constexpr size_type size() const noexcept {
        return static_cast<size_type>(end_value - start_value);
    }
    constexpr size_type max_size() const noexcept {
        return static_cast<size_type>(allocator_.max_size());
    }
    constexpr void reserve(size_type new_capacity) {
        if (new_capacity > max_size()) throw std::length_error("new_capacity > max_size()!");
        if (new_capacity > capacity()) doRealloc(new_capacity);
    }
    constexpr size_type capacity() const noexcept {
        return static_cast<size_type>(end_capacity - start_value);
    }
    constexpr void shrink_to_fit() {
        /*
        https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit
        'It depends on the implementation whether the request is fulfilled.'
        */
    }

    //=============MODIFIERS=============//
private:
    static constexpr double RESIZE_COEFFICIENT = 1.61;
    constexpr void tryRealloc() {
        if (size() >= capacity() - 1) {
            reserve(RESIZE_COEFFICIENT * size());
            return;
        }

        double down_resize_coeff = capacity() / (RESIZE_COEFFICIENT * RESIZE_COEFFICIENT);
        if (size() < down_resize_coeff) {
            doRealloc(capacity() / RESIZE_COEFFICIENT);
        }
    }

    constexpr void doRealloc(size_type new_capacity) {
        iterator temp = iterator(allocator_.allocate(new_capacity));
        copy(begin(), end(), temp, new_capacity);

        allocator_.deallocate(start_value.getPtr(), capacity());

        end_value    = temp + std::min(size(), new_capacity);
        start_value  = temp;
        end_capacity = temp + new_capacity;
    }

public:
    constexpr void clear() noexcept {
        end_value = start_value;
    }
    // TODO
    constexpr iterator insert(const_iterator pos, const_reference value) {
        size_type pos_num = pos - begin();

        tryRealloc();

        copy(begin() + pos_num, end(), begin() + pos_num + 1);
        at(pos_num) = value;

        ++end_value;

        return begin() + pos_num;
    }
    template< class... Args >
    constexpr iterator emplace( const_iterator pos, Args&&... args ) {
        return insert(pos, T(std::forward<Args...>(args...)));
    }
    constexpr iterator erase(const_iterator pos) {
        copy(pos + 1, end(), pos);
        --end_value;
        return pos;
    }
    constexpr iterator erase(const_iterator first, const_iterator last) {
        copy(last, end(), first);
        end_value = end_value - (last - first);
        return end_value;
    }
    constexpr void push_back(const_reference value) {
        insert(end(), value);
    }
    constexpr void push_back(T&& value) {
        insert(end(), std::move(value));
    }
    template< class... Args >
    constexpr reference emplace_back(Args&&... args) {
        emplace(end(), args...); 
        return at(size() - 1);
    }
    constexpr void pop_back() {
        erase(end());
    }
    constexpr void resize(size_type count) {
        doRealloc(count);
    }
    constexpr void resize(size_type count, const_reference value) {
        if (size() == count) return;

        doRealloc(count);

        if (size() < count) {
            fill(end(), begin() + capacity(), value);
            end_value = end_capacity;
        }
    }

private:
    Allocator allocator_;
    iterator  start_value;
    iterator  end_value;
    iterator  end_capacity;
};

}