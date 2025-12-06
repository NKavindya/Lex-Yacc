/* 
 * symbol_table.c - Symbol Table Implementation
 * 
 * Implements hierarchical symbol table for semantic analysis and code generation.
 * Features:
 * - Scope management (global, function, class, block scopes)
 * - Symbol insertion with duplicate detection
 * - Symbol lookup with lexical scoping (search current scope, then parents)
 * - Stack frame offset calculation for variables and parameters
 * - Scope registry for printing all scopes
 */

#include "symbol_table.h"  // Symbol table data structures and function prototypes
#include <stdlib.h>        // malloc, free
#include <string.h>        // strdup, strcmp

/* Scope registry: linked list of all scopes for printing */
static SymTable *scope_head = NULL;  // Head of scope registry linked list
static SymTable *scope_tail = NULL;  // Tail of scope registry (for efficient appending)

/* x86-32 architecture: uses 32-bit (4 bytes) words */
static const int WORD_SIZE = 4;

/**
 * align_to_word - Align a value to word boundary
 * 
 * Aligns a byte offset to the next word boundary (multiple of 4).
 * Required for proper stack frame layout on x86-32 architecture.
 * 
 * Example: 5 -> 8, 12 -> 12, 13 -> 16
 * 
 * @param value Byte offset to align
 * @return Aligned offset (multiple of WORD_SIZE)
 */
static int align_to_word(int value) {
    int remainder = value % WORD_SIZE;  // Calculate remainder
    // If already aligned, return as-is; otherwise round up to next word boundary
    return remainder == 0 ? value : value + (WORD_SIZE - remainder);
}

/**
 * symtable_create - Create a new symbol table (scope)
 * 
 * Allocates and initializes a new symbol table representing a scope.
 * A scope can be global, function, class, or block level.
 * 
 * @param scopeName Name of the scope (function name, class name, or NULL for anonymous)
 * @param parent Parent scope (NULL for global scope)
 * @return Pointer to newly created and initialized symbol table
 */
SymTable *symtable_create(const char *scopeName, SymTable *parent) {
    // Allocate memory for new symbol table
    SymTable *t = (SymTable*)malloc(sizeof(SymTable));
    
    // Initialize symbol table fields
    t->scopeName = scopeName ? strdup(scopeName) : NULL;  // Duplicate scope name if provided
    t->parent = parent;                                    // Set parent scope
    t->symbols = NULL;                                     // Initialize symbol list to empty
    t->next = NULL;                                        // Initialize registry pointer
    t->next_offset = 0;                                    // Start stack frame offset at 0
    t->frame_size = 0;                                     // Initialize frame size to 0
    
    return t;  // Return newly created symbol table
}

/**
 * sym_new - Create a new symbol entry
 * 
 * Helper function to allocate and initialize a symbol entry.
 * Used internally by symtable_insert.
 * 
 * @param name Symbol name
 * @param typeName Type of the symbol
 * @param kind Category of symbol
 * @param lineno Source line number
 * @return Pointer to newly created symbol
 */
static Symbol *sym_new(const char *name, const char *typeName, SymKind kind, int lineno) {
    // Allocate memory for new symbol
    Symbol *s = (Symbol*)malloc(sizeof(Symbol));
    
    // Initialize symbol fields
    s->name = name ? strdup(name) : NULL;           // Duplicate name if provided
    s->typeName = typeName ? strdup(typeName) : NULL; // Duplicate type name if provided
    s->kind = kind;                                 // Set symbol category
    s->lineno = lineno;                             // Store source line number
    s->size = 0;                                    // Size not set yet (calculated during insertion)
    s->offset = -1;                                 // Offset not set yet (-1 means uninitialized)
    s->next = NULL;                                 // Initialize next pointer
    s->params = NULL;                               // Initialize parameter list (for functions)
    
    return s;  // Return newly created symbol
}

/**
 * symtable_type_size - Get memory size in bytes for a type
 * 
 * Returns the number of bytes required to store a value of the given type.
 * Used for calculating stack frame offsets during symbol insertion.
 * 
 * @param typeName Type name ("integer", "int", "float", "void", or class name)
 * @return Size in bytes:
 *   - 4 bytes for integer/int
 *   - 8 bytes for float
 *   - 0 bytes for void
 *   - 4 bytes (WORD_SIZE) for custom types/pointers (treated as pointers)
 */
