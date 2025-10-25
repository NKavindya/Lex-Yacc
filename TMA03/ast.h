#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef enum {
    NODE_PROGRAM,
    NODE_CLASS_DECL,
    NODE_CLASS_INHERIT_LIST,
    NODE_CLASS_BODY,
    NODE_ATTRIBUTE,
    NODE_FUNC_DECL,
    NODE_VAR_DECL,
    NODE_PARAM_LIST,
    NODE_PARAM,
    NODE_FUNC_BODY,
    NODE_STATEMENT_LIST,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_RETURN,
    NODE_READ,
    NODE_WRITE,
    NODE_FUNCTION_CALL,
    NODE_ID,
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_TYPE,
    NODE_VISIBILITY,
    NODE_EMPTY
} NodeKind;

typedef struct AST {
    NodeKind kind;
    char *name;               // identifier or operator
    char *typeName;           // for type nodes or annotated type
    int intValue;             // for integer literals
    double floatValue;        // for float literals
    int lineno;
    struct AST *child;        // first child
    struct AST *sibling;      // next sibling (for lists)
    struct AST *extra;        // auxiliary (e.g., rhs for assign)
} AST;

AST *ast_new(NodeKind kind, const char *name, int lineno);
AST *ast_new_int(int val, int lineno);
AST *ast_new_float(double val, int lineno);
void ast_append_child(AST *parent, AST *child);
void ast_append_sibling(AST **list, AST *node);
void ast_print(AST *node, int indent);
void ast_free(AST *node);

#endif
