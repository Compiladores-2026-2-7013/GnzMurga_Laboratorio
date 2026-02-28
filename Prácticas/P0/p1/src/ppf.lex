%{
    #include <iostream>
%}

%option c++
%option noyywrap

digito [0-9]
letra [a-zA-Z]
palabra {letra}+
espacio [ \t\n]

%%

{espacio} {/* La accion lexica puede ir vacia si queremos que el escaner ignore la regla*/}
{digito}+ { std::cout << "Encontre un numero: " << yytext << std::endl; }
{palabra} { std::cout << "Encontre una palabra: " << yytext << std::endl; }

%%

int main() {
    FlexLexer* lexer = new yyFlexLexer;
    lexer->yylex();
}