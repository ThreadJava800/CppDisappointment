#pragma once

// template<class T, class E>
// union expected_union {
//     T value;
//     E error;

//      expected_union() {}
//     ~expected_union() {}

//     expected_union& operator=(const expected_union& other) {
        
        
//         return *this;
//     }
// };

enum class expected_type {
    VALUE,
    ERROR
};

template<class T, class E>
class expected {
public:
    explicit expected() {}

    explicit expected(T _value) {
        type_ = expected_type::VALUE;
        value_ = _value;
    }

    explicit expected(E _error) {
        type_ = expected_type::ERROR;
        error_ = _error;
    }

    bool has_value() const noexcept {
        return type_ == expected_type::VALUE;
    };

    T& value() const noexcept {
        return value_;
    }

    E& error() const noexcept {
        return error_;
    }

private:
    expected_type type_;
    T value_;
    E error_;
};