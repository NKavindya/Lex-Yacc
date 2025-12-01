/* 
 * ast.h - abstract syntax tree (AST) data structure definitions
 * 
 * the AST is a tree representation of the source code's syntactic structure.
 * each node represents a language construct (class, function, statement, expression, etc.)
 * the tree structure preserves the hierarchical relationships in the source code.
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>  // for FILE TYPE used in function signatures

/**
 * NodeKind - enumeration of all possible AST node types
 * 
 * each value represents a different language construct:
 * - PROGRAM: root node containing all top-level declarations
 * - CLASS_DECL: class/struct declaration
 * - CLASS_INHERIT_LIST: list of parent classes (inheritance)
 * - CLASS_BODY: body of a class containing members
 * - ATTRIBUTE: class attribute/member variable
 * - FUNC_DECL: function declaration (signature)
 * - VAR_DECL: variable declaration
 * - PARAM_LIST: function parameter list
 * - PARAM: single function parameter
 * - FUNC_BODY: function body containing statements
 * - STATEMENT_LIST: list of statements
 * - ASSIGN: assignment statement
 * - IF: conditional statement
 * - WHILE: loop statement
 * - RETURN: return statement
 * - READ: input statement
 * - WRITE: output statement
 * - FUNCTION_CALL: function invocation
 * - ID: identifier (variable name, function name, etc.)
 * - INT_LITERAL: integer constant
 * - FLOAT_LITERAL: floating-point constant
 * - STRING_LITERAL: string constant
 * - BINARY_OP: binary operator (+, -, *, /, ==, <, etc.)
 * - UNARY_OP: unary operator (+, -, not)
 * - TYPE: type specification (integer, float, custom type)
 * - VISIBILITY: visibility modifier (public, private)
 * - EMPTY: empty/placeholder node
 */
typedef enum {
    NODE_PROGRAM,           // root node: entire PROGRAM
    NODE_CLASS_DECL,        // class declaration: class name { ... }
    NODE_CLASS_INHERIT_LIST, // inheritance list: isa parent1, parent2
    NODE_CLASS_BODY,        // class body: { members }
    NODE_ATTRIBUTE,         // class attribute: attribute x : integer
    NODE_FUNC_DECL,         // function declaration: func name() => TYPE
    NODE_VAR_DECL,          // variable declaration: local x : integer
    NODE_PARAM_LIST,        // parameter list: (param1, param2, ...)
    NODE_PARAM,             // single parameter: name : TYPE
    NODE_FUNC_BODY,         // function body: { statements }
    NODE_STATEMENT_LIST,    // list of statements
    NODE_ASSIGN,            // assignment: variable := expression
    NODE_IF,                // conditional: IF (condition) then ... else ...
    NODE_WHILE,             // loop: WHILE (condition) { ... }
    NODE_RETURN,            // RETURN: RETURN (expression)
    NODE_READ,              // input: READ (variable)
    NODE_WRITE,             // output: WRITE (expression)
    NODE_FUNCTION_CALL,     // function call: funcname(args)
    NODE_ID,                // identifier: variable/function/class name
    NODE_INT_LITERAL,       // integer literal: 42
    NODE_FLOAT_LITERAL,     // float literal: 3.14
    NODE_STRING_LITERAL,    // string literal: "hello"
    NODE_BINARY_OP,         // binary operator: +, -, *, /, ==, <, >, etc.
    NODE_UNARY_OP,          // unary operator: +, -, not
    NODE_TYPE,              // TYPE: integer, float, or custom TYPE name
    NODE_VISIBILITY,        // VISIBILITY: public or private
    NODE_EMPTY              // EMPTY/placeholder node
} NodeKind;

/**
 * AST - abstract syntax tree node structure
 * 
 * represents a single node in the AST. uses a tree structure with:
 * - child: points to first child node (for hierarchical relationships)
 * - sibling: points to next sibling node (for lists of nodes)
 * - extra: auxiliary pointer for special cases (e.g., function body stored separately)
 * 
 * example: in "x := y + z", the ASSIGN node has:
 *   - child: ID node for "x"
 *   - extra: BINARY_OP node for "+" (which has children "y" and "z")
 */
typedef struct AST {
    NodeKind kind;          // TYPE of this node (from nodekind enum)
    char *name;             // identifier name or operator symbol (e.g., "x", "+", "funcname")
    char *typeName;         // TYPE name for TYPE nodes or annotated types (e.g., "integer", "float")
    int intValue;           // integer value for node_int_literal nodes
    double floatValue;      // float value for node_float_literal nodes
    int lineno;             // source line number where this construct appears (for error reporting)
    struct AST *child;      // pointer to first child node (hierarchical relationship)
    struct AST *sibling;    // pointer to next sibling node (for lists: statements, parameters, etc.)
    struct AST *extra;      // auxiliary pointer (e.g., function body, right-hand side of assignment)
} AST;

/* function prototypes for AST manipulation */

/**
 * ast_new - create a new AST node
 * @param kind type of node to create
 * @param name identifier or operator name (can be NULL)
 * @param lineno source line number
 * @return pointer to newly allocated AST node
 */
AST *ast_new(NodeKind kind, const char *name, int lineno);

/**
 * ast_new_int - create an integer literal node
 * @param val integer value
 * @param lineno source line number
 * @return pointer to NODE_INT_LITERAL node with value set
 */
AST *ast_new_int(int val, int lineno);

/**
 * ast_new_float - create a float literal node
 * @param val float value
 * @param lineno source line number
 * @return pointer to NODE_FLOAT_LITERAL node with value set
 */
AST *ast_new_float(double val, int lineno);

/**
 * ast_new_string - create a string literal node
 * @param val string value
 * @param lineno source line number
 * @return pointer to NODE_STRING_LITERAL node with value set
 */
AST *ast_new_string(const char *val, int lineno);

/**
 * ast_append_child - add a child node to a parent
 * @param parent parent node (must not be NULL)
 * @param child child node to add
 * 
 * if parent already has children, appends to sibling chain.
 */
void ast_append_child(AST *parent, AST *child);

/**
 * ast_append_sibling - add a sibling node to a list
 * @param list pointer to head of sibling list (may be modified)
 * @param node node to append to the list
 * 
 * appends node to the end of the sibling chain.
 */
void ast_append_sibling(AST **list, AST *node);

/**
 * ast_print - print AST structure to stdout (for debugging)
 * @param node root node to start printing from
 * @param indent current indentation level (for tree visualization)
 * 
 * recursively prints the AST tree structure with indentation.
 */
void ast_print(AST *node, int indent);

/**
 * ast_free - free all memory allocated for AST subtree
 * @param node root node of subtree to free
 * 
 * recursively frees node and all its children and siblings.
 * also frees name and typeName strings.
 */
void ast_free(AST *node);

#endif  // ast_h
