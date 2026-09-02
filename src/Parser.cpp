#include "Parser.h"
#include <iostream>

using namespace std;

Parser::Parser(vector<Token> tokens)
{
    this->tokens = tokens;
    this->current = 0;
}

bool Parser::isAtEnd()
{
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

Token Parser::advance()
{
    if (!isAtEnd())
        current++;

    return previous();
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;

    return peek().type == type;
}

bool Parser::match(TokenType type)
{
    if (!check(type))
        return false;

    advance();
    return true;
}

Token Parser::consume(TokenType type, string message)
{
    if (check(type))
        return advance();

    error(peek(), message);

    return Token();
}

void Parser::error(Token token, string message)
{
    cout << "Parser Error at line "
         << token.line
         << ": "
         << message
         << endl;
}

void Parser::parse()
{
    cout << "     " << endl;
    cout << "       BANG PARSER" << endl;
    cout << "      " << endl;

    program();

    cout << endl;
    cout << "Parsing completed." << endl;
}

void Parser::program()
{
    while (!isAtEnd())
    {
        statement();
    }
}

void Parser::statement()
{
    try
    {
        if (match(TokenType::DHORO))
        {
            declaration();
        }
        else if (match(TokenType::DEKHAO))
        {
            printStatement();
        }
        else if (match(TokenType::JODI))
        {
            ifStatement();
        }
        else if (match(TokenType::JOTOKKHON))
        {
            whileStatement();
        }
        else if (check(TokenType::IDENTIFIER))
        {
            assignment();
        }
        else
        {
            error(peek(), "Unexpected token.");
            synchronize();
        }
    }
    catch (...)
    {
        synchronize();
    }
}

void Parser::declaration()
{
    // dhoro purno x = 10;
    // dhoro dosomik price = 25.5;

    if (!match(TokenType::PURNO) &&
        !match(TokenType::DOSOMIK))
    {
        error(peek(), "Expected data type.");
        synchronize();
        return;
    }

    consume(
        TokenType::IDENTIFIER,
        "Expected variable name."
    );

    consume(
        TokenType::ASSIGN,
        "Expected '=' after variable name."
    );

    expression();

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after declaration."
    );
}

void Parser::assignment()
{
    // x = x + 5;

    consume(
        TokenType::IDENTIFIER,
        "Expected variable name."
    );

    consume(
        TokenType::ASSIGN,
        "Expected '=' after variable name."
    );

    expression();

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after assignment."
    );
}

void Parser::printStatement()
{
    // dekhao(x);

    consume(
        TokenType::LEFT_PAREN,
        "Expected '(' after dekhao."
    );

    expression();

    consume(
        TokenType::RIGHT_PAREN,
        "Expected ')' after expression."
    );

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after dekhao statement."
    );
}

void Parser::ifStatement()
{
    // jodi (x > 5) { ... }

    consume(
        TokenType::LEFT_PAREN,
        "Expected '(' after jodi."
    );

    expression();

    consume(
        TokenType::RIGHT_PAREN,
        "Expected ')' after condition."
    );

    consume(
        TokenType::LEFT_BRACE,
        "Expected '{' before if block."
    );

    while (!check(TokenType::RIGHT_BRACE) &&
           !isAtEnd())
    {
        statement();
    }

    consume(
        TokenType::RIGHT_BRACE,
        "Expected '}' after if block."
    );

    // Optional nahole
    if (match(TokenType::NAHOLE))
    {
        consume(
            TokenType::LEFT_BRACE,
            "Expected '{' before else block."
        );

        while (!check(TokenType::RIGHT_BRACE) &&
               !isAtEnd())
        {
            statement();
        }

        consume(
            TokenType::RIGHT_BRACE,
            "Expected '}' after else block."
        );
    }
}

void Parser::whileStatement()
{
    // jotokkhon (x <= 10) { ... }

    consume(
        TokenType::LEFT_PAREN,
        "Expected '(' after jotokkhon."
    );

    expression();

    consume(
        TokenType::RIGHT_PAREN,
        "Expected ')' after condition."
    );

    consume(
        TokenType::LEFT_BRACE,
        "Expected '{' before while block."
    );

    while (!check(TokenType::RIGHT_BRACE) &&
           !isAtEnd())
    {
        statement();
    }

    consume(
        TokenType::RIGHT_BRACE,
        "Expected '}' after while block."
    );
}

void Parser::expression()
{
    comparison();
}

void Parser::comparison()
{
    term();

    while (match(TokenType::EQUAL) ||
           match(TokenType::NOT_EQUAL) ||
           match(TokenType::LESS) ||
           match(TokenType::GREATER) ||
           match(TokenType::LESS_EQUAL) ||
           match(TokenType::GREATER_EQUAL))
    {
        term();
    }
}

void Parser::term()
{
    factor();

    while (match(TokenType::PLUS) ||
           match(TokenType::MINUS))
    {
        factor();
    }
}

void Parser::factor()
{
    primary();

    while (match(TokenType::MULTIPLY) ||
           match(TokenType::DIVIDE))
    {
        primary();
    }
}

void Parser::primary()
{
    if (match(TokenType::NUMBER))
        return;

    if (match(TokenType::STRING))
        return;

    if (match(TokenType::IDENTIFIER))
        return;

    if (match(TokenType::LEFT_PAREN))
    {
        expression();

        consume(
            TokenType::RIGHT_PAREN,
            "Expected ')' after expression."
        );

        return;
    }

    error(
        peek(),
        "Expected expression."
    );

    synchronize();
}

void Parser::synchronize()
{
    // Basic error recovery.
    // Skip tokens until we find a semicolon

    while (!isAtEnd())
    {
        if (previous().type == TokenType::SEMICOLON)
            return;

        if (check(TokenType::DHORO) ||
            check(TokenType::DEKHAO) ||
            check(TokenType::JODI) ||
            check(TokenType::JOTOKKHON))
        {
            return;
        }

        advance();
    }
}