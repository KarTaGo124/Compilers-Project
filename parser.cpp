#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype)
{
    if (check(ttype))
    {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype)
{
    if (isAtEnd())
        return false;
    return current->type == ttype;
}

bool Parser::advance()
{
    if (!isAtEnd())
    {
        Token *temp = current;
        if (previous)
            delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR))
        {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            std::exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd()
{
    return (current->type == Token::END);
}

Parser::Parser(Scanner *sc) : scanner(sc)
{
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR)
    {
        cout << "Error en el primer token: " << current->text << endl;
        std::exit(1);
    }
}

Stm *Parser::parseVarDeclaration()
{
    bool isVal = false;
    if (match(Token::VAL))
    {
        isVal = true;
    }
    else if (!match(Token::VAR))
    {
        return nullptr;
    }
    if (!match(Token::ID))
    {
        cout << "Error: se esperaba un identificador después de 'var/val'." << endl;
        std::exit(1);
    }
    string id = previous->text;
    if (!match(Token::COLON))
    {
        cout << "Error: se esperaba ':' después del identificador." << endl;
        std::exit(1);
    }

    string type;
    if (match(Token::INT))
    {
        type = "Int";
    }
    else if (match(Token::FLOAT))
    {
        type = "Float";
    }
    else if (match(Token::STRING_TYPE))
    {
        type = "String";
    }
    else if (match(Token::BOOLEAN))
    {
        type = "Boolean";
    }
    else
    {
        cout << "Error: tipo de variable desconocido." << endl;
        std::exit(1);
    }

    Exp *value = nullptr;
    if (match(Token::ASSIGN))
    {
        value = parseExpression();
    }

    match(Token::SEMICOLON);

    VarDec *vd = new VarDec(id, type, value, isVal);
    return vd;
}

VarDecList *Parser::parseVarDecList()
{
    VarDecList *vdl = new VarDecList();
    VarDec *aux = dynamic_cast<VarDec *>(parseVarDeclaration());
    while (aux != NULL)
    {
        vdl->add(aux);
        if (match(Token::SEMICOLON))
        {
            aux = dynamic_cast<VarDec *>(parseVarDeclaration());
        }
        else
        {
            break;
        }
    }
    return vdl;
}

StatementList *Parser::parseStatementList()
{
    StatementList *sl = new StatementList();
    Stm *stmt = parseStatement();
    sl->add(stmt);

    while (!isAtEnd() && (check(Token::SEMICOLON) || check(Token::ID) || check(Token::PRINT) || check(Token::PRINTLN)))
    {
        if (match(Token::SEMICOLON))
        {

            continue;
        }
        stmt = parseStatement();
        sl->add(stmt);
    }
    return sl;
}

Block *Parser::parseBlock()
{
    if (!match(Token::LEFT_BRACE))
    {
        cout << "Error: se esperaba '{' para iniciar un bloque." << endl;
        std::exit(1);
    }

    StatementList *sl = new StatementList();

    while (!check(Token::RIGHT_BRACE) && !isAtEnd())
    {
        Stm *stmt = parseStatement();
        if (stmt != nullptr)
        {
            sl->add(stmt);
        }
        match(Token::SEMICOLON);
    }
    if (!match(Token::RIGHT_BRACE))
    {
        cout << "Error: se esperaba '}' al final del bloque." << endl;
        std::exit(1);
    }

    return new Block(sl);
}

Program *Parser::parseProgram()
{
    StatementList *statements = new StatementList();

    while (!isAtEnd())
    {
        Stm *stmt = parseStatement();
        if (stmt != nullptr)
        {
            statements->add(stmt);
        }
        match(Token::SEMICOLON);
    }

    return new Program(statements);
}

Stm *Parser::parseStatement()
{

    if (check(Token::VAR) || check(Token::VAL))
    {
        return parseVarDeclaration();
    }

    else if (check(Token::FUN))
    {
        return parseFunDeclaration();
    }

    else if (check(Token::PRINT) || check(Token::PRINTLN))
    {
        return parsePrintStatement();
    }

    else if (check(Token::IF))
    {
        return parseIfStatement();
    }

    else if (check(Token::WHILE))
    {
        return parseWhileStatement();
    }

    else if (check(Token::FOR))
    {
        return parseForStatement();
    }

    else if (check(Token::RETURN))
    {
        return parseReturnStatement();
    }

    else if (check(Token::BREAK))
    {
        return parseBreakStatement();
    }

    else if (check(Token::CONTINUE))
    {
        return parseContinueStatement();
    }
    else if (check(Token::LEFT_BRACE))
    {
        return parseBlock();
    }
    else if (check(Token::ID))
    {
        return parseIDStatement();
    }
    else if (check(Token::INCREMENT) || check(Token::DECREMENT))
    {
        return parseIncrementDecrement();
    }
    else
    {
        cout << "Error: statement inesperado: " << current->text << endl;
        std::exit(1);
    }
}

Stm *Parser::parseIfStatement()
{
    if (!match(Token::IF))
        return nullptr;

    if (!match(Token::LEFT_PAREN))
    {
        cout << "Error: se esperaba '(' después de 'if'" << endl;
        std::exit(1);
    }

    Exp *condition = parseExpression();

    if (!match(Token::RIGHT_PAREN))
    {
        cout << "Error: se esperaba ')' después de la condición del if" << endl;
        std::exit(1);
    }

    Stm *thenStmt = parseStatement();
    Stm *elseStmt = nullptr;

    if (match(Token::ELSE))
    {
        elseStmt = parseStatement();
    }

    return new IfStatement(condition, thenStmt, elseStmt);
}

Stm *Parser::parseWhileStatement()
{
    if (!match(Token::WHILE))
        return nullptr;

    if (!match(Token::LEFT_PAREN))
    {
        cout << "Error: se esperaba '(' después de 'while'" << endl;
        std::exit(1);
    }

    Exp *condition = parseExpression();

    if (!match(Token::RIGHT_PAREN))
    {
        cout << "Error: se esperaba ')' después de la condición del while" << endl;
        std::exit(1);
    }

    Stm *stmt = parseStatement();

    return new WhileStatement(condition, stmt);
}

Stm *Parser::parseForStatement()
{
    if (!match(Token::FOR))
        return nullptr;

    if (!match(Token::LEFT_PAREN))
    {
        cout << "Error: se esperaba '(' después de 'for'" << endl;
        std::exit(1);
    }

    if (!match(Token::ID))
    {
        cout << "Error: se esperaba identificador en el for" << endl;
        std::exit(1);
    }
    string varId = previous->text;

    if (!match(Token::IN))
    {
        cout << "Error: se esperaba 'in' después del identificador" << endl;
        std::exit(1);
    }

    Exp *rangeExp = parseRangeExpression();

    if (!match(Token::RIGHT_PAREN))
    {
        cout << "Error: se esperaba ')' después del rango" << endl;
        std::exit(1);
    }
    Stm *stmt = parseStatement();

    return new ForStatement(varId, rangeExp, stmt);
}
Stm *Parser::parseReturnStatement()
{
    if (!match(Token::RETURN))
        return nullptr;

    Exp *value = nullptr;
    if (!check(Token::SEMICOLON) && !check(Token::RIGHT_BRACE) && !isAtEnd())
    {
        value = parseExpression();
    }

    match(Token::SEMICOLON);

    return new ReturnStatement(value);
}

Stm *Parser::parseBreakStatement()
{
    if (!match(Token::BREAK))
        return nullptr;

    match(Token::SEMICOLON);

    return new BreakStatement();
}

Stm *Parser::parseContinueStatement()
{
    if (!match(Token::CONTINUE))
        return nullptr;

    match(Token::SEMICOLON);

    return new ContinueStatement();
}

Exp *Parser::parseExpression()
{
    return parseOrExp();
}

Exp *Parser::parseOrExp()
{
    Exp *expr = parseAndExp();

    while (match(Token::OR))
    {
        Exp *right = parseAndExp();
        expr = new BinaryExp(expr, right, OR_OP);
    }

    return expr;
}

Exp *Parser::parseAndExp()
{
    Exp *expr = parseEqExp();

    while (match(Token::AND))
    {
        Exp *right = parseEqExp();
        expr = new BinaryExp(expr, right, AND_OP);
    }

    return expr;
}

Exp *Parser::parseEqExp()
{
    Exp *expr = parseRelExp();

    while (check(Token::EQ) || check(Token::NE))
    {
        BinaryOp op = match(Token::EQ) ? EQ_OP : NE_OP;
        if (op == NE_OP)
            advance();
        Exp *right = parseRelExp();
        expr = new BinaryExp(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseRelExp()
{
    Exp *expr = parseAddExp();

    while (check(Token::LT) || check(Token::LE) || check(Token::GT) || check(Token::GE))
    {
        BinaryOp op;
        if (match(Token::LT))
            op = LT_OP;
        else if (match(Token::LE))
            op = LE_OP;
        else if (match(Token::GT))
            op = GT_OP;
        else if (match(Token::GE))
            op = GE_OP;

        Exp *right = parseAddExp();
        expr = new BinaryExp(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseAddExp()
{
    Exp *expr = parseMulExp();

    while (check(Token::PLUS) || check(Token::MINUS))
    {
        BinaryOp op = match(Token::PLUS) ? PLUS_OP : MINUS_OP;
        if (op == MINUS_OP)
            advance();
        Exp *right = parseMulExp();
        expr = new BinaryExp(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseMulExp()
{
    Exp *expr = parseUnExp();

    while (check(Token::MUL) || check(Token::DIV) || check(Token::MOD))
    {
        BinaryOp op;
        if (match(Token::MUL))
            op = MUL_OP;
        else if (match(Token::DIV))
            op = DIV_OP;
        else if (match(Token::MOD))
            op = MOD_OP;

        Exp *right = parseUnExp();
        expr = new BinaryExp(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseUnExp()
{
    if (check(Token::NOT) || check(Token::MINUS) || check(Token::INCREMENT) || check(Token::DECREMENT))
    {
        UnaryExp::UnaryOp op;
        if (match(Token::NOT))
            op = UnaryExp::NOT_OP;
        else if (match(Token::MINUS))
            op = UnaryExp::NEG_OP;
        else if (match(Token::INCREMENT))
            op = UnaryExp::PRE_INC_OP;
        else if (match(Token::DECREMENT))
            op = UnaryExp::PRE_DEC_OP;

        Exp *expr = parseUnExp();
        return new UnaryExp(op, expr);
    }

    return parsePostExp();
}

Exp *Parser::parsePostExp()
{
    Exp *expr = parsePrimary();

    while (check(Token::INCREMENT) || check(Token::DECREMENT))
    {
        UnaryExp::UnaryOp op;
        if (match(Token::INCREMENT))
            op = UnaryExp::POST_INC_OP;
        else if (match(Token::DECREMENT))
            op = UnaryExp::POST_DEC_OP;

        expr = new UnaryExp(op, expr);
    }

    return expr;
}

Exp *Parser::parsePrimary()
{
    if (match(Token::NUM))
    {
        int value = stoi(previous->text);
        return new NumberExp(value);
    }

    if (match(Token::DECIMAL))
    {
        float value = stof(previous->text);
        DecimalExp *decimalExp = new DecimalExp(value);
        decimalExp->has_f = previous->text.back() == 'f';
        return decimalExp;
    }

    if (match(Token::STRING))
    {
        return new StringExp(previous->text);
    }

    if (match(Token::TRUE))
    {
        return new BoolExp(true);
    }
    if (match(Token::FALSE))
    {
        return new BoolExp(false);
    }
    if (match(Token::LEFT_PAREN))
    {
        Exp *expr = parseExpression();
        if (!match(Token::RIGHT_PAREN))
        {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            std::exit(1);
        }
        return new ParenthesizedExp(expr);
    }

    if (match(Token::ID))
    {
        string name = previous->text;

        if (check(Token::LEFT_PAREN))
        {
            advance();
            FunctionCallExp *funcCall = new FunctionCallExp(name);

            if (!check(Token::RIGHT_PAREN))
            {
                list<Exp *> args = parseArgumentList();
                for (Exp *arg : args)
                {
                    funcCall->addArg(arg);
                }
            }
            if (!match(Token::RIGHT_PAREN))
            {
                cout << "Error: se esperaba ')' después de los argumentos." << endl;
                std::exit(1);
            }

            return funcCall;
        }
        else
        {
            return new IdentifierExp(name);
        }
    }
    cout << "Error: expresión inesperada: " << current->text << endl;
    std::exit(1);
}

Exp *Parser::parseRangeExpression()
{
    Exp *start = parseExpression();

    bool isUntil = false;
    bool isDownTo = false;

    if (match(Token::RANGE))
    {
        isUntil = false;
        isDownTo = false;
    }
    else if (match(Token::UNTIL))
    {
        isUntil = true;
        isDownTo = false;
    }
    else if (match(Token::DOWNTO))
    {
        isUntil = false;
        isDownTo = true;
    }
    else
    {
        cout << "Error: se esperaba '..', 'until' o 'downTo' en la expresión de rango." << endl;
        std::exit(1);
    }

    Exp *end = parseExpression();

    Exp *step = nullptr;
    if (match(Token::STEP))
    {
        step = parseExpression();
    }

    return new RangeExp(start, end, isUntil, isDownTo, step);
}

Stm *Parser::parseAssignment()
{
    if (!match(Token::ID))
    {
        cout << "Error: se esperaba un identificador para asignación." << endl;
        std::exit(1);
    }
    string id = previous->text;

    AssignStatement::AssignOp op = AssignStatement::ASSIGN_OP;
    if (match(Token::ASSIGN))
    {
        op = AssignStatement::ASSIGN_OP;
    }
    else if (match(Token::PLUS_ASSIGN))
    {
        op = AssignStatement::PLUS_ASSIGN_OP;
    }
    else if (match(Token::MINUS_ASSIGN))
    {
        op = AssignStatement::MINUS_ASSIGN_OP;
    }
    else if (match(Token::MUL_ASSIGN))
    {
        op = AssignStatement::MUL_ASSIGN_OP;
    }
    else if (match(Token::DIV_ASSIGN))
    {
        op = AssignStatement::DIV_ASSIGN_OP;
    }
    else if (match(Token::MOD_ASSIGN))
    {
        op = AssignStatement::MOD_ASSIGN_OP;
    }
    else
    {
        cout << "Error: se esperaba un operador de asignación." << endl;
        std::exit(1);
    }

    Exp *value = parseExpression();
    match(Token::SEMICOLON);

    return new AssignStatement(id, value, op);
}
Stm *Parser::parsePrintStatement()
{
    bool isPrintln = false;
    if (match(Token::PRINTLN))
    {
        isPrintln = true;
    }
    else if (!match(Token::PRINT))
    {
        cout << "Error: se esperaba 'print' o 'println'." << endl;
        std::exit(1);
    }

    if (!match(Token::LEFT_PAREN))
    {
        cout << "Error: se esperaba '(' después de print/println." << endl;
        std::exit(1);
    }

    Exp *expr = parseExpression();

    if (!match(Token::RIGHT_PAREN))
    {
        cout << "Error: se esperaba ')' después de la expresión." << endl;
        std::exit(1);
    }
    match(Token::SEMICOLON);

    return new PrintStatement(expr, isPrintln);
}

Stm *Parser::parseFunDeclaration()
{
    if (!match(Token::FUN))
    {
        cout << "Error: se esperaba 'fun'." << endl;
        exit(1);
    }

    if (!match(Token::ID))
    {
        cout << "Error: se esperaba un identificador después de 'fun'." << endl;
        exit(1);
    }
    string name = previous->text;

    if (!match(Token::LEFT_PAREN))
    {
        cout << "Error: se esperaba '(' después del nombre de función." << endl;
        exit(1);
    }

    list<pair<string, string>> params;
    if (!check(Token::RIGHT_PAREN))
    {
        params = parseParameterList();
    }

    if (!match(Token::RIGHT_PAREN))
    {
        cout << "Error: se esperaba ')' después de los parámetros." << endl;
        exit(1);
    }

    string returnType = "Unit";
    if (match(Token::COLON))
    {
        if (match(Token::INT))
        {
            returnType = "Int";
        }
        else if (match(Token::FLOAT))
        {
            returnType = "Float";
        }
        else if (match(Token::STRING_TYPE))
        {
            returnType = "String";
        }
        else if (match(Token::BOOLEAN))
        {
            returnType = "Boolean";
        }
        else if (match(Token::UNIT))
        {
            returnType = "Unit";
        }
        else
        {
            cout << "Error: tipo de retorno desconocido." << endl;
            exit(1);
        }
    }

    Block *body = parseBlock();

    FunctionDecl *funcDecl = new FunctionDecl(name, returnType, body);
    for (auto &param : params)
    {
        funcDecl->addParam(param.first, param.second);
    }
    return funcDecl;
}

list<pair<string, string>> Parser::parseParameterList()
{
    list<pair<string, string>> params;

    if (!match(Token::ID))
    {
        cout << "Error: se esperaba un identificador de parámetro." << endl;
        exit(1);
    }
    string paramName = previous->text;

    if (!match(Token::COLON))
    {
        cout << "Error: se esperaba ':' después del nombre del parámetro." << endl;
        exit(1);
    }

    string paramType;
    if (match(Token::INT))
    {
        paramType = "Int";
    }
    else if (match(Token::FLOAT))
    {
        paramType = "Float";
    }
    else if (match(Token::STRING_TYPE))
    {
        paramType = "String";
    }
    else if (match(Token::BOOLEAN))
    {
        paramType = "Boolean";
    }
    else
    {
        cout << "Error: tipo de parámetro desconocido." << endl;
        exit(1);
    }

    params.push_back(make_pair(paramName, paramType));

    while (match(Token::COMMA))
    {
        if (!match(Token::ID))
        {
            cout << "Error: se esperaba un identificador de parámetro." << endl;
            exit(1);
        }
        paramName = previous->text;

        if (!match(Token::COLON))
        {
            cout << "Error: se esperaba ':' después del nombre del parámetro." << endl;
            exit(1);
        }

        if (match(Token::INT))
        {
            paramType = "Int";
        }
        else if (match(Token::FLOAT))
        {
            paramType = "Float";
        }
        else if (match(Token::STRING_TYPE))
        {
            paramType = "String";
        }
        else if (match(Token::BOOLEAN))
        {
            paramType = "Boolean";
        }
        else
        {
            cout << "Error: tipo de parámetro desconocido." << endl;
            exit(1);
        }

        params.push_back(make_pair(paramName, paramType));
    }

    return params;
}

list<Exp *> Parser::parseArgumentList()
{
    list<Exp *> args;

    args.push_back(parseExpression());

    while (match(Token::COMMA))
    {
        args.push_back(parseExpression());
    }

    return args;
}

Stm *Parser::parseIncrementDecrement()
{
    AssignStatement::AssignOp op;
    if (match(Token::INCREMENT))
    {
        op = AssignStatement::INCREMENT_OP;
    }
    else if (match(Token::DECREMENT))
    {
        op = AssignStatement::DECREMENT_OP;
    }
    else
    {
        cout << "Error: se esperaba '++' o '--'" << endl;
        std::exit(1);
    }

    if (!match(Token::ID))
    {
        cout << "Error: se esperaba un identificador después de '++' o '--'" << endl;
        std::exit(1);
    }
    string id = previous->text;

    match(Token::SEMICOLON);

    return new AssignStatement(id, nullptr, op);
}

Stm *Parser::parseIDStatement()
{
    if (!match(Token::ID))
    {
        cout << "Error: se esperaba un identificador." << endl;
        std::exit(1);
    }
    string id = previous->text;

    if (check(Token::LEFT_PAREN))
    {
        match(Token::LEFT_PAREN);
        list<Exp *> args;
        if (!check(Token::RIGHT_PAREN))
        {
            args = parseArgumentList();
        }
        if (!match(Token::RIGHT_PAREN))
        {
            cout << "Error: se esperaba ')' después de los argumentos" << endl;
            std::exit(1);
        }
        match(Token::SEMICOLON);

        FunctionCallExp *funcCall = new FunctionCallExp(id);
        for (Exp *arg : args)
        {
            funcCall->addArg(arg);
        }

        return new ExpressionStatement(funcCall);
    }
    else if (check(Token::INCREMENT))
    {
        match(Token::INCREMENT);
        match(Token::SEMICOLON);
        return new AssignStatement(id, nullptr, AssignStatement::POST_INCREMENT_OP);
    }
    else if (check(Token::DECREMENT))
    {
        match(Token::DECREMENT);
        match(Token::SEMICOLON);
        return new AssignStatement(id, nullptr, AssignStatement::POST_DECREMENT_OP);
    }
    else
    {
        AssignStatement::AssignOp op = AssignStatement::ASSIGN_OP;
        if (match(Token::ASSIGN))
        {
            op = AssignStatement::ASSIGN_OP;
        }
        else if (match(Token::PLUS_ASSIGN))
        {
            op = AssignStatement::PLUS_ASSIGN_OP;
        }
        else if (match(Token::MINUS_ASSIGN))
        {
            op = AssignStatement::MINUS_ASSIGN_OP;
        }
        else if (match(Token::MUL_ASSIGN))
        {
            op = AssignStatement::MUL_ASSIGN_OP;
        }
        else if (match(Token::DIV_ASSIGN))
        {
            op = AssignStatement::DIV_ASSIGN_OP;
        }
        else if (match(Token::MOD_ASSIGN))
        {
            op = AssignStatement::MOD_ASSIGN_OP;
        }
        else
        {
            cout << "Error: se esperaba un operador de asignación." << endl;
            std::exit(1);
        }

        Exp *value = parseExpression();
        match(Token::SEMICOLON);

        return new AssignStatement(id, value, op);
    }
}
