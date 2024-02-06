#include "parser.h"
#include <stdio.h>

Parser parser;

Production rules[77] = {
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
    {P_value, 1, {P_input_stmt}},
    {P_value, 1, {P_expression}},
    {P_value, 1, {P_STR}},
    {P_value, 1, {P_CHR}},
    {P_value, 1, {P_BOOL}},
    {P_value, 1, {P_NIL}},
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
    {P_else_stmt, 5, {P_ELSE, P_COLON, P_T_INDENT, P_stmt, P_T_DEDENT}},
    {P_else_stmt, 1, {P_EPSILON}},
    {P_loop_stmt, 10, {P_LOOP, P_IDENT, P_in, P_LPAREN, P_expression, P_RPAREN, P_COLON, P_T_INDENT, P_stmt, P_T_DEDENT}},
    {P_loop_stmt, 12, {P_LOOP, P_IDENT, P_in, P_LPAREN, P_expression, P_to, P_expression, P_RPAREN, P_COLON, P_T_INDENT, P_stmt, P_T_DEDENT}},
    {P_loop_stmt, 14, {P_LOOP, P_IDENT, P_in, P_LPAREN, P_expression, P_to, P_expression, P_by, P_expression, P_RPAREN, P_COLON, P_T_INDENT, P_stmt, P_T_DEDENT}},
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
    {P_relational, 3, {P_relational, P_GREATER, P_abs}},
    {P_relational, 3, {P_relational, P_GREATEREQUAL, P_abs}},
    {P_relational, 3, {P_relational, P_LESS, P_abs}},
    {P_relational, 3, {P_relational, P_LESSEQUAL, P_abs}},
    {P_relational, 1, {P_abs}},
    {P_abs, 3, {P_ABS, P_abs, P_ABS}},
    {P_abs, 1, {P_term}},
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
    {P_unary, 1, {P_literal}},
    {P_literal, 3, {P_LPAREN, P_expression, P_RPAREN}},
    {P_literal, 1, {P_INT}},
    {P_literal, 1, {P_DBL}},
    {P_literal, 1, {P_true}},
    {P_literal, 1, {P_false}},
    {P_literal, 1, {P_IDENT}}
};

parsing_state parsing_table[154][77] = 
{{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(9), GT(10), GT(11), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
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
{X, X, X, X, X, X, X, X, X, X, S(139), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(140), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(66), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(141), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(142), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, S(143), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},    
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(144), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, S(145), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(146), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},    
{X, X, X, X, X, X, X, X, X, X, S(147), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, S(148), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, S(149), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},    
{X, X, S(1), X, X, X, X, X, X, X, X, S(2), S(3), S(4), S(5), S(6), S(7), X, S(8), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, GT(150), GT(12), GT(13), GT(14), X, GT(15), GT(16), X, X, GT(17), X, GT(18), X, X, X, X, X, X, X, X, X, X, X},
{R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), S(123), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), X, X, X, X, X, X, X, X, X, X, X, X, X, GT(151), X, X, X, X, X, X, X, X, X, X, X, X},
{X, X, X, X, X, X, X, X, X, X, S(152), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
{R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},    
{R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X}    
};

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
    case T_AMPERSAND:
        return P_AMPERSAND;
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
    case T_DT_BOOL:
        return P_bool;
    case T_DT_CHR:
        return P_chr;
    case T_DT_DBL:
        return P_dbl;
    case T_DT_INT:
        return P_int;
    case T_DT_NIL:
        return P_nil;
    case T_DT_STR:
        return P_str;
    }
}

