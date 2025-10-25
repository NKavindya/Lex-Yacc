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
        symtable_add_param(funcSym, p->name, p->typeName?p->typeName:"<nil>", p->lineno);
        p = p->sibling;
    }
}

void semantic_passA_build(SymTable *curScope, AST *node) {
    if (!node) return;
    switch (node->kind) {
        case NODE_CLASS_DECL: {
            if (symtable_insert(globalTable, node->name, node->name, SYM_CLASS, node->lineno)) {
                sem_error(node->lineno, "Class '%s' redeclared", node->name);
            } else {
                SymTable *classScope = symtable_create(node->name, globalTable);
                SymTable *t = globalTable;
                while (t->next) t = t->next;
                t->next = classScope;

                AST *member = node->child;
                while (member) {
                    if (member->kind == NODE_ATTRIBUTE) {
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
                            bind_function_params(fs, f->child);
                        }
                    }
                    member = member->sibling;
                }
            }
            break;
        }
        case NODE_FUNC_DECL: {
            if (symtable_insert(curScope, node->name, node->typeName?node->typeName:"<nil>", SYM_FUNC, node->lineno)) {
                sem_error(node->lineno, "Function '%s' redeclared in scope '%s'", node->name, curScope->scopeName?curScope->scopeName:"<global>");
            } else {
                SymTable *fnScope = symtable_create(node->name, curScope);
                SymTable *t = curScope;
                while (t->next) t = t->next;
                t->next = fnScope;

                AST *p = node->child;
                Symbol *fs = symtable_lookup(curScope, node->name);
                bind_function_params(fs, p);

                for (AST *pp = p; pp; pp = pp->sibling) {
                    if (symtable_insert(fnScope, pp->name, pp->typeName?pp->typeName:"<nil>", SYM_PARAM, pp->lineno)) {
                        sem_error(pp->lineno, "Parameter '%s' duplicated in function '%s'", pp->name, node->name);
                    }
                }

                AST *body = node->extra;
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
            if (symtable_insert(curScope, node->name, node->typeName?node->typeName:"<nil>", SYM_VAR, node->lineno)) {
                sem_error(node->lineno, "Variable '%s' redeclared in scope '%s'", node->name, curScope->scopeName?curScope->scopeName:"<global>");
            }
            break;
        }
        default:
            if (node->child) passA_walk_list(curScope, node->child);
            break;
    }
    if (node->sibling) semantic_passA_build(curScope, node->sibling);
}

void semantic_passA(AST *root) {
    globalTable = symtable_create("global", NULL);
    AST *p = root->child;
    while (p) {
        semantic_passA_build(globalTable, p);
        p = p->sibling;
    }
}

/* ---- PASS B: semantic checks (expressions/statements) ---- */
const char *resolve_type_of_expr(SymTable *curScope, AST *expr);
void semantic_passB_check(SymTable *curScope, AST *node);

