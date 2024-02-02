#ifndef _MINT_H_
#define _MINT_H_

#include <stdio.h>

static const int DEFAULT_INT_VALUE = 0;

#define ASSIGNMENT_OPER(oper)                          \
void operator oper(const MInt& val2) {                 \
    value oper val2.value;                             \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
}                                                      \


#define ARITHMETIC_OPER(oper)                          \
MInt operator oper(const MInt& val2) {                 \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    return MInt(value oper val2.value);                \
}                                                      \


#define UNARY_OPER(oper)                               \
MInt operator oper() {                                 \
    value = oper value;                                \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    return *this;                                      \
}                                                      \


#define COMPARISON_OPER(oper)                          \
bool operator oper(const MInt& val2) {                 \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    return value oper val2.value;                      \
}                                                      \


#define PREFIX_OPER(oper)                              \
MInt operator oper() {                                 \
    oper value;                                        \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    return *this;                                      \
}                                                      \


#define POSTFIX_OPER(oper)                             \
MInt operator oper(int) {                              \
    MInt ret_value = *this;                            \
    value oper;                                        \
                                                       \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    return ret_value;                                  \
}                                                      \

class MInt {
public:
    int value = DEFAULT_INT_VALUE;

public:
    explicit MInt(int _value) : value(_value) {
        printf("CTOR: value = %d\n", value);
    }

    ~MInt() {
        printf("DTOR: value = %d\n", value);
    }

    ASSIGNMENT_OPER(=)
    ASSIGNMENT_OPER(+=)
    ASSIGNMENT_OPER(-=)
    ASSIGNMENT_OPER(/=)
    ASSIGNMENT_OPER(*=)
    ASSIGNMENT_OPER(%=)
    ASSIGNMENT_OPER(&=)
    ASSIGNMENT_OPER(|=)
    ASSIGNMENT_OPER(^=)
    ASSIGNMENT_OPER(<<=)
    ASSIGNMENT_OPER(>>=)

    PREFIX_OPER(++)
    PREFIX_OPER(--)
    POSTFIX_OPER(++)
    POSTFIX_OPER(--)

    ARITHMETIC_OPER(+)
    ARITHMETIC_OPER(-)
    ARITHMETIC_OPER(*)
    ARITHMETIC_OPER(/)
    ARITHMETIC_OPER(%)
    ARITHMETIC_OPER(&)
    ARITHMETIC_OPER(|)
    ARITHMETIC_OPER(^)
    ARITHMETIC_OPER(<<)
    ARITHMETIC_OPER(>>)

    UNARY_OPER(+)
    UNARY_OPER(-)
    UNARY_OPER(~)

    COMPARISON_OPER(==)
    COMPARISON_OPER(!=)
    COMPARISON_OPER(<)
    COMPARISON_OPER(>)
    COMPARISON_OPER(<=)
    COMPARISON_OPER(>=)

    // logical operations
    bool operator!() {
        value = !value;
        return value;
    }

    ARITHMETIC_OPER(&&)
    ARITHMETIC_OPER(||)
};

#endif