/* 
 * ast.c - Abstract Syntax Tree Implementation
 * 
 * Provides functions for creating, manipulating, and printing AST nodes.
 * The AST is built during parsing and used by semantic analysis and code generation.
 */

#include "ast.h"      // AST data structure definitions
#include <stdlib.h>   // malloc, free
#include <string.h>   // strdup, strcmp
#include <stdio.h>    // printf, fprintf

/**
 * ast_new - Create a new AST node with specified type and name
 * 
 * Allocates memory for a new AST node and initializes all fields.
 * The name parameter is duplicated if provided (caller retains ownership of original).
 * 
 * @param kind Type of node to create (from NodeKind enum)
 * @param name Identifier or operator name (NULL if not applicable)
 * @param lineno Source line number where this construct appears
 * @return Pointer to newly allocated and initialized AST node
 */
AST *ast_new(NodeKind kind, const char *name, int lineno) {
    // Allocate memory for new AST node
    AST *n = (AST*)malloc(sizeof(AST));
    
    // Initialize node fields
    n->kind = kind;                                    // Set node type
    n->name = name ? strdup(name) : NULL;             // Duplicate name string if provided
    n->typeName = NULL;                                // Type name not set initially
    n->intValue = 0;                                   // Integer value defaults to 0
    n->floatValue = 0.0;                               // Float value defaults to 0.0
    n->lineno = lineno;                                // Store source line number
    n->child = n->sibling = n->extra = NULL;          // Initialize all pointers to NULL
    
    return n;  // Return newly created node
}

/**
 * ast_new_int - Create an integer literal node
 * 
 * Convenience function to create an AST node for an integer constant.
 * 
 * @param val Integer value to store in the node
 * @param lineno Source line number
 * @return Pointer to NODE_INT_LITERAL node with intValue set
 */
AST *ast_new_int(int val, int lineno) {
    AST *n = ast_new(NODE_INT_LITERAL, NULL, lineno);  // Create integer literal node
    n->intValue = val;                                  // Store the integer value
    return n;
}

/**
 * ast_new_float - Create a float literal node
 * 
 * Convenience function to create an AST node for a floating-point constant.
 * 
 * @param val Float value to store in the node
 * @param lineno Source line number
 * @return Pointer to NODE_FLOAT_LITERAL node with floatValue set
 */
AST *ast_new_float(double val, int lineno) {
    AST *n = ast_new(NODE_FLOAT_LITERAL, NULL, lineno);  // Create float literal node
    n->floatValue = val;                                 // Store the float value
    return n;
}

/**
 * ast_new_string - Create a string literal node
 * 
 * Convenience function to create an AST node for a string constant.
 * 
 * @param val String value to store in the node (will be duplicated)
 * @param lineno Source line number
 * @return Pointer to NODE_STRING_LITERAL node with name set to duplicated string
 */
AST *ast_new_string(const char *val, int lineno) {
    AST *n = ast_new(NODE_STRING_LITERAL, strdup(val), lineno);  // Create string literal node with duplicated string
    return n;
}

/**
 * ast_append_child - Add a child node to a parent node
 * 
 * Adds a child node to the parent. If the parent already has children,
 * the new child is appended to the sibling chain of existing children.
 * 
 * @param parent Parent node (must not be NULL)
 * @param child Child node to add
 */
void ast_append_child(AST *parent, AST *child) {
    if (!parent) return;  // Safety check: parent must exist
    
    // If parent has no children yet, make this the first child
    if (!parent->child) {
        parent->child = child;
    } else {
        // Otherwise, append to the sibling chain of existing children
        ast_append_sibling(&parent->child, child);
    }
}

/**
 * ast_append_sibling - Add a node to the end of a sibling list
 * 
 * Appends a node to the end of a linked list of sibling nodes.
 * Used for building lists (statements, parameters, etc.).
 * 
 * @param list Pointer to head of sibling list (may be modified if list is empty)
 * @param node Node to append to the list
 */
void ast_append_sibling(AST **list, AST *node) {
    if (!list || !node) return;  // Safety checks
    
    // If list is empty, make this node the head
    if (!*list) { 
        *list = node; 
        return; 
    }
    
    // Traverse to end of sibling chain
    AST *p = *list;
    while (p->sibling) p = p->sibling;
    
    // Append new node at the end
    p->sibling = node;
}

/**
 * print_indent - Helper function to print indentation spaces
 * 
 * Prints a number of spaces for tree visualization.
 * Each level of indentation is 2 spaces.
 * 
 * @param indent Number of indentation levels (each level = 2 spaces)
 */
static void print_indent(int indent) {
    for (int i=0; i<indent; i++) printf("  ");  // Print 2 spaces per indentation level
}

/**
 * ast_print - Recursively print AST structure to stdout
 * 
 * Prints the AST in a tree format with indentation showing hierarchy.
 * For each node, prints:
 * - Node type and relevant information (name, type, value)
 * - Source line number
 * - Children nodes (indented)
 * - Extra pointer content (if present)
 * 
 * @param node Root node of subtree to print
 * @param indent Current indentation level (0 = root, increases for children)
 */
void ast_print(AST *node, int indent) {
    if (!node) return;  // Safety check: nothing to print if node is NULL
    
    // Print indentation and node information
    print_indent(indent);
    
    // Print node-specific information based on node type
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
    
    // Recursively print all children (traverse sibling chain)
    AST *c = node->child;
    while (c) {
        ast_print(c, indent+1);  // Print child with increased indentation
        c = c->sibling;           // Move to next sibling
    }
    
    // Print extra pointer content if present (e.g., function body, assignment RHS)
    if (node->extra) {
        print_indent(indent+1);
        printf("EXTRA:\n");
        ast_print(node->extra, indent+2);  // Print extra content with extra indentation
    }
}

/**
 * ast_free - Recursively free all memory allocated for AST subtree
 * 
 * Frees the node and all its descendants (children and siblings).
 * Also frees dynamically allocated strings (name and typeName).
 * 
 * Memory deallocation order:
 * 1. Recursively free all children and their siblings
 * 2. Free extra pointer content
 * 3. Free name and typeName strings
 * 4. Free the node itself
 * 
 * @param node Root node of subtree to free
 */
void ast_free(AST *node) {
    if (!node) return;  // Safety check: nothing to free if node is NULL
    
    // Recursively free all children (traverse sibling chain)
    AST *c = node->child;
    while (c) {
        AST *next = c->sibling;  // Save next sibling before freeing current
        ast_free(c);             // Recursively free child subtree
        c = next;                // Move to next sibling
    }
    
    // Free extra pointer content if present
    if (node->extra) ast_free(node->extra);
    
    // Free dynamically allocated strings
    if (node->name) free(node->name);
    if (node->typeName) free(node->typeName);
    
    // Free the node itself
    free(node);
}
