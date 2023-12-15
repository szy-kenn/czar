#ifndef _CZAR_FSMACHINE_H_
#define _CZAR_FSMACHINE_H_

#include "../../utils/utils.h"
#include "fselements.h"

typedef struct {
    StateNode *init_state;
} StateMachine;

StateMachine *fsmachine_initialize(StateNode *init_state);
void *fsmachine_add_state(StateNode *src_state, char *inputs, StateNode *dest_state);
void fsmachine_print(StateMachine *state_machine);
void fsmachine_free(StateMachine *state_machine);

#endif