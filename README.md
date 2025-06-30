# Compilador Kotlin - Implementación de Tipos String y Float

## Introducción

Este proyecto presenta la implementación de un compilador para el lenguaje Kotlin, haciendo enfoque en la implementación de tipos de datos **String** y **Float**. Si bien el compilador ya maneja estructuras de control tradicionales como bucles `for`, `while` y declaraciones condicionales, el núcleo de esta implementación se centra en la manipulación de cadenas de texto (string) y números decimales (float).

## Gramática del Lenguaje

El compilador sigue la siguiente gramática EBNF:

```ebnf
Program = TopLevelDeclaration*

TopLevelDeclaration = VarDeclaration
                    | FunDeclaration

Statement = VarDeclaration
          | Assignment
          | PrintStatement
          | IfStatement
          | WhileStatement
          | DoWhileStatement
          | ForStatement
          | ReturnStatement
          | BreakStatement
          | ContinueStatement
          | Block
          | RunBlock
          | ExpressionStatement

VarDeclaration = ("var" | "val") IDENTIFIER ":" Type ["=" Expression] [";"]
FunDeclaration = "fun" IDENTIFIER "(" [Parameters] ")" [":" Type] Block
Parameters = Parameter ("," Parameter)*
Parameter = IDENTIFIER ":" Type

Assignment = IDENTIFIER AssignOperator Expression [";"]
           | IDENTIFIER ("++" | "--") [";"]
           | ("++" | "--") IDENTIFIER [";"]
AssignOperator = "=" | "+=" | "-=" | "*=" | "/=" | "%="

PrintStatement = ("print" | "println") "(" Expression ")" [";"]
IfStatement = "if" "(" Expression ")" Statement ["else" Statement]
WhileStatement = "while" "(" Expression ")" Statement
DoWhileStatement = "do" Statement "while" "(" Expression ")" [";"]
ForStatement = "for" "(" IDENTIFIER "in" Range ")" Statement
ReturnStatement = "return" [Expression] [";"]
BreakStatement = "break" [";"]
ContinueStatement = "continue" [";"]
Block = "{" Statement* "}"
RunBlock = "run" "{" Statement* "}"
ExpressionStatement = (FunctionCallExpression | IDENTIFIER) [";"]

Range = Expression RangeOperator Expression ["step" Expression]
RangeOperator = ".." | "until" | "downTo"

Expression = OrExpression
OrExpression = AndExpression ("||" AndExpression)*
AndExpression = EqualityExpression ("&&" EqualityExpression)*
EqualityExpression = RelationalExpression (("==" | "!=") RelationalExpression)*
RelationalExpression = AdditiveExpression (("<" | "<=" | ">" | ">=") AdditiveExpression)*
AdditiveExpression = MultiplicativeExpression (("+" | "-") MultiplicativeExpression)*
MultiplicativeExpression = UnaryExpression (("*" | "/" | "%") UnaryExpression)*
UnaryExpression = ("!" | "-" | "+" | "++" | "--") UnaryExpression
                | PostfixExpression
PostfixExpression = PrimaryExpression ("++" | "--")*
PrimaryExpression = IDENTIFIER
                  | NUMBER
                  | DECIMAL
                  | STRING
                  | BOOLEAN
                  | ParenthesizedExpression
                  | FunctionCallExpression
                  | RunExpression

ParenthesizedExpression = "(" Expression ")"
FunctionCallExpression = IDENTIFIER "(" [Arguments] ")"
RunExpression = "run" "{" Statement* "}"
Arguments = Expression ("," Expression)*

Type = "Int" | "Float" | "String" | "Boolean" | "Unit"

BOOLEAN = "true" | "false"
NUMBER = DIGIT+
DECIMAL = DIGIT+ "." DIGIT* "f" | DIGIT+ "f"
STRING = '"' CHARACTER* '"'
IDENTIFIER = LETTER (LETTER | DIGIT | "_")*

COMMENT = "//" CHARACTER* NEWLINE | "/*" CHARACTER* "*/"

DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
LETTER = "a" | "b" | "c" | ... | "z" | "A" | "B" | "C" | ... | "Z" | "_"
CHARACTER = (* cualquier carácter válido *)
NEWLINE = "\n" | "\r\n"
```

