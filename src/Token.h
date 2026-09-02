#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;


// All types of tokens in our language
enum class TokenType
{
    // Keywords
    DHORO,
    PURNO,
    DOSOMIK,
    DEKHAO,
    JODI,
    NAHOLE,
    JOTOKKHON,

    // Identifier
    IDENTIFIER,

    // Values
    NUMBER,
    STRING,

    // Arithmetic operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    // Assignment and comparison
    ASSIGN,          // =
    EQUAL,           // ==
    NOT_EQUAL,       // !=
    LESS,            // <
    GREATER,         // >
    LESS_EQUAL,      // <=
    GREATER_EQUAL,   // >=

    // Symbols
    SEMICOLON,       // ;
    COMMA,           // ,
    LEFT_PAREN,      // (
    RIGHT_PAREN,     // )
    LEFT_BRACE,      // {
    RIGHT_BRACE,     // }

    // Special
    END_OF_FILE,
    UNKNOWN
};


// Token class
class Token
{
public:

    TokenType type;
    string value;
    int line;


    // Default constructor
    Token();


    // Normal constructor
    Token(TokenType type, string value, int line);


    // Display token
    string toString() const;
};


// Convert TokenType to readable name
string tokenTypeToString(TokenType type);

#endif

