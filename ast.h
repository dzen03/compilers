#ifndef AST_H
#define AST_H

typedef enum {
  NODE_TYPE_STATEMENT_LIST,
  NODE_TYPE_ASSIGNMENT,
  NODE_TYPE_IF,
  NODE_TYPE_IF_ELSEIF,
  NODE_TYPE_ELIF,
  NODE_TYPE_WHILE,
  NODE_TYPE_FOR,
  NODE_TYPE_PRINT,
  NODE_TYPE_EXPRESSION,
  NODE_TYPE_VARIABLE,
  NODE_TYPE_NUMBER,
  NODE_TYPE_BOOL,
  NODE_TYPE_UNARY,
  NODE_TYPE_BINARY
} NodeType;

typedef struct ASTNode {
  NodeType type;
  char* id;
  int value;
  struct ASTNode* left;
  struct ASTNode* right;
  struct ASTNode* condition;
  struct ASTNode* body;
  struct ASTNode* elseBody;
  struct ASTNode* next;
} ASTNode;

ASTNode* createStatementList(ASTNode* stmt, ASTNode* next);
ASTNode* createAssignmentNode(char* id, ASTNode* expr);
ASTNode* createIfNode(ASTNode* cond, ASTNode* body, ASTNode* elseBody);
ASTNode* createIfElseIfNode(ASTNode* cond, ASTNode* body, ASTNode* elifs, ASTNode* elseBody);
ASTNode* createElifNode(ASTNode* cond, ASTNode* body, ASTNode* nextElif);
ASTNode* createWhileNode(ASTNode* cond, ASTNode* body);
ASTNode* createForNode(ASTNode* init, ASTNode* cond, ASTNode* incr, ASTNode* body);
ASTNode* createPrintNode(ASTNode* expr);
ASTNode* createExpressionNode(int op, ASTNode* left, ASTNode* right);
ASTNode* createVariableNode(char* id);
ASTNode* createNumberNode(int value);
ASTNode* createBooleanNode(int value);
ASTNode* createUnaryNode(int op, ASTNode* operand);
ASTNode* createBinaryNode(int op, ASTNode* left, ASTNode* right);

void printAST(ASTNode* node, int level);
void generateCode(ASTNode* node, int* reg);

#endif // AST_H