```

```
## Implementación de Tipos String y Float

### 1. Clases Elaboradas

La incorporación de tipos String y Float al compilador requirió el desarrollo de clases específicas y la extensión de las estructuras existentes para manejar estos nuevos tipos de datos de manera eficiente y coherente con el diseño del sistema.

#### Tipo String

El manejo de cadenas de texto se implementó a través de la clase **StringExp** que representa los literales de cadena en el árbol sintáctico abstracto (AST).
Esta clase hereda de la clase base ``Exp`` y encapsula el valor de la cadena como una propiedad de tipo ``string``:
```cpp
class StringExp : public Exp
{
public:
    string value;
    StringExp(const string &v);
    int accept(Visitor *visitor);
    ~StringExp();
};
```

El constructor de `StringExp` recibe una referencia constante a un string, lo que mejora la eficiencia en la copia y evita modificaciones accidentales del valor durante la construcción. La función `accept` implementa el patrón Visitor, permitiendo que diferentes tipos de visitadores procesen esta expresión de manera polimórfica.

A nivel léxico, el scanner maneja los literales de cadena mediante el procesamiento de los caracteres entre comillas dobles. Este fragmento de código del scanner muestra cómo se reconocen las cadenas:

```cpp
else if (c == '"')
{
    current++;
    size_t start = current;

    while (current < input.length() && input[current] != '"')
    {
        if (input[current] == '\\' && current + 1 < input.length())
        {
            current += 2;  // Manejo básico de escape sequences
        }
        else
        {
            current++;
        }
    }

    if (current >= input.length())
    {
        token = new Token(Token::ERR, input, first, current - first);
    }
    else
    {
        token = new Token(Token::STRING, input, start, current - start);
        current++;
    }
}
```

En el manejo de errores se ha incluido la detección de cadenas no terminadas retornando un token de error cuando no se encuentra la comilla de cierre.

#### Tipo Float

La implementación de números float se centra en la clase **DecimalExp**, que maneja tanto la representación interna como las operaciones aritméticas de precisión float:

```cpp
class DecimalExp : public Exp
{
public:
    float value;
    std::string original_text;
    DecimalExp(float v);
    int accept(Visitor *visitor);
    ~DecimalExp();
};
```

La clase utiliza el tipo `float` de C++ para almacenar valores de precisión simple. El constructor acepta un valor float y lo almacena directamente, manteniendo la precisión original del literal analizado.

El scanner implementa un análisis que reconoce múltiples formatos de números flotantes en la función `nextToken()`:

```cpp
if (isdigit(c))
{
    current++;
    bool is_float = false;
    bool is_int = false;
    bool has_f = false;

    while (current < input.length() && isdigit(input[current]))
        current++;

    if (current < input.length() && input[current] == '.')
    {
        if (current + 1 < input.length() && input[current + 1] == '.')
        {
            is_int = true;  // Detecta operador de rango ".."
        }
        else
        {
            is_float = true;
            current++;
            while (current < input.length() && isdigit(input[current]))
                current++;
        }
    }
    else
    {
        is_int = true;
    }

    if (is_float && current < input.length() && input[current] == 'f')
    {
        has_f = true;
        current++;
        token = new Token(Token::DECIMAL, input, first, current - first);
        token->has_f = has_f;
    }
    else if (is_float)
    {
        token = new Token(Token::ERR, input, first, current - first);
    }
    else if (is_int && current < input.length() && input[current] == 'f')
    {
        has_f = true;
        current++;
        token = new Token(Token::DECIMAL, input, first, current - first);
        token->has_f = has_f;
    }
    else
    {
        token = new Token(Token::NUM, input, first, current - first);
    }
}
```

Con esta implementación se podrán reconocer formatos como `3.14f`, `2.5f`, y `42f`. Un aspecto importante a resaltar es que los literales flotantes sin el sufijo 'f' se marcan como errores, forzando la especificación explícita del tipo flotante. La lógica también maneja cuidadosamente la distinción entre el punto decimal y el operador de rango "..".

### 2. Operaciones Implementadas

