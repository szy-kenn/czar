#ifndef _CZAR_FSMACHINE_H_
#define _CZAR_FSMACHINE_H_

#include "../../utils/utils.h"
#include "fselements.h"

typedef struct {
    int state_count;
    int state_memory;
    StateNode *init_state;
    StateNode **states;
} StateMachine;

void fsmachine_initialize(StateMachine *state_machine);
void fsmachine_transition_add(StateMachine *state_machine, int current_state_idx, char *inputs,
                              int next_state_idx);
StateNode *fsmachine_state_add(StateMachine *state_machine, bool is_accepting_state);
StateNode *fsmachine_state_get(StateMachine *state_machine, int idx);
void fsmachine_print(StateMachine *state_machine);
void fsmachine_free(StateMachine *state_machine);

#endif