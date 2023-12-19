#include "lexer.h"
#include <stdio.h>

Lexer lexer;

// gets the current character, and updates the position
char char_get() {
    lexer.col++;
    return lexer.source[lexer.current++];
}

/* gets the lexeme, this will get freed in tokens_free() */
char *lexeme_get() {
    char *buffer = malloc(lexer.current - lexer.start);
    for (int i = lexer.start; i < lexer.current - 1; i++) {
        buffer[i - lexer.start] = lexer.source[i];
    }
    buffer[lexer.current - lexer.start - 1] = '\0';
    return buffer;
}

void tokens_print(Token *token_array, int tokenCount);

void tokens_free(Token *token_array) {
    for (int i = 0; i < lexer.token_count; i++) {
        free(lexer.token_array[i].lexeme);
    }

    memory_free(sizeof(token_array), token_array, lexer.token_count);
}

void token_add(token_t token_type, char *lexeme) {
    if (lexer.token_memory < lexer.token_count + 1) {
        int old_memory = lexer.token_memory;
        lexer.token_memory = capacity_expand(old_memory);
        lexer.token_array = (Token *)memory_expand(
            sizeof(Token), lexer.token_array, old_memory, lexer.token_memory);
    }

    Token token = {.key = lexer.token_count,
                   .token_type = token_type,
                   .col = lexer.col - (lexer.current - lexer.start),
                   .line = lexer.line};

    token.lexeme = malloc(strlen(lexeme) + 1);
    strcpy(token.lexeme, lexeme);

    lexer.token_array[lexer.token_count] = token; // add new token
    lexer.token_count++;
}

char *token_name_get(token_t token_type) {

    switch (token_type) {
        case T_PLUS:
            return "T_PLUS";
        case T_MINUS:
            return "T_MINUS";
        case T_MUL:
            return "T_MUL";
        case T_DIV:
            return "T_DIV";
        case T_MOD:
            return "T_MOD";
        case T_EXP:
            return "T_EXP";
        case T_ABS:
            return "T_ABS";
        case T_EQL:
            return "T_EQL";
        case T_PLUS_EQL:
            return "T_PLUS_EQL";
        case T_MINUS_EQL:
            return "T_MINUS_EQL";
        case T_MUL_EQL:
            return "T_MUL_EQL";
        case T_DIV_EQL:
            return "T_DIV_EQL";
        case T_LESS:
            return "T_LESS";
        case T_GREATER:
            return "T_GREATER";
        case T_LESS_EQL:
            return "T_LESS_EQL";
        case T_GREATER_EQL:
            return "T_GREATER_EQL";
        case T_EQL_EQL:
            return "T_EQL_EQL";
        case T_NOT_EQL:
            return "T_NOT_EQL";
        case T_AND:
            return "T_AND";
        case T_OR:
            return "T_OR";
        case T_NOT:
            return "T_NOT";
        case T_INT:
            return "T_INT";
        case T_STR:
            return "T_STR";
        case T_DBL:
            return "T_DBL";
        case T_CHR:
            return "T_CHR";
        case T_BOOL:
            return "T_BOOL";
        case T_NIL:
            return "T_NIL";
        case T_ENUM:
            return "T_ENUM";
        case T_DTYPE:
            return "T_DTYPE";
        case T_IDENT:
            return "T_IDENT";
        case T_NUMBER:
            return "T_NUMBER";
        case T_LOOP:
            return "T_LOOP";
        case T_IN:
            return "T_IN";
        case T_TO:
            return "T_TO";
        case T_BY:
            return "T_BY";
        case T_WHEN:
            return "T_WHEN";
        case T_ELSE:
            return "T_ELSE";
        case T_WHILE:
            return "T_WHILE";
        case T_STOP:
            return "T_STOP";
        case T_TRUE:
            return "T_TRUE";
        case T_FALSE:
            return "T_FALSE";
        case T_AS:
            return "T_AS";
        case T_GLOBAL:
            return "T_GLOBAL";
        case T_FIXED:
            return "T_FIXED";
        case T_FLEX:
            return "T_FLEX";
        case T_IS:
            return "T_IS";
        case T_THEN:
            return "T_THEN";
        case T_SQUOTE:
            return "T_SQUOTE";
        case T_DQUOTE:
            return "T_DQUOTE";
        case T_TQUOTE:
            return "T_TQUOTE";
        case T_QMARK:
            return "T_QMARK";
        case T_LPAREN:
            return "T_LPAREN";
        case T_RPAREN:
            return "T_RPAREN";
        case T_LSQGLY:
            return "T_LSQGLY";
        case T_RSQGLY:
            return "T_RSQGLY";
        case T_LBRACKET:
            return "T_LBRACKET";
        case T_RBRACKET:
            return "T_RBRACKET";
        case T_COLON:
            return "T_COLON";
        case T_COMMA:
            return "T_COMMA";
        case T_TAB:
            return "T_TAB";
        case T_NEWLINE:
            return "T_NEWLINE";
        case T_EOF:
            return "T_EOF";
        case T_FORM:
            return "T_FORM";
        case T_FUNCTION:
            return "T_FUNCTION";
        case T_DDASH:
            return "T_DDASH";
        case T_INDENT:
            return "T_INDENT";
        case T_DEDENT:
            return "T_DEDENT";
        case T_INPUT:
            return "T_INPUT";
        case T_OUTPUT:
            return "T_OUTPUT";
        case T_INVALID:
            return "T_INVALID";
        case T_ERROR:
            return "T_ERROR";
    }
}

