#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include <vector>
#include <string>

using namespace std;

class Parser
{
private:
    vector<Token> tokens;
    int current;

    // Basic parser functions
    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, string message);

    // Grammar functions
    void program();
    void statement();
    void declaration();
    void assignment();
    void printStatement();
    void ifStatement();
    void whileStatement();

    // Expression functions
    void expression();
    void comparison();
    void term();
    void factor();
    void primary();

    // Error recovery
    void synchronize();

    // Error message
    void error(Token token, string message);

public:
    Parser(vector<Token> tokens);

    void parse();
};

#endif