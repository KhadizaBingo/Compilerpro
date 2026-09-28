#include "AST.h"

using namespace std;


// ---------------- Expressions ----------------

NumberExpr::NumberExpr(string value, bool isDecimal)
{
    this->value = value;
    this->isDecimal = isDecimal;
}

StringExpr::StringExpr(string value)
{
    this->value = value;
}

VariableExpr::VariableExpr(string name)
{
    this->name = name;
}

BinaryExpr::BinaryExpr(ExprPtr left, Token op, ExprPtr right)
{
    this->left = move(left);
    this->op = op;
    this->right = move(right);
}

GroupingExpr::GroupingExpr(ExprPtr inner)
{
    this->inner = move(inner);
}


// ---------------- Statements ----------------

VarDeclStmt::VarDeclStmt(TokenType declaredType, string name, ExprPtr initializer)
{
    this->declaredType = declaredType;
    this->name = name;
    this->initializer = move(initializer);
}

AssignStmt::AssignStmt(string name, ExprPtr value)
{
    this->name = name;
    this->value = move(value);
}

PrintStmt::PrintStmt(ExprPtr value)
{
    this->value = move(value);
}

BlockStmt::BlockStmt(vector<StmtPtr> statements)
{
    this->statements = move(statements);
}

IfStmt::IfStmt(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch)
{
    this->condition = move(condition);
    this->thenBranch = move(thenBranch);
    this->elseBranch = move(elseBranch);
}

WhileStmt::WhileStmt(ExprPtr condition, StmtPtr body)
{
    this->condition = move(condition);
    this->body = move(body);
}
