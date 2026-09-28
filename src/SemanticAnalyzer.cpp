#include "SemanticAnalyzer.h"
#include <iostream>

using namespace std;

// ---------------- Constructor ----------------

SemanticAnalyzer::SemanticAnalyzer(vector<Token> tokens)
{
    this->tokens = tokens;
    this->current = 0;
}

// ---------------- Token helpers ----------------

bool SemanticAnalyzer::isAtEnd()
{
    return peek().type == TokenType::END_OF_FILE;
}

Token SemanticAnalyzer::peek()
{
    return tokens[current];
}

Token SemanticAnalyzer::previous()
{
    return tokens[current - 1];
}

Token SemanticAnalyzer::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}

bool SemanticAnalyzer::check(TokenType type)
{
    if (isAtEnd())
        return false;
    return peek().type == type;
}

bool SemanticAnalyzer::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

// ---------------- Type helpers ----------------

DataType SemanticAnalyzer::tokenToDataType(TokenType type)
{
    if (type == TokenType::PURNO)
        return DataType::PURNO_TYPE;
    if (type == TokenType::DOSOMIK)
        return DataType::DOSOMIK_TYPE;
    if (type == TokenType::STRING)
        return DataType::STRING_TYPE;

    return DataType::UNKNOWN_TYPE;
}

void SemanticAnalyzer::error(Token token, string message)
{
    cout << "Semantic Error at line "
         << token.line
         << ": "
         << message
         << endl;
}

// ---------------- Entry point ----------------

void SemanticAnalyzer::analyze()
{
    program();
    cout << "Semantic Analysis Completed." << endl;
}

void SemanticAnalyzer::program()
{
    while (!isAtEnd())
    {
        statement();
    }
}

// ---------------- Statements ----------------

void SemanticAnalyzer::statement()
{
    if (check(TokenType::DHORO))
    {
        declaration();
    }
    else if (check(TokenType::IDENTIFIER))
    {
        assignment();
    }
    else if (check(TokenType::DEKHAO))
    {
        printStatement();
    }
    else if (check(TokenType::JODI))
    {
        ifStatement();
    }
    else if (check(TokenType::JOTOKKHON))
    {
        whileStatement();
    }
    else if (check(TokenType::LEFT_BRACE))
    {
        // Block: { statement* }
        advance(); // consume '{'
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
        {
            statement();
        }
        if (!match(TokenType::RIGHT_BRACE))
        {
            error(peek(), "Expected '}' to close block.");
        }
    }
    else
    {
        error(peek(), "Unexpected token '" + peek().value + "'.");
        advance(); // avoid infinite loop on bad input
    }
}

// dhoro (purno | dosomik) IDENTIFIER = expression ;
void SemanticAnalyzer::declaration()
{
    Token dhoroToken = advance(); // consume 'dhoro'

    if (!check(TokenType::PURNO) && !check(TokenType::DOSOMIK))
    {
        error(peek(), "Expected type keyword 'purno' or 'dosomik' after 'dhoro'.");
        return;
    }

    Token typeToken = advance(); // consume type keyword
    DataType declaredType = tokenToDataType(typeToken.type);

    if (!check(TokenType::IDENTIFIER))
    {
        error(peek(), "Expected variable name after type keyword.");
        return;
    }

    Token nameToken = advance(); // consume identifier

    if (symbolTable.find(nameToken.value) != symbolTable.end())
    {
        error(nameToken, "Variable '" + nameToken.value + "' is already declared.");
    }
    else
    {
        symbolTable[nameToken.value] = declaredType;
    }

    if (!match(TokenType::ASSIGN))
    {
        error(peek(), "Expected '=' after variable name in declaration.");
        return;
    }

    DataType valueType = expression();

    if (valueType != declaredType && valueType != DataType::UNKNOWN_TYPE)
    {
        error(nameToken, "Type mismatch: cannot assign value of a different type to '" + nameToken.value + "'.");
    }

    if (!match(TokenType::SEMICOLON))
    {
        error(peek(), "Expected ';' after declaration.");
    }
}

// IDENTIFIER = expression ;
void SemanticAnalyzer::assignment()
{
    Token nameToken = advance(); // consume identifier

    auto it = symbolTable.find(nameToken.value);
    if (it == symbolTable.end())
    {
        error(nameToken, "Variable '" + nameToken.value + "' was not declared before use.");
    }

    if (!match(TokenType::ASSIGN))
    {
        error(peek(), "Expected '=' after identifier in assignment.");
        return;
    }

    DataType valueType = expression();

    if (it != symbolTable.end() && valueType != it->second && valueType != DataType::UNKNOWN_TYPE)
    {
        error(nameToken, "Type mismatch: cannot assign this value to '" + nameToken.value + "'.");
    }

    if (!match(TokenType::SEMICOLON))
    {
        error(peek(), "Expected ';' after assignment.");
    }
}

// dekhao expression ;
void SemanticAnalyzer::printStatement()
{
    advance(); // consume 'dekhao'
    expression();

    if (!match(TokenType::SEMICOLON))
    {
        error(peek(), "Expected ';' after print statement.");
    }
}

