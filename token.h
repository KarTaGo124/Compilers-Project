#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/**
 * Token class for the Kotlin compiler lexical analyzer
 * Represents different types of tokens found in Kotlin source code
 */
class Token
{
public:
    enum Type
    {
        // Variables and basic symbols
        VAR,   // 'var' keyword
        VAL,   // 'val' keyword (immutable)
        COLON, // ':'
        END,   // End of input

        // Arithmetic operators
        PLUS,  // '+'
        MINUS, // '-'
        MUL,   // '*'
        DIV,   // '/'
        MOD,   // '%'

        // Literals
        NUM,     // Integer numbers
        DECIMAL, // Floating point numbers
        STRING,  // String literals
        ERR,     // Lexical errors

        // Assignment operators
        PLUS_ASSIGN,  // '+='
        MINUS_ASSIGN, // '-='
        MUL_ASSIGN,   // '*='
        DIV_ASSIGN,   // '/='
        MOD_ASSIGN,   // '%='        // Increment/Decrement operators
        INCREMENT,    // '++'
        DECREMENT,    // '--'

        // Delimiters and identifiers
        LEFT_PAREN,  // '('
        RIGHT_PAREN, // ')'
        ID,          // Variable names
        ASSIGN,      // '='
        SEMICOLON,   // ';' (optional in Kotlin)
        COMMA,       // ','

        // Comparison operators
        LT, // '<'
        LE, // '<='
        GT, // '>'
        GE, // '>='
        EQ, // '=='
        NE, // '!='

        // Logical operators
        AND, // '&&'
        OR,  // '||'
        NOT, // '!'

        // Range operators
        RANGE, // '..'
        UNTIL, // 'until'

        // I/O functions
        PRINT,   // 'print'
        PRINTLN, // 'println'

        // Control flow keywords
        IF,          // 'if'
        LEFT_BRACE,  // '{'
        RIGHT_BRACE, // '}'
        ELSE,        // 'else'

        // Loop keywords
        WHILE, // 'while'
        DO,    // 'do'
        FOR,   // 'for'
        IN,    // 'in' (for ranges)

        // Function keywords
        FUN,    // 'fun'
        RETURN, // 'return'

        // Control keywords
        BREAK,    // 'break'
        CONTINUE, // 'continue'

        // Boolean literals
        FALSE, // 'false'
        TRUE,  // 'true'

        // Null literal
        NULL_LITERAL, // 'null'

        // Type keywords
        INT,         // 'Int'
        FLOAT,       // 'Float'
        STRING_TYPE, // 'String' type
        BOOLEAN      // 'Boolean'
    };

    Type type;
    std::string text;
    bool has_f = false; // Flag for 'f' suffix in float literals

    // Constructors
    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string &source, int first, int last);

    // Output operators
    friend std::ostream &operator<<(std::ostream &outs, const Token &tok);
    friend std::ostream &operator<<(std::ostream &outs, const Token *tok);
};

#endif // TOKEN_H