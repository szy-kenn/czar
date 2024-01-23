#include "parser.h"
#include <stdio.h>

Parser parser;

Production rules[100] = {
    {P_ACCEPT, 2, {P_program, P_END}},
    {P_program, 1, {P_stmts}},
    {P_stmts, 2, {P_stmts, P_stmt}},
    {P_stmts, 1, {P_stmt}},
    {P_stmt, 1, {P_decl_stmt}},
    {P_stmt, 1, {P_assign_stmt}},
    {P_stmt, 1, {P_input_stmt}},
    {P_stmt, 1, {P_output_stmt}},
    {P_stmt, 1, {P_when_stmt}},
    {P_stmt, 1, {P_loop_stmt}},
    {P_decl_stmt, 5, {P_GLOBAL, P_mut_type, P_IDENT, P_COLON, P_data_type}},
    {P_decl_stmt, 6, {P_GLOBAL, P_mut_type, P_IDENT, P_COLON, P_EQUAL, P_value}},
    {P_decl_stmt, 4, {P_mut_type, P_IDENT, P_COLON, P_data_type}},
    {P_decl_stmt, 6, {P_mut_type, P_IDENT, P_COLON, P_data_type, P_EQUAL, P_value}},
    {P_mut_type, 1, {P_FIXED}},
    {P_mut_type, 1, {P_FLEX}},
    {P_assign_stmt, 3, {P_IDENT, P_EQUAL, P_value}},
    {P_assign_stmt, 1, {P_input_stmt}},
    {P_assign_stmt, 1, {P_expression}},
    {P_assign_stmt, 1, {P_STR}},
    {P_assign_stmt, 1, {P_CHR}},
    {P_assign_stmt, 1, {P_BOOL}},
    {P_assign_stmt, 1, {P_NIL}},
    {P_input_stmt, 4, {P_INPUT, P_LPAREN, P_string_consts, P_RPAREN}},
    {P_input_stmt, 3, {P_INPUT, P_LPAREN, P_RPAREN}},
    {P_output_stmt, 4, {P_OUTPUT, P_LPAREN, P_string_consts, P_RPAREN}},
    {P_output_stmt, 3, {P_OUTPUT, P_LPAREN, P_RPAREN}},
    {P_string_consts, 3, {P_string_consts, P_AMPERSAND, P_string_const}},
    {P_string_consts, 1, {P_string_const}},
    {P_string_const, 1, {P_expression}},
    {P_string_const, 1, {P_STR}},
    {P_string_const, 1, {P_CHR}},
    {P_when_stmt, 7, {P_WHEN, P_expression, P_COLON, P_T_INDENT, P_stmt, P_T_DEDENT, P_else_stmt}},
    {P_else_stmt, 8, {P_ELSE, P_WHEN, P_expression, P_COLON, P_T_INDENT, P_stmt, P_T_DEDENT, P_else_stmt}},
    {P_else_stmt, 4, {P_ELSE, P_COLON, P_T_INDENT, P_stmt}},
    {P_else_stmt, 1, {P_EPSILON}},
    {P_loop_stmt, 9, {P_LOOP, P_IDENT, P_in, P_LPAREN, P_expression, P_RPAREN, P_COLON, P_T_INDENT, P_stmt}},
    {P_loop_stmt, 11, {P_LOOP, P_IDENT, P_in, P_LPAREN, P_expression, P_to, P_expression, P_RPAREN, P_COLON, P_T_INDENT, P_stmt}},
    {P_loop_stmt, 13, {P_LOOP, P_IDENT, P_in, P_LPAREN, P_expression, P_to, P_expression, P_by, P_expression, P_RPAREN, P_COLON, P_T_INDENT, P_stmt}},
    {P_data_type, 1, {P_int}},
    {P_data_type, 1, {P_dbl}},
    {P_data_type, 1, {P_str}},
    {P_data_type, 1, {P_chr}},
    {P_data_type, 1, {P_bool}},
    {P_data_type, 1, {P_nil}},
    {P_expression, 3, {P_expression, P_OR, P_conjunction}},
    {P_expression, 1, {P_conjunction}},
    {P_conjunction, 3, {P_conjunction, P_AND, P_equality}},
    {P_conjunction, 1, {P_equality}},
    {P_equality, 3, {P_equality, P_EQUALS, P_relational}},
    {P_equality, 3, {P_equality, P_NOTEQUALS, P_relational}},
    {P_equality, 1, {P_relational}},
    {P_relational, 3, {P_relational, P_GREATER, P_term}},
    {P_relational, 3, {P_relational, P_GREATEREQUAL, P_term}},
    {P_relational, 3, {P_relational, P_LESS, P_term}},
    {P_relational, 3, {P_relational, P_LESSEQUAL, P_term}},
    {P_relational, 1, {P_term}},
    {P_term, 3, {P_term, P_PLUS, P_factor}},
    {P_term, 3, {P_term, P_MINUS, P_factor}},
    {P_term, 1, {P_factor}},
    {P_factor, 3, {P_factor, P_TIMES, P_power}},
    {P_factor, 3, {P_factor, P_DIVIDE, P_power}},
    {P_factor, 3, {P_factor, P_MODULO, P_power}},
    {P_factor, 1, {P_power}},
    {P_power, 3, {P_power, P_POW, P_unary}},
    {P_power, 1, {P_unary}},
    {P_unary, 2, {P_NOT, P_unary}},
    {P_unary, 2, {P_MINUS, P_unary}},
    {P_unary, 1, {P_abs}},
    {P_abs, 3, {P_ABS, P_abs, P_ABS}},
    {P_abs, 1, {P_literal}},
    {P_literal, 3, {P_LPAREN, P_expression, P_RPAREN}},
    {P_literal, 1, {P_INT}},
    {P_literal, 1, {P_DBL}},
    {P_literal, 1, {P_true}},
    {P_literal, 1, {P_false}},
    {P_literal, 1, {P_IDENT}}
};

