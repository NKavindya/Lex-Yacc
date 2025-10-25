#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

SymTable *symtable_create(const char *scopeName, SymTable *parent) {
    SymTable *t = (SymTable*)malloc(sizeof(SymTable));
    t->scopeName = scopeName ? strdup(scopeName) : NULL;
    t->parent = parent;
    t->symbols = NULL;
    t->next = NULL;
    return t;
}

Symbol *sym_new(const char *name, const char *typeName, SymKind kind, int lineno) {
    Symbol *s = (Symbol*)malloc(sizeof(Symbol));
    s->name = name ? strdup(name) : NULL;
    s->typeName = typeName ? strdup(typeName) : NULL;
    s->kind = kind;
    s->lineno = lineno;
    s->next = NULL;
    s->params = NULL;
    return s;
}

/* Insert in current scope only. Return 0 on success; 1 if duplicate in same scope */
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno) {
    // check duplicates in this scope
    for (Symbol *p = table->symbols; p; p = p->next) {
        if (strcmp(p->name, name) == 0) return 1; // duplicate
    }
    Symbol *s = sym_new(name, typeName, kind, lineno);
    s->next = table->symbols;
    table->symbols = s;
    return 0;
}

/* lookup: climb parents */
Symbol *symtable_lookup(SymTable *table, const char *name) {
    for (SymTable *t = table; t; t = t->parent) {
        for (Symbol *s = t->symbols; s; s = s->next) {
            if (strcmp(s->name, name) == 0) return s;
        }
    }
    return NULL;
}

void symtable_add_param(Symbol *funcSym, const char *name, const char *typeName, int lineno) {
    Symbol *param = sym_new(name, typeName, SYM_PARAM, lineno);
    param->next = funcSym->params;
    funcSym->params = param;
}

void symtable_print_all(SymTable *global, FILE *out) {
    // print all scopes by traversal (global and child scopes via next link)
    SymTable *t = global;
    while (t) {
        fprintf(out, "Scope: %s\n", t->scopeName ? t->scopeName : "anon");
        for (Symbol *s = t->symbols; s; s = s->next) {
            const char *k = (s->kind==SYM_VAR?"VAR": s->kind==SYM_FUNC?"FUNC": s->kind==SYM_CLASS?"CLASS": s->kind==SYM_PARAM?"PARAM":"ATTR");
            fprintf(out, "  %s\t%s\t%s\t(line %d)\n", s->name, s->typeName? s->typeName:"<nil>", k, s->lineno);
            if (s->params) {
                fprintf(out, "    params:");
                for (Symbol *p = s->params; p; p = p->next) {
                    fprintf(out, " %s:%s", p->name, p->typeName?p->typeName:"<nil>");
                }
                fprintf(out, "\n");
            }
        }
        t = t->next;
    }
}

/* free helpers omitted for brevity (not needed for short-run tools) */
void symtable_free_all(SymTable *global) {
    // naive: do nothing (OS will reclaim memory). Implement if needed.
    (void)global;
}
