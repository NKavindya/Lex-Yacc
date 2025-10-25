#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symbol_table.h"

/* global containers for symbol tables and errors */
SymTable *globalTable = NULL;
FILE *errFile = NULL;

/* simple error collection (print immediately) */
void sem_error(int lineno, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (errFile) {
        fprintf(errFile, "Semantic Error [line %d]: ", lineno);
        vfprintf(errFile, fmt, ap);
        fprintf(errFile, "\n");
    } else {
        printf("Semantic Error [line %d]: ", lineno);
        vfprintf(stdout, fmt, ap);
        printf("\n");
    }
    va_end(ap);
}

/* ---- PASS A: build symbol tables (declarations) ---- */
void semantic_passA_build(SymTable *curScope, AST *node);

/* helper to walk sibling list */
void passA_walk_list(SymTable *curScope, AST *list) {
    AST *p = list;
    while (p) {
        semantic_passA_build(curScope, p);
        p = p->sibling;
    }
}

/* insert function parameters into function symbol entry */
void bind_function_params(Symbol *funcSym, AST *paramList) {
    AST *p = paramList;
    while (p) {
        // param nodes: NODE_PARAM with name and typeName
        symtable_add_param(funcSym, p->name, p->typeName?p->typeName:"<nil>", p->lineno);
        p = p->sibling;
    }
}

void semantic_passA_build(SymTable *curScope, AST *node) {
    if (!node) return;
    switch (node->kind) {
        case NODE_CLASS_DECL: {
            // insert class into global
            if (symtable_insert(globalTable, node->name, node->name, SYM_CLASS, node->lineno)) {
                sem_error(node->lineno, "Class '%s' redeclared", node->name);
            } else {
                // create class scope and link it via next pointer for dump convenience
                SymTable *classScope = symtable_create(node->name, globalTable);
                // attach to globalTable->next chain
                SymTable *t = globalTable;
                while (t->next) t = t->next;
                t->next = classScope;
                // process members: attributes and funcs
                AST *member = node->child;
                while (member) {
                    if (member->kind == NODE_ATTRIBUTE) {
                        // attribute: child contains var_decl node
                        AST *v = member->child;
                        if (v && v->kind == NODE_VAR_DECL) {
                            if (symtable_insert(classScope, v->name, v->typeName?v->typeName:"<nil>", SYM_ATTR, v->lineno)) {
                                sem_error(v->lineno, "Attribute '%s' redeclared in class '%s'", v->name, node->name);
                            }
                        }
                    } else if (member->kind == NODE_FUNC_DECL) {
                        AST *f = member;
                        if (symtable_insert(classScope, f->name, f->typeName?f->typeName:"<nil>", SYM_FUNC, f->lineno)) {
                            sem_error(f->lineno, "Function '%s' redeclared in class '%s'", f->name, node->name);
                        } else {
                            Symbol *fs = symtable_lookup(classScope, f->name);
                            // bind params into function symbol metadata
                            bind_function_params(fs, f->child); // child is paramlist
                        }
                    }
                    member = member->sibling;
                }
            }
            break;
        }
        case NODE_FUNC_DECL: {
            // insert function in current scope (global or class)
            if (symtable_insert(curScope, node->name, node->typeName?node->typeName:"<nil>", SYM_FUNC, node->lineno)) {
                sem_error(node->lineno, "Function '%s' redeclared in scope '%s'", node->name, curScope->scopeName?curScope->scopeName:"<global>");
            } else {
                // create function child scope and insert parameters & local vars
                SymTable *fnScope = symtable_create(node->name, curScope);
                // attach to end of scope chain for printing
                SymTable *t = curScope;
                while (t->next) t = t->next;
                t->next = fnScope;
                // insert parameters (node->child is param list)
                AST *p = node->child;
                Symbol *fs = symtable_lookup(curScope, node->name);
                bind_function_params(fs, p);
                // insert params into fnScope
                for (AST *pp = p; pp; pp = pp->sibling) {
                    if (symtable_insert(fnScope, pp->name, pp->typeName?pp->typeName:"<nil>", SYM_PARAM, pp->lineno)) {
                        sem_error(pp->lineno, "Parameter '%s' duplicated in function '%s'", pp->name, node->name);
                    }
                }
                // Process local var declarations in function body (node->extra = func body)
                AST *body = node->extra; // var/stmt list
                if (body) {
                    AST *st = body->child;
                    while (st) {
                        if (st->kind == NODE_VAR_DECL) {
                            if (symtable_insert(fnScope, st->name, st->typeName?st->typeName:"<nil>", SYM_VAR, st->lineno)) {
                                sem_error(st->lineno, "Local variable '%s' redeclared in function '%s'", st->name, node->name);
                            }
                        }
                        st = st->sibling;
                    }
                }
            }
            break;
        }
        case NODE_VAR_DECL: {
            // insert var in current scope
            if (symtable_insert(curScope, node->name, node->typeName?node->typeName:"<nil>", SYM_VAR, node->lineno)) {
                sem_error(node->lineno, "Variable '%s' redeclared in scope '%s'", node->name, curScope->scopeName?curScope->scopeName:"<global>");
            }
            break;
        }
        default:
            // traverse children for declarations
            if (node->child) passA_walk_list(curScope, node->child);
            break;
    }
    // handle sibling
    if (node->sibling) semantic_passA_build(curScope, node->sibling);
}

