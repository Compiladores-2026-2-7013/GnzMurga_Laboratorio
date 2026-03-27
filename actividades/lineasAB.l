%option noyywrap
%{
    #include <stdlib.h>
    int suma = 0;
    char linebuf[4096];
    int pos = 0;
%}
%%
[0-9]+[ \t]*"A"\n  {
    /* extraer y sumar números */
    char *p = yytext;
    int s = 0;
    while(*p) {
        if(*p >= '0' && *p <= '9') s += strtol(p, &p, 10);
        else p++;
    }
    printf("%d\n", s);
}
[^\n]*"B"\n  { printf("%s", yytext); }
.|\n         { /* ignorar resto */ }
%%
int main(int argc, char **argv) {
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
    return 0;
}