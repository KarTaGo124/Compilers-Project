#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include <string>
#include "visitor.h"
using namespace std;
enum BinaryOp
{
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    MOD_OP,
    LT_OP,
    LE_OP,
    GT_OP,
    GE_OP,
    EQ_OP,
    NE_OP,
    AND_OP,
    OR_OP
};

class Exp
{
public:
    bool has_f = false;
    virtual int accept(Visitor *visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};

class BinaryExp : public Exp
{
public:
    Exp *left, *right;
    string type;
    BinaryOp op;
    BinaryExp(Exp *l, Exp *r, BinaryOp op);
    int accept(Visitor *visitor);
    ~BinaryExp();
};

class NumberExp : public Exp
{
public:
    int value;
    NumberExp(int v);
    int accept(Visitor *visitor);
    ~NumberExp();
};

class DecimalExp : public Exp
{
public:
    float value;
    DecimalExp(float v);
    int accept(Visitor *visitor);
    ~DecimalExp();
};

class BoolExp : public Exp
{
public:
    int value;
    BoolExp(bool v);
    int accept(Visitor *visitor);
    ~BoolExp();
};

class IdentifierExp : public Exp
{
public:
    string name;
    IdentifierExp(const std::string &n);
    int accept(Visitor *visitor);
    ~IdentifierExp();
};

class RangeExp : public Exp
{
public:
    Exp *start;
    Exp *end;
    bool until;
    RangeExp(Exp *start, Exp *end, bool until = false);
    int accept(Visitor *visitor);
    ~RangeExp();
};

class StringExp : public Exp
{
public:
    string value;
    StringExp(const string &v);
    int accept(Visitor *visitor);
    ~StringExp();
};

class FunctionCallExp : public Exp
{
public:
    string name;
    list<Exp *> args;
    FunctionCallExp(const string &name);
    void addArg(Exp *arg);
    int accept(Visitor *visitor);
    ~FunctionCallExp();
};

class UnaryExp : public Exp
{
public:
    enum UnaryOp
    {
        NOT_OP,
        NEG_OP,
        PRE_INC_OP,
        PRE_DEC_OP,
        POST_INC_OP,
        POST_DEC_OP
    };
    UnaryOp op;
    Exp *expr;
    UnaryExp(UnaryOp op, Exp *expr);
    int accept(Visitor *visitor);
    ~UnaryExp();
};

class Stm
{
public:
    virtual int accept(Visitor *visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm
{
public:
    enum AssignOp
    {
        ASSIGN_OP,
        PLUS_ASSIGN_OP,
        MINUS_ASSIGN_OP,
        MUL_ASSIGN_OP,
        DIV_ASSIGN_OP,
        MOD_ASSIGN_OP,
        INCREMENT_OP,
        DECREMENT_OP
    };
    string id;
    Exp *rhs;
    AssignOp op;
    AssignStatement(string id, Exp *e, AssignOp op = ASSIGN_OP);
    int accept(Visitor *visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm
{
public:
    Exp *e;
    bool newline;
    PrintStatement(Exp *e, bool newline);
    int accept(Visitor *visitor);
    ~PrintStatement();
};

class Block : public Stm
{
public:
    StatementList *statements;
    Block(StatementList *statements);
    int accept(Visitor *visitor);
    ~Block();
};

class FunctionDecl : public Stm
{
public:
    string name;
    string returnType;
    list<pair<string, string>> params;
    Block *body;
    FunctionDecl(const string &name, const string &returnType, Block *body);
    void addParam(const string &name, const string &type);
    int accept(Visitor *visitor);
    ~FunctionDecl();
};

class ReturnStatement : public Stm
{
public:
    Exp *expr;
    ReturnStatement(Exp *expr = nullptr);
    int accept(Visitor *visitor);
    ~ReturnStatement();
};

class BreakStatement : public Stm
{
public:
    BreakStatement();
    int accept(Visitor *visitor);
    ~BreakStatement();
};

class ContinueStatement : public Stm
{
public:
    ContinueStatement();
    int accept(Visitor *visitor);
    ~ContinueStatement();
};

class VarDec : public Stm
{
public:
    string id;
    string type;
    Exp *value;
    bool isVal;
    VarDec(const string &id, const string &type, Exp *value = nullptr, bool isVal = false);
    int accept(Visitor *visitor);
    ~VarDec();
};

class VarDecList
{
public:
    std::list<VarDec *> decls;
    VarDecList();
    void add(VarDec *decl);
    int accept(Visitor *visitor);
    ~VarDecList();
};

class StatementList
{
public:
    list<Stm *> stms;
    StatementList();
    void add(Stm *stm);
    int accept(Visitor *visitor);
    ~StatementList();
};

class Program
{
public:
    StatementList *statements;
    Program(StatementList *statements);
    ~Program();
};

class IfStatement : public Stm
{
public:
    Exp *condition;
    Stm *thenStmt;
    Stm *elseStmt;
    IfStatement(Exp *condition, Stm *thenStmt, Stm *elseStmt);
    int accept(Visitor *visitor);
    ~IfStatement();
};

class WhileStatement : public Stm
{
public:
    Exp *condition;
    Stm *stmt;
    WhileStatement(Exp *condition, Stm *stmt);
    int accept(Visitor *visitor);
    ~WhileStatement();
};

class ForStatement : public Stm
{
public:
    std::string id;
    Exp *range;
    Stm *stmt;
    ForStatement(std::string id, Exp *range, Stm *stmt);
    int accept(Visitor *visitor) override;
    ~ForStatement();
};

#endif