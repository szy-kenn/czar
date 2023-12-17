#include "fsutils.h"
#include <string.h>

StateNode *transition_from(StateNode *state_node, char input) {
    StateNode *current_node = state_node;
    Hashmap *deltaArray = current_node->delta;

    char tmp_chr[2] = {input, '\0'};
    return hashmap_get(deltaArray, tmp_chr);

    // if (tmp_sn != NULL) {
    //     return tmp_sn;
    // }

    // for (int i = 0; i < TABLE_SIZE; i++) {
    //     if (deltaArray->table[i] != NULL) {
    //         if (strchr(deltaArray->table[i]->key, input) != NULL) {
    //             return (StateNode *)deltaArray->table[i]->value;
    //         }
    //     }
    // }
}
