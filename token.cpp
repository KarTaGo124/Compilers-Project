#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type) : type(type)
{
    text = "";
}

Token::Token(Type type, char c) : type(type)
{
    text = string(1, c);
}

Token::Token(Type type, const string &source, int first, int last) : type(type)
{
    text = source.substr(first, last);
}

std::ostream &operator<<(std::ostream &outs, const Token &tok)
{
    switch (tok.type)
    {

    case Token::END:
        outs << "TOKEN(END)";
        break;
    case Token::VAR:
        outs << "TOKEN(VAR)";
        break;
    case Token::VAL:
        outs << "TOKEN(VAL)";
        break;
    case Token::COLON:
        outs << "TOKEN(COLON)";
        break;

    case Token::PLUS:
        outs << "TOKEN(PLUS)";
        break;
    case Token::MINUS:
        outs << "TOKEN(MINUS)";
        break;
    case Token::MUL:
        outs << "TOKEN(MUL)";
        break;
    case Token::DIV:
        outs << "TOKEN(DIV)";
        break;
    case Token::MOD:
        outs << "TOKEN(MOD)";
        break;

    case Token::NUM:
        outs << "TOKEN(NUM)";
        break;
    case Token::DECIMAL:
        outs << "TOKEN(DECIMAL)";
        break;
    case Token::STRING:
        outs << "TOKEN(STRING)";
        break;
    case Token::ERR:
        outs << "TOKEN(ERR)";
        break;

    case Token::PLUS_ASSIGN:
        outs << "TOKEN(PLUS_ASSIGN)";
        break;
    case Token::MINUS_ASSIGN:
        outs << "TOKEN(MINUS_ASSIGN)";
        break;
    case Token::MUL_ASSIGN:
        outs << "TOKEN(MUL_ASSIGN)";
        break;
    case Token::DIV_ASSIGN:
        outs << "TOKEN(DIV_ASSIGN)";
        break;
    case Token::MOD_ASSIGN:
        outs << "TOKEN(MOD_ASSIGN)";
        break;

    case Token::INCREMENT:
        outs << "TOKEN(INCREMENT)";
        break;
    case Token::DECREMENT:
        outs << "TOKEN(DECREMENT)";
        break;

    case Token::LEFT_PAREN:
        outs << "TOKEN(LEFT_PAREN)";
        break;
    case Token::RIGHT_PAREN:
        outs << "TOKEN(RIGHT_PAREN)";
        break;
    case Token::LEFT_BRACE:
        outs << "TOKEN(LEFT_BRACE)";
        break;
    case Token::RIGHT_BRACE:
        outs << "TOKEN(RIGHT_BRACE)";
        break;

    case Token::ID:
        outs << "TOKEN(ID)";
        break;
    case Token::ASSIGN:
        outs << "TOKEN(ASSIGN)";
        break;

    case Token::SEMICOLON:
        outs << "TOKEN(SEMICOLON)";
        break;
    case Token::COMMA:
        outs << "TOKEN(COMMA)";
        break;

    case Token::LT:
        outs << "TOKEN(LT)";
        break;
    case Token::LE:
        outs << "TOKEN(LE)";
        break;
    case Token::GT:
        outs << "TOKEN(GT)";
        break;
    case Token::GE:
        outs << "TOKEN(GE)";
        break;
    case Token::EQ:
        outs << "TOKEN(EQ)";
        break;
    case Token::NE:
        outs << "TOKEN(NE)";
        break;

    case Token::AND:
        outs << "TOKEN(AND)";
        break;
    case Token::OR:
        outs << "TOKEN(OR)";
        break;
    case Token::NOT:
        outs << "TOKEN(NOT)";
        break;

    case Token::RANGE:
        outs << "TOKEN(RANGE)";
        break;
    case Token::UNTIL:
        outs << "TOKEN(UNTIL)";
        break;

    case Token::PRINT:
        outs << "TOKEN(PRINT)";
        break;
    case Token::PRINTLN:
        outs << "TOKEN(PRINTLN)";
        break;

    case Token::IF:
        outs << "TOKEN(IF)";
        break;
    case Token::ELSE:
        outs << "TOKEN(ELSE)";
        break;

    case Token::WHILE:
        outs << "TOKEN(WHILE)";
        break;
    case Token::DO:
        outs << "TOKEN(DO)";
        break;
    case Token::FOR:
        outs << "TOKEN(FOR)";
        break;
    case Token::IN:
        outs << "TOKEN(IN)";
        break;

    case Token::FUN:
        outs << "TOKEN(FUN)";
        break;
    case Token::RETURN:
        outs << "TOKEN(RETURN)";
        break;

    case Token::BREAK:
        outs << "TOKEN(BREAK)";
        break;
    case Token::CONTINUE:
        outs << "TOKEN(CONTINUE)";
        break;

    case Token::FALSE:
        outs << "TOKEN(FALSE)";
        break;
    case Token::TRUE:
        outs << "TOKEN(TRUE)";
        break;

    case Token::NULL_LITERAL:
        outs << "TOKEN(NULL_LITERAL)";
        break;

    case Token::INT:
        outs << "TOKEN(INT)";
        break;
    case Token::FLOAT:
        outs << "TOKEN(FLOAT)";
        break;
    case Token::STRING_TYPE:
        outs << "TOKEN(STRING_TYPE)";
        break;
    case Token::BOOLEAN:
        outs << "TOKEN(BOOLEAN)";
        break;
    case Token::UNIT:
        outs << "TOKEN(UNIT)";
        break;

    default:
        outs << "TOKEN(UNKNOWN)";
        break;
    }
    return outs;
}

std::ostream &operator<<(std::ostream &outs, const Token *tok)
{
    return outs << *tok;
}
