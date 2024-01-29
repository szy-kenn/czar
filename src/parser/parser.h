#ifndef _CZAR_PARSER_H_
#define _CZAR_PARSER_H_

#include "parser-input-string.h"
#include "../lexer/lexer.h"
#include "../dstructs/tree/tree.h"
#include "../dstructs/stack/token_stack/token_stack.h"
// typedef struct ParseTreeNode;

typedef enum action_type_t {
    SHIFT,
    REDUCE,
    GOTO,
    ACCEPTED,
    ERROR,
} ActionType;


/**
 * 
 * "$end", "error", "IDENT", "STR", "CHR", "BOOL", "NIL",
             "INT", "DBL", "T_INDENT", "T_DEDENT", "GLOBAL", "FIXED",
             "FLEX", "INPUT", "OUTPUT", "WHEN", "ELSE", "LOOP",
             "AND", "OR", "EQUALS", "NOTEQUALS", "GREATER",
             "GREATEREQUAL", "LESS", "LESSEQUAL", "PLUS",
             "MINUS", "TIMES", "DIVIDE", "MODULO", "POW",
             "NOT", "ABS", "COLON", "EQUAL", "(", ")", "&", "in",
             "to", "by", "int", "dbl", "str", "chr", "bool", "nil",
             "true", "false","accept", "program", "stmts", "stmt", "decl_stmt",
                "mut_type", "assign_stmt", "value", "input_stmt", 
                "output_stmt", "string_consts", "string_const", 
                "when_stmt", "else_stmt", "loop_stmt", "data_type",
                "expression", "conjunction", "equality",
                "relational", "abs", "term", "factor",
                "power", "unary", "literal"
 * 
*/

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
    InputString rhs[20];
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