#include "fselements.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

StateNode *fsnode_create(int state_type_count, ...) {

    StateNode *state_node = (StateNode *)malloc(sizeof(StateNode));
    State *types = (State *)malloc(sizeof(State) * state_type_count);

    va_list ptr;
    va_start(ptr, state_type_count);

    for (int i = 0; i < state_type_count; i++) {
        types[i] = va_arg(ptr, State);
    }

    va_end(ptr);

    state_node->delta = malloc(sizeof(Hashmap));
    hashmap_init(state_node->delta);
    state_node->state_type_count = state_type_count;
    state_node->types = types;
    return state_node;
}

void fsnode_add_transition(StateNode *state_node, char *inputs, StateNode *next_state) {
    hashmap_add(state_node->delta, inputs, next_state);
}

void fsnode_print(StateNode *state_node) {
    StateNode *current_state = state_node;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (current_state->delta->table[i] != NULL) {
            StateNode *tmp = current_state->delta->table[i]->value;
            printf("Q - %s -> Q\n", current_state->delta->table[i]->key);
        }
    }
}

void fsnode_free(StateNode *state_node) {
    free(state_node->types);
    hashmap_free(state_node->delta);
    free(state_node);
}
