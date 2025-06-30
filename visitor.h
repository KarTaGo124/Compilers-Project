#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include "environment.h"
#include <list>
#include <unordered_map>
#include <iostream>
#include <stack>

class Exp;
class BinaryExp;
class NumberExp;
class DecimalExp;
class BoolExp;
class IdentifierExp;
class RangeExp;
class StringExp;
class ParenthesizedExp;
class FunctionCallExp;
class UnaryExp;
class RunExp;
class AssignStatement;
class PrintStatement;
class ExpressionStatement;

class IfStatement;
class WhileStatement;
class DoWhileStatement;
class ForStatement;

class VarDec;
class VarDecList;
class StatementList;
class Block;
class RunBlock;
class Program;

class FunctionDecl;
class ReturnStatement;
class BreakStatement;
class ContinueStatement;

class Visitor
{
public:
    virtual int visit(BinaryExp *exp) = 0;
    virtual int visit(NumberExp *exp) = 0;
    virtual int visit(DecimalExp *exp) = 0;
    virtual int visit(BoolExp *exp) = 0;
    virtual int visit(IdentifierExp *exp) = 0;
    virtual int visit(RangeExp *exp) = 0;
    virtual int visit(StringExp *exp) = 0;
    virtual int visit(ParenthesizedExp *exp) = 0;
    virtual int visit(FunctionCallExp *exp) = 0;
    virtual int visit(UnaryExp *exp) = 0;
    virtual int visit(RunExp *exp) = 0;
    virtual void visit(AssignStatement *stm) = 0;
    virtual void visit(PrintStatement *stm) = 0;
    virtual void visit(ExpressionStatement *stm) = 0;
    virtual void visit(IfStatement *stm) = 0;
    virtual void visit(WhileStatement *stm) = 0;
    virtual void visit(DoWhileStatement *stm) = 0;
    virtual void visit(ForStatement *stm) = 0;
    virtual void visit(VarDec *stm) = 0;
    virtual void visit(VarDecList *stm) = 0;
    virtual void visit(StatementList *stm) = 0;
    virtual void visit(Block *stm) = 0;
    virtual void visit(RunBlock *stm) = 0;
    virtual void visit(FunctionDecl *stm) = 0;
    virtual void visit(ReturnStatement *stm) = 0;
    virtual void visit(BreakStatement *stm) = 0;
    virtual void visit(ContinueStatement *stm) = 0;
};

class PrintVisitor : public Visitor
{
private:
    int indent = 0;

    void imprimirIndentacion();

public:
    void imprimir(Program *program);
    int visit(BinaryExp *exp) override;
    int visit(NumberExp *exp) override;
    int visit(DecimalExp *exp) override;
    int visit(BoolExp *exp) override;
    int visit(IdentifierExp *exp) override;
    int visit(RangeExp *exp) override;
    int visit(StringExp *exp) override;
    int visit(ParenthesizedExp *exp) override;
    int visit(FunctionCallExp *exp) override;
    int visit(UnaryExp *exp) override;
    int visit(RunExp *exp) override;
    void visit(AssignStatement *stm) override;
    void visit(PrintStatement *stm) override;
    void visit(ExpressionStatement *stm) override;
    void visit(IfStatement *stm) override;
    void visit(WhileStatement *stm) override;
    void visit(DoWhileStatement *stm) override;
    void visit(ForStatement *stm) override;
    void visit(VarDec *stm) override;
    void visit(VarDecList *stm) override;
    void visit(StatementList *stm) override;
    void visit(Block *stm) override;
    void visit(RunBlock *stm) override;
    void visit(FunctionDecl *stm) override;
    void visit(ReturnStatement *stm) override;
    void visit(BreakStatement *stm) override;
    void visit(ContinueStatement *stm) override;
};

class EvalVisitor : public Visitor
{
    Environment env;
    std::unordered_map<string, FunctionDecl *> functions;
    int lastType;
    int lastInt;
    float lastFloat;
    string lastString;
    bool returnExecuted;
    bool breakExecuted;
    bool continueExecuted;
    bool inBlockExecutionContext;
    bool inFunctionBody;

public:
    void ejecutar(Program *program);
    void executeBlock(Block *block);
    int visit(BinaryExp *exp) override;
    int visit(NumberExp *exp) override;
    int visit(DecimalExp *exp) override;
    int visit(BoolExp *exp) override;
    int visit(IdentifierExp *exp) override;
    int visit(RangeExp *exp) override;
    int visit(StringExp *exp) override;
    int visit(ParenthesizedExp *exp) override;
    int visit(FunctionCallExp *exp) override;
    int visit(UnaryExp *exp) override;
    int visit(RunExp *exp) override;
    void visit(AssignStatement *stm) override;
    void visit(PrintStatement *stm) override;
    void visit(ExpressionStatement *stm) override;
    void visit(IfStatement *stm) override;
    void visit(WhileStatement *stm) override;
    void visit(DoWhileStatement *stm) override;
    void visit(ForStatement *stm) override;
    void visit(VarDec *stm) override;
    void visit(VarDecList *stm) override;
    void visit(StatementList *stm) override;
    void visit(Block *stm) override;
    void visit(RunBlock *stm) override;
    void visit(FunctionDecl *stm) override;
    void visit(ReturnStatement *stm) override;
    void visit(BreakStatement *stm) override;
    void visit(ContinueStatement *stm) override;
};

class GenCodeVisitor : public Visitor
{
private:
    std::ostream &out;
    std::unordered_map<string, bool> memoriaGlobal;
    std::unordered_map<string, int> memoria;
    std::unordered_map<string, int> variableTypes;
    std::unordered_map<double, string> floatConstants;
    std::unordered_map<string, string> functionReturnTypes; // Para almacenar tipos de retorno de funciones
    std::stack<string> labelStack;
    int offset;
    int labelcont;
    int stringBufferCounter; // Para manejar múltiples concatenaciones
    bool entornoFuncion;
    string nombreFuncion;

    int getVariableType(const string &name);
    void setVariableType(const string &name, int type);
    string getFloatConstantLabel(double value);
    void processForFloatConstants(Block *block);
    void processExpForFloatConstants(Exp *exp);

public:
    GenCodeVisitor(std::ostream &output) : out(output), offset(0), labelcont(1), stringBufferCounter(0), entornoFuncion(false) {}

    void generar(Program *program);
    int visit(BinaryExp *exp) override;
    int visit(NumberExp *exp) override;
    int visit(DecimalExp *exp) override;
    int visit(BoolExp *exp) override;
    int visit(IdentifierExp *exp) override;
    int visit(RangeExp *exp) override;
    int visit(StringExp *exp) override;
    int visit(ParenthesizedExp *exp) override;
    int visit(FunctionCallExp *exp) override;
    int visit(UnaryExp *exp) override;
    int visit(RunExp *exp) override;
    void visit(AssignStatement *stm) override;
    void visit(PrintStatement *stm) override;
    void visit(ExpressionStatement *stm) override;
    void visit(IfStatement *stm) override;
    void visit(WhileStatement *stm) override;
    void visit(DoWhileStatement *stm) override;
    void visit(ForStatement *stm) override;
    void visit(VarDec *stm) override;
    void visit(VarDecList *stm) override;
    void visit(StatementList *stm) override;
    void visit(Block *stm) override;
    void visit(RunBlock *stm) override;
    void visit(FunctionDecl *stm) override;
    void visit(ReturnStatement *stm) override;
    void visit(BreakStatement *stm) override;
    void visit(ContinueStatement *stm) override;
};

#endif