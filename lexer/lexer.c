#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int is_ident(int chr) { return (isalnum(chr) || chr == '_'); }

int is_delimiter(char chr, char *delimiters, int delimiter_count) {
    for (int i = 0; i < delimiter_count; i++) {
        if (chr == delimiters[i] || chr == EOF) {
            return 1;
        }
    }

    return 0;
}

void digits_get(FILE *fp, char *dest, int *cur_pos) {
    char current_digit;
    char next_char;

    while (isdigit(current_digit = char_get(fp, *cur_pos))) {
        char_concat(dest, current_digit);
        (*cur_pos)++;
    }
}

void substr_get(FILE *fp, char *dest, int *cur_pos) {
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

void tokens_print(Token *token_array, int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        printf("[%s | %s]\n", token_array[i].name, token_array[i].value);
    }
}

void token_add(Token *token_array, int *token_count, token_t token_type, char *value, char *name) {
    Token new_token = {.token_type = token_type};
    strcpy(new_token.name, name);
    strcpy(new_token.value, value);
    token_array[*token_count] = new_token;
    (*token_count)++;
}

int start_tokenization(FILE *fp, Token *token_array) {
    char current_char;
    int current_position = 0;
    int current_line = 0;
    int token_count = 0;
    char *substring = malloc(sizeof(char) * 100); // TODO: don't make this arbitrary

    while ((current_char = getc(fp)) != EOF) {

        current_char = char_get(fp, current_position);
        char next_char;

        if (current_char != ' ') {
            if (isdigit(current_char)) {
                // TODO: get all consecutive digits
            }

            else if (is_ident(current_char)) {
                substr_get(fp, substring, &current_position);

                // TODO: continue keywords, reserved words
                if (strcmp(substring, "int") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring, "T_DTYPE");
                } else if (strcmp(substring, "str") == 0) {
                    // TODO: continue
                }

                // after keywords, reserved words
                else {
                    token_add(token_array, &token_count, T_IDENT, substring, "T_ID");
                }

                *substring = '\0';
            } else {
                switch (current_char) {
                    // TODO: symbols
                    case '+':
                        /* code */
                        break;

                    default:
                        break;
                }
            }
        }

        current_position++;
    }

    return token_count;
}