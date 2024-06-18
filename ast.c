#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_VARS 31

typedef struct {
  char* name;
  int reg;
} VarRegMap;

VarRegMap varRegMap[MAX_VARS];
int varCount = 0;

int getVarReg(char* name) {
  for (int i = 0; i < varCount; i++) {
    if (strcmp(varRegMap[i].name, name) == 0) {
      return varRegMap[i].reg;
    }
  }
  if (varCount < MAX_VARS) {
    varRegMap[varCount].name = strdup(name);
    varRegMap[varCount].reg = varCount + 1; // registers x1 - x{MAX_VARS}
    return varRegMap[varCount++].reg;
  }
  fprintf(stderr, "Error: too many variables\n");
  exit(1);
}

ASTNode* createStatementList(ASTNode* stmt, ASTNode* next) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_STATEMENT_LIST;
  node->left = stmt;
  node->right = next;
  return node;
}

ASTNode* createAssignmentNode(char* id, ASTNode* expr) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_ASSIGNMENT;
  node->id = strdup(id);
  node->left = expr;
  return node;
}

ASTNode* createIfNode(ASTNode* cond, ASTNode* body, ASTNode* elseBody) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_IF;
  node->condition = cond;
  node->body = body;
  node->elseBody = elseBody;
  return node;
}

ASTNode* createIfElseIfNode(ASTNode* cond, ASTNode* body, ASTNode* elifs, ASTNode* elseBody) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_IF_ELSEIF;
  node->condition = cond;
  node->body = body;
  node->elseBody = elseBody;
  node->right = elifs;
  return node;
}

ASTNode* createElifNode(ASTNode* cond, ASTNode* body, ASTNode* nextElif) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_ELIF;
  node->condition = cond;
  node->body = body;
  node->right = nextElif;
  return node;
}

ASTNode* createWhileNode(ASTNode* cond, ASTNode* body) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_WHILE;
  node->condition = cond;
  node->body = body;
  return node;
}

ASTNode* createForNode(ASTNode* init, ASTNode* cond, ASTNode* incr, ASTNode* body) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_FOR;
  node->left = init;
  node->condition = cond;
  node->right = incr;
  node->body = body;
  return node;
}

ASTNode* createPrintNode(ASTNode* expr) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_PRINT;
  node->left = expr;
  return node;
}

ASTNode* createExpressionNode(int op, ASTNode* left, ASTNode* right) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_EXPRESSION;
  node->value = op;
  node->left = left;
  node->right = right;
  return node;
}

ASTNode* createVariableNode(char* id) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_VARIABLE;
  node->id = strdup(id);
  return node;
}

ASTNode* createNumberNode(int value) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_NUMBER;
  node->value = value;
  return node;
}

ASTNode* createBooleanNode(int value) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_BOOL;
  node->value = value;
  return node;
}

ASTNode* createUnaryNode(int op, ASTNode* operand) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_UNARY;
  node->value = op;
  node->left = operand;
  return node;
}

ASTNode* createBinaryNode(int op, ASTNode* left, ASTNode* right) {
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  node->type = NODE_TYPE_BINARY;
  node->value = op;
  node->left = left;
  node->right = right;
  return node;
}

