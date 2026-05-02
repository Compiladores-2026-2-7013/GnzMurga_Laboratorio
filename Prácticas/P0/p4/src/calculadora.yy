%{
#include <iostream>
#include <fstream>
#include <string>
#include <FlexLexer.h>
using namespace std;
%}

%language "C++"
%require "3.2"
%defines "Parser.hpp"
%output "Parser.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {calc}
%parse-param {Lexer* lexer}

%code requires
{
    namespace calc {
        class Lexer;
    } // namespace calc
} // %code requires
 
%code
{
 #include "Lexer.hpp"
 #define yylex(x) lexer->lex(x) // Referencia a 1 en Lexer.hpp
}

%union {
  struct {
    double fval;  // Cambiado a double para soportar flotantes (Ejercicio 1)
    int tipo;     // 1: Entero, 2: Flotante
  } numero;
}

%token<numero> NUM

%left MAS MENOS    // Agregado MENOS (Ejercicio 3)
%left MUL DIV      // Agregado DIV (Ejercicio 3)

%nonassoc PARIZQ
%nonassoc PARDER

%type<numero> expresion

%start programa    // Cambiado para múltiples líneas (Ejercicio 2)

%%

/* Ejercicio 2: Reconocer múltiples líneas */
programa: /* vacío */
        | programa line
        ;

line: expresion '\n' { 
        if ($1.tipo == 1)
            cout << "Resultado: " << (int)$1.fval << endl;
        else
            cout << "Resultado: " << $1.fval << endl;
     }
    | expresion { 
        if ($1.tipo == 1)
            cout << "Resultado: " << (int)$1.fval << endl;
        else
            cout << "Resultado: " << $1.fval << endl;
     }
    ;

/* Operaciones aritméticas con soporte para flotantes */
expresion : expresion MAS expresion { 
              $$.fval = $1.fval + $3.fval; 
              $$.tipo = ($1.tipo == 2 || $3.tipo == 2) ? 2 : 1;
          }
	  | expresion MENOS expresion {  // Ejercicio 3: Resta
              $$.fval = $1.fval - $3.fval; 
              $$.tipo = ($1.tipo == 2 || $3.tipo == 2) ? 2 : 1;
          }
	  | expresion MUL expresion { 
              $$.fval = $1.fval * $3.fval; 
              $$.tipo = ($1.tipo == 2 || $3.tipo == 2) ? 2 : 1;
          }
	  | expresion DIV expresion {  // Ejercicio 3: División
              if ($3.fval == 0) {
                  cerr << "Error semántico: División por cero detectada" << endl;
                  $$.fval = 0;
                  $$.tipo = 2;
              } else {
                  $$.fval = $1.fval / $3.fval; 
                  $$.tipo = 2; // División siempre produce flotante
              }
          }
	  | PARIZQ expresion PARDER { 
              $$ = $2; 
          }
	  | MENOS expresion %prec MUL {  // Ejercicio 4: Números negativos
              $$.fval = -$2.fval; 
              $$.tipo = $2.tipo;
          }
	  | NUM { 
              $$ = $1; 
          }
	  ;

%%

void calc::Parser::error(const std::string& msg) {
    std::cerr << "Error sintáctico: " << msg << std::endl;
    std::cerr << "Por favor, verifica la sintaxis de tu expresión." << std::endl;
}


#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char *argv[]){
    if(argc < 2){
        cout<<"Faltan argumentos "<<endl;
        return EXIT_FAILURE;
    }
    filebuf fb;
    fb.open(string(argv[1]), ios::in);
    istream in(&fb);
    calc::Lexer lexer(&in);
    calc::Parser parser(&lexer);
    parser.parse();
    fb.close();
    return 0;
    /*
    calc::Lexer scanner{ std::cin, std::cerr };
    calc::Parser parser{ &scanner };
    //std::cout.precision(10);
    parser.parse();*/
}
