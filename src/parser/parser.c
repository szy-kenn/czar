#include "parser.h"
#include <stdio.h>

Parser parser;

void parser_initialize(Token *token_array, int token_count) {
    parser.current = 0;
    parser.token_array = token_array;
    parser.token_count = token_count;
};

void parser_start(){

};
