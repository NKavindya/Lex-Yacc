/* 
 * ast.c - abstract syntax tree implementation
 * 
 * provides functions for creating, manipulating, and printing AST nodes.
 * the AST is built during parsing and used by semantic analysis and code generation.
 */

#include "ast.h"      // AST data structure definitions
#include <stdlib.h>   // malloc, free
#include <string.h>   // strdup, strcmp
#include <stdio.h>    // printf, fprintf

/**
 * ast_new - create a new AST node with specified type and name
 * 
 * allocates memory for a new AST node and initializes all fields.
 * the name parameter is duplicated if provided (caller retains ownership of original).
 * 
 * @param kind type of node to create (from NodeKind enum)
 * @param name identifier or operator name (NULL if not applicable)
 * @param lineno source line number where this construct appears
 * @return pointer to newly allocated and initialized AST node
 */
AST *ast_new(NodeKind kind, const char *name, int lineno) {
    // allocate memory for new AST node
    AST *n = (AST*)malloc(sizeof(AST));
    
    // initialize node fields
    n->kind = kind;                                    // set node type
    n->name = name ? strdup(name) : NULL;             // duplicate name string if provided
    n->typeName = NULL;                                // type name not set initially
    n->intValue = 0;                                   // integer value defaults to 0
    n->floatValue = 0.0;                               // float value defaults to 0.0
    n->lineno = lineno;                                // store source line number
    n->child = n->sibling = n->extra = NULL;          // initialize all pointers to NULL
    
    return n;  // return newly created node
}

/**
 * ast_new_int - create an integer literal node
 * 
 * convenience function to create an AST node for an integer constant.
 * 
 * @param val integer value to store in the node
 * @param lineno source line number
 * @return pointer to NODE_INT_LITERAL node with intValue set
 */
AST *ast_new_int(int val, int lineno) {
    AST *n = ast_new(NODE_INT_LITERAL, NULL, lineno);  // create integer literal node
    n->intValue = val;                                  // store the integer value
    return n;
}

/**
 * ast_new_float - create a float literal node
 * 
 * convenience function to create an AST node for a floating-point constant.
 * 
 * @param val float value to store in the node
 * @param lineno Source line number
 * @return Pointer to NODE_FLOAT_LITERAL node with floatValue set
 */
AST *ast_new_float(double val, int lineno) {
    AST *n = ast_new(NODE_FLOAT_LITERAL, NULL, lineno);  // create float literal node
    n->floatValue = val;                                 // store the float value
    return n;
}

/**
 * ast_new_string - create a string literal node
 * 
 * convenience function to create an AST node for a string constant.
 * 
 * @param val string value to store in the node (will be duplicated)
 * @param lineno source line number
 * @return Pointer to NODE_STRING_LITERAL node with name set to duplicated string
 */
AST *ast_new_string(const char *val, int lineno) {
    AST *n = ast_new(NODE_STRING_LITERAL, strdup(val), lineno);  // create string literal node with duplicated string
    return n;
}

/**
 * ast_append_child - add a child node to a parent node
 * 
 * adds a child node to the parent. if the parent already has children,
 * the new child is appended to the sibling chain of existing children.
 * 
 * @param parent parent node (must not be NULL)
 * @param child child node to add
 */
void ast_append_child(AST *parent, AST *child) {
    if (!parent) return;  // safety check: parent must exist
    
    // IF parent has no children yet, make this the first child
    if (!parent->child) {
        parent->child = child;
    } else {
        // otherwise, append to the sibling chain of existing children
        ast_append_sibling(&parent->child, child);
    }
}

/**
 * ast_append_sibling - add a node to the end of a sibling list
 * 
 * appends a node to the end of a linked list of sibling nodes.
 * used for building lists (statements, parameters, etc.).
 * 
 * @param list pointer to head of sibling list (may be modified if list is empty)
 * @param node node to append to the list
 */
void ast_append_sibling(AST **list, AST *node) {
    if (!list || !node) return;  // safety checks
    
    // IF list is EMPTY, make this node the head
    if (!*list) { 
        *list = node; 
        return; 
    }
    
    // traverse to end of sibling chain
    AST *p = *list;
    while (p->sibling) p = p->sibling;
    
    // append new node at the end
    p->sibling = node;
}

/**
 * print_indent - helper function to print indentation spaces
 * 
 * prints a number of spaces for tree visualization.
 * each level of indentation is 2 spaces.
 * 
 * @param indent Number of indentation levels (each level = 2 spaces)
 */
static void print_indent(int indent) {
    for (int i=0; i<indent; i++) printf("  ");  // print 2 spaces per indentation level
}

