#include "czar-state-machine.h"

StateMachine *czar_state_machine_init() {

    StateMachine *state_machine = malloc(sizeof(StateMachine));
    fsmachine_initialize(state_machine);

    /* Q0 (STARTING STATE) */
    int start_idx = fsmachine_state_add(state_machine, false, T_ERROR);

    /* Q1 (IDENTIFIER) */
    int ident_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    /* Q2 (INVALID STATE) - unrecognized token goes here */
    int invalid_idx = fsmachine_state_add(state_machine, true, T_INVALID);
    fsmachine_transition_add(state_machine, start_idx,
                             charset_create("#$&{};\\~`"),
                             invalid_idx);

    /* start -> identifier state */
    fsmachine_transition_add(
        state_machine, start_idx,
        charset_excludes(IDENTIFIER_SET, "abcdefgilnostw0123456789"),
        ident_idx);

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
    int bo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int boo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int bool_idx = fsmachine_state_add(state_machine, true, T_BOOL);

    /* q0 - > b */
    fsmachine_transition_add(state_machine, start_idx, charset_create("b"),
                             b_idx);

    /* b -> identifier state */
    fsmachine_transition_add(state_machine, b_idx,
                             charset_excludes(IDENTIFIER_SET, "oy"), ident_idx);

    /* ===== by ===== */

    /* b -> by */
    fsmachine_transition_add(state_machine, b_idx, charset_create("y"), by_idx);

    /* by -> identifier state */
    fsmachine_transition_add(state_machine, by_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* b -> bo */
    fsmachine_transition_add(state_machine, b_idx, charset_create("o"), bo_idx);

    /* bo --> boo*/
    fsmachine_transition_add(state_machine, bo_idx, charset_create("o"),
                             boo_idx);
    fsmachine_transition_add(state_machine, bo_idx,
                             charset_excludes(IDENTIFIER_SET, "o"), ident_idx);

    /* boo --> bool */
    fsmachine_transition_add(state_machine, boo_idx, charset_create("l"),
                             bool_idx);
    fsmachine_transition_add(state_machine, boo_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);

    /* bool -> identifier */
    fsmachine_transition_add(state_machine, bool_idx,
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
    fsmachine_transition_add(state_machine, start_idx, charset_create("d"),
                             d_idx);

    /* d -> identifier state */
    fsmachine_transition_add(state_machine, d_idx,
                             charset_excludes(IDENTIFIER_SET, "b"), ident_idx);

    /* d -> db */
    fsmachine_transition_add(state_machine, d_idx, charset_create("b"), db_idx);

    /* db -> identifier state */
    fsmachine_transition_add(state_machine, db_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);

    /* db -> dbl */
    fsmachine_transition_add(state_machine, db_idx, charset_create("l"),
                             dbl_idx);

    /* dbl -> identifier state */
    fsmachine_transition_add(state_machine, dbl_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== E ========== */
    int e_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int el_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int els_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int else_idx = fsmachine_state_add(state_machine, true, T_ELSE);

    int en_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int enu_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int enum_idx = fsmachine_state_add(state_machine, true, T_ENUM);

    fsmachine_transition_add(state_machine, start_idx, charset_create("e"),
                             e_idx);

    fsmachine_transition_add(state_machine, e_idx,
                             charset_excludes(IDENTIFIER_SET, "ln"), ident_idx);

    /*  ===== else ===== */

    /* e --> el */
    fsmachine_transition_add(state_machine, e_idx, charset_create("l"), el_idx);

    /* el --> els */
    fsmachine_transition_add(state_machine, el_idx, charset_create("s"),
                             els_idx);

    /* el --> identifier */
    fsmachine_transition_add(state_machine, el_idx,
                             charset_excludes(IDENTIFIER_SET, "s"), ident_idx);

    /* els --> else */
    fsmachine_transition_add(state_machine, els_idx, charset_create("e"),
                             else_idx);

    /* els --> identifier */
    fsmachine_transition_add(state_machine, els_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /* else --> identifier */
    fsmachine_transition_add(state_machine, else_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== enum ===== */

    /* e --> en */
    fsmachine_transition_add(state_machine, e_idx, charset_create("n"), en_idx);

    /* en -> enu */
    fsmachine_transition_add(state_machine, en_idx, charset_create("u"),
                             enu_idx);

    /* en -> identifier */
    fsmachine_transition_add(state_machine, en_idx,
                             charset_excludes(IDENTIFIER_SET, "u"), ident_idx);

    /* enu -> enum */
    fsmachine_transition_add(state_machine, enu_idx, charset_create("m"),
                             enum_idx);

    /* enu -> identifier */
    fsmachine_transition_add(state_machine, enu_idx,
                             charset_excludes(IDENTIFIER_SET, "m"), ident_idx);

    /* enum -> identifier */
    fsmachine_transition_add(state_machine, enum_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

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

    /* q0 --> f */
    fsmachine_transition_add(state_machine, start_idx, charset_create("f"),
                             f_idx);
    /* f --> Identifier state */
    fsmachine_transition_add(state_machine, f_idx,
                             charset_excludes(IDENTIFIER_SET, "ail"),
                             ident_idx);

    /* ===== false ===== */

    /* f --> fa */
    fsmachine_transition_add(state_machine, f_idx, charset_create("a"), fa_idx);

    /* fa --> fal */
    fsmachine_transition_add(state_machine, fa_idx, charset_create("l"),
                             fal_idx);

    /* fa --> identifier */
    fsmachine_transition_add(state_machine, fa_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);

    /* fal --> fals */
    fsmachine_transition_add(state_machine, fal_idx, charset_create("s"),
                             fals_idx);

    /* fal --> identifier */
    fsmachine_transition_add(state_machine, fal_idx,
                             charset_excludes(IDENTIFIER_SET, "s"), ident_idx);

    /* fals --> false */
    fsmachine_transition_add(state_machine, fals_idx, charset_create("e"),
                             false_idx);

    /* fals --> identifier */
    fsmachine_transition_add(state_machine, fals_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /* false --> identifier */
    fsmachine_transition_add(state_machine, false_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== fixed ===== */

    /* f --> fi */
    fsmachine_transition_add(state_machine, f_idx, charset_create("i"), fi_idx);

    /* fi -> fix */
    fsmachine_transition_add(state_machine, fi_idx, charset_create("x"),
                             fix_idx);

    /* fi -> identifier */
    fsmachine_transition_add(state_machine, fi_idx,
                             charset_excludes(IDENTIFIER_SET, "x"), ident_idx);

    /* fix --> fixe */
    fsmachine_transition_add(state_machine, fix_idx, charset_create("e"),
                             fixe_idx);

    /* fix --> identifier */
    fsmachine_transition_add(state_machine, fix_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /* fixe --> fixed */
    fsmachine_transition_add(state_machine, fixe_idx, charset_create("d"),
                             fixed_idx);

    /* fixe --> identifier */
    fsmachine_transition_add(state_machine, fixe_idx,
                             charset_excludes(IDENTIFIER_SET, "d"), ident_idx);

    /* fixed --> identifier */
    fsmachine_transition_add(state_machine, fixed_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== flex ===== */

    /* f --> fl */
    fsmachine_transition_add(state_machine, f_idx, charset_create("l"), fl_idx);

    /* fl -> fle */
    fsmachine_transition_add(state_machine, fl_idx, charset_create("e"),
                             fle_idx);

    /* fl -> identifier */
    fsmachine_transition_add(state_machine, fl_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /* fle -> flex */
    fsmachine_transition_add(state_machine, fle_idx, charset_create("x"),
                             flex_idx);

    /* fle -> identifier */
    fsmachine_transition_add(state_machine, fle_idx,
                             charset_excludes(IDENTIFIER_SET, "x"), ident_idx);

    /* flex -> identifier */
    fsmachine_transition_add(state_machine, flex_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== G ========== */
    int g_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int gl_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int glo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int glob_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int globa_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int global_idx = fsmachine_state_add(state_machine, true, T_GLOBAL);

    /* ===== global ===== */

    /* q0 --> g */
    fsmachine_transition_add(state_machine, start_idx, charset_create("g"),
                             g_idx);

    /* g --> gl */
    fsmachine_transition_add(state_machine, g_idx, charset_create("l"), gl_idx);

    /* g -> identifier */
    fsmachine_transition_add(state_machine, g_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);

    /* gl -> glo */
    fsmachine_transition_add(state_machine, gl_idx, charset_create("o"),
                             glo_idx);

    /* gl -> identifier */
    fsmachine_transition_add(state_machine, gl_idx,
                             charset_excludes(IDENTIFIER_SET, "o"), ident_idx);

    /* glo -> glob */
    fsmachine_transition_add(state_machine, glo_idx, charset_create("b"),
                             glob_idx);

    /* glo -> identifier */
    fsmachine_transition_add(state_machine, glo_idx,
                             charset_excludes(IDENTIFIER_SET, "b"), ident_idx);

    /* glob -> globa */
    fsmachine_transition_add(state_machine, glob_idx, charset_create("a"),
                             globa_idx);

    /* glob -> identifier */
    fsmachine_transition_add(state_machine, glob_idx,
                             charset_excludes(IDENTIFIER_SET, "a"), ident_idx);

    /* globa -> global */
    fsmachine_transition_add(state_machine, globa_idx, charset_create("l"),
                             global_idx);

    /* globa -> identifier */
    fsmachine_transition_add(state_machine, globa_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);

    /* global -> identifier */
    fsmachine_transition_add(state_machine, global_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== I ========== */
    int i_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int in_idx = fsmachine_state_add(state_machine, true, T_IN);

    int inp_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int inpu_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int input_idx = fsmachine_state_add(state_machine, true, T_INPUT);

    int int_idx = fsmachine_state_add(state_machine, true, T_INT);
    int is_idx = fsmachine_state_add(state_machine, true, T_IS);

    /* q0 --> i */
    fsmachine_transition_add(state_machine, start_idx, charset_create("i"),
                             i_idx);
    /* i --> Identifier state */
    fsmachine_transition_add(state_machine, i_idx,
                             charset_excludes(IDENTIFIER_SET, "ns"), ident_idx);
    /* in --> Identifier state */
    fsmachine_transition_add(state_machine, in_idx,
                             charset_excludes(IDENTIFIER_SET, "pt"), ident_idx);
    /* ===== in ===== */
    /*  i --> n */
    fsmachine_transition_add(state_machine, i_idx, charset_create("n"), in_idx);

    /* ===== input ===== */
    /* in --> p */
    fsmachine_transition_add(state_machine, in_idx, charset_create("p"),
                             inp_idx);
    /* inp --> u */
    fsmachine_transition_add(state_machine, inp_idx, charset_create("u"),
                             inpu_idx);
    /* inp -> identifier */
    fsmachine_transition_add(state_machine, inp_idx,
                             charset_excludes(IDENTIFIER_SET, "u"), ident_idx);
    /* inpu --> t */
    fsmachine_transition_add(state_machine, inpu_idx, charset_create("t"),
                             input_idx);
    /* inpu --> identifier */
    fsmachine_transition_add(state_machine, inpu_idx,
                             charset_excludes(IDENTIFIER_SET, "t"), ident_idx);
    /* input --> identifier */
    fsmachine_transition_add(state_machine, input_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);
    /* ===== int ===== */
    /* in --> t */
    fsmachine_transition_add(state_machine, in_idx, charset_create("t"),
                             int_idx);
    /* int --> identifier */
    fsmachine_transition_add(state_machine, int_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);
    /* ===== is ===== */
    fsmachine_transition_add(state_machine, i_idx, charset_create("s"), is_idx);
    /* is --> identifier */
    fsmachine_transition_add(state_machine, is_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== L ========== */
    int l_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int lo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int loo_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int loop_idx = fsmachine_state_add(state_machine, true, T_LOOP);

    /* ===== loop ===== */
    /* q0 --> l */
    fsmachine_transition_add(state_machine, start_idx, charset_create("l"),
                             l_idx);
    /*  l --> o */
    fsmachine_transition_add(state_machine, l_idx, charset_create("o"), lo_idx);
    /* l --> identifier */
    fsmachine_transition_add(state_machine, l_idx,
                             charset_excludes(IDENTIFIER_SET, "o"), ident_idx);
    /* lo --> o */
    fsmachine_transition_add(state_machine, lo_idx, charset_create("o"),
                             loo_idx);
    /* lo --> identifier */
    fsmachine_transition_add(state_machine, lo_idx,
                             charset_excludes(IDENTIFIER_SET, "o"), ident_idx);
    /* loo --> p */
    fsmachine_transition_add(state_machine, loo_idx, charset_create("p"),
                             loop_idx);
    /* loo --> identifier */
    fsmachine_transition_add(state_machine, loo_idx,
                             charset_excludes(IDENTIFIER_SET, "p"), ident_idx);
    /* loop --> identifier */
    fsmachine_transition_add(state_machine, loop_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== N ========== */
    int n_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int ni_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int nil_idx = fsmachine_state_add(state_machine, true, T_NIL);

    int no_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int not_idx = fsmachine_state_add(state_machine, true, T_NOT);
    /* q0 --> n */
    fsmachine_transition_add(state_machine, start_idx, charset_create("n"),
                             n_idx);
    /* n --> Identifier state */
    fsmachine_transition_add(state_machine, n_idx,
                             charset_excludes(IDENTIFIER_SET, "io"), ident_idx);

    /* ===== nil ===== */
    /* n --> i */
    fsmachine_transition_add(state_machine, n_idx, charset_create("i"), ni_idx);
    /* ni --> l */
    fsmachine_transition_add(state_machine, ni_idx, charset_create("l"),
                             nil_idx);
    /* ni --> identifier set */
    fsmachine_transition_add(state_machine, ni_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);
    /* nil --> identifier set */
    fsmachine_transition_add(state_machine, nil_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== not ===== */
    /* n --> o */
    fsmachine_transition_add(state_machine, n_idx, charset_create("o"), no_idx);
    /* no --> t */
    fsmachine_transition_add(state_machine, no_idx, charset_create("t"),
                             not_idx);
    /* no --> identifier set */
    fsmachine_transition_add(state_machine, no_idx,
                             charset_excludes(IDENTIFIER_SET, "t"), ident_idx);
    /* not --> identifier set */
    fsmachine_transition_add(state_machine, not_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);
    /* ========== O ========== */
    int o_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int or_idx = fsmachine_state_add(state_machine, true, T_OR);

    int ou_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int out_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int outp_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int outpu_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int output_idx = fsmachine_state_add(state_machine, true, T_OUTPUT);

    /*q0 -> o*/
    fsmachine_transition_add(state_machine, start_idx, charset_create("o"),
                             o_idx);

    /*o -> identifier*/
    fsmachine_transition_add(state_machine, o_idx,
                             charset_excludes(IDENTIFIER_SET, "ru"), ident_idx);

    /* ===== or ===== */

    /*o -> or*/
    fsmachine_transition_add(state_machine, o_idx, charset_create("r"), or_idx);

    /*or -> identifier*/
    fsmachine_transition_add(state_machine, or_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== output ===== */

    /*o -> ou*/
    fsmachine_transition_add(state_machine, o_idx, charset_create("u"), ou_idx);

    /*ou -> out*/
    fsmachine_transition_add(state_machine, ou_idx, charset_create("t"),
                             out_idx);

    /*ou -> identifier*/
    fsmachine_transition_add(state_machine, ou_idx,
                             charset_excludes(IDENTIFIER_SET, "t"), ident_idx);

    /*out -> outp*/
    fsmachine_transition_add(state_machine, out_idx, charset_create("p"),
                             outp_idx);

    /*out -> identifier*/
    fsmachine_transition_add(state_machine, out_idx,
                             charset_excludes(IDENTIFIER_SET, "p"), ident_idx);

    /*outp -> outpu*/
    fsmachine_transition_add(state_machine, outp_idx, charset_create("u"),
                             outpu_idx);

    /*outp -> identifier*/
    fsmachine_transition_add(state_machine, outp_idx,
                             charset_excludes(IDENTIFIER_SET, "u"), ident_idx);

    /*outpu -> output*/
    fsmachine_transition_add(state_machine, outpu_idx, charset_create("t"),
                             output_idx);

    /*outpu -> identifier*/
    fsmachine_transition_add(state_machine, outpu_idx,
                             charset_excludes(IDENTIFIER_SET, "t"), ident_idx);

    /*output -> identifier*/
    fsmachine_transition_add(state_machine, output_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== S ========== */

    int s_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int st_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int str_idx = fsmachine_state_add(state_machine, true, T_DTYPE);

    /* ===== str ===== */

    /*q0 -> s*/
    fsmachine_transition_add(state_machine, start_idx, charset_create("s"),
                             s_idx);

    /*s -> st*/
    fsmachine_transition_add(state_machine, s_idx, charset_create("t"), st_idx);

    /*s -> identifier*/
    fsmachine_transition_add(state_machine, s_idx,
                             charset_excludes(IDENTIFIER_SET, "t"), ident_idx);

    /*st -> str*/
    fsmachine_transition_add(state_machine, st_idx, charset_create("r"),
                             str_idx);

    /*st -> identifier*/
    fsmachine_transition_add(state_machine, st_idx,
                             charset_excludes(IDENTIFIER_SET, "r"), ident_idx);

    /*str -> identifier*/
    fsmachine_transition_add(state_machine, str_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== T ========== */

    int t_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int th_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int the_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int then_idx = fsmachine_state_add(state_machine, true, T_THEN);

    int to_idx = fsmachine_state_add(state_machine, true, T_TO);

    int tr_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int tru_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int true_idx = fsmachine_state_add(state_machine, true, T_TRUE);

    /*q0 -> t*/
    fsmachine_transition_add(state_machine, start_idx, charset_create("t"),
                             t_idx);

    /*t -> identifier*/
    fsmachine_transition_add(state_machine, t_idx,
                             charset_excludes(IDENTIFIER_SET, "hor"),
                             ident_idx);

    /* ===== then ===== */

    /*t -> th*/
    fsmachine_transition_add(state_machine, t_idx, charset_create("h"), th_idx);

    /*th -> the*/
    fsmachine_transition_add(state_machine, th_idx, charset_create("e"),
                             the_idx);

    /*th -> identifier*/
    fsmachine_transition_add(state_machine, th_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /*the -> then*/
    fsmachine_transition_add(state_machine, the_idx, charset_create("n"),
                             then_idx);

    /*the -> identifier*/
    fsmachine_transition_add(state_machine, the_idx,
                             charset_excludes(IDENTIFIER_SET, "n"), ident_idx);

    /*then -> identifier*/
    fsmachine_transition_add(state_machine, then_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== to ===== */

    /*t -> to*/
    fsmachine_transition_add(state_machine, t_idx, charset_create("o"), to_idx);

    /*to -> identifier*/
    fsmachine_transition_add(state_machine, to_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== true ===== */

    /*t -> tr*/
    fsmachine_transition_add(state_machine, t_idx, charset_create("r"), tr_idx);

    /*tr -> tru*/
    fsmachine_transition_add(state_machine, tr_idx, charset_create("u"),
                             tru_idx);

    /*tr -> identifier*/
    fsmachine_transition_add(state_machine, tr_idx,
                             charset_excludes(IDENTIFIER_SET, "u"), ident_idx);

    /*tru -> true*/
    fsmachine_transition_add(state_machine, tru_idx, charset_create("e"),
                             true_idx);

    /*tru -> identifier*/
    fsmachine_transition_add(state_machine, tru_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /*true -> identifier*/
    fsmachine_transition_add(state_machine, true_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ========== W ========== */

    int w_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int wh_idx = fsmachine_state_add(state_machine, true, T_IDENT);

    int whe_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int when_idx = fsmachine_state_add(state_machine, true, T_WHEN);

    int whi_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int whil_idx = fsmachine_state_add(state_machine, true, T_IDENT);
    int while_idx = fsmachine_state_add(state_machine, true, T_WHILE);

    /* q0 --> w */
    fsmachine_transition_add(state_machine, start_idx, charset_create("w"),
                             w_idx);
    /* w --> Identifier state */
    fsmachine_transition_add(state_machine, w_idx,
                             charset_excludes(IDENTIFIER_SET, "h"), ident_idx);

    /* ===== when ===== */

    /* w --> wh */
    fsmachine_transition_add(state_machine, w_idx, charset_create("h"), wh_idx);

    /* wh --> whe */
    fsmachine_transition_add(state_machine, wh_idx, charset_create("e"),
                             whe_idx);

    /* wh --> identifier */
    fsmachine_transition_add(state_machine, wh_idx,
                             charset_excludes(IDENTIFIER_SET, "ei"), ident_idx);

    /* whe --> when */
    fsmachine_transition_add(state_machine, whe_idx, charset_create("n"),
                             when_idx);

    /* whe --> identifier */
    fsmachine_transition_add(state_machine, whe_idx,
                             charset_excludes(IDENTIFIER_SET, "n"), ident_idx);

    /* when --> identifier */
    fsmachine_transition_add(state_machine, when_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ===== while ===== */

    /* wh --> whi */
    fsmachine_transition_add(state_machine, wh_idx, charset_create("i"),
                             whi_idx);

    /* whi --> whil */
    fsmachine_transition_add(state_machine, whi_idx, charset_create("l"),
                             whil_idx);

    /* whi --> identifier */
    fsmachine_transition_add(state_machine, whi_idx,
                             charset_excludes(IDENTIFIER_SET, "l"), ident_idx);

    /* whil --> while */
    fsmachine_transition_add(state_machine, whil_idx, charset_create("e"),
                             while_idx);

    /* whil --> identifier */
    fsmachine_transition_add(state_machine, whil_idx,
                             charset_excludes(IDENTIFIER_SET, "e"), ident_idx);

    /* while --> identifier */
    fsmachine_transition_add(state_machine, while_idx,
                             charset_create(IDENTIFIER_SET), ident_idx);

    /* ########## FLOAT ########## */

    int digit_idx = fsmachine_state_add(state_machine, true, T_INT);
    int decimal_point_idx =
        fsmachine_state_add(state_machine, false, T_INVALID);
    int decimal_digit_idx = fsmachine_state_add(state_machine, true, T_DBL);
    int dead_decimal_point_idx =
        fsmachine_state_add(state_machine, false, T_ERROR);

    /* q0 --> digits */
    fsmachine_transition_add(state_machine, start_idx, charset_create(DIGITS),
                             digit_idx);

    /* q0 --> . */
    fsmachine_transition_add(state_machine, start_idx, charset_create("."),
                             decimal_point_idx);

    /* digits loop */
    fsmachine_transition_add(state_machine, digit_idx, charset_create(DIGITS),
                             digit_idx);

    /* digits --> . */
    fsmachine_transition_add(state_machine, digit_idx, charset_create("."),
                             decimal_point_idx);

    /* . --> decimal digits */
    fsmachine_transition_add(state_machine, decimal_point_idx,
                             charset_create(DIGITS), decimal_digit_idx);               

    /* decimal digits loop */
    fsmachine_transition_add(state_machine, decimal_digit_idx,
                             charset_create(DIGITS), decimal_digit_idx);

    /* decimal digit --> . */
    fsmachine_transition_add(state_machine, decimal_digit_idx,
                             charset_create("."), dead_decimal_point_idx);

    /* . --> dead state */
    fsmachine_transition_add(state_machine, decimal_point_idx,
                             charset_create("."), dead_decimal_point_idx);

    /* ===== DEAD decimal point loop ===== */
    fsmachine_transition_add(state_machine, dead_decimal_point_idx,
                             charset_includes(DIGITS, "."),
                             dead_decimal_point_idx);

    /* ########## STRING ########## */

    int left_dquotation_idx =
        fsmachine_state_add(state_machine, false, T_ERROR);
    int string_idx = fsmachine_state_add(state_machine, false, T_ERROR);
    int right_dquotation_idx = fsmachine_state_add(state_machine, true, T_STR);
    int string_error_idx = fsmachine_state_add(state_machine, true, T_ERROR);

    /* q0 --> " */
    fsmachine_transition_add(state_machine, start_idx, charset_create("\""),
                             left_dquotation_idx);

    /* " --> " */
    fsmachine_transition_add(state_machine, left_dquotation_idx,
                             charset_create("\n\""), string_error_idx);

    /* " --> string */
    fsmachine_transition_add(state_machine, left_dquotation_idx,
                             charset_excludes(STRING_SET, "\n\""), string_idx);

    /* string loop */
    fsmachine_transition_add(state_machine, string_idx,
                             charset_excludes(STRING_SET, "\n\""), string_idx);

    /* " --> new line */
    fsmachine_transition_add(state_machine, string_idx, charset_create("\n"),
                             string_error_idx);

    /* string --> " */
    fsmachine_transition_add(state_machine, string_idx, charset_create("\""),
                             right_dquotation_idx);

    // fsmachine_null_terminator_transition_add(state_machine, string_idx,
    //                                          string_error_idx);

    /* ########## CHARACTER ########## */

    int left_squotation_idx =
        fsmachine_state_add(state_machine, false, T_ERROR);
    int character_idx = fsmachine_state_add(state_machine, false, T_ERROR);
    int right_squotation_idx = fsmachine_state_add(state_machine, true, T_CHR);
    int character_error_idx = fsmachine_state_add(state_machine, true, T_ERROR);

    /* q0 --> ' */
    fsmachine_transition_add(state_machine, start_idx, charset_create("'"),
                             left_squotation_idx);

    /* ' --> ' */
    fsmachine_transition_add(state_machine, left_squotation_idx,
                             charset_create("\n'"), character_error_idx);

    /* ' --> char */
    fsmachine_transition_add(state_machine, left_squotation_idx,
                             charset_excludes(STRING_SET, "\n'"),
                             character_idx);

    /* char --> not ' */
    fsmachine_transition_add(state_machine, character_idx,
                             charset_excludes(STRING_SET, "'"),
                             character_error_idx);

    /* char --> ' */
    fsmachine_transition_add(state_machine, character_idx, charset_create("'"),
                             right_squotation_idx);

    /* ########## ARRAY ########## */

    int left_bracket_idx = fsmachine_state_add(state_machine, true, T_LBRACKET);
    int right_bracket_idx =
        fsmachine_state_add(state_machine, true, T_RBRACKET);
    int array_error = fsmachine_state_add(state_machine, true, T_ERROR);

    /* q0 --> [ */
    fsmachine_transition_add(state_machine, start_idx, charset_create("["),
                             left_bracket_idx);

    /* [ --> ] */
    fsmachine_transition_add(state_machine, start_idx, charset_create("]"),
                             right_bracket_idx);

    /* ===== ARRAY ERROR  ===== */
    // fsmachine_transition_add(state_machine, left_bracket_idx,
    //                          charset_create("]"), array_error);
    // fsmachine_transition_add(state_machine, left_bracket_idx,
    //                          charset_excludes(IDENTIFIER_SET, "cdis"),
    //                          array_error);
    // /* ===== CHAR ARRAY ===== */
    // int chr_arr_c_idx = fsmachine_state_add(state_machine, false, T_CHR);
    // int chr_arr_h_idx = fsmachine_state_add(state_machine, false, T_CHR);
    // int chr_arr_r_idx = fsmachine_state_add(state_machine, false, T_CHR);
    // fsmachine_transition_add(state_machine, left_bracket_idx,
    //                          charset_create("c"), chr_arr_c_idx);
    // fsmachine_transition_add(state_machine, chr_arr_c_idx,
    // charset_create("h"),
    //                          chr_arr_h_idx);
    // fsmachine_transition_add(state_machine, chr_arr_h_idx,
    // charset_create("r"),
    //                          chr_arr_r_idx);
    // fsmachine_transition_add(state_machine, chr_arr_r_idx,
    // charset_create("]"),
    //                          right_bracket_idx);
    // /* ===== chr array error  ===== */
    // fsmachine_transition_add(state_machine, chr_arr_c_idx,
    //                          charset_excludes(IDENTIFIER_SET, "h"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, chr_arr_h_idx,
    //                          charset_excludes(IDENTIFIER_SET, "r"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, chr_arr_r_idx,
    //                          charset_excludes(STRING_SET, "]"), array_error);
    // /* ===== DBL ARRAY ===== */
    // int dbl_arr_d_idx = fsmachine_state_add(state_machine, false, T_DBL);
    // int dbl_arr_b_idx = fsmachine_state_add(state_machine, false, T_DBL);
    // int dbl_arr_l_idx = fsmachine_state_add(state_machine, false, T_DBL);
    // fsmachine_transition_add(state_machine, left_bracket_idx,
    //                          charset_create("d"), dbl_arr_d_idx);
    // fsmachine_transition_add(state_machine, dbl_arr_d_idx,
    // charset_create("b"),
    //                          dbl_arr_b_idx);
    // fsmachine_transition_add(state_machine, dbl_arr_b_idx,
    // charset_create("l"),
    //                          dbl_arr_l_idx);
    // fsmachine_transition_add(state_machine, dbl_arr_l_idx,
    // charset_create("]"),
    //                          right_bracket_idx);
    // /* ===== dbl array error  ===== */
    // fsmachine_transition_add(state_machine, dbl_arr_d_idx,
    //                          charset_excludes(IDENTIFIER_SET, "b"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, dbl_arr_b_idx,
    //                          charset_excludes(IDENTIFIER_SET, "l"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, dbl_arr_l_idx,
    //                          charset_excludes(STRING_SET, "]"), array_error);
    // /* ===== INT ARRAY ===== */
    // int int_arr_i_idx = fsmachine_state_add(state_machine, false, T_INT);
    // int int_arr_n_idx = fsmachine_state_add(state_machine, false, T_INT);
    // int int_arr_t_idx = fsmachine_state_add(state_machine, false, T_INT);
    // fsmachine_transition_add(state_machine, left_bracket_idx,
    //                          charset_create("i"), int_arr_i_idx);
    // fsmachine_transition_add(state_machine, int_arr_i_idx,
    // charset_create("n"),
    //                          int_arr_n_idx);
    // fsmachine_transition_add(state_machine, int_arr_n_idx,
    // charset_create("t"),
    //                          int_arr_t_idx);
    // fsmachine_transition_add(state_machine, int_arr_t_idx,
    // charset_create("]"),
    //                          right_bracket_idx);
    // /* ===== int array error  ===== */
    // fsmachine_transition_add(state_machine, int_arr_i_idx,
    //                          charset_excludes(IDENTIFIER_SET, "n"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, int_arr_n_idx,
    //                          charset_excludes(IDENTIFIER_SET, "t"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, int_arr_t_idx,
    //                          charset_excludes(STRING_SET, "]"), array_error);
    // /* ===== STR ARRAY ===== */
    // int str_arr_s_idx = fsmachine_state_add(state_machine, false, T_STR);
    // int str_arr_t_idx = fsmachine_state_add(state_machine, false, T_STR);
    // int str_arr_r_idx = fsmachine_state_add(state_machine, false, T_STR);
    // fsmachine_transition_add(state_machine, left_bracket_idx,
    //                          charset_create("s"), str_arr_s_idx);
    // fsmachine_transition_add(state_machine, str_arr_s_idx,
    // charset_create("t"),
    //                          str_arr_t_idx);
    // fsmachine_transition_add(state_machine, str_arr_t_idx,
    // charset_create("r"),
    //                          str_arr_r_idx);
    // fsmachine_transition_add(state_machine, str_arr_r_idx,
    // charset_create("]"),
    //                          right_bracket_idx);
    // /* ===== str array error  ===== */
    // fsmachine_transition_add(state_machine, str_arr_s_idx,
    //                          charset_excludes(IDENTIFIER_SET, "t"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, str_arr_t_idx,
    //                          charset_excludes(IDENTIFIER_SET, "r"),
    //                          array_error);
    // fsmachine_transition_add(state_machine, str_arr_r_idx,
    //                          charset_excludes(STRING_SET, "]"), array_error);

    /* ########## RELATIONAL SYMBOLS ########## */
    int less_than_idx = fsmachine_state_add(state_machine, true, T_LESS);
    int greater_than_idx = fsmachine_state_add(state_machine, true, T_GREATER);
    int less_than_eql_idx = fsmachine_state_add(state_machine, true, T_LESS_EQL);
    int greater_than_eql_idx = fsmachine_state_add(state_machine, true, T_GREATER_EQL);
    int eql_symbol_idx = fsmachine_state_add(state_machine, true, T_EQL);
    int eql_eql_idx = fsmachine_state_add(state_machine, true, T_EQL_EQL);
    int not_symbol_idx = fsmachine_state_add(state_machine, false, T_ERROR);
    int not_eql_idx = fsmachine_state_add(state_machine, true, T_NOT_EQL);
    
    /* q0 --> < */
    fsmachine_transition_add(state_machine, start_idx, charset_create("<"), less_than_idx);

    /* < --> = */
    fsmachine_transition_add(state_machine, less_than_idx, charset_create("="), less_than_eql_idx);

    /* q0 --> > */
    fsmachine_transition_add(state_machine, start_idx, charset_create(">"), greater_than_idx);

    /* > --> = */
    fsmachine_transition_add(state_machine, greater_than_idx, charset_create("="), greater_than_eql_idx);

    // /* q0 --> = */
    fsmachine_transition_add(state_machine, start_idx, charset_create("="), eql_symbol_idx);

    // /* = --> = */
    fsmachine_transition_add(state_machine, eql_symbol_idx, charset_create("="), eql_symbol_idx);

    /* q0 --> ! */
    fsmachine_transition_add(state_machine, start_idx, charset_create("!"), not_symbol_idx);

    /* ! --> = */
    fsmachine_transition_add(state_machine, not_symbol_idx, charset_create("="), not_eql_idx);

    /* ########## OPERATION SYMBOLS ########## */

    /* ===== arithmetic operation ===== */
    int plus_idx = fsmachine_state_add(state_machine, true, T_PLUS);
    int minus_idx = fsmachine_state_add(state_machine, true, T_MINUS);
    int mul_idx = fsmachine_state_add(state_machine, true, T_MUL);
    int div_idx = fsmachine_state_add(state_machine, true, T_DIV);
    int modulo_idx = fsmachine_state_add(state_machine, true, T_MOD);
    int exponent_idx = fsmachine_state_add(state_machine, true, T_EXP);
    int abs_idx = fsmachine_state_add(state_machine, true, T_ABS);

    /* Q0 -> + */
    fsmachine_transition_add(state_machine, start_idx, charset_create("+"),
                             plus_idx);

    /* Q0 -> - */
    fsmachine_transition_add(state_machine, start_idx, charset_create("-"),
                             minus_idx);

    /* Q0 -> * */
    fsmachine_transition_add(state_machine, start_idx, charset_create("*"),
                             mul_idx);

    /* Q0 -> / */
    fsmachine_transition_add(state_machine, start_idx, charset_create("/"),
                             div_idx);

    /* Q0 -> % */
    fsmachine_transition_add(state_machine, start_idx, charset_create("%"),
                             modulo_idx);

    /* Q0 -> ^ */
    fsmachine_transition_add(state_machine, start_idx, charset_create("^"),
                             exponent_idx);

    /* Q0 -> | */
    fsmachine_transition_add(state_machine, start_idx, charset_create("|"),
                             abs_idx);

    /* ===== shorthand operation ===== */
    int plus_eql_idx = fsmachine_state_add(state_machine, true, T_PLUS_EQL);
    int minus_eql_idx = fsmachine_state_add(state_machine, true, T_MINUS_EQL);
    int mul_eql_idx = fsmachine_state_add(state_machine, true, T_MUL_EQL);
    int div_eql_idx = fsmachine_state_add(state_machine, true, T_DIV_EQL);

    /* + -> += */
    fsmachine_transition_add(state_machine, plus_idx, charset_create("="),
                             plus_eql_idx);

    /* - -> -= */
    fsmachine_transition_add(state_machine, minus_idx, charset_create("="),
                             minus_eql_idx);

    /* * -> *= */
    fsmachine_transition_add(state_machine, mul_idx, charset_create("="),
                             mul_eql_idx);

    /* / -> /= */
    fsmachine_transition_add(state_machine, div_idx, charset_create("="),
                             div_eql_idx);

    /* ########## COMMENT ########## */
    int ddash_idx = fsmachine_state_add(state_machine, true, T_COMMENT);
    int comment_idx = fsmachine_state_add(state_machine, true, T_COMMENT);

    fsmachine_transition_add(state_machine, minus_idx, charset_create("-"), ddash_idx);
    fsmachine_transition_add(state_machine, ddash_idx, charset_excludes(STRING_SET, "\n"), comment_idx);
    fsmachine_transition_add(state_machine, comment_idx, charset_excludes(STRING_SET, "\n"), comment_idx);
    
    /* SYMBOLS */

    /*Left Parenthesis*/
    int left_parenthesis_idx = fsmachine_state_add(state_machine, true, T_LPAREN);

    /* q0 -> ( */
    fsmachine_transition_add(state_machine, start_idx, "(", left_parenthesis_idx);

    /* Right Parenthesis*/
    int right_parenthesis_idx = fsmachine_state_add(state_machine, true, T_RPAREN);

    fsmachine_transition_add(state_machine, start_idx, ")", right_parenthesis_idx);

    /* Colon*/
    int colon_idx = fsmachine_state_add(state_machine, true, T_COLON);

    fsmachine_transition_add(state_machine, start_idx, ":", colon_idx);

    /*Comma*/
    int comma_idx = fsmachine_state_add(state_machine, true, T_COMMA);

    fsmachine_transition_add(state_machine, start_idx, ",", comma_idx);

    /*Question Mark*/
    int q_mark_idx = fsmachine_state_add(state_machine, true, T_QMARK);

    fsmachine_transition_add(state_machine, start_idx, "?", q_mark_idx);


    /* ########## COMMENT ########## */

    /* ########## WHITESPACES ########## */
    int return_idx = fsmachine_state_add(state_machine, true, T_INVALID);
    int newline_idx = fsmachine_state_add(state_machine, true, T_NEWLINE);

    /* Q0 - space -> Q0 */
    fsmachine_transition_add(state_machine, start_idx, charset_create(" "),
                             start_idx);

    /* Q0 --> /r */
    fsmachine_transition_add(state_machine, start_idx, charset_create("\r"),
                             return_idx);

    /* /r -> newline */
    fsmachine_transition_add(state_machine, return_idx, charset_create("\n"),
                             newline_idx);

    /* Q0 --> newline */
    fsmachine_transition_add(state_machine, start_idx, charset_create("\n"),
                             newline_idx);

    return state_machine;
}