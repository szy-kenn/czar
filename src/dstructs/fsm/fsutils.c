#include "fsutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StateNode *transition_from(StateNode *state_node, char input) {
    StateNode *current_node = state_node;
    Hashmap *deltaArray = current_node->delta;

    if (input == '\0') {
        char *tmp = malloc(10);
        tmp = "#00000000";
        return hashmap_get(deltaArray, tmp);
    } else {
        char tmp_chr[2] = {input, '\0'};
        return hashmap_get(deltaArray, tmp_chr);
    }
}

// returns q1