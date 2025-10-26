#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symbol_table.h"

/* =====================================================
   GLOBALS
   ===================================================== */
SymTable *globalTable = NULL;
FILE *errFile = NULL;

/* --- simple structure to track duplicates --- */
#define MAX_ERRORS 1000
static char *errorMsgs[MAX_ERRORS];
static int errorCount = 0;

/* =====================================================
   ERROR HANDLER
   ===================================================== */
static int already_reported(const char *msg) {
    for (int i = 0; i < errorCount; i++) {
        if (strcmp(errorMsgs[i], msg) == 0) return 1;
    }
    return 0;
}

static void sem_error(int lineno, const char *fmt, ...) {
    char buffer[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    char fullMsg[1200];
    snprintf(fullMsg, sizeof(fullMsg), "Semantic Error [line %d]: %s", lineno, buffer);

    if (!already_reported(fullMsg)) {
        if (errorCount < MAX_ERRORS)
            errorMsgs[errorCount++] = strdup(fullMsg);

        if (errFile)
            fprintf(errFile, "%s\n", fullMsg);
        else
            fprintf(stdout, "%s\n", fullMsg);
    }
}

/* =====================================================
   PASS A — BUILD SYMBOL TABLES
   ===================================================== */
static void semantic_passA_build(SymTable *curScope, AST *node);

static void passA_walk_list(SymTable *curScope, AST *list) {
    for (AST *p = list; p; p = p->sibling) {
        semantic_passA_build(curScope, p);
    }
}

static void bind_function_params(Symbol *funcSym, AST *paramList) {
    for (AST *p = paramList; p; p = p->sibling) {
        symtable_add_param(funcSym, p->name,
            p->typeName ? p->typeName : "<nil>", p->lineno);
    }
}

static void semantic_passA_build(SymTable *curScope, AST *node) {
    if (!node) return;

    switch (node->kind) {
        case NODE_FUNC_DECL: {
            if (symtable_insert(curScope, node->name,
                                node->typeName ? node->typeName : "<nil>",
                                SYM_FUNC, node->lineno)) {
                sem_error(node->lineno,
                          "Function '%s' redeclared in scope '%s'",
                          node->name,
                          curScope->scopeName ? curScope->scopeName : "<global>");
            } else {
                SymTable *fnScope = symtable_create(node->name, curScope);
                SymTable *t = curScope;
                while (t->next) t = t->next;
                t->next = fnScope;

                AST *param = node->child;
                Symbol *funcSym = symtable_lookup(curScope, node->name);
                bind_function_params(funcSym, param);

                /* parameter declarations */
                for (AST *pp = param; pp; pp = pp->sibling) {
                    if (symtable_insert(fnScope, pp->name,
                                        pp->typeName ? pp->typeName : "<nil>",
                                        SYM_PARAM, pp->lineno)) {
                        sem_error(pp->lineno,
                                  "Parameter '%s' duplicated in function '%s'",
                                  pp->name, node->name);
                    }
                }

                /* local vars in function body */
                AST *body = node->extra;
                if (body && body->child) {
                    for (AST *st = body->child; st; st = st->sibling) {
                        if (st->kind == NODE_VAR_DECL) {
                            if (symtable_insert(fnScope, st->name,
                                                st->typeName ? st->typeName : "<nil>",
                                                SYM_VAR, st->lineno)) {
                                sem_error(st->lineno,
                                          "Local variable '%s' redeclared in function '%s'",
                                          st->name, node->name);
                            }
                        }
                    }
                }
            }
            break;
        }
        case NODE_VAR_DECL: {
            if (symtable_insert(curScope, node->name,
                                node->typeName ? node->typeName : "<nil>",
                                SYM_VAR, node->lineno)) {
                sem_error(node->lineno,
                          "Variable '%s' redeclared in scope '%s'",
                          node->name,
                          curScope->scopeName ? curScope->scopeName : "<global>");
            }
            break;
        }
        default:
            if (node->child)
                passA_walk_list(curScope, node->child);
            break;
    }

    if (node->sibling)
        semantic_passA_build(curScope, node->sibling);
}

void semantic_passA(AST *root) {
    globalTable = symtable_create("global", NULL);
    for (AST *p = root->child; p; p = p->sibling)
        semantic_passA_build(globalTable, p);
}

/* =====================================================
   PASS B — SEMANTIC CHECKS
   ===================================================== */

static const char *resolve_type_of_expr(SymTable *curScope, AST *expr);

static const char *resolve_type_of_expr(SymTable *curScope, AST *expr) {
    if (!expr) return "<void>";

    switch (expr->kind) {
        case NODE_INT_LITERAL: return "int";
        case NODE_FLOAT_LITERAL: return "float";

        /* fallback for string literals if not defined */
#ifdef NODE_STRING_LITERAL
        case NODE_STRING_LITERAL: return "string";
#endif

        case NODE_ID: {
            Symbol *s = symtable_lookup(curScope, expr->name);
            if (!s) {
                sem_error(expr->lineno,
                          "Identifier '%s' used before declaration",
                          expr->name);
                return "<error>";
            }
            return s->typeName ? s->typeName : "<nil>";
        }

        case NODE_BINARY_OP: {
            const char *lt = resolve_type_of_expr(curScope, expr->child);
            const char *rt = resolve_type_of_expr(curScope,
                                                  expr->child ? expr->child->sibling : NULL);

            if (!expr->name) return "<error>";

            /* arithmetic */
            if (strcmp(expr->name, "+") == 0 ||
                strcmp(expr->name, "-") == 0 ||
                strcmp(expr->name, "*") == 0 ||
                strcmp(expr->name, "/") == 0) {

                int leftNum = (strcmp(lt, "int") == 0 || strcmp(lt, "float") == 0);
                int rightNum = (strcmp(rt, "int") == 0 || strcmp(rt, "float") == 0);
                if (leftNum && rightNum) {
                    // if either is float, promote result to float
                    if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0)
                        return "float";
                    return "int";
                } else {
                    sem_error(expr->lineno,
                              "Arithmetic operands must be numeric (found %s and %s)",
                              lt, rt);
                    return "<error>";
                }
            }

            /* relational */
            if (strcmp(expr->name, "==") == 0 || strcmp(expr->name, "!=") == 0 ||
                strcmp(expr->name, "<") == 0 || strcmp(expr->name, ">") == 0 ||
                strcmp(expr->name, "<=") == 0 || strcmp(expr->name, ">=") == 0) {
                if (strcmp(lt, rt) == 0 ||
                    ((strcmp(lt, "int") == 0 || strcmp(lt, "float") == 0) &&
                     (strcmp(rt, "int") == 0 || strcmp(rt, "float") == 0)))
                    return "int";
                sem_error(expr->lineno,
                          "Incompatible types for relational operation (%s, %s)",
                          lt, rt);
                return "<error>";
            }

            /* logical */
            if (strcmp(expr->name, "&&") == 0 || strcmp(expr->name, "||") == 0) {
                if (strcmp(lt, "int") == 0 && strcmp(rt, "int") == 0)
                    return "int";
                sem_error(expr->lineno,
                          "Logical operands must be integers (found %s and %s)",
                          lt, rt);
                return "<error>";
            }

            return "<error>";
        }

        case NODE_FUNCTION_CALL: {
            Symbol *fn = symtable_lookup(curScope, expr->name);
            if (!fn || fn->kind != SYM_FUNC) {
                sem_error(expr->lineno,
                          "Call to undefined function '%s'", expr->name);
                return "<error>";
            }

            /* argument vs parameter checking */
            int argCount = 0, paramCount = 0;
            for (AST *a = expr->child; a; a = a->sibling) argCount++;
            for (Symbol *p = fn->params; p; p = p->next) paramCount++;

            if (argCount != paramCount) {
                sem_error(expr->lineno,
                          "Call to '%s' with wrong number of arguments "
                          "(expected %d, got %d)",
                          expr->name, paramCount, argCount);
            }

            AST *a = expr->child;
            Symbol *pp = fn->params;
            while (a && pp) {
                const char *atype = resolve_type_of_expr(curScope, a);
                if (strcmp(atype, pp->typeName) != 0)
                    sem_error(expr->lineno,
                              "Argument type mismatch in call to '%s' "
                              "(param %s expects %s, got %s)",
                              expr->name, pp->name,
                              pp->typeName ? pp->typeName : "<nil>", atype);
                a = a->sibling;
                pp = pp->next;
            }

            return fn->typeName ? fn->typeName : "<nil>";
        }

        default:
            return "<nil>";
    }
}

