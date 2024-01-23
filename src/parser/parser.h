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

typedef enum input_string {
    P_END,
    P_ERROR,
    P_IDENT,
    P_STR,
    P_CHR,
    P_BOOL,
    P_NIL,
    P_INT,
    P_DBL,
    P_T_INDENT,
    P_T_DEDENT,
    P_GLOBAL,
    P_FIXED,
    P_FLEX,
    P_INPUT,
    P_OUTPUT,
    P_WHEN,
    P_ELSE,
    P_LOOP,
    P_AND,
    P_OR,
    P_EQUALS,
    P_NOTEQUALS,
    P_GREATER,
    P_GREATEREQUAL,
    P_LESS,
    P_LESSEQUAL,
    P_PLUS,
    P_MINUS,
    P_TIMES,
    P_DIVIDE,
    P_MODULO,
    P_POW,
    P_NOT,
    P_ABS,
    P_COLON,
    P_EQUAL,
    P_LPAREN,
    P_RPAREN,
    P_AMPERSAND,
    P_in,
    P_to,
    P_by,
    P_int,
    P_dbl,
    P_str,
    P_chr,
    P_bool,
    P_nil,
    P_true,
    P_false,
    P_ACCEPT,
    P_program,
    P_stmts,
    P_stmt,
    P_decl_stmt,
    P_mut_type,
    P_assign_stmt,
    P_value,
    P_input_stmt,
    P_output_stmt,
    P_string_consts,
    P_string_const,
    P_when_stmt,
    P_else_stmt,
    P_loop_stmt,
    P_data_type,
    P_expression,
    P_conjunction,
    P_equality,
    P_relational,
    P_abs,
    P_term,
    P_factor,
    P_power,
    P_unary,
    P_literal,
    P_EPSILON = 1000,
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