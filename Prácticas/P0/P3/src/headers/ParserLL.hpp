#ifndef __PARSER_LL_HPP__
#define __PARSER_LL_HPP__
#include "Grammar.h"
#include "Symbols.hpp"
#include "Lexer.hpp"

#include <string>
#include <vector>
#include <map>
#include <stack>
using namespace std;

class ParserLL
{
private:
  Grammar grammar; // Representa la gramática para el reconocimiento sintáctico
  Token currentToken; // Almacena el token que se está procesando actualmente
  Lexer *lexer; // Referencia al analizador léxico
  map<NoTerm, map<Token, int>> parseTable; // Matriz de análisis sintáctico predictivo
  stack<int> parseStack; // Estructura de pila para el análisis sintáctico
  
public:
  ParserLL(Lexer *lexer); // Inicializa el parser con un lexer
  ~ParserLL(); // Libera recursos del parser
  
  void loadGrammar(); // Carga todos los símbolos y producciones de la gramática
  void loadTable(); // Construye la matriz de análisis predictivo
  void advance(); // Obtiene el siguiente token del lexer
  void match(Token expected); // Verifica coincidencia del token esperado
  void error(string msg); // Gestiona errores sintácticos
  void parse(); // Ejecuta el algoritmo de análisis sintáctico predictivo
};

#endif // __PARSER_LL_HPP__
