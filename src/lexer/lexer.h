#ifndef LEXER_H_
#define LEXER_H_

#include "../dstructs/fsm/fsmachine.h"
#include "../dstructs/fsm/fsutils.h"
#include "../dstructs/stack/stack.h"
#include "../memory_manager/memory_manager.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    // operators
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_MOD,
    T_EXP,
    T_ABS,

    // assignment operators
    T_EQL,
    T_PLUS_EQL,
    T_MINUS_EQL,
    T_MUL_EQL,
    T_DIV_EQL,

    // relational operators
    T_LESS,
    T_GREATER,
    T_LESS_EQL,
    T_GREATER_EQL,
    T_EQL_EQL,
    T_NOT_EQL,

    // logical operators
    T_AND,
    T_OR,
    T_NOT,

    // data types
    T_INT,
    T_STR,
    T_DBL,
    T_CHR,
    T_BOOL,
    T_NIL,
    T_ENUM,

    // DATA TYPES (RESERVED WORD) / IDENTIFIER
    T_DTYPE,

    // literals
    T_IDENT,
    T_NUMBER,

    // keywords
    T_LOOP,
    T_IN,
    T_TO,
    T_BY,
    T_WHEN,
    T_ELSE,
    T_WHILE,
    T_STOP,
    T_TRUE,
    T_FALSE,
    T_AS,

    // variable declaration
    T_GLOBAL,
    T_FIXED,
    T_FLEX,

    // noise words
    T_IS,
    T_THEN,

    // SPECIAL
    T_SQUOTE,
    T_DQUOTE,
    T_TQUOTE,
    T_QMARK,
    T_LPAREN,
    T_RPAREN,
    T_LSQGLY,
    T_RSQGLY,
    T_LBRACKET,
    T_RBRACKET,
    T_COLON,
    T_COMMA,
    T_TAB,
    T_NEWLINE,
    T_EOF,

    // RESERVED WORD / SYMBOL (for future purposes)
    T_FORM,
    T_FUNCTION,

    // COMMENT
    T_COMMENT,

    // INDENTATIONS
    T_INDENT,
    T_DEDENT,

    // IO FUNCTIONS
    T_INPUT,
    T_OUTPUT,

    // INVALID
    T_INVALID,
    T_ERROR
} token_t;

typedef struct {
    int key;
    token_t token_type;
    char *lexeme;
    int line;
    int col;
} Token;

typedef struct {
    StateMachine *state_machine;
    StateMachine *indent_state_machine;
    StateNode *current_state;
    StateNode *indent_current_state;
    Stack *indent_stack;
    Token *token_array;
    int indent_val;
    int token_count;
    int token_memory;
    char *source;
    int start;
    int current;
    int line;
    int col;
} Lexer;

void tokens_print(Token *token_array, int tokenCount);
void tokens_free(Token *token_array);
void lexer_initialize(char *src, StateMachine *state_machine,
                      StateMachine *indent_state_machine);
int lexer_start(bool print_transition);
int tokens_save(char *file_name);
void lexer_free();

#endif