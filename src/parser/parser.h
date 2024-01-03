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
    ParseTreeNode *root;
} ParseTree;

typedef struct {
    void *type;
    void *value;
    ParseTreeNode *parent;
    ParseTreeNode *children;
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
    UnaryOperator unary_operator;
    Abs *abs;
} Unary;

typedef struct {
    Unary *left;
    Unary *right;
} Power;

typedef struct {
    Power *left;
    FactorOperator factor_operator;
    Power *right;
} Factor;

typedef struct {
    Factor *left;
    TermOperator term_operator;
    Factor *right;
} Term;

typedef struct {
    Term *left;
    RelationalOperator relational_operator;
    Term *right;
} Relational;

typedef struct {
    Relational *left;
    EqualityOperator equality_operator;
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