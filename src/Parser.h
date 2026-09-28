<<<<<<< HEAD
#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include <vector>
#include <string>
=======
#pragma once

#include "Token.h"

#include <string>
#include <vector>
>>>>>>> 0fbaf83e220dddb1c691a0bb4b7ee1d578062a83

using namespace std;

class Parser
{
<<<<<<< HEAD
private:
    vector<Token> tokens;
    int current;

    // Basic parser functions
=======
public:
    explicit Parser(vector<Token> tokens);

    void parse();

private:
    vector<Token> tokens;
    size_t current;

>>>>>>> 0fbaf83e220dddb1c691a0bb4b7ee1d578062a83
    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
<<<<<<< HEAD
=======

>>>>>>> 0fbaf83e220dddb1c691a0bb4b7ee1d578062a83
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, string message);

<<<<<<< HEAD
    // Grammar functions
=======
    void error(Token token, string message);

>>>>>>> 0fbaf83e220dddb1c691a0bb4b7ee1d578062a83
    void program();
    void statement();
    void declaration();
    void assignment();
    void printStatement();
    void ifStatement();
    void whileStatement();

<<<<<<< HEAD
    // Expression functions
=======
>>>>>>> 0fbaf83e220dddb1c691a0bb4b7ee1d578062a83
    void expression();
    void comparison();
    void term();
    void factor();
    void primary();

<<<<<<< HEAD
    // Error recovery
    void synchronize();

    // Error message
    void error(Token token, string message);

public:
    Parser(vector<Token> tokens);

    void parse();
};

#endif
=======
    void synchronize();
};
>>>>>>> 0fbaf83e220dddb1c691a0bb4b7ee1d578062a83
