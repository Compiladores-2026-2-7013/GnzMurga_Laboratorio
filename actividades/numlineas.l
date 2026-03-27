%option noyywrap yylineno
%%
\n  { printf("\n%d ", yylineno); }
.   { printf("%c", yytext[0]); }
%%
int main(int argc, char **argv) {
    yyin = fopen(argv[1], "r");
    printf("1 ");
    yylex();
    fclose(yyin);
    return 0;
}