/**
Production rules[40] = {
    
    // 0
    {P_ACCEPT, 1, {P_EXPR}},

    // 1 - 2
    {P_EXPR, 1, {P_CONJ}},
    {P_EXP, 3, {P_EXPR, P_OR, P_CONJ}},

    // 3 - 4
    {P_CONJ, 3, {P_CONJ, P_AND, P_EQL}},
    {P_CONJ, 1, {P_EQL}},

    // 5 - 7
    {P_EQL, 3, {P_EQL, P_EQL_EQL, P_REL}},
    {P_EQL, 3, {P_EQL, P_NOT_EQL, P_REL}},
    {P_EQL, 1, {P_REL}},

    // 8 - 12
    {P_REL, 3, {P_REL, P_GREATER, P_TERM}},
    {P_REL, 3, {P_REL, P_GREATER_EQL, P_TERM}},
    {P_REL, 3, {P_REL, P_LESS, P_TERM}},
    {P_REL, 3, {P_REL, P_LESS_EQL, P_TERM}},
    {P_REL, 1, {P_TERM}},

    // 13 - 15
    {P_TERM, 3, {P_TERM, P_PLUS, P_FACTOR}},
    {P_TERM, 3, {P_TERM, P_MINUS, P_FACTOR}},
    {P_TERM, 1, {P_FACTOR}},

    // 16 - 19 
    {P_FACTOR, 3, {P_FACTOR, P_MUL, P_POWER}},
    {P_FACTOR, 3, {P_FACTOR, P_DIV, P_POWER}},
    {P_FACTOR, 3, {P_FACTOR, P_MOD, P_POWER}},
    {P_FACTOR, 1, {P_POWER}},

    // 20-21
    {P_POWER, 3, {P_POWER, P_EXP, P_UNARY}},
    {P_POWER, 1, {P_UNARY}},

    // 22-24 
    {P_UNARY, 2, {P_NOT, P_UNARY}},
    {P_UNARY, 2, {P_MINUS, P_UNARY}},
    {P_UNARY, 1, {P_ABSVAL}},

    // 25-26
    {P_ABSVAL, 3, {P_ABS, P_ABSVAL, P_ABS}},
    {P_ABSVAL, 1, {P_LITERAL}},

    // 27 - 32
    {P_LITERAL, 3, {P_LPAREN, P_EXPR, P_RPAREN}},
    {P_LITERAL, 1, {P_INT}},
    {P_LITERAL, 1, {P_DBL}},
    {P_LITERAL, 1, {P_TRUE}},
    {P_LITERAL, 1, {P_FALSE}},
    {P_LITERAL, 1, {P_ID}},
};
**/

