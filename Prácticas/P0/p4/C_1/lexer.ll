%{
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "Parser.hpp"
#include "Lexer.hpp"

using namespace std;

#undef YY_DECL
#define YY_DECL int calc::Lexer::lex(calc::Parser::semantic_type *yylval)
%}

%option c++
%option noyywrap
%option yylineno
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

%%

[ \t\r]+                { /* Ignorar espacios en blanco */ }

\n                      { /* ignorar saltos de línea */ }

"//".*                  { /* Ignorar comentarios de línea */ }

"int"                   { return Parser::token::TINT;      }
"float"                 { return Parser::token::TFLOAT;    }
"if"                    { return Parser::token::IF;        }
"else"                  { return Parser::token::ELSE;      }
"while"                 { return Parser::token::WHILE;     }

"+"                     { return Parser::token::MAS;       }
"-"                     { return Parser::token::MENOS;     }
"*"                     { return Parser::token::MUL;       }
"/"                     { return Parser::token::DIV;       }
"="                     { return Parser::token::ASIGN;     }
"("                     { return Parser::token::PARIZQ;    }
")"                     { return Parser::token::PARDER;    }
"{"                     { return Parser::token::LLAVEIZQ;  }
"}"                     { return Parser::token::LLAVEDER;  }
","                     { return Parser::token::COMA;      }
";"                     { return Parser::token::PUNTOCOMA; }

[0-9]+(\.[0-9]+)?       {
                            yylval->numero.fval = atof(yytext);
                            yylval->numero.tipo = (strchr(yytext, '.') != NULL) ? 2 : 1;
                            return Parser::token::NUM;
                        }

[a-zA-Z_][a-zA-Z0-9_]* {
                            yylval->ident = strdup(yytext);
                            return Parser::token::ID;
                        }

.                       {
                            cerr << "Error léxico en línea " << yylineno
                                 << ": carácter no reconocido '" << yytext << "'" << endl;
                        }

<<EOF>>                 { yyterminate(); }

%%
