#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

/** 
 * 1. Definir en un comentario la gramática G(N, T, P, S)
 *
 * G(N, T, P, programa)
 *
 * N = {
 *   programa, declaraciones, declaracion, tipo, lista_ids,
 *   sentencias, sentencia, expr, expr_p, termino
 * }
 *
 * T = {
 *   int, float, id, num, ',', ';', '=', '+', '(', ')', EOF
 * }
 *
 * P:
 *   programa      -> declaraciones sentencias EOF
 *   declaraciones -> declaracion declaraciones | e
 *   declaracion   -> tipo lista_ids ';'
 *   tipo          -> int | float
 *   lista_ids     -> id lista_ids_p
 *   lista_ids_p   -> ',' id lista_ids_p | e
 *   sentencias    -> sentencia sentencias | e
 *   sentencia     -> id '=' expr ';'
 *   expr          -> termino expr_p
 *   expr_p        -> '+' termino expr_p | e
 *   termino       -> id | num | '(' expr ')'
 */


/**
 * 2. Determinar la enumeración para el conjunto T
 */
enum Token {
	primero = 0,
	t_int,
	t_float,
	t_id,
	t_num,
	t_comma,
	t_semicolon,
	t_assign,
	t_plus,
	t_lparen,
	t_rparen,
	t_eof,
	t_error
};

/**
 * 3. Determinar la enumeración para el conjunto N
 */
enum NoTerm {
	strEmpty = 0,
	nt_programa,
	nt_declaraciones,
	nt_declaracion,
	nt_tipo,
	nt_lista_ids,
	nt_sentencias,
	nt_sentencia,
	nt_expr,
	nt_expr_p,
	nt_termino
};

#endif // !__SYMBOLS_HPP__
