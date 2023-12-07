#include "lexer.h"
#include "../utils/stack.h"
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

char decimal_checker(FILE *fp, char *dest, int *cur_pos) {
    char current_digit;
    char next_char;
    int current_position;

    while (isdigit(current_digit = char_get(fp, *cur_pos))) {
        next_char = char_peek(fp, *cur_pos + 1);
        if (isdigit(next_char)) {
            char_concat(dest, current_digit);
            (*cur_pos)++;
        } else if (next_char == '.') {
            char_concat(dest, current_digit);
            return 1;
            break;
        } else {
            return 0;
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
        // current_char = char_get(fp, *cur_pos);
        char_concat(dest, current_char);
        (*cur_pos)++;
    }

    (*cur_pos)--;

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

// returns the number of spaces consumed (discarded)
int space_consume(FILE *fp, int *current_position) {
    int count = 0;

    while (char_get(fp, *current_position) == ' ') {
        count++;
        (*current_position)++;
    }

    if (count > 0) {
        (*current_position)--;
    }

    return count;
}

void print_error(FILE *fp, char *error, char *message, int line, int position) {

    int error_position = position;
    int linestart_position;
    char current_char;

    while (position >= 0 && char_get(fp, position) != '\n') {
        position--;
    }

    position++;
    linestart_position = position;

    while ((current_char = char_get(fp, position)) != '\n' && (current_char != EOF)) {
        printf("%c", current_char);
        position++;
    }

    printf("\n");
    position = linestart_position;
    while (position != error_position) {
        printf(" ");
        position++;
    }

    printf("^\n");
    printf("\033[0;31m%s\033[0;37m: %s at line %d", error, message, line);
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
    int _status = 0;
    char current_char;
    char before_char;
    int current_position = 0;
    int current_line = 1;
    int token_count = 0;

    Stack *indent_stack = stack_create();
    stack_push(indent_stack, 0);

    char *substring = malloc(sizeof(char) * MAX_BUFFER);
    *substring = '\0';

    while ((current_char = char_get(fp, current_position)) != EOF) {

        char next_char;

        if (current_position == 0 &&
            (char_get(fp, current_position) == ' ' || char_get(fp, current_position) == '\t')) {
            print_error(fp, "Indentation Error", "Unexpected indentation", current_line,
                        current_position);
            _status = -1;
            break;
        }

        if (char_get(fp, current_position - 1) == '\n') {
            int spaces = space_consume(fp, &current_position);
            if (indent_stack->top->value < spaces) {
                stack_push(indent_stack, spaces);

                int length = snprintf(NULL, 0, "%d", spaces);
                char *digit_str = malloc(length + 1);
                snprintf(digit_str, length + 1, "%d", spaces);

                token_add(token_array, &token_count, T_INDENT, digit_str, "T_INDENT");
                free(digit_str);

            } else if (indent_stack->top->value > spaces) {
                while (indent_stack->top->value > spaces) {

                    int indent_val = stack_pop(indent_stack)->value;
                    int length = snprintf(NULL, 0, "%d", indent_val);
                    char *digit_str = malloc(length + 1);
                    snprintf(digit_str, length + 1, "%d", indent_val);

                    if (indent_stack->top->value < spaces) {
                        print_error(fp, "Indentation Error", "Unexpected indentation", current_line,
                                    current_position);
                        free(digit_str);
                        _status = -1;
                        break;
                    } else {
                        token_add(token_array, &token_count, T_DEDENT, digit_str, "T_DEDENT");
                    }
                    free(digit_str);
                }
            }

            if (_status < 0) {
                break;
            }
        }

        if (current_char != ' ') {

            if (isdigit(current_char)) {
                before_char = char_peek(fp, current_position - 1);

                // digit starts with decimal
                if (before_char == '.') {
                    char_concat(substring, before_char);
                    // contains another starting decimal
                    if ((char_peek(fp, current_position - 2)) == '.') {
                        print_error(fp, "Lexical Error", "It must only contain one decimal",
                                    current_line, current_position);
                        return -1;
                        // contains another decimal
                    } else if (decimal_checker(fp, substring, &current_position) == 1) {
                        digits_get(fp, substring, &current_position);
                        print_error(fp, "Lexical Error", "It must only contain one decimal",
                                    current_line, current_position);
                        return -1;
                        // double
                    } else if (decimal_checker(fp, substring, &current_position) == 0) {
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_INT, substring, "T_DBL");
                    }
                } else {
                    // contains decimal
                    if (decimal_checker(fp, substring, &current_position) == 1) {
                        current_position++;
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_DBL, substring, "T_DBL");
                        // contains another decimal
                        if (decimal_checker(fp, substring, &current_position) == 1) {
                            print_error(fp, "Lexical Error", "It must only contain one decimal",
                                        current_line, current_position);
                            _status = -1;
                            break;
                        }
                        // int
                    } else if (decimal_checker(fp, substring, &current_position) == 0) {
                        digits_get(fp, substring, &current_position);
                        token_add(token_array, &token_count, T_INT, substring, "T_INT");
                    }
                }
                *substring = '\0';
            }

            else if (isalnum(current_char) || current_char == '_') {
                int word_get_status = word_get(fp, substring, &current_position);

                if (word_get_status < 0) {
                    print_error(fp, "Lexical Error", "Variable name exceeds character limit",
                                current_line, current_position);
                    _status = -1;
                    break;
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
                } else if (strcmp(substring, "in") == 0) {
                    token_add(token_array, &token_count, T_IN, substring, "T_IN"); // adds token
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
                    token_add(token_array, &token_count, T_TRUE, substring, "T_TRUE"); // adds token
                } else if (strcmp(substring, "false") == 0) {
                    token_add(token_array, &token_count, T_FALSE, substring,
                              "T_FALSE"); // adds token
                } else if (strcmp(substring, "and") == 0) {
                    token_add(token_array, &token_count, T_AND, substring, "T_AND"); // adds token
                } else if (strcmp(substring, "or") == 0) {
                    token_add(token_array, &token_count, T_OR, substring, "T_OR"); // adds token
                } else if (strcmp(substring, "not") == 0) {
                    token_add(token_array, &token_count, T_NOT, substring, "T_NOT"); // adds token
                } else if (strcmp(substring, "global") == 0) {
                    token_add(token_array, &token_count, T_GLOBAL, substring,
                              "T_GLOBAL"); // adds token
                } else if (strcmp(substring, "fixed") == 0) {
                    token_add(token_array, &token_count, T_FIXED, substring,
                              "T_FIXED"); // adds token
                } else if (strcmp(substring, "flex") == 0) {
                    token_add(token_array, &token_count, T_FLEX, substring, "T_FLEX"); // adds token
                } else if (strcmp(substring, "enum") == 0) {
                    token_add(token_array, &token_count, T_ENUM, substring, "T_ENUM"); // adds token
                } else if (strcmp(substring, "is") == 0) {
                    token_add(token_array, &token_count, T_IS, substring, "T_IS"); // adds token
                } else if (strcmp(substring, "as") == 0) {
                    token_add(token_array, &token_count, T_AS, substring, "T_AS"); // adds token
                } else if (strcmp(substring, "input") == 0) {
                    token_add(token_array, &token_count, T_INPUT, substring,
                              "T_INPUT"); // adds token
                } else if (strcmp(substring, "output") == 0) {
                    token_add(token_array, &token_count, T_OUTPUT, substring,
                              "T_OUTPUT"); // adds token
                }

                else {
                    token_add(token_array, &token_count, T_IDENT, substring, "T_ID"); // adds token
                }

                *substring = '\0';
            } else {

                switch (current_char) {
                    case '+':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_PLUS_EQL, "+=", "T_PLUS_EQL");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_PLUS, "+", "T_PLUS");
                        }
                        break;

                    case '-':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_MINUS_EQL, "-=", "T_MINUS_EQL");
                            current_position++;
                        } else if (next_char == '-') {
                            token_add(token_array, &token_count, T_DDASH, "--", "T_DDASH");
                            current_position++;

                            current_char = char_get(fp, current_position);
                            while (current_char != '\n' && current_char != EOF) {
                                current_position++;
                                current_char = char_get(fp, current_position);
                            }

                        } else {
                            token_add(token_array, &token_count, T_MINUS, "-", "T_MINUS");
                        }
                        break;

                    case '*':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_MUL_EQL, "*=", "T_MUL_EQL");
                            current_position++;
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
                        token_add(token_array, &token_count, T_SQUOTE, "\"", "T_DQUOTE");
                        current_position++;

                        next_char = char_peek(fp, current_position);
                        if (next_char == '"') {
                            print_error(fp, "Lexical Error",
                                        "String must contain atleast one character", current_line,
                                        current_position);
                            _status = -1;
                            break;
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
                            print_error(fp, "Lexical Error", "Unterminated string literal",
                                        current_line, current_position);
                            _status = -1;
                            break;
                        }

                        token_add(token_array, &token_count, T_SQUOTE, "\"", "T_DQUOTE");
                        break;

                    case '\'':
                        current_position++;

                        while ((current_char = char_get(fp, current_position)) != EOF) {
                            if (current_char == '\n') {
                                print_error(fp, "Lexical Error", "Unterminated character literal",
                                            current_line, current_position);
                                _status = -1;
                                break;
                            } else if (current_char == '\'') {
                                int char_size = strlen(substring);
                                if (char_size == 1) {
                                    token_add(token_array, &token_count, T_CHR, substring, "T_CHR");
                                    token_add(token_array, &token_count, T_SQUOTE, "'", "T_SQUOTE");
                                    *substring = '\0';
                                    break;
                                } else if (char_size == 0) {
                                    print_error(
                                        fp, "Lexical Error",
                                        "Single-quoted string must contain a single character",
                                        current_line, current_position);
                                    current_position--;
                                    _status = -1;
                                    break;
                                } else {
                                    print_error(fp, "Lexical Error",
                                                "Single-quoted string must only "
                                                "contain a single character",
                                                current_line, current_position);
                                    current_position--;
                                    _status = -1;
                                    break;
                                }
                            } else {
                                print_error(fp, "Lexical Error", "Unterminated character literal",
                                            current_line, current_position);
                                _status = -1;
                                break;
                            }

                            char_concat(substring, current_char);
                            current_position++;
                        }

                        next_char = char_get(fp, current_position + 1);

                        if (next_char == EOF || next_char == '\n') {
                            print_error(fp, "Lexical Error", "Unterminated character literal",
                                        current_line, current_position);
                            _status = -1;
                            break;
                        } else {
                            token_add(token_array, &token_count, T_SQUOTE, "'", "T_SQUOTE");
                        }

                        break;

                    case '!':
                        next_char = char_peek(fp, current_position + 1);
                        if (next_char == '=') {
                            token_add(token_array, &token_count, T_NOT_EQL, "!=", "T_NOT_EQL");
                            current_position++;
                        } else {
                            token_add(token_array, &token_count, T_INVALID, "!", "T_INVALID");
                        }
                        break;

                    case '?':
                        token_add(token_array, &token_count, T_QMARK, "?", "T_QMARK");
                        break;

                    case '.':
                        next_char = char_peek(fp, current_position + 1);
                        if (isdigit(next_char)) {
                            break;
                        } else if (next_char = char_peek(fp, current_position + 1) &&
                                               (isdigit(next_char))) {
                            break;
                        } else if (char_peek(fp, current_position - 1) == '.' &&
                                   isdigit(char_peek(fp, current_position - 2))) {
                            print_error(fp, "Lexical Error", "It must only contain 1 decimal",
                                        current_line, current_position);
                            return -1;
                        } else {
                            token_add(token_array, &token_count, T_DOT, ".", "T_DOT");
                            break;
                        }

                    case '@':
                        token_add(token_array, &token_count, T_FUNCTION, "@", "T_FUNCTION");
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

                        current_position++;
                        int _pos = current_position;

                        char_concat(substring, current_char);

                        char *temp_substr = malloc(sizeof(char) * MAX_BUFFER);
                        *temp_substr = '\0';

                        while (char_get(fp, current_position) == ' ') {
                            current_position++;
                        }

                        word_get(fp, temp_substr, &current_position);
                        current_position++;

                        if (strcmp(temp_substr, "int") == 0 || strcmp(temp_substr, "chr") == 0 ||
                            strcmp(temp_substr, "str") == 0 || strcmp(temp_substr, "dbl") == 0 ||
                            strcmp(temp_substr, "bool") == 0) {

                            while (char_get(fp, current_position) == ' ') {
                                current_position++;
                            }

                            if ((current_char = char_get(fp, current_position)) == ']') {
                                strcat(substring, temp_substr);
                                char_concat(substring, current_char);
                                token_add(token_array, &token_count, T_DTYPE, substring, "T_DTYPE");
                                *substring = '\0';

                            } else {
                                token_add(token_array, &token_count, T_LBRACKET, "[", "T_LBRACKET");
                                current_position = _pos - 1;
                            }
                        } else {
                            token_add(token_array, &token_count, T_LBRACKET, "[", "T_LBRACKET");
                            current_position = _pos - 1;
                        }

                        free(temp_substr);
                        *substring = '\0';
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

                    case '\n':
                        current_line++;
                        token_add(token_array, &token_count, T_NEWLINE, "\\n", "T_NEWLINE");
                        break;

                    default:
                        char_concat(substring, current_char);
                        token_add(token_array, &token_count, T_INVALID, substring, "T_INVALID");
                        *substring = '\0';
                        break;
                }
            }
        }

        current_position++;
    }

    if (current_char == EOF) {
        token_add(token_array, &token_count, T_EOF, "EOF", "T_EOF");
    }

    free(substring);
    stack_free(indent_stack);

    if (_status < 0) {
        return _status;
    }

    return token_count;
}