/* ----------------------------------------------------- */

static void semantic_passB_check(SymTable *curScope, AST *node) {
    if (!node) return;

    switch (node->kind) {
        case NODE_ASSIGN: {
            AST *lhs = node->child;
            AST *rhs = lhs ? lhs->sibling : NULL;

            if (!lhs || lhs->kind != NODE_ID) {
                sem_error(node->lineno,
                          "Left side of assignment must be an identifier");
            } else {
                const char *lt = resolve_type_of_expr(curScope, lhs);
                const char *rt = resolve_type_of_expr(curScope, rhs);
                if (strcmp(lt, "<error>") != 0 && strcmp(rt, "<error>") != 0) {
                    // allow int -> float promotion
                    if (strcmp(lt, rt) != 0) {
                        if (strcmp(lt, "float") == 0 && strcmp(rt, "int") == 0) {
                            // ok: int promoted to float
                        } else {
                            sem_error(node->lineno,
                                      "Type mismatch in assignment: left is %s, right is %s",
                                      lt, rt);
                        }
                    }
                }
            }
            break;
        }

        case NODE_READ: {
            AST *v = node->child;
            if (!v || v->kind != NODE_ID)
                sem_error(node->lineno, "READ expects an identifier");
            else if (!symtable_lookup(curScope, v->name))
                sem_error(v->lineno, "READ on undeclared variable '%s'", v->name);
            break;
        }

        case NODE_RETURN: {
            (void)resolve_type_of_expr(curScope, node->child);
            break;
        }

        case NODE_FUNCTION_CALL:
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
        case NODE_ID:
            (void)resolve_type_of_expr(curScope, node);
            break;

        default:
            break;
    }

    for (AST *c = node->child; c; c = c->sibling)
        semantic_passB_check(curScope, c);
}

void semantic_passB(AST *root) {
    for (AST *p = root->child; p; p = p->sibling) {
        if (p->kind == NODE_FUNC_DECL) {
            SymTable *fnScope = NULL;
            for (SymTable *t = globalTable->next; t; t = t->next)
                if (t->scopeName && strcmp(t->scopeName, p->name) == 0) {
                    fnScope = t;
                    break;
                }
            if (p->extra)
                semantic_passB_check(fnScope ? fnScope : globalTable, p->extra);
        }
    }
}
