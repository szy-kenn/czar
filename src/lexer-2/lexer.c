#include "lexer.h"
#include <stdio.h>

void tokens_print(Token *token_array, int tokenCount);
void tokens_free(Token *token_array, int arr_length);
int start_tokenization(FILE *fp, Token *tokenArray);
void lexer_initialize(const char *src) {
    lexer.line = 1;
    lexer.source = src;
}

Lexer lexer;