const char *resolve_type_of_expr(SymTable *curScope, AST *expr) {
    if (!expr) return "<void>";
    switch (expr->kind) {
        case NODE_INT_LITERAL: return "integer";
        case NODE_FLOAT_LITERAL: return "float";
        case NODE_ID: {
            Symbol *s = symtable_lookup(curScope, expr->name);
            if (!s) { sem_error(expr->lineno, "Identifier '%s' used before declaration", expr->name); return "<error>"; }
            return s->typeName ? s->typeName : "<nil>";
        }
        case NODE_BINARY_OP: {
            const char *lt = resolve_type_of_expr(curScope, expr->child);
            const char *rt = resolve_type_of_expr(curScope, expr->child ? expr->child->sibling : NULL);

            if (!expr->name) return "<error>";
            if (strcmp(expr->name,"+")==0 || strcmp(expr->name,"-")==0 || strcmp(expr->name,"*")==0 || strcmp(expr->name,"/")==0) {
                int leftNumeric = (strcmp(lt,"int")==0 || strcmp(lt,"integer")==0 || strcmp(lt,"float")==0);
                int rightNumeric = (strcmp(rt,"int")==0 || strcmp(rt,"integer")==0 || strcmp(rt,"float")==0);
                if (leftNumeric && rightNumeric) {
                    if (strcmp(lt,"float")==0 || strcmp(rt,"float")==0) return "float";
                    return "int";
                } else {
                    sem_error(expr->lineno, "Arithmetic operands must be numeric (found %s and %s)", lt, rt);
                    return "<error>";
                }
            }
            if (strcmp(expr->name,"==")==0 || strcmp(expr->name,"<>")==0 || strcmp(expr->name,"<")==0 ||
                strcmp(expr->name,">")==0 || strcmp(expr->name,"<=")==0 || strcmp(expr->name,">=")==0) {
                if (strcmp(lt,rt)==0) return "integer";
                if ((strcmp(lt,"integer")==0 || strcmp(lt,"float")==0) && (strcmp(rt,"integer")==0 || strcmp(rt,"float")==0)) return "integer";
                sem_error(expr->lineno, "Incompatible types for relational operation (%s, %s)", lt, rt);
                return "<error>";
            }
            if (strcmp(expr->name,"&&")==0 || strcmp(expr->name,"||")==0) {
                if (strcmp(lt,"integer")==0 && strcmp(rt,"integer")==0) return "integer";
                sem_error(expr->lineno, "Logical operands must be integers (found %s and %s)", lt, rt);
                return "<error>";
            }
            return "<error>";
        }
        case NODE_UNARY_OP: {
            const char *opType = resolve_type_of_expr(curScope, expr->child);
            if (strcmp(expr->name,"-")==0) {
                if (strcmp(opType,"integer")==0 || strcmp(opType,"float")==0) return opType;
                sem_error(expr->lineno, "Unary '-' operand must be numeric");
                return "<error>";
            }
            return "<error>";
        }
        case NODE_FUNCTION_CALL: {
            Symbol *fn = symtable_lookup(curScope, expr->name);
            if (!fn || fn->kind != SYM_FUNC) { sem_error(expr->lineno,"Call to undefined function '%s'", expr->name); return "<error>"; }

            int argCount=0,paramCount=0;
            AST *arg=expr->child;
            Symbol *p=fn->params;
            for (AST *a=arg;a;a=a->sibling) argCount++;
            for (Symbol *pp=p;pp;pp=pp->next) paramCount++;
            if (argCount!=paramCount) sem_error(expr->lineno,"Call to '%s' with wrong number of arguments (expected %d, got %d)", expr->name,paramCount,argCount);

            AST *a=arg; Symbol *pp=p;
            while(a && pp){
                const char *at=resolve_type_of_expr(curScope,a);
                if (strcmp(at,pp->typeName)!=0) sem_error(expr->lineno,"Argument type mismatch in call to '%s' (param %s expects %s, got %s)", expr->name,pp->name,pp->typeName?pp->typeName:"<nil>",at);
                a=a->sibling; pp=pp->next;
            }
            return fn->typeName?fn->typeName:"<nil>";
        }
        default:
            return "<nil>";
    }
}

void semantic_passB_check(SymTable *curScope, AST *node) {
    if (!node) return;
    switch (node->kind) {
        case NODE_ASSIGN: {
            AST *lhs=node->child; AST *rhs=lhs?lhs->sibling:NULL;
            if (!lhs || lhs->kind!=NODE_ID) sem_error(node->lineno,"Left side of assignment must be an identifier");
            else {
                const char *ltype=resolve_type_of_expr(curScope,lhs);
                const char *rtype=resolve_type_of_expr(curScope,rhs);
                if (strcmp(ltype,"<error>")!=0 && strcmp(rtype,"<error>")!=0 && strcmp(ltype,rtype)!=0)
                    sem_error(node->lineno,"Type mismatch in assignment: left is %s, right is %s",ltype,rtype);
            }
            break;
        }
        case NODE_FUNCTION_CALL:
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
        case NODE_ID:
            (void)resolve_type_of_expr(curScope,node);
            break;
        case NODE_READ: {
            AST *v=node->child;
            if (!v||v->kind!=NODE_ID) sem_error(node->lineno,"READ expects an identifier");
            else if (!symtable_lookup(curScope,v->name)) sem_error(v->lineno,"READ on undeclared variable '%s'",v->name);
            break;
        }
        case NODE_RETURN: {
            AST *e=node->child;
            (void)resolve_type_of_expr(curScope,e);
            break;
        }
        default: break;
    }
    for (AST *c=node->child;c;c=c->sibling) semantic_passB_check(curScope,c);
    if (node->sibling) semantic_passB_check(curScope,node->sibling);
}

void semantic_passB(AST *root) {
    for (AST *p=root->child;p;p=p->sibling){
        if (p->kind==NODE_CLASS_DECL){
            SymTable *classScope=NULL;
            for(SymTable *t=globalTable->next;t;t=t->next) if(t->scopeName&&strcmp(t->scopeName,p->name)==0){classScope=t;break;}
            AST *m=p->child;
            while(m){
                if(m->kind==NODE_FUNC_DECL){
                    SymTable *fnScope=NULL;
                    for(SymTable *t=classScope;t;t=t->next) if(t->scopeName&&strcmp(t->scopeName,m->name)==0){fnScope=t;break;}
                    if(m->extra) semantic_passB_check(fnScope?fnScope:classScope,m->extra);
                }
                m=m->sibling;
            }
        } else if (p->kind==NODE_FUNC_DECL){
            SymTable *fnScope=NULL;
            for(SymTable *t=globalTable->next;t;t=t->next) if(t->scopeName&&strcmp(t->scopeName,p->name)==0){fnScope=t;break;}
            if(p->extra) semantic_passB_check(fnScope?fnScope:globalTable,p->extra);
        }
    }
}
