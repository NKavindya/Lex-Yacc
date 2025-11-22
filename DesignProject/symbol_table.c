#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

static SymTable *scope_head = NULL;
static SymTable *scope_tail = NULL;

static const int WORD_SIZE = 4;  /* x86-32 uses 32-bit (4 bytes) words */

static int align_to_word(int value) {
    int remainder = value % WORD_SIZE;
    return remainder == 0 ? value : value + (WORD_SIZE - remainder);
}

SymTable *symtable_create(const char *scopeName, SymTable *parent) {
    SymTable *t = (SymTable*)malloc(sizeof(SymTable));
    t->scopeName = scopeName ? strdup(scopeName) : NULL;
    t->parent = parent;
    t->symbols = NULL;
    t->next = NULL;
    t->next_offset = 0;
    t->frame_size = 0;
    return t;
}

Symbol *sym_new(const char *name, const char *typeName, SymKind kind, int lineno) {
    Symbol *s = (Symbol*)malloc(sizeof(Symbol));
    s->name = name ? strdup(name) : NULL;
    s->typeName = typeName ? strdup(typeName) : NULL;
    s->kind = kind;
    s->lineno = lineno;
    s->size = 0;
    s->offset = -1;
    s->next = NULL;
    s->params = NULL;
    return s;
}

int symtable_type_size(const char *typeName) {
    if (!typeName) return WORD_SIZE;
    if (strcmp(typeName, "int") == 0 || strcmp(typeName, "integer") == 0) return 4;
    if (strcmp(typeName, "float") == 0) return 8;
    if (strcmp(typeName, "void") == 0) return 0;
    return WORD_SIZE; // treat user types/pointers uniformly
}

/* insert in current scope only. Return 0 on success; 1 if duplicate in same scope */
int symtable_insert(SymTable *table, const char *name, const char *typeName, SymKind kind, int lineno) {
    // check duplicates in this scope
    for (Symbol *p = table->symbols; p; p = p->next) {
        if (strcmp(p->name, name) == 0) return 1; // duplicate
    }
    Symbol *s = sym_new(name, typeName, kind, lineno);
    if (kind == SYM_VAR || kind == SYM_PARAM || kind == SYM_ATTR) {
        int size = symtable_type_size(typeName);
        if (size < WORD_SIZE && size > 0) size = WORD_SIZE; // align scalars to word
        table->next_offset = align_to_word(table->next_offset);
        table->next_offset += size;
        table->frame_size = table->next_offset;
        s->offset = table->next_offset;
        s->size = size;
    }
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
    if (!funcSym->params) {
    funcSym->params = param;
    } else {
        Symbol *tail = funcSym->params;
        while (tail->next) tail = tail->next;
        tail->next = param;
    }
}

void symtable_registry_reset(SymTable *global) {
    scope_head = global;
    scope_tail = global;
    if (scope_head) scope_head->next = NULL;
}

void symtable_register_scope(SymTable *scope) {
    if (!scope) return;
    scope->next = NULL;
    if (!scope_head) {
        scope_head = scope_tail = scope;
    } else {
        scope_tail->next = scope;
        scope_tail = scope;
    }
}

SymTable *symtable_find_scope(SymTable *global, const char *scopeName, SymTable *parent) {
    SymTable *start = scope_head ? scope_head : global;
    for (SymTable *t = start; t; t = t->next) {
        if (scopeName && t->scopeName && strcmp(t->scopeName, scopeName) == 0) {
            if (!parent || t->parent == parent) return t;
        }
    }
    return NULL;
}

void symtable_print_all(SymTable *global, FILE *out) {
    SymTable *start = scope_head ? scope_head : global;
    SymTable *t = start;
    while (t) {
        fprintf(out, "Scope: %s\n", t->scopeName ? t->scopeName : "anon");
        fprintf(out, "  frame_size = %d bytes\n", t->frame_size);
        
        /* find function symbol for this scope to get parameter list */
        Symbol *funcSym = NULL;
        if (t->parent) {
            /* look for function symbol in parent scope */
            for (Symbol *fs = t->parent->symbols; fs; fs = fs->next) {
                if (fs->kind == SYM_FUNC && fs->name && t->scopeName && strcmp(fs->name, t->scopeName) == 0) {
                    funcSym = fs;
                    break;
                }
            }
        }
        
        for (Symbol *s = t->symbols; s; s = s->next) {
            const char *k = (s->kind==SYM_VAR?"VAR": s->kind==SYM_FUNC?"FUNC": s->kind==SYM_CLASS?"CLASS": s->kind==SYM_PARAM?"PARAM":"ATTR");
            if (s->offset >= 0) {
                if (s->kind == SYM_PARAM && funcSym && funcSym->params) {
                    /* calculate EBP offset for parameter: first param at EBP+8, second at EBP+12, etc. */
                    int paramIndex = 0;
                    for (Symbol *p = funcSym->params; p; p = p->next) {
                        if (p->name && s->name && strcmp(p->name, s->name) == 0) {
                            int ebp_offset = 8 + (paramIndex * WORD_SIZE);
                            fprintf(out, "  %s\t%s\t%s\t(line %d)\tEBP+%d size=%d\n", 
                                    s->name, s->typeName? s->typeName:"<nil>", k, s->lineno, ebp_offset, s->size);
                            break;
                        }
                        paramIndex++;
                    }
                    if (paramIndex == 0 && funcSym->params && funcSym->params->name && s->name && 
                        strcmp(funcSym->params->name, s->name) != 0) {
                        /* parameter not found in list, use stored offset */
                        fprintf(out, "  %s\t%s\t%s\t(line %d)\toffset=%d size=%d\n", 
                                s->name, s->typeName? s->typeName:"<nil>", k, s->lineno, s->offset, s->size);
                    }
                } else {
                    /* local variable: negative offset from EBP */
                    fprintf(out, "  %s\t%s\t%s\t(line %d)\tEBP-%d size=%d\n", 
                            s->name, s->typeName? s->typeName:"<nil>", k, s->lineno, s->offset, s->size);
                }
            } else {
                fprintf(out, "  %s\t%s\t%s\t(line %d)\n", s->name, s->typeName? s->typeName:"<nil>", k, s->lineno);
            }
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