/* entry for Pass A */
void semantic_passA(AST *root) {
    // create global table
    globalTable = symtable_create("global", NULL);
    // attach globalTable as head of scope list
    // process top-level declarations in root->child
    AST *p = root->child;
    while (p) {
        semantic_passA_build(globalTable, p);
        p = p->sibling;
    }
}

/* ---- PASS B: semantic checks (basic) ---- */
const char *resolve_type_of_expr(SymTable *curScope, AST *expr);
void semantic_passB_check(SymTable *curScope, AST *node);

/* helper: resolve expression type (string) */
const char *resolve_type_of_expr(SymTable *curScope, AST *expr) {
    if (!expr) return "<void>";
    switch (expr->kind) {
        case NODE_INT_LITERAL: return "integer";
        case NODE_FLOAT_LITERAL: return "float";
        case NODE_ID: {
            Symbol *s = symtable_lookup(curScope, expr->name);
            if (!s) {
                sem_error(expr->lineno, "Identifier '%s' used before declaration", expr->name);
                return "<error>";
            }
            return s->typeName ? s->typeName : "<nil>";
        }
        case NODE_BINARY_OP: {
            const char *lt = resolve_type_of_expr(curScope, expr->child);
            const char *rt = resolve_type_of_expr(curScope, expr->child ? expr->child->sibling : NULL);
            // basic numeric check for arithmetic ops
            if (!expr->name) return "<error>";
            if (strcmp(expr->name, "+")==0 || strcmp(expr->name, "-")==0 || strcmp(expr->name, "*")==0 || strcmp(expr->name, "/")==0) {
                // Allow both int and float arithmetic
                int leftNumeric = (strcmp(lt, "int") == 0 || strcmp(lt, "integer") == 0 || strcmp(lt, "float") == 0);
                int rightNumeric = (strcmp(rt, "int") == 0 || strcmp(rt, "integer") == 0 || strcmp(rt, "float") == 0);

                if (leftNumeric && rightNumeric) {
                    // Mixed int/float → float result
                    if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0)
                        return "float";
                    return "int";
                } else {
                    sem_error(expr->lineno, "Arithmetic operands must be numeric (found %s and %s)", lt, rt);
                    return "<error>";
                }
            }
            // relational ops -> yields integer/boolean
            if (strcmp(expr->name,"==")==0 || strcmp(expr->name,"<>")==0 || strcmp(expr->name,"<")==0 || strcmp(expr->name,">")==0 ||
                strcmp(expr->name,"<=")==0 || strcmp(expr->name,">=")==0) {
                // allow numeric or same type comparisons
                if (strcmp(lt,rt)==0) return "integer";
                if ((strcmp(lt,"integer")==0 || strcmp(lt,"float")==0) && (strcmp(rt,"integer")==0 || strcmp(rt,"float")==0)) return "integer";
                sem_error(expr->lineno, "Incompatible types for relational operation (%s, %s)", lt, rt);
                return "<error>";
            }
            // logical ops: not implemented in detail
            return "<error>";
        }
        case NODE_FUNCTION_CALL: {
            Symbol *fn = symtable_lookup(curScope, expr->name);
            if (!fn || fn->kind != SYM_FUNC) {
                sem_error(expr->lineno, "Call to undefined function '%s'", expr->name);
                return "<error>";
            }
            // check args count and types
            // build param list from fn->params (linked list), but note our params stored reversed
            int argCount = 0, paramCount = 0;
            AST *arg = expr->child;
            Symbol *p = fn->params;
            // count args
            for (AST *a = arg; a; a = a->sibling) argCount++;
            for (Symbol *pp = p; pp; pp = pp->next) paramCount++;
            if (argCount != paramCount) {
                sem_error(expr->lineno, "Call to '%s' with wrong number of arguments (expected %d, got %d)", expr->name, paramCount, argCount);
                return fn->typeName ? fn->typeName : "<nil>";
            }
            // simple type checks (pairwise)
            AST *a = arg;
            Symbol *pp = p;
            while (a && pp) {
                const char *at = resolve_type_of_expr(curScope, a);
                if (strcmp(at, pp->typeName)!=0) {
                    sem_error(expr->lineno, "Argument type mismatch in call to '%s' (param %s expects %s, got %s)", expr->name, pp->name, pp->typeName?pp->typeName:"<nil>", at);
                }
                a = a->sibling; pp = pp->next;
            }
            return fn->typeName ? fn->typeName : "<nil>";
        }
        default:
            return "<nil>";
    }
}

