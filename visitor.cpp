#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <iomanip>
#include <unordered_map>
#include <typeinfo>
#include <cmath>
#include <sstream>
using namespace std;

string formatFloat(float value)
{
    if (value == floor(value))
    {
        return to_string((int)value);
    }

    string result = to_string(value);

    result.erase(result.find_last_not_of('0') + 1, string::npos);
    result.erase(result.find_last_not_of('.') + 1, string::npos);

    return result;
}

int getOperatorPrecedence(BinaryOp op)
{
    switch (op)
    {
    case OR_OP:
        return 1;
    case AND_OP:
        return 2;
    case EQ_OP:
    case NE_OP:
        return 3;
    case LT_OP:
    case LE_OP:
    case GT_OP:
    case GE_OP:
        return 4;
    case PLUS_OP:
    case MINUS_OP:
        return 5;
    case MUL_OP:
    case DIV_OP:
    case MOD_OP:
        return 6;
    default:
        return 0;
    }
}

bool needsParentheses(Exp *expr, BinaryOp parentOp, bool isRightOperand)
{
    BinaryExp *binExpr = dynamic_cast<BinaryExp *>(expr);
    if (!binExpr)
        return false;

    int parentPrec = getOperatorPrecedence(parentOp);
    int childPrec = getOperatorPrecedence(binExpr->op);

    if (childPrec < parentPrec)
        return true;
    if (childPrec == parentPrec && isRightOperand &&
        (parentOp == MINUS_OP || parentOp == DIV_OP || parentOp == MOD_OP))
        return true;

    return false;
}

int BinaryExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int NumberExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int DecimalExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int BoolExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int RangeExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int StringExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int FunctionCallExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int UnaryExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}
int ForStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}
int StatementList::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int Block::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int RunBlock::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int FunctionDecl::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int ReturnStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int BreakStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int ContinueStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int PrintVisitor::visit(BinaryExp *exp)
{
    bool leftNeedsParens = needsParentheses(exp->left, exp->op, false);
    if (leftNeedsParens && !dynamic_cast<ParenthesizedExp *>(exp->left))
    {
        cout << "(";
        exp->left->accept(this);
        cout << ")";
    }
    else
    {
        exp->left->accept(this);
    }

    cout << ' ' << Exp::binopToChar(exp->op) << ' ';

    bool rightNeedsParens = needsParentheses(exp->right, exp->op, true);
    if (rightNeedsParens && !dynamic_cast<ParenthesizedExp *>(exp->right))
    {
        cout << "(";
        exp->right->accept(this);
        cout << ")";
    }
    else
    {
        exp->right->accept(this);
    }

    return 0;
}

int PrintVisitor::visit(NumberExp *exp)
{
    cout << exp->value;
    if (exp->has_f)
    {
        cout << "f";
    }
    return 0;
}

int PrintVisitor::visit(DecimalExp *exp)
{
    cout << exp->original_text;
    return 0;
}

int PrintVisitor::visit(BoolExp *exp)
{
    (exp->value) ? cout << "true" : cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp *exp)
{
    cout << exp->name;
    return 0;
}

int PrintVisitor::visit(RangeExp *exp)
{
    exp->start->accept(this);
    if (exp->downTo)
        cout << " downTo ";
    else if (exp->until)
        cout << " until ";
    else
        cout << "..";
    exp->end->accept(this);

    if (exp->step != nullptr)
    {
        cout << " step ";
        exp->step->accept(this);
    }

    return 0;
}

int PrintVisitor::visit(StringExp *exp)
{
    cout << "\"" << exp->value << "\"";
    return 0;
}

int PrintVisitor::visit(ParenthesizedExp *exp)
{
    cout << "(";
    exp->expr->accept(this);
    cout << ")";
    return 0;
}

int PrintVisitor::visit(FunctionCallExp *exp)
{
    cout << exp->name << "(";
    bool first = true;
    for (auto arg : exp->args)
    {
        if (!first)
            cout << ", ";
        arg->accept(this);
        first = false;
    }
    cout << ")";
    return 0;
}

int PrintVisitor::visit(UnaryExp *exp)
{
    switch (exp->op)
    {
    case UnaryExp::NOT_OP:
        cout << "!";
        exp->expr->accept(this);
        break;
    case UnaryExp::NEG_OP:
        cout << "-";
        exp->expr->accept(this);
        break;
    case UnaryExp::POS_OP:
        cout << "+";
        exp->expr->accept(this);
        break;
    case UnaryExp::PRE_INC_OP:
        cout << "++";
        exp->expr->accept(this);
        break;
    case UnaryExp::PRE_DEC_OP:
        cout << "--";
        exp->expr->accept(this);
        break;
    case UnaryExp::POST_INC_OP:
        exp->expr->accept(this);
        cout << "++";
        break;
    case UnaryExp::POST_DEC_OP:
        exp->expr->accept(this);
        cout << "--";
        break;
    }
    return 0;
}

int PrintVisitor::visit(RunExp *exp)
{
    cout << "run {" << endl;
    indent++;
    exp->block->statements->accept(this);
    indent--;
    cout << endl;
    imprimirIndentacion();
    cout << "}";
    return 0;
}

void PrintVisitor::visit(AssignStatement *stm)
{
    imprimirIndentacion();
    if (stm->op == AssignStatement::INCREMENT_OP)
    {
        cout << "++" << stm->id;
    }
    else if (stm->op == AssignStatement::DECREMENT_OP)
    {
        cout << "--" << stm->id;
    }
    else if (stm->op == AssignStatement::POST_INCREMENT_OP)
    {
        cout << stm->id << "++";
    }
    else if (stm->op == AssignStatement::POST_DECREMENT_OP)
    {
        cout << stm->id << "--";
    }
    else
    {
        cout << stm->id;
        if (stm->op == AssignStatement::ASSIGN_OP)
            cout << " = ";
        else if (stm->op == AssignStatement::PLUS_ASSIGN_OP)
            cout << " += ";
        else if (stm->op == AssignStatement::MINUS_ASSIGN_OP)
            cout << " -= ";
        else if (stm->op == AssignStatement::MUL_ASSIGN_OP)
            cout << " *= ";
        else if (stm->op == AssignStatement::DIV_ASSIGN_OP)
            cout << " /= ";
        else if (stm->op == AssignStatement::MOD_ASSIGN_OP)
            cout << " %= ";

        stm->rhs->accept(this);
    }
}

void PrintVisitor::visit(PrintStatement *stm)
{
    imprimirIndentacion();
    if (stm->newline == true)
    {
        cout << "println(";
        stm->e->accept(this);
        cout << ")";
    }
    else if (stm->newline == false)
    {
        cout << "print(";
        stm->e->accept(this);
        cout << ")";
    }
}

void PrintVisitor::visit(ExpressionStatement *stm)
{
    imprimirIndentacion();
    stm->expr->accept(this);
}

void PrintVisitor::imprimir(Program *program)
{
    cout << endl;
    indent = 0;
    program->statements->accept(this);
    cout << endl;
};

void PrintVisitor::imprimirIndentacion()
{
    for (int i = 0; i < indent; i++)
        cout << "    ";
}

void PrintVisitor::visit(IfStatement *stm)
{
    imprimirIndentacion();
    cout << "if (";
    stm->condition->accept(this);
    cout << ")" << endl;
    imprimirIndentacion();
    cout << "{" << endl;
    indent++;
    stm->thenStmt->accept(this);
    cout << endl;
    indent--;
    imprimirIndentacion();
    cout << "}" << endl;
    if (stm->elseStmt != nullptr)
    {
        imprimirIndentacion();
        cout << "else" << endl;
        imprimirIndentacion();
        cout << "{" << endl;
        indent++;
        stm->elseStmt->accept(this);
        cout << endl;
        indent--;
        imprimirIndentacion();
        cout << "}" << endl;
    }
}

void PrintVisitor::visit(WhileStatement *stm)
{
    imprimirIndentacion();
    cout << "while (";
    stm->condition->accept(this);
    cout << ")" << endl;
    imprimirIndentacion();
    cout << "{" << endl;
    indent++;
    stm->stmt->accept(this);
    cout << endl;
    indent--;
    imprimirIndentacion();
    cout << "}" << endl;
}

void PrintVisitor::visit(DoWhileStatement *stm)
{
    imprimirIndentacion();
    cout << "do" << endl;
    imprimirIndentacion();
    cout << "{" << endl;
    indent++;
    stm->stmt->accept(this);
    cout << endl;
    indent--;
    imprimirIndentacion();
    cout << "} while (";
    stm->condition->accept(this);
    cout << ")" << endl;
}

void PrintVisitor::visit(ForStatement *stm)
{
    imprimirIndentacion();
    cout << "for (" << stm->id << " in ";
    stm->range->accept(this);
    cout << ")" << endl;
    imprimirIndentacion();
    cout << "{" << endl;
    indent++;
    stm->stmt->accept(this);
    cout << endl;
    indent--;
    imprimirIndentacion();
    cout << "}" << endl;
}

void PrintVisitor::visit(VarDec *stm)
{
    imprimirIndentacion();
    cout << (stm->isVal ? "val " : "var ") << stm->id << " : " << stm->type;
    if (stm->value != nullptr)
    {
        cout << " = ";
        stm->value->accept(this);
    }
}

