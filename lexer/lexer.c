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
    char underbefore_char;
    char before_char;
    char current_digit;
    char next_char;
    before_char = char_peek(fp, *cur_pos - 1);
    underbefore_char = char_peek(fp, *cur_pos - 2);

    if (underbefore_char == '-') {
        if (!isdigit(before_char)) {
        }
        else if (before_char == '.') {
            char_concat(dest, before_char);
        }
    } 

    while (isdigit(current_digit = char_get(fp, *cur_pos)) || (current_digit == '.')) {
        next_char = char_peek(fp, *cur_pos + 1);
        if (isdigit(next_char)) {
            char_concat(dest, current_digit);
            (*cur_pos)++;
        } else {
            char_concat(dest, current_digit);
            break;
        }
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
    char underbefore_char;
    char before_char;
    int current_position = 0;
    int current_line = 0;
    int token_count = 0;
    char *substring = malloc(sizeof(char) * MAX_BUFFER);
    *substring = '\0';

    while ((current_char = char_get(fp, current_position)) != EOF) {

        char next_char;

        if (current_char != ' ') {

            // TODO: fix
            if (isdigit(current_char)) {
                underbefore_char = char_peek(fp, current_position - 2);
                before_char = char_peek(fp, current_position - 1);
                next_char = char_peek(fp, current_position + 1);

                if (underbefore_char == '-') {
                    if (before_char == '.') {
                        char_concat(substring, underbefore_char);
                        char_concat(substring, before_char);
                        digits_get(fp, substring, &current_position);
                        while (isdigit(current_char = char_get(fp, current_position)) || (current_char == '.')) {
                            next_char = char_peek(fp, current_position + 1);
                            if (isdigit(next_char)) {
                                char_concat(substring, current_char);
                                (current_position)++;
                            }
                            else if (!isdigit(next_char)) {
                                break;
                            }
                        }
                        if (next_char != '.') {
                            token_add(token_array, &token_count, T_DBL, substring, "T_DBL");
                        } else {
                            print_error("Lexical Error", "It must only contain one decimal",
                                         current_line);
                            return -1;
                        }
                    } else if (before_char == '+' || before_char == ' ') {
                        while (isdigit(current_char = char_get(fp, current_position))) {
                            next_char = char_peek(fp, current_position + 1);
                            if (isdigit(next_char)) {
                                char_concat(substring, current_char);
                                (current_position)++;
                            } else {
                                break;
                            }
                        }
                        if (next_char == '.') {
                            current_position++;
                            char_concat(substring, current_char);
                            while (isdigit(current_char = char_get(fp, current_position)) || (current_char == '.')) {
                                next_char = char_peek(fp, current_position + 1);
                                if (isdigit(next_char)) {
                                    char_concat(substring, current_char);
                                    (current_position)++;
                                } else {
                                    break;
                                }
                            } if (next_char != '.') {
                                digits_get(fp, substring, &current_position);
                                token_add(token_array, &token_count, T_DBL, substring, "T_DBL");
                            } else if (next_char == '.') {
                                print_error("Lexical Error", "It must only contain one decimal",
                                            current_line);
                                return -1;
                            } 
                        } else {
                            digits_get(fp, substring, &current_position);
                            token_add(token_array, &token_count, T_INT, substring, "T_INT");
                        }
                    } else {
                        char_concat(substring, before_char);
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_INT, substring, "T_INT");
                    }
                } else if (before_char == '-') {
                    if (!isdigit(underbefore_char)){
                        char_concat(substring, before_char);
                    } else if (underbefore_char == '-'){
                        char_concat(substring, before_char);
                    }
                    while (isdigit(current_char = char_get(fp, current_position))) {
                        next_char = char_peek(fp, current_position + 1);
                        if (isdigit(next_char)) {
                            char_concat(substring, current_char);
                            (current_position)++;
                        } 
                        else {
                            break;
                        }
                    }
                    if (next_char == '.') {
                        current_position++;
                        char_concat(substring, current_char);
                        while (isdigit(current_char = char_get(fp, current_position)) || (current_char == '.')) {
                            next_char = char_peek(fp, current_position + 1);
                            if (isdigit(next_char)) {
                                char_concat(substring, current_char);
                                (current_position)++;
                            }
                            else if (!isdigit(next_char)) {
                                break;
                            }
                        } if (next_char != '.') {
                            digits_get(fp, substring, &current_position);
                            token_add(token_array, &token_count, T_DBL, substring, "T_DBL");
                        } else {
                            print_error("Lexical Error", "It must only contain one decimal",
                                         current_line);
                            return -1;
                        }
                    
                    } else {
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_INT, substring, "T_INT");
                    }
                } else if (before_char != '-' && before_char != '.') {
                    while (isdigit(current_char = char_get(fp, current_position))) {
                        next_char = char_peek(fp, current_position + 1);
                        if (isdigit(next_char)) {
                            char_concat(substring, current_char);
                            (current_position)++;
                        } else {
                            break;
                        }
                    }
                    current_position++;
                    if (next_char == '.') {
                        char_concat(substring, current_char);
                        while (isdigit(current_char = char_get(fp, current_position)) || (current_char == '.')) {
                            next_char = char_peek(fp, current_position + 1);
                            if (isdigit(next_char)) {
                                char_concat(substring, current_char);
                                (current_position)++;
                            } else {
                                break;
                            }
                        } if (next_char != '.') {
                            digits_get(fp, substring, &current_position);
                            token_add(token_array, &token_count, T_DBL, substring, "T_DBL");
                        } if (next_char == '.') {
                            print_error("Lexical Error", "It must only contain one decimal",
                                         current_line);
                            return -1;
                        }
                        
                    } else if (next_char != '.') {
                        current_position--;
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_INT, substring, "T_INT");
                    }
                } 
                else if (before_char == '.') {
                    if (underbefore_char == '.') {
                        print_error("Lexical Error", "Itat must only contain one decimal",
                                    current_line);
                        return -1;
                    } else {
                        char_concat(substring, before_char);
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_DBL, substring, "T_DBL");
                    }
                }
                else {
                    print_error("Lexical Error", "Itat must only contain one decimal",
                                current_line);
                            return -1;
                }
                *substring = '\0';
            }  

            // TODO: fix
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
                // tokenization for keywords (datatype)
                else if (strcmp(substring, "int") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring,
                              "T_DTYPE"); // adds token
                } else if (strcmp(substring, "str") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring,
                              "T_DTYPE"); // adds token
                } else if (strcmp(substring, "chr") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring,
                              "T_DTYPE"); // adds token
                } else if (strcmp(substring, "dbl") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring,
                              "T_DTYPE"); // adds token
                } else if (strcmp(substring, "bool") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring,
                              "T_DTYPE"); // adds token
                } else if (strcmp(substring, "nil") == 0) {
                    token_add(token_array, &token_count, T_DTYPE, substring,
                              "T_DTYPE"); // adds token
                }
                // tokenization for keywords (control)
                else if (strcmp(substring, "when") == 0) {
                    token_add(token_array, &token_count, T_WHEN, substring, "T_WHEN"); // adds token
                } else if (strcmp(substring, "else") == 0) {
                    token_add(token_array, &token_count, T_ELSE, substring, "T_ELSE"); // adds token
                } else if (strcmp(substring, "loop") == 0) {
                    token_add(token_array, &token_count, T_LOOP, substring, "T_LOOP"); // adds token
                } else if (strcmp(substring, "to") == 0) {
                    token_add(token_array, &token_count, T_TO, substring, "T_TO"); // adds token
                } else if (strcmp(substring, "by") == 0) {
                    token_add(token_array, &token_count, T_BY, substring, "T_BY"); // adds token
                } else if (strcmp(substring, "while") == 0) {
                    token_add(token_array, &token_count, T_WHILE, substring,
                              "T_WHILE"); // adds token
                }

                // tokenization for reserved words
                else if (strcmp(substring, "true") == 0) {
                    token_add(token_array, &token_count, T_TRUE, substring, "T_TRUE");// adds token
                } else if (strcmp(substring, "false") == 0) {
                    token_add(token_array, &token_count, T_FALSE, substring, "T_FALSE");// adds token
                } else if (strcmp(substring, "and") == 0) {
                    token_add(token_array, &token_count, T_AND, substring, "T_AND");// adds token
                } else if (strcmp(substring, "or") == 0) {
                    token_add(token_array, &token_count, T_OR, substring, "T_OR");// adds token
                } else if (strcmp(substring, "not") == 0) {
                    token_add(token_array, &token_count, T_NOT, substring, "T_NOT");// adds token
                } else if (strcmp(substring, "global") == 0) {
                    token_add(token_array, &token_count, T_GLOBAL, substring, "T_GLOBAL");// adds token
                } else if (strcmp(substring, "fixed") == 0) {
                    token_add(token_array, &token_count, T_FIXED, substring, "T_FIXED");// adds token
                } else if (strcmp(substring, "flex") == 0) {
                    token_add(token_array, &token_count, T_FLEX, substring, "T_FLEX");// adds token
                } else if (strcmp(substring, "enum") == 0) {
                    token_add(token_array, &token_count, T_ENUM, substring, "T_ENUM");// adds token
                } else if (strcmp(substring, "is") == 0) {
                    token_add(token_array, &token_count, T_IS, substring, "T_IS");// adds token
                } else if (strcmp(substring, "as") == 0) {
                    token_add(token_array, &token_count, T_AS, substring, "T_AS");// adds token
                }

                else {
                    token_add(token_array, &token_count, T_IDENT, substring, "T_ID"); // adds token
                }

                *substring = '\0';
            } else {

                // TODO: add '@' and '!=', array data type [], string concatenation '+'
                switch (current_char) {
                    case '+':
                        next_char = char_peek(fp, current_position + 1);
                        before_char = char_peek(fp, current_position - 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_ADD_EQL, "+=", "T_ADD_EQL");
                            current_position++;
                        } 
                        else if (isdigit(next_char) && (before_char == '-') || (before_char == '+')) {
                            break;
                        } else if (next_char == '.' && before_char == '-') {
                            break;
                        }
                        else {
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
                            } else if (isalnum(char_peek(fp, current_position - 1)) && (isdigit(current_char = (char_peek(fp, current_position + 2)))) || current_char == '.' ) {
                                token_add(token_array, &token_count, T_SUB, "-", "T_SUB"); } 
                                else {
                                token_add(token_array, &token_count, T_DDASH, "--", "T_DDASH");
                                current_position++;

                                current_char = char_get(fp, current_position);
                                while (current_char != '\n' && current_char != EOF) {
                                    current_position++;
                                    current_char = char_get(fp, current_position);
                                }
                            }
                        } else if (isdigit(next_char) && (!isdigit(char_peek(fp, current_position - 1)))) {
                            if (char_peek(fp, current_position - 1) != '-' || char_peek(fp, current_position - 1) != ' ' ) {
                                break;
                            }
                            else {
                                token_add(token_array, &token_count, T_SUB, "-", "T_bSUB");// if (next_char != '.'){
                            }
                            break;
                        } 
                        else if (next_char == '.' ) {
                            if (char_peek(fp, current_position - 1) != '\0'){
                                if(isdigit(char_peek(fp, current_position - 1))) {
                                    token_add(token_array, &token_count, T_SUB, "-", "T_SUB");
                                }
                                else {
                                    break;
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
                        current_position++;

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

                    case '!':
                        // TODO: not equal token
                        break;

                    case '?':
                        token_add(token_array, &token_count, T_QMARK, "?", "T_QMARK");
                        break;

                    case '.':
                        next_char = char_peek(fp, current_position + 1);
                        if (isdigit(next_char)){
                            break;
                        } else if (next_char = char_peek(fp, current_position + 1) && (isdigit(next_char))){
                            break;
                        } else {
                            token_add(token_array, &token_count, T_DOT, ".", "T_DOT");
                            break;
                        }

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
                        // TODO: invalid token
                        // invalid_char[1] = {current_char};
                        // token_add(token_array, &token_count, T_INVALID, invalid_char,
                        // "T_INVALID");
                        break;
                }
            }
        }

        current_position++;
    }

    return token_count;
}