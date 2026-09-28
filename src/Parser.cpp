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

// ---------------- Entry point ----------------

vector<StmtPtr> Parser::parse()
{
    vector<StmtPtr> statements;

    while (!isAtEnd())
    {
        StmtPtr stmt = statement();

        if (stmt)
        {
            statements.push_back(move(stmt));
        }
    }

    return statements;
}

// ---------------- Statements ----------------

StmtPtr Parser::statement()
{
    try
    {
        if (match(TokenType::DHORO))
        {
            return declaration();
        }
        else if (match(TokenType::DEKHAO))
        {
            return printStatement();
        }
        else if (match(TokenType::JODI))
        {
            return ifStatement();
        }
        else if (match(TokenType::JOTOKKHON))
        {
            return whileStatement();
        }
        else if (check(TokenType::LEFT_BRACE))
        {
            return block();
        }
        else if (check(TokenType::IDENTIFIER))
        {
            return assignment();
        }
        else
        {
            error(peek(), "Unexpected token.");
            synchronize();
            return nullptr;
        }
    }
    catch (...)
    {
        synchronize();
        return nullptr;
    }
}

// dhoro (purno | dosomik) IDENTIFIER = expression ;
StmtPtr Parser::declaration()
{
    TokenType declaredType;

    if (match(TokenType::PURNO))
    {
        declaredType = TokenType::PURNO;
    }
    else if (match(TokenType::DOSOMIK))
    {
        declaredType = TokenType::DOSOMIK;
    }
    else
    {
        error(peek(), "Expected data type.");
        synchronize();
        return nullptr;
    }

    Token name = consume(
        TokenType::IDENTIFIER,
        "Expected variable name."
    );

    consume(
        TokenType::ASSIGN,
        "Expected '=' after variable name."
    );

    ExprPtr initializer = expression();

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after declaration."
    );

    return make_unique<VarDeclStmt>(declaredType, name.value, move(initializer));
}

// IDENTIFIER = expression ;
StmtPtr Parser::assignment()
{
    Token name = consume(
        TokenType::IDENTIFIER,
        "Expected variable name."
    );

    consume(
        TokenType::ASSIGN,
        "Expected '=' after variable name."
    );

    ExprPtr value = expression();

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after assignment."
    );

    return make_unique<AssignStmt>(name.value, move(value));
}

// dekhao ( expression ) ;
StmtPtr Parser::printStatement()
{
    consume(
        TokenType::LEFT_PAREN,
        "Expected '(' after dekhao."
    );

    ExprPtr value = expression();

    consume(
        TokenType::RIGHT_PAREN,
        "Expected ')' after expression."
    );

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after dekhao statement."
    );

    return make_unique<PrintStmt>(move(value));
}

// { statement* }
StmtPtr Parser::block()
{
    consume(
        TokenType::LEFT_BRACE,
        "Expected '{' to start block."
    );

    vector<StmtPtr> statements;

    while (!check(TokenType::RIGHT_BRACE) &&
           !isAtEnd())
    {
        StmtPtr stmt = statement();

        if (stmt)
        {
            statements.push_back(move(stmt));
        }
    }

    consume(
        TokenType::RIGHT_BRACE,
        "Expected '}' after block."
    );

    return make_unique<BlockStmt>(move(statements));
}

// jodi ( expression ) { ... } (nahole { ... })?
StmtPtr Parser::ifStatement()
{
    consume(
        TokenType::LEFT_PAREN,
        "Expected '(' after jodi."
    );

    ExprPtr condition = expression();

    consume(
        TokenType::RIGHT_PAREN,
        "Expected ')' after condition."
    );

    StmtPtr thenBranch = block();
    StmtPtr elseBranch = nullptr;

    // Optional nahole
    if (match(TokenType::NAHOLE))
    {
        elseBranch = block();
    }

    return make_unique<IfStmt>(move(condition), move(thenBranch), move(elseBranch));
}

// jotokkhon ( expression ) { ... }
StmtPtr Parser::whileStatement()
{
    consume(
        TokenType::LEFT_PAREN,
        "Expected '(' after jotokkhon."
    );

    ExprPtr condition = expression();

    consume(
        TokenType::RIGHT_PAREN,
        "Expected ')' after condition."
    );

    StmtPtr body = block();

    return make_unique<WhileStmt>(move(condition), move(body));
}

// ---------------- Expressions ----------------

ExprPtr Parser::expression()
{
    return comparison();
}

ExprPtr Parser::comparison()
{
    ExprPtr expr = term();

    while (check(TokenType::EQUAL) ||
           check(TokenType::NOT_EQUAL) ||
           check(TokenType::LESS) ||
           check(TokenType::GREATER) ||
           check(TokenType::LESS_EQUAL) ||
           check(TokenType::GREATER_EQUAL))
    {
        Token op = advance();
        ExprPtr right = term();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

ExprPtr Parser::term()
{
    ExprPtr expr = factor();

    while (check(TokenType::PLUS) ||
           check(TokenType::MINUS))
    {
        Token op = advance();
        ExprPtr right = factor();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

ExprPtr Parser::factor()
{
    ExprPtr expr = primary();

    while (check(TokenType::MULTIPLY) ||
           check(TokenType::DIVIDE))
    {
        Token op = advance();
        ExprPtr right = primary();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

ExprPtr Parser::primary()
{
    if (match(TokenType::NUMBER))
    {
        Token token = previous();
        bool isDecimal = token.value.find('.') != string::npos;
        return make_unique<NumberExpr>(token.value, isDecimal);
    }

    if (match(TokenType::STRING))
    {
        return make_unique<StringExpr>(previous().value);
    }

    if (match(TokenType::IDENTIFIER))
    {
        return make_unique<VariableExpr>(previous().value);
    }

    if (match(TokenType::LEFT_PAREN))
    {
        ExprPtr expr = expression();

        consume(
            TokenType::RIGHT_PAREN,
            "Expected ')' after expression."
        );

        return make_unique<GroupingExpr>(move(expr));
    }

    error(
        peek(),
        "Expected expression."
    );

    synchronize();

    // Return a harmless placeholder so the AST stays well-formed
    // even after a syntax error, letting the parser keep going.
    return make_unique<NumberExpr>("0", false);
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
