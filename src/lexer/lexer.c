#include "lexer.h"
#include "../czar-state-machine/czar-state-machine.h"
#include <stdio.h>

Lexer lexer;
StateMachine *indentation_state_machine;
StateNode *current_indentation_state;
int indent_val = 0;

// gets the current character, and updates the position
char char_get() { return lexer.source[lexer.current++]; }

/* gets the lexeme, this will get freed in tokens_free() */
char *lexeme_get() {
    char *buffer = malloc(lexer.current - lexer.start);
    for (int i = lexer.start, idx = 0; i < lexer.current; i++, idx++) {
        buffer[idx] = lexer.source[i];
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

void token_add(char *lexeme) {
    if (lexer.token_memory < lexer.token_count + 1) {
        int old_memory = lexer.token_memory;
        lexer.token_memory = capacity_expand(old_memory);
        lexer.token_array =
            (Token *)memory_expand(sizeof(lexer.token_array), lexer.token_array,
                                   old_memory, lexer.token_memory);
    }

    Token token = {.key = lexer.token_count,
                   .token_type = lexer.current_state->output,
                   .col = lexer.current + 1,
                   .line = lexer.line};

    token.lexeme = malloc(strlen(lexeme) + 1);
    strcpy(token.lexeme, lexeme);

    lexer.token_array[lexer.token_count] = token; // add new token
    lexer.token_count++;
}

void indentation_check(char current_char) {
    StateNode *indent_next_state =
        transition_from(current_indentation_state, current_char);
    current_indentation_state = indent_next_state;

    if (current_indentation_state) {
        switch (current_indentation_state->output) {
            case 1:
                indent_val++;
                break;
            case T_INDENT:
                printf("Indent Value: %d\n", indent_val);
                indent_val = 0;
                break;
            default:
                break;
        }
    }
}

void lexer_initialize(char *src, StateMachine *state_machine) {
    lexer.source = src;
    lexer.state_machine = state_machine;
    lexer.line = 1;
    lexer.start = 0;
    lexer.current = 0;
    lexer.token_count = 0;
    lexer.token_memory = 0;
    lexer.token_array = NULL;
    lexer.current_state = NULL;

    indentation_state_machine = indentation_state_machine_init();
    current_indentation_state = indentation_state_machine->init_state;
}

void lexer_start() {
    lexer.current_state = lexer.state_machine->init_state;
    printf("##### STARTING LEXICAL ANALYSIS #####\n");

    char current_char = char_get();

    while (current_char != '\0') {

        /* -1 since lexer.current has already been incremented in char_get()
         */
        lexer.start = lexer.current - 1;

        for (;;) {

            /* check indentation */
            indentation_check(current_char);

            /* transition to the next state with the current input */
            StateNode *next_state =
                transition_from(lexer.current_state, current_char);

            /*
             * if we landed to an existing state, then get the next input
             * that will be used in the next iteration
             */

            if (next_state != NULL) {
                printf("| q%d -- %c --> q%d | \n", lexer.current_state->idx,
                       current_char, next_state->idx);
                current_char = char_get();
                lexer.current_state = next_state;
            }

            /**
             * if the next state is NULL, meaning the input stream ends here,
             * and we'll add a token depending on what state we finished
             */

            else {
                printf(" -> NULL\n");
                char *lexeme = lexeme_get();
                token_add(lexeme);
                printf("ADD TOKEN: %s -> %d\n", lexeme,
                       lexer.current_state->output);
                break;
            }
        }

        lexer.current_state = lexer.state_machine->init_state;
    }
}

void lexer_free() { tokens_free(lexer.token_array); }