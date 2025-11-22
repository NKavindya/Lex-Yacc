#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define REG_POOL 7
#define WORD_SIZE 8

static const char *REG_NAMES[REG_POOL] = {"R1","R2","R3","R4","R5","R6","R7"};

typedef struct {
    FILE *out;
    int available[REG_POOL];
    int labelCounter;
} CodeGenContext;

typedef struct {
    CodeGenContext *cg;
    SymTable *scope;
    char funcName[64];
    char endLabel[64];
} FunctionContext;

static void cg_emit(CodeGenContext *cg, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(cg->out, fmt, ap);
    va_end(ap);
}

static void cg_init(CodeGenContext *cg, FILE *out) {
    cg->out = out;
    for (int i = 0; i < REG_POOL; ++i) cg->available[i] = 1;
    cg->labelCounter = 0;
}

static const char *reg_name(int idx) {
    if (idx < 0 || idx >= REG_POOL) return "R?";
    return REG_NAMES[idx];
}

static int cg_alloc_reg(CodeGenContext *cg) {
    for (int i = 0; i < REG_POOL; ++i) {
        if (cg->available[i]) {
            cg->available[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "[codegen] register exhaustion\n");
    exit(1);
}

static void cg_free_reg(CodeGenContext *cg, int reg) {
    if (reg < 0 || reg >= REG_POOL) return;
    cg->available[reg] = 1;
}

static void cg_make_label(CodeGenContext *cg, char *buffer, size_t len, const char *prefix) {
    snprintf(buffer, len, "%s_%03d", prefix, cg->labelCounter++);
}

static Symbol *cg_lookup(FunctionContext *fn, const char *name) {
    return symtable_lookup(fn->scope, name);
}

static void cg_store_var(FunctionContext *fn, Symbol *sym, int reg) {
    if (!sym) return;
    if (sym->offset >= 0 && sym->kind != SYM_CLASS && sym->kind != SYM_FUNC) {
        cg_emit(fn->cg, "    STORE %s, [FP-%d]    ; %s\n", reg_name(reg), sym->offset, sym->name);
    } else {
        cg_emit(fn->cg, "    STORE %s, [%s]    ; %s\n", reg_name(reg), sym->name ? sym->name : "tmp", sym->name ? sym->name : "");
    }
}

static int cg_load_var(FunctionContext *fn, Symbol *sym) {
    int reg = cg_alloc_reg(fn->cg);
    if (!sym) {
        cg_emit(fn->cg, "    LOADI %s, 0\n", reg_name(reg));
        return reg;
    }
    if (sym->offset >= 0 && sym->kind != SYM_CLASS && sym->kind != SYM_FUNC) {
        cg_emit(fn->cg, "    LOAD %s, [FP-%d]    ; %s\n", reg_name(reg), sym->offset, sym->name);
    } else {
        cg_emit(fn->cg, "    LOAD %s, [%s]    ; %s\n", reg_name(reg), sym->name ? sym->name : "tmp", sym->name ? sym->name : "");
    }
    return reg;
}

static int cg_generate_expr(FunctionContext *fn, AST *expr);
static void cg_generate_statement(FunctionContext *fn, AST *stmt);

static void cg_generate_block(FunctionContext *fn, AST *list) {
    for (AST *node = list; node; node = node->sibling) {
        switch (node->kind) {
            case NODE_VAR_DECL:
            case NODE_ATTRIBUTE:
                /* storage already reserved */
                break;
            default:
                cg_generate_statement(fn, node);
                break;
        }
    }
}

static void cg_emit_binary(FunctionContext *fn, const char *mnemonic, int lhsReg, int rhsReg) {
    cg_emit(fn->cg, "    %s %s, %s, %s\n", mnemonic, reg_name(lhsReg), reg_name(lhsReg), reg_name(rhsReg));
    cg_free_reg(fn->cg, rhsReg);
}

static int cg_generate_function_call(FunctionContext *fn, AST *call) {
    int argCount = 0;
    for (AST *arg = call->child; arg; arg = arg->sibling) {
        int r = cg_generate_expr(fn, arg);
        cg_emit(fn->cg, "    PUSH %s\n", reg_name(r));
        cg_free_reg(fn->cg, r);
        argCount++;
    }
    cg_emit(fn->cg, "    CALL FUNC_%s\n", call->name ? call->name : "anon");
    if (argCount > 0)
        cg_emit(fn->cg, "    ADD SP, SP, #%d\n", argCount * WORD_SIZE);
    int target = cg_alloc_reg(fn->cg);
    cg_emit(fn->cg, "    MOV %s, R0\n", reg_name(target));
    return target;
}

static int cg_generate_expr(FunctionContext *fn, AST *expr) {
    if (!expr) {
        int r = cg_alloc_reg(fn->cg);
        cg_emit(fn->cg, "    LOADI %s, 0\n", reg_name(r));
        return r;
    }

    switch (expr->kind) {
        case NODE_ID: {
            Symbol *sym = cg_lookup(fn, expr->name);
            return cg_load_var(fn, sym);
        }
        case NODE_INT_LITERAL: {
            int r = cg_alloc_reg(fn->cg);
            cg_emit(fn->cg, "    LOADI %s, %d\n", reg_name(r), expr->intValue);
            return r;
        }
        case NODE_FLOAT_LITERAL: {
            int r = cg_alloc_reg(fn->cg);
            cg_emit(fn->cg, "    LOADF %s, %f\n", reg_name(r), expr->floatValue);
            return r;
        }
        case NODE_STRING_LITERAL: {
            int r = cg_alloc_reg(fn->cg);
            cg_emit(fn->cg, "    LOAD_STR %s, \"%s\"\n", reg_name(r), expr->name ? expr->name : "");
            return r;
        }
        case NODE_BINARY_OP: {
            int left = cg_generate_expr(fn, expr->child);
            int right = cg_generate_expr(fn, expr->child ? expr->child->sibling : NULL);
            const char *op = expr->name ? expr->name : "";
            if (strcmp(op, "+") == 0) cg_emit_binary(fn, "ADD", left, right);
            else if (strcmp(op, "-") == 0) cg_emit_binary(fn, "SUB", left, right);
            else if (strcmp(op, "*") == 0) cg_emit_binary(fn, "MUL", left, right);
            else if (strcmp(op, "/") == 0) cg_emit_binary(fn, "DIV", left, right);
            else if (strcmp(op, "and") == 0 || strcmp(op, "&&") == 0) cg_emit_binary(fn, "AND", left, right);
            else if (strcmp(op, "or") == 0 || strcmp(op, "||") == 0) cg_emit_binary(fn, "OR", left, right);
            else if (strcmp(op, "==") == 0) cg_emit_binary(fn, "CMPEQ", left, right);
            else if (strcmp(op, "<>") == 0 || strcmp(op, "!=") == 0) cg_emit_binary(fn, "CMPNE", left, right);
            else if (strcmp(op, "<") == 0) cg_emit_binary(fn, "CMPLT", left, right);
            else if (strcmp(op, ">") == 0) cg_emit_binary(fn, "CMPGT", left, right);
            else if (strcmp(op, "<=") == 0) cg_emit_binary(fn, "CMPLE", left, right);
            else if (strcmp(op, ">=") == 0) cg_emit_binary(fn, "CMPGE", left, right);
            else cg_emit_binary(fn, "ADD", left, right);
            return left;
        }
        case NODE_UNARY_OP: {
            int inner = cg_generate_expr(fn, expr->child);
            const char *op = expr->name ? expr->name : "";
            if (strcmp(op, "not") == 0) {
                cg_emit(fn->cg, "    NOT %s, %s\n", reg_name(inner), reg_name(inner));
            } else if (strcmp(op, "-") == 0) {
                cg_emit(fn->cg, "    NEG %s, %s\n", reg_name(inner), reg_name(inner));
            }
            return inner;
        }
        case NODE_FUNCTION_CALL:
            return cg_generate_function_call(fn, expr);
        default:
            break;
    }

    int r = cg_alloc_reg(fn->cg);
    cg_emit(fn->cg, "    LOADI %s, 0\n", reg_name(r));
    return r;
}

static void cg_generate_if(FunctionContext *fn, AST *node) {
    char elseLabel[64], endLabel[64];
    cg_make_label(fn->cg, elseLabel, sizeof(elseLabel), "L_if_else");
    cg_make_label(fn->cg, endLabel, sizeof(endLabel), "L_if_end");

    int condReg = cg_generate_expr(fn, node->child);
    cg_emit(fn->cg, "    JZ %s, %s\n", reg_name(condReg), elseLabel);
    cg_free_reg(fn->cg, condReg);

    AST *thenBlock = node->child ? node->child->sibling : NULL;
    AST *elseBlock = thenBlock ? thenBlock->sibling : NULL;
    cg_generate_block(fn, thenBlock);
    cg_emit(fn->cg, "    JMP %s\n", endLabel);

    cg_emit(fn->cg, "%s:\n", elseLabel);
    cg_generate_block(fn, elseBlock);

    cg_emit(fn->cg, "%s:\n", endLabel);
}

static void cg_generate_while(FunctionContext *fn, AST *node) {
    char topLabel[64], endLabel[64];
    cg_make_label(fn->cg, topLabel, sizeof(topLabel), "L_while_top");
    cg_make_label(fn->cg, endLabel, sizeof(endLabel), "L_while_end");

    cg_emit(fn->cg, "%s:\n", topLabel);
    int condReg = cg_generate_expr(fn, node->child);
    cg_emit(fn->cg, "    JZ %s, %s\n", reg_name(condReg), endLabel);
    cg_free_reg(fn->cg, condReg);

    AST *body = node->child ? node->child->sibling : NULL;
    cg_generate_block(fn, body);
    cg_emit(fn->cg, "    JMP %s\n", topLabel);
    cg_emit(fn->cg, "%s:\n", endLabel);
}

static void cg_generate_statement(FunctionContext *fn, AST *stmt) {
    if (!stmt) return;
    switch (stmt->kind) {
        case NODE_ASSIGN: {
            AST *lhs = stmt->child;
            AST *rhs = lhs ? lhs->sibling : NULL;
            Symbol *sym = cg_lookup(fn, lhs ? lhs->name : NULL);
            int r = cg_generate_expr(fn, rhs);
            cg_store_var(fn, sym, r);
            cg_free_reg(fn->cg, r);
            break;
        }
        case NODE_IF:
            cg_generate_if(fn, stmt);
            break;
        case NODE_WHILE:
            cg_generate_while(fn, stmt);
            break;
        case NODE_READ: {
            AST *id = stmt->child;
            Symbol *sym = cg_lookup(fn, id ? id->name : NULL);
            int r = cg_alloc_reg(fn->cg);
            cg_emit(fn->cg, "    READ %s\n", reg_name(r));
            cg_store_var(fn, sym, r);
            cg_free_reg(fn->cg, r);
            break;
        }
        case NODE_WRITE: {
            int r = cg_generate_expr(fn, stmt->child);
            cg_emit(fn->cg, "    WRITE %s\n", reg_name(r));
            cg_free_reg(fn->cg, r);
            break;
        }
        case NODE_RETURN: {
            int r = cg_generate_expr(fn, stmt->child);
            cg_emit(fn->cg, "    MOV R0, %s\n", reg_name(r));
            cg_free_reg(fn->cg, r);
            cg_emit(fn->cg, "    JMP %s\n", fn->endLabel);
            break;
        }
        case NODE_FUNCTION_CALL: {
            int r = cg_generate_function_call(fn, stmt);
            cg_free_reg(fn->cg, r);
            break;
        }
        default:
            if (stmt->child)
                cg_generate_block(fn, stmt->child);
            break;
    }
}

static void cg_generate_function(FunctionContext *fn, AST *funcNode, SymTable *scope) {
    if (!funcNode || !scope) return;
    fn->scope = scope;
    snprintf(fn->funcName, sizeof(fn->funcName), "%s", funcNode->name ? funcNode->name : "anon");
    snprintf(fn->endLabel, sizeof(fn->endLabel), "FUNC_%.*s_END", 48, fn->funcName);

    CodeGenContext *cg = fn->cg;
    cg_emit(cg, "FUNC_%s:\n", fn->funcName);
    cg_emit(cg, "    PUSH RA\n");
    cg_emit(cg, "    PUSH FP\n");
    cg_emit(cg, "    MOV FP, SP\n");
    if (scope->frame_size > 0)
        cg_emit(cg, "    SUB SP, SP, #%d    ; reserve locals\n", scope->frame_size);

    AST *body = funcNode->extra;
    if (body)
        cg_generate_block(fn, body->child);

    cg_emit(cg, "%s:\n", fn->endLabel);
    cg_emit(cg, "    MOV SP, FP\n");
    cg_emit(cg, "    POP FP\n");
    cg_emit(cg, "    POP RA\n");
    cg_emit(cg, "    RET\n\n");
}

int codegen_generate(AST *root, SymTable *global, const char *outPath) {
    if (!root || !outPath) return 1;
    FILE *out = fopen(outPath, "w");
    if (!out) return 1;

    CodeGenContext cg;
    cg_init(&cg, out);
    cg_emit(&cg, "; Auto-generated assembly\n\n");

    FunctionContext fn = {0};
    fn.cg = &cg;
    fn.scope = global;

    for (AST *p = root->child; p; p = p->sibling) {
        if (p->kind == NODE_FUNC_DECL) {
            SymTable *fnScope = symtable_find_scope(global, p->name, global);
            if (!fnScope)
                fnScope = symtable_find_scope(global, p->name, NULL);
            cg_generate_function(&fn, p, fnScope ? fnScope : global);
        }
    }

    fclose(out);
    return 0;
}

