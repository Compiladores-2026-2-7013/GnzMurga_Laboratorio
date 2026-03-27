%option noyywrap
%{
    #include <ctype.h>
%}
%%
[a-z]+  {
            for(int i = 0; i < yyleng; i++)
                fprintf(yyout, "%c", toupper(yytext[i]));
        }
.|\n    { ECHO; }
%%
int main(int argc, char **argv) {
    yyin  = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");
    yylex();
    fclose(yyin);
    fclose(yyout);
    return 0;
}
