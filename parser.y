%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

void yyerror(const char *s);
int yylex(void);

ASTNode* root;  // Корневой узел AST
%}

%union {
    int intval;
    int boolval;
    char* id;
    struct ASTNode* ast;
}

%token <id> ID
%token <intval> NUMBER
%token <boolval> BOOL_LITERAL
%token IF ELSE ELIF WHILE FOR PRINT
%token AND OR NOT
%token EQ NE LE GE
%type <ast> program statements statement assignment if_statement elif_statements while_statement for_statement print_statement expression term factor
%start program

%%

program: statements
       { root = $1; $$ = $1; }
       ;

statements: statement statements
          { $$ = createStatementList($1, $2); }
          | statement
          { $$ = createStatementList($1, NULL); }
          ;

statement: assignment
         { $$ = $1; }
         | if_statement
         { $$ = $1; }
         | while_statement
         { $$ = $1; }
         | for_statement
         { $$ = $1; }
         | print_statement
         { $$ = $1; }
         ;

assignment: ID '=' expression ';'
          { $$ = createAssignmentNode($1, $3); }
          ;

if_statement: IF '(' expression ')' '{' statements '}'
            { $$ = createIfNode($3, $6, NULL); }
            | IF '(' expression ')' '{' statements '}' ELSE '{' statements '}'
            { $$ = createIfNode($3, $6, $10); }
            | IF '(' expression ')' '{' statements '}' elif_statements ELSE '{' statements '}'
            { $$ = createIfElseIfNode($3, $6, $8, $11); }
            ;

elif_statements: ELIF '(' expression ')' '{' statements '}' elif_statements
               { $$ = createElifNode($3, $6, $8); }
               | ELIF '(' expression ')' '{' statements '}'
               { $$ = createElifNode($3, $6, NULL); }
               ;

while_statement: WHILE '(' expression ')' '{' statements '}'
               { $$ = createWhileNode($3, $6); }
               ;

for_statement: FOR '(' assignment expression ';' expression ')' '{' statements '}'
             { $$ = createForNode($3, $4, $6, $9); }
             ;

print_statement: PRINT '(' expression ')' ';'
               { $$ = createPrintNode($3); }
               ;

expression: expression '+' term
          { $$ = createExpressionNode('+', $1, $3); }
          | expression '-' term
          { $$ = createExpressionNode('-', $1, $3); }
          | expression EQ term
          { $$ = createExpressionNode(EQ, $1, $3); }
          | expression NE term
          { $$ = createExpressionNode(NE, $1, $3); }
          | expression LE term
          { $$ = createExpressionNode(LE, $1, $3); }
          | expression GE term
          { $$ = createExpressionNode(GE, $1, $3); }
          | expression '<' term
          { $$ = createExpressionNode('<', $1, $3); }
          | expression '>' term
          { $$ = createExpressionNode('>', $1, $3); }
          | term
          { $$ = $1; }
          ;

term: term '*' factor
    { $$ = createExpressionNode('*', $1, $3); }
    | term '/' factor
    { $$ = createExpressionNode('/', $1, $3); }
    | factor
    { $$ = $1; }
    ;

factor: '(' expression ')'
      { $$ = $2; }
      | ID
      { $$ = createVariableNode($1); }
      | NUMBER
      { $$ = createNumberNode($1); }
      | BOOL_LITERAL
      { $$ = createBooleanNode($1); }
      | NOT factor
      { $$ = createUnaryNode(NOT, $2); }
      | factor AND factor
      { $$ = createBinaryNode(AND, $1, $3); }
      | factor OR factor
      { $$ = createBinaryNode(OR, $1, $3); }
      ;
%%

void yyerror(const char *s) {
    extern int yylineno; // текущая строка
    extern char *yytext; // текущий текст
    fprintf(stderr, "Error: %s at line %d, near '%s'\n", s, yylineno, yytext);
}

int main(int argc, char** argv) {
    if (yyparse() == 0) { // Если парсинг прошел успешно
        printf("Parse Successful!\n");
        printAST(root, 0);  // Вывод AST
        int reg = 1;
        generateCode(root, &reg);  // Генерация кода
    } else {
        printf("Parse Failed.\n");
    }
    return 0;
}
