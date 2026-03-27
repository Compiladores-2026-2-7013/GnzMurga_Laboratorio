#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "Lexer.hpp"
#include "Symbols.hpp"
using namespace std;

namespace C_1
{
  class Parser
  {

  private:
    Lexer *lexer;
    int token;
    bool hasError;

    void match(Token expected);
    bool startsDeclaracion(int tk) const;
    bool startsSentencia(int tk) const;

    void declaraciones();
    void declaracion();
    void tipo();
    void lista_ids();
    void sentencias();
    void sentencia();
    void expr();
    void expr_p();
    void termino();
    
  public:
    Parser();
    Parser(Lexer *lexer);
    ~Parser();
    
    void programa(); //función del símbolo incial
    /**
     *
     *
     * Funciones del resto de NTs 
     *
     *
     */
    
    Token eat();
    void error(string msg);
    void parse();
  };    
    
}


#endif
