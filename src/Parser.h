#pragma once

#include "Token.h"
#include "AST.h"

#include <string>
#include <vector>

using namespace std;

class Parser
{
public:
    explicit Parser(vector<Token> tokens);

    // Parses the whole token stream and returns the program
    // as a list of top-level statements (the AST).
    vector<StmtPtr> parse();

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

    StmtPtr statement();
    StmtPtr declaration();
    StmtPtr assignment();
    StmtPtr printStatement();
    StmtPtr ifStatement();
    StmtPtr whileStatement();
    StmtPtr block();

    ExprPtr expression();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr primary();

    void synchronize();
};
