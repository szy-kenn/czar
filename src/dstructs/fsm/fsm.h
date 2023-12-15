#include "../../utils/utils.h"

typedef struct {
    State type;
    StateTransition **transitions;
} StateNode;

typedef struct {
    StateNode *next_state;
    char *inputs;
} StateTransition;

typedef struct {
    StateNode *init_state;
    StateNode **final_states;
    char *inputs;
} StateMachine;

StateMachine *fsm_initialize(StateNode *init_state, StateNode **final_states, char *inputs);
void *fsm_add_state(StateMachine *state_machine, StateNode *src_state, char *inputs,
                    StateNode *dest_state);