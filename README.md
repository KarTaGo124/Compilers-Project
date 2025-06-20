# Gramática EBNF - Compilador Kotlin

```ebnf
Program = Statement* ;

Statement = VarDeclaration
          | FunDeclaration
          | Assignment
          | PrintStatement
          | IfStatement
          | WhileStatement
          | ForStatement
          | ReturnStatement
          | BreakStatement
          | ContinueStatement
          | Block
          | FunctionCall
          | IncrementDecrement ;

VarDeclaration = ("var" | "val") IDENTIFIER ":" Type ["=" Expression] [";"] ;
FunDeclaration = "fun" IDENTIFIER "(" [Parameters] ")" [":" Type] Block ;
Parameters = Parameter ("," Parameter)* ;
Parameter = IDENTIFIER ":" Type ;

Assignment = IDENTIFIER AssignOperator Expression [";"] ;
AssignOperator = "=" | "+=" | "-=" | "*=" | "/=" | "%=" ;
IncrementDecrement = ("++" | "--") IDENTIFIER [";"]
                   | IDENTIFIER ("++" | "--") [";"] ;

PrintStatement = ("print" | "println") "(" Expression ")" [";"] ;
IfStatement = "if" "(" Expression ")" Statement ["else" Statement] ;
WhileStatement = "while" "(" Expression ")" Statement ;
ForStatement = "for" "(" IDENTIFIER "in" Range ")" Statement ;
ReturnStatement = "return" [Expression] [";"] ;
BreakStatement = "break" [";"] ;
ContinueStatement = "continue" [";"] ;
Block = "{" Statement* "}" ;
FunctionCall = IDENTIFIER "(" [Arguments] ")" [";"] ;
Arguments = Expression ("," Expression)* ;

Range = Expression RangeOperator Expression ["step" Expression] ;
RangeOperator = ".." | "until" | "downTo" ;

Expression = OrExpression ;
OrExpression = AndExpression ("||" AndExpression)* ;
AndExpression = EqualityExpression ("&&" EqualityExpression)* ;
EqualityExpression = RelationalExpression (("==" | "!=") RelationalExpression)* ;
RelationalExpression = AdditiveExpression (("<" | "<=" | ">" | ">=") AdditiveExpression)* ;
AdditiveExpression = MultiplicativeExpression (("+" | "-") MultiplicativeExpression)* ;
MultiplicativeExpression = UnaryExpression (("*" | "/" | "%") UnaryExpression)* ;
UnaryExpression = ("!" | "-" | "++" | "--") UnaryExpression
                | PostfixExpression ;
PostfixExpression = PrimaryExpression ("++" | "--")* ;
PrimaryExpression = IDENTIFIER
                  | NUMBER
                  | DECIMAL
                  | STRING
                  | BOOLEAN
                  | ParenthesizedExpression
                  | FunctionCallExpression ;

ParenthesizedExpression = "(" Expression ")" ;
FunctionCallExpression = IDENTIFIER "(" [Arguments] ")" ;

Type = "Int" | "Float" | "String" | "Boolean" | "Unit" ;

BOOLEAN = "true" | "false" ;
NUMBER = DIGIT+ ;
DECIMAL = DIGIT+ "." DIGIT* "f" | DIGIT+ "f" ;
STRING = '"' CHARACTER* '"' ;
IDENTIFIER = LETTER (LETTER | DIGIT | "_")* ;
DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
LETTER = "a" | "b" | "c" | ... | "z" | "A" | "B" | "C" | ... | "Z" | "_" ;
CHARACTER = (* cualquier carácter excepto '"' *) ;
```
