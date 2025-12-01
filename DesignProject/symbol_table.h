/* 
 * symbol_table.h - Symbol Table Data Structures and Functions
 * 
 * The symbol table is a hierarchical data structure that tracks all identifiers
 * (variables, functions, classes, parameters, attributes) in the program.
 * It maintains scope information and is used for:
 * - Name resolution (looking up identifiers)
 * - Type checking (verifying type compatibility)
 * - Code generation (determining variable offsets in stack frames)
 * 
 * Structure:
 * - SymTable: Represents a scope (global, function, class, block)
 * - Symbol: Represents a single identifier with its properties
 * - Hierarchical: Scopes have parent-child relationships (global -> function -> block)
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"    // For AST node types used in semantic analysis
#include <stdio.h>  // For FILE type used in printing functions

/**
 * SymKind - Types of symbols in the symbol table
 * 
 * Categories of identifiers that can be stored:
 * - SYM_VAR: Local or global variable
 * - SYM_FUNC: Function definition
 * - SYM_CLASS: Class/struct definition
 * - SYM_PARAM: Function parameter
 * - SYM_ATTR: Class attribute/member variable
 */
typedef enum { 
    SYM_VAR,    // Variable (local or global)
    SYM_FUNC,   // Function
    SYM_CLASS,   // Class/struct
    SYM_PARAM,   // Function parameter
    SYM_ATTR     // Class attribute/member
} SymKind;

/**
 * Symbol - Represents a single identifier in the symbol table
 * 
 * Stores information about an identifier:
 * - name: Identifier name (e.g., "x", "myFunc", "MyClass")
 * - typeName: Type of the symbol (e.g., "integer", "float", or class name)
 * - kind: Category of symbol (variable, function, class, etc.)
 * - lineno: Source line where symbol is declared
 * - size: Memory size in bytes (for variables/parameters)
 * - offset: Stack frame offset (for local variables and parameters)
 * - next: Next symbol in the same scope (linked list)
 * - params: For functions, linked list of parameter symbols
 */
typedef struct Symbol {
    char *name;         // Identifier name (e.g., "x", "myFunc")
    char *typeName;     // Type name: "integer", "float", "void", or custom class name
    SymKind kind;       // Symbol category: variable, function, class, parameter, attribute
    int lineno;         // Source line number where symbol is declared
    int size;           // Memory size in bytes (for variables, parameters, attributes)
    int offset;         // Stack frame offset (negative for locals, positive for parameters)
    struct Symbol *next; // Next symbol in same scope (linked list)
    // For functions: parameter list stored as linked list of Symbols (kind SYM_PARAM)
    struct Symbol *params; // Head of parameter list (for function symbols)
} Symbol;

/**
 * SymTable - Represents a scope in the symbol table
 * 
 * A scope is a region where identifiers are visible (global, function, class, block).
 * Scopes form a hierarchy: global -> function -> block.
 * 
 * - scopeName: Name of the scope (function name, class name, or "global")
 * - parent: Parent scope (NULL for global scope)
 * - symbols: Linked list of symbols declared in this scope
 * - next: Next scope in registry (for printing all scopes)
 * - next_offset: Next available stack frame offset (for local variables)
 * - frame_size: Total bytes reserved for this scope's stack frame
 */
typedef struct SymTable {
    char *scopeName;        // Scope name (function name, class name, or "global")
    struct SymTable *parent; // Parent scope (NULL for global scope)
    Symbol *symbols;        // Linked list of symbols declared in this scope
    struct SymTable *next;  // Next scope in registry (for listing all scopes)
    int next_offset;        // Next assignable stack frame offset (for local variables)
    int frame_size;         // Total bytes reserved for this scope's stack frame
} SymTable;

/* ========== Symbol Table Creation and Management ========== */

