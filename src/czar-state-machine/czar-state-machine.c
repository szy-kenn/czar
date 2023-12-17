#include "czar-state-machine.h"

StateMachine *czar_state_machine_init() {

    StateMachine *state_machine = malloc(sizeof(StateMachine));
    fsmachine_initialize(state_machine);

    // Q0 (STARTING STATE)
    int start_idx = fsmachine_state_add(state_machine, false, T_ERROR);

    // Q1 (IDENTIFIER)
    int ident_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    // identifier state to identifier state (self-loop)
    fsmachine_transition_add(state_machine, ident_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ########## KEYWORDS ########## */

    /* ========== A ========== */
    int a_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int an_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int and_idx = fsmachine_state_add(state_machine, true, T_AND);
    int as_idx = fsmachine_state_add(state_machine, true, T_AS);

    /* Q0 -> a */
    fsmachine_transition_add(state_machine, start_idx, charset_create("a"),
                             a_idx);

    /* a -> identifier state */
    fsmachine_transition_add(state_machine, a_idx,
                             charset_excludes(IDENTIFIER_SET, "ns"), ident_idx);

    /* ===== and ===== */

    /* a -> an */
    fsmachine_transition_add(state_machine, a_idx, charset_create("n"), an_idx);

    /* an -> identifier state */
    fsmachine_transition_add(state_machine, an_idx,
                             charset_excludes(IDENTIFIER_SET, "d"), ident_idx);

    /* an -> and */
    fsmachine_transition_add(state_machine, an_idx, charset_create("d"),
                             and_idx);

    /* and -> identifier state */
    fsmachine_transition_add(state_machine, and_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* a -> as */
    fsmachine_transition_add(state_machine, a_idx, charset_create("s"), as_idx);

    /* as -> identifier state */
    fsmachine_transition_add(state_machine, as_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== B ========== */
    int b_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int by_idx = fsmachine_state_add(state_machine, true, T_BY);

    /* q0 - > b */
    fsmachine_transition_add(state_machine, start_idx, charset_create("b"),
                             a_idx);

    /* b -> identifier state */
    fsmachine_transition_add(state_machine, b_idx,
                             charset_excludes(IDENTIFIER_SET, "y"), ident_idx);

    /* ===== by ===== */

    /* b -> by */
    fsmachine_transition_add(state_machine, b_idx, charset_create("y"), by_idx);

    /* by -> identifier state */
    fsmachine_transition_add(state_machine, by_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== C ========== */
    int c_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int ch_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int chr_idx = fsmachine_state_add(state_machine, true, T_DTYPE);

    /* ===== chr ===== */

    /* q0 -> c */
    fsmachine_transition_add(state_machine, start_idx, charset_create("c"),
                             c_idx);

    /* c -> identifier state */
    fsmachine_transition_add(state_machine, c_idx,
                             charset_excludes(IDENTIFIER_SET, "h"), ident_idx);

    /* c -> ch */
    fsmachine_transition_add(state_machine, c_idx, charset_create("h"), ch_idx);

    /* ch -> identifier state */
    fsmachine_transition_add(state_machine, ch_idx,
                             charset_excludes(IDENTIFIER_SET, "r"), ident_idx);

    /* chr -> identifier state */
    fsmachine_transition_add(state_machine, chr_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ch -> chr */
    fsmachine_transition_add(state_machine, ch_idx, charset_create("r"),
                             chr_idx);

    /* ========== D ========== */
    int d_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int db_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int dbl_idx = fsmachine_state_add(state_machine, true, T_DTYPE);

    /* ===== dbl ===== */
    fsmachine_transition_add(state_machine, start_idx, charset_create("d"), d_idx);

    /* d -> identifier state */
    fsmachine_transition_add(state_machine, d_idx, charset_excludes(IDENTIFIER_SET, "b"),
                             ident_idx);

    /* d -> db */
    fsmachine_transition_add(state_machine, d_idx, charset_create("b"), db_idx);

    /* db -> identifier state */
    fsmachine_transition_add(state_machine, db_idx, charset_excludes(IDENTIFIER_SET, "l"),
                             ident_idx);

    /* db -> dbl */
    fsmachine_transition_add(state_machine, db_idx, charset_create("l"), dbl_idx);

    /* dbl -> identifier state */
    fsmachine_transition_add(state_machine, dbl_idx, charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== E ========== */
    int e_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int el_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int els_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int else_idx = fsmachine_state_add(state_machine, true, T_ELSE);

    int en_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int enu_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int enum_idx = fsmachine_state_add(state_machine, true, T_ENUM);

    /*  ===== else ===== */
    /* ===== enum ===== */

    /* ========== F ========== */
    int f_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int fa_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int fal_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int fals_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int false_idx = fsmachine_state_add(state_machine, true, T_FALSE);

    int fi_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int fix_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int fixe_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int fixed_idx = fsmachine_state_add(state_machine, true, T_FIXED);

    int fl_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int fle_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int flex_idx = fsmachine_state_add(state_machine, true, T_FLEX);

    /* ===== false ===== */
    /* ===== fixed ===== */
    /* ===== flex ===== */

    /* ========== G ========== */
    int g_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int gl_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int glo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int glob_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int globa_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int global_idx = fsmachine_state_add(state_machine, true, T_GLOBAL);

    /* ===== global ===== */

    /* ========== I ========== */
    int i_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int in_idx = fsmachine_state_add(state_machine, true, T_IN);

    int inp_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int inpu_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int input_idx = fsmachine_state_add(state_machine, true, T_INPUT);

    int int_idx = fsmachine_state_add(state_machine, true, T_INT);
    int is_idx = fsmachine_state_add(state_machine, true, T_IS);

    /* ===== in ===== */
    /* ===== input ===== */
    /* ===== int ===== */
    /* ===== is ===== */

    /* ========== L ========== */
    int l_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int lo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int loo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int loop_idx = fsmachine_state_add(state_machine, true, T_LOOP);

    /* ===== loop ===== */

    /* ========== N ========== */
    int n_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int ni_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int nil_idx = fsmachine_state_add(state_machine, true, T_NIL);

    int no_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int not_idx = fsmachine_state_add(state_machine, true, T_NOT);

    /* ===== nil ===== */
    /* ===== not ===== */

    /* ========== O ========== */
    int o_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int r_idx = fsmachine_state_add(state_machine, true, T_OR);

    int ou_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int out_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int outp_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int outpu_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int output_idx = fsmachine_state_add(state_machine, true, T_OUTPUT);

    /* ===== or ===== */
    /* ===== output ===== */

    /* ========== S ========== */

    int s_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int st_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int str_idx = fsmachine_state_add(state_machine, true, T_DTYPE);

    /* ===== str ===== */

    /* ========== T ========== */

    int t_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int th_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int the_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int then_idx = fsmachine_state_add(state_machine, true, T_THEN);

    int to_idx = fsmachine_state_add(state_machine, true, T_TO);

    int tr_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int tru_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int true_idx = fsmachine_state_add(state_machine, true, T_TRUE);

    /* ===== then ===== */
    /* ===== to ===== */
    /* ===== true ===== */

    /* ========== W ========== */

    int w_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int wh_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int whe_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int when_idx = fsmachine_state_add(state_machine, true, T_WHEN);

    int whi_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int whil_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int while_idx = fsmachine_state_add(state_machine, true, T_WHILE);

    /* ===== when ===== */
    /* ===== while ===== */

    /* ########## WHITESPACES ########## */
    int space_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    // fsmachine_print(state_machine);
    // fsmachine_free(state_machine);
    return state_machine;
}