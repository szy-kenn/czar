#include "fsutils.h"
#include <string.h>

StateNode *transition_from(StateNode *state_node, char input) {
    StateNode *current_node = state_node;
    Hashmap *deltaArray = current_node->delta;

    char tmp_chr[2] = {input, '\0'};
    return hashmap_get(deltaArray, tmp_chr);
}
