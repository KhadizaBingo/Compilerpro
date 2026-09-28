#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Token.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

enum class DataType
{
    PURNO_TYPE,
    DOSOMIK_TYPE,
    STRING_TYPE,
    UNKNOWN_TYPE
};

class SemanticAnalyzer
{
private:
    vector<Token> tokens;
    int current;

    // Stores variable name and its type
    map<string, DataType> symbolTable;

    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);

    void program();
    void statement();

    void declaration();
    void assignment();
    void printStatement();
    void ifStatement();
    void whileStatement();

    DataType expression();
    DataType comparison();
    DataType term();
    DataType factor();
    DataType primary();

    DataType tokenToDataType(TokenType type);

    void error(Token token, string message);

public:
    SemanticAnalyzer(vector<Token> tokens);

    void analyze();
};

#endif