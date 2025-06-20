#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"

class Parser
{
private:
    Scanner *scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();

    Exp *parseExpression();
    Exp *parseOrExp();
    Exp *parseAndExp();
    Exp *parseEqExp();
    Exp *parseRelExp();
    Exp *parseAddExp();
    Exp *parseMulExp();
    Exp *parseUnExp();
    Exp *parsePostExp();
    Exp *parsePrimary();
    Stm *parseStatement();
    Stm *parseTopLevelStatement();
    Stm *parseVarDeclaration();
    Stm *parseFunDeclaration();
    Stm *parseAssignment();
    Stm *parseIDStatement();
    Stm *parseIncrementDecrement();
    Stm *parsePrintStatement();
    Stm *parseIfStatement();
    Stm *parseWhileStatement();
    Stm *parseDoWhileStatement();
    Stm *parseForStatement();
    Stm *parseReturnStatement();
    Stm *parseBreakStatement();
    Stm *parseContinueStatement();
    Exp *parseRangeExpression();
    Block *parseBlock();
    RunBlock *parseRunBlock();
    list<pair<string, string>> parseParameterList();
    list<Exp *> parseArgumentList();

public:
    Parser(Scanner *scanner);
    Program *parseProgram();
    StatementList *parseStatementList();
    VarDecList *parseVarDecList();
};

#endif