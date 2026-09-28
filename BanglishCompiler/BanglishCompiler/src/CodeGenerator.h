#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "AST.h"
#include "Token.h"

using namespace std;

class CodeGenerator
{
public:
    CodeGenerator() = default;
    virtual ~CodeGenerator() = default;

    // Walks the parsed program and returns a full, self-contained
    // .cpp source file that reproduces the same behaviour in real C++.
    string generate(const vector<StmtPtr>& program);

private:
    ostringstream out;
    int indentLevel;

    void emitIndent();

    void generateStmt(const Stmt* stmt);
    void generateVarDecl(const VarDeclStmt* stmt);
    void generateAssign(const AssignStmt* stmt);
    void generatePrint(const PrintStmt* stmt);
    void generateBlock(const BlockStmt* stmt);
    void generateIf(const IfStmt* stmt);
    void generateWhile(const WhileStmt* stmt);

    string generateExpr(const Expr* expr);

    string cppType(TokenType declaredType);
    string binaryOperator(TokenType opType);
    string escapeString(const string& raw);
};
