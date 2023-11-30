#include "lexer.h"
#include "../utils/utils.h"
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

//  get the character without updating the pointer
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

void digits_get(FILE *fp, char *dest, int *cur_pos) {
    char current_digit;
    char next_char;

    while (isdigit(current_digit = char_get(fp, *cur_pos))) {
        char_concat(dest, current_digit);
        (*cur_pos)++;
    }
}

int word_get(FILE *fp, char *dest, int *cur_pos) {
    char current_char;
    char next_char;

    while (isalnum(current_char = char_get(fp, *cur_pos)) || current_char == '_') {
        if (strlen(dest) >= MAX_BUFFER) {
            return -1;
        }
        current_char = char_get(fp, *cur_pos);
        char_concat(dest, current_char);
        (*cur_pos)++;
    }

    return 0;
}

int is_delimiter(char chr, char *delimiters, int delimiter_count) {
    for (int i = 0; i < delimiter_count; i++) {
        if (chr == delimiters[i] || chr == EOF) {
            return 1;
        }
    }

    return 0;
}

int delimited_str_get(FILE *fp, char *dest, int *cur_pos, char *delimiters, int delimiter_count) {

    int length = 0;

    while (!is_delimiter(char_get(fp, *cur_pos), delimiters, delimiter_count)) {
        if (dest != NULL) {
            char_concat(dest, char_get(fp, *cur_pos));
        }
        (*cur_pos)++;
        length++;
    }

    if (dest != NULL) {
        char_concat(dest, '\0');
    }

    return length;
}

void tokens_print(Token *token_array, int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        printf("[%s | %s]\n", token_array[i].name, token_array[i].value);
    }
}

void tokens_free(Token *token_array, int arr_length) {
    for (int i = 0; i < arr_length; i++) {
        free(token_array[i].value);
    }

    free(token_array);
}

