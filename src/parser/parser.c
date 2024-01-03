#include "parser.h"
#include <stdio.h>

Parser parser;

void parser_initialize(Token *token_array, int token_count) {
    parser.current = 0;
    parser.token_array = token_array;
    parser.token_count = token_count;
};

void parser_start() {
    for (int i = 0; i < parser.token_count; i++) {
        printf("%d", parser.token_array[i].token_type);

        switch (parser.token_array[i].token_type) {
            case T_INT:
            case T_DBL:
                printf("expression");
                break;
            default:
                break;
        }
    }
};