int symtable_type_size(const char *typeName) {
    if (!typeName) return WORD_SIZE;  // Default to word size if type unknown
    
    // Check for integer types (both "int" and "integer" are accepted)
    if (strcmp(typeName, "int") == 0 || strcmp(typeName, "integer") == 0) return 4;
    
    // Check for float type
    if (strcmp(typeName, "float") == 0) return 8;
    
    // Check for void type (no storage needed)
    if (strcmp(typeName, "void") == 0) return 0;
    
    // Custom types and pointers: treat as word-sized pointers
    return WORD_SIZE;
}

/**
 * symtable_insert - Insert a symbol into a scope
 * 
 * Adds a new symbol to the specified scope. Checks for duplicate names in the same scope.
 * For variables, parameters, and attributes, calculates and assigns stack frame offset.
 * 
 * Stack frame layout:
 * - Parameters: Positive offsets from EBP (EBP+8, EBP+12, ...)
 * - Local variables: Negative offsets from EBP (EBP-4, EBP-8, ...)
 * 
 * @param table Scope to insert symbol into
 * @param name Symbol name
 * @param typeName Type of the symbol ("integer", "float", class name, etc.)
 * @param kind Category of symbol (variable, function, class, parameter, attribute)
 * @param lineno Source line number where symbol is declared
 * @return 0 on success, 1 if duplicate name found in same scope
 */
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno) {
    // Check for duplicate names in current scope only (not parent scopes)
    for (Symbol *p = table->symbols; p; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            return 1;  // Duplicate found in same scope
        }
    }
    
    // Create new symbol entry
    Symbol *s = sym_new(name, typeName, kind, lineno);
    
    // Calculate stack frame offset for data-bearing symbols (variables, parameters, attributes)
    if (kind == SYM_VAR || kind == SYM_PARAM || kind == SYM_ATTR) {
        int size = symtable_type_size(typeName);  // Get type size in bytes
        
        // Align scalar types smaller than word size to word boundary
        if (size < WORD_SIZE && size > 0) size = WORD_SIZE;
        
        // Align current offset to word boundary
        table->next_offset = align_to_word(table->next_offset);
        
        // Calculate offset: current offset becomes this symbol's offset
        s->offset = table->next_offset;
        s->size = size;
        
        // Update scope's next available offset and frame size
        table->next_offset += size;
        table->frame_size = table->next_offset;
    }
    
    // Insert symbol at head of symbol list (for efficiency)
    s->next = table->symbols;
    table->symbols = s;
    
    return 0;  // Success
}

/**
 * symtable_lookup - Look up a symbol by name using lexical scoping
 * 
 * Searches for a symbol starting in the given scope and climbing up the parent chain.
 * Implements lexical (static) scoping: searches current scope first, then parent,
 * then grandparent, etc., until global scope is reached.
 * 
 * This matches the language's scoping rules: identifiers are resolved in the
 * innermost scope where they are declared.
 * 
 * @param table Starting scope to search (searches this scope and all ancestors)
 * @param name Name of symbol to find
 * @return Pointer to symbol if found, NULL if not found in any scope
 */
Symbol *symtable_lookup(SymTable *table, const char *name) {
    // Climb scope hierarchy: start at current scope, then parent, then grandparent, etc.
    for (SymTable *t = table; t; t = t->parent) {
        // Search all symbols in current scope
        for (Symbol *s = t->symbols; s; s = s->next) {
            if (strcmp(s->name, name) == 0) {
                return s;  // Found symbol in this scope
            }
        }
    }
    return NULL;  // Symbol not found in any scope
}

/**
 * symtable_add_param - Add a parameter to a function symbol's parameter list
 * 
 * Appends a parameter to a function's parameter list.
 * Used when building function signatures during semantic analysis Pass A.
 * Parameters are stored as a linked list of symbols with kind SYM_PARAM.
 * 
 * @param funcSym Function symbol to add parameter to (must be SYM_FUNC kind)
 * @param name Parameter name
 * @param typeName Parameter type
 * @param lineno Source line number
 */
