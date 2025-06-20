# Gramática EBNF - Compilador Kotlin

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

ParenthesizedExpression = "(" Expression ")"
FunctionCallExpression = IDENTIFIER "(" [Arguments] ")"
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