parsing_state parsing_table[150][150] = {
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(9), GT(10), GT(11), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(19), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, S(3), S(4), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(20), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), R(14), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), R(15), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(21), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(22), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(32), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(42), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{S(43), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(1), R(1), S(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), S(2), S(3), S(4), S(5), S(6), S(7), R(1), S(8), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), R(1), X, X, X, GT(44), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), R(3), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), R(4), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{X, X, S(45), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), R(5), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), R(6), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), R(7), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), R(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), R(9), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{X, X, S(23), S(46), S(47), S(48), S(49), S(24), S(25), X, X, X, X, X, S(5), X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, GT(50), GT(51), X, X, X, X, X, X, X, GT(52), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(53), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), S(54), S(55), X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), S(56), X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, GT(57), GT(58), X, X, X, X, GT(59), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), S(54), S(55), X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), S(60), X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, GT(61), GT(58), X, X, X, X, GT(59), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), R(76), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), R(72), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), R(73), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(62), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(63), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(64), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(65), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), R(74), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), R(75), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(67), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), S(68), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), R(46), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), S(69), S(70), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), R(48), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), S(71), S(72), S(73), S(74), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), R(51), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), R(56), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), S(75), S(76), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), R(58), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), S(77), S(78), S(79), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), R(61), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), S(80), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), R(65), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), R(67), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), R(70), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(81), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, ACCEPT, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), R(2), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},       
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(82), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), R(19), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), R(20), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), R(21), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), R(22), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), R(16), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), R(17), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), S(66), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), R(18), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(83), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), R(30), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), R(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), R(24), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(84), S(85), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), R(28), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), S(66), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), R(29), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), R(26), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(86), S(85), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), R(69), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), R(68), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(87), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(88), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(89), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, X, X, X, X, X, X, X, S(90), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(91), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(92), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(93), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(94), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(95), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(96), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(97), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(98), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(99), GT(39), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(100), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(101), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(102), GT(40), GT(41)},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), X, X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(103), GT(41)},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(104), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(105), S(106), S(107), S(108), S(109), S(110), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(111), X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(112), X, X, X, X, X, X, S(105), S(106), S(107), S(108), S(109), S(110), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(113), X, X, X, X, X, X, X, X, X, X},
{R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), R(23), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), S(54), S(55), X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, GT(114), X, X, X, X, GT(59), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), R(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), R(57), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), R(71), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), S(68), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), R(45), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(115), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), S(69), S(70), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), R(47), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), S(71), S(72), S(73), S(74), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), R(49), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), S(71), S(72), S(73), S(74), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), R(50), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), R(52), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), R(53), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), R(54), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), R(55), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), S(77), S(78), S(79), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), R(59), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), S(77), S(78), S(79), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), R(60), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), S(80), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), R(62), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), S(80), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), R(63), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), S(80), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), R(64), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), R(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(116), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), R(40), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), R(41), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), R(42), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), R(43), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), R(44), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), S(117), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), R(12), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), S(46), S(47), S(48), S(49), S(24), S(25), X, X, X, X, X, S(5), X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, GT(118), GT(51), X, X, X, X, X, X, X, GT(52), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), R(10), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), R(27), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, S(119), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(120), X, X, S(121), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), S(46), S(47), S(48), S(49), S(24), S(25), X, X, X, X, X, S(5), X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, GT(122), GT(51), X, X, X, X, X, X, X, GT(52), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), R(11), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), S(123), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), X, X, X, X, X, X, X, X, X, X, X, X, X, GT(124), X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(125), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(126), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), R(13), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(127), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(128), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(129), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(130), X, X, X, S(131), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(132), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, X, X, X, X, X, X, X, S(133), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(134), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(135), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(23), X, X, X, X, S(24), S(25), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(26), X, X, X, X, S(27), S(28), X, X, S(29), X, X, X, X, X, X, X, X, X, X, X, S(30), S(31), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(136), GT(33), GT(34), GT(35), GT(36), GT(37), GT(38), GT(39), GT(40), GT(41)},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(137), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(138), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(139), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(140), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(141), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(142), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(143), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(144), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(145), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, S(146), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(147), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), S(123), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), X, X, X, X, X, X, X, X, X, X, X, X, X, GT(148), X, X, X, X, X, X, X, X, X, X, X, X},
{R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X}};

