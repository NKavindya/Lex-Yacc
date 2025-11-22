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
    int size;         // bytes reserved (for data-bearing symbols)
    int offset;       // stack-frame offset
    struct Symbol *next;
    // For functions: parameter types as linked list of Symbols (kind SYM_PARAM)
    struct Symbol *params; // head of param list
} Symbol;

typedef struct SymTable {
    char *scopeName;
    struct SymTable *parent;
    Symbol *symbols;   // linked list
    struct SymTable *next; // for listing scopes
    int next_offset;   // next assignable frame offset
    int frame_size;    // total bytes reserved for scope
} SymTable;

/* creation & lookup */
SymTable *symtable_create(const char *scopeName, SymTable *parent);
Symbol *symtable_lookup(SymTable *table, const char *name);
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno);
void symtable_add_param(Symbol *funcSym, const char *name, const char *typeName, int lineno);
void symtable_registry_reset(SymTable *global);
void symtable_register_scope(SymTable *scope);
SymTable *symtable_find_scope(SymTable *global, const char *scopeName, SymTable *parent);
int symtable_type_size(const char *typeName);

/* printing */
void symtable_print_all(SymTable *global, FILE *out);

/* utilities */
void symtable_free_all(SymTable *global);

#endif
