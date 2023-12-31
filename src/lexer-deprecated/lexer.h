#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>

typedef enum {
    // operators
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_MOD,
    T_EXP,
    T_ABS,

    // assignment
    T_EQL,
    T_PLUS_EQL,
    T_MINUS_EQL,
    T_MUL_EQL,
    T_DIV_EQL,

    // logical operators
    T_LESS,
    T_GREATER,
    T_LESS_EQL,
    T_GREATER_EQL,
    T_EQL_EQL,
    T_AND,
    T_OR,
    T_NOT,
    T_NOT_EQL,

    // data types
    T_INT,
    T_STR,
    T_DBL,
    T_CHR,
    T_BOOL,
    T_NIL,

    // DATA TYPES IDENTIFIER
    T_DTYPE,
    T_IDENT,

    // reserved words
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

    T_TAB,

    T_GLOBAL,
    T_FIXED,
    T_FLEX,

    T_ENUM,

    T_IS,
    T_AS,
    T_AMP,
    T_CHECK,
    T_ARROW,

    // SPECIAL
    T_SQUOTE,
    T_DQUOTE,
    T_TQUOTE,
    T_QMARK,
    T_USCORE,
    T_DOT,

    T_FORM,
    T_FUNCTION,

    T_LPAREN,
    T_RPAREN,
    T_LSQGLY,
    T_RSQGLY,
    T_LBRACKET,
    T_RBRACKET,
    T_COLON,
    T_COMMA,

    // COMMENT
    T_DDASH,

    T_NEWLINE,
    T_INDENT,
    T_DEDENT,
    T_EOF,

    T_INPUT,
    T_OUTPUT,

    // INVALID
    T_INVALID
} token_t;

typedef struct token {
    token_t token_type;
    char *value;
    char name[30];
} Token;

void tokens_print(Token *token_array, int tokenCount);
void tokens_free(Token *token_array, int arr_length);
int start_tokenization(FILE *fp, Token *tokenArray);

#endif