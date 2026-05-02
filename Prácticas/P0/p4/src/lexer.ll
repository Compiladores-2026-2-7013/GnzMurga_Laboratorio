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
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

%%

[ \t\r]+                 { /* Ignorar espacios en blanco */ }

\n                       { return '\n'; }

"//".*                   { /* Ignorar comentarios */ }

"+"	{
	 cout << "Terminal + detectado" << endl;	
	 return Parser::token::MAS;
	}

"-"	{
	 cout << "Terminal - detectado" << endl;	
	 return Parser::token::MENOS;
	}

"*"	{
	 cout << "Terminal * detectado" << endl;	
	 return Parser::token::MUL;
	}

"/"	{
	 cout << "Terminal / detectado" << endl;	
	 return Parser::token::DIV;
	}

"("	{
	 cout << "Terminal ( detectado" << endl;	
	 return Parser::token::PARIZQ;
	}

")"	{
	 cout << "Terminal ) detectado" << endl;	
	 return Parser::token::PARDER;
	}

[0-9]+(\.[0-9]+)?	{
	 cout << "Terminal numérico " << yytext << " detectado" << endl;
	 yylval->numero.fval = atof(yytext);
	 yylval->numero.tipo = (strchr(yytext, '.') != NULL) ? 2 : 1; /* 1: Entero, 2: Flotante */
	 return Parser::token::NUM;
	}

.	{
	  cout << "Error léxico: carácter no reconocido '" << yytext << "'" << endl;
	}

<<EOF>> {	
         yyterminate();
        }

%%