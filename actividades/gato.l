%option noyywrap
%{
    #include <string.h>
    char board[3][3];
    int row = 0, col = 0;

    void check_winner() {
        char w = 0;
        for(int i=0;i<3;i++) {
            if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][0]!='_')
                w = board[i][0];
            if(board[0][i]==board[1][i] && board[1][i]==board[2][i] && board[0][i]!='_')
                w = board[0][i];
        }
        if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[0][0]!='_') w=board[0][0];
        if(board[0][2]==board[1][1] && board[1][1]==board[2][0] && board[0][2]!='_') w=board[0][2];

        int lleno = 1;
        for(int i=0;i<3;i++) for(int j=0;j<3;j++) if(board[i][j]=='_') lleno=0;

        if(w) printf("gana %c\n", w);
        else if(lleno) printf("empate\n");
        else printf("sin ganador\n");
        row=0; col=0;
    }
%}
%%
[XO_]   {
    board[row][col++] = yytext[0];
}
\n      {
    if(col > 0) { row++; col=0; }
    if(row == 3) check_winner();
}
.       { /* ignorar */ }
%%
int main(int argc, char **argv) {
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
    return 0;
}