void printAST(ASTNode* node, int level) {
  if (!node) return;
  for (int i = 0; i < level; ++i) {
    printf("  ");
  }
  switch (node->type) {
    case NODE_TYPE_STATEMENT_LIST:
      printf("Statement List\n");
      printAST(node->left, level + 1);
      printAST(node->right, level + 1);
      break;
    case NODE_TYPE_ASSIGNMENT:
      printf("Assignment: %s\n", node->id);
      printAST(node->left, level + 1);
      break;
    case NODE_TYPE_IF:
      printf("If\n");
      printAST(node->condition, level + 1);
      printAST(node->body, level + 1);
      if (node->elseBody) {
        printf("Else\n");
        printAST(node->elseBody, level + 1);
      }
      break;
    case NODE_TYPE_IF_ELSEIF:
      printf("If-ElseIf\n");
      printAST(node->condition, level + 1);
      printAST(node->body, level + 1);
      printAST(node->right, level + 1); // elif branches
      if (node->elseBody) {
        printf("Else\n");
        printAST(node->elseBody, level + 1);
      }
      break;
    case NODE_TYPE_ELIF:
      printf("Elif\n");
      printAST(node->condition, level + 1);
      printAST(node->body, level + 1);
      printAST(node->right, level + 1); // next elif branch
      break;
    case NODE_TYPE_WHILE:
      printf("While\n");
      printAST(node->condition, level + 1);
      printAST(node->body, level + 1);
      break;
    case NODE_TYPE_FOR:
      printf("For\n");
      printAST(node->left, level + 1); // init
      printAST(node->condition, level + 1);
      printAST(node->right, level + 1); // increment (or whatever)
      printAST(node->body, level + 1);
      break;
    case NODE_TYPE_PRINT:
      printf("Print\n");
      printAST(node->left, level + 1);
      break;
    case NODE_TYPE_EXPRESSION:
      printf("Expression");
      switch (node->value) {
        case '+': printf(" + "); break;
        case '-': printf(" - "); break;
        case '*': printf(" * "); break;
        case '/': printf(" / "); break;
        case EQ: printf(" == "); break;
        case NE: printf(" != "); break;
        case LE: printf(" <= "); break;
        case GE: printf(" >= "); break;
        case '<': printf(" < "); break;
        case '>': printf(" > "); break;
        case AND: printf(" && "); break;
        case OR: printf(" || "); break;
        case NOT: printf("!"); break;
        default: break;
      }
      printf("\n");
      printAST(node->left, level + 1);
      printAST(node->right, level + 1);
      break;
    case NODE_TYPE_VARIABLE:
      printf("Variable: %s\n", node->id);
      break;
    case NODE_TYPE_NUMBER:
      printf("Number: %d\n", node->value);
      break;
    case NODE_TYPE_BOOL:
      printf("Boolean: %s\n", (node->value ? "true" : "false"));
      break;
    case NODE_TYPE_UNARY:
      printf("Unary Operation: %d\n", node->value);
      printAST(node->left, level + 1);
      break;
    case NODE_TYPE_BINARY:
      printf("Binary Operation: %d\n", node->value);
      printAST(node->left, level + 1);
      printAST(node->right, level + 1);
      break;
    default:
      printf("Unknown node type\n");
      break;
  }
}

