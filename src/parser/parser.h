#ifndef _CZAR_PARSER_H_
#define _CZAR_PARSER_H_

#include "../lexer/lexer.h"
typedef struct ParseTreeNode;

typedef struct {
    int current;
    Token *token_array;
    int token_count;
} Parser;

typedef struct {
    struct ParseTreeNode *root;
} ParseTree;

typedef struct {
    void *type;
    void *value;
    struct ParseTreeNode *parent;
    struct ParseTreeNode *children;
} ParseTreeNode;

// node types (expressions)

typedef struct Expression;
typedef struct Disjunction;
typedef struct Conjunction;
typedef struct Equality;
typedef struct Relational;
typedef struct Term;
typedef struct Factor;
typedef struct Power;
typedef struct Unary;
typedef struct Abs;
typedef struct Literal;

// ENUMS

typedef enum { UNARY_NOT, UNARY_MINUS } UnaryOperator;
typedef enum { FACTOR_MUL, FACTOR_DIV, FACTOR_MOD } FactorOperator;
typedef enum { TERM_ADD, TERM_SUB } TermOperator;
typedef enum {
    REL_GREATER_THAN,
    REL_GREATER_EQL,
    REL_LESS_THAN,
    REL_LESS_EQL
} RelationalOperator;
typedef enum { EQL_EQL, NOT_EQL } EqualityOperator;

// STRUCTS

typedef struct {
    void *value;
} Literal;

typedef struct {
    Literal *literal;
} Abs;

typedef struct {
    UnaryOperator operator;
    Abs *abs;
} Unary;

typedef struct {
    Unary *left;
    Unary *right;
} Power;

typedef struct {
    Power *left;
    FactorOperator operator;
    Power *right;
} Factor;

typedef struct {
    Factor *left;
    TermOperator operator;
    Factor *right;
} Term;

typedef struct {
    Term *left;
    RelationalOperator operator;
    Term *right;
} Relational;

typedef struct {
    Relational *left;
    EqualityOperator operator;
    Relational *right;
} Equality;

typedef struct {
    Equality *left;
    Equality *right;
} Conjunction;

typedef struct {
    Conjunction *left;
    Conjunction *right;
} Disjunction;

typedef struct {
    Disjunction *expr;
} Expression;

void parser_initialize(Token *token_array, int token_count);
void parser_start();

#endif