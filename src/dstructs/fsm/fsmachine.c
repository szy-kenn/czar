#include "fsmachine.h"
#include "../../memory_manager/memory_manager.h"
#include "../../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _fsmachine_print(StateNode *state_node) {
    StateNode *current_state = state_node;

    if (current_state != NULL) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Pair *tmp_p = current_state->delta->table[i];
            if (tmp_p != NULL) {
                StateNode *tmp_sn = (StateNode *)tmp_p->value;

                /* current state */
                (current_state->is_accepting_state
                     ? printf("[q%d] - ", current_state->idx)
                     : printf("q%d - ", current_state->idx));

                /* accepted input */
                printf("%s - ", tmp_p->key);

                /* next state */
                if (tmp_sn != NULL) {
                    (tmp_sn->is_accepting_state ? printf("[q%d]\n", tmp_sn->idx)
                                                : printf("q%d\n", tmp_sn->idx));
                    if (tmp_sn->idx > current_state->idx) {
                        _fsmachine_print(tmp_sn);
                    }
                } else {
                    printf("NULL\n");
                }
            }
        }
    }
}

void fsmachine_print(StateMachine *state_machine) {
    StateNode *current_state = state_machine->init_state;
    printf("-> ");
    _fsmachine_print(current_state);
}

static void _fsmachine_free(StateNode *state_node) {
    StateNode *current_state = state_node;

    if (current_state != NULL) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Pair *tmp_p = current_state->delta->table[i];
            if (tmp_p != NULL) {
                StateNode *tmp_sn = (StateNode *)tmp_p->value;
                fsnode_free(current_state);
                if (tmp_sn != NULL && tmp_sn->idx > current_state->idx) {
                    _fsmachine_free(tmp_sn);
                }
            } else {
                fsnode_free(current_state);
                break;
            }
        }
    }
}

void fsmachine_free(StateMachine *state_machine) {
    StateNode *current_state = state_machine->init_state;
    _fsmachine_free(current_state);
    memory_free(sizeof(state_machine->states), state_machine->states,
                state_machine->state_memory);
    state_machine->states = NULL;
    free(state_machine);
}

StateNode *fsmachine_state_get(StateMachine *state_machine, int idx) {
    if (idx >= 0 && idx <= state_machine->state_count) {
        return state_machine->states[idx];
    }
    return NULL;
}

int fsmachine_state_add(StateMachine *state_machine, bool is_accepting_state,
                        int output) {

    if (state_machine->state_memory < state_machine->state_count + 1) {
        int old_memory = state_machine->state_memory;
        state_machine->state_memory = capacity_expand(old_memory);
        state_machine->states = (StateNode **)memory_expand(
            sizeof(StateNode *), state_machine->states, old_memory,
            state_machine->state_memory);
    }

    state_machine->states[state_machine->state_count] =
        fsnode_create(state_machine->state_count, is_accepting_state, output);

    if (state_machine->state_count == 0) {
        state_machine->init_state =
            state_machine->states[state_machine->state_count];
    }

    state_machine->state_count++;
    return state_machine->state_count - 1;
}

void fsmachine_null_terminator_transition_add(StateMachine *state_machine,
                                              int current_state_idx,
                                              int next_state_idx) {
    StateNode *current_state, *next_state;
    current_state = fsmachine_state_get(state_machine, current_state_idx);
    next_state = fsmachine_state_get(state_machine, next_state_idx);
    fsnode_null_terminator_add_transition(current_state, next_state);
}

/* pass a dynamically allocated char *inputs */
void fsmachine_transition_add(StateMachine *state_machine,
                              int current_state_idx, char *inputs,
                              int next_state_idx) {
    StateNode *current_state, *next_state;
    current_state = fsmachine_state_get(state_machine, current_state_idx);
    next_state = fsmachine_state_get(state_machine, next_state_idx);

    int current = 0;

    while (current != strlen(inputs)) {
        char *tmp;
        tmp = malloc(2);
        *tmp = inputs[current];
        *(tmp + 1) = '\0';
        fsnode_add_transition(current_state, tmp, next_state);
        current++;
    }

    printf("Added q%d - %s -> q%d\n", current_state_idx, inputs,
           next_state_idx);
    free(inputs);
}

void fsmachine_initialize(StateMachine *state_machine) {
    state_machine->state_count = 0;
    state_machine->state_memory = 0;
    state_machine->init_state = NULL;
    state_machine->states = NULL;
}
