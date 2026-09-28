#include "Lexer.h"
#include "Lexer.h"

#include <iostream>

using namespace std;


// Constructor
Lexer::Lexer(string source)
{
    this->source = source;
    this->position = 0;
    this->line = 1;
}


// Check if we reached the end
bool Lexer::isAtEnd()
{
    return position >= source.length();
}


// Return current character
char Lexer::peek()
{
    if (isAtEnd())
    {
        return '\0';
    }

    return source[position];
}


// Return next character
char Lexer::peekNext()
{
    if (position + 1 >= source.length())
    {
        return '\0';
    }

    return source[position + 1];
}


// Read current character
// Then move to next position
char Lexer::advance()
{
    char current = source[position];

    position++;

    return current;
}


// Check digit
bool Lexer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}


// Check letter
bool Lexer::isLetter(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}


// Check letter or digit
bool Lexer::isLetterOrDigit(char c)
{
    return isLetter(c) || isDigit(c);
}


// Check keywords
TokenType Lexer::checkKeyword(string word)
{
    if (word == "dhoro")
    {
        return TokenType::DHORO;
    }

    if (word == "purno")
    {
        return TokenType::PURNO;
    }

    if (word == "dosomik")
    {
        return TokenType::DOSOMIK;
    }

    if (word == "dekhao")
    {
        return TokenType::DEKHAO;
    }

    if (word == "jodi")
    {
        return TokenType::JODI;
    }

    if (word == "nahole")
    {
        return TokenType::NAHOLE;
    }

    if (word == "jotokkhon")
    {
        return TokenType::JOTOKKHON;
    }


    // If it is not a keyword,
    // it is an identifier
    return TokenType::IDENTIFIER;
}


// Read a number
Token Lexer::readNumber()
{
    int startLine = line;

    string number = "";


    // Read integer part
    while (isDigit(peek()))
    {
        number += advance();
    }


    // Check decimal point
    if (peek() == '.' && isDigit(peekNext()))
    {
        number += advance();


        // Read decimal part
        while (isDigit(peek()))
        {
            number += advance();
        }
    }


    return Token(
        TokenType::NUMBER,
        number,
        startLine
    );
}


// Read identifier
// Example: x, total, studentName
Token Lexer::readIdentifier()
{
    int startLine = line;

    string word = "";


    while (isLetterOrDigit(peek()))
    {
        word += advance();
    }


    // Check whether it is keyword
    TokenType type = checkKeyword(word);


    return Token(
        type,
        word,
        startLine
    );
}


// Read string
// Example: "Hello"
Token Lexer::readString()
{
    int startLine = line;

    string value = "";


    // Remove opening quotation mark
    advance();


    // Read until closing quotation
    while (!isAtEnd() && peek() != '"')
    {
        if (peek() == '\n')
        {
            line++;
        }

        value += advance();
    }


    // String did not close
    if (isAtEnd())
    {
        cout << "Lexer Error: String not closed at line "
             << startLine
             << endl;

        return Token(
            TokenType::UNKNOWN,
            value,
            startLine
        );
    }


    // Remove closing quotation mark
    advance();


    return Token(
        TokenType::STRING,
        value,
        startLine
    );
}


// Skip // comment
void Lexer::skipComment()
{
    while (!isAtEnd() && peek() != '\n')
    {
        advance();
    }
}


