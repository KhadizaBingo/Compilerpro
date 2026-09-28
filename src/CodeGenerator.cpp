#include "CodeGenerator.h"

using namespace std;

// ---------------- Helpers ----------------

void CodeGenerator::emitIndent()
{
    for (int i = 0; i < indentLevel; i++)
    {
        out << "    ";
    }
}

string CodeGenerator::cppType(TokenType declaredType)
{
    if (declaredType == TokenType::PURNO)
        return "int";

    if (declaredType == TokenType::DOSOMIK)
        return "double";

    // Falls back to a generic numeric type if something unexpected
    // reaches code generation (should be caught earlier by the parser).
    return "auto";
}

string CodeGenerator::binaryOperator(TokenType opType)
{
    switch (opType)
    {
        case TokenType::PLUS:
            return "+";
        case TokenType::MINUS:
            return "-";
        case TokenType::MULTIPLY:
            return "*";
        case TokenType::DIVIDE:
            return "/";
        case TokenType::EQUAL:
            return "==";
        case TokenType::NOT_EQUAL:
            return "!=";
        case TokenType::LESS:
            return "<";
        case TokenType::GREATER:
            return ">";
        case TokenType::LESS_EQUAL:
            return "<=";
        case TokenType::GREATER_EQUAL:
            return ">=";
        default:
            return "/* unknown op */";
    }
}

string CodeGenerator::escapeString(const string& raw)
{
    string escaped;

    for (char c : raw)
    {
        if (c == '"' || c == '\\')
        {
            escaped += '\\';
        }

        escaped += c;
    }

    return escaped;
}

// ---------------- Expressions ----------------

string CodeGenerator::generateExpr(const Expr* expr)
{
    if (const auto* number = dynamic_cast<const NumberExpr*>(expr))
    {
        return number->value;
    }

    if (const auto* str = dynamic_cast<const StringExpr*>(expr))
    {
        return "\"" + escapeString(str->value) + "\"";
    }

    if (const auto* variable = dynamic_cast<const VariableExpr*>(expr))
    {
        return variable->name;
    }

    if (const auto* binary = dynamic_cast<const BinaryExpr*>(expr))
    {
        return "(" +
               generateExpr(binary->left.get()) +
               " " +
               binaryOperator(binary->op.type) +
               " " +
               generateExpr(binary->right.get()) +
               ")";
    }

    if (const auto* grouping = dynamic_cast<const GroupingExpr*>(expr))
    {
        return "(" + generateExpr(grouping->inner.get()) + ")";
    }

    return "/* unknown expression */";
}

// ---------------- Statements ----------------

void CodeGenerator::generateVarDecl(const VarDeclStmt* stmt)
{
    emitIndent();
    out << cppType(stmt->declaredType)
        << " "
        << stmt->name
        << " = "
        << generateExpr(stmt->initializer.get())
        << ";\n";
}

void CodeGenerator::generateAssign(const AssignStmt* stmt)
{
    emitIndent();
    out << stmt->name
        << " = "
        << generateExpr(stmt->value.get())
        << ";\n";
}

void CodeGenerator::generatePrint(const PrintStmt* stmt)
{
    emitIndent();
    out << "std::cout << "
        << generateExpr(stmt->value.get())
        << " << std::endl;\n";
}

void CodeGenerator::generateBlock(const BlockStmt* stmt)
{
    emitIndent();
    out << "{\n";

    indentLevel++;

    for (const StmtPtr& inner : stmt->statements)
    {
        generateStmt(inner.get());
    }

    indentLevel--;

    emitIndent();
    out << "}\n";
}

void CodeGenerator::generateIf(const IfStmt* stmt)
{
    emitIndent();
    out << "if ("
        << generateExpr(stmt->condition.get())
        << ")\n";

    generateStmt(stmt->thenBranch.get());

    if (stmt->elseBranch)
    {
        emitIndent();
        out << "else\n";
        generateStmt(stmt->elseBranch.get());
    }
}

void CodeGenerator::generateWhile(const WhileStmt* stmt)
{
    emitIndent();
    out << "while ("
        << generateExpr(stmt->condition.get())
        << ")\n";

    generateStmt(stmt->body.get());
}

void CodeGenerator::generateStmt(const Stmt* stmt)
{
    if (const auto* varDecl = dynamic_cast<const VarDeclStmt*>(stmt))
    {
        generateVarDecl(varDecl);
        return;
    }

    if (const auto* assign = dynamic_cast<const AssignStmt*>(stmt))
    {
        generateAssign(assign);
        return;
    }

    if (const auto* print = dynamic_cast<const PrintStmt*>(stmt))
    {
        generatePrint(print);
        return;
    }

    if (const auto* block = dynamic_cast<const BlockStmt*>(stmt))
    {
        generateBlock(block);
        return;
    }

    if (const auto* ifStmt = dynamic_cast<const IfStmt*>(stmt))
    {
        generateIf(ifStmt);
        return;
    }

    if (const auto* whileStmt = dynamic_cast<const WhileStmt*>(stmt))
    {
        generateWhile(whileStmt);
        return;
    }

    emitIndent();
    out << "// unknown statement\n";
}

// ---------------- Entry point ----------------

string CodeGenerator::generate(const vector<StmtPtr>& program)
{
    out.str("");
    indentLevel = 1;

    out << "// Auto-generated by BanglishCompiler. Do not edit by hand.\n";
    out << "#include <iostream>\n";
    out << "\n";
    out << "int main()\n";
    out << "{\n";

    for (const StmtPtr& stmt : program)
    {
        generateStmt(stmt.get());
    }

    emitIndent();
    out << "return 0;\n";
    out << "}\n";

    return out.str();
}
