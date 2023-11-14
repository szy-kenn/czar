#include <stdio.h>

typedef enum { T_INT } token_t;

typedef struct token {
    token_t token_type;
    char value[30];
    char name[30];
} Token;

int start_tokenization(FILE *fp, Token *tokenArray);