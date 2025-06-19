#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token
{
public:
    enum Type
    {
        VAR,
        VAL,
        COLON,
        END,
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        NUM,
        DECIMAL,
        STRING,
        ERR,
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        INCREMENT,
        DECREMENT,
        LEFT_PAREN,
        RIGHT_PAREN,
        ID,
        ASSIGN,
        SEMICOLON,
        COMMA,
        LT,
        LE,
        GT,
        GE,
        EQ,
        NE,
        AND,
        OR,
        NOT,
        RANGE,
        UNTIL,
        DOWNTO,
        STEP,
        PRINT,
        PRINTLN,
        IF,
        LEFT_BRACE,
        RIGHT_BRACE,
        ELSE,
        WHILE,
        DO,
        FOR,
        IN,
        FUN,
        RETURN,
        BREAK,
        CONTINUE,
        FALSE,
        TRUE,
        NULL_LITERAL,
        INT,
        FLOAT,
        STRING_TYPE,
        BOOLEAN,
        UNIT
    };

    Type type;
    std::string text;
    bool has_f = false;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string &source, int first, int last);

    friend std::ostream &operator<<(std::ostream &outs, const Token &tok);
    friend std::ostream &operator<<(std::ostream &outs, const Token *tok);
};

#endif