/**
 * symtable_create - Create a new symbol table (scope)
 * 
 * Allocates and initializes a new symbol table representing a scope.
 * 
 * @param scopeName Name of the scope (function name, class name, or NULL for anonymous)
 * @param parent Parent scope (NULL for global scope)
 * @return Pointer to newly created symbol table
 */
SymTable *symtable_create(const char *scopeName, SymTable *parent);

/**
 * symtable_lookup - Look up a symbol by name
 * 
 * Searches for a symbol starting in the given scope and climbing up the parent chain.
 * Implements lexical scoping: searches current scope, then parent, then grandparent, etc.
 * 
 * @param table Starting scope to search (searches this scope and all parents)
 * @param name Name of symbol to find
 * @return Pointer to symbol if found, NULL otherwise
 */
Symbol *symtable_lookup(SymTable *table, const char *name);

/**
 * symtable_insert - Insert a symbol into a scope
 * 
 * Adds a new symbol to the specified scope. Checks for duplicates in the same scope.
 * For variables, parameters, and attributes, calculates stack frame offset.
 * 
 * @param table Scope to insert symbol into
 * @param name Symbol name
 * @param typeName Type of the symbol ("integer", "float", class name, etc.)
 * @param kind Category of symbol (variable, function, class, etc.)
 * @param lineno Source line number where symbol is declared
 * @return 0 on success, 1 if duplicate found in same scope
 */
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno);

/**
 * symtable_add_param - Add a parameter to a function symbol
 * 
 * Appends a parameter to a function's parameter list.
 * Used when building function signatures during semantic analysis.
 * 
 * @param funcSym Function symbol to add parameter to
 * @param name Parameter name
 * @param typeName Parameter type
 * @param lineno Source line number
 */
void symtable_add_param(Symbol *funcSym, const char *name, const char *typeName, int lineno);

/**
 * symtable_registry_reset - Reset the scope registry
 * 
 * Initializes the scope registry with the global scope.
 * The registry is used to track all scopes for printing.
 * 
 * @param global Global symbol table to use as registry head
 */
void symtable_registry_reset(SymTable *global);

/**
 * symtable_register_scope - Register a scope in the scope registry
 * 
 * Adds a scope to the registry for later printing/listing.
 * 
 * @param scope Scope to register
 */
void symtable_register_scope(SymTable *scope);

/**
 * symtable_find_scope - Find a scope by name in the registry
 * 
 * Searches the scope registry for a scope with the given name and parent.
 * 
 * @param global Global symbol table (fallback if registry is empty)
 * @param scopeName Name of scope to find
 * @param parent Parent scope (can be NULL to match any parent)
 * @return Pointer to found scope, or NULL if not found
 */
SymTable *symtable_find_scope(SymTable *global, const char *scopeName, SymTable *parent);

/**
 * symtable_type_size - Get memory size for a type
 * 
 * Returns the size in bytes required to store a value of the given type.
 * Used for calculating stack frame offsets.
 * 
 * @param typeName Type name ("integer", "float", "void", or class name)
 * @return Size in bytes (4 for integer, 8 for float, 4 for pointers/classes, 0 for void)
 */
int symtable_type_size(const char *typeName);

/* ========== Symbol Table Printing ========== */

/**
 * symtable_print_all - Print all scopes and symbols to file
 * 
 * Outputs a complete dump of the symbol table showing:
 * - All scopes (global, functions, classes)
 * - All symbols in each scope with their types and offsets
 * - Stack frame sizes
 * - Parameter lists for functions
 * 
 * @param global Global symbol table (starting point)
 * @param out Output file stream (must be open for writing)
 */
void symtable_print_all(SymTable *global, FILE *out);

/* ========== Memory Management ========== */

/**
 * symtable_free_all - Free all memory allocated for symbol tables
 * 
 * Frees all symbol tables and symbols. Currently a stub (OS reclaims memory).
 * 
 * @param global Global symbol table (currently unused, for future implementation)
 */
void symtable_free_all(SymTable *global);

#endif  // SYMBOL_TABLE_H
