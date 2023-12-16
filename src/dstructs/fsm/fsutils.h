#ifndef _CZAR_FSUTILS_H_
#define _CZAR_FSUTILS_H_

#include "fselements.h"
#include "fsmachine.h"

StateNode *transition_from(StateNode *state_node, char input);
#endif