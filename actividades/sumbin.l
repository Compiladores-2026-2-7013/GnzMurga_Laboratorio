%option noyywrap
%{
    #include <stdlib.h>
    long long suma = 0;
%}
%%
0b[01]+  { suma += strtoll(yytext + 2, NULL, 2); }
.|\n     { /* ignorar */ }
%%
int main(int argc, char **argv) {
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
    printf("%lld\n", suma);
    return 0;
}