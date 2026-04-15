#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

/** 
 * 1. Definir en un comentario la gramática G'(N', T, P', S)
 * 
 * Gramática G' = (N', Σ, P', programa)
 * 
 * N' = {programa, declaraciones, declaraciones', declaracion, tipo, lista_var, 
 *       lista_var', sentencias, sentencias', sentencia, expresion, expresion', 
 *       termino, termino', factor}
 * 
 * Σ = {int, float, identificador, numero, +, -, *, /, =, ;, ',', (, ), if, else, while, $}
 * 
 */


/**
 * Conjunto T (Terminales)
 */
enum Token { 
    t_int,           // int
    t_float,         // float
    t_id,            // identificador
    t_num,           // numero
    t_plus,          // +
    t_minus,         // -
    t_times,          // *
    t_div,           // /
    t_assign,        // =
    t_semicolon,     // ;
    t_comma,         // ,
    t_lparen,        // (
    t_rparen,        // )
    t_if,            // if
    t_else,          // else
    t_while,         // while
    t_eof,          // EOF (fin de archivo)
    t_error         // Error léxico
};

/**
 * Conjunto N' (No Terminales)
 */
enum NoTerm { 
    nt_empty,              // ε (producción vacía)
    nt_programa,           // programa
    nt_declaraciones,      // declaraciones
    nt_declaraciones_p,    // declaraciones' (prima)
    nt_declaracion,        // declaracion
    nt_tipo,               // tipo
    nt_lista_var,          // lista_var
    nt_lista_var_p,        // lista_var' (prima)
    nt_sentencias,         // sentencias
    nt_sentencias_p,       // sentencias' (prima)
    nt_sentencia,          // sentencia
    nt_S_emparejada,       // S_emparejada
    nt_S_abierta,          // S_abierta
    nt_expresion,          // expresion
    nt_expresion_p,        // expresion' (prima)
    nt_termino,            // termino
    nt_termino_p,          // termino' (prima)
    nt_factor              // factor
};

#endif // !__SYMBOLS_HPP__