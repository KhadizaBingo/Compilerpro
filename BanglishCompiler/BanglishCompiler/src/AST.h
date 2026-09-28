#pragma once
#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

#include "Token.h"

using namespace std;



//  Expressions


// Base class for every expression node (numbers, strings,
// variables, binary operations, parenthesized expressions).
class Expr
{
public:
    virtual ~Expr() = default;
};

using ExprPtr = unique_ptr<Expr>;


// Numeric literal, e.g. 10 or 25.5
class NumberExpr : public Expr
{
public:
    string value;
    bool isDecimal; // true if the literal contained a '.'

    NumberExpr(string value, bool isDecimal);
};


// String literal, e.g. "hello"
class StringExpr : public Expr
{
public:
    string value;

    explicit StringExpr(string value);
};


// Reference to a previously declared variable
class VariableExpr : public Expr
{
public:
    string name;

    explicit VariableExpr(string name);
};


// Binary operation, e.g. x + y, x <= 10
class BinaryExpr : public Expr
{
public:
    ExprPtr left;
    Token op;
    ExprPtr right;

    BinaryExpr(ExprPtr left, Token op, ExprPtr right);
};


// Parenthesized expression, e.g. (x + y)
class GroupingExpr : public Expr
{
public:
    ExprPtr inner;

    explicit GroupingExpr(ExprPtr inner);
};



//  Statements


// Base class for every statement node.
class Stmt
{
public:
    virtual ~Stmt() = default;
};

using StmtPtr = unique_ptr<Stmt>;


// dhoro (purno | dosomik) IDENTIFIER = expression ;
class VarDeclStmt : public Stmt
{
public:
    TokenType declaredType; // TokenType::PURNO or TokenType::DOSOMIK
    string name;
    ExprPtr initializer;

    VarDeclStmt(TokenType declaredType, string name, ExprPtr initializer);
};


// IDENTIFIER = expression ;
class AssignStmt : public Stmt
{
public:
    string name;
    ExprPtr value;

    AssignStmt(string name, ExprPtr value);
};


// dekhao ( expression ) ;
class PrintStmt : public Stmt
{
public:
    ExprPtr value;

    explicit PrintStmt(ExprPtr value);
};


// { statement* }
class BlockStmt : public Stmt
{
public:
    vector<StmtPtr> statements;

    explicit BlockStmt(vector<StmtPtr> statements);
};


// jodi ( expression ) block (nahole block)?
class IfStmt : public Stmt
{
public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch; // nullptr when there is no 'nahole' branch

    IfStmt(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch);
};


// jotokkhon ( expression ) block
class WhileStmt : public Stmt
{
public:
    ExprPtr condition;
    StmtPtr body;

    WhileStmt(ExprPtr condition, StmtPtr body);
};

#endif
