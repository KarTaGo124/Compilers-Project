#include <iostream>
#include "exp.h"
using namespace std;

Exp::~Exp() {}
BinaryExp::BinaryExp(Exp *l, Exp *r, BinaryOp op) : left(l), right(r), op(op)
{
    if (op == PLUS_OP || op == MINUS_OP || op == MUL_OP || op == DIV_OP || op == MOD_OP)
    {
        type = "int";
    }
    else
    {
        type = "bool";
    }
}

BinaryExp::~BinaryExp()
{
    delete left;
    delete right;
}

NumberExp::NumberExp(int v) : value(v) {}
NumberExp::~NumberExp() {}

DecimalExp::DecimalExp(float v) : value(v) {}
DecimalExp::~DecimalExp() {}

IdentifierExp::IdentifierExp(const string &n) : name(n) {}
IdentifierExp::~IdentifierExp() {}

BoolExp::BoolExp(bool v) : value(v) {}
BoolExp::~BoolExp() {}

AssignStatement::AssignStatement(string id, Exp *e, AssignOp op) : id(id), rhs(e), op(op) {}
AssignStatement::~AssignStatement()
{
    delete rhs;
}

PrintStatement::PrintStatement(Exp *e, bool newline) : e(e), newline(newline) {}
PrintStatement::~PrintStatement()
{
    delete e;
}

ExpressionStatement::ExpressionStatement(Exp *expr) : expr(expr) {}
ExpressionStatement::~ExpressionStatement()
{
    delete expr;
}

int ExpressionStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

IfStatement::IfStatement(Exp *condition, Stm *thenStmt, Stm *elseStmt) : condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {}
IfStatement::~IfStatement()
{
    delete condition;
    delete thenStmt;
    delete elseStmt;
}

WhileStatement::WhileStatement(Exp *condition, Stm *stmt) : condition(condition), stmt(stmt) {}
WhileStatement::~WhileStatement()
{
    delete condition;
    delete stmt;
}

ForStatement::ForStatement(std::string id, Exp *range, Stm *stmt) : id(id), range(range), stmt(stmt) {}

ForStatement::~ForStatement()
{
    delete range;
    delete stmt;
}

RangeExp::RangeExp(Exp *start, Exp *end, bool until, bool downTo, Exp *step)
    : start(start), end(end), until(until), downTo(downTo), step(step) {}
RangeExp::~RangeExp()
{
    delete start;
    delete end;
    if (step)
        delete step;
}

StringExp::StringExp(const string &v) : value(v) {}
StringExp::~StringExp() {}

ParenthesizedExp::ParenthesizedExp(Exp *expr) : expr(expr) {}
int ParenthesizedExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}
ParenthesizedExp::~ParenthesizedExp() { delete expr; }

FunctionCallExp::FunctionCallExp(const string &name) : name(name) {}
void FunctionCallExp::addArg(Exp *arg)
{
    args.push_back(arg);
}
FunctionCallExp::~FunctionCallExp()
{
    for (Exp *arg : args)
    {
        delete arg;
    }
}

UnaryExp::UnaryExp(UnaryOp op, Exp *expr) : op(op), expr(expr) {}
UnaryExp::~UnaryExp() { delete expr; }

FunctionDecl::FunctionDecl(const string &name, const string &returnType, Block *body) : name(name), returnType(returnType), body(body) {}
void FunctionDecl::addParam(const string &name, const string &type)
{
    params.push_back(make_pair(name, type));
}
FunctionDecl::~FunctionDecl() { delete body; }

ReturnStatement::ReturnStatement(Exp *expr) : expr(expr) {}
ReturnStatement::~ReturnStatement() { delete expr; }

BreakStatement::BreakStatement() {}
BreakStatement::~BreakStatement() {}

ContinueStatement::ContinueStatement() {}
ContinueStatement::~ContinueStatement() {}

VarDec::VarDec(const string &id, const string &t, Exp *val, bool isVal) : id(id), type(t), value(val), isVal(isVal) {}
VarDec::~VarDec() { delete value; }

VarDecList::VarDecList() : decls() {}
void VarDecList::add(VarDec *v)
{
    decls.push_back(v);
}

VarDecList::~VarDecList()
{
    for (VarDec *decl : decls)
    {
        delete decl;
    }
}

StatementList::StatementList() {}
void StatementList::add(Stm *s)
{
    stms.push_back(s);
}
StatementList::~StatementList()
{
    for (auto s : stms)
    {
        delete s;
    }
}

Block::Block(StatementList *statements) : statements(statements) {}
Block::~Block()
{
    delete statements;
}

Program::Program(StatementList *statements) : statements(statements) {}
Program::~Program()
{
    delete statements;
}

Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op)
{
    string c;
    switch (op)
    {
    case PLUS_OP:
        c = "+";
        break;
    case MINUS_OP:
        c = "-";
        break;
    case MUL_OP:
        c = "*";
        break;
    case DIV_OP:
        c = "/";
        break;
    case MOD_OP:
        c = "%";
        break;
    case LT_OP:
        c = "<";
        break;
    case LE_OP:
        c = "<=";
        break;
    case GT_OP:
        c = ">";
        break;
    case GE_OP:
        c = ">=";
        break;
    case EQ_OP:
        c = "==";
        break;
    case NE_OP:
        c = "!=";
        break;
    case AND_OP:
        c = "&&";
        break;
    case OR_OP:
        c = "||";
        break;
    default:
        c = "$";
    }
    return c;
}