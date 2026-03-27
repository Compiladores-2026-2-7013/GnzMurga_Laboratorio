%option noyywrap
%{
    int lineas = 0, palabras = 0, bytes = 0;
%}
%%
\n          { lineas++; bytes++; }
[^ \t\n]+   { palabras++; bytes += yyleng; }
.           { bytes++; }
%%
int main(int argc, char **argv) {
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
    printf("%d %d %d\n", lineas, palabras, bytes);
    return 0;
}