void PrintVisitor::visit(VarDecList *stm)
{
    for (auto i : stm->decls)
    {
        i->accept(this);
    }
}

void PrintVisitor::visit(StatementList *stm)
{
    auto it = stm->stms.begin();
    while (it != stm->stms.end())
    {
        (*it)->accept(this);
        ++it;
        if (it != stm->stms.end())
        {
            cout << endl;
        }
    }
}

void PrintVisitor::visit(Block *stm)
{
    stm->statements->accept(this);
}

void PrintVisitor::visit(RunBlock *stm)
{
    imprimirIndentacion();
    cout << "run {" << endl;
    indent++;
    stm->statements->accept(this);
    indent--;
    imprimirIndentacion();
    cout << "}";
}

void PrintVisitor::visit(FunctionDecl *stm)
{
    imprimirIndentacion();
    cout << "fun " << stm->name << "(";

    bool first = true;
    for (const auto &param : stm->params)
    {
        if (!first)
            cout << ", ";
        cout << param.first << ": " << param.second;
        first = false;
    }
    cout << "): " << stm->returnType << endl;
    imprimirIndentacion();
    cout << "{" << endl;
    indent++;
    stm->body->accept(this);
    cout << endl;
    indent--;
    imprimirIndentacion();
    cout << "}" << endl;
}

void PrintVisitor::visit(ReturnStatement *stm)
{
    imprimirIndentacion();
    cout << "return";
    if (stm->expr)
    {
        cout << " ";
        stm->expr->accept(this);
    }
}

void PrintVisitor::visit(BreakStatement *stm)
{
    imprimirIndentacion();
    cout << "break";
}

void PrintVisitor::visit(ContinueStatement *stm)
{
    imprimirIndentacion();
    cout << "continue";
}

static int evalTypeFromEnv(Environment &env, string name)
{
    string strtype = env.lookup_type(name);
    if (strtype == "Int")
        return 1;
    if (strtype == "Float")
        return 2;
    if (strtype == "Boolean")
        return 3;
    if (strtype == "String")
        return 5;
    return -1;
}

int EvalVisitor::visit(BinaryExp *exp)
{
    int leftType = exp->left->accept(this);
    int leftInt = lastInt;
    float leftFloat = lastFloat;
    string leftString = lastString;

    int rightType = exp->right->accept(this);
    int rightInt = lastInt;
    float rightFloat = lastFloat;
    string rightString = lastString;

    switch (exp->op)
    {
    case PLUS_OP:
        if (leftType == 5 || rightType == 5)
        {
            lastType = 5;
            string left_str;
            string right_str;

            if (leftType == 5)
            {
                left_str = leftString;
            }
            else if (leftType == 3)
            {
                left_str = leftInt ? "true" : "false";
            }
            else if (leftType == 2)
            {
                left_str = formatFloat(leftFloat);
            }
            else
            {
                left_str = to_string(leftInt);
            }

            if (rightType == 5)
            {
                right_str = rightString;
            }
            else if (rightType == 3)
            {
                right_str = rightInt ? "true" : "false";
            }
            else if (rightType == 2)
            {
                right_str = formatFloat(rightFloat);
            }
            else
            {
                right_str = to_string(rightInt);
            }

            lastString = left_str + right_str;
        }
        else if (leftType == 1 && rightType == 1)
        {
            lastType = 1;
            lastInt = leftInt + rightInt;
        }
        else if (leftType == 2 || rightType == 2)
        {
            lastType = 2;
            lastFloat = (leftType == 2 ? leftFloat : leftInt) + (rightType == 2 ? rightFloat : rightInt);
        }
        break;
    case MINUS_OP:
        if (leftType == 1 && rightType == 1)
        {
            lastType = 1;
            lastInt = leftInt - rightInt;
        }
        else if (leftType == 2 || rightType == 2)
        {
            lastType = 2;
            lastFloat = (leftType == 2 ? leftFloat : leftInt) - (rightType == 2 ? rightFloat : rightInt);
        }
        break;
    case MUL_OP:
        if (leftType == 1 && rightType == 1)
        {
            lastType = 1;
            lastInt = leftInt * rightInt;
        }
        else if (leftType == 2 || rightType == 2)
        {
            lastType = 2;
            lastFloat = (leftType == 2 ? leftFloat : leftInt) * (rightType == 2 ? rightFloat : rightInt);
        }
        break;
    case DIV_OP:
        if (leftType == 1 && rightType == 1)
        {
            lastType = 1;
            lastInt = leftInt / rightInt;
        }
        else
        {
            lastType = 2;
            lastFloat = (leftType == 2 ? leftFloat : leftInt) / (rightType == 2 ? rightFloat : rightInt);
        }
        break;
    case MOD_OP:
        if (leftType == 2 || rightType == 2)
        {
            lastType = 2;
            lastFloat = fmod((leftType == 2 ? leftFloat : leftInt), (rightType == 2 ? rightFloat : rightInt));
        }
        else
        {
            lastType = 1;
            lastInt = leftInt % rightInt;
        }
        break;
    case LT_OP:
        lastType = 3;
        if (leftType == 2 || rightType == 2)
            lastInt = ((leftType == 2 ? leftFloat : leftInt) < (rightType == 2 ? rightFloat : rightInt)) ? 1 : 0;
        else
            lastInt = (leftInt < rightInt) ? 1 : 0;
        break;
    case LE_OP:
        lastType = 3;
        if (leftType == 2 || rightType == 2)
            lastInt = ((leftType == 2 ? leftFloat : leftInt) <= (rightType == 2 ? rightFloat : rightInt)) ? 1 : 0;
        else
            lastInt = (leftInt <= rightInt) ? 1 : 0;
        break;
    case GT_OP:
        lastType = 3;
        if (leftType == 2 || rightType == 2)
            lastInt = ((leftType == 2 ? leftFloat : leftInt) > (rightType == 2 ? rightFloat : rightInt)) ? 1 : 0;
        else
            lastInt = (leftInt > rightInt) ? 1 : 0;
        break;
    case GE_OP:
        lastType = 3;
        if (leftType == 2 || rightType == 2)
            lastInt = ((leftType == 2 ? leftFloat : leftInt) >= (rightType == 2 ? rightFloat : rightInt)) ? 1 : 0;
        else
            lastInt = (leftInt >= rightInt) ? 1 : 0;
        break;
    case EQ_OP:
        lastType = 3;
        if (leftType == 5 && rightType == 5)
            lastInt = (leftString == rightString) ? 1 : 0;
        else if (leftType == 2 || rightType == 2)
            lastInt = ((leftType == 2 ? leftFloat : leftInt) == (rightType == 2 ? rightFloat : rightInt)) ? 1 : 0;
        else
            lastInt = (leftInt == rightInt) ? 1 : 0;
        break;
    case NE_OP:
        lastType = 3;
        if (leftType == 5 && rightType == 5)
            lastInt = (leftString != rightString) ? 1 : 0;
        else if (leftType == 2 || rightType == 2)
            lastInt = ((leftType == 2 ? leftFloat : leftInt) != (rightType == 2 ? rightFloat : rightInt)) ? 1 : 0;
        else
            lastInt = (leftInt != rightInt) ? 1 : 0;
        break;
    case AND_OP:
        lastType = 3;
        lastInt = (leftInt && rightInt) ? 1 : 0;
        break;
    case OR_OP:
        lastType = 3;
        lastInt = (leftInt || rightInt) ? 1 : 0;
        break;
    default:
        cout << "Error: operador binario no soportado." << endl;
        exit(1);
    }
    return lastType;
}

int EvalVisitor::visit(NumberExp *exp)
{
    lastType = 1;
    lastInt = exp->value;
    return lastType;
}
int EvalVisitor::visit(DecimalExp *exp)
{
    lastType = 2;
    lastFloat = exp->value;
    return lastType;
}
int EvalVisitor::visit(BoolExp *exp)
{
    lastType = 3;
    lastInt = exp->value;
    return lastType;
}
int EvalVisitor::visit(IdentifierExp *exp)
{
    int t = evalTypeFromEnv(env, exp->name);
    lastType = t;
    if (t == 1)
        lastInt = env.lookup(exp->name);
    else if (t == 2)
        lastFloat = env.lookup_float(exp->name);
    else if (t == 3)
        lastInt = env.lookup_bool(exp->name);
    else if (t == 5)
        lastString = env.lookup_string(exp->name);
    return t;
}

int EvalVisitor::visit(RangeExp *exp)
{
    int start_t = exp->start->accept(this);
    int start_val = lastInt;
    int end_t = exp->end->accept(this);
    int end_val = lastInt;

    lastType = 4;
    lastInt = start_val;
    lastFloat = (float)end_val;
    return lastType;
}

int EvalVisitor::visit(StringExp *exp)
{
    lastType = 5;
    lastString = exp->value;
    lastInt = 0;
    lastFloat = 0.0f;
    return lastType;
}

int EvalVisitor::visit(ParenthesizedExp *exp)
{

    return exp->expr->accept(this);
}

