#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

SymTable *symtable_create(const char *scopeName, SymTable *parent) {
    SymTable *t = (SymTable*)malloc(sizeof(SymTable));
    t->scopeName = scopeName ? strdup(scopeName) : NULL;
    t->parent = parent;
    t->symbols = NULL;
    t->next = NULL;
    t->next_offset = 4;  /* start at 4 bytes (first local at EBP-4) */
    t->frame_size = 0;
    return t;
}

static Symbol *sym_new(const char *name, const char *typeName,
                       SymKind kind, int lineno) {
    Symbol *s = (Symbol*)malloc(sizeof(Symbol));
    s->name = name ? strdup(name) : NULL;
    s->typeName = typeName ? strdup(typeName) : NULL;
    s->kind = kind;
    s->lineno = lineno;
    s->size = 4;  /* default: 4 bytes (32-bit word) */
    s->offset = -1;  /* uninitialized */
    s->next = NULL;
    s->params = NULL;
    return s;
}

/* Insert in current scope only. Return 0 on success; 1 if duplicate in same scope */
int symtable_insert(SymTable *table, const char *name, const char *typeName,
                    SymKind kind, int lineno) {
    for (Symbol *p = table->symbols; p; p = p->next) {
        if (strcmp(p->name, name) == 0) return 1; /* duplicate */
    }
    Symbol *s = sym_new(name, typeName, kind, lineno);
    s->next = table->symbols;
    table->symbols = s;
    return 0;
}

/* lookup climbing parents */
Symbol *symtable_lookup(SymTable *table, const char *name) {
    for (SymTable *t = table; t; t = t->parent) {
        for (Symbol *s = t->symbols; s; s = s->next) {
            if (strcmp(s->name, name) == 0) return s;
        }
    }
    return NULL;
}

void symtable_add_param(Symbol *funcSym, const char *name,
                        const char *typeName, int lineno) {
    Symbol *param = sym_new(name, typeName, SYM_PARAM, lineno);
    param->next = funcSym->params;
    funcSym->params = param;
}

void symtable_print_all(SymTable *global, FILE *out) {
    SymTable *t = global;
    while (t) {
        fprintf(out, "Scope: %s\n", t->scopeName ? t->scopeName : "anon");
        for (Symbol *s = t->symbols; s; s = s->next) {
            const char *k =
                (s->kind == SYM_VAR ? "VAR" :
                 s->kind == SYM_FUNC ? "FUNC" :
                 s->kind == SYM_CLASS ? "CLASS" :
                 s->kind == SYM_PARAM ? "PARAM" : "ATTR");
            fprintf(out, "  %s\t%s\t%s\t(line %d)\n",
                    s->name, s->typeName ? s->typeName : "<nil>",
                    k, s->lineno);
            if (s->params) {
                fprintf(out, "    params:");
                for (Symbol *p = s->params; p; p = p->next) {
                    fprintf(out, " %s:%s", p->name,
                            p->typeName ? p->typeName : "<nil>");
                }
                fprintf(out, "\n");
            }
        }
        t = t->next;
    }
}

void symtable_free_all(SymTable *global) {
    (void)global; /* OS will reclaim memory; implement if needed. */
}

/* Find a scope by name, optionally within a parent scope */
SymTable *symtable_find_scope(SymTable *global, const char *scopeName, SymTable *parent) {
    if (!global || !scopeName) return NULL;
    
    /* Search in parent's children if parent specified */
    if (parent) {
        for (SymTable *t = parent->next; t; t = t->next) {
            if (t->scopeName && strcmp(t->scopeName, scopeName) == 0) {
                return t;
            }
        }
    }
    
    /* Search globally */
    for (SymTable *t = global; t; t = t->next) {
        if (t->scopeName && strcmp(t->scopeName, scopeName) == 0) {
            return t;
        }
    }
    return NULL;
}

/* Get size in bytes for a type */
int symtable_type_size(const char *typeName) {
    if (!typeName) return 4;  /* default: 4 bytes */
    if (strcmp(typeName, "integer") == 0 || strcmp(typeName, "int") == 0) return 4;
    if (strcmp(typeName, "float") == 0) return 4;  /* 32-bit float */
    return 4;  /* default: assume 4 bytes for user-defined types */
}


