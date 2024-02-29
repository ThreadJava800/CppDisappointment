#ifndef _SHARED_PTR_H_
#define _SHARED_PTR_H_

#include <atomic>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <type_traits>

#define IS_THREAD_SAFE 1


template<typename T, class Deleter>
class ControlBlock {
public:
    ControlBlock(T* _value)
        : value(_value), 
          shared_ptr_counter(1),
          deleter(Deleter()) {
    }

    ControlBlock(T* _value, Deleter _deleter) 
        : value(_value), 
          shared_ptr_counter(1),
          deleter(_deleter) {
    }

    size_t getSharedCount() const noexcept {
    #ifndef IS_THREAD_SAFE
        return shared_ptr_counter;
    #else
        return shared_ptr_counter.load();
    #endif
    }

    void addSharedPtr() noexcept {
        shared_ptr_counter++;
    }

    void removeSharedPtr() noexcept {
        shared_ptr_counter--;
    }

    virtual T* getValuePtr() const {
        return value;
    }

    size_t getUseCount() const noexcept {
        return shared_ptr_counter;
    }

    virtual ~ControlBlock() {
        deleter(value);
    }

private:
    T*      value;
    Deleter deleter;

#ifndef IS_THREAD_SAFE
    size_t  shared_ptr_counter;
#else
    std::atomic<uint32_t> shared_ptr_counter{1};
#endif
};

template<typename T>
struct DefaultDeleter {
    void operator()(T* value) {
    }
};

template<typename T, class Deleter = DefaultDeleter<T>>
struct DefaultAllocator {
    ControlBlock<T, Deleter>* operator()(T* value, Deleter deleter = DefaultDeleter<T>()) {
        return new ControlBlock<T, Deleter>(value, deleter);
    }
};

template<typename T>
class SharedControlBlock : public ControlBlock<T, DefaultDeleter<T>> {
public:
    explicit SharedControlBlock(T _value)
        : ControlBlock<T, DefaultDeleter<T>>(nullptr),
          value(_value) {
    }

    T* getValuePtr() const override {
        return const_cast<T*>(&value);
    }

private:
    T value;
};

template<typename T>
struct SharedAllocator {
    ControlBlock<T, DefaultDeleter<T>>* operator()(T value) {
        return new SharedControlBlock<T>(value);
    }
};

template<typename T, class Deleter = DefaultDeleter<T>, class Allocator = DefaultAllocator<T, Deleter>>
class SharedPtr {
private:
    explicit SharedPtr(T _value) 
        : control_block(Allocator()(_value)) {
    }

public:
    template<typename U>
    friend SharedPtr<U, DefaultDeleter<U>, SharedAllocator<U>> makeShared(U value);

    explicit SharedPtr(std::nullptr_t _value = nullptr)
        : control_block(Allocator()(_value)) {
    }

    explicit SharedPtr(T* _value) 
        : control_block(Allocator()(_value)) {
    }

    explicit SharedPtr(T* _value, Deleter _deleter) 
        : control_block(Allocator()(_value, _deleter)) {
    }

    SharedPtr(const SharedPtr& other) noexcept
        : control_block(other.control_block) {
        if (control_block) 
            control_block->addSharedPtr();
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (control_block != other.control_block)
            delete control_block;

        control_block = other.control_block;
        if (control_block) 
            control_block->addSharedPtr(); 

        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : control_block(other.control_block) {
        other.control_block = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            delete control_block;

            control_block = other.control_block;
            other.control_block = nullptr;
        }

        return *this;
    }

    ~SharedPtr() {
        if (!control_block) return;

        control_block->removeSharedPtr();

        if (control_block->getSharedCount() == 0)
            delete control_block;
    }

    T& operator*() const {
        if (!control_block) throw std::invalid_argument("Control block ptr was NULL!");
        return *(control_block->getValuePtr());
    }

    T* operator->() const noexcept {
        if (!control_block) return nullptr;
        return control_block->getValuePtr();
    }

    T& operator[](size_t index) const {
        if (!control_block) throw std::invalid_argument("Control block ptr was NULL!");
        return (control_block->getValuePtr())[index];
    }

    operator bool() const noexcept {
        if (!control_block) return false;
        return control_block->getValuePtr();
    }

    T* get() const noexcept {
        if (!control_block) return nullptr;
        return control_block->getValuePtr();
    }

    size_t useCount() const {
        if (!control_block) throw std::invalid_argument("Control block ptr was NULL!");
        return control_block->getSharedCount();
    }

private:
    ControlBlock<T, Deleter>* control_block;
};

template<typename U>
static SharedPtr<U, DefaultDeleter<U>, SharedAllocator<U>> makeShared(U value) {
    return SharedPtr<U, DefaultDeleter<U>, SharedAllocator<U>>(value);
}

#endif