void token_add(Token *token_array, int *token_count, token_t token_type, char *value, char *name) {
    Token new_token = {.token_type = token_type};
    new_token.value = (char *)malloc(strlen(value) + 1);
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
    char *substring = malloc(sizeof(char) * MAX_BUFFER);
    *substring = '\0';

    while ((current_char = char_get(fp, current_position)) != EOF) {

        char next_char;

        if (current_char != ' ') {
            if (isdigit(current_char)) {
                digits_get(fp, substring, &current_position);
                token_add(token_array, &token_count, T_DIGIT, substring, "T_DIGIT");
                next_char = char_peek(fp, current_position);
                if(!isdigit(next_char) && next_char != '\n' && next_char != EOF){
                    print_error("Lexical Error",
                                "It must only contain digits.",
                                current_line + 1);
                    return -1;
                }
                 
            *substring = '\0';
                // TODO: get all consecutive digits
            }

            else if (isalnum(current_char) || current_char == '_') {
                int word_get_status = word_get(fp, substring, &current_position);

                if (word_get_status < 0) {
                    print_error("Lexical Error", "Variable name exceeds character limit",
                                current_line);
                    return -1;
                }

                // if the substring contains an underscore, then it is an identifier
                if (strchr(substring, '_') != NULL) {
                    token_add(token_array, &token_count, T_IDENT, substring, "T_ID");
                }

                // TODO: continue keywords, reserved words
                else if (strcmp(substring, "int") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring, "T_DTYPE");
                } else if (strcmp(substring, "str") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring, "T_DTYPE");
                } else if (strcmp(substring, "chr") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring, "T_DTYPE");
                }

                else if (strcmp(substring, "while") == 0) {
                    token_add(token_array, &token_count, T_WHILE, substring, "T_WHILE");
                }
                else if (strcmp(substring, "chr") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring, "T_DTYPE");
                }

                else {
                    token_add(token_array, &token_count, T_IDENT, substring, "T_ID");
                }

                *substring = '\0';
            } else {
                switch (current_char) {
                    // TODO: symbols
                    case '+':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_ADD_EQL, "+=", "T_ADD_EQL");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_ADD, "+", "T_ADD");
                        }
                        break;

                    case '-':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_SUB_EQL, "-=", "T_SUB_EQL");
                            current_position++;
                        } else if (next_char == '-') {
                            if (char_peek(fp, current_position + 2) == '*') {
                                token_add(token_array, &token_count, T_DDASH_STAR, "--*",
                                          "T_DDASH_STAR");
                                current_position += 2;
                            } else {
                                token_add(token_array, &token_count, T_DDASH, "--", "T_DDASH");
                                current_position++;

                                current_char = char_get(fp, current_position);
                                while (current_char != '\n' && current_char != EOF) {
                                    current_position++;
                                    current_char = char_get(fp, current_position);
                                }
                            }
                        } else {
                            token_add(token_array, &token_count, T_SUB, "-", "T_SUB");
                        }
                        break;

                    case '*':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_MUL_EQL, "*=", "T_MUL_EQL");
                            current_position++;
                        } else if (next_char == '-') {
                            if (next_char == char_peek(fp, current_position + 2)) {
                                token_add(token_array, &token_count, T_STAR_DDASH, "*--",
                                          "T_STAR_DDASH");
                                current_position += 2;
                            } else {
                                token_add(token_array, &token_count, T_MUL, "*", "T_MUL");
                            }
                        } else {
                            token_add(token_array, &token_count, T_MUL, "*", "T_MUL");
                        }
                        break;

                    case '/':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_DIV_EQL, "/=", "T_DIV_EQL");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_DIV, "/", "T_DIV");
                        }
                        break;

                    case '=':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_EQL_EQL, "==", "T_EQL_EQL");
                            current_position++;
                        } else if (next_char == '>') {
                            token_add(token_array, &token_count, T_ARROW, "=>", "T_ARROW");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_EQL, "=", "T_EQL");
                        }
                        break;

                    case '%':
                        token_add(token_array, &token_count, T_MOD, "%", "T_MOD");
                        break;

                    case '^':
                        token_add(token_array, &token_count, T_EXP, "^", "T_EXP");
                        break;

                    case '|':
                        token_add(token_array, &token_count, T_ABS, "|", "T_ABS");
                        break;

                    case '<':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_LESS_EQL, "<=", "T_LESS_EQL");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_LESS, "<", "T_LESS");
                        }
                        break;

                    case '>':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_GREATER_EQL,
                                      ">=", "T_GREATER_EQL");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_GREATER, ">", "T_GREATER");
                        }
                        break;

                    case '"':

                        // token_add(token_array, &token_count, T_DQUOTE, "\"", "T_DQUOTE");
                        current_position++;

                        next_char = char_peek(fp, current_position);
                        if (next_char == '"') {
                            print_error("Lexical Error",
                                        "String must contain atleast one character", current_line);
                            return -1;
                        }

                        int string_length =
                            delimited_str_get(fp, NULL, &current_position, "\"\n", 2);
                        current_position -= string_length;
                        char *string_buffer = malloc(string_length + 1);
                        delimited_str_get(fp, string_buffer, &current_position, "\"\n", 2);
                        token_add(token_array, &token_count, T_STR, string_buffer, "T_STR");

                        free(string_buffer);

                        current_char = char_get(fp, current_position);
                        if (current_char != '"') {
                            print_error("Lexical Error", "Unterminated string literal",
                                        current_line);
                            return -1;
                        }

                        break;

                    case '\'':
                        // token_add(token_array, &token_count, T_SQUOTE, "'", "T_SQUOTE");
                        current_position++;

                        // current_char = char_get(fp, current_position);
                        // if (current_char == '\'') {
                        //     print_error("Lexical Error",
                        //                 "Quoted strings must contain atleast one character.",
                        //                 current_line);

                        //     return -1;
                        // }

                        // char_concat(substring, current_char);
                        // token_add(token_array, &token_count, T_CHR, substring, "T_CHR");
                        // current_position++;
                        // substring = '\0';``

                        // if (current_char != '\'') {
                        //     print_error("Lexical Error",
                        //                 "Single-quoted string must only contain one character.",
                        //                 current_line);
                        //     return -1;
                        // }

                        while ((current_char = char_get(fp, current_position)) != EOF) {
                            if (current_char == '\n') {
                                printf("Error: Not terminated");
                                break;
                            } else if (current_char == '\'') {
                                int char_size = sizeof(substring) / sizeof(char *);
                                if (char_size == 1) {
                                    token_add(token_array, &token_count, T_CHR, substring, "T_CHR");
                                    current_position++;
                                    *substring = '\0';
                                    break;
                                } else if (char_size == 0) {
                                    printf("Error: Must contain one character");
                                    break;
                                } else {
                                    printf("Error: Should contain one character");
                                    break;
                                }
                            }

                            char_concat(substring, char_get(fp, current_position));
                            current_position++;
                        }

                        break;

                    case '?':
                        token_add(token_array, &token_count, T_QMARK, "?", "T_QMARK");
                        break;

                    case '.':
                        token_add(token_array, &token_count, T_DOT, ".", "T_DOT");
                        break;

                    case ',':
                        token_add(token_array, &token_count, T_COMMA, ",", "T_COMMA");
                        break;

                    case '(':
                        token_add(token_array, &token_count, T_LPAREN, "(", "T_LPAREN");
                        break;

                    case ')':
                        token_add(token_array, &token_count, T_RPAREN, ")", "T_RPAREN");
                        break;

                    case '{':
                        token_add(token_array, &token_count, T_LSQGLY, "{", "T_LSQGLY");
                        break;

                    case '}':
                        token_add(token_array, &token_count, T_RSQGLY, "}", "T_RSQGLY");
                        break;

                    case '[':
                        token_add(token_array, &token_count, T_LBRACKET, "[", "T_LBRACKET");
                        break;

                    case ']':
                        token_add(token_array, &token_count, T_RBRACKET, "]", "T_RBRACKET");
                        break;

                    case ':':
                        token_add(token_array, &token_count, T_COLON, ":", "T_COLON");
                        break;

                    case '&':
                        token_add(token_array, &token_count, T_AMP, "&", "T_AMP");
                        break;

                    case '\t':
                        token_add(token_array, &token_count, T_TAB, "\\t", "T_TAB");
                        break;

                    case '\n':
                        current_line++;
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