int EvalVisitor::visit(FunctionCallExp *exp)
{

    auto it = functions.find(exp->name);
    if (it == functions.end())
    {
        cout << "Error: Function '" << exp->name << "' not declared" << endl;
        lastType = 1;
        lastInt = 0;
        return lastType;
    }

    FunctionDecl *func = it->second;

    if (exp->args.size() != func->params.size())
    {
        cout << "Error: Function '" << exp->name << "' expects " << func->params.size()
             << " arguments, got " << exp->args.size() << endl;
        lastType = 1;
        lastInt = 0;
        return lastType;
    }

    env.add_level();

    auto param_it = func->params.begin();
    for (auto arg : exp->args)
    {
        int arg_type = arg->accept(this);
        string param_name = param_it->first;
        string param_type = param_it->second;
        if (param_type == "Int" && arg_type == 1)
        {
            env.add_var(param_name, lastInt, "Int");
        }
        else if (param_type == "Float" && (arg_type == 1 || arg_type == 2))
        {
            env.add_var(param_name, arg_type == 1 ? (float)lastInt : lastFloat, "Float");
        }
        else if (param_type == "String" && arg_type == 5)
        {
            env.add_var(param_name, lastString, "String");
        }

        param_it++;
    }
    bool previousReturnState = returnExecuted;

    returnExecuted = false;
    executeBlock(func->body);

    returnExecuted = previousReturnState;

    if (func->returnType == "Int")
    {
        lastType = 1;
    }
    else if (func->returnType == "Float")
    {
        lastType = 2;
    }
    else if (func->returnType == "String")
    {
        lastType = 5;
    }
    else if (func->returnType == "Boolean")
    {
        lastType = 3;
    }
    else
    {
        lastType = 1;
        lastInt = 0;
    }

    env.remove_level();

    return lastType;
}

int EvalVisitor::visit(UnaryExp *exp)
{
    int operand_t = exp->expr->accept(this);

    switch (exp->op)
    {
    case UnaryExp::NOT_OP:
        lastType = 3;
        lastInt = !lastInt;
        break;
    case UnaryExp::NEG_OP:
        if (operand_t == 1)
        {
            lastType = 1;
            lastInt = -lastInt;
        }
        else if (operand_t == 2)
        {
            lastType = 2;
            lastFloat = -lastFloat;
        }
        break;
    case UnaryExp::POS_OP:
        if (operand_t == 1)
        {
            lastType = 1;
            lastInt = +lastInt;
        }
        else if (operand_t == 2)
        {
            lastType = 2;
            lastFloat = +lastFloat;
        }
        break;
    case UnaryExp::PRE_INC_OP:
    {
        IdentifierExp *id_exp = dynamic_cast<IdentifierExp *>(exp->expr);
        if (id_exp)
        {
            int var_type = evalTypeFromEnv(env, id_exp->name);
            if (var_type == 1)
            {
                int current_val = env.lookup(id_exp->name);
                int new_val = current_val + 1;
                env.update(id_exp->name, new_val);
                lastType = 1;
                lastInt = new_val;
            }
            else if (var_type == 2)
            {
                float current_val = env.lookup_float(id_exp->name);
                float new_val = current_val + 1.0f;
                env.update(id_exp->name, new_val);
                lastType = 2;
                lastFloat = new_val;
            }
        }
        break;
    }
    case UnaryExp::POST_INC_OP:
    {
        IdentifierExp *id_exp = dynamic_cast<IdentifierExp *>(exp->expr);
        if (id_exp)
        {
            int var_type = evalTypeFromEnv(env, id_exp->name);
            if (var_type == 1)
            {
                int current_val = env.lookup(id_exp->name);
                int new_val = current_val + 1;
                env.update(id_exp->name, new_val);
                lastType = 1;
                lastInt = current_val;
            }
            else if (var_type == 2)
            {
                float current_val = env.lookup_float(id_exp->name);
                float new_val = current_val + 1.0f;
                env.update(id_exp->name, new_val);
                lastType = 2;
                lastFloat = current_val;
            }
        }
        break;
    }
    case UnaryExp::PRE_DEC_OP:
    {
        IdentifierExp *id_exp = dynamic_cast<IdentifierExp *>(exp->expr);
        if (id_exp)
        {
            int var_type = evalTypeFromEnv(env, id_exp->name);
            if (var_type == 1)
            {
                int current_val = env.lookup(id_exp->name);
                int new_val = current_val - 1;
                env.update(id_exp->name, new_val);
                lastType = 1;
                lastInt = new_val;
            }
            else if (var_type == 2)
            {
                float current_val = env.lookup_float(id_exp->name);
                float new_val = current_val - 1.0f;
                env.update(id_exp->name, new_val);
                lastType = 2;
                lastFloat = new_val;
            }
        }
        break;
    }
    case UnaryExp::POST_DEC_OP:
    {
        IdentifierExp *id_exp = dynamic_cast<IdentifierExp *>(exp->expr);
        if (id_exp)
        {
            int var_type = evalTypeFromEnv(env, id_exp->name);
            if (var_type == 1)
            {
                int current_val = env.lookup(id_exp->name);
                int new_val = current_val - 1;
                env.update(id_exp->name, new_val);
                lastType = 1;
                lastInt = current_val;
            }
            else if (var_type == 2)
            {
                float current_val = env.lookup_float(id_exp->name);
                float new_val = current_val - 1.0f;
                env.update(id_exp->name, new_val);
                lastType = 2;
                lastFloat = current_val;
            }
        }
        break;
    }
    }

    return lastType;
}

int EvalVisitor::visit(RunExp *exp)
{
    env.add_level();

    int originalType = lastType;
    int originalInt = lastInt;
    float originalFloat = lastFloat;
    string originalString = lastString;

    StatementList *stmtList = exp->block->statements;
    int finalType = 1;
    int finalInt = 0;
    float finalFloat = 0.0f;
    string finalString = "";

    if (stmtList && !stmtList->stms.empty())
    {
        for (auto stmt : stmtList->stms)
        {
            if (ExpressionStatement *exprStmt = dynamic_cast<ExpressionStatement *>(stmt))
            {
                finalType = exprStmt->expr->accept(this);
                finalInt = lastInt;
                finalFloat = lastFloat;
                finalString = lastString;
            }
            else
            {
                stmt->accept(this);
            }

            if (breakExecuted || continueExecuted || returnExecuted)
            {
                break;
            }
        }
    }

    env.remove_level();

    lastType = finalType;
    lastInt = finalInt;
    lastFloat = finalFloat;
    lastString = finalString;

    return lastType;
}

void EvalVisitor::visit(AssignStatement *stm)
{
    if (stm->op == AssignStatement::ASSIGN_OP)
    {
        int t = stm->rhs->accept(this);
        if (t == 2)
            env.update(stm->id, lastFloat);
        else if (t == 5)
            env.update(stm->id, lastString);
        else if (t == 3)
            env.update(stm->id, (bool)lastInt);
        else
            env.update(stm->id, lastInt);
    }
    else if (stm->op == AssignStatement::INCREMENT_OP)
    {
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current + 1);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current + 1.0f);
        }
    }
    else if (stm->op == AssignStatement::DECREMENT_OP)
    {
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current - 1);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current - 1.0f);
        }
    }
    else if (stm->op == AssignStatement::PLUS_ASSIGN_OP)
    {
        int t = stm->rhs->accept(this);
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current + lastInt);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current + (t == 2 ? lastFloat : (float)lastInt));
        }
        else if (type == "String")
        {
            string current = env.lookup_string(stm->id);
            if (t == 1)
            {
                env.update(stm->id, current + to_string(lastInt));
            }
            else if (t == 2)
            {
                env.update(stm->id, current + formatFloat(lastFloat));
            }
            else if (t == 3)
            {
                env.update(stm->id, current + (lastInt ? "true" : "false"));
            }
            else
            {
                env.update(stm->id, current + lastString);
            }
        }
    }
    else if (stm->op == AssignStatement::MINUS_ASSIGN_OP)
    {
        int t = stm->rhs->accept(this);
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current - lastInt);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current - (t == 2 ? lastFloat : (float)lastInt));
        }
    }
    else if (stm->op == AssignStatement::MUL_ASSIGN_OP)
    {
        int t = stm->rhs->accept(this);
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current * lastInt);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current * (t == 2 ? lastFloat : (float)lastInt));
        }
    }
    else if (stm->op == AssignStatement::DIV_ASSIGN_OP)
    {
        int t = stm->rhs->accept(this);
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current / lastInt);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current / (t == 2 ? lastFloat : (float)lastInt));
        }
    }
    else if (stm->op == AssignStatement::MOD_ASSIGN_OP)
    {
        int t = stm->rhs->accept(this);
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current % lastInt);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            float mod_value = (t == 2 ? lastFloat : (float)lastInt);
            float result = fmod(current, mod_value);
            env.update(stm->id, result);
        }
    }
    else if (stm->op == AssignStatement::POST_INCREMENT_OP)
    {
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current + 1);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current + 1.0f);
        }
    }
    else if (stm->op == AssignStatement::POST_DECREMENT_OP)
    {
        string type = env.lookup_type(stm->id);
        if (type == "Int")
        {
            int current = env.lookup(stm->id);
            env.update(stm->id, current - 1);
        }
        else if (type == "Float")
        {
            float current = env.lookup_float(stm->id);
            env.update(stm->id, current - 1.0f);
        }
    }
}

