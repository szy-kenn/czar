#ifndef _CZAR_FSELEMENTS_H_
#define _CZAR_FSELEMENTS_H_

#include "../../utils/utils.h"
#include "../hashmap/hashmap.h"

typedef struct state_node_t StateNode;

typedef struct state_node_t {
    int idx;
    bool is_accepting_state;
    int output;
    Hashmap *delta;
} StateNode;

void fsnode_print(StateNode *state_node);
StateNode *fsnode_create(int idx, bool is_accepting_state, int output);
void fsnode_free(StateNode *state_node);
void fsnode_add_transition(StateNode *state_node, char *inputs, StateNode *next_state);

#endif