/** parsing_state parsing_table[100][100] = {
//   $      %	    (	    )	    *	    +	    -	    /	    ^	    |	    error	INT	    DBL	    TRUE	FALSE	ID	    OR	    AND	    NOT 	==	    !=	    >	    >=	    <	    <=	    acc	    expr	conj	eql	    rel	    trm	    fac	    pow	    una	    abs	    lit
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      GT(10), GT(11), GT(12), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {R(28), R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(29), R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(30), R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(31), R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(32), R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(20), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(21), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      X,      X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(22), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      GT(23), GT(11), GT(12), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {S(24), X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      S(25),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(1),  R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   S(26),  R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(4),  R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   S(27),  S(28),  R(4),   R(4),   R(4),   R(4),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(7),  R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   S(29),  S(30),  S(31),  S(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(12), R(12),  R(12),  R(12),  R(12),  S(33),  S(34),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(15), S(37),  R(15),  R(15),  S(35),  R(15),  R(15),  S(36),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(19), R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  S(38),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(21), R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(24), R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(26), R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(22), R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(23), R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      X,      X,      X,      X,      X,      X,      X,      S(39),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      X,      S(40),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      S(25),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {ACCEPT, X,     X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      GT(41), GT(12), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(42), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(43), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(44), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(45), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(46), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(47), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(48), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(49), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(50), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(51), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(52), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(53), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(54), GT(18), GT(19)  },
    {R(25), R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(27), R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(2),  R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   S(26),  R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(3),  R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   S(27),  S(28),  R(3),   R(3),   R(3),   R(3),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(5),  R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   S(29),  S(30),  S(31),  S(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(6),  R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   S(29),  S(30),  S(31),  S(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(8),  R(8),   R(8),   R(8),   R(8),   S(33),  S(34),  R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(9),  R(9),   R(9),   R(9),   R(9),   S(33),  S(34),  R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(10), R(10),  R(10),  R(10),  R(10),  S(33),  S(34),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(11), R(11),  R(11),  R(11),  R(11),  S(33),  S(34),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(13), S(37),  R(13),  R(13),  S(35),  R(13),  R(13),  S(36),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(14), S(37),  R(14),  R(14),  S(35),  R(14),  R(14),  S(36),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(16), R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  S(38),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(17), R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  S(38),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(18), R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  S(38),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	X,      X,  	X,  	X,  	X,  	X,  	X,  	X,  	X,  	X,  	X       }
};

**/

