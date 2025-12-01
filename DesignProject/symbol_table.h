/* 
 * symbol_table.h - symbol table data structures and functions
 * 
 * the symbol table is a hierarchical data structure that tracks all identifiers
 * (variables, functions, classes, parameters, attributes) in the program.
 * it maintains scope information and is used for:
 * - name resolution (looking up identifiers)
 * - type checking (verifying type compatibility)
 * - code generation (determining variable offsets in stack frames)
 * 
 * structure:
 * - SymTable: represents a scope (global, function, class, block)
 * - Symbol: represents a single identifier with its properties
 * - hierarchical: scopes have parent-child relationships (global -> function -> block)
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"    // for AST node types used in semantic analysis
#include <stdio.h>  // for FILE TYPE used in printing functions

/**
 * SymKind - types of symbols in the symbol table
 * 
 * categories of identifiers that can be stored:
 * - SYM_VAR: local or global variable
 * - SYM_FUNC: function definition
 * - SYM_CLASS: class/struct definition
 * - SYM_PARAM: function parameter
 * - SYM_ATTR: class attribute/member variable
 */
typedef enum { 
    SYM_VAR,    // variable (local or global)
    SYM_FUNC,   // function
    SYM_CLASS,   // class/struct
    SYM_PARAM,   // function parameter
    SYM_ATTR     // class attribute/member
} SymKind;

/**
 * Symbol - represents a single identifier in the symbol table
 * 
 * stores information about an identifier:
 * - name: identifier name (e.g., "x", "myFunc", "MyClass")
 * - typeName: type of the symbol (e.g., "integer", "float", or class name)
 * - kind: category of symbol (variable, function, class, etc.)
 * - lineno: source line where symbol is declared
 * - size: memory size in bytes (for variables/parameters)
 * - offset: stack frame offset (for local variables and parameters)
 * - next: next symbol in the same scope (linked list)
 * - params: for functions, linked list of parameter symbols
 */
typedef struct Symbol {
    char *name;         // identifier name (e.g., "x", "myfunc")
    char *typeName;     // TYPE name: "integer", "float", "void", or custom class name
    SymKind kind;       // symbol category: variable, function, class, parameter, attribute
    int lineno;         // source line number where symbol is declared
    int size;           // memory size in bytes (for variables, parameters, attributes)
    int offset;         // stack frame offset (negative for locals, positive for parameters)
    struct Symbol *next; // next symbol in same scope (linked list)
    // for functions: parameter list stored as linked list of symbols (kind SYM_PARAM)
    struct Symbol *params; // head of parameter list (for function symbols)
} Symbol;

/**
 * SymTable - represents a scope in the symbol table
 * 
 * a scope is a region where identifiers are visible (global, function, class, block).
 * scopes form a hierarchy: global -> function -> block.
 * 
 * - scopeName: name of the scope (function name, class name, or "global")
 * - parent: parent scope (NULL for global scope)
 * - symbols: linked list of symbols declared in this scope
 * - next: next scope in registry (for printing all scopes)
 * - next_offset: next available stack frame offset (for local variables)
 * - frame_size: total bytes reserved for this scope's stack frame
 */
typedef struct SymTable {
    char *scopeName;        // scope name (function name, class name, or "global")
    struct SymTable *parent; // parent scope (NULL for global scope)
    Symbol *symbols;        // linked list of symbols declared in this scope
    struct SymTable *next;  // next scope in registry (for listing all scopes)
    int next_offset;        // next assignable stack frame offset (for local variables)
    int frame_size;         // total bytes reserved for this scope's stack frame
} SymTable;

/* ========== symbol table creation and management ========== */

/**
 * symtable_create - create a new symbol table (scope)
 * 
 * allocates and initializes a new symbol table representing a scope.
 * 
 * @param scopeName name of the scope (function name, class name, or NULL for anonymous)
 * @param parent parent scope (NULL for global scope)
 * @return pointer to newly created symbol table
 */
SymTable *symtable_create(const char *scopeName, SymTable *parent);

/**
 * symtable_lookup - look up a symbol by name
 * 
 * searches for a symbol starting in the given scope and climbing up the parent chain.
 * implements lexical scoping: searches current scope, then parent, then grandparent, etc.
 * 
 * @param table starting scope to search (searches this scope and all parents)
 * @param name name of symbol to find
 * @return pointer to symbol if found, NULL otherwise
 */
Symbol *symtable_lookup(SymTable *table, const char *name);

/**
 * symtable_insert - insert a symbol into a scope
 * 
 * adds a new symbol to the specified scope. checks for duplicates in the same scope.
 * for variables, parameters, and attributes, calculates stack frame offset.
 * 
 * @param table scope to insert symbol into
 * @param name symbol name
 * @param typeName type of the symbol ("integer", "float", class name, etc.)
 * @param kind category of symbol (variable, function, class, etc.)
 * @param lineno source line number where symbol is declared
 * @return 0 on success, 1 if duplicate found in same scope
 */
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno);

/**
 * symtable_add_param - add a parameter to a function symbol
 * 
 * appends a parameter to a function's parameter list.
 * used when building function signatures during semantic analysis.
 * 
 * @param funcSym function symbol to add parameter to
 * @param name parameter name
 * @param typeName parameter type
 * @param lineno source line number
 */
void symtable_add_param(Symbol *funcSym, const char *name, const char *typeName, int lineno);

/**
 * symtable_registry_reset - reset the scope registry
 * 
 * initializes the scope registry with the global scope.
 * the registry is used to track all scopes for printing.
 * 
 * @param global global symbol table to use as registry head
 */
void symtable_registry_reset(SymTable *global);

/**
 * symtable_register_scope - register a scope in the scope registry
 * 
 * adds a scope to the registry for later printing/listing.
 * 
 * @param scope scope to register
 */
void symtable_register_scope(SymTable *scope);

/**
 * symtable_find_scope - find a scope by name in the registry
 * 
 * searches the scope registry for a scope with the given name and parent.
 * 
 * @param global global symbol table (fallback if registry is empty)
 * @param scopeName name of scope to find
 * @param parent parent scope (can be NULL to match any parent)
 * @return pointer to found scope, or NULL if not found
 */
SymTable *symtable_find_scope(SymTable *global, const char *scopeName, SymTable *parent);

/**
 * symtable_type_size - get memory size for a type
 * 
 * returns the size in bytes required to store a value of the given type.
 * used for calculating stack frame offsets.
 * 
 * @param typeName type name ("integer", "float", "void", or class name)
 * @return size in bytes (4 for integer, 8 for float, 4 for pointers/classes, 0 for void)
 */
int symtable_type_size(const char *typeName);

/* ========== symbol table printing ========== */

/**
 * symtable_print_all - print all scopes and symbols to file
 * 
 * outputs a complete dump of the symbol table showing:
 * - all scopes (global, functions, classes)
 * - all symbols in each scope with their types and offsets
 * - stack frame sizes
 * - parameter lists for functions
 * 
 * @param global global symbol table (starting point)
 * @param out output file stream (must be open for writing)
 */
void symtable_print_all(SymTable *global, FILE *out);

/* ========== memory management ========== */

/**
 * symtable_free_all - free all memory allocated for symbol tables
 * 
 * frees all symbol tables and symbols. currently a stub (OS reclaims memory).
 * 
 * @param global global symbol table (currently unused, for future implementation)
 */
void symtable_free_all(SymTable *global);

#endif  // symbol_table_h
