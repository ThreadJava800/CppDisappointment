#pragma once

template<class E>
class unexpected {
public:

    constexpr unexpected(const unexpected&) = default;
    constexpr unexpected(unexpected&&)      = default;

    constexpr explicit unexpected(E&& _error) 
      :  error_ (std::move(_error))
    {}
    
    ~unexpected() = default;

    constexpr const E& error() const& noexcept {
        return error_;
    }

private:
    E error_;
};

enum class expected_type {
    VALUE,
    ERROR
};

template<class T, class E>
class expected {
public:
    explicit expected() {}

    explicit expected(T&& _value) 
      :  value_ (std::move(_value)),
         type_  (expected_type::VALUE)
    {}

    explicit expected(unexpected<E> const& _error) 
      :  error_ (std::move(_error.error())),
         type_  (expected_type::ERROR)
    {}

    explicit expected(unexpected<E>&& _error) 
      :  error_ (std::move(_error.error())),
         type_  (expected_type::ERROR)
    {}

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

    union {
      T value_;
      E error_;
    };
};