int index_get(token_t token) {
    switch (token)
    {
    case T_STR:
        return P_STR;
    case T_CHR:
        return P_CHR;
    case T_BOOL:
        return P_BOOL;
    case T_NIL:
        return P_NIL;
    case T_INT:
        return P_INT;
    case T_DBL:
        return P_DBL;
    case T_INDENT:
        return P_T_INDENT;
    case T_DEDENT:
        return P_T_DEDENT;
    case T_GLOBAL:
        return P_GLOBAL;
    case T_FIXED:
        return P_FIXED;
    case T_FLEX:
        return P_FLEX;
    case T_INPUT:
        return P_INPUT;
    case T_OUTPUT:
        return P_OUTPUT;
    case T_WHEN:
        return P_WHEN;
    case T_ELSE:
        return P_ELSE;
    case T_LOOP:
        return P_LOOP;
    case T_COLON:
        return P_COLON;
    // case T_AMPERSAND
    case T_MOD:
        return P_MODULO;
    case T_LPAREN:
        return P_LPAREN;
    case T_RPAREN:
        return P_RPAREN;
    case T_MUL:
        return P_TIMES;
    case T_PLUS:
        return P_PLUS;
    case T_MINUS:
        return P_MINUS;
    case T_DIV:
        return P_DIVIDE;
    case T_EXP:
        return P_POW;
    case T_ABS:
        return P_ABS;
    case T_TRUE:
        return P_true;
    case T_FALSE:
        return P_false;
    case T_IDENT:
        return P_IDENT;
    case T_OR:
        return P_OR;
    case T_AND:
        return P_AND;
    case T_NOT:
        return P_NOT;
    case T_EQL:
        return P_EQUAL;
    case T_EQL_EQL:
        return P_EQUALS;
    case T_NOT_EQL:
        return P_NOTEQUALS;
    case T_GREATER:
        return P_GREATER;
    case T_GREATER_EQL:
        return P_GREATEREQUAL;
    case T_LESS:
        return P_LESS;
    case T_LESS_EQL:
        return P_LESSEQUAL;
    case T_EOF:
        return P_END;
    case T_IN:
        return P_in;
    case T_TO:
        return P_to;
    case T_BY:
        return P_by;
    }
}

void input_string_print(InputString input_string) {
    switch (input_string) {
    case (P_END):
        printf("P_END");
        return;
    case (P_MODULO):
        printf("P_MOD");
        return;
    case (P_LPAREN):
        printf("P_LPAREN");
        return;
    case (P_RPAREN):
        printf("P_RPAREN");
        return;
    case (P_TIMES):
        printf("P_MUL");
        return;
    case (P_PLUS):
        printf("P_PLUS");
        return;
    case (P_MINUS):
        printf("P_MINUS");
        return;
    case (P_DIVIDE):
        printf("P_DIV");
        return;
    case (P_POW):
        printf("P_EXP");
        return;
    case (P_ABS):
        printf("P_ABS");
        return;
    case (P_ERROR):
        printf("P_ERR");
        return;
    case (P_INT):
        printf("P_INT");
        return;
    case (P_DBL):
        printf("P_DBL");
        return;
    case (P_true):
        printf("P_TRUE");
        return;
    case (P_false):
        printf("P_FALSE");
        return;
    case (P_IDENT):
        printf("P_ID");
        return;
    case (P_OR):
        printf("P_OR");
        return;
    case (P_AND):
        printf("P_AND");
        return;
    case (P_NOT):
        printf("P_NOT");
        return;
    case (P_EQUALS):
        printf("P_EQL_EQL");
        return;
    case (P_NOTEQUALS):
        printf("P_NOT_EQL");
        return;
    case (P_GREATER):
        printf("P_GREATER");
        return;
    case (P_GREATEREQUAL):
        printf("P_GREATER_EQL");
        return;
    case (P_LESS):
        printf("P_LESS");
        return;
    case (P_LESSEQUAL):
        printf("P_LESS_EQL");
        return;
    case (P_ACCEPT):
        printf("P_ACCEPT");
        return;
    case (P_expression):
        printf("P_EXPR");
        return;
    case (P_conjunction):
        printf("P_CONJ");
        return;
    case (P_equality):
        printf("P_EQL");
        return;
    case (P_relational):
        printf("P_REL");
        return;
    case (P_term):
        printf("P_TERM");
        return;
    case (P_factor):
        printf("P_FACTOR");
        return;
    case (P_power):
        printf("P_POWER");
        return;
    case (P_unary):
        printf("P_UNARY");
        return;
    case (P_abs):
        printf("P_ABSVAL");
        return;
    case (P_literal):
        printf("P_LITERAL");
        return;    
    }
}

