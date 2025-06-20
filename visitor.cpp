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
