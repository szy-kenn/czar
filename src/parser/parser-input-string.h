#ifndef _PARSER_INPUT_STRING_H_
#define _PARSER_INPUT_STRING_H_

#include "../lexer/lexer.h"

typedef enum {
    P_MOD = T_MOD,
    P_LPAREN = T_LPAREN,
    P_RPAREN = T_RPAREN,
    P_MUL = T_MUL,
    P_PLUS = T_PLUS,
    P_MINUS = T_MINUS,
    P_DIV = T_DIV,
    P_EXP = T_EXP,
    P_ABS = T_ABS,
    P_ERROR,
    P_INT = T_INT,
    P_DBL = T_DBL,
    P_TRUE = T_TRUE,
    P_FALSE = T_FALSE,
    P_ID = T_IDENT,
    P_OR = T_OR,
    P_AND = T_AND,
    P_NOT = T_NOT,
    P_EQL_EQL = T_EQL_EQL,
    P_NOT_EQL = T_NOT_EQL,
    P_GREATER = T_GREATER,
    P_GREATER_EQL = T_GREATER_EQL,
    P_LESS = T_LESS,
    P_LESS_EQL = T_LESS_EQL,
    P_END = 150,
    P_ACCEPT = 151,
    P_EXPR = 152,
    P_CONJUNCTION = 153,
    P_EQUALITY = 154,
    P_RELATIONAL = 155,
    P_TERM = 156,
    P_FACTOR = 157,
    P_POWER = 158,
    P_UNARY = 159,
    P_ABSVAL = 160,
    P_LITERAL = 161
} InputString;

#endif