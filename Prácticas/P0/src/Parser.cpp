#include "headers/Parser.hpp"
#include <iostream>

namespace C_1
{
  Parser::Parser(/* args */)
  {
    lexer = nullptr;
    token = t_error;
    hasError = false;
  }

  Parser::Parser(Lexer *lexer)
  {
    this->lexer = lexer;
    token = t_error;
    hasError = false;
  }
    
  Parser::~Parser()
  {
  }

  void Parser::programa(){
    declaraciones();
    sentencias();
  }

  /************************************************************************/
  /**                                                                    **/
  /**                       Funciones por cada NT                        **/
  /**                                                                    **/
  /************************************************************************/


  Token Parser::eat(){
    return (Token) lexer->yylex();
  }

  void Parser::error(string msg){
    hasError = true;
    cout << "ERROR DE SINTAXIS " << msg << " en la linea " << lexer->getLine() << endl;
  }

  void Parser::match(Token expected){
    if(token == expected){
      token = eat();
      return;
    }

    error("se esperaba token " + to_string(expected) + " y se encontro '" + lexer->getLexeme() + "'");
  }

  bool Parser::startsDeclaracion(int tk) const {
    return tk == t_int || tk == t_float;
  }

  bool Parser::startsSentencia(int tk) const {
    return tk == t_id;
  }

  void Parser::declaraciones(){
    while(startsDeclaracion(token) && !hasError){
      declaracion();
    }
  }

  void Parser::declaracion(){
    tipo();
    lista_ids();
    match(t_semicolon);
  }

  void Parser::tipo(){
    if(token == t_int){
      match(t_int);
      return;
    }
    if(token == t_float){
      match(t_float);
      return;
    }
    error("se esperaba tipo (int|float)");
  }

  void Parser::lista_ids(){
    match(t_id);
    while(token == t_comma && !hasError){
      match(t_comma);
      match(t_id);
    }
  }

  void Parser::sentencias(){
    while(startsSentencia(token) && !hasError){
      sentencia();
    }
  }

  void Parser::sentencia(){
    match(t_id);
    match(t_assign);
    expr();
    match(t_semicolon);
  }

  void Parser::expr(){
    termino();
    expr_p();
  }

  void Parser::expr_p(){
    while(token == t_plus && !hasError){
      match(t_plus);
      termino();
    }
  }

  void Parser::termino(){
    if(token == t_id){
      match(t_id);
      return;
    }
    if(token == t_num){
      match(t_num);
      return;
    }
    if(token == t_lparen){
      match(t_lparen);
      expr();
      match(t_rparen);
      return;
    }
    error("se esperaba id, num o expresion parentizada");
  }   

  void Parser::parse(){
    token = eat();
    programa();
    if(!hasError && token == t_eof)
      cout << "La cadena es aceptada" << endl;
    else 
      cout << "La cadena no pertenece al lenguaje generado por la gramática" << endl;
  }

}
