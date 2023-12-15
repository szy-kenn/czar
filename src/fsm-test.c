// #include "dstructs/fsm/fsmachine.h"
#include "dstructs/fsm/fselements.h"
#include <stdio.h>
#include <stdlib.h>

void main(void) {
    StateNode *sn1 = fsnode_create(1, S_START);
    StateNode *sn2 = fsnode_create(1, S_INSIDE);
    StateNode *sn3 = fsnode_create(1, S_FINAL);

    fsnode_add_transition(sn1, "1", sn2);
    fsnode_add_transition(sn1, "3", sn1);
    fsnode_add_transition(sn2, "2", sn3);

    fsnode_print(sn1);
    fsnode_print(sn2);
    fsnode_print(sn3);

    fsnode_free(sn1);
    fsnode_free(sn2);
    fsnode_free(sn3);
}