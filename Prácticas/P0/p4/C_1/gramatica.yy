%{
#include <iostream>
#include <fstream>
#include <string>
#include <FlexLexer.h>
using namespace std;
%}

%language "C++"
%require  "3.2"
%defines  "Parser.hpp"
%output   "Parser.cpp"

%define api.parser.class {Parser}
%define api.namespace    {calc}

%parse-param {Lexer* lexer}

%code requires
{
    #include <string>
    namespace calc {
        class Lexer;
    }
}

%code
{
    #include "Lexer.hpp"
    #define yylex(x) lexer->lex(x)
}

/* ── Tipos semánticos ── */
%union {
    struct {
        double fval;
        int    tipo;   /* 1: entero, 2: flotante */
    } numero;
    char* ident;
}

/* ── Tokens ── */
%token              TINT TFLOAT
%token              IF ELSE WHILE
%token              ASIGN
%token              PARIZQ PARDER LLAVEIZQ LLAVEDER
%token              COMA PUNTOCOMA
%token <numero>     NUM
%token <ident>      ID

%left  MAS MENOS
%left  MUL DIV
%right UMENOS

%type  <numero>     expresion

%start programa

%%

/* ════════════════════════════════════
   programa → declaraciones sentencias
   ════════════════════════════════════ */
programa
    : declaraciones sentencias
        { cout << "\n✔ Programa analizado correctamente." << endl; }
    ;

/* ════════════════════════════════════
   declaraciones
   ════════════════════════════════════ */
declaraciones
    : declaraciones declaracion
    | declaracion
    ;

declaracion
    : tipo lista_var PUNTOCOMA
        { cout << "  [decl] Declaración reconocida." << endl; }
    ;

tipo
    : TINT   { cout << "  [tipo] int"   << endl; }
    | TFLOAT { cout << "  [tipo] float" << endl; }
    ;

lista_var
    : lista_var COMA ID
        { cout << "  [var]  " << $3 << endl; free($3); }
    | ID
        { cout << "  [var]  " << $1 << endl; free($1); }
    ;

/* ════════════════════════════════════
   sentencias
   ════════════════════════════════════ */
sentencias
    : sentencias sentencia
    | sentencia
    ;

sentencia
    /* Asignación */
    : ID ASIGN expresion PUNTOCOMA
        {
            cout << "  [sent] Asignación → " << $1
                 << " = " << ($3.tipo==1?(int)$3.fval:$3.fval) << endl;
            free($1);
        }

    /* if-else */
    | IF PARIZQ expresion PARDER
        LLAVEIZQ sentencias LLAVEDER
        ELSE
        LLAVEIZQ sentencias LLAVEDER
        { cout << "  [sent] if-else reconocido." << endl; }

    /* while */
    | WHILE PARIZQ expresion PARDER
        LLAVEIZQ sentencias LLAVEDER
        { cout << "  [sent] while reconocido." << endl; }

    /* recuperación de error: consume hasta el siguiente ';' */
    | error PUNTOCOMA
        { yyerrok; cerr << "  [recuperación] Sentencia omitida, continuando..." << endl; }
    ;

/* ════════════════════════════════════
   expresion
   ════════════════════════════════════ */
expresion
    : expresion MAS expresion
        { $$.fval=$1.fval+$3.fval; $$.tipo=($1.tipo==2||$3.tipo==2)?2:1; }
    | expresion MENOS expresion
        { $$.fval=$1.fval-$3.fval; $$.tipo=($1.tipo==2||$3.tipo==2)?2:1; }
    | expresion MUL expresion
        { $$.fval=$1.fval*$3.fval; $$.tipo=($1.tipo==2||$3.tipo==2)?2:1; }
    | expresion DIV expresion
        {
            if ($3.fval == 0) {
                cerr << "Error semántico: división por cero." << endl;
                $$.fval=0; $$.tipo=2;
            } else {
                $$.fval=$1.fval/$3.fval; $$.tipo=2;
            }
        }
    | PARIZQ expresion PARDER
        { $$=$2; }
    | MENOS expresion %prec UMENOS
        { $$.fval=-$2.fval; $$.tipo=$2.tipo; }
    | ID
        { $$.fval=0; $$.tipo=1; free($1); }
    | NUM
        { $$=$1; }
    ;

%%

/* ════════════════════════════════════
   Error: muestra línea y descripción
   del token inesperado (Bison provee
   el nombre del token en `msg`)
   ════════════════════════════════════ */
void calc::Parser::error(const std::string& msg) {
    std::cerr << "\n╔══ ERROR SINTÁCTICO ══════════════════════════╗" << std::endl;
    std::cerr << "║  Detalle : " << msg                              << std::endl;
    std::cerr << "╚═════════════════════════════════════════════╝" << std::endl;
}

/* ════════════════════════════════════
   main
   ════════════════════════════════════ */
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./C_1 <archivo>" << std::endl;
        return EXIT_FAILURE;
    }
    std::filebuf fb;
    fb.open(std::string(argv[1]), std::ios::in);
    if (!fb.is_open()) {
        std::cerr << "Error: no se pudo abrir '" << argv[1] << "'" << std::endl;
        return EXIT_FAILURE;
    }
    std::istream in(&fb);
    calc::Lexer  lexer(&in);
    calc::Parser parser(&lexer);
    int result = parser.parse();
    fb.close();
    return result;
}
