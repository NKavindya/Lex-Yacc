#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

AST *ast_new(NodeKind kind, const char *name, int lineno) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->kind = kind;
    n->name = name ? strdup(name) : NULL;
    n->typeName = NULL;
    n->intValue = 0;
    n->floatValue = 0.0;
    n->lineno = lineno;
    n->child = n->sibling = n->extra = NULL;
    return n;
}

AST *ast_new_int(int val, int lineno) {
    AST *n = ast_new(NODE_INT_LITERAL, NULL, lineno);
    n->intValue = val;
    return n;
}

AST *ast_new_float(double val, int lineno) {
    AST *n = ast_new(NODE_FLOAT_LITERAL, NULL, lineno);
    n->floatValue = val;
    return n;
}

void ast_append_child(AST *parent, AST *child) {
    if (!parent) return;
    if (!parent->child) parent->child = child;
    else ast_append_sibling(&parent->child, child);
}

void ast_append_sibling(AST **list, AST *node) {
    if (!list || !node) return;
    if (!*list) { *list = node; return; }
    AST *p = *list;
    while (p->sibling) p = p->sibling;
    p->sibling = node;
}

static void print_indent(int indent) {
    for (int i=0;i<indent;i++) printf("  ");
}

void ast_print(AST *node, int indent) {
    if (!node) return;
    print_indent(indent);
    switch (node->kind) {
        case NODE_PROGRAM: printf("PROGRAM\n"); break;
        case NODE_CLASS_DECL: printf("CLASS_DECL name=%s (line %d)\n", node->name?node->name:"", node->lineno); break;
        case NODE_ATTRIBUTE: printf("ATTRIBUTE name=%s type=%s (line %d)\n", node->name?node->name:"", node->typeName?node->typeName:"", node->lineno); break;
        case NODE_FUNC_DECL: printf("FUNC_DECL name=%s return=%s (line %d)\n", node->name?node->name:"", node->typeName?node->typeName:"", node->lineno); break;
        case NODE_VAR_DECL: printf("VAR_DECL name=%s type=%s (line %d)\n", node->name?node->name:"", node->typeName?node->typeName:"", node->lineno); break;
        case NODE_PARAM: printf("PARAM name=%s type=%s (line %d)\n", node->name?node->name:"", node->typeName?node->typeName:"", node->lineno); break;
        case NODE_ASSIGN: printf("ASSIGN (line %d)\n", node->lineno); break;
        case NODE_IF: printf("IF (line %d)\n", node->lineno); break;
        case NODE_WHILE: printf("WHILE (line %d)\n", node->lineno); break;
        case NODE_RETURN: printf("RETURN (line %d)\n", node->lineno); break;
        case NODE_READ: printf("READ (line %d)\n", node->lineno); break;
        case NODE_WRITE: printf("WRITE (line %d)\n", node->lineno); break;
        case NODE_FUNCTION_CALL: printf("CALL name=%s (line %d)\n", node->name?node->name:"", node->lineno); break;
        case NODE_ID: printf("ID name=%s (line %d)\n", node->name?node->name:"", node->lineno); break;
        case NODE_INT_LITERAL: printf("INT %d (line %d)\n", node->intValue, node->lineno); break;
        case NODE_FLOAT_LITERAL: printf("FLOAT %g (line %d)\n", node->floatValue, node->lineno); break;
        case NODE_BINARY_OP: printf("BINOP %s (line %d)\n", node->name?node->name:"", node->lineno); break;
        case NODE_UNARY_OP: printf("UNOP %s (line %d)\n", node->name?node->name:"", node->lineno); break;
        default: printf("NODE kind=%d (line %d)\n", node->kind, node->lineno); break;
    }
    // print children
    AST *c = node->child;
    while (c) {
        ast_print(c, indent+1);
        c = c->sibling;
    }
    // extra pointer (for some structures)
    if (node->extra) {
        print_indent(indent+1);
        printf("EXTRA:\n");
        ast_print(node->extra, indent+2);
    }
}

void ast_free(AST *node) {
    if (!node) return;
    AST *c = node->child;
    while (c) {
        AST *next = c->sibling;
        ast_free(c);
        c = next;
    }
    if (node->extra) ast_free(node->extra);
    if (node->name) free(node->name);
    if (node->typeName) free(node->typeName);
    free(node);
}
