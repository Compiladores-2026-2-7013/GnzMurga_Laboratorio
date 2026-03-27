#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <istream>
#include <string>
#include "Symbols.hpp"

class Lexer
{
public:
    explicit Lexer(std::istream *in);

    int yylex();
    int getLine() const;
    const std::string &getLexeme() const;

private:
    std::istream *input;
    int line;
    std::string lexeme;

    int readChar();
    void unreadChar(int ch);
};

#endif /* __LEXER_HPP__ */