void input_string_print(InputString input_string) {
    switch (input_string) {

    case (P_END):
        printf("P_END");
        return;
    case (P_ERROR):
        printf("P_ERROR");
        return;
    case (P_IDENT):
        printf("P_IDENT");
        return;
    case (P_STR):
        printf("P_STR");
        return;
    case (P_CHR):
        printf("P_CHR");
        return;
    case (P_BOOL):
        printf("P_BOOL");
        return;
    case (P_NIL):
        printf("P_NIL");
        return;
    case (P_INT):
        printf("P_INT");
        return;
    case (P_DBL):
        printf("P_DBL");
        return;
    case (P_T_INDENT):
        printf("P_T_INDENT");
        return;
    case (P_T_DEDENT):
        printf("P_T_DEDENT");
        return;
    case (P_GLOBAL):
        printf("P_GLOBAL");
        return;
    case (P_FIXED):
        printf("P_FIXED");
        return;
    case (P_FLEX):
        printf("P_FLEX");
        return;
    case (P_INPUT):
        printf("P_INPUT");
        return;
    case (P_OUTPUT):
        printf("P_OUTPUT");
        return;
    case (P_WHEN):
        printf("P_WHEN");
        return;
    case (P_ELSE):
        printf("P_ELSE");
        return;
    case (P_LOOP):
        printf("P_LOOP");
        return;
    case (P_AND):
        printf("P_AND");
        return;
    case (P_OR):
        printf("P_OR");
        return;
    case (P_EQUALS):
        printf("P_EQUALS");
        return;
    case (P_NOTEQUALS):
        printf("P_NOTEQUALS");
        return;
    case (P_GREATER):
        printf("P_GREATER");
        return;
    case (P_GREATEREQUAL):
        printf("P_GREATEREQUAL");
        return;
    case (P_LESS):
        printf("P_LESS");
        return;
    case (P_LESSEQUAL):
        printf("P_LESSEQUAL");
        return;
    case (P_PLUS):
        printf("P_PLUS");
        return;
    case (P_MINUS):
        printf("P_MINUS");
        return;
    case (P_TIMES):
        printf("P_TIMES");
        return;
    case (P_DIVIDE):
        printf("P_DIVIDE");
        return;
    case (P_MODULO):
        printf("P_MODULO");
        return;
    case (P_POW):
        printf("P_POW");
        return;
    case (P_NOT):
        printf("P_NOT");
        return;
    case (P_ABS):
        printf("P_ABS");
        return;
    case (P_COLON):
        printf("P_COLON");
        return;
    case (P_EQUAL):
        printf("P_EQUAL");
        return;
    case (P_LPAREN):
        printf("P_LPAREN");
        return;
    case (P_RPAREN):
        printf("P_RPAREN");
        return;
    case (P_AMPERSAND):
        printf("P_AMPERSAND");
        return;
    case (P_in):
        printf("P_in");
        return;
    case (P_to):
        printf("P_to");
        return;
    case (P_by):
        printf("P_by");
        return;
    case (P_int):
        printf("P_int");
        return;
    case (P_dbl):
        printf("P_dbl");
        return;
    case (P_str):
        printf("P_str");
        return;
    case (P_chr):
        printf("P_chr");
        return;
    case (P_bool):
        printf("P_bool");
        return;
    case (P_nil):
        printf("P_nil");
        return;
    case (P_true):
        printf("P_true");
        return;
    case (P_false):
        printf("P_false");
        return;
    case (P_ACCEPT):
        printf("P_ACCEPT");
        return;
    case (P_program):
        printf("P_program");
        return;
    case (P_stmts):
        printf("P_stmts");
        return;
    case (P_stmt):
        printf("P_stmt");
        return;
    case (P_decl_stmt):
        printf("P_decl_stmt");
        return;
    case (P_mut_type):
        printf("P_mut_type");
        return;
    case (P_assign_stmt):
        printf("P_assign_stmt");
        return;
    case (P_value):
        printf("P_value");
        return;
    case (P_input_stmt):
        printf("P_input_stmt");
        return;
    case (P_output_stmt):
        printf("P_output_stmt");
        return;
    case (P_string_consts):
        printf("P_string_consts");
        return;
    case (P_string_const):
        printf("P_string_const");
        return;
    case (P_when_stmt):
        printf("P_when_stmt");
        return;
    case (P_else_stmt):
        printf("P_else_stmt");
        return;
    case (P_loop_stmt):
        printf("P_loop_stmt");
        return;
    case (P_data_type):
        printf("P_data_type");
        return;
    case (P_expression):
        printf("P_expression");
        return;
    case (P_conjunction):
        printf("P_conjunction");
        return;
    case (P_equality):
        printf("P_equality");
        return;
    case (P_relational):
        printf("P_relational");
        return;
    case (P_abs):
        printf("P_abs");
        return;
    case (P_term):
        printf("P_term");
        return;
    case (P_factor):
        printf("P_factor");
        return;
    case (P_power):
        printf("P_power");
        return;
    case (P_unary):
        printf("P_unary");
        return;
    case (P_literal):
        printf("P_literal");
        return;
    }
}