// jodi ( expression ) { ... } (nahole { ... })?
void SemanticAnalyzer::ifStatement()
{
    advance(); // consume 'jodi'

    if (!match(TokenType::LEFT_PAREN))
    {
        error(peek(), "Expected '(' after 'jodi'.");
        return;
    }

    expression(); // condition type isn't restricted here

    if (!match(TokenType::RIGHT_PAREN))
    {
        error(peek(), "Expected ')' after condition.");
        return;
    }

    statement(); // handles the '{' block

    if (check(TokenType::NAHOLE))
    {
        advance(); // consume 'nahole'
        statement(); // else block
    }
}

// jotokkhon ( expression ) { ... }
void SemanticAnalyzer::whileStatement()
{
    advance(); // consume 'jotokkhon'

    if (!match(TokenType::LEFT_PAREN))
    {
        error(peek(), "Expected '(' after 'jotokkhon'.");
        return;
    }

    expression();

    if (!match(TokenType::RIGHT_PAREN))
    {
        error(peek(), "Expected ')' after condition.");
        return;
    }

    statement(); // handles the '{' block
}

// ---------------- Expressions (recursive descent, with type checking) ----------------

// expression -> comparison ( ( == | != ) comparison )*
DataType SemanticAnalyzer::expression()
{
    DataType type = comparison();

    while (check(TokenType::EQUAL) || check(TokenType::NOT_EQUAL))
    {
        Token op = advance();
        DataType rightType = comparison();

        if (type != rightType && type != DataType::UNKNOWN_TYPE && rightType != DataType::UNKNOWN_TYPE)
        {
            error(op, "Type mismatch in comparison expression.");
        }
    }

    return type;
}

// comparison -> term ( ( < | > | <= | >= ) term )*
DataType SemanticAnalyzer::comparison()
{
    DataType type = term();

    while (check(TokenType::LESS) || check(TokenType::GREATER) ||
           check(TokenType::LESS_EQUAL) || check(TokenType::GREATER_EQUAL))
    {
        Token op = advance();
        DataType rightType = term();

        if (type != rightType && type != DataType::UNKNOWN_TYPE && rightType != DataType::UNKNOWN_TYPE)
        {
            error(op, "Type mismatch in comparison expression.");
        }
    }

    return type;
}

// term -> factor ( ( + | - ) factor )*
DataType SemanticAnalyzer::term()
{
    DataType type = factor();

    while (check(TokenType::PLUS) || check(TokenType::MINUS))
    {
        Token op = advance();
        DataType rightType = factor();

        if (type == DataType::STRING_TYPE || rightType == DataType::STRING_TYPE)
        {
            if (!(op.type == TokenType::PLUS && type == DataType::STRING_TYPE && rightType == DataType::STRING_TYPE))
            {
                error(op, "Invalid operation on string type.");
            }
        }
        else if (type != rightType && type != DataType::UNKNOWN_TYPE && rightType != DataType::UNKNOWN_TYPE)
        {
            error(op, "Type mismatch: cannot mix 'purno' and 'dosomik' without explicit handling.");
        }

        // Numeric promotion: if either side is dosomik, result becomes dosomik
        if (type == DataType::DOSOMIK_TYPE || rightType == DataType::DOSOMIK_TYPE)
        {
            type = DataType::DOSOMIK_TYPE;
        }
    }

    return type;
}

// factor -> primary ( ( * | / ) primary )*
DataType SemanticAnalyzer::factor()
{
    DataType type = primary();

    while (check(TokenType::MULTIPLY) || check(TokenType::DIVIDE))
    {
        Token op = advance();
        DataType rightType = primary();

        if (type == DataType::STRING_TYPE || rightType == DataType::STRING_TYPE)
        {
            error(op, "Invalid operation on string type.");
        }
        else if (type != rightType && type != DataType::UNKNOWN_TYPE && rightType != DataType::UNKNOWN_TYPE)
        {
            error(op, "Type mismatch: cannot mix 'purno' and 'dosomik' without explicit handling.");
        }

        if (type == DataType::DOSOMIK_TYPE || rightType == DataType::DOSOMIK_TYPE)
        {
            type = DataType::DOSOMIK_TYPE;
        }
    }

    return type;
}

// primary -> NUMBER | STRING | IDENTIFIER | ( expression )
DataType SemanticAnalyzer::primary()
{
    if (check(TokenType::NUMBER))
    {
        Token numToken = advance();
        // Decide purno vs dosomik by checking for a decimal point in the literal
        if (numToken.value.find('.') != string::npos)
        {
            return DataType::DOSOMIK_TYPE;
        }
        return DataType::PURNO_TYPE;
    }

    if (check(TokenType::STRING))
    {
        advance();
        return DataType::STRING_TYPE;
    }

    if (check(TokenType::IDENTIFIER))
    {
        Token idToken = advance();
        auto it = symbolTable.find(idToken.value);
        if (it == symbolTable.end())
        {
            error(idToken, "Variable '" + idToken.value + "' was not declared before use.");
            return DataType::UNKNOWN_TYPE;
        }
        return it->second;
    }

    if (match(TokenType::LEFT_PAREN))
    {
        DataType type = expression();
        if (!match(TokenType::RIGHT_PAREN))
        {
            error(peek(), "Expected ')' after expression.");
        }
        return type;
    }

    error(peek(), "Expected an expression but found '" + peek().value + "'.");
    advance(); // avoid infinite loop
    return DataType::UNKNOWN_TYPE;
}