InputString token_get(int idx) {
    return index_get(parser.token_array[idx].token_type);
}

void _stack_print(Stack *stack) {
    node_t *cur = stack->top;

    while (cur != NULL) {
        if (cur == stack->top) {
            printf("[");
            input_string_print(cur->value);
        } else {
            printf(", ");
            input_string_print(cur->value);
        }
        cur = cur->next;
        if (cur == NULL) {
            printf("]\n");
        }
    }
}

void parser_initialize(Token *token_array, int token_count) {
    parser.current = 0;
    parser.token_array = token_array;
    parser.token_count = token_count;
};

void parser_start(bool debug) {

    printf("========== STARTING SYNTAX ANALYSIS ==========\n");
    bool is_parsing = true;
    int current_state_idx = 0;
    int current_idx = 0;
    InputString current_token = token_get(current_idx);
    parsing_state current_state;

    // for state indices
    Stack *state_stack = stack_create();
    stack_push(state_stack, current_state_idx);

    // for tokens
    Stack *token_stack = stack_create();

    while(is_parsing) {
        current_state = parsing_table[current_state_idx][current_token];     // current state from parsing table (start_state - input_string -> current_state)

        // check what action to perform
        switch (current_state.action_type) {

            case SHIFT:
                if (debug) {
                    printf("q%d\t-- T%03d\t SHIFT(%d)\t--> q%d\n", current_state_idx, current_token, current_state.value, current_state.value);
                }
                stack_push(state_stack, current_state.value);
                stack_push(token_stack, current_token);
                current_state_idx = current_state.value;

                if (current_idx  < parser.token_count - 1) {
                    current_idx++;
                }

                current_token = token_get(current_idx);
                _stack_print(token_stack);
                break;
            
            case REDUCE:
                if (debug) {
                    printf("q%d\t-- T%03d\t REDUCE(%d)\t--> q%d\n", current_state_idx, current_token, current_state.value, current_state.value);
                }

                Production rule = rules[current_state.value];
                for (int i = 0; i < rule.rhs_count; i++) {
                    if (rule.rhs[rule.rhs_count-1-i] == token_stack->top->value) {
                        stack_pop(state_stack);
                        // printf("Transition: q%d -> ", stack_pop(state_stack)->value);
                        // printf("q%d\n", state_stack->top->value);
                        stack_pop(token_stack);
                    }
                }

                current_state_idx = state_stack->top->value;
                current_token = rule.lhs;
                stack_push(token_stack, current_token);
                _stack_print(token_stack);
                break;

            case GOTO:
                if (debug) {
                    printf("q%d\t-- T%03d\t GOTO(%d)\t--> q%d\n", current_state_idx, current_token, current_state.value, current_state.value);
                } 
                current_token = token_get(current_idx);
                current_state_idx = current_state.value;
                stack_push(state_stack, current_state_idx);
                break;

            case ACCEPTED:
                if (debug) {
                    printf("Accepted!\n");
                }
                is_parsing = false;
                break;

            case ERROR:
                if (debug)
                    printf("Syntax Error!\n");
                    continue;

            default:
                break;
         }

    }
    stack_free(state_stack);
    stack_free(token_stack);
};