/**
 * ast_print - recursively print AST structure to stdout
 * 
 * prints the AST in a tree format with indentation showing hierarchy.
 * for each node, prints:
 * - node type and relevant information (name, type, value)
 * - source line number
 * - children nodes (indented)
 * - extra pointer content (if present)
 * 
 * @param node root node of subtree to print
 * @param indent current indentation level (0 = root, increases for children)
 */
void ast_print(AST *node, int indent) {
    if (!node) return;  // safety check: nothing to print IF node is NULL
    
    // print indentation and node information
    print_indent(indent);
    
    // print node-specific information based on node TYPE
    switch (node->kind) {
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            break;
        case NODE_CLASS_DECL:
            printf("CLASS_DECL name=%s (line %d)\n",
            node->name?node->name:"", node->lineno);
            break;
        case NODE_ATTRIBUTE:
            printf("ATTRIBUTE name=%s type=%s (line %d)\n",
            node->name?node->name:"", node->typeName?node->typeName:"", node->lineno);
            break;
        case NODE_FUNC_DECL:
            printf("FUNC_DECL name=%s return=%s (line %d)\n",
            node->name?node->name:"", node->typeName?node->typeName:"", node->lineno);
            break;
        case NODE_VAR_DECL:
            printf("VAR_DECL name=%s type=%s (line %d)\n",
            node->name?node->name:"", node->typeName?node->typeName:"", node->lineno);
            break;
        case NODE_PARAM:
            printf("PARAM name=%s type=%s (line %d)\n",
            node->name?node->name:"", node->typeName?node->typeName:"", node->lineno);
            break;
        case NODE_ASSIGN:
            printf("ASSIGN (line %d)\n",
            node->lineno);
            break;
        case NODE_IF:
            printf("IF (line %d)\n",
            node->lineno);
            break;
        case NODE_WHILE:
            printf("WHILE (line %d)\n",
            node->lineno);
            break;
        case NODE_RETURN:
            printf("RETURN (line %d)\n",
            node->lineno);
            break;
        case NODE_READ:
            printf("READ (line %d)\n",
            node->lineno);
            break;
        case NODE_WRITE:
            printf("WRITE (line %d)\n",
            node->lineno);
            break;
        case NODE_FUNCTION_CALL:
            printf("CALL name=%s (line %d)\n",
            node->name?node->name:"", node->lineno);
            break;
        case NODE_ID:
            printf("ID name=%s (line %d)\n",
            node->name?node->name:"", node->lineno);
            break;
        case NODE_INT_LITERAL:
            printf("INT %d (line %d)\n",
            node->intValue, node->lineno);
            break;
        case NODE_FLOAT_LITERAL:
            printf("FLOAT %g (line %d)\n",
            node->floatValue, node->lineno);
            break;
        case NODE_STRING_LITERAL:
            printf("STRING \"%s\" (line %d)\n",
            node->name?node->name:"", node->lineno);
            break;
        case NODE_BINARY_OP:
            printf("BINOP %s (line %d)\n",
            node->name?node->name:"", node->lineno);
            break;
        case NODE_UNARY_OP:
            printf("UNOP %s (line %d)\n",
            node->name?node->name:"", node->lineno);
            break;
        default:
            printf("NODE kind=%d (line %d)\n",
            node->kind, node->lineno);
            break;
    }
    
    // recursively print all children (traverse sibling chain)
    AST *c = node->child;
    while (c) {
        ast_print(c, indent+1);  // print child with increased indentation
        c = c->sibling;           // move to next sibling
    }
    
    // print extra pointer content IF present (e.g., function body, assignment rhs)
    if (node->extra) {
        print_indent(indent+1);
        printf("EXTRA:\n");
        ast_print(node->extra, indent+2);  // print extra content with extra indentation
    }
}

/**
 * ast_free - recursively free all memory allocated for AST subtree
 * 
 * frees the node and all its descendants (children and siblings).
 * also frees dynamically allocated strings (name and typeName).
 * 
 * memory deallocation order:
 * 1. recursively free all children and their siblings
 * 2. free extra pointer content
 * 3. free name and typeName strings
 * 4. free the node itself
 * 
 * @param node Root node of subtree to free
 */
void ast_free(AST *node) {
    if (!node) return;  // safety check: nothing to free IF node is NULL
    
    // recursively free all children (traverse sibling chain)
    AST *c = node->child;
    while (c) {
        AST *next = c->sibling;  // save next sibling before freeing current
        ast_free(c);             // recursively free child subtree
        c = next;                // move to next sibling
    }
    
    // free extra pointer content IF present
    if (node->extra) ast_free(node->extra);
    
    // free dynamically allocated strings
    if (node->name) free(node->name);
    if (node->typeName) free(node->typeName);
    
    // free the node itself
    free(node);
}
