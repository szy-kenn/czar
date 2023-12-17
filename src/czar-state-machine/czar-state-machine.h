#ifndef _CZAR_STATE_MACHINE_H_
#define _CZAR_STATE_MACHINE_H_

#include "../charset/charset.h"
#include "../dstructs/fsm/fsutils.h"
#include "../lexer/lexer.h"
#include <stdlib.h>

StateMachine *czar_state_machine_init();
StateMachine *indentation_state_machine_init();

#endif