La implementación de operaciones para String y Float se maneja principalmente a través del sistema de visitors, especialmente `EvalVisitor` y `PrintVisitor`, que procesan las expresiones según el tipo.

#### Operaciones con String

Las operaciones con strings se implementaron a través del reconocimiento del token `STRING_TYPE` en el scanner para las declaraciones de tipo:

```cpp
else if (word == "String")
{
    token = new Token(Token::STRING_TYPE, word, 0, word.length());
}
```

Esta implementación permite declaraciones como `var mensaje: String = "Hola mundo"`. El manejo de las operaciones de concatenación, comparación y asignación se realiza en los visitors, que procesan los tokens `Token::STRING` generados por los literales de cadena.

Las funciones de impresión se manejan los strings a través de los siguientes tokens:

```cpp
else if (word == "print")
{
    token = new Token(Token::PRINT, word, 0, word.length());
}
else if (word == "println")
{
    token = new Token(Token::PRINTLN, word, 0, word.length());
}
```

#### Operaciones con Float

Las operaciones aritméticas con números flotantes se reconocen a través del token `DECIMAL` y se procesan según la presencia de `has_f`. El scanner maneja bien la diferenciación entre literales enteros y flotantes:

```cpp
token = new Token(Token::DECIMAL, input, first, current - first);
token->has_f = has_f;
```

El conjunto de tokens también reconoce el tipo Float para declaraciones:

```cpp
else if (word == "Float")
{
    token = new Token(Token::FLOAT, word, 0, word.length());
}
```

Las operaciones compuestas se manejan a través de tokens como `PLUS_ASSIGN`, `MINUS_ASSIGN`, etc.:

```cpp
case '+':
    if (current + 1 < input.length() && input[current + 1] == '=')
    {
        token = new Token(Token::PLUS_ASSIGN, "+=", 0, 2);
        current++;
    }
    // ... otras variaciones
```

### 3. Estructura de Visitors y Diseño
#### PrintVisitor

El `PrintVisitor` se encarga de generar la representación textual del AST. Para este nuevo tipo, la clase incluye métodos `visit` que manejan `StringExp` y `DecimalExp`:

```cpp
class PrintVisitor : public Visitor
{
private:
    int indent = 0;
    void imprimirIndentacion();

public:
    void imprimir(Program *program);
    int visit(StringExp *exp) override;
    int visit(DecimalExp *exp) override;
    // ... otros métodos visit
};
```

Además se implementó un sistema de indentación para mejorar la legibilidad de la salida y asi poder crear una jerarquía visual. La función `imprimirIndentacion()` genera espacios apropiados para cada nivel de anidamiento.

#### EvalVisitor

El `EvalVisitor` ejecuta directamente el código. Su diseño incluye variables de estado para manejar diferentes tipos de datos:

```cpp
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
    int visit(StringExp *exp) override;
    int visit(DecimalExp *exp) override;
    // ... otros métodos visit
};
```

Nuestro compilador también utiliza variables de estado (`lastType`, `lastInt`, `lastFloat`, `lastString`) para mantener el resultado de la última evaluación. Este diseño permite que las operaciones accedan al valor y tipo del resultado sin necesidad de estructuras de datos complejas.

La implementación de los métodos visit para Float y String en el EvalVisitor son las siguientes:

```cpp
int EvalVisitor::visit(DecimalExp *exp)
{
    lastType = 2;         
    lastFloat = exp->value;
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
```

Las operaciones binarias manejan la conversión automática de tipos y la concatenación de strings. Para el operador PLUS_OP, si cualquiera de los operandos es string, se realiza concatenación automática:

```cpp
case PLUS_OP:
    if (leftType == 5 || rightType == 5) {
        lastType = 5;
        string left_str, right_str;
        
        if (leftType == 5) left_str = leftString;
        else if (leftType == 3) left_str = leftInt ? "true" : "false";
        else if (leftType == 2) left_str = formatFloat(leftFloat);
        else left_str = to_string(leftInt);
        
        if (rightType == 5) right_str = rightString;
        else if (rightType == 3) right_str = rightInt ? "true" : "false";
        else if (rightType == 2) right_str = formatFloat(rightFloat);
        else right_str = to_string(rightInt);
        
        lastString = left_str + right_str;
    }
    else if (leftType == 2 || rightType == 2) {
        lastType = 2;
        lastFloat = (leftType == 2 ? leftFloat : leftInt) + 
                   (rightType == 2 ? rightFloat : rightInt);
    }
    else if (leftType == 1 && rightType == 1) {
        lastType = 1;
        lastInt = leftInt + rightInt;
    }
    break;
```