char *input_string_get(InputString value) {

    char *val = malloc(100);

    switch (value){
    case (P_END):
        strcpy(val,"P_END");
        return val;
    case (P_ERROR):
        strcpy(val,"P_ERROR");
        return val;
    case (P_IDENT):
        strcpy(val,"P_IDENT");
        return val;
    case (P_STR):
        strcpy(val,"P_STR");
        return val;
    case (P_CHR):
        strcpy(val,"P_CHR");
        return val;
    case (P_BOOL):
        strcpy(val,"P_BOOL");
        return val;
    case (P_NIL):
        strcpy(val,"P_NIL");
        return val;
    case (P_INT):
        strcpy(val,"P_INT");
        return val;
    case (P_DBL):
        strcpy(val,"P_DBL");
        return val;
    case (P_T_INDENT):
        strcpy(val,"P_T_INDENT");
        return val;
    case (P_T_DEDENT):
        strcpy(val,"P_T_DEDENT");
        return val;
    case (P_GLOBAL):
        strcpy(val,"P_GLOBAL");
        return val;
    case (P_FIXED):
        strcpy(val,"P_FIXED");
        return val;
    case (P_FLEX):
        strcpy(val,"P_FLEX");
        return val;
    case (P_INPUT):
        strcpy(val,"P_INPUT");
        return val;
    case (P_OUTPUT):
        strcpy(val,"P_OUTPUT");
        return val;
    case (P_WHEN):
        strcpy(val,"P_WHEN");
        return val;
    case (P_ELSE):
        strcpy(val,"P_ELSE");
        return val;
    case (P_LOOP):
        strcpy(val,"P_LOOP");
        return val;
    case (P_AND):
        strcpy(val,"P_AND");
        return val;
    case (P_OR):
        strcpy(val,"P_OR");
        return val;
    case (P_EQUALS):
        strcpy(val,"P_EQUALS");
        return val;
    case (P_NOTEQUALS):
        strcpy(val,"P_NOTEQUALS");
        return val;
    case (P_GREATER):
        strcpy(val,"P_GREATER");
        return val;
    case (P_GREATEREQUAL):
        strcpy(val,"P_GREATEREQUAL");
        return val;
    case (P_LESS):
        strcpy(val,"P_LESS");
        return val;
    case (P_LESSEQUAL):
        strcpy(val,"P_LESSEQUAL");
        return val;
    case (P_PLUS):
        strcpy(val,"P_PLUS");
        return val;
    case (P_MINUS):
        strcpy(val,"P_MINUS");
        return val;
    case (P_TIMES):
        strcpy(val,"P_TIMES");
        return val;
    case (P_DIVIDE):
        strcpy(val,"P_DIVIDE");
        return val;
    case (P_MODULO):
        strcpy(val,"P_MODULO");
        return val;
    case (P_POW):
        strcpy(val,"P_POW");
        return val;
    case (P_NOT):
        strcpy(val,"P_NOT");
        return val;
    case (P_ABS):
        strcpy(val,"P_ABS");
        return val;
    case (P_COLON):
        strcpy(val,"P_COLON");
        return val;
    case (P_EQUAL):
        strcpy(val,"P_EQUAL");
        return val;
    case (P_LPAREN):
        strcpy(val,"P_LPAREN");
        return val;
    case (P_RPAREN):
        strcpy(val,"P_RPAREN");
        return val;
    case (P_AMPERSAND):
        strcpy(val,"P_AMPERSAND");
        return val;
    case (P_in):
        strcpy(val,"P_in");
        return val;
    case (P_to):
        strcpy(val,"P_to");
        return val;
    case (P_by):
        strcpy(val,"P_by");
        return val;
    case (P_int):
        strcpy(val,"P_int");
        return val;
    case (P_dbl):
        strcpy(val,"P_dbl");
        return val;
    case (P_str):
        strcpy(val,"P_str");
        return val;
    case (P_chr):
        strcpy(val,"P_chr");
        return val;
    case (P_bool):
        strcpy(val,"P_bool");
        return val;
    case (P_nil):
        strcpy(val,"P_nil");
        return val;
    case (P_true):
        strcpy(val,"P_true");
        return val;
    case (P_false):
        strcpy(val,"P_false");
        return val;
    case (P_ACCEPT):
        strcpy(val,"P_ACCEPT");
        return val;
    case (P_program):
        strcpy(val,"P_program");
        return val;
    case (P_stmts):
        strcpy(val,"P_stmts");
        return val;
    case (P_stmt):
        strcpy(val,"P_stmt");
        return val;
    case (P_decl_stmt):
        strcpy(val,"P_decl_stmt");
        return val;
    case (P_mut_type):
        strcpy(val,"P_mut_type");
        return val;
    case (P_assign_stmt):
        strcpy(val,"P_assign_stmt");
        return val;
    case (P_value):
        strcpy(val,"P_value");
        return val;
    case (P_input_stmt):
        strcpy(val,"P_input_stmt");
        return val;
    case (P_output_stmt):
        strcpy(val,"P_output_stmt");
        return val;
    case (P_string_consts):
        strcpy(val,"P_string_consts");
        return val;
    case (P_string_const):
        strcpy(val,"P_string_const");
        return val;
    case (P_when_stmt):
        strcpy(val,"P_when_stmt");
        return val;
    case (P_else_stmt):
        strcpy(val,"P_else_stmt");
        return val;
    case (P_loop_stmt):
        strcpy(val,"P_loop_stmt");
        return val;
    case (P_data_type):
        strcpy(val,"P_data_type");
        return val;
    case (P_expression):
        strcpy(val,"P_expression");
        return val;
    case (P_conjunction):
        strcpy(val,"P_conjunction");
        return val;
    case (P_equality):
        strcpy(val,"P_equality");
        return val;
    case (P_relational):
        strcpy(val,"P_relational");
        return val;
    case (P_abs):
        strcpy(val,"P_abs");
        return val;
    case (P_term):
        strcpy(val,"P_term");
        return val;
    case (P_factor):
        strcpy(val,"P_factor");
        return val;
    case (P_power):
        strcpy(val,"P_power");
        return val;
    case (P_unary):
        strcpy(val,"P_unary");
        return val;
    case (P_literal):
        strcpy(val,"P_literal");
        return val;
    }
}

