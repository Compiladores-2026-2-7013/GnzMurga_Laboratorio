#include "headers/Lexer.hpp"

#include <cctype>

Lexer::Lexer(std::istream *in) : input(in), line(1), lexeme("") {}

int Lexer::readChar() {
    return input->get();
}

void Lexer::unreadChar(int ch) {
    if (ch != EOF) {
        input->putback(static_cast<char>(ch));
    }
}

int Lexer::yylex() {
    lexeme.clear();

    int ch = readChar();

    while (ch != EOF && std::isspace(static_cast<unsigned char>(ch))) {
        if (ch == '\n') {
            ++line;
        }
        ch = readChar();
    }

    if (ch == EOF) {
        return t_eof;
    }

    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
        lexeme.push_back(static_cast<char>(ch));
        ch = readChar();

        while (ch != EOF &&
               (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            lexeme.push_back(static_cast<char>(ch));
            ch = readChar();
        }

        unreadChar(ch);

        if (lexeme == "int") {
            return t_int;
        }
        if (lexeme == "float") {
            return t_float;
        }
        return t_id;
    }

    if (std::isdigit(static_cast<unsigned char>(ch))) {
        lexeme.push_back(static_cast<char>(ch));
        ch = readChar();

        while (ch != EOF && std::isdigit(static_cast<unsigned char>(ch))) {
            lexeme.push_back(static_cast<char>(ch));
            ch = readChar();
        }

        unreadChar(ch);
        return t_num;
    }

    lexeme.push_back(static_cast<char>(ch));

    switch (ch) {
        case ',':
            return t_comma;
        case ';':
            return t_semicolon;
        case '=':
            return t_assign;
        case '+':
            return t_plus;
        case '(':
            return t_lparen;
        case ')':
            return t_rparen;
        default:
            return t_error;
    }
}

int Lexer::getLine() const {
    return line;
}

const std::string &Lexer::getLexeme() const {
    return lexeme;
}