void char_print(char c) {

    switch (c) {
        case '\n':
            printf("\\n");
            break;

        case '\r':
            printf("\\r");
            break;

        case '\t':
            printf("\\t");
            break;

        case '\0':
            printf("\\0");
            break;

        case ' ':
            printf("<space>");
            break;

        default:
            printf("%c", c);
            break;
    }
}

void transition_print(StateNode *current_state, char input,
                      StateNode *next_state) {
    if (current_state->is_accepting_state) {
        printf("(q%d)\t--\t", current_state->idx);
    } else {
        printf("q%d\t--\t", current_state->idx);
    }

    char_print(input);

    if (next_state == NULL) {
        printf("\t--> NULL\n");
    } else if (next_state->is_accepting_state) {
        printf("\t--> (q%d)\n", next_state->idx);
    } else {
        printf("\t-->  q%d\n", next_state->idx);
    }
}

void token_print(Token *token) {
    printf("---------------------------------\n");
    printf(">> TOKEN #%003d | Ln %d, Col %d | %s | ", token->key, token->line,
           token->col, token_name_get(token->token_type));
    for (int i = 0; i < strlen(token->lexeme); i++) {
        char_print(token->lexeme[i]);
    }
    printf("\n---------------------------------\n");
}

void indentation_check(char current_char) {

    StateNode *indent_next_state =
        transition_from(lexer.indent_current_state, current_char);

    if (indent_next_state != NULL) {
        lexer.indent_current_state = indent_next_state;
        switch (lexer.indent_current_state->output) {
            case 1:
                lexer.indent_val++;
                break;
            case T_INDENT:
                printf("Indent Value: %d\n", lexer.indent_val);

                if (lexer.indent_stack->top->value < lexer.indent_val) {
                    stack_push(lexer.indent_stack, lexer.indent_val);
                    int length = snprintf(NULL, 0, "%d", lexer.indent_val);
                    char *digit_str = malloc(length + 1);
                    snprintf(digit_str, length + 1, "%d", lexer.indent_val);

                    token_add(T_INDENT, digit_str);
                    free(digit_str);
                } else {
                    while (lexer.indent_stack->top->value > lexer.indent_val) {

                        int top_indent_val =
                            stack_pop(lexer.indent_stack)->value;
                        int length = snprintf(NULL, 0, "%d", top_indent_val);
                        char *digit_str = malloc(length + 1);
                        snprintf(digit_str, length + 1, "%d", top_indent_val);

                        if (lexer.indent_stack->top->value < lexer.indent_val) {
                            token_add(T_ERROR, "INDENT ERROR");
                        } else {
                            token_add(T_DEDENT, digit_str);
                        }

                        free(digit_str);
                    }
                }

                token_print(&lexer.token_array[lexer.token_count - 1]);

                lexer.indent_val = 0;
                break;
            default:
                lexer.indent_val = 0;
                break;
        }
    }
}

