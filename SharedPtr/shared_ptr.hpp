#ifndef _SHARED_PTR_H_
#define _SHARED_PTR_H_

#include <cstdlib>
#include <functional>
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

    size_t getSharedCount() {
        return shared_ptr_counter;
    }

    void addSharedPtr() {
        shared_ptr_counter++;
    }

    void removeSharedPtr() {
        shared_ptr_counter--;
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

    SharedPtr(const SharedPtr& other) {
        control_block = other.control_block;
        if (control_block) 
            control_block->addSharedPtr();
    }

    SharedPtr& operator=(const SharedPtr& other) {
        control_block = other.control_block;
        if (control_block) 
            control_block->addSharedPtr(); 

        return *this;
    }

    ~SharedPtr() {
        if (control_block && control_block->getSharedCount() == 0)
            delete control_block;
    }

private:
    ControlBlock<T, Deleter>* control_block;
};

#endif