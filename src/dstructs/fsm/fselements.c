#include "fselements.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

StateNode *fsnode_create(int idx, bool is_accepting_state) {

    StateNode *state_node = (StateNode *)malloc(sizeof(StateNode));
    state_node->idx = idx;
    state_node->delta = malloc(sizeof(Hashmap));
    hashmap_init(state_node->delta);
    state_node->is_accepting_state = is_accepting_state;
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
    hashmap_free(state_node->delta);
    free(state_node);
}
