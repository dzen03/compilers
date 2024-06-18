1. Grammar:
    ```
    program -> statements
    
    statements -> statement statements
    | statement
    
    statement -> assignment
    | if_statement
    | while_statement
    | for_statement
    | print_statement
    
    assignment -> ID '=' expression ';'
    
    if_statement -> IF '(' expression ')' '{' statements '}'
    | IF '(' expression ')' '{' statements '}' ELSE '{' statements '}'
    | IF '(' expression ')' '{' statements '}' elif_statements ELSE '{' statements '}'
    
    elif_statements -> ELIF '(' expression ')' '{' statements '}' elif_statements
    | ELIF '(' expression ')' '{' statements '}'
    
    while_statement -> WHILE '(' expression ')' '{' statements '}'
    
    for_statement -> FOR '(' assignment expression ';' expression ')' '{' statements '}'
    
    print_statement -> PRINT '(' expression ')' ';'
    
    expression -> expression '+' term
    | expression '-' term
    | expression EQ term
    | expression NE term
    | expression LE term
    | expression GE term
    | expression '<' term
    | expression '>' term
    | term
    
    term -> term '*' factor
    | term '/' factor
    | factor
    
    factor -> '(' expression ')'
    | ID
    | NUMBER
    | BOOL_LITERAL
    | NOT factor
    | factor AND factor
    | factor OR factor
    ```

2. Lexer:
    ```
    ID          -> [a-zA-Z_][a-zA-Z0-9_]*
    NUMBER      -> [0-9]+
    BOOL_LITERAL -> "true" | "false"
    
    IF          -> "if"
    ELSE        -> "else"
    ELIF        -> "elif"
    WHILE       -> "while"
    FOR         -> "for"
    PRINT       -> "print"
    
    AND         -> "&&"
    OR          -> "||"
    NOT         -> "!"
    EQ          -> "=="
    NE          -> "!="
    LE          -> "<="
    GE          -> ">="
    '<'         -> "<"
    '>'         -> ">"
    '+'         -> "+"
    '-'         -> "-"
    '*'         -> "*"
    '/'         -> "/"
    '='         -> "="
    '('         -> "("
    ')'         -> ")"
    '{'         -> "{"
    '}'         -> "}"
    ';'         -> ";"
    
    COMMENT     -> "/\\*([^*]|\\*+[^*/])*\\*+/"
    WHITESPACE  -> [ \t\n]+
    ```

3. Examples:
   1. Correct simple:
       ```
       a = 10;
       print(a);
       ```
       ```
       Parse Successful!
       Statement List
        Assignment: a
          Number: 10
        Statement List
          Print
            Variable: a
       addi x1, x0, 10
       add x1, x1, x0
       add x2, x1, x0
       ewrite x2
       ```
   2. Loop:
       ```
       for (i = 1; i < 10; i + 1)
       {
         print(i);
       }
       ```
       ```
       Parse Successful!
       Statement List
         For
           Assignment: i
             Number: 1
           Expression < 
             Variable: i
             Number: 10
           Expression + 
             Variable: i
             Number: 1
           Statement List
             Print
               Variable: i
       addi x1, x0, 1
       add x1, x1, x0
       L2:
       add x4, x1, x0
       addi x5, x0, 10
       slt x4, x4, x5
       beq x4, x0, L3
       add x6, x1, x0
       ewrite x6
       add x7, x1, x0
       addi x8, x0, 1
       add x1, x7, x8
       jal x0, L2
       L3:
       ```
   3. Error:
      ```
      a = ;
      ```
      ```
      Parse Failed.
      Error: syntax error at line 1, near ';'
      ```
   4. Comments:
      ```
      /*test
      test
      */
    
      print(10);
      ```
      ```
      Parse Successful!
      Statement List
        Print
          Number: 10
      addi x1, x0, 10
      ewrite x1
      ```
   5. if - elif - else:
       ```
       a = 10;
       if (a == 5)
       {
         print(true);
       }
       elif (a == 10)
       {
         print(200);
       }
       else
       {
         a = 0;
       }
       ```
       ```
       Parse Successful!
       Statement List
        Assignment: a
          Number: 10
        Statement List
          If-ElseIf
            Expression == 
              Variable: a
              Number: 5
            Statement List
              Print
                Boolean: true
            Elif
              Expression == 
                Variable: a
                Number: 10
              Statement List
                Print
                  Number: 200
       Else
            Statement List
              Assignment: a
                Number: 0
       addi x1, x0, 10
       add x1, x1, x0
       add x3, x1, x0
       addi x4, x0, 5
       seq x3, x3, x4
       beq x3, x0, L5
       addi x6, x0, 1
       ewrite x6
       jal x0, L2
       L5:
       add x8, x1, x0
       addi x9, x0, 10
       seq x8, x8, x9
       beq x7, x0, L7
       addi x10, x0, 200
       ewrite x10
       jal x0, L11
       L7:
       L2:
       addi x11, x0, 0
       add x1, x11, x0
       L11:
       ```