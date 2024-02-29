#ifndef _SHARED_PTR_H_
#define _SHARED_PTR_H_

#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <type_traits>


template<typename T>
struct DefaultDeleter {
    void operator()(T* value) {
        delete value;
    }
};

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
        return shared_ptr_counter;
    }

    void addSharedPtr() noexcept {
        shared_ptr_counter++;
    }

    void removeSharedPtr() noexcept {
        shared_ptr_counter--;
    }

    T* getValuePtr() const noexcept {
        return value;
    }

    size_t getUseCount() const noexcept {
        return shared_ptr_counter;
    }

    ~ControlBlock() {
        deleter(value);
    }

private:
    T*      value;
    size_t  shared_ptr_counter;
    Deleter deleter;
};

template<typename T, class Deleter = DefaultDeleter<T>>
class SharedPtr {
public:
    explicit SharedPtr(std::nullptr_t _value = nullptr)
        : control_block(new ControlBlock<T, Deleter>(_value)) {
    }

    explicit SharedPtr(T* _value) 
        : control_block(new ControlBlock<T, Deleter>(_value)) {
    }

    explicit SharedPtr(T* _value, Deleter _deleter) 
        : control_block(new ControlBlock<T, Deleter>(_value, _deleter)) {
    }

    SharedPtr(const SharedPtr& other) noexcept
        : control_block(other.control_block) {
        if (control_block) 
            control_block->addSharedPtr();
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        control_block = other.control_block;
        if (control_block) 
            control_block->addSharedPtr(); 

        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : control_block(std::move(other.control_block)) {
        if (control_block) 
            control_block->addSharedPtr(); 
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        control_block = std::move(other.control_block);
        if (control_block) 
            control_block->addSharedPtr(); 

        return *this;
    }

    ~SharedPtr() {
        if (control_block && control_block->getSharedCount() == 0)
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

#endif