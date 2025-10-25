#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"
#include <stdio.h>

typedef enum { SYM_VAR, SYM_FUNC, SYM_CLASS, SYM_PARAM, SYM_ATTR } SymKind;

typedef struct Symbol {
    char *name;
    char *typeName;   // e.g., "integer", "float", or classname
    SymKind kind;
    int lineno;
    struct Symbol *next;
    // For functions: parameter types as linked list of Symbols (kind SYM_PARAM)
    struct Symbol *params; // head of param list
} Symbol;

typedef struct SymTable {
    char *scopeName;
    struct SymTable *parent;
    Symbol *symbols;   // linked list
    struct SymTable *next; // for listing scopes
} SymTable;

/* creation & lookup */
SymTable *symtable_create(const char *scopeName, SymTable *parent);
Symbol *symtable_lookup(SymTable *table, const char *name);
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno);
void symtable_add_param(Symbol *funcSym, const char *name, const char *typeName, int lineno);

/* printing */
void symtable_print_all(SymTable *global, FILE *out);

/* utilities */
void symtable_free_all(SymTable *global);

#endif
