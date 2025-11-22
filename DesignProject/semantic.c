#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symbol_table.h"

/* globals */
SymTable *globalTable = NULL;
FILE *errFile = NULL;

/* --- simple structure to track duplicates --- */
#define MAX_ERRORS 1000
static char *errorMsgs[MAX_ERRORS];
static int errorCount = 0;

/* error handler */
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

/* pass A - build symbol table */
static void semantic_passA_build(SymTable *curScope, AST *node);

static AST *get_class_body(AST *classNode) {
    if (!classNode) return NULL;
    AST *first = classNode->child;
    if (first && first->kind == NODE_CLASS_INHERIT_LIST) return first->sibling;
    return first;
}

static int is_numeric_type(const char *typeName) {
    return typeName && (strcmp(typeName, "int") == 0 || strcmp(typeName, "float") == 0);
}

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
        case NODE_CLASS_DECL: {
            if (symtable_insert(curScope, node->name,
                                node->name,
                                SYM_CLASS, node->lineno)) {
                sem_error(node->lineno,
                          "Class '%s' redeclared in scope '%s'",
                          node->name,
                          curScope->scopeName ? curScope->scopeName : "<global>");
            } else {
                SymTable *classScope = symtable_create(node->name, curScope);
                symtable_register_scope(classScope);
                AST *body = get_class_body(node);
                if (body) passA_walk_list(classScope, body);
            }
            break;
        }
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
                symtable_register_scope(fnScope);

                AST *param = node->child;
                Symbol *funcSym = symtable_lookup(curScope, node->name);
                bind_function_params(funcSym, param);

                for (AST *pp = param; pp; pp = pp->sibling) {
                    if (symtable_insert(fnScope, pp->name,
                                        pp->typeName ? pp->typeName : "<nil>",
                                        SYM_PARAM, pp->lineno)) {
                        sem_error(pp->lineno,
                                  "Parameter '%s' duplicated in function '%s'",
                                  pp->name, node->name);
                    }
                }

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
        case NODE_ATTRIBUTE: {
            AST *var = node->child;
            if (var) {
                if (symtable_insert(curScope, var->name,
                                    var->typeName ? var->typeName : "<nil>",
                                    SYM_ATTR, var->lineno)) {
                    sem_error(var->lineno,
                              "Attribute '%s' redeclared in scope '%s'",
                              var->name,
                              curScope->scopeName ? curScope->scopeName : "<global>");
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
    symtable_registry_reset(globalTable);
    for (AST *p = root->child; p; p = p->sibling)
        semantic_passA_build(globalTable, p);
}

/* passB - semantic check */

static const char *resolve_type_of_expr(SymTable *curScope, AST *expr);

static const char *resolve_type_of_expr(SymTable *curScope, AST *expr) {
    if (!expr) return "<void>";

    switch (expr->kind) {
        case NODE_INT_LITERAL: return "int";
        case NODE_FLOAT_LITERAL: return "float";
        case NODE_STRING_LITERAL: return "string";

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
            if (strcmp(expr->name, "==") == 0 || strcmp(expr->name, "<>") == 0 ||
                strcmp(expr->name, "!=") == 0 ||
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
            if (strcmp(expr->name, "&&") == 0 || strcmp(expr->name, "||") == 0 ||
                strcmp(expr->name, "and") == 0 || strcmp(expr->name, "or") == 0) {
                if (strcmp(lt, "int") == 0 && strcmp(rt, "int") == 0)
                    return "int";
                sem_error(expr->lineno,
                          "Logical operands must be integers (found %s and %s)",
                          lt, rt);
                return "<error>";
            }

            return "<error>";
        }

        case NODE_UNARY_OP: {
            const char *operand = resolve_type_of_expr(curScope, expr->child);
            if (!expr->name) return operand;
            if (strcmp(expr->name, "not") == 0) {
                if (strcmp(operand, "int") == 0)
                    return "int";
                sem_error(expr->lineno,
                          "Operand of 'not' must be integer (found %s)",
                          operand);
                return "<error>";
            }
            if (strcmp(expr->name, "+") == 0 || strcmp(expr->name, "-") == 0) {
                if (is_numeric_type(operand))
                    return operand;
                sem_error(expr->lineno,
                          "Unary %s expects numeric operand (found %s)",
                          expr->name, operand);
                return "<error>";
            }
            return operand;
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

static void semantic_passB_visit(AST *node, SymTable *scope, const char *currentReturn);

static void check_assignment(AST *node, SymTable *scope) {
            AST *lhs = node->child;
            AST *rhs = lhs ? lhs->sibling : NULL;

            if (!lhs || lhs->kind != NODE_ID) {
        sem_error(node->lineno, "Left side of assignment must be an identifier");
        return;
    }

    const char *lt = resolve_type_of_expr(scope, lhs);
    const char *rt = resolve_type_of_expr(scope, rhs);
    if (strcmp(lt, "<error>") == 0 || strcmp(rt, "<error>") == 0)
        return;

                    if (strcmp(lt, rt) != 0) {
        if (!(strcmp(lt, "float") == 0 && strcmp(rt, "int") == 0)) {
                            sem_error(node->lineno,
                                      "Type mismatch in assignment: left is %s, right is %s",
                                      lt, rt);
                        }
                    }
                }

static void check_condition(AST *condNode, SymTable *scope, const char *keyword) {
    const char *t = resolve_type_of_expr(scope, condNode);
    if (!is_numeric_type(t))
        sem_error(condNode ? condNode->lineno : 0,
                  "%s condition must be numeric (found %s)",
                  keyword, t);
}

static void semantic_passB_visit(AST *node, SymTable *scope, const char *currentReturn) {
    for (AST *p = node; p; p = p->sibling) {
        if (!p) continue;
        switch (p->kind) {
            case NODE_CLASS_DECL: {
                SymTable *classScope = symtable_find_scope(globalTable, p->name, scope);
                AST *body = get_class_body(p);
                semantic_passB_visit(body, classScope ? classScope : scope, currentReturn);
                continue;
            }
            case NODE_FUNC_DECL: {
                SymTable *fnScope = symtable_find_scope(globalTable, p->name, scope);
                const char *fnReturn = p->typeName ? p->typeName : "void";
                if (p->extra)
                    semantic_passB_visit(p->extra, fnScope ? fnScope : scope, fnReturn);
                continue;
            }
            case NODE_FUNC_BODY: {
                semantic_passB_visit(p->child, scope, currentReturn);
                continue;
            }
            case NODE_ASSIGN:
                check_assignment(p, scope);
            break;
        case NODE_READ: {
                AST *v = p->child;
            if (!v || v->kind != NODE_ID)
                    sem_error(p->lineno, "READ expects an identifier");
                else if (!symtable_lookup(scope, v->name))
                sem_error(v->lineno, "READ on undeclared variable '%s'", v->name);
            break;
        }
            case NODE_WRITE:
                (void)resolve_type_of_expr(scope, p->child);
                break;
        case NODE_RETURN: {
                const char *exprType = resolve_type_of_expr(scope, p->child);
                if (!currentReturn) {
                    sem_error(p->lineno, "RETURN outside of a function");
                } else if (strcmp(currentReturn, "void") == 0) {
                    if (exprType && strcmp(exprType, "<nil>") != 0 && strcmp(exprType, "<void>") != 0)
                        sem_error(p->lineno, "Void functions should not return a value");
                } else if (strcmp(exprType, currentReturn) != 0) {
                    if (!(strcmp(currentReturn, "float") == 0 && strcmp(exprType, "int") == 0)) {
                        sem_error(p->lineno,
                                  "Return type mismatch: expected %s, got %s",
                                  currentReturn, exprType);
                    }
                }
            break;
        }
            case NODE_IF: {
                AST *cond = p->child;
                check_condition(cond, scope, "IF");
                AST *thenBlock = cond ? cond->sibling : NULL;
                AST *elseBlock = thenBlock ? thenBlock->sibling : NULL;
                semantic_passB_visit(thenBlock, scope, currentReturn);
                semantic_passB_visit(elseBlock, scope, currentReturn);
                continue;
            }
            case NODE_WHILE: {
                AST *cond = p->child;
                check_condition(cond, scope, "WHILE");
                AST *body = cond ? cond->sibling : NULL;
                semantic_passB_visit(body, scope, currentReturn);
                continue;
            }
        case NODE_FUNCTION_CALL:
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
        case NODE_ID:
                (void)resolve_type_of_expr(scope, p);
            break;
        default:
            break;
    }

        if (p->child)
            semantic_passB_visit(p->child, scope, currentReturn);
        if (p->extra)
            semantic_passB_visit(p->extra, scope, currentReturn);
    }
}

void semantic_passB(AST *root) {
    semantic_passB_visit(root, globalTable, NULL);
        }

int semantic_error_total(void) {
    return errorCount;
}
