#pragma once

#include "Token.h"

#include <string>
#include <vector>

using namespace std;

class Parser
{
public:
    explicit Parser(vector<Token> tokens);

    void parse();

private:
    vector<Token> tokens;
    size_t current;

    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();

    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, string message);

    void error(Token token, string message);

    void program();
    void statement();
    void declaration();
    void assignment();
    void printStatement();
    void ifStatement();
    void whileStatement();

    void expression();
    void comparison();
    void term();
    void factor();
    void primary();

    void synchronize();
};
