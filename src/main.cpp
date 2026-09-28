#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Token.h"
#include "Lexer.h"
#include "AST.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "CodeGenerator.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Path to the .bng source file. Defaults to the declaration test file,
    // but any file can be passed: BanglishCompiler.exe tests/loop.bng
    string sourcePath = (argc > 1) ? argv[1] : "tests/declaration.bng";

    // Open source file
    ifstream file(sourcePath);

    // Check file
    if (!file.is_open())
    {
        cout << "Error: Could not open source file '" << sourcePath << "'." << endl;
        return 1;
    }

    // Read entire file
    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();
    file.close();

    // ---------------- Lexing ----------------

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "==============================" << endl;
    cout << "       BANG LEXER OUTPUT" << endl;
    cout << "==============================" << endl;

    for (const Token& token : tokens)
    {
        cout << token.toString() << endl;
    }

    // ---------------- Semantic Analysis ----------------

    cout << endl;
    cout << "==============================" << endl;
    cout << "   SEMANTIC ANALYZER OUTPUT" << endl;
    cout << "==============================" << endl;

    SemanticAnalyzer analyzer(tokens);
    analyzer.analyze();

    // ---------------- Parsing (build the AST) ----------------

    cout << endl;
    cout << "==============================" << endl;
    cout << "       BANG PARSER OUTPUT" << endl;
    cout << "==============================" << endl;

    Parser parser(tokens);
    vector<StmtPtr> program = parser.parse();

    cout << "Parsed " << program.size() << " top-level statement(s)." << endl;

    // ---------------- Code Generation ----------------

    cout << endl;
    cout << "==============================" << endl;
    cout << "     GENERATED C++ SOURCE" << endl;
    cout << "==============================" << endl;

    CodeGenerator generator;
    string generatedCode = generator.generate(program);

    cout << generatedCode << endl;

    // Write the generated C++ out so it can be compiled on its own,
    // e.g.: g++ generated/output.cpp -o generated/output.exe
    string outputPath = "generated/output.cpp";
    ofstream outFile(outputPath);

    if (outFile.is_open())
    {
        outFile << generatedCode;
        outFile.close();
        cout << "Generated C++ written to " << outputPath << endl;
    }
    else
    {
        cout << "Error: Could not write generated code to " << outputPath << "." << endl;
    }

    return 0;
}
