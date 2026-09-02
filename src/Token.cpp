#include "Token.h"

#include <sstream>

using namespace std;


// Default constructor
Token::Token()
{
    type = TokenType::UNKNOWN;
    value = "";
    line = 0;
}


// Constructor
Token::Token(TokenType type, string value, int line)
{
    this->type = type;
    this->value = value;
    this->line = line;
}


// Convert token to readable text
string Token::toString() const
{
    stringstream output;

    output << tokenTypeToString(type)
           << " : "
           << value
           << " (Line "
           << line
           << ")";

    return output.str();
}


// Convert token type into text
string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::DHORO:
            return "DHORO";

        case TokenType::PURNO:
            return "PURNO";

        case TokenType::DOSOMIK:
            return "DOSOMIK";

        case TokenType::DEKHAO:
            return "DEKHAO";

        case TokenType::JODI:
            return "JODI";

        case TokenType::NAHOLE:
            return "NAHOLE";

        case TokenType::JOTOKKHON:
            return "JOTOKKHON";

        case TokenType::IDENTIFIER:
            return "IDENTIFIER";

        case TokenType::NUMBER:
            return "NUMBER";

        case TokenType::STRING:
            return "STRING";

        case TokenType::PLUS:
            return "PLUS";

        case TokenType::MINUS:
            return "MINUS";

        case TokenType::MULTIPLY:
            return "MULTIPLY";

        case TokenType::DIVIDE:
            return "DIVIDE";

        case TokenType::ASSIGN:
            return "ASSIGN";

        case TokenType::EQUAL:
            return "EQUAL";

        case TokenType::NOT_EQUAL:
            return "NOT_EQUAL";

        case TokenType::LESS:
            return "LESS";

        case TokenType::GREATER:
            return "GREATER";

        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";

        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";

        case TokenType::SEMICOLON:
            return "SEMICOLON";

        case TokenType::COMMA:
            return "COMMA";

        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";

        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";

        case TokenType::LEFT_BRACE:
            return "LEFT_BRACE";

        case TokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";

        case TokenType::END_OF_FILE:
            return "EOF";

        case TokenType::UNKNOWN:
            return "UNKNOWN";
    }

    return "UNKNOWN";
}
