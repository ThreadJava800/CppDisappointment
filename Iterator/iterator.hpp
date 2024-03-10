#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <iostream>
#include <type_traits>

using ptrdiff_t = long;

struct input_iterator_tag                                             {};
struct output_iterator_tag                                            {};
struct forward_iterator_tag       : public input_iterator_tag         {};
struct bidirectional_iterator_tag : public forward_iterator_tag       {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag    : public random_access_iterator_tag {};

template<class Iter>
struct iterator_traits {
    using iterator_category = Iter::iterator_category;
    using value_type        = Iter::value_type;
    using difference_type   = Iter::difference_type;
    using pointer           = Iter::pointer;
    using reference         = Iter::reference;
};

template<typename T>
requires std::is_object_v<T>
struct iterator_traits<T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
};

template<
        class Category,
        typename T,
        typename Distance  = ptrdiff_t,
        typename Pointer   = T*,
        typename Reference = T&
        > 
struct iterator {
    using iterator_category = Category;
    using value_type        = T;
    using difference_type   = Distance;
    using pointer           = Pointer;
    using reference         = Reference;
};

template<typename T>
struct output_iterator : public iterator<
                                        output_iterator_tag,
                                        T,
                                        ptrdiff_t,
                                        T*,
                                        T&
                                        > {};

template<typename T>
struct input_iterator : public iterator<
                                        input_iterator_tag,
                                        T,
                                        ptrdiff_t,
                                        T*,
                                        T&
                                        > {};

template<class STL_Container, typename T>
class push_back_iterator : public output_iterator<STL_Container> {
public:

    using iterator_category = output_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    explicit push_back_iterator(STL_Container& _container)
        : container(&_container) {}

    push_back_iterator<STL_Container, T>& operator*() {
        return *this;
    }

    push_back_iterator<STL_Container, T>& operator++() { 
        return *this; 
    } 

    push_back_iterator<STL_Container, T>& operator=(const T& value) {
        if (container) container->push_back(value);

        return *this;
    }

private:
    STL_Container* container;
};

template<typename T>
class istream_iterator : public input_iterator<T> {
public:

    using iterator_category = input_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    explicit istream_iterator()
        : stream          (nullptr),
          is_proceed_input(true) {}

    explicit istream_iterator(std::istream& _stream)
        : stream          (&_stream),
          is_proceed_input(true) {}

    const T& operator*() const {
        return input_value;
    }

    istream_iterator& operator=(istream_iterator&) {
        return *this;
    }

    istream_iterator& operator++() {
        if (stream) *stream >> input_value;
        is_proceed_input = !!(*stream);

        return *this;
    }

    bool operator==(const istream_iterator& other) {
        return stream == other.stream || !is_proceed_input;
    }

private:
    std::istream* stream;
    bool          is_proceed_input;
    T             input_value;
};

template<typename T>
class ostream_iterator : public output_iterator<T> {
public:

    using iterator_category = output_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    explicit ostream_iterator(std::ostream& _stream, const char* _stream_value = nullptr)
        : stream      (&_stream),
          stream_value(_stream_value) {}

    ostream_iterator<T>& operator=(const T& _value) {
        if (stream_value) *stream << stream_value;
        *stream << _value;

        return *this;
    }

    ostream_iterator<T>& operator*() {
        return *this;
    }

    ostream_iterator<T>& operator++() { 
        return *this; 
    } 

private:
    std::ostream* stream;
    const char*   stream_value;
};

#endif