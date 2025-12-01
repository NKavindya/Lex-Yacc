/* 
 * ast.h - Abstract Syntax Tree (AST) Data Structure Definitions
 * 
 * The AST is a tree representation of the source code's syntactic structure.
 * Each node represents a language construct (class, function, statement, expression, etc.)
 * The tree structure preserves the hierarchical relationships in the source code.
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>  // For FILE type used in function signatures

/**
 * NodeKind - Enumeration of all possible AST node types
 * 
 * Each value represents a different language construct:
 * - PROGRAM: Root node containing all top-level declarations
 * - CLASS_DECL: Class/struct declaration
 * - CLASS_INHERIT_LIST: List of parent classes (inheritance)
 * - CLASS_BODY: Body of a class containing members
 * - ATTRIBUTE: Class attribute/member variable
 * - FUNC_DECL: Function declaration (signature)
 * - VAR_DECL: Variable declaration
 * - PARAM_LIST: Function parameter list
 * - PARAM: Single function parameter
 * - FUNC_BODY: Function body containing statements
 * - STATEMENT_LIST: List of statements
 * - ASSIGN: Assignment statement
 * - IF: Conditional statement
 * - WHILE: Loop statement
 * - RETURN: Return statement
 * - READ: Input statement
 * - WRITE: Output statement
 * - FUNCTION_CALL: Function invocation
 * - ID: Identifier (variable name, function name, etc.)
 * - INT_LITERAL: Integer constant
 * - FLOAT_LITERAL: Floating-point constant
 * - STRING_LITERAL: String constant
 * - BINARY_OP: Binary operator (+, -, *, /, ==, <, etc.)
 * - UNARY_OP: Unary operator (+, -, not)
 * - TYPE: Type specification (integer, float, custom type)
 * - VISIBILITY: Visibility modifier (public, private)
 * - EMPTY: Empty/placeholder node
 */
typedef enum {
    NODE_PROGRAM,           // Root node: entire program
    NODE_CLASS_DECL,        // Class declaration: class Name { ... }
    NODE_CLASS_INHERIT_LIST, // Inheritance list: isa Parent1, Parent2
    NODE_CLASS_BODY,        // Class body: { members }
    NODE_ATTRIBUTE,         // Class attribute: attribute x : integer
    NODE_FUNC_DECL,         // Function declaration: func name() => type
    NODE_VAR_DECL,          // Variable declaration: local x : integer
    NODE_PARAM_LIST,        // Parameter list: (param1, param2, ...)
    NODE_PARAM,             // Single parameter: name : type
    NODE_FUNC_BODY,         // Function body: { statements }
    NODE_STATEMENT_LIST,    // List of statements
    NODE_ASSIGN,            // Assignment: variable := expression
    NODE_IF,                // Conditional: if (condition) then ... else ...
    NODE_WHILE,             // Loop: while (condition) { ... }
    NODE_RETURN,            // Return: return (expression)
    NODE_READ,              // Input: read (variable)
    NODE_WRITE,             // Output: write (expression)
    NODE_FUNCTION_CALL,     // Function call: funcName(args)
    NODE_ID,                // Identifier: variable/function/class name
    NODE_INT_LITERAL,       // Integer literal: 42
    NODE_FLOAT_LITERAL,     // Float literal: 3.14
    NODE_STRING_LITERAL,    // String literal: "hello"
    NODE_BINARY_OP,         // Binary operator: +, -, *, /, ==, <, >, etc.
    NODE_UNARY_OP,          // Unary operator: +, -, not
    NODE_TYPE,              // Type: integer, float, or custom type name
    NODE_VISIBILITY,        // Visibility: public or private
    NODE_EMPTY              // Empty/placeholder node
} NodeKind;

/**
 * AST - Abstract Syntax Tree node structure
 * 
 * Represents a single node in the AST. Uses a tree structure with:
 * - child: Points to first child node (for hierarchical relationships)
 * - sibling: Points to next sibling node (for lists of nodes)
 * - extra: Auxiliary pointer for special cases (e.g., function body stored separately)
 * 
 * Example: In "x := y + z", the ASSIGN node has:
 *   - child: ID node for "x"
 *   - extra: BINARY_OP node for "+" (which has children "y" and "z")
 */
typedef struct AST {
    NodeKind kind;          // Type of this node (from NodeKind enum)
    char *name;             // Identifier name or operator symbol (e.g., "x", "+", "funcName")
    char *typeName;         // Type name for type nodes or annotated types (e.g., "integer", "float")
    int intValue;           // Integer value for NODE_INT_LITERAL nodes
    double floatValue;      // Float value for NODE_FLOAT_LITERAL nodes
    int lineno;             // Source line number where this construct appears (for error reporting)
    struct AST *child;      // Pointer to first child node (hierarchical relationship)
    struct AST *sibling;    // Pointer to next sibling node (for lists: statements, parameters, etc.)
    struct AST *extra;      // Auxiliary pointer (e.g., function body, right-hand side of assignment)
} AST;

/* Function prototypes for AST manipulation */

/**
 * ast_new - Create a new AST node
 * @param kind Type of node to create
 * @param name Identifier or operator name (can be NULL)
 * @param lineno Source line number
 * @return Pointer to newly allocated AST node
 */
AST *ast_new(NodeKind kind, const char *name, int lineno);

/**
 * ast_new_int - Create an integer literal node
 * @param val Integer value
 * @param lineno Source line number
 * @return Pointer to NODE_INT_LITERAL node with value set
 */
AST *ast_new_int(int val, int lineno);

/**
 * ast_new_float - Create a float literal node
 * @param val Float value
 * @param lineno Source line number
 * @return Pointer to NODE_FLOAT_LITERAL node with value set
 */
AST *ast_new_float(double val, int lineno);

/**
 * ast_new_string - Create a string literal node
 * @param val String value
 * @param lineno Source line number
 * @return Pointer to NODE_STRING_LITERAL node with value set
 */
AST *ast_new_string(const char *val, int lineno);

/**
 * ast_append_child - Add a child node to a parent
 * @param parent Parent node (must not be NULL)
 * @param child Child node to add
 * 
 * If parent already has children, appends to sibling chain.
 */
void ast_append_child(AST *parent, AST *child);

/**
 * ast_append_sibling - Add a sibling node to a list
 * @param list Pointer to head of sibling list (may be modified)
 * @param node Node to append to the list
 * 
 * Appends node to the end of the sibling chain.
 */
void ast_append_sibling(AST **list, AST *node);

/**
 * ast_print - Print AST structure to stdout (for debugging)
 * @param node Root node to start printing from
 * @param indent Current indentation level (for tree visualization)
 * 
 * Recursively prints the AST tree structure with indentation.
 */
void ast_print(AST *node, int indent);

/**
 * ast_free - Free all memory allocated for AST subtree
 * @param node Root node of subtree to free
 * 
 * Recursively frees node and all its children and siblings.
 * Also frees name and typeName strings.
 */
void ast_free(AST *node);

#endif  // AST_H
