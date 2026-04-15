%{
#include <iostream>
#include <string>
using namespace std;

#include "headers/Symbols.hpp"
#include "headers/Lexer.hpp"

int line=1;
%}

ID      [a-zA-Z_][a-zA-Z0-9_]*
NUM     [0-9]+

%option c++
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

%%

[ \t\r]+           {}
"\n"               {line++;}

"int"              {return t_int;}
"float"            {return t_float;}
"if"               {return t_if;}
"else"             {return t_else;}
"while"            {return t_while;}

{ID}               {return t_id;}
{NUM}              {return t_num;}

"+"                {return t_plus;}
"-"                {return t_minus;}
"*"                {return t_times;}
"/"                {return t_div;}
"="                {return t_assign;}
";"                {return t_semicolon;}
","                {return t_comma;}
"("                {return t_lparen;}
")"                {return t_rparen;}

<<EOF>>            {return t_eof;}

.                  {cout << "ERROR LEXICO en linea " << line << ": " << yytext << endl; return t_error;}

%%

int yyFlexLexer::yywrap(){
    return 1;
}

int Lexer::getLine()
{
    return line;
}