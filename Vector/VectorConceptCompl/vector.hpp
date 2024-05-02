#pragma once

#include <cstdio>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <type_traits>

#include "../../Allocator/allocator.hpp"
#include "../../Iterator/iterator.hpp"

namespace slow_vec {

template<typename T, class InputIterator>
void fill(InputIterator start, InputIterator end, T& value) {
    for (; start != end; ++start) {
        *start = value;
    }
}

template<class InputIterator, class OutputIterator, typename size_type>
void copy(InputIterator start, InputIterator end, OutputIterator output, size_type max_count)
    requires(sizeof(typename OutputIterator::value_type) <= sizeof(long long int) || std::is_trivially_copy_constructible_v<typename OutputIterator::value_type>)  {
    std::cout << "EFFECTIVE\n";
    std::memcpy(&*output, &*start, std::min(end - start, max_count));
}
template<class InputIterator, class OutputIterator, typename size_type>
void copy(InputIterator start, InputIterator end, OutputIterator output, size_type max_count) {
    InputIterator beginning = start;
    std::cout << "NOT EFFECTIVE\n";
    for (; (start != end) && (start - beginning < max_count); ++start, ++output) {
        *output = *start;
    }
}

template<class InputIterator, class OutputIterator>
void copy(InputIterator start, InputIterator end, OutputIterator output)
    requires(sizeof(typename OutputIterator::value_type) <= sizeof(long long int) || std::is_trivially_copy_constructible_v<typename OutputIterator::value_type>) {
    std::cout << "EFFECTIVE\n";
    std::memcpy(&*output, &*start, end - start);
}
template<class InputIterator, class OutputIterator>
void copy(InputIterator start, InputIterator end, OutputIterator output) {
    std::cout << "NOT EFFECTIVE\n";
    for (; start != end; ++start, ++output) {
        *output = *start;
    }
}

template<class InputIterator, class OutputIterator>
void copyBackwards(InputIterator start, InputIterator end, OutputIterator output) {
    for (; start != end; --start, --output) {
        *output = *start;
    }
}

template<typename T>
class initializer_list {
public:

    using value_type      = T;
    using reference       = const T&;
    using const_reference = const T&;
    using size_type       = std::size_t;
    using iterator        = const T*;
    using const_iterator  = const T*;

    constexpr initializer_list() noexcept
        : args_start (nullptr),
          args_end   (nullptr) {
    }
    constexpr const_iterator begin() const noexcept {
        return args_start;
    }
    constexpr const_iterator end() const noexcept {
        return args_end;
    }
    constexpr size_type size() const noexcept {
        return static_cast<size_type>(args_end - args_start);
    }

private:
    constexpr initializer_list(const_iterator iterator, size_type size)
        : args_start (iterator),
          args_end   (iterator + size) { 
    }

    initializer_list::iterator args_start;
    initializer_list::iterator args_end;
};

template<
        typename T,
        class Allocator = allocator<T>
        >
class vector {
private:
    static constexpr const char* ITERATOR_ERROR         = "vector_iterator ptr is NULL!";
    static constexpr const char* OUT_OF_BOUNDS_ERROR    = "Position was greater capacity!";
    static constexpr const char* CONTAINER_IS_EMPTY_ERR = "Container is empty!";
    static constexpr const char* OUT_OF_MEMORY          = "new_capacity > max_size()!";

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
    class vector_iterator : public input_iterator<T> {
    public:
        constexpr explicit vector_iterator(pointer ptr)
            : ptr_(ptr) {
        }
        constexpr reference operator*() const {
            if (!ptr_) throw std::invalid_argument(ITERATOR_ERROR);
            return *ptr_;
        }
        constexpr vector_iterator& operator++() {
            ptr_++;
            return *this;
        }
        constexpr vector_iterator operator+(size_type n) const {
            return vector_iterator(ptr_ + n);
        }
        constexpr size_type operator-(const vector_iterator& another) const {
            return ptr_ - another.ptr_;
        }
        constexpr vector_iterator operator-(size_type n) const {
            return vector_iterator(ptr_ - n);
        }
        constexpr vector_iterator& operator--() {
            ptr_--;
            return *this;
        }
        constexpr bool operator==(const vector_iterator& other) const {
            return ptr_ == other.ptr_;
        }
        constexpr bool operator!=(const vector_iterator& other) const {
            return ptr_ != other.ptr_;
        }
        constexpr auto operator<=>(const vector_iterator& rhs) const { 
            return *ptr_ <=> *rhs.ptr_; 
        }
        constexpr const_pointer getPtr() const {
            return ptr_;
        }
    protected:
        pointer ptr_;
    };

public:

    using const_iterator  = const vector_iterator;

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
    constexpr vector(input_iterator<T> first, input_iterator<T> last, const Allocator& allocator = Allocator())
        : allocator_   (allocator),
          start_value  (allocator_.allocate(last - first)),
          end_value    (start_value + (last - first)),
          end_capacity (end_value) {
        copy(first, last, start_value);
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
    constexpr vector(std::initializer_list<T> init_list, const Allocator& allocator = Allocator())
        : allocator_   (allocator),
          start_value  (allocator_.allocate(init_list.size())),
          end_value    (start_value + init_list.size()),
          end_capacity (end_value) {
        copy(init_list.begin(), init_list.end(), start_value);
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
    constexpr vector& operator=(std::initializer_list<T> init_list) {
        resize(init_list.size());
        copy(init_list.begin(), init_list.end(), start_value);
    }
    constexpr reference operator[](size_type pos) {
        if (pos >= capacity()) throw std::out_of_range(OUT_OF_BOUNDS_ERROR); 
        return static_cast<reference>(*(start_value + pos));
    }
    constexpr const_reference operator[](size_type pos) const {
        if (pos >= capacity()) throw std::out_of_range(OUT_OF_BOUNDS_ERROR); 
        return *(start_value + pos);
    }

    //=============ITERATORS=============//
    constexpr vector_iterator begin() noexcept {
        return start_value;
    }
    constexpr const_iterator begin() const noexcept {
        return start_value;
    }
    constexpr const_iterator cbegin() const noexcept {
        return static_cast<const_iterator>(start_value);
    }
    constexpr vector_iterator end() noexcept {
        return end_value;
    }
    constexpr const_iterator end() const noexcept {
        return end_value;
    }
    constexpr const_iterator cend() const noexcept {
        return static_cast<const_iterator>(end_value);
    }

    //=============ELEMENT ACCESS=============//
    constexpr reference at(size_type pos) {
        return operator[](pos);
    }
    constexpr const_reference at(size_type pos) const {
        return operator[](pos);
    }
    constexpr reference front() {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
        return *start_value;
    }
    constexpr const_reference front() const {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
        return *start_value;
    }
    constexpr reference back() {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
        return *end_value;
    }
    constexpr const_reference back() const {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
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
        if (new_capacity > max_size()) throw std::length_error(OUT_OF_MEMORY);
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
            reserve(RESIZE_COEFFICIENT * capacity());
        }
    }

    constexpr void doRealloc(size_type new_capacity) {
        vector_iterator temp = vector_iterator(allocator_.allocate(new_capacity));
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
    constexpr vector_iterator insert(const_iterator pos, const_reference value) {
        size_type pos_num = pos - begin();

        tryRealloc();

        copyBackwards(end() - 1, begin() + pos_num - 1, end());
        at(pos_num) = value;

        ++end_value;

        return begin() + pos_num;
    }
    constexpr vector_iterator insert(const_iterator pos, size_type count, const_reference value) {
        for (size_type i = 0; i < count; ++i) {
            insert(pos, value);
        }
        return pos + count;
    }
    constexpr vector_iterator insert(const_iterator pos, vector_iterator first, vector_iterator last) {
        --last;--first;
        size_type pos_num = pos - begin();

        for (; last != first; --last) {
            insert(begin() + pos_num, *last);
        }
        return pos + (last - first);
    }
    constexpr vector_iterator insert(const_iterator pos, std::initializer_list<T> init_list) {
        auto first = init_list.begin() - 1;
        auto last  = init_list.end() - 1;

        size_type pos_num = pos - begin();

        for (; last != first; --last) {
            insert(begin() + pos_num, *last);
        }
        return pos + init_list.size();
    }
    template< class... Args >
    constexpr vector_iterator emplace(const_iterator pos, Args&&... args) {
        return insert(pos, T(std::forward<Args...>(args...)));
    }
    constexpr vector_iterator erase(const_iterator pos) {
        copy(pos + 1, end(), pos);
        --end_value;
        return pos;
    }
    constexpr vector_iterator erase(const_iterator first, const_iterator last) {
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
    constexpr void swap(vector& another) noexcept {
        std::swap(allocator_,   another.allocator_);
        std::swap(start_value,  another.start_value);
        std::swap(end_value,    another.end_value);
        std::swap(end_capacity, another.end_capacity);
    }

    //=============UTILITY=============//
    constexpr allocator_type get_allocator() const noexcept {
        return allocator_;
    }
    constexpr void assign(size_type count, const_reference value) {
        resize(count);
        for (size_type i = 0; i < count; ++i) {
            at(i) = value;
        }
    }
    constexpr void assign(input_iterator<T> first, input_iterator<T> last) {
        resize(last - first);
        copy(first, last, start_value);
    }
    constexpr void assign(std::initializer_list<T> init_list) {
        resize(init_list.size());
        copy(init_list.begin(), init_list.end(), start_value);
    }

private:
    Allocator allocator_;
    vector_iterator  start_value;
    vector_iterator  end_value;
    vector_iterator  end_capacity;
};

}

namespace fast_vec {

template<typename T, class InputIterator>
void fill(InputIterator start, InputIterator end, T& value) {
    for (; start != end; ++start) {
        *start = value;
    }
}

template<class InputIterator, class OutputIterator, typename size_type>
void copy(InputIterator start, InputIterator end, OutputIterator output, size_type max_count)
    requires(sizeof(typename OutputIterator::value_type) <= sizeof(long long int) || std::is_trivially_copy_constructible_v<typename OutputIterator::value_type>)  {
    std::cout << "EFFECTIVE\n";
    std::memcpy(&*output, &*start, std::min(end - start, max_count) * sizeof(typename OutputIterator::value_type));
}

template<class InputIterator, class OutputIterator>
void copy(InputIterator start, InputIterator end, OutputIterator output)
    requires(sizeof(typename OutputIterator::value_type) <= sizeof(long long int) || std::is_trivially_copy_constructible_v<typename OutputIterator::value_type>) {
    std::cout << "EFFECTIVE\n";
    std::memcpy(&*output, &*start, (end - start) * sizeof(typename OutputIterator::value_type));
}

template<class InputIterator, class OutputIterator>
void copyBackwards(InputIterator start, InputIterator end, OutputIterator output) {
    for (; start != end; --start, --output) {
        *output = *start;
    }
}

template<typename T>
class initializer_list {
public:

    using value_type      = T;
    using reference       = const T&;
    using const_reference = const T&;
    using size_type       = std::size_t;
    using iterator        = const T*;
    using const_iterator  = const T*;

    constexpr initializer_list() noexcept
        : args_start (nullptr),
          args_end   (nullptr) {
    }
    constexpr const_iterator begin() const noexcept {
        return args_start;
    }
    constexpr const_iterator end() const noexcept {
        return args_end;
    }
    constexpr size_type size() const noexcept {
        return static_cast<size_type>(args_end - args_start);
    }

private:
    constexpr initializer_list(const_iterator iterator, size_type size)
        : args_start (iterator),
          args_end   (iterator + size) { 
    }

    initializer_list::iterator args_start;
    initializer_list::iterator args_end;
};

template<
        typename T,
        class Allocator = allocator<T>
        >
class vector {
private:
    static constexpr const char* ITERATOR_ERROR         = "vector_iterator ptr is NULL!";
    static constexpr const char* OUT_OF_BOUNDS_ERROR    = "Position was greater capacity!";
    static constexpr const char* CONTAINER_IS_EMPTY_ERR = "Container is empty!";
    static constexpr const char* OUT_OF_MEMORY          = "new_capacity > max_size()!";

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
    class vector_iterator : public input_iterator<T> {
    public:
        constexpr explicit vector_iterator(pointer ptr)
            : ptr_(ptr) {
        }
        constexpr reference operator*() const {
            if (!ptr_) throw std::invalid_argument(ITERATOR_ERROR);
            return *ptr_;
        }
        constexpr vector_iterator& operator++() {
            ptr_++;
            return *this;
        }
        constexpr vector_iterator operator+(size_type n) const {
            return vector_iterator(ptr_ + n);
        }
        constexpr size_type operator-(const vector_iterator& another) const {
            return ptr_ - another.ptr_;
        }
        constexpr vector_iterator operator-(size_type n) const {
            return vector_iterator(ptr_ - n);
        }
        constexpr vector_iterator& operator--() {
            ptr_--;
            return *this;
        }
        constexpr bool operator==(const vector_iterator& other) const {
            return ptr_ == other.ptr_;
        }
        constexpr bool operator!=(const vector_iterator& other) const {
            return ptr_ != other.ptr_;
        }
        constexpr auto operator<=>(const vector_iterator& rhs) const { 
            return *ptr_ <=> *rhs.ptr_; 
        }
        constexpr const_pointer getPtr() const {
            return ptr_;
        }
    protected:
        pointer ptr_;
    };

public:

    using const_iterator  = const vector_iterator;

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
    constexpr vector(input_iterator<T> first, input_iterator<T> last, const Allocator& allocator = Allocator())
        : allocator_   (allocator),
          start_value  (allocator_.allocate(last - first)),
          end_value    (start_value + (last - first)),
          end_capacity (end_value) {
        copy(first, last, start_value);
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
    constexpr vector(std::initializer_list<T> init_list, const Allocator& allocator = Allocator())
        : allocator_   (allocator),
          start_value  (allocator_.allocate(init_list.size())),
          end_value    (start_value + init_list.size()),
          end_capacity (end_value) {
        copy(init_list.begin(), init_list.end(), start_value);
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
    constexpr vector& operator=(std::initializer_list<T> init_list) {
        resize(init_list.size());
        copy(init_list.begin(), init_list.end(), start_value);
    }
    constexpr reference operator[](size_type pos) {
        if (pos >= capacity()) throw std::out_of_range(OUT_OF_BOUNDS_ERROR); 
        return static_cast<reference>(*(start_value + pos));
    }
    constexpr const_reference operator[](size_type pos) const {
        if (pos >= capacity()) throw std::out_of_range(OUT_OF_BOUNDS_ERROR); 
        return *(start_value + pos);
    }

    //=============ITERATORS=============//
    constexpr vector_iterator begin() noexcept {
        return start_value;
    }
    constexpr const_iterator begin() const noexcept {
        return start_value;
    }
    constexpr const_iterator cbegin() const noexcept {
        return static_cast<const_iterator>(start_value);
    }
    constexpr vector_iterator end() noexcept {
        return end_value;
    }
    constexpr const_iterator end() const noexcept {
        return end_value;
    }
    constexpr const_iterator cend() const noexcept {
        return static_cast<const_iterator>(end_value);
    }

    //=============ELEMENT ACCESS=============//
    constexpr reference at(size_type pos) {
        return operator[](pos);
    }
    constexpr const_reference at(size_type pos) const {
        return operator[](pos);
    }
    constexpr reference front() {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
        return *start_value;
    }
    constexpr const_reference front() const {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
        return *start_value;
    }
    constexpr reference back() {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
        return *end_value;
    }
    constexpr const_reference back() const {
        if (!size()) throw std::out_of_range(CONTAINER_IS_EMPTY_ERR);
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
        if (new_capacity > max_size()) throw std::length_error(OUT_OF_MEMORY);
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
            reserve(RESIZE_COEFFICIENT * capacity());
        }
    }

    constexpr void doRealloc(size_type new_capacity) {
        vector_iterator temp = vector_iterator(allocator_.allocate(new_capacity));
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
    constexpr vector_iterator insert(const_iterator pos, const_reference value) {
        size_type pos_num = pos - begin();

        tryRealloc();

        copyBackwards(end() - 1, begin() + pos_num - 1, end());
        at(pos_num) = value;

        ++end_value;

        return begin() + pos_num;
    }
    constexpr vector_iterator insert(const_iterator pos, size_type count, const_reference value) {
        for (size_type i = 0; i < count; ++i) {
            insert(pos, value);
        }
        return pos + count;
    }
    constexpr vector_iterator insert(const_iterator pos, vector_iterator first, vector_iterator last) {
        --last;--first;
        size_type pos_num = pos - begin();

        for (; last != first; --last) {
            insert(begin() + pos_num, *last);
        }
        return pos + (last - first);
    }
    constexpr vector_iterator insert(const_iterator pos, std::initializer_list<T> init_list) {
        auto first = init_list.begin() - 1;
        auto last  = init_list.end() - 1;

        size_type pos_num = pos - begin();

        for (; last != first; --last) {
            insert(begin() + pos_num, *last);
        }
        return pos + init_list.size();
    }
    template< class... Args >
    constexpr vector_iterator emplace(const_iterator pos, Args&&... args) {
        return insert(pos, T(std::forward<Args...>(args...)));
    }
    constexpr vector_iterator erase(const_iterator pos) {
        copy(pos + 1, end(), pos);
        --end_value;
        return pos;
    }
    constexpr vector_iterator erase(const_iterator first, const_iterator last) {
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
    constexpr void swap(vector& another) noexcept {
        std::swap(allocator_,   another.allocator_);
        std::swap(start_value,  another.start_value);
        std::swap(end_value,    another.end_value);
        std::swap(end_capacity, another.end_capacity);
    }

    //=============UTILITY=============//
    constexpr allocator_type get_allocator() const noexcept {
        return allocator_;
    }
    constexpr void assign(size_type count, const_reference value) {
        resize(count);
        for (size_type i = 0; i < count; ++i) {
            at(i) = value;
        }
    }
    constexpr void assign(input_iterator<T> first, input_iterator<T> last) {
        resize(last - first);
        copy(first, last, start_value);
    }
    constexpr void assign(std::initializer_list<T> init_list) {
        resize(init_list.size());
        copy(init_list.begin(), init_list.end(), start_value);
    }

private:
    Allocator allocator_;
    vector_iterator  start_value;
    vector_iterator  end_value;
    vector_iterator  end_capacity;
};

}