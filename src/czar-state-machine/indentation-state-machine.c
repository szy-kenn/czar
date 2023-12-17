#include "czar-state-machine.h"

StateMachine *indentation_state_machine_init() {
    StateMachine *state_machine = malloc(sizeof(StateMachine));
    fsmachine_initialize(state_machine);

    // Q0 (STARTING STATE)
    int start_idx = fsmachine_state_add(state_machine, false, 0);
    int indentation_idx = fsmachine_state_add(state_machine, false, 1);
    int add_token_idx = fsmachine_state_add(state_machine, true, T_INDENT);
    int inside_idx = fsmachine_state_add(state_machine, false, 0);

    /* q0 - not space -> q0 */
    fsmachine_transition_add(state_machine, start_idx,
                             charset_excludes(CHARACTER_SET, " "), start_idx);

    /* q0 - space -> q1 */
    fsmachine_transition_add(state_machine, start_idx, charset_create(" "),
                             indentation_idx);

    /* q1 - space -> q1 */
    fsmachine_transition_add(state_machine, indentation_idx,
                             charset_create(" "), indentation_idx);

    /* q1 - \n -> q0 */
    fsmachine_transition_add(state_machine, indentation_idx,
                             charset_create("\n"), start_idx);

    /* q1 - not space or \n -> q2 */
    fsmachine_transition_add(state_machine, indentation_idx,
                             charset_excludes(CHARACTER_SET, " \n"),
                             add_token_idx);

    /* q2 - not \n -> q3 */
    fsmachine_transition_add(state_machine, add_token_idx,
                             charset_excludes(CHARACTER_SET, "\n"), inside_idx);

    /* q2 - \n -> q0 */
    fsmachine_transition_add(state_machine, add_token_idx, charset_create("\n"),
                             start_idx);

    /* q3 - not \n -> q3 */
    fsmachine_transition_add(state_machine, inside_idx,
                             charset_excludes(CHARACTER_SET, "\n"), inside_idx);

    /* q3 - \n -> q0 */
    fsmachine_transition_add(state_machine, inside_idx, charset_create("\n"),
                             start_idx);

    return state_machine;
}