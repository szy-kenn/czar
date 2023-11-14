#include "lexer.h"
#include <stdio.h>

int start_tokenization(FILE *fp, Token *tokenArray) {
    char currentChar;

    while ((currentChar = getc(fp)) != EOF) {
        printf("%c", currentChar);
    }
}