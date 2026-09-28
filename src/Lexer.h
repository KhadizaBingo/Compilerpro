#pragma once
#ifndef LEXER_H
#define LEXER_H

#include "Token.h"

#include <string>
#include <vector>

using namespace std;


class Lexer
{
private:

    // Complete source code
   std::string source;

    // Current position in source
    int position;

    // Current line number
    int line;


    // Check whether source has ended
    bool isAtEnd();


    // Look at current character
    char peek();


    // Look at next character
    char peekNext();


    // Read current character and move forward
    char advance();


    // Check whether character is a digit
    bool isDigit(char c);


    // Check whether character is a letter
    bool isLetter(char c);


    // Check letter or digit
    bool isLetterOrDigit(char c);


    // Read a number
    Token readNumber();


    // Read identifier or keyword
    Token readIdentifier();


    // Read string
    Token readString();


    // Check whether word is a keyword
    TokenType checkKeyword(string word);


    // Skip comment
    void skipComment();


public:

    // Constructor
    Lexer(string source);


    // Convert source code into tokens
    vector<Token> tokenize();
};

#endif