void semantic_passB_check(SymTable *curScope, AST *node) {
    if (!node) return;
    switch (node->kind) {
        case NODE_ASSIGN: {
            AST *lhs = node->child;
            AST *rhs = lhs ? lhs->sibling : NULL;
            if (!lhs || lhs->kind != NODE_ID) {
                sem_error(node->lineno, "Left side of assignment must be an identifier");
            } else {
                const char *ltype = resolve_type_of_expr(curScope, lhs);
                const char *rtype = resolve_type_of_expr(curScope, rhs);
                if (strcmp(ltype,"<error>")==0 || strcmp(rtype,"<error>")==0) { /* error already reported */ }
                else if (strcmp(ltype, rtype) != 0) {
                    sem_error(node->lineno, "Type mismatch in assignment: left is %s, right is %s", ltype, rtype);
                }
            }
            break;
        }
        case NODE_FUNCTION_CALL:
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
        case NODE_ID:
            // resolve expression types to ensure identifiers exist
            (void)resolve_type_of_expr(curScope, node);
            break;
        case NODE_READ: {
            AST *v = node->child;
            if (!v || v->kind!=NODE_ID) {
                sem_error(node->lineno, "READ expects an identifier");
            } else {
                Symbol *s = symtable_lookup(curScope, v->name);
                if (!s) sem_error(v->lineno, "READ on undeclared variable '%s'", v->name);
            }
            break;
        }
        case NODE_RETURN: {
            AST *e = node->child;
            // We don't maintain current function's return type here; for demo skip check.
            (void)resolve_type_of_expr(curScope, e);
            break;
        }
        default:
            break;
    }
    // recurse into children (expressions, statements)
    for (AST *c = node->child; c; c = c->sibling) {
        semantic_passB_check(curScope, c);
    }
    // sibling
    if (node->sibling) semantic_passB_check(curScope, node->sibling);
}

/* entry for pass B: walk AST and check expressions/statements using global table as starting scope.
   In a complete compiler we would track current scope (class/function) while traversing.
*/
void semantic_passB(AST *root) {
    // open error file already opened by driver
    // naive approach: just walk root and check
    for (AST *p = root->child; p; p = p->sibling) {
        // for class members and functions we can set scope appropriately
        if (p->kind == NODE_CLASS_DECL) {
            // find class scope in globalTable->next chain
            SymTable *classScope = NULL;
            for (SymTable *t = globalTable->next; t; t = t->next) {
                if (t->scopeName && p->name && strcmp(t->scopeName, p->name)==0) { classScope = t; break; }
            }
            // check members using classScope
            AST *m = p->child;
            while (m) {
                if (m->kind == NODE_FUNC_DECL) {
                    // function: find its scope (by name)
                    SymTable *fnScope = NULL;
                    for (SymTable *t = classScope; t; t = t->next) {
                        if (t->scopeName && strcmp(t->scopeName, m->name)==0) { fnScope = t; break; }
                    }
                    // check function body expressions/statements using fnScope
                    if (m->extra) semantic_passB_check(fnScope?fnScope:classScope, m->extra);
                } else if (m->kind == NODE_ATTRIBUTE) {
                    // attributes have no bodies
                }
                m = m->sibling;
            }
        } else if (p->kind == NODE_FUNC_DECL) {
            // top-level function
            // find function scope in global->next chain
            SymTable *fnScope = NULL;
            for (SymTable *t = globalTable->next; t; t = t->next) {
                if (t->scopeName && p->name && strcmp(t->scopeName, p->name)==0) { fnScope = t; break; }
            }
            if (p->extra) semantic_passB_check(fnScope?fnScope:globalTable, p->extra);
        }
    }
}