void symtable_add_param(Symbol *funcSym, const char *name, const char *typeName, int lineno) {
    // Create new parameter symbol
    Symbol *param = sym_new(name, typeName, SYM_PARAM, lineno);
    
    // Append to end of parameter list
    if (!funcSym->params) {
        // First parameter: make it the head
        funcSym->params = param;
    } else {
        // Subsequent parameters: append to end of list
        Symbol *tail = funcSym->params;
        while (tail->next) tail = tail->next;  // Find end of list
        tail->next = param;                     // Append new parameter
    }
}

/**
 * symtable_registry_reset - Reset the scope registry
 * 
 * Initializes the scope registry with the global scope.
 * The registry is a linked list of all scopes, used for printing and lookup.
 * 
 * @param global Global symbol table to use as registry head
 */
void symtable_registry_reset(SymTable *global) {
    scope_head = global;  // Set global scope as registry head
    scope_tail = global;  // Set global scope as registry tail
    if (scope_head) scope_head->next = NULL;  // Initialize next pointer
}

/**
 * symtable_register_scope - Register a scope in the scope registry
 * 
 * Adds a scope to the registry linked list for later printing/listing.
 * The registry maintains a list of all scopes (global, functions, classes).
 * 
 * @param scope Scope to register (must not be NULL)
 */
void symtable_register_scope(SymTable *scope) {
    if (!scope) return;  // Safety check
    
    scope->next = NULL;  // Initialize next pointer
    
    // Append to end of registry
    if (!scope_head) {
        // First scope: initialize both head and tail
        scope_head = scope_tail = scope;
    } else {
        // Subsequent scopes: append to tail
        scope_tail->next = scope;
        scope_tail = scope;  // Update tail pointer
    }
}

/**
 * symtable_find_scope - Find a scope by name in the registry
 * 
 * Searches the scope registry for a scope with the given name and parent.
 * Used to locate function scopes and class scopes during semantic analysis.
 * 
 * @param global Global symbol table (used as fallback if registry is empty)
 * @param scopeName Name of scope to find (function name, class name, etc.)
 * @param parent Parent scope (can be NULL to match any parent)
 * @return Pointer to found scope, or NULL if not found
 */
SymTable *symtable_find_scope(SymTable *global, const char *scopeName, SymTable *parent) {
    // Start search from registry head, or global if registry is empty
    SymTable *start = scope_head ? scope_head : global;
    
    // Traverse registry linked list
    for (SymTable *t = start; t; t = t->next) {
        // Check if scope name matches
        if (scopeName && t->scopeName && strcmp(t->scopeName, scopeName) == 0) {
            // If parent specified, check if it matches; otherwise accept any parent
            if (!parent || t->parent == parent) {
                return t;  // Found matching scope
            }
        }
    }
    return NULL;  // Scope not found
}

/**
 * symtable_print_all - Print all scopes and symbols to file
 * 
 * Outputs a complete dump of the symbol table showing:
 * - All scopes (global, functions, classes) with their names
 * - Stack frame sizes for each scope
 * - All symbols in each scope with:
 *   - Name, type, kind (VAR/FUNC/CLASS/PARAM/ATTR)
 *   - Source line number
 *   - Stack frame offset (EBP+offset for parameters, EBP-offset for locals)
 *   - Memory size
 * - Parameter lists for functions
 * 
 * Format:
 *   Scope: scopeName
 *     frame_size = N bytes
 *     symbol_name  type  kind  (line N)  EBP+/-offset  size=N
 *     ...
 * 
 * @param global Global symbol table (starting point if registry is empty)
 * @param out Output file stream (must be open for writing)
 */
