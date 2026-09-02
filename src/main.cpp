#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Token.h"
#include "Lexer.h"

using namespace std;


int main()
{
    // Open source file
    ifstream file("tests/declaration.bng");


    // Check file
    if (!file.is_open())
    {
        cout << "Error: Could not open source file."
             << endl;

        return 1;
    }


    // Read entire file
    stringstream buffer;

    buffer << file.rdbuf();

    string source = buffer.str();


    file.close();


    // Create Lexer
    Lexer lexer(source);


    // Generate tokens
    vector<Token> tokens = lexer.tokenize();


    // Display tokens
    cout << "=============================="
         << endl;

    cout << "       BANG LEXER OUTPUT"
         << endl;

    cout << "=============================="
         << endl;


    for (const Token& token : tokens)
    {
        cout << token.toString()
             << endl;
    }


    return 0;
}