token_node_t *token_get(int idx) {
    token_node_t *token_node = malloc(sizeof(token_node_t));
    token_node->parse_tree_node = malloc(sizeof(TreeNode));
    token_node->parse_tree_node->children = NULL;
    token_node->parse_tree_node->children_count = 0;
    token_node->parse_tree_node->children_memory = 0;
    token_node->parse_tree_node->parent = NULL;
    token_node->parse_tree_node->value = parser.token_array[idx].lexeme;
    token_node->type = index_get(parser.token_array[idx].token_type);
    return token_node;
}

void _stack_print(TokenStack *stack) {
    token_node_t *cur = stack->top;

    while (cur != NULL) {
        if (cur == stack->top) {
            printf("[");
            input_string_print(cur->type);
        } else {
            printf(", ");
            input_string_print(cur->type);
        }
        cur = cur->next;
        if (cur == NULL) {
            printf("]\n");
        }
    }
}

void space_print(int reps, FILE *fp) {
    for (int i = 0; i < reps; i++) {
        fprintf(fp, "    ");
    }
}

void _to_ast_rcv(TreeNode *tree_node, int depth, FILE *fp) {
    if (tree_node == NULL) {
        fprintf(fp, "---\n");
        return;
    }

    if (strcmp(tree_node->value, "P_program") == 0) {

        space_print(depth, fp);
        fprintf(fp, "%s:\n", tree_node->value);

        depth++;
    }

    if (strcmp(tree_node->value, "P_decl_stmt") == 0) {

        space_print(depth, fp);
        fprintf(fp, "%s:\n", tree_node->value);

        depth++;
        space_print(depth, fp);

        if (tree_node->children_count == 5) {
            fprintf(fp, "Scope: %s\n", tree_node->children[4]->value);
        } else {
            fprintf(fp, "Scope: local\n");
        }

        space_print(depth, fp);
        fprintf(fp, "Mutability Type: %s\n", tree_node->children[3]->children[0]->value);

        space_print(depth, fp);
        fprintf(fp, "Identifier Name: %s\n", tree_node->children[2]->value);
        
        space_print(depth, fp);       
        fprintf(fp, "Data type: %s\n", tree_node->children[0]->children[0]->value);
        
    } else if (strcmp(tree_node->value, "P_assign_stmt") == 0) {

        space_print(depth, fp);
        fprintf(fp, "%s:\n", tree_node->value);

        depth++;

        space_print(depth, fp);
        fprintf(fp, "Identifier Name: %s\n", tree_node->children[tree_node->children_count - 1]->value);

        space_print(depth, fp);
        fprintf(fp, "Value:\n");
        _to_ast_rcv(tree_node->children[0], depth+1, fp);
    } else if (strcmp(tree_node->value, "P_input_stmt") == 0 ||
               strcmp(tree_node->value, "P_output_stmt") == 0) {
        space_print(depth, fp);
        fprintf(fp, "%s:\n", tree_node->value);

        depth++;

        space_print(depth, fp);
        fprintf(fp, "Contents:\n");
        depth++;
        _to_ast_rcv(tree_node->children[1], depth, fp);

    } else if (strcmp(tree_node->value, "P_loop_stmt") == 0) {

        space_print(depth, fp);
        fprintf(fp, "%s:\n", tree_node->value);
        
        depth++;

        space_print(depth, fp);
        fprintf(fp, "Loop variable: %s\n", tree_node->children[tree_node->children_count - 2]->value);
        
        if (tree_node->children_count == 9) {
            space_print(depth, fp);
            fprintf(fp, "Start: 0\n");
            space_print(depth, fp);
            fprintf(fp, "End: \n");

            space_print(depth+1, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[4], depth+2, fp);
        }
        if (tree_node->children_count == 11) {
            space_print(depth, fp);
            fprintf(fp, "Start:\n");
            space_print(depth+1, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[6], depth+2, fp);
            
            space_print(depth, fp);
            fprintf(fp, "End: \n");
            space_print(depth+1, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[4], depth+2, fp);
        }
        if (tree_node->children_count == 13) {
            space_print(depth, fp);
            fprintf(fp, "Start:\n");
            space_print(depth+1, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[8], depth+2, fp);
            
            space_print(depth, fp);
            fprintf(fp, "End: \n");
            space_print(depth+1, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[6], depth+2, fp);

            space_print(depth, fp);
            fprintf(fp, "Step: \n");
            space_print(depth+1, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[4], depth+2, fp);
        }

        space_print(depth, fp);
        fprintf(fp, "Body:\n");
        _to_ast_rcv(tree_node->children[1], depth+1, fp);

    } else if (strcmp(tree_node->value, "P_when_stmt") == 0) {

        space_print(depth, fp);
        fprintf(fp, "%s:\n", tree_node->value);
        depth++;

        space_print(depth, fp);
        fprintf(fp, "Condition: \n");
        _to_ast_rcv(tree_node->children[5], depth+1, fp);
        
        space_print(depth, fp);
        fprintf(fp, "Body: \n");
        _to_ast_rcv(tree_node->children[2], depth+1, fp);

        space_print(depth, fp);
        fprintf(fp, "Else Statement: \n");
        _to_ast_rcv(tree_node->children[0], depth+1, fp);

    } else if (strcmp(tree_node->value, "P_else_stmt") == 0) {

        if (tree_node->children_count == 0) {
            space_print(depth, fp);
            fprintf(fp, "// do nothing\n");
        } else if (tree_node->children_count == 5) {
            space_print(depth, fp);
            fprintf(fp, "Body: \n");
            _to_ast_rcv(tree_node->children[1], depth+1, fp);
        } else if (tree_node->children_count == 8) {
            space_print(depth, fp);
            fprintf(fp, "Condition: \n");
            _to_ast_rcv(tree_node->children[5], depth+1, fp);

            space_print(depth, fp);
            fprintf(fp, "Body: \n");
            _to_ast_rcv(tree_node->children[2], depth+1, fp);

            space_print(depth, fp);
            fprintf(fp, "Else Statement: \n");
            _to_ast_rcv(tree_node->children[0], depth+1, fp);
        }
    } 
    
    else if (strcmp(tree_node->value, "P_string_consts") == 0) {
        for (int i = 0; i < tree_node->children_count; i+=2) {
            _to_ast_rcv(tree_node->children[i], depth, fp);
        }       
    } 
    else if (strcmp(tree_node->value, "P_string_const") == 0) {
        if (tree_node->children[0]->children_count == 0) {
            space_print(depth, fp);
            fprintf(fp, "String literal:\n");
            space_print(depth+1, fp);
            fprintf(fp, "%s\n", tree_node->children[0]->value);
        } else if (strcmp(tree_node->children[0]->value, "P_expression") == 0){
            space_print(depth, fp);
            fprintf(fp, "Expression:\n");
            _to_ast_rcv(tree_node->children[0], depth+1, fp);
        } 
    } else if (strcmp(tree_node->value, "P_value") == 0) {
        if (tree_node->children[0]->children_count == 0) {
            space_print(depth, fp);
            fprintf(fp, "String literal: %s\n", tree_node->children[0]->value);
        } else {
            _to_ast_rcv(tree_node->children[0], depth, fp);
        }
    } else if (strcmp(tree_node->value, "P_expression") == 0 ||
             strcmp(tree_node->value, "P_conjunction") == 0 ||
             strcmp(tree_node->value, "P_equality") == 0 ||
             strcmp(tree_node->value, "P_relational") == 0 ||
             strcmp(tree_node->value, "P_term") == 0 ||
             strcmp(tree_node->value, "P_factor") == 0 ||
             strcmp(tree_node->value, "P_power") == 0 ||
             strcmp(tree_node->value, "P_unary") == 0) {

        if (tree_node->children_count == 3) {
        space_print(depth, fp);
        fprintf(fp, "Operator: %s\n", tree_node->children[1]->value);

        space_print(depth, fp);
        
        fprintf(fp, "Right:\n");
        _to_ast_rcv(tree_node->children[0], depth+1, fp);

        space_print(depth, fp);
        fprintf(fp, "Left:\n");
        _to_ast_rcv(tree_node->children[2], depth+1, fp);

        depth++;

        } else {
            _to_ast_rcv(tree_node->children[0], depth, fp);
        }


    } else if (strcmp(tree_node->value, "P_literal") == 0) {
        if (tree_node->children_count == 1) {
            space_print(depth, fp);
            fprintf(fp, "Value: %s\n", tree_node->children[0]->value);
        } else {
            _to_ast_rcv(tree_node->children[1], depth, fp);
        }
    } else if (strcmp(tree_node->value, "P_abs") == 0) {
        if (tree_node->children_count == 1) {
            if (tree_node->children[0]->children_count == 0) {
                space_print(depth, fp);
                fprintf(fp, "Value: %s\n", tree_node->children[0]->value);
            } else {
                _to_ast_rcv(tree_node->children[0], depth, fp);
            }
        } else {
            _to_ast_rcv(tree_node->children[1], depth, fp);
        }
    }
    
    else {
        for (int i = tree_node->children_count - 1; i >= 0; i--) {
            _to_ast_rcv(tree_node->children[i], depth, fp);
        }
    }
}


