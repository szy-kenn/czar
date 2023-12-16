#include "dstructs/fsm/fsutils.h"
#include <stdio.h>
#include <stdlib.h>

void main(void) {

    StateMachine *state_machine = malloc(sizeof(StateMachine));
    fsmachine_initialize(state_machine);

    /* keywords */

    char str[5] = "while";

    for (int i = 0; i < 6; i++) {
        (i == 5) ? fsmachine_state_add(state_machine, true)
                 : fsmachine_state_add(state_machine, false);
    }

    for (int i = 0; i < 5; i++) {

        char *tmp = malloc(2);
        *tmp = str[i];
        *(tmp + 1) = '\0';

        if (i < 4) {
            fsmachine_transition_add(state_machine, i, tmp, i + 1);
        } else {
            fsmachine_transition_add(state_machine, i, tmp, i + 1);
        }
    }

    char input[5] = "while";

    StateNode *tmp;
    for (int i = 0; i < 5; i++) {
        int old_idx;
        if (i == 0) {
            old_idx = state_machine->init_state->idx;
            tmp = transition_from(state_machine->init_state, input[i]);
        } else {
            old_idx = tmp->idx;
            tmp = transition_from(tmp, input[i]);
        }
        if (tmp != NULL) {
            printf("Input: %c ; q%d to q%d | %s\n", input[i], old_idx, tmp->idx,
                   (tmp->is_accepting_state ? "ACCEPTING" : "NOT ACCEPTING"));
        } else {
            printf("Input: %c ; q%d to NULL | %s\n", input[i], old_idx,
                   tmp->is_accepting_state ? "ACCEPTING" : "NOT ACCEPTING");
        }
    }

    fsmachine_print(state_machine);
    fsmachine_free(state_machine);
}