void symtable_print_all(SymTable *global, FILE *out) {
    // Start from registry head, or global if registry is empty
    SymTable *start = scope_head ? scope_head : global;
    SymTable *t = start;
    
    // Traverse all scopes in registry
    while (t) {
        // Print scope header
        fprintf(out, "Scope: %s\n", t->scopeName ? t->scopeName : "anon");
        fprintf(out, "  frame_size = %d bytes\n", t->frame_size);
        
        /* Find function symbol for this scope to get parameter list */
        /* This is needed to calculate correct EBP offsets for parameters */
        Symbol *funcSym = NULL;
        if (t->parent) {
            // Look for function symbol in parent scope (function scopes are children of global/class)
            for (Symbol *fs = t->parent->symbols; fs; fs = fs->next) {
                if (fs->kind == SYM_FUNC && fs->name && t->scopeName && 
                    strcmp(fs->name, t->scopeName) == 0) {
                    funcSym = fs;  // Found function symbol for this scope
                    break;
                }
            }
        }
        
        // Print all symbols in this scope
        for (Symbol *s = t->symbols; s; s = s->next) {
            // Convert symbol kind to string
            const char *k = (s->kind==SYM_VAR?"VAR": 
                            s->kind==SYM_FUNC?"FUNC": 
                            s->kind==SYM_CLASS?"CLASS": 
                            s->kind==SYM_PARAM?"PARAM":"ATTR");
            
            // Print symbol with offset information if available
            if (s->offset >= 0) {
                // Symbol has stack frame offset (variable, parameter, or attribute)
                if (s->kind == SYM_PARAM && funcSym && funcSym->params) {
                    /* Calculate EBP offset for parameter: 
                     * x86-32 calling convention: first param at EBP+8, second at EBP+12, etc.
                     * (EBP+4 is return address, EBP+0 is saved EBP)
                     */
                    int paramIndex = 0;
                    // Find parameter in function's parameter list
                    for (Symbol *p = funcSym->params; p; p = p->next) {
                        if (p->name && s->name && strcmp(p->name, s->name) == 0) {
                            // Calculate EBP offset: EBP+8 for first param, EBP+12 for second, etc.
                            int ebp_offset = 8 + (paramIndex * WORD_SIZE);
                            fprintf(out, "  %s\t%s\t%s\t(line %d)\tEBP+%d size=%d\n", 
                                    s->name, s->typeName? s->typeName:"<nil>", k, s->lineno, ebp_offset, s->size);
                            break;
                        }
                        paramIndex++;
                    }
                    // Fallback: if parameter not found in list, use stored offset
                    if (paramIndex == 0 && funcSym->params && funcSym->params->name && s->name && 
                        strcmp(funcSym->params->name, s->name) != 0) {
                        fprintf(out, "  %s\t%s\t%s\t(line %d)\toffset=%d size=%d\n", 
                                s->name, s->typeName? s->typeName:"<nil>", k, s->lineno, s->offset, s->size);
                    }
                } else {
                    /* Local variable or attribute: negative offset from EBP */
                    /* Stack grows downward: locals are at EBP-4, EBP-8, etc. */
                    fprintf(out, "  %s\t%s\t%s\t(line %d)\tEBP-%d size=%d\n", 
                            s->name, s->typeName? s->typeName:"<nil>", k, s->lineno, s->offset, s->size);
                }
            } else {
                // Symbol has no offset (function, class, or symbol without storage)
                fprintf(out, "  %s\t%s\t%s\t(line %d)\n", 
                        s->name, s->typeName? s->typeName:"<nil>", k, s->lineno);
            }
            
            // Print parameter list for functions
            if (s->params) {
                fprintf(out, "    params:");
                for (Symbol *p = s->params; p; p = p->next) {
                    fprintf(out, " %s:%s", p->name, p->typeName?p->typeName:"<nil>");
                }
                fprintf(out, "\n");
            }
        }
        t = t->next;  // Move to next scope in registry
    }
}

/**
 * symtable_free_all - Free all memory allocated for symbol tables
 * 
 * Currently a stub implementation. Memory is reclaimed by the OS when the program exits.
 * For a production compiler, this would recursively free all symbol tables and symbols.
 * 
 * @param global Global symbol table (currently unused, for future implementation)
 */
void symtable_free_all(SymTable *global) {
    // Naive implementation: do nothing (OS will reclaim memory when program exits)
    // For production use, implement recursive freeing of all scopes and symbols
    (void)global;  // Suppress unused parameter warning
}