void EvalVisitor::visit(PrintStatement *stm)
{
    int t = stm->e->accept(this);
    if (t == 2)
    {
        cout << formatFloat(lastFloat);
    }
    else if (t == 1)
    {
        cout << lastInt;
    }
    else if (t == 3)
    {
        cout << (lastInt ? "true" : "false");
    }
    else if (t == 4)
    {
        cout << lastInt << ".." << (int)lastFloat;
    }
    else if (t == 5)
    {
        cout << lastString;
    }
    if (stm->newline)
        cout << endl;
}

void EvalVisitor::visit(ExpressionStatement *stm)
{
    stm->expr->accept(this);
}

void EvalVisitor::ejecutar(Program *program)
{
    cout << endl;
    lastType = 1;
    lastInt = 0;
    lastFloat = 0.0f;
    lastString = "";
    returnExecuted = false;
    breakExecuted = false;
    continueExecuted = false;
    inBlockExecutionContext = false;
    inFunctionBody = false;
    env.add_level();

    for (auto stmt : program->statements->stms)
    {
        if (FunctionDecl *funcDecl = dynamic_cast<FunctionDecl *>(stmt))
        {
            functions[funcDecl->name] = funcDecl;
        }
        else if (VarDec *varDecl = dynamic_cast<VarDec *>(stmt))
        {
            stmt->accept(this);
        }
    }
    auto mainFunc = functions.find("main");
    if (mainFunc != functions.end())
    {
        executeBlock(mainFunc->second->body);
    }
    else
    {
        cout << "Error: No se encontro la función main()" << endl;
    }

    cout << endl;
}

void EvalVisitor::visit(VarDec *stm)
{
    if (stm->value)
    {
        int t = stm->value->accept(this);
        if (t == 2)
            env.add_var(stm->id, lastFloat, stm->type);
        else if (t == 5)
            env.add_var(stm->id, lastString, stm->type);
        else if (t == 3)
            env.add_var(stm->id, (bool)lastInt, stm->type);
        else
            env.add_var(stm->id, lastInt, stm->type);
    }
    else
    {
        env.add_var(stm->id, stm->type);
    }
}

void EvalVisitor::visit(VarDecList *stm)
{
    for (auto i : stm->decls)
    {
        i->accept(this);
    }
}

void EvalVisitor::visit(StatementList *stm)
{
    for (auto i : stm->stms)
    {
        i->accept(this);
        if (returnExecuted || breakExecuted || continueExecuted)
            break;
    }
}

void EvalVisitor::visit(Block *stm)
{
    if (inBlockExecutionContext)
    {
        env.add_level();
        stm->statements->accept(this);
        env.remove_level();
    }
}

void EvalVisitor::executeBlock(Block *block)
{
    env.add_level();
    block->statements->accept(this);
    env.remove_level();
}

void EvalVisitor::visit(RunBlock *stm)
{
    env.add_level();
    stm->statements->accept(this);
    env.remove_level();
}

void EvalVisitor::visit(IfStatement *stm)
{
    int t = stm->condition->accept(this);
    bool res = (t == 3) ? (bool)lastInt : (bool)lastInt;

    bool prevInBlockExecutionContext = inBlockExecutionContext;
    inBlockExecutionContext = true;

    if (res)
    {
        stm->thenStmt->accept(this);
    }
    else if (stm->elseStmt != nullptr)
    {
        stm->elseStmt->accept(this);
    }

    inBlockExecutionContext = prevInBlockExecutionContext;
}

void EvalVisitor::visit(WhileStatement *stm)
{
    bool prevInBlockExecutionContext = inBlockExecutionContext;

    while (true)
    {
        int t = stm->condition->accept(this);
        bool res = (t == 3) ? (bool)lastInt : (bool)lastInt;

        if (!res)
            break;

        breakExecuted = false;
        continueExecuted = false;
        inBlockExecutionContext = true;
        stm->stmt->accept(this);
        inBlockExecutionContext = prevInBlockExecutionContext;

        if (breakExecuted)
        {
            breakExecuted = false;
            break;
        }
        if (continueExecuted)
        {
            continueExecuted = false;
            continue;
        }
    }
}

void EvalVisitor::visit(DoWhileStatement *stm)
{
    bool prevInBlockExecutionContext = inBlockExecutionContext;

    do
    {
        breakExecuted = false;
        continueExecuted = false;
        inBlockExecutionContext = true;
        stm->stmt->accept(this);
        inBlockExecutionContext = prevInBlockExecutionContext;

        if (breakExecuted)
        {
            breakExecuted = false;
            break;
        }
        if (continueExecuted)
        {
            continueExecuted = false;
        }
        if (returnExecuted)
        {
            break;
        }

        int t = stm->condition->accept(this);
        bool res = (t == 3) ? (bool)lastInt : (bool)lastInt;

        if (!res)
            break;

    } while (true);
}

void EvalVisitor::visit(ForStatement *stm)
{
    bool prevInBlockExecutionContext = inBlockExecutionContext;

    if (RangeExp *range = dynamic_cast<RangeExp *>(stm->range))
    {
        int start_t = range->start->accept(this);
        int start_val = lastInt;
        int end_t = range->end->accept(this);
        int end_val = lastInt;

        int step_val = 1;
        if (range->step != nullptr)
        {
            int step_t = range->step->accept(this);
            step_val = lastInt;
            if (step_val == 0)
            {
                cout << "Error: step no puede ser 0 en un rango" << endl;
                return;
            }
        }

        env.add_var(stm->id, start_val, "Int");

        if (range->downTo)
        {
            if (step_val > 0)
                step_val = -step_val;
            int limit = range->until ? end_val : end_val - 1;
            for (int i = start_val; i > limit; i += step_val)
            {
                env.update(stm->id, i);

                breakExecuted = false;
                continueExecuted = false;
                inBlockExecutionContext = true;
                stm->stmt->accept(this);
                inBlockExecutionContext = prevInBlockExecutionContext;

                if (breakExecuted)
                {
                    breakExecuted = false;
                    break;
                }
                if (continueExecuted)
                {
                    continueExecuted = false;
                    continue;
                }
            }
        }
        else
        {
            int limit = range->until ? end_val : end_val + 1;
            for (int i = start_val; i < limit; i += step_val)
            {
                env.update(stm->id, i);

                breakExecuted = false;
                continueExecuted = false;
                inBlockExecutionContext = true;
                stm->stmt->accept(this);
                inBlockExecutionContext = prevInBlockExecutionContext;

                if (breakExecuted)
                {
                    breakExecuted = false;
                    break;
                }
                if (continueExecuted)
                {
                    continueExecuted = false;
                    continue;
                }
            }
        }
    }
    else
    {
        cout << "Error: Solo se aceptan expresiones por rango" << endl;
    }
}

void EvalVisitor::visit(FunctionDecl *stm)
{

    functions[stm->name] = stm;
}

void EvalVisitor::visit(ReturnStatement *stm)
{
    if (stm->expr)
    {
        stm->expr->accept(this);
    }
    returnExecuted = true;
}

void EvalVisitor::visit(BreakStatement *stm)
{
    breakExecuted = true;
}

void EvalVisitor::visit(ContinueStatement *stm)
{
    continueExecuted = true;
}

int DoWhileStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

void GenCodeVisitor::generar(Program *program)
{
    if (!program)
        return;

    if (program->statements)
    {
        for (auto stmt : program->statements->stms)
        {
            if (VarDec *varDecl = dynamic_cast<VarDec *>(stmt))
            {
                memoriaGlobal[varDecl->id] = true;
                // Check if the variable has a float initialization
                if (varDecl->value)
                {
                    processExpForFloatConstants(varDecl->value);
                }
            }
            else if (FunctionDecl *funcDecl = dynamic_cast<FunctionDecl *>(stmt))
            {
                // Process function body to collect float constants
                processForFloatConstants(funcDecl->body);
            }
        }
    }

    out << ".data\n";
    out << "print_fmt: .string \"%ld\\n\"\n";
    out << "print_str_fmt: .string \"%s\\n\"\n";
    out << "print_float_fmt: .string \"%.6g\\n\"\n";
    out << "string_buffer: .space 512\n";  // Buffer principal para concatenación de strings
    out << "string_buffer2: .space 512\n"; // Buffer secundario para concatenaciones anidadas
    out << "stringBufferCounter: .long 0\n"; // Contador para alternar buffers

    for (auto it = memoriaGlobal.begin(); it != memoriaGlobal.end(); ++it)
    {
        out << it->first << ": .quad 0\n";
    }

    out << "\n.section .rodata\n";
    out << ".align 8\n";
    out << ".L_zero: .double 0.0\n";
    out << ".L_one: .double 1.0\n";
    out << ".str_concat_fmt: .string \"%s%s\"\n";

    for (auto it = floatConstants.begin(); it != floatConstants.end(); ++it)
    {
        out << it->second << ": .double " << it->first << "\n";
    }

    out << "\n.text\n";

    if (program->statements)
    {
        for (auto stmt : program->statements->stms)
        {
            if (FunctionDecl *funcDecl = dynamic_cast<FunctionDecl *>(stmt))
            {
                funcDecl->accept(this);
            }
        }
    }

    out << ".section .note.GNU-stack,\"\",@progbits\n";
}

int GenCodeVisitor::visit(NumberExp *exp)
{
    out << " movq $" << exp->value << ", %rax\n";
    return 1;
}

