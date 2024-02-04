#ifndef _MINT_H_
#define _MINT_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const int   MAX_NAME_LENGTH    = 128;
static const char* GRAPHVIZ_FILE_NAME = "graph.dot";
static const int   DEFAULT_INT_VALUE  = 0;

FILE* DOT_FILE      = nullptr;
int   VARIABLES_CNT = 0;

#define ASSIGNMENT_OPER(oper)                          \
void operator oper(const MInt& val2) {                 \
    value oper val2.value;                             \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    drawArrow(val2.id, id, #oper);                     \
}                                                      \


#define ARITHMETIC_OPER(oper)                          \
MInt operator oper(const MInt& val2) {                 \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    MInt ret_value = MInt(value oper val2.value);      \
    drawArrow(id, ret_value.id, #oper);                \
    drawArrow(val2.id, ret_value.id, #oper);           \
    return ret_value;                                  \
}                                                      \


#define UNARY_OPER(oper)                               \
MInt operator oper() {                                 \
    value = oper value;                                \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    drawArrow(id, id, #oper);                          \
    return *this;                                      \
}                                                      \


#define COMPARISON_OPER(oper)                          \
bool operator oper(const MInt& val2) {                 \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    drawArrow(id, val2.id, #oper);                     \
    return value oper val2.value;                      \
}                                                      \


#define PREFIX_OPER(oper)                              \
MInt operator oper() {                                 \
    oper value;                                        \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    drawArrow(id, id, #oper);                          \
    return *this;                                      \
}                                                      \


#define POSTFIX_OPER(oper)                             \
MInt operator oper(int) {                              \
    MInt ret_value = *this;                            \
    value oper;                                        \
                                                       \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);  \
    drawArrow(id, id, #oper);                          \
    return ret_value;                                  \
}   

void mprintf(FILE *file, const char *fmt...) {
    if (!file) return;

    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
}

void drawArrow(int from_id, int to_id, const char* text) {
    mprintf(
        DOT_FILE, 
        "\tlabel%ld->label%ld [color=\"black\", style=\"dashed\",arrowhead=\"none\",label=\"%s\"]", 
        from_id, 
        to_id,
        text
    );
}

void drawBlock(int val_id, int value) {
    mprintf(
        DOT_FILE, 
        "\tlabel%ld[shape=record, style=\"rounded, filled\", fillcolor=\"red\", label=\"{id: %ld | value: %ld}\"];\n", 
        val_id, 
        abs(val_id),
        value
    );
}


class MInt {
public:
    int value = DEFAULT_INT_VALUE;
    int id    = 0;

public:
    explicit MInt(int _value) : value(_value), id(++VARIABLES_CNT) {
        printf("CTOR: id = %d, value = %d, addr = %p\n", id, value, this);
        drawBlock(id - 1, value);
        drawBlock(id    , value);
        drawArrow(id - 1, id, "CTOR");

        VARIABLES_CNT++;
    }

    MInt(const MInt& another) : value(another.value), id(++VARIABLES_CNT) {
        printf("COPY CTOR: id = %d, value = %d, addr = %p\n", id, value, this);
        drawBlock(id    , value);
        drawArrow(another.id, id, "COPY CTOR");

        VARIABLES_CNT++;
    }

    ~MInt() {
        printf("DTOR: id = %d, value = %d, addr = %p\n", id, value, this);
        value = DEFAULT_INT_VALUE;

        drawBlock(-id, value);
        drawArrow(id, -id, "DTOR");
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