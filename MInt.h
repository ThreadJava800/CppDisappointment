#ifndef _MINT_H_
#define _MINT_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static const char* VALUE_PREFIX  = "value";
static const char*  OPER_PREFIX  = "oper";
static const char* DEFAULT_COLOR = "red";

static const int   MAX_INFO_LEN  = 1024;

static const int   MAX_NAME_LENGTH    = 128;
static const char* GRAPHVIZ_FILE_NAME = "graph.dot";
static const int   DEFAULT_INT_VALUE  = 0;
static const char* GRAPHVIZ_COLORS[]  = {"red", "green", "blue", 
                                         "yellow", "orange", "turquoise", 
                                         "sienna", "white", "cornflowerblue", 
                                         "chocolate4", "blueviolet", "darkgoldenrod1", 
                                         "darkblue", "darkmagenta"};
       const int   COLOR_CNT          = sizeof(GRAPHVIZ_COLORS) / sizeof(const char*);

FILE* DOT_FILE      = nullptr;
int   VARIABLES_CNT = 0;
int   OPER_CNT      = 0;
char* INFO_STR      = nullptr;
int   INFO_STR_POS  = 0;

#define ASSIGNMENT_OPER(oper)                                                       \
void operator oper(const MInt& val2) {                                              \
    value oper val2.value;                                                          \
                                                                                    \
    char temp_str[MAX_INFO_LEN];                                                    \
    sprintf(temp_str, " -> (%s %d => %d)", #oper, val2.value, value);               \
    strcat(INFO_STR, temp_str);                                                     \
                                                                                    \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);                               \
    drawArrow(VALUE_PREFIX, val2.id, VALUE_PREFIX, id, #oper);                      \
}                                                                                   \


#define ARITHMETIC_OPER(oper)                                                                        \
MInt operator oper(const MInt& val2) {                                                               \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);                                                \
    MInt ret_value = MInt(value oper val2.value);                                                    \
                                                                                                     \
    char temp_str[MAX_INFO_LEN];                                                                     \
    sprintf(temp_str, " -> (%d %s %d = %d)", value, #oper, val2.value, ret_value.value);             \
    strcat(INFO_STR, temp_str);                                                                      \
                                                                                                     \
    const char* color = getRandomColor();                                                            \
                                                                                                     \
    drawOperBlock(OPER_CNT++, #oper, color);                                                         \
    drawArrow(VALUE_PREFIX, id, OPER_PREFIX, OPER_CNT - 1, "", color);                               \
    drawArrow(VALUE_PREFIX, val2.id, OPER_PREFIX, OPER_CNT - 1, "", color);                          \
    drawArrow(OPER_PREFIX, OPER_CNT - 1, VALUE_PREFIX, ret_value.id, "", color);                     \
    return ret_value;                                                                                \
}                                                                                                    \


#define UNARY_OPER(oper)                                   \
MInt operator oper() {                                     \
    value = oper value;                                    \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);      \
    sprintf(INFO_STR, "%s -> %d", INFO_STR, value); \
    drawArrow(VALUE_PREFIX, id, VALUE_PREFIX, id, #oper);  \
    return *this;                                          \
}                                                          \


#define COMPARISON_OPER(oper)                                                   \
bool operator oper(const MInt& val2) {                                          \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);                           \
    sprintf(INFO_STR, "%s -> ?: %d %s %d", INFO_STR, value, #oper, val2.value); \
    drawArrow(VALUE_PREFIX, id, VALUE_PREFIX, val2.id, #oper);                  \
    return value oper val2.value;                                               \
}                                                                               \


#define PREFIX_OPER(oper)                                 \
MInt operator oper() {                                    \
    oper value;                                           \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);     \
    sprintf(INFO_STR, "%s -> %d", INFO_STR, value);       \
    drawArrow(VALUE_PREFIX, id, VALUE_PREFIX, id, #oper); \
    return *this;                                         \
}                                                         \


#define POSTFIX_OPER(oper)                                \
MInt operator oper(int) {                                 \
    MInt ret_value = *this;                               \
    value oper;                                           \
                                                          \
    printf("OPERATOR %s, VALUE: %d\n", #oper, value);     \
    sprintf(INFO_STR, "%s -> %d", INFO_STR, value);       \
    drawArrow(VALUE_PREFIX, id, VALUE_PREFIX, id, #oper); \
    return ret_value;                                     \
}   


const char* getRandomColor() {
    return GRAPHVIZ_COLORS[rand() % COLOR_CNT];
}

void mprintf(FILE *file, const char *fmt...) {
    if (!file) return;

    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
}

void drawArrow(const char* from_type, int from_id, const char* to_type, int to_id, const char* text, const char* color = DEFAULT_COLOR) {
    mprintf(
        DOT_FILE, 
        "\t%s%ld->%s%ld [color=\"%s\", style=\"dashed\",arrowhead=\"normal\",label=\"%s\"]", 
        from_type,
        from_id, 
        to_type,
        to_id,
        color,
        text
    );
}

void drawOperBlock(int val_id, const char* oper, const char* color) {
    mprintf(
        DOT_FILE, 
        "\t%s%ld[shape=record, style=\"rounded, filled\", color=\"black\", fillcolor=\"%s\", label=\"{%s}\"];\n", 
        OPER_PREFIX,
        val_id,
        color, 
        oper
    );
}

void drawValueBlock(int val_id, int value) {
    fprintf(stderr, "%s\n", INFO_STR);
    mprintf(
        DOT_FILE, 
        "\t%s%ld[shape=record, style=\"rounded, filled\", fillcolor=\"red\", label=\"value: %ld | %s\"];\n", 
        VALUE_PREFIX,
        val_id, 
        value,
        INFO_STR
    );
}


class MInt {
public:
    int value = DEFAULT_INT_VALUE;
    int id    = 0;

public:
    explicit MInt(int _value) : value(_value), id(++VARIABLES_CNT) {
        printf("CTOR: id = %d, value = %d, addr = %p\n", id, value, this);
        drawValueBlock(id - 1, value);
        drawValueBlock(id    , value);
        drawArrow(VALUE_PREFIX, id - 1, VALUE_PREFIX, id, "CTOR");

        char temp_str[MAX_INFO_LEN];
        sprintf(temp_str, "OBJECT CREATED FROM %d", value);
        strcat(INFO_STR, temp_str);

        VARIABLES_CNT++;
    }

    MInt(const MInt& another) : value(another.value), id(++VARIABLES_CNT) {
        printf("COPY CTOR: id = %d, value = %d, addr = %p\n", id, value, this);
        drawValueBlock(id    , value);
        drawArrow(VALUE_PREFIX, another.id, VALUE_PREFIX, id, "COPY CTOR");

        VARIABLES_CNT++;
    }

    ~MInt() {
        printf("DTOR: id = %d, value = %d, addr = %p\n", id, value, this);
        value = DEFAULT_INT_VALUE;

        drawValueBlock(-id, value);
        drawArrow(VALUE_PREFIX, id, VALUE_PREFIX, -id, "DTOR");
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