void to_ast(Tree *parse_tree, FILE *fp) {
    _to_ast_rcv(parse_tree->root, 0, fp);
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
    token_node_t *current_token = token_get(current_idx);
    parsing_state current_state;

    FILE *ast = fopen("ast.txt", "w");

    Tree *parse_tree = tree_create("P_PROGRAM");

    // for state indices
    Stack *state_stack = stack_create();
    stack_push(state_stack, current_state_idx);

    // for tokens
    TokenStack *token_stack = token_stack_create();

    while(is_parsing) {
        current_state = parsing_table[current_state_idx][current_token->type];     // current state from parsing table (start_state - input_string -> current_state)

        // check what action to perform
        switch (current_state.action_type) {

            case SHIFT:
                if (debug) {
                    printf("q%d\t-- T%03d\t SHIFT(%d)\t--> q%d\n", current_state_idx, current_token->type, current_state.value, current_state.value);
                }
                stack_push(state_stack, current_state.value);

                token_stack_push(token_stack, current_token->type, current_token->parse_tree_node);
                current_state_idx = current_state.value;

                if (current_idx  < parser.token_count - 1) {
                    current_idx++;
                }

                current_token = token_get(current_idx);
                _stack_print(token_stack);
                break;
            
            case REDUCE:
                if (debug) {
                    printf("q%d\t-- T%03d\t REDUCE(%d)\t--> q%d\n", current_state_idx, current_token->type, current_state.value, current_state.value);
                }

                Production rule = rules[current_state.value];

                current_token->type = rule.lhs;
                current_token->parse_tree_node->value = input_string_get(rule.lhs); // change to the string value of the type
                if (current_token->parse_tree_node->parent == NULL) {
                    tree_insert(parse_tree->root, current_token->parse_tree_node);
                }

                for (int i = 0; i < rule.rhs_count; i++) {
                    if (rule.rhs[rule.rhs_count-1-i] == token_stack->top->type) {
                        stack_pop(state_stack);
                        token_node_t *popped_token = token_stack_pop(token_stack);
                        tree_insert(current_token->parse_tree_node, popped_token->parse_tree_node);
                    }
                }

                current_state_idx = state_stack->top->value;
                token_stack_push(token_stack, current_token->type, current_token->parse_tree_node);
                _stack_print(token_stack);
                break;

            case GOTO:
                if (debug) {
                    printf("q%d\t-- T%03d\t GOTO(%d)\t--> q%d\n", current_state_idx, current_token->type, current_state.value, current_state.value);
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
                is_parsing = false;
                break;

            default:
                break;
         }
    }
 
    Tree *final_tree = malloc(sizeof(Tree));
    final_tree->root = parse_tree->root->children[parse_tree->root->children_count - 1];
    printf("GENERATING PARSE TREE:\n");
    tree_print(final_tree);

    to_ast(final_tree, ast);
    printf("GENERATING AST: Saved in \033[0;33m`ast.txt`\033[0;37m\n");
    stack_free(state_stack);
    token_stack_free(token_stack);
    fclose(ast);
    free(current_token);
};
