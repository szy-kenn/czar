#ifndef _CZAR_PARSER_H_
#define _CZAR_PARSER_H_

#include "../lexer/lexer.h"
// typedef struct ParseTreeNode;

typedef enum action_type_t {
    SHIFT,
    REDUCE,
    GOTO,
    ACCEPTED,
    ERROR,
} ActionType;

typedef enum input_string {
    P_END,
    P_MOD,
    P_LPAREN,
    P_RPAREN,
    P_MUL,
    P_PLUS,
    P_MINUS,
    P_DIV,
    P_EXP,
    P_ABS,
    P_ERR,
    P_INT,
    P_DBL,
    P_TRUE,
    P_FALSE,
    P_ID,
    P_OR,
    P_AND,
    P_NOT,
    P_EQL_EQL,
    P_NOT_EQL,
    P_GREATER,
    P_GREATER_EQL,
    P_LESS,
    P_LESS_EQL,
    P_ACCEPT,
    P_EXPR,
    P_CONJ,
    P_EQL,
    P_REL,
    P_TERM,
    P_FACTOR,
    P_POWER,
    P_UNARY,
    P_ABSVAL,
    P_LITERAL
} InputString;

typedef struct state {
    ActionType action_type;
    int value;
} parsing_state;

#define S(value) {SHIFT, value}
#define R(value) {REDUCE, value}
#define GT(value) {GOTO, value}
#define ACCEPT {ACCEPTED, 0}
#define X {ERROR, 0}

typedef struct production_t {
    InputString lhs;
    int rhs_count;
    InputString rhs[5];
} Production;

typedef struct {
    int current;
    Token *token_array;
    int token_count;
} Parser;

/*
typedef struct {
    struct ParseTreeNode *root;
} ParseTree;

typedef struct {
    void *type;
    void *value;
    struct ParseTreeNode *parent;
    struct ParseTreeNode *children;
} ParseTreeNode;
*/

void parser_initialize(Token *token_array, int token_count);
void parser_start(bool debug);

#endif