#### GenCodeVisitor - Generación de Código Assembly

El `GenCodeVisitor` en nuestro visitor.cpp es el responsable de convertir el AST a código assembly. Para los tipos Float y String, implementa estrategias específicas de generación de código que aprovechan las características de la arquitectura objetivo.

##### Generación de Código para Float

La generación de código para números flotantes utiliza registros XMM y la sección `.rodata` para almacenar constantes:

```cpp
int GenCodeVisitor::visit(DecimalExp *exp)
{
    string label = getFloatConstantLabel(exp->value);
    out << " movsd " << label << "(%rip), %xmm0\n";
    return 2;
}
```

El sistema de gestión de constantes flotantes evita la duplicación mediante un mapa de constantes:

```cpp
string GenCodeVisitor::getFloatConstantLabel(double value)
{
    for (auto it = floatConstants.begin(); it != floatConstants.end(); ++it) {
        if (it->first == value) {
            return it->second;
        }
    }
    string label = ".float_" + to_string(floatConstants.size());
    floatConstants[value] = label;
    return label;
}
```

Las operaciones aritméticas con floats utilizamos instrucciones SSE2:

```cpp
case PLUS_OP:
    out << " addsd %xmm1, %xmm0\n";
    break;
case MUL_OP:
    out << " mulsd %xmm1, %xmm0\n";
    break;
case DIV_OP:
    out << " divsd %xmm1, %xmm0\n";
    break;
```

Para operaciones mixtas (int + float), se implementa una conversión automática:

```cpp
if (leftType == 2 || rightType == 2) {
    if (rightType == 2) {
        out << " movsd %xmm0, %xmm1\n";
    } else {
        out << " cvtsi2sd %rax, %xmm1\n";
    }
    
    if (leftType == 2) {
        out << " movsd (%rsp), %xmm0\n";
    } else {
        out << " popq %rax\n";
        out << " cvtsi2sd %rax, %xmm0\n";
    }
}
```

##### Generación de Código para String

Los strings se almacenan en la sección `.rodata` con etiquetas únicas:

```cpp
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
```

La concatenación de strings utiliza las funciones de biblioteca estándar `strcpy` y `strcat`:

```cpp
if ((leftType == 5 || rightType == 5) && exp->op == PLUS_OP) {
    out << " movq %rax, %r8\n";         
    out << " popq %r9\n";              
    
    out << " leaq string_buffer(%rip), %rax\n";
    
    // Copia el primer string
    out << " movq %rax, %rdi\n";        
    out << " movq %r9, %rsi\n";         
    out << " call strcpy@PLT\n";
    
    // Concatena el segundo string
    out << " leaq string_buffer(%rip), %rdi\n";
    out << " movq %r8, %rsi\n";        
    out << " call strcat@PLT\n";
    
    out << " leaq string_buffer(%rip), %rax\n"; 
    return 5;
}
```

El generador de código también maneja comparaciones de floats utilizando la instrucción `comisd`:

```cpp
case EQ_OP:
    out << " comisd %xmm1, %xmm0\n";    
    out << " movl $0, %eax\n";
    out << " sete %al\n";
    out << " movzbq %al, %rax\n";
    return 3;
```

### 4. Estructura de Datos y Memoria

El compilador utiliza buffers estáticos para el manejo de strings y mapas para la gestión de constantes flotantes:

```cpp
string_buffer: .space 512
string_buffer2: .space 512
stringBufferCounter: .long 0

.align 8
.float_0: .double 3.14159
.float_1: .double 2.71828
```

## Conclusión

La implementación exitosa de tipos String y Float en este compilador demuestra la flexibilidad del diseño empleado. El uso de Visitor.cpp y visitor.h permite una gestión eficiente de los diferentes tipos de datos, manteniendo la claridad del código en lenguaje Kotlin
