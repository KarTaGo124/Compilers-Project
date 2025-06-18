#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

/**
 * Scanner constructor
 * Initializes the scanner with input source code
 */
Scanner::Scanner(const char *s) : input(s), first(0), current(0) {}

/**
 * Helper function to check if character is whitespace
 */
bool is_white_space(char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

/**
 * Main tokenization method
 * Returns the next token from the input stream
 */
Token *Scanner::nextToken()
{
    Token *token;

    // Skip whitespace
    while (current < input.length() && is_white_space(input[current]))
        current++;

    // Check for end of file
    if (current >= input.length())
        return new Token(Token::END);

    char c = input[current];
    first = current;

    // Process numeric literals
    if (isdigit(c))
    {
        current++;
        bool is_float = false;
        bool is_int = false;
        bool has_f = false;

        // Parse integer part
        while (current < input.length() && isdigit(input[current]))
            current++;

        // Check for decimal point
        if (current < input.length() && input[current] == '.')
        {
            // Check if it's a range operator (..)
            if (current + 1 < input.length() && input[current + 1] == '.')
            {
                is_int = true;
            }
            else
            {
                is_float = true;
                current++;
                // Parse fractional part
                while (current < input.length() && isdigit(input[current]))
                    current++;
            }
        }
        else
        {
            is_int = true;
        } // Check for 'f' suffix in float literals
        if (is_float && current < input.length() && input[current] == 'f')
        {
            has_f = true;
            current++;
            token = new Token(Token::DECIMAL, input, first, current - first);
            token->has_f = has_f;
        }
        else if (is_float)
        {
            // Float without 'f' suffix - generate error
            token = new Token(Token::ERR, input, first, current - first);
        }
        // Integer with 'f' suffix - treat as float
        else if (is_int && current < input.length() && input[current] == 'f')
        {
            has_f = true;
            current++;
            token = new Token(Token::DECIMAL, input, first, current - first);
            token->has_f = has_f;
        }
        else
        {
            // Regular integer
            token = new Token(Token::NUM, input, first, current - first);
        }
    }
    // Process identifiers and keywords
    else if (isalpha(c))
    {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first); // Map keywords to tokens
        if (word == "Int")
        {
            token = new Token(Token::INT, word, 0, word.length());
        }
        else if (word == "Float")
        {
            token = new Token(Token::FLOAT, word, 0, word.length());
        }
        else if (word == "String")
        {
            token = new Token(Token::STRING_TYPE, word, 0, word.length());
        }
        else if (word == "Boolean")
        {
            token = new Token(Token::BOOLEAN, word, 0, word.length());
        }
        else if (word == "print")
        {
            token = new Token(Token::PRINT, word, 0, word.length());
        }
        else if (word == "println")
        {
            token = new Token(Token::PRINTLN, word, 0, word.length());
        }
        else if (word == "if")
        {
            token = new Token(Token::IF, word, 0, word.length());
        }
        else if (word == "else")
        {
            token = new Token(Token::ELSE, word, 0, word.length());
        }
        else if (word == "while")
        {
            token = new Token(Token::WHILE, word, 0, word.length());
        }
        else if (word == "do")
        {
            token = new Token(Token::DO, word, 0, word.length());
        }
        else if (word == "for")
        {
            token = new Token(Token::FOR, word, 0, word.length());
        }
        else if (word == "in")
        {
            token = new Token(Token::IN, word, 0, word.length());
        }
        else if (word == "var")
        {
            token = new Token(Token::VAR, word, 0, word.length());
        }
        else if (word == "val")
        {
            token = new Token(Token::VAL, word, 0, word.length());
        }
        else if (word == "fun")
        {
            token = new Token(Token::FUN, word, 0, word.length());
        }
        else if (word == "return")
        {
            token = new Token(Token::RETURN, word, 0, word.length());
        }
        else if (word == "break")
        {
            token = new Token(Token::BREAK, word, 0, word.length());
        }
        else if (word == "continue")
        {
            token = new Token(Token::CONTINUE, word, 0, word.length());
        }
        else if (word == "true")
        {
            token = new Token(Token::TRUE, word, 0, word.length());
        }
        else if (word == "false")
        {
            token = new Token(Token::FALSE, word, 0, word.length());
        }
        else if (word == "null")
        {
            token = new Token(Token::NULL_LITERAL, word, 0, word.length());
        }
        else if (word == "until")
        {
            token = new Token(Token::UNTIL, word, 0, word.length());
        }
        else
        {
            // Regular identifier
            token = new Token(Token::ID, word, 0, word.length());
        }
    } // Process operators and delimiters
    else if (strchr(":+-*/%()=;,<>!&|{}.", c))
    {
        switch (c)
        {
        case '+':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::PLUS_ASSIGN, "+=", 0, 2);
                current++;
            }
            else if (current + 1 < input.length() && input[current + 1] == '+')
            {
                token = new Token(Token::INCREMENT, "++", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::PLUS, c);
            }
            break;
        case '-':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::MINUS_ASSIGN, "-=", 0, 2);
                current++;
            }
            else if (current + 1 < input.length() && input[current + 1] == '-')
            {
                token = new Token(Token::DECREMENT, "--", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::MINUS, c);
            }
            break;
        case '*':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::MUL_ASSIGN, "*=", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::MUL, c);
            }
            break;
        case '/':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::DIV_ASSIGN, "/=", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::DIV, c);
            }
            break;
        case '%':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::MOD_ASSIGN, "%=", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::MOD, c);
            }
            break;
        case ',':
            token = new Token(Token::COMMA, c);
            break;
        case '(':
            token = new Token(Token::LEFT_PAREN, c);
            break;
        case ')':
            token = new Token(Token::RIGHT_PAREN, c);
            break;
        case '{':
            token = new Token(Token::LEFT_BRACE, c);
            break;
        case '}':
            token = new Token(Token::RIGHT_BRACE, c);
            break;
        case ':':
            token = new Token(Token::COLON, c);
            break;
        case '=':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::EQ, "==", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::ASSIGN, c);
            }
            break;
        case '<':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::LE, "<=", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::LT, c);
            }
            break;
        case '>':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::GE, ">=", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::GT, c);
            }
            break;
        case '!':
            if (current + 1 < input.length() && input[current + 1] == '=')
            {
                token = new Token(Token::NE, "!=", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::NOT, c);
            }
            break;
        case '&':
            if (current + 1 < input.length() && input[current + 1] == '&')
            {
                token = new Token(Token::AND, "&&", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::ERR, c);
            }
            break;
        case '|':
            if (current + 1 < input.length() && input[current + 1] == '|')
            {
                token = new Token(Token::OR, "||", 0, 2);
                current++;
            }
            else
            {
                token = new Token(Token::ERR, c);
            }
            break;
        case '.':
            if (current + 1 < input.length() && input[current + 1] == '.')
            {
                size_t start = current;
                current += 2;
                return new Token(Token::RANGE, input, start, 2);
            }
            else
            {
                token = new Token(Token::ERR, c);
            }
            break;
        case ';':
            token = new Token(Token::SEMICOLON, c);
            break;
        default:
            cout << "Unexpected character encountered" << endl;
            token = new Token(Token::ERR, c);
        }
        current++;
    }
    // Process string literals
    else if (c == '"')
    {
        current++; // Skip opening quote
        size_t start = current;

        // Find closing quote
        while (current < input.length() && input[current] != '"')
        {
            // Handle escape sequences
            if (input[current] == '\\' && current + 1 < input.length())
            {
                current += 2; // Skip escape sequence
            }
            else
            {
                current++;
            }
        }

        if (current >= input.length())
        {
            // Unterminated string
            token = new Token(Token::ERR, input, first, current - first);
        }
        else
        {
            // Create string token (without quotes)
            token = new Token(Token::STRING, input, start, current - start);
            current++; // Skip closing quote
        }
    }
    else
    {
        // Unknown character - generate error token
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

/**
 * Reset scanner to beginning of input
 */
void Scanner::reset()
{
    first = 0;
    current = 0;
}

/**
 * Scanner destructor
 */
Scanner::~Scanner() {}

/**
 * Test function for the scanner
 * Tokenizes entire input and prints all tokens
 */
void test_scanner(Scanner *scanner)
{
    Token *current;
    cout << "Starting Scanner Test:" << endl
         << endl;
    while ((current = scanner->nextToken())->type != Token::END)
    {
        if (current->type == Token::ERR)
        {
            cout << "Scanner Error - Invalid character: " << current->text << endl;
            break;
        }
        else
        {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}