int GenCodeVisitor::visit(DecimalExp *exp)
{
    string label = getFloatConstantLabel(exp->value);
    out << " movsd " << label << "(%rip), %xmm0\n";
    return 2;
}

int GenCodeVisitor::visit(BoolExp *exp)
{
    out << " movq $" << (exp->value ? 1 : 0) << ", %rax\n";
    return 3;
}

int GenCodeVisitor::visit(StringExp *exp)
{
    static int stringCount = 0;
    string label = "str_" + to_string(stringCount++);
    out << " .section .rodata\n";
    out << label << ": .string \"" << exp->value << "\"\n";
    out << " .text\n";
    out << " leaq " << label << "(%rip), %rax\n";
    return 5;
}

int GenCodeVisitor::visit(IdentifierExp *exp)
{
    int type = getVariableType(exp->name);

    if (type == 2)
    {
        if (memoriaGlobal.count(exp->name))
            out << " movsd " << exp->name << "(%rip), %xmm0\n";
        else if (memoria.count(exp->name))
            out << " movsd " << memoria[exp->name] << "(%rbp), %xmm0\n";
        else
            out << " xorpd %xmm0, %xmm0\n";
    }
    else
    {
        if (memoriaGlobal.count(exp->name))
            out << " movq " << exp->name << "(%rip), %rax\n";
        else if (memoria.count(exp->name))
            out << " movq " << memoria[exp->name] << "(%rbp), %rax\n";
        else
            out << " movq $0, %rax\n";
    }
    return type;
}