void lexer_initialize(char *src, StateMachine *state_machine,
                      StateMachine *indent_state_machine) {
    lexer.source = src;
    lexer.state_machine = state_machine;
    lexer.indent_state_machine = indent_state_machine;
    lexer.line = 1;
    lexer.col = 1;
    lexer.start = 0;
    lexer.current = 0;
    lexer.token_count = 0;
    lexer.token_memory = 0;
    lexer.token_array = NULL;
    lexer.current_state = NULL;
    lexer.indent_current_state = NULL;
    lexer.indent_stack = NULL;
    lexer.indent_val = 0;
}

int lexer_start(bool print_transition) {
    lexer.current_state = lexer.state_machine->init_state;
    lexer.indent_current_state = lexer.indent_state_machine->init_state;
    lexer.indent_stack = stack_create();
    stack_push(lexer.indent_stack, 0);

    printf("========== STARTING LEXICAL ANALYSIS ==========\n");

    char current_char = char_get();
    StateNode *next_state;

    while (current_char != '\0') {

        /* -1 since lexer.current has already been incremented in char_get()
         */
        lexer.start = lexer.current - 1;

        for (;;) {

            /* transition to the next state with the current input */
            next_state = transition_from(lexer.current_state, current_char);

            if (next_state != NULL) {
                /**
                 * if we landed to an existing state, then get the next input
                 * that will be used in the next iteration
                 */

                /* check indentation */
                indentation_check(current_char);
                if (print_transition)
                    transition_print(lexer.current_state, current_char,
                                     next_state);
                current_char = char_get();
                lexer.current_state = next_state;
                if (next_state->idx == 0) break;

            } else {
                /**
                 * if the next state is NULL, meaning the input stream
                 * ends here, and we'll add a token depending on the output of
                 * the state where we finished
                 */

                if (print_transition)
                    transition_print(lexer.current_state, current_char,
                                     next_state);

                char *lexeme = lexeme_get();
                token_add(lexer.current_state->output, lexeme);
                token_print(&lexer.token_array[lexer.token_count - 1]);

                if (lexer.current_state->output == T_NEWLINE) {
                    lexer.line++;
                    /* since the current char is already in the new line */
                    lexer.col = 2;
                }
                break;
            }
        }

        lexer.current_state = lexer.state_machine->init_state;
    }

    return lexer.token_count;
}

int tokens_save(char *file_name) {
    FILE *fp = fopen(file_name, "w");

    if (fp == NULL) return -1;

    fprintf(fp, "================================================\n");

    for (int i = 0; i < lexer.token_count; i++) {
        fprintf(fp, "#%003d | Ln %d:%d | %s | ", lexer.token_array[i].key,
                lexer.token_array[i].line, lexer.token_array[i].col,
                token_name_get(lexer.token_array[i].token_type));
        printf("%s", lexer.token_array[i].lexeme);
        for (int j = 0; j < strlen(lexer.token_array[i].lexeme); j++) {
            char c = lexer.token_array[i].lexeme[j];
            switch (c) {
                case '\n':
                    fprintf(fp, "\\n");
                    break;

                case '\r':
                    fprintf(fp, "\\r");
                    break;

                case '\t':
                    fprintf(fp, "\\t");
                    break;

                case '\0':
                    fprintf(fp, "\\0");
                    break;

                case ' ':
                    fprintf(fp, "<space>");
                    break;

                default:
                    fprintf(fp, "%c", c);
                    break;
            }
        }
        fprintf(fp, "\n");
    }
}

void lexer_free() {
    tokens_free(lexer.token_array);
    free(lexer.source);
    free(lexer.state_machine);
    free(lexer.indent_state_machine);
    free(lexer.indent_stack);
}