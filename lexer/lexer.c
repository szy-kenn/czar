#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


int start_tokenization(FILE *fp, Token *tokenArray) {
    char currentChar;

    while ((currentChar = getc(fp)) != EOF) {
        printf("%c", currentChar);
    }
}

// get the character with the given positon in the file
char char_get(FILE *fp, int position) {
    fseek(fp, position, SEEK_SET);
    char chr = fgetc(fp);
    return chr;
}

// get the character without updating the pointer
char char_peek(FILE *fp, int position) {
    fseek(fp, position, SEEK_SET);
    char chr = fgetc(fp);

    // return to original positon
    fseek(fp, position - 1, SEEK_SET);
    return chr;
}

void char_concat(char *dest, char new_chr) {
    int length = strcspn(dest, "\0");
    dest[length] = new_chr;
    dest[length + 1] = '\0';
}

int is_ident(int chr) {
    return (isalnum(chr) || chr == '_');
}

int is_delimiter(char chr, char *delimiters, int delimiter_count) {
    for(int i = 0; i < delimiter_count; i++) {
        if (chr == delimiters[i] || chr == EOF){
            return 1;
        }
    }

    return 0;
}

void substr_get(FILE *fp, char *dest,  int *cur_pos) {
    char current_char; 
    char next_char;
    
    while (is_ident(current_char = char_get(fp, *cur_pos))) {
        char_concat(dest, current_char);
        (*cur_pos)++;
    }
}

void delimited_str_get(FILE *fp, char *dest, int *cur_pos, char *delimiters, int delimiter_count) {
    char current_char = char_get(fp, *cur_pos);

    while (!is_delimiter(current_char, delimiters, delimiter_count)) {
        char_concat(dest, current_char);
        (*cur_pos)++;
    }
}

void add_token(Token *token_array, int *token_count, token_t token_type, char *value, char *name) {
    Token new_token = {.name = strcpy(new_token.name, name), .value = strcpy(new_token.value, value), .token_type = token_type};
    token_array[*token_count] = new_token;
    (*token_count)++;
}