int GenCodeVisitor::visit(BinaryExp *exp)
{
    int leftType = exp->left->accept(this);

    if (leftType == 2)
    {
        out << " movsd %xmm0, -8(%rsp)\n";
        out << " subq $8, %rsp\n";
    }
    else
    {
        out << " pushq %rax\n";
    }

    int rightType = exp->right->accept(this);

    // Manejar concatenación de strings
    if ((leftType == 5 || rightType == 5) && exp->op == PLUS_OP)
    {
        // Implementación simple usando registros temporales
        out << " movq %rax, %r8\n";    // segundo string en %r8
        out << " popq %r9\n";          // primer string en %r9
        
        // Usar string_buffer
        out << " leaq string_buffer(%rip), %rax\n";
        
        // strcpy(buffer, primer_string)
        out << " movq %rax, %rdi\n";   // destino = buffer
        out << " movq %r9, %rsi\n";    // primer string
        out << " call strcpy@PLT\n";
        
        // strcat(buffer, segundo_string)  
        out << " leaq string_buffer(%rip), %rdi\n"; // buffer
        out << " movq %r8, %rsi\n";    // segundo string
        out << " call strcat@PLT\n";    
        
        // Resultado en %rax
        out << " leaq string_buffer(%rip), %rax\n";
        
        return 5; // retorna tipo string
    }

    bool isFloat = (leftType == 2 || rightType == 2);

    if (isFloat)
    {
        if (rightType == 2)
        {
            out << " movsd %xmm0, %xmm1\n";
        }
        else
        {
            out << " cvtsi2sd %rax, %xmm1\n";
        }

        if (leftType == 2)
        {
            out << " movsd (%rsp), %xmm0\n";
            out << " addq $8, %rsp\n";
        }
        else
        {
            out << " popq %rax\n";
            out << " cvtsi2sd %rax, %xmm0\n";
        }

        switch (exp->op)
        {
        case PLUS_OP:
            out << " addsd %xmm1, %xmm0\n";
            break;
        case MINUS_OP:
            out << " subsd %xmm1, %xmm0\n";
            break;
        case MUL_OP:
            out << " mulsd %xmm1, %xmm0\n";
            break;
        case DIV_OP:
            out << " divsd %xmm1, %xmm0\n";
            break;
        case LT_OP:
            out << " comisd %xmm1, %xmm0\n";
            out << " movl $0, %eax\n";
            out << " setb %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case LE_OP:
            out << " comisd %xmm1, %xmm0\n";
            out << " movl $0, %eax\n";
            out << " setbe %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case GT_OP:
            out << " comisd %xmm1, %xmm0\n";
            out << " movl $0, %eax\n";
            out << " seta %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case GE_OP:
            out << " comisd %xmm1, %xmm0\n";
            out << " movl $0, %eax\n";
            out << " setae %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case EQ_OP:
            out << " comisd %xmm1, %xmm0\n";
            out << " movl $0, %eax\n";
            out << " sete %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case NE_OP:
            out << " comisd %xmm1, %xmm0\n";
            out << " movl $0, %eax\n";
            out << " setne %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        }
        return 2;
    }
    else
    {
        out << " movq %rax, %rcx\n";
        out << " popq %rax\n";

        switch (exp->op)
        {
        case PLUS_OP:
            out << " addq %rcx, %rax\n";
            break;
        case MINUS_OP:
            out << " subq %rcx, %rax\n";
            break;
        case MUL_OP:
            out << " imulq %rcx, %rax\n";
            break;
        case DIV_OP:
            out << " cqto\n";
            out << " idivq %rcx\n";
            break;
        case MOD_OP:
            out << " cqto\n";
            out << " idivq %rcx\n";
            out << " movq %rdx, %rax\n";
            break;
        case LT_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setl %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case LE_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setle %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case GT_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setg %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case GE_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setge %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case EQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " sete %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case NE_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setne %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case AND_OP:
            out << " testq %rax, %rax\n";
            out << " setne %al\n";
            out << " testq %rcx, %rcx\n";
            out << " setne %cl\n";
            out << " andb %cl, %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        case OR_OP:
            out << " testq %rax, %rax\n";
            out << " setne %al\n";
            out << " testq %rcx, %rcx\n";
            out << " setne %cl\n";
            out << " orb %cl, %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        }
        return 1;
    }
}

int GenCodeVisitor::visit(UnaryExp *exp)
{
    int type = exp->expr->accept(this);

    switch (exp->op)
    {
    case UnaryExp::NEG_OP:
        if (type == 2)
        {
            out << " xorpd %xmm1, %xmm1\n";
            out << " subsd %xmm0, %xmm1\n";
            out << " movsd %xmm1, %xmm0\n";
        }
        else
        {
            out << " negq %rax\n";
        }
        break;
    case UnaryExp::POS_OP:
        break;
    case UnaryExp::NOT_OP:
        if (type == 2)
        {
            out << " xorpd %xmm1, %xmm1\n";
            out << " comisd %xmm1, %xmm0\n";
            out << " setz %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        }
        else
        {
            out << " testq %rax, %rax\n";
            out << " setz %al\n";
            out << " movzbq %al, %rax\n";
            return 3;
        }
        break;
    case UnaryExp::PRE_INC_OP:
    case UnaryExp::POST_INC_OP:
    case UnaryExp::PRE_DEC_OP:
    case UnaryExp::POST_DEC_OP:
        if (IdentifierExp *id_exp = dynamic_cast<IdentifierExp *>(exp->expr))
        {
            int var_type = getVariableType(id_exp->name);
            if (var_type == 2)
            {
                if (memoriaGlobal.count(id_exp->name))
                {
                    out << " movsd " << id_exp->name << "(%rip), %xmm0\n";
                    out << " movsd .L_one(%rip), %xmm1\n";
                    if (exp->op == UnaryExp::PRE_INC_OP || exp->op == UnaryExp::POST_INC_OP)
                        out << " addsd %xmm1, %xmm0\n";
                    else
                        out << " subsd %xmm1, %xmm0\n";
                    out << " movsd %xmm0, " << id_exp->name << "(%rip)\n";
                }
                else
                {
                    out << " movsd " << memoria[id_exp->name] << "(%rbp), %xmm0\n";
                    out << " movsd .L_one(%rip), %xmm1\n";
                    out << " addsd %xmm1, %xmm0\n";
                    out << " movsd %xmm0, " << memoria[id_exp->name] << "(%rbp)\n";
                }
                return 2;
            }
            else
            {
                if (memoriaGlobal.count(id_exp->name))
                {
                    out << " movq " << id_exp->name << "(%rip), %rax\n";
                    if (exp->op == UnaryExp::PRE_INC_OP || exp->op == UnaryExp::POST_INC_OP)
                    {
                        out << " incq " << id_exp->name << "(%rip)\n";
                        if (exp->op == UnaryExp::PRE_INC_OP)
                            out << " incq %rax\n";
                    }
                    else
                    {
                        out << " decq " << id_exp->name << "(%rip)\n";
                        if (exp->op == UnaryExp::PRE_DEC_OP)
                            out << " decq %rax\n";
                    }
                }
                else
                {
                    out << " movq " << memoria[id_exp->name] << "(%rbp), %rax\n";
                    if (exp->op == UnaryExp::PRE_INC_OP || exp->op == UnaryExp::POST_INC_OP)
                    {
                        out << " incq " << memoria[id_exp->name] << "(%rbp)\n";
                        if (exp->op == UnaryExp::PRE_INC_OP)
                            out << " incq %rax\n";
                        // Para POST_INC_OP, NO incrementamos %rax, retornamos el valor original
                    }
                    else
                    {
                        out << " decq " << memoria[id_exp->name] << "(%rbp)\n";
                        if (exp->op == UnaryExp::PRE_DEC_OP)
                            out << " decq %rax\n";
                        // Para POST_DEC_OP, NO decrementamos %rax, retornamos el valor original
                    }
                }
                return 1;
            }
        }
        break;
    default:
        break;
    }
    return type;
}

int GenCodeVisitor::visit(ParenthesizedExp *exp)
{
    return exp->expr->accept(this);
}

int GenCodeVisitor::visit(RangeExp *exp)
{

    return 0;
}

int GenCodeVisitor::visit(RunExp *exp)
{
    if (exp->block && exp->block->statements)
    {
        exp->block->statements->accept(this);
    }
    return 0;
}

int GenCodeVisitor::visit(FunctionCallExp *exp)
{
    vector<string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    vector<string> xmmRegs = {"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"};

    if (exp->name == "println" || exp->name == "print")
    {
        if (!exp->args.empty())
        {
            auto it = exp->args.begin();
            int type = (*it)->accept(this);

            if (type == 5)
            {
                out << " movq %rax, %rsi\n";
                out << " leaq print_str_fmt(%rip), %rdi\n";
                out << " movl $0, %eax\n";
                out << " call printf@PLT\n";
            }
            else if (type == 2)
            {
                out << " movq $1, %rax\n";
                out << " leaq print_float_fmt(%rip), %rdi\n";
                out << " call printf@PLT\n";
            }
            else
            {
                out << " movq %rax, %rsi\n";
                out << " leaq print_fmt(%rip), %rdi\n";
                out << " xorl %eax, %eax\n";  // Limpiar %eax antes de printf
                out << " call printf@PLT\n";
            }
        }
        return 0;
    }

    int intArgIndex = 0;
    int floatArgIndex = 0;

    for (auto it = exp->args.begin(); it != exp->args.end(); it++)
    {
        int argType = (*it)->accept(this);

        if (argType == 2)
        {
            if (floatArgIndex < 8)
            {
                if (floatArgIndex != 0)
                {
                    out << " movsd %xmm0, " << xmmRegs[floatArgIndex] << "\n";
                }
                floatArgIndex++;
            }
            else
            {
                out << " movsd %xmm0, -8(%rsp)\n";
                out << " subq $8, %rsp\n";
            }
        }
        else
        {
            if (intArgIndex < 6)
            {
                out << " movq %rax, " << argRegs[intArgIndex] << "\n";
                intArgIndex++;
            }
            else
            {
                out << " pushq %rax\n";
            }
        }
    }

    out << " movl $" << min(floatArgIndex, 8) << ", %eax\n";
    out << " call " << exp->name << "\n";

    if (exp->args.size() > 6)
    {
        int stackCleanup = (exp->args.size() - 6) * 8;
        out << " addq $" << stackCleanup << ", %rsp\n";
    }

    // Determinar el tipo de retorno de la función
    if (functionReturnTypes.count(exp->name))
    {
        string returnType = functionReturnTypes[exp->name];
        if (returnType == "Float")
            return 2;
        else if (returnType == "Boolean")
            return 3;
        else if (returnType == "String")
            return 5;
        else if (returnType == "Int")
            return 1;
    }

    return 0;
}

void GenCodeVisitor::visit(AssignStatement *stm)
{
    if (!stm)
        return;

    int varType = getVariableType(stm->id);

    switch (stm->op)
    {
    case AssignStatement::ASSIGN_OP:
    {
        int valueType = stm->rhs->accept(this);

        if (varType == 2)
        {
            if (valueType == 2)
            {
                if (memoriaGlobal.count(stm->id))
                    out << " movsd %xmm0, " << stm->id << "(%rip)\n";
                else
                    out << " movsd %xmm0, " << memoria[stm->id] << "(%rbp)\n";
            }
            else
            {
                out << " cvtsi2sd %rax, %xmm0\n";
                if (memoriaGlobal.count(stm->id))
                    out << " movsd %xmm0, " << stm->id << "(%rip)\n";
                else
                    out << " movsd %xmm0, " << memoria[stm->id] << "(%rbp)\n";
            }
        }
        else
        {
            if (valueType == 2)
            {
                out << " cvttsd2si %xmm0, %rax\n";
            }
            if (memoriaGlobal.count(stm->id))
                out << " movq %rax, " << stm->id << "(%rip)\n";
            else
                out << " movq %rax, " << memoria[stm->id] << "(%rbp)\n";
        }
        break;
    }

    case AssignStatement::PLUS_ASSIGN_OP:
    case AssignStatement::MINUS_ASSIGN_OP:
    case AssignStatement::MUL_ASSIGN_OP:
    case AssignStatement::DIV_ASSIGN_OP:
    case AssignStatement::MOD_ASSIGN_OP:
    {
        // Manejar concatenación de strings para +=
        if (varType == 5 && stm->op == AssignStatement::PLUS_ASSIGN_OP)
        {
            // Cargar la variable string actual (primer string)
            if (memoriaGlobal.count(stm->id))
                out << " movq " << stm->id << "(%rip), %rdi\n";
            else
                out << " movq " << memoria[stm->id] << "(%rbp), %rdi\n";
            
            out << " pushq %rdi\n";  // guardar primer string
            
            // Evaluar el lado derecho (segundo string)
            int rhsType = stm->rhs->accept(this);
            out << " movq %rax, %rsi\n";  // segundo string en %rsi
            out << " pushq %rsi\n";  // guardar segundo string
            
            // Usar sprintf para concatenar: sprintf(buffer, "%s%s", str1, str2)
            out << " leaq string_buffer(%rip), %rdi\n";  // destino
            out << " leaq .str_concat_fmt(%rip), %rsi\n";  // formato "%s%s"
            out << " movq 8(%rsp), %rdx\n";  // primer string
            out << " movq (%rsp), %rcx\n";   // segundo string
            out << " movl $0, %eax\n";
            out << " call sprintf@PLT\n";
            
            out << " addq $16, %rsp\n";  // limpiar stack
            
            // Guardar el resultado (puntero al buffer)
            out << " leaq string_buffer(%rip), %rax\n";
            if (memoriaGlobal.count(stm->id))
                out << " movq %rax, " << stm->id << "(%rip)\n";
            else
                out << " movq %rax, " << memoria[stm->id] << "(%rbp)\n";
        }
        else if (varType == 2)
        {
            if (memoriaGlobal.count(stm->id))
                out << " movsd " << stm->id << "(%rip), %xmm0\n";
            else
                out << " movsd " << memoria[stm->id] << "(%rbp), %xmm0\n";

            out << " movsd %xmm0, -8(%rsp)\n";
            out << " subq $8, %rsp\n";

            int rhsType = stm->rhs->accept(this);

            if (rhsType == 1)
            {
                out << " cvtsi2sd %rax, %xmm0\n";
            }

            out << " movsd (%rsp), %xmm1\n";
            out << " addq $8, %rsp\n";

            switch (stm->op)
            {
            case AssignStatement::PLUS_ASSIGN_OP:
                out << " addsd %xmm0, %xmm1\n";
                break;
            case AssignStatement::MINUS_ASSIGN_OP:
                out << " subsd %xmm0, %xmm1\n";
                break;
            case AssignStatement::MUL_ASSIGN_OP:
                out << " mulsd %xmm0, %xmm1\n";
                break;
            case AssignStatement::DIV_ASSIGN_OP:
                out << " divsd %xmm0, %xmm1\n";
                break;
            }

            if (memoriaGlobal.count(stm->id))
                out << " movsd %xmm1, " << stm->id << "(%rip)\n";
            else
                out << " movsd %xmm1, " << memoria[stm->id] << "(%rbp)\n";
        }
        else
        {
            if (memoriaGlobal.count(stm->id))
                out << " movq " << stm->id << "(%rip), %rax\n";
            else
                out << " movq " << memoria[stm->id] << "(%rbp), %rax\n";
            out << " pushq %rax\n";

            int rhsType = stm->rhs->accept(this);
            if (rhsType == 2)
            {
                out << " cvttsd2si %xmm0, %rax\n";
            }
            out << " movq %rax, %rcx\n";
            out << " popq %rax\n";

            switch (stm->op)
            {
            case AssignStatement::PLUS_ASSIGN_OP:
                out << " addq %rcx, %rax\n";
                break;
            case AssignStatement::MINUS_ASSIGN_OP:
                out << " subq %rcx, %rax\n";
                break;
            case AssignStatement::MUL_ASSIGN_OP:
                out << " imulq %rcx, %rax\n";
                break;
            case AssignStatement::DIV_ASSIGN_OP:
                out << " cqto\n";
                out << " idivq %rcx\n";
                break;
            case AssignStatement::MOD_ASSIGN_OP:
                out << " cqto\n";
                out << " idivq %rcx\n";
                out << " movq %rdx, %rax\n";
                break;
            }

            if (memoriaGlobal.count(stm->id))
                out << " movq %rax, " << stm->id << "(%rip)\n";
            else
                out << " movq %rax, " << memoria[stm->id] << "(%rbp)\n";
        }
        break;
    }

    case AssignStatement::INCREMENT_OP:
    case AssignStatement::POST_INCREMENT_OP:
        if (varType == 2)
        {
            if (memoriaGlobal.count(stm->id))
            {
                out << " movsd " << stm->id << "(%rip), %xmm0\n";
                out << " movsd .L_one(%rip), %xmm1\n";
                out << " addsd %xmm1, %xmm0\n";
                out << " movsd %xmm0, " << stm->id << "(%rip)\n";
            }
            else
            {
                out << " movsd " << memoria[stm->id] << "(%rbp), %xmm0\n";
                out << " movsd .L_one(%rip), %xmm1\n";
                out << " addsd %xmm1, %xmm0\n";
                out << " movsd %xmm0, " << memoria[stm->id] << "(%rbp)\n";
            }
        }
        else
        {
            if (memoriaGlobal.count(stm->id))
                out << " incq " << stm->id << "(%rip)\n";
            else
                out << " incq " << memoria[stm->id] << "(%rbp)\n";
        }
        break;

    case AssignStatement::DECREMENT_OP:
    case AssignStatement::POST_DECREMENT_OP:
        if (varType == 2)
        {
            if (memoriaGlobal.count(stm->id))
            {
                out << " movsd " << stm->id << "(%rip), %xmm0\n";
                out << " movsd .L_one(%rip), %xmm1\n";
                out << " subsd %xmm1, %xmm0\n";
                out << " movsd %xmm0, " << stm->id << "(%rip)\n";
            }
            else
            {
                out << " movsd " << memoria[stm->id] << "(%rbp), %xmm0\n";
                out << " movsd .L_one(%rip), %xmm1\n";
                out << " subsd %xmm1, %xmm0\n";
                out << " movsd %xmm0, " << memoria[stm->id] << "(%rbp)\n";
            }
        }
        else
        {
            if (memoriaGlobal.count(stm->id))
                out << " decq " << stm->id << "(%rip)\n";
            else
                out << " decq " << memoria[stm->id] << "(%rbp)\n";
        }
        break;
    }
}

void GenCodeVisitor::visit(PrintStatement *stm)
{
    if (!stm || !stm->e)
        return;

    int type = stm->e->accept(this);

    if (type == 5)
    {
        out << " movq %rax, %rsi\n";
        out << " leaq print_str_fmt(%rip), %rdi\n";
        out << " movl $0, %eax\n";
        out << " call printf@PLT\n";
    }
    else if (type == 2)
    {
        out << " movq $1, %rax\n";
        out << " leaq print_float_fmt(%rip), %rdi\n";
        out << " call printf@PLT\n";
    }
    else
    {
        out << " movq %rax, %rsi\n";
        out << " leaq print_fmt(%rip), %rdi\n";
        out << " movl $0, %eax\n";
        out << " call printf@PLT\n";
    }
}

void GenCodeVisitor::visit(ExpressionStatement *stm)
{
    if (stm && stm->expr)
    {
        stm->expr->accept(this);
    }
}

void GenCodeVisitor::visit(VarDec *stm)
{
    if (!stm)
        return;

    int varType = 1;
    if (stm->type == "Float")
    {
        varType = 2;
    }
    else if (stm->type == "Boolean")
    {
        varType = 3;
    }
    else if (stm->type == "String")
    {
        varType = 5;
    }

    setVariableType(stm->id, varType);

    if (!entornoFuncion)
    {
        memoriaGlobal[stm->id] = true;
    }
    else
    {
        memoria[stm->id] = offset;
        offset -= 8;
    }

    if (stm->value)
    {
        int valueType = stm->value->accept(this);

        if (varType == 2)
        {
            if (valueType == 2)
            {
                if (!entornoFuncion)
                {
                    out << " movsd %xmm0, " << stm->id << "(%rip)\n";
                }
                else
                {
                    out << " movsd %xmm0, " << memoria[stm->id] << "(%rbp)\n";
                }
            }
            else
            {
                out << " cvtsi2sd %rax, %xmm0\n";
                if (!entornoFuncion)
                {
                    out << " movsd %xmm0, " << stm->id << "(%rip)\n";
                }
                else
                {
                    out << " movsd %xmm0, " << memoria[stm->id] << "(%rbp)\n";
                }
            }
        }
        else
        {
            if (valueType == 2)
            {
                out << " cvttsd2si %xmm0, %rax\n";
            }
            if (!entornoFuncion)
            {
                out << " movq %rax, " << stm->id << "(%rip)\n";
            }
            else
            {
                out << " movq %rax, " << memoria[stm->id] << "(%rbp)\n";
            }
        }
    }
}

void GenCodeVisitor::visit(VarDecList *stm)
{
    if (stm)
    {
        for (auto dec : stm->decls)
        {
            if (dec)
                dec->accept(this);
        }
    }
}

void GenCodeVisitor::visit(StatementList *stm)
{
    if (stm)
    {
        for (auto s : stm->stms)
        {
            if (s)
                s->accept(this);
        }
    }
}

void GenCodeVisitor::visit(Block *stm)
{
    if (stm && stm->statements)
    {
        stm->statements->accept(this);
    }
}

void GenCodeVisitor::visit(RunBlock *stm)
{
    if (stm && stm->statements)
    {
        stm->statements->accept(this);
    }
}

void GenCodeVisitor::visit(IfStatement *stm)
{
    if (!stm)
        return;

    int label = labelcont++;
    stm->condition->accept(this);
    out << " cmpq $0, %rax\n";
    out << " je else_" << label << "\n";
    if (stm->thenStmt)
        stm->thenStmt->accept(this);
    out << " jmp endif_" << label << "\n";
    out << "else_" << label << ":\n";
    if (stm->elseStmt)
        stm->elseStmt->accept(this);
    out << "endif_" << label << ":\n";
}

void GenCodeVisitor::visit(WhileStatement *stm)
{
    if (!stm)
        return;

    int labelId = labelcont++;

    out << ".while_start_" << labelId << ":" << endl;
    
    // Evaluar la condición
    stm->condition->accept(this);
    
    // Si la condición es float, convertir a entero
    // (En nuestro caso, las condiciones enteras están en %rax)
    out << "    cmpq $0, %rax" << endl;
    out << "    je .while_end_" << labelId << endl;
    
    // Ejecutar el cuerpo del bucle
    if (stm->stmt)
        stm->stmt->accept(this);
    
    // Saltar de vuelta al inicio
    out << "    jmp .while_start_" << labelId << endl;
    out << ".while_end_" << labelId << ":" << endl;
}

void GenCodeVisitor::visit(DoWhileStatement *stm)
{
    if (!stm)
        return;

    int labelId = labelcont++;
    string doLabel = "dowhile_" + to_string(labelId);
    string endLabel = "enddowhile_" + to_string(labelId);

    labelStack.push(doLabel);

    out << ".do_while_start_" << labelId << ":" << endl;
    
    // Ejecutar el cuerpo del bucle primero
    if (stm->stmt)
        stm->stmt->accept(this);
    
    // Evaluar la condición
    int conditionType = stm->condition->accept(this);
    
    // Manejar conversión de float a int si es necesario
    if (conditionType == 2) { // Si es Float
        out << "    cvttsd2si %xmm0, %rax" << endl;
    }
    
    out << "    cmpq $0, %rax" << endl;
    out << "    jne .do_while_start_" << labelId << endl;
    
    out << ".do_while_end_" << labelId << ":" << endl;

    labelStack.pop();
}

void GenCodeVisitor::visit(ForStatement *stm)
{
    if (!stm)
        return;

    // Verificamos si es que el rango es de tipo rango, si usa un id o un metodo de arrays
    if (RangeExp *range = dynamic_cast<RangeExp *>(stm->range))
    {
        // Manejar for (i in range)
        int labelId = labelcont++;

        // Evaluar y guardar start y end
        range->start->accept(this);
        offset -= 8;
        int startOffset = offset;
        out << "    movq %rax, " << startOffset << "(%rbp)" << endl;

        range->end->accept(this);
        offset -= 8;
        int endOffset = offset;
        out << "    movq %rax, " << endOffset << "(%rbp)" << endl;
        
        int stepValue = 1;
        int stepOffset = 0;
        if (range->step != nullptr) { // si es que estamos definiendo un step distinto de 1
            range->step->accept(this); // genero el valor de ese step
            offset -= 8; 
            stepOffset = offset;
            out << "    movq %rax, " << stepOffset << "(%rbp)" << endl; // lo guardo en memoria
        }
        
        // variable del loop
        if (memoria.find(stm->id) == memoria.end()) {
            offset -= 8;
            memoria[stm->id] = offset;
            setVariableType(stm->id, 1);
        }
        
        out << "    movq " << startOffset << "(%rbp), %rax" << endl;
        out << "    movq %rax, " << memoria[stm->id] << "(%rbp)" << endl;
        
        out << ".for_start_" << labelId << ":" << endl;
        
        out << "    movq " << memoria[stm->id] << "(%rbp), %rax" << endl;
        out << "    movq " << endOffset << "(%rbp), %rcx" << endl;
        out << "    cmpq %rcx, %rax" << endl;
        
        if (range->until) {
            // i < end
            out << "    jge .for_end_" << labelId << endl;
        } else if (range->downTo) {
            // i >= end
            out << "    jl .for_end_" << labelId << endl;
        } else {
            // i <= end (for .. to ..)
            out << "    jg .for_end_" << labelId << endl;
        }
        
        if (stm->stmt)
            stm->stmt->accept(this);
        
        out << "    movq " << memoria[stm->id] << "(%rbp), %rax" << endl;
        
        if (range->downTo) {
            // Para downTo: decrementar
            if (range->step != nullptr) { // si es que tomamos un step distinto a uno, lo recuperamos del stepOffSet
                out << "    movq " << stepOffset << "(%rbp), %rcx" << endl;
                out << "    subq %rcx, %rax" << endl;
            } else {
                out << "    decq %rax" << endl;  // so reducimos en 1 como siempre
            }
        } else {
            if (range->step != nullptr) { // misma logica, si es que tenemos un step que no es 1, lo recuperamos, sino añadimos 1
                out << "    movq " << stepOffset << "(%rbp), %rcx" << endl;
                out << "    addq %rcx, %rax" << endl;
            } else {
                out << "    incq %rax" << endl;
            }
        }
        
        out << "    movq %rax, " << memoria[stm->id] << "(%rbp)" << endl;
        
        out << "    jmp .for_start_" << labelId << endl;
        out << ".for_end_" << labelId << ":" << endl;
    }
    // Aquí se pueden agregar más casos para IdentifierExp y ArrayMethodExp si es necesario
}

void GenCodeVisitor::visit(FunctionDecl *stm)
{
    if (!stm)
        return;

    // Almacenar el tipo de retorno de la función
    functionReturnTypes[stm->name] = stm->returnType;

    entornoFuncion = true;
    memoria.clear();
    offset = -8;
    nombreFuncion = stm->name;

    vector<string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    vector<string> xmmRegs = {"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"};

    out << ".globl " << stm->name << "\n";
    out << stm->name << ":\n";
    out << " pushq %rbp\n";
    out << " movq %rsp, %rbp\n";

    int intParamIndex = 0;
    int floatParamIndex = 0;

    for (auto it = stm->params.begin(); it != stm->params.end(); it++)
    {
        string paramName = it->first;
        string paramType = it->second;

        memoria[paramName] = offset;

        if (paramType == "Float")
        {
            setVariableType(paramName, 2);
            if (floatParamIndex < 8)
            {
                out << " movsd " << xmmRegs[floatParamIndex] << ", " << offset << "(%rbp)\n";
                floatParamIndex++;
            }
        }
        else
        {
            int type = 1;
            if (paramType == "Boolean")
                type = 3;
            else if (paramType == "String")
                type = 5;

            setVariableType(paramName, type);

            if (intParamIndex < 6)
            {
                out << " movq " << argRegs[intParamIndex] << ", " << offset << "(%rbp)\n";
                intParamIndex++;
            }
        }

        offset -= 8;
    }

    int reserva = -offset - 8;
    if (reserva > 0)
    {
        out << " subq $" << reserva << ", %rsp\n";
    }

    if (stm->body)
    {
        stm->body->accept(this);
    }

    out << ".end_" << stm->name << ":\n";

    if (stm->returnType == "Float")
    {
    }
    else
    {
    }

    out << " leave\n";
    out << " ret\n";

    entornoFuncion = false;
}

void GenCodeVisitor::visit(ReturnStatement *stm)
{
    if (stm && stm->expr)
    {
        int type = stm->expr->accept(this);
        // El valor ya está en %rax para enteros o %xmm0 para floats
        // No necesitamos hacer nada más, las convenciones de llamada se encargan
    }
    out << "    jmp .end_" << nombreFuncion << endl;
}

void GenCodeVisitor::visit(BreakStatement *stm)
{
    if (!labelStack.empty())
    {
        string currentLabel = labelStack.top();
        if (currentLabel.find("while_") == 0)
        {
            string label = currentLabel.substr(6);
            out << "    jmp .while_end_" << label << endl;
        }
        else if (currentLabel.find("dowhile_") == 0)
        {
            string label = currentLabel.substr(8);
            out << "    jmp .do_while_end_" << label << endl;
        }
        else if (currentLabel.find("for_") == 0)
        {
            string label = currentLabel.substr(4);
            out << "    jmp .for_end_" << label << endl;
        }
    }
}

void GenCodeVisitor::visit(ContinueStatement *stm)
{
    if (!labelStack.empty())
    {
        string currentLabel = labelStack.top();
        if (currentLabel.find("while_") == 0)
        {
            string label = currentLabel.substr(6);
            out << "    jmp .while_start_" << label << endl;
        }
        else if (currentLabel.find("dowhile_") == 0)
        {
            string label = currentLabel.substr(8);
            out << "    jmp .do_while_start_" << label << endl;
        }
        else if (currentLabel.find("for_") == 0)
        {
            string label = currentLabel.substr(4);
            out << "    jmp .for_start_" << label << endl;
        }
    }
}

int GenCodeVisitor::getVariableType(const string &name)
{
    if (variableTypes.count(name))
    {
        return variableTypes[name];
    }
    return 1;
}

void GenCodeVisitor::setVariableType(const string &name, int type)
{
    variableTypes[name] = type;
}

string GenCodeVisitor::getFloatConstantLabel(double value)
{
    for (auto it = floatConstants.begin(); it != floatConstants.end(); ++it)
    {
        if (it->first == value)
        {
            return it->second;
        }
    }
    string label = ".float_" + to_string(floatConstants.size());
    floatConstants[value] = label;
    return label;
}

void GenCodeVisitor::processForFloatConstants(Block *block)
{
    if (!block || !block->statements)
        return;

    for (auto stmt : block->statements->stms)
    {
        if (VarDec *varDecl = dynamic_cast<VarDec *>(stmt))
        {
            if (varDecl->value)
            {
                processExpForFloatConstants(varDecl->value);
            }
        }
        else if (AssignStatement *assign = dynamic_cast<AssignStatement *>(stmt))
        {
            if (assign->rhs)
            {
                processExpForFloatConstants(assign->rhs);
            }
        }
        else if (PrintStatement *print = dynamic_cast<PrintStatement *>(stmt))
        {
            if (print->e)
            {
                processExpForFloatConstants(print->e);
            }
        }
        else if (ExpressionStatement *exprStmt = dynamic_cast<ExpressionStatement *>(stmt))
        {
            if (exprStmt->expr)
            {
                processExpForFloatConstants(exprStmt->expr);
            }
        }
        else if (IfStatement *ifStmt = dynamic_cast<IfStatement *>(stmt))
        {
            if (ifStmt->condition)
                processExpForFloatConstants(ifStmt->condition);
            if (ifStmt->thenStmt)
                processForFloatConstants(dynamic_cast<Block *>(ifStmt->thenStmt));
            if (ifStmt->elseStmt)
                processForFloatConstants(dynamic_cast<Block *>(ifStmt->elseStmt));
        }
        else if (WhileStatement *whileStmt = dynamic_cast<WhileStatement *>(stmt))
        {
            if (whileStmt->condition)
                processExpForFloatConstants(whileStmt->condition);
            if (whileStmt->stmt)
                processForFloatConstants(dynamic_cast<Block *>(whileStmt->stmt));
        }
        else if (DoWhileStatement *doWhileStmt = dynamic_cast<DoWhileStatement *>(stmt))
        {
            if (doWhileStmt->condition)
                processExpForFloatConstants(doWhileStmt->condition);
            if (doWhileStmt->stmt)
                processForFloatConstants(dynamic_cast<Block *>(doWhileStmt->stmt));
        }
        else if (ForStatement *forStmt = dynamic_cast<ForStatement *>(stmt))
        {
            if (forStmt->range)
                processExpForFloatConstants(forStmt->range);
            if (forStmt->stmt)
                processForFloatConstants(dynamic_cast<Block *>(forStmt->stmt));
        }
        else if (FunctionDecl *funcDecl = dynamic_cast<FunctionDecl *>(stmt))
        {
            if (funcDecl->body)
                processForFloatConstants(funcDecl->body);
        }
        else if (ReturnStatement *retStmt = dynamic_cast<ReturnStatement *>(stmt))
        {
            if (retStmt->expr)
                processExpForFloatConstants(retStmt->expr);
        }
    }
}

void GenCodeVisitor::processExpForFloatConstants(Exp *exp)
{
    if (!exp)
        return;

    if (DecimalExp *decExp = dynamic_cast<DecimalExp *>(exp))
    {
        getFloatConstantLabel(decExp->value);
    }
    else if (BinaryExp *binExp = dynamic_cast<BinaryExp *>(exp))
    {
        processExpForFloatConstants(binExp->left);
        processExpForFloatConstants(binExp->right);
    }
    else if (UnaryExp *unExp = dynamic_cast<UnaryExp *>(exp))
    {
        processExpForFloatConstants(unExp->expr);
    }
    else if (ParenthesizedExp *parenExp = dynamic_cast<ParenthesizedExp *>(exp))
    {
        processExpForFloatConstants(parenExp->expr);
    }
    else if (FunctionCallExp *funcCall = dynamic_cast<FunctionCallExp *>(exp))
    {
        for (auto arg : funcCall->args)
        {
            processExpForFloatConstants(arg);
        }
    }
    else if (RangeExp *rangeExp = dynamic_cast<RangeExp *>(exp))
    {
        processExpForFloatConstants(rangeExp->start);
        processExpForFloatConstants(rangeExp->end);
        if (rangeExp->step)
            processExpForFloatConstants(rangeExp->step);
    }
    else if (RunExp *runExp = dynamic_cast<RunExp *>(exp))
    {
        if (runExp->block)
            processForFloatConstants(runExp->block);
    }
}
