%option noyywrap yylineno
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    FILE *fout;
%}

%%
"if"|"then"|"else"|"while"|"do"|"case"|"is"|"void"|"true"|"false"|"begin"|"end"|"not"
                { fprintf(fout, "<RESERVADA, %s>\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*
                { fprintf(fout, "<IDENTIFICADOR, %s>\n", yytext); }

[0-9]+\.[0-9]+  { fprintf(fout, "<REAL, %s>\n", yytext); }

[0-9]+          { fprintf(fout, "<ENTERO, %s>\n", yytext); }

\"[^\"]*\"      { fprintf(fout, "<CADENA, %s>\n", yytext); }

"--"[^\n]*\n    { fprintf(fout, "<COMENTARIO_LINEA, %s>\n", yytext); }

"<*"([^*]|\*[^>])*"*>"
                { fprintf(fout, "<COMENTARIO_BLOQUE, %s>\n", yytext); }

"+"|"-"|"*"|"/"|"%"
                { fprintf(fout, "<OP_ARIT, %s>\n", yytext); }

">"|"<"|"<>"|"="
                { fprintf(fout, "<OP_REL, %s>\n", yytext); }

":="            { fprintf(fout, "<ASIGNACION, %s>\n", yytext); }

"("|")"|"{"|"}"|";"|","
                { fprintf(fout, "<ESPECIAL, %s>\n", yytext); }

[ \t\n]+        { fprintf(fout, "<ESPACIO, %s>\n", yytext); }

.               { fprintf(fout, "Error: caracter '%s' en linea %d\n", yytext, yylineno); }

%%
int main(int argc, char **argv) {
    if(argc < 2) { fprintf(stderr, "Uso: %s archivo.art\n", argv[0]); return 1; }

    /* Verificar extension .art */
    char *ext = strrchr(argv[1], '.');
    if(!ext || strcmp(ext, ".art") != 0) {
        fprintf(stderr, "Error: se requiere archivo .art\n"); return 1;
    }

    /* Construir nombre de salida .tokens */
    char outname[256];
    strncpy(outname, argv[1], ext - argv[1]);
    outname[ext - argv[1]] = '\0';
    strcat(outname, ".tokens");

    yyin = fopen(argv[1], "r");
    fout = fopen(outname, "w");
    yylex();
    fclose(yyin);
    fclose(fout);
    return 0;
}