// Main Lexer function
vector<Token> Lexer::tokenize()
{
    vector<Token> tokens;


    // Continue until complete source is read
    while (!isAtEnd())
    {
        char c = peek();


        // =====================================
        // WHITESPACE
        // =====================================

        if (c == ' ' ||
            c == '\t' ||
            c == '\r')
        {
            advance();
            continue;
        }


        // New line
        if (c == '\n')
        {
            line++;

            advance();

            continue;
        }


        // =====================================
        // COMMENTS
        // =====================================

        if (c == '/' && peekNext() == '/')
        {
            skipComment();

            continue;
        }


        // =====================================
        // NUMBERS
        // =====================================

        if (isDigit(c))
        {
            tokens.push_back(readNumber());

            continue;
        }


        // =====================================
        // IDENTIFIER / KEYWORD
        // =====================================

        if (isLetter(c))
        {
            tokens.push_back(readIdentifier());

            continue;
        }


        // =====================================
        // STRING
        // =====================================

        if (c == '"')
        {
            tokens.push_back(readString());

            continue;
        }


        // =====================================
        // PLUS
        // =====================================

        if (c == '+')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::PLUS,
                    "+",
                    line
                )
            );

            continue;
        }


        // =====================================
        // MINUS
        // =====================================

        if (c == '-')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::MINUS,
                    "-",
                    line
                )
            );

            continue;
        }


        // =====================================
        // MULTIPLY
        // =====================================

        if (c == '*')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::MULTIPLY,
                    "*",
                    line
                )
            );

            continue;
        }


        // =====================================
        // DIVIDE
        // =====================================

        if (c == '/')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::DIVIDE,
                    "/",
                    line
                )
            );

            continue;
        }


        // =====================================
        // = or ==
        // =====================================

        if (c == '=')
        {
            advance();


            if (peek() == '=')
            {
                advance();

                tokens.push_back(
                    Token(
                        TokenType::EQUAL,
                        "==",
                        line
                    )
                );
            }
            else
            {
                tokens.push_back(
                    Token(
                        TokenType::ASSIGN,
                        "=",
                        line
                    )
                );
            }


            continue;
        }


        // =====================================
        // !=
        // =====================================

        if (c == '!')
        {
            advance();


            if (peek() == '=')
            {
                advance();

                tokens.push_back(
                    Token(
                        TokenType::NOT_EQUAL,
                        "!=",
                        line
                    )
                );
            }
            else
            {
                cout << "Lexer Error: Unexpected ! at line "
                     << line
                     << endl;

                tokens.push_back(
                    Token(
                        TokenType::UNKNOWN,
                        "!",
                        line
                    )
                );
            }


            continue;
        }


        // =====================================
        // < or <=
        // =====================================

        if (c == '<')
        {
            advance();


            if (peek() == '=')
            {
                advance();

                tokens.push_back(
                    Token(
                        TokenType::LESS_EQUAL,
                        "<=",
                        line
                    )
                );
            }
            else
            {
                tokens.push_back(
                    Token(
                        TokenType::LESS,
                        "<",
                        line
                    )
                );
            }


            continue;
        }


        // =====================================
        // > or >=
        // =====================================

        if (c == '>')
        {
            advance();


            if (peek() == '=')
            {
                advance();

                tokens.push_back(
                    Token(
                        TokenType::GREATER_EQUAL,
                        ">=",
                        line
                    )
                );
            }
            else
            {
                tokens.push_back(
                    Token(
                        TokenType::GREATER,
                        ">",
                        line
                    )
                );
            }


            continue;
        }


        // =====================================
        // SEMICOLON
        // =====================================

        if (c == ';')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::SEMICOLON,
                    ";",
                    line
                )
            );

            continue;
        }


        // =====================================
        // COMMA
        // =====================================

        if (c == ',')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::COMMA,
                    ",",
                    line
                )
            );

            continue;
        }


        // =====================================
        // (
        // =====================================

        if (c == '(')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::LEFT_PAREN,
                    "(",
                    line
                )
            );

            continue;
        }


        // =====================================
        // )
        // =====================================

        if (c == ')')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::RIGHT_PAREN,
                    ")",
                    line
                )
            );

            continue;
        }


        // =====================================
        // {
        // =====================================

        if (c == '{')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::LEFT_BRACE,
                    "{",
                    line
                )
            );

            continue;
        }


        // =====================================
        // }
        // =====================================

        if (c == '}')
        {
            advance();

            tokens.push_back(
                Token(
                    TokenType::RIGHT_BRACE,
                    "}",
                    line
                )
            );

            continue;
        }


        // =====================================
        // UNKNOWN CHARACTER
        // =====================================

        cout << "Lexer Error: Unknown character '"
             << c
             << "' at line "
             << line
             << endl;


        tokens.push_back(
            Token(
                TokenType::UNKNOWN,
                string(1, c),
                line
            )
        );


        // Move forward so compiler does not get stuck
        advance();
    }


    // Add EOF token
    tokens.push_back(
        Token(
            TokenType::END_OF_FILE,
            "",
            line
        )
    );


    return tokens;
}