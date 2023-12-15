#include "fsmachine.h"
#include "../../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

void fsmachine_print(StateMachine *state_machine) {
    int count = 0;
    StateNode *current_state = state_machine->init_state;

    while (current_state != NULL) {
        printf("q%d: %p", count++, &current_state);
        current_state = NULL;
    }
}

StateMachine *fsmachine_initialize(StateNode *init_state) {
    StateMachine *state_machine = (StateMachine *)malloc(sizeof(StateMachine));
    state_machine->init_state = init_state;
    return state_machine;
}

void fsmachine_free(StateMachine *state_machine) { free(state_machine); }