void generateCode(ASTNode* node, int* reg) {
  if (!node) return;
  switch (node->type) {
    case NODE_TYPE_STATEMENT_LIST:
      generateCode(node->left, reg);
      generateCode(node->right, reg);
      break;
    case NODE_TYPE_ASSIGNMENT: {
      int expr_reg = *reg;
      generateCode(node->left, reg);
      int var_reg = getVarReg(node->id);
      printf("add x%d, x%d, x0\n", var_reg, expr_reg); // Перемещение значения в регистр назначения
      break;
    }
    case NODE_TYPE_PRINT:
      generateCode(node->left, reg);
      printf("ewrite x%d\n", *reg - 1);
      break;
    case NODE_TYPE_IF: {
      int cond_reg = *reg;
      generateCode(node->condition, reg);
      int label_else = (*reg)++;
      int label_end = (*reg)++;
      printf("beq x%d, x0, L%d\n", cond_reg, label_else);
      generateCode(node->body, reg);
      printf("jal x0, L%d\n", label_end);
      printf("L%d:\n", label_else);
      if (node->elseBody) {
        generateCode(node->elseBody, reg);
      }
      printf("L%d:\n", label_end);
      break;
    }
    case NODE_TYPE_IF_ELSEIF: {
      int cond_reg = *reg;
      int label_end = (*reg)++;
      generateCode(node->condition, reg);
      int label_elif = (*reg)++;
      printf("beq x%d, x0, L%d\n", cond_reg, label_elif);
      generateCode(node->body, reg);
      printf("jal x0, L%d\n", label_end);
      printf("L%d:\n", label_elif);
      generateCode(node->right, reg);
      if (node->elseBody) {
        printf("L%d:\n", label_end);
        generateCode(node->elseBody, reg);
      }
      printf("L%d:\n", label_end);
      break;
    }
    case NODE_TYPE_ELIF: {
      int cond_reg = *reg;
      int label_next_elif = (*reg)++;
      generateCode(node->condition, reg);
      printf("beq x%d, x0, L%d\n", cond_reg, label_next_elif);
      generateCode(node->body, reg);
      printf("jal x0, L%d\n", *reg);
      printf("L%d:\n", label_next_elif);
      if (node->right) {
        generateCode(node->right, reg);
      }
      break;
    }
    case NODE_TYPE_WHILE: {
      int label_start = (*reg)++;
      int label_end = (*reg)++;
      printf("L%d:\n", label_start);
      int cond_reg = *reg;
      generateCode(node->condition, reg);
      printf("beq x%d, x0, L%d\n", cond_reg, label_end);
      generateCode(node->body, reg);
      printf("jal x0, L%d\n", label_start);
      printf("L%d:\n", label_end);
      break;
    }
    case NODE_TYPE_FOR: {
      generateCode(node->left, reg); // init
      int label_start = (*reg)++;
      int label_end = (*reg)++;
      printf("L%d:\n", label_start);
      int cond_reg = *reg;
      generateCode(node->condition, reg);
      printf("beq x%d, x0, L%d\n", cond_reg, label_end);
      generateCode(node->body, reg);
      generateCode(node->right, reg); // increment (or whatever)
      printf("jal x0, L%d\n", label_start);
      printf("L%d:\n", label_end);
      break;
    }
    case NODE_TYPE_EXPRESSION: {
      int left_reg = *reg;
      generateCode(node->left, reg);
      int right_reg = *reg;
      generateCode(node->right, reg);
      switch (node->value) {
        case '+':
          printf("add x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case '-':
          printf("sub x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case '*':
          printf("mul x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case '/':
          printf("div x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case EQ:
          printf("seq x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case NE:
          printf("sne x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case LE:
          printf("sge x%d, x%d, x%d\n", left_reg, right_reg, left_reg);
          break;
        case GE:
          printf("sge x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case '<':
          printf("slt x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case '>':
          printf("slt x%d, x%d, x%d\n", left_reg, right_reg, left_reg);
          break;
        case AND:
          printf("and x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        case OR:
          printf("or x%d, x%d, x%d\n", left_reg, left_reg, right_reg);
          break;
        default:
          break;
      }
      break;
    }
    case NODE_TYPE_VARIABLE: {
      int var_reg = getVarReg(node->id);
      printf("add x%d, x%d, x0\n", *reg, var_reg);
      *reg += 1;
      break;
    }
    case NODE_TYPE_NUMBER:
      printf("addi x%d, x0, %d\n", *reg, node->value);
      *reg += 1;
      break;
    case NODE_TYPE_BOOL:
      printf("addi x%d, x0, %d\n", *reg, node->value);
      *reg += 1;
      break;
    case NODE_TYPE_UNARY:
      generateCode(node->left, reg);
      switch (node->value) {
        case NOT:
          printf("xori x%d, x%d, 1\n", *reg - 1, *reg - 1);
          break;
        default:
          break;
      }
      break;
    case NODE_TYPE_BINARY:
      generateCode(node->left, reg);
      generateCode(node->right, reg);
      switch (node->value) {
        case AND:
          printf("and x%d, x%d, x%d\n", *reg - 2, *reg - 2, *reg - 1);
          break;
        case OR:
          printf("or x%d, x%d, x%d\n", *reg - 2, *reg - 2, *reg - 1);
          break;
        default:
          break;
      }
      *reg -= 1; // result still used
      break;
    default:
      perror("found undefined node type");
      return;
  }
}

