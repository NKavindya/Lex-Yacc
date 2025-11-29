#include "codegen.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* x86-32 Architecture Configuration */
#define REG_POOL 6  /* EAX, EBX, ECX, EDX, ESI, EDI (EBP and ESP are special) */
#define WORD_SIZE 4  /* x86-32 uses 32-bit (4 bytes) words */

/* x86 General Purpose Registers (excluding EBP, ESP which are special) */
static const char *REG_NAMES[REG_POOL] = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI"};

/* IR temporary counter for 3AC */
static int ir_temp_counter = 0;

typedef struct {
    FILE *out;
    int available[REG_POOL];
    int labelCounter;
    int tempCounter;  /* For 3AC temporaries */
} CodeGenContext;

typedef struct {
    CodeGenContext *cg;
    SymTable *scope;
    char funcName[64];
    char endLabel[64];
    Symbol *funcSym;  /* Function symbol for parameter lookup */
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
    cg->tempCounter = 0;
}

static const char *reg_name(int idx) {
    if (idx < 0 || idx >= REG_POOL) return "EAX";  /* Default to EAX */
    return REG_NAMES[idx];
}

static int cg_alloc_reg(CodeGenContext *cg) {
    /* Prefer caller-saved registers first (EAX, ECX, EDX) */
    int preferred[] = {0, 2, 3, 1, 4, 5};  /* EAX, ECX, EDX, EBX, ESI, EDI */
    for (int i = 0; i < REG_POOL; ++i) {
        int reg = preferred[i];
        if (cg->available[reg]) {
            cg->available[reg] = 0;
            return reg;
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

/* Calculate x86 parameter offset: first param at EBP+8, second at EBP+12, etc. */
static int get_param_offset(Symbol *funcSym, const char *paramName) {
    if (!funcSym || !funcSym->params) return -1;
    int paramIndex = 0;
    for (Symbol *p = funcSym->params; p; p = p->next) {
        if (p->name && strcmp(p->name, paramName) == 0) {
            /* x86 cdecl: [EBP+4] = return address, [EBP+8] = first param */
            return 8 + (paramIndex * WORD_SIZE);
        }
        paramIndex++;
    }
    return -1;
}

static void cg_store_var(FunctionContext *fn, Symbol *sym, int reg) {
    if (!sym) return;
    if (sym->kind == SYM_PARAM) {
        int offset = get_param_offset(fn->funcSym, sym->name);
        if (offset > 0) {
            cg_emit(fn->cg, "    mov DWORD PTR [EBP+%d], %s    ; %s (parameter)\n", offset, reg_name(reg), sym->name);
        }
    } else if (sym->offset >= 0 && sym->kind != SYM_CLASS && sym->kind != SYM_FUNC) {
        /* Local variable: negative offset from EBP */
        cg_emit(fn->cg, "    mov DWORD PTR [EBP-%d], %s    ; %s (local)\n", sym->offset, reg_name(reg), sym->name);
    } else {
        /* Global variable: absolute address */
        cg_emit(fn->cg, "    mov DWORD PTR [%s], %s    ; %s (global)\n", sym->name ? sym->name : "tmp", reg_name(reg), sym->name ? sym->name : "");
    }
}

static int cg_load_var(FunctionContext *fn, Symbol *sym) {
    int reg = cg_alloc_reg(fn->cg);
    if (!sym) {
        cg_emit(fn->cg, "    mov %s, 0\n", reg_name(reg));
        return reg;
    }
    if (sym->kind == SYM_PARAM) {
        int offset = get_param_offset(fn->funcSym, sym->name);
        if (offset > 0) {
            cg_emit(fn->cg, "    mov %s, DWORD PTR [EBP+%d]    ; %s (parameter)\n", reg_name(reg), offset, sym->name);
        } else {
            cg_emit(fn->cg, "    mov %s, 0    ; %s (param not found)\n", reg_name(reg), sym->name);
        }
    } else if (sym->offset >= 0 && sym->kind != SYM_CLASS && sym->kind != SYM_FUNC) {
        /* Local variable: negative offset from EBP */
        cg_emit(fn->cg, "    mov %s, DWORD PTR [EBP-%d]    ; %s (local)\n", reg_name(reg), sym->offset, sym->name);
    } else {
        /* Global variable: absolute address */
        cg_emit(fn->cg, "    mov %s, DWORD PTR [%s]    ; %s (global)\n", reg_name(reg), sym->name ? sym->name : "tmp", sym->name ? sym->name : "");
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
    cg_emit(fn->cg, "    %s %s, %s\n", mnemonic, reg_name(lhsReg), reg_name(rhsReg));
    cg_free_reg(fn->cg, rhsReg);
}

static int cg_generate_function_call(FunctionContext *fn, AST *call) {
    int argCount = 0;
    /* Push arguments right-to-left (x86 cdecl convention) */
    AST *args[64];
    int argIdx = 0;
    for (AST *arg = call->child; arg; arg = arg->sibling) {
        args[argIdx++] = arg;
        argCount++;
    }
    /* Push in reverse order */
    for (int i = argCount - 1; i >= 0; i--) {
        int r = cg_generate_expr(fn, args[i]);
        cg_emit(fn->cg, "    push %s\n", reg_name(r));
        cg_free_reg(fn->cg, r);
    }
    cg_emit(fn->cg, "    call _%s\n", call->name ? call->name : "anon");
    if (argCount > 0)
        cg_emit(fn->cg, "    add ESP, %d    ; clean up stack\n", argCount * WORD_SIZE);
    int target = cg_alloc_reg(fn->cg);
    /* Return value is in EAX (x86 convention) */
    if (target != 0) {
        cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(target));
    }
    return target;
}

static int cg_generate_expr(FunctionContext *fn, AST *expr) {
    if (!expr) {
        int r = cg_alloc_reg(fn->cg);
        cg_emit(fn->cg, "    mov %s, 0\n", reg_name(r));
        return r;
    }

    switch (expr->kind) {
        case NODE_ID: {
            Symbol *sym = cg_lookup(fn, expr->name);
            return cg_load_var(fn, sym);
        }
        case NODE_INT_LITERAL: {
            int r = cg_alloc_reg(fn->cg);
            cg_emit(fn->cg, "    mov %s, %d\n", reg_name(r), expr->intValue);
            return r;
        }
        case NODE_FLOAT_LITERAL: {
            int r = cg_alloc_reg(fn->cg);
            /* For float, we'll use integer representation (simplified) */
            cg_emit(fn->cg, "    mov %s, %d    ; float: %f\n", reg_name(r), (int)expr->floatValue, expr->floatValue);
            return r;
        }
        case NODE_STRING_LITERAL: {
            int r = cg_alloc_reg(fn->cg);
            cg_emit(fn->cg, "    mov %s, OFFSET str_%s    ; string literal\n", reg_name(r), expr->name ? expr->name : "anon");
            return r;
        }
        case NODE_BINARY_OP: {
            int left = cg_generate_expr(fn, expr->child);
            int right = cg_generate_expr(fn, expr->child ? expr->child->sibling : NULL);
            const char *op = expr->name ? expr->name : "";
            if (strcmp(op, "+") == 0) {
                cg_emit_binary(fn, "add", left, right);
            } else if (strcmp(op, "-") == 0) {
                cg_emit_binary(fn, "sub", left, right);
            } else if (strcmp(op, "*") == 0) {
                /* x86 mul uses EAX:EDX */
                cg_emit(fn->cg, "    mov EAX, %s\n", reg_name(left));
                cg_emit(fn->cg, "    mul DWORD PTR [ESP]    ; push right operand\n");
                cg_emit(fn->cg, "    push EAX\n");
                cg_emit(fn->cg, "    pop %s\n", reg_name(left));
                cg_emit(fn->cg, "    add ESP, 4\n");
                cg_free_reg(fn->cg, right);
                /* Simplified: use EAX directly */
                if (left != 0) {
                    cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(left));
                }
            } else if (strcmp(op, "/") == 0) {
                /* x86 div uses EAX:EDX */
                cg_emit(fn->cg, "    mov EAX, %s\n", reg_name(left));
                cg_emit(fn->cg, "    cdq    ; sign extend EAX to EDX:EAX\n");
                cg_emit(fn->cg, "    push %s\n", reg_name(right));
                cg_emit(fn->cg, "    idiv DWORD PTR [ESP]\n");
                cg_emit(fn->cg, "    add ESP, 4\n");
                cg_free_reg(fn->cg, right);
                if (left != 0) {
                    cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(left));
                }
            } else if (strcmp(op, "and") == 0 || strcmp(op, "&&") == 0) {
                cg_emit_binary(fn, "and", left, right);
            } else if (strcmp(op, "or") == 0 || strcmp(op, "||") == 0) {
                cg_emit_binary(fn, "or", left, right);
            } else if (strcmp(op, "==") == 0) {
                cg_emit(fn->cg, "    cmp %s, %s\n", reg_name(left), reg_name(right));
                cg_emit(fn->cg, "    sete AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_free_reg(fn->cg, right);
            } else if (strcmp(op, "<>") == 0 || strcmp(op, "!=") == 0) {
                cg_emit(fn->cg, "    cmp %s, %s\n", reg_name(left), reg_name(right));
                cg_emit(fn->cg, "    setne AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_free_reg(fn->cg, right);
            } else if (strcmp(op, "<") == 0) {
                cg_emit(fn->cg, "    cmp %s, %s\n", reg_name(left), reg_name(right));
                cg_emit(fn->cg, "    setl AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_free_reg(fn->cg, right);
            } else if (strcmp(op, ">") == 0) {
                cg_emit(fn->cg, "    cmp %s, %s\n", reg_name(left), reg_name(right));
                cg_emit(fn->cg, "    setg AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_free_reg(fn->cg, right);
            } else if (strcmp(op, "<=") == 0) {
                cg_emit(fn->cg, "    cmp %s, %s\n", reg_name(left), reg_name(right));
                cg_emit(fn->cg, "    setle AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_free_reg(fn->cg, right);
            } else if (strcmp(op, ">=") == 0) {
                cg_emit(fn->cg, "    cmp %s, %s\n", reg_name(left), reg_name(right));
                cg_emit(fn->cg, "    setge AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_free_reg(fn->cg, right);
            } else {
                cg_emit_binary(fn, "add", left, right);
            }
            return left;
        }
        case NODE_UNARY_OP: {
            int inner = cg_generate_expr(fn, expr->child);
            const char *op = expr->name ? expr->name : "";
            if (strcmp(op, "not") == 0) {
                cg_emit(fn->cg, "    not %s    ; logical not\n", reg_name(inner));
            } else if (strcmp(op, "-") == 0) {
                cg_emit(fn->cg, "    neg %s    ; negate\n", reg_name(inner));
            }
            return inner;
        }
        case NODE_FUNCTION_CALL:
            return cg_generate_function_call(fn, expr);
        default:
            break;
    }

    int r = cg_alloc_reg(fn->cg);
    cg_emit(fn->cg, "    mov %s, 0\n", reg_name(r));
    return r;
}

static void cg_generate_if(FunctionContext *fn, AST *node) {
    char elseLabel[64], endLabel[64];
    cg_make_label(fn->cg, elseLabel, sizeof(elseLabel), "L_if_else");
    cg_make_label(fn->cg, endLabel, sizeof(endLabel), "L_if_end");

    int condReg = cg_generate_expr(fn, node->child);
    cg_emit(fn->cg, "    test %s, %s\n", reg_name(condReg), reg_name(condReg));
    cg_emit(fn->cg, "    jz %s\n", elseLabel);
    cg_free_reg(fn->cg, condReg);

    AST *thenBlock = node->child ? node->child->sibling : NULL;
    AST *elseBlock = thenBlock ? thenBlock->sibling : NULL;
    cg_generate_block(fn, thenBlock);
    cg_emit(fn->cg, "    jmp %s\n", endLabel);

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
    cg_emit(fn->cg, "    test %s, %s\n", reg_name(condReg), reg_name(condReg));
    cg_emit(fn->cg, "    jz %s\n", endLabel);
    cg_free_reg(fn->cg, condReg);

    AST *body = node->child ? node->child->sibling : NULL;
    cg_generate_block(fn, body);
    cg_emit(fn->cg, "    jmp %s\n", topLabel);
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
            cg_emit(fn->cg, "    call _read    ; read input\n");
            cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(r));
            cg_store_var(fn, sym, r);
            cg_free_reg(fn->cg, r);
            break;
        }
        case NODE_WRITE: {
            int r = cg_generate_expr(fn, stmt->child);
            cg_emit(fn->cg, "    push %s\n", reg_name(r));
            cg_emit(fn->cg, "    call _write    ; write output\n");
            cg_emit(fn->cg, "    add ESP, 4\n");
            cg_free_reg(fn->cg, r);
            break;
        }
        case NODE_RETURN: {
            int r = cg_generate_expr(fn, stmt->child);
            if (r != 0) {
                cg_emit(fn->cg, "    mov EAX, %s\n", reg_name(r));
            }
            cg_free_reg(fn->cg, r);
            cg_emit(fn->cg, "    jmp %s\n", fn->endLabel);
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
    /* Ensure endLabel fits: "_" + funcName (max 58 chars) + "_END" + null = 64 bytes total */
    /* Format "_%s_END" needs: 1 + funcName + 4 + 1 = 64, so funcName max is 58 */
    /* Use format specifier with length limit to avoid truncation warning */
    snprintf(fn->endLabel, sizeof(fn->endLabel), "_%.58s_END", fn->funcName);
    
    /* Find function symbol for parameter lookup */
    fn->funcSym = symtable_lookup(scope, fn->funcName);
    if (!fn->funcSym) {
        /* Try global scope */
        SymTable *global = scope;
        while (global->parent) global = global->parent;
        fn->funcSym = symtable_lookup(global, fn->funcName);
    }

    CodeGenContext *cg = fn->cg;
    cg_emit(cg, "_%s:\n", fn->funcName);
    /* x86 function prologue */
    cg_emit(cg, "    push EBP\n");
    cg_emit(cg, "    mov EBP, ESP\n");
    if (scope->frame_size > 0)
        cg_emit(cg, "    sub ESP, %d    ; reserve space for locals\n", scope->frame_size);

    AST *body = funcNode->extra;
    if (body)
        cg_generate_block(fn, body->child);

    cg_emit(cg, "%s:\n", fn->endLabel);
    /* x86 function epilogue */
    cg_emit(cg, "    mov ESP, EBP\n");
    cg_emit(cg, "    pop EBP\n");
    cg_emit(cg, "    ret\n\n");
}

int codegen_generate(AST *root, SymTable *global, const char *outPath) {
    if (!root || !outPath) return 1;
    FILE *out = fopen(outPath, "w");
    if (!out) return 1;

    CodeGenContext cg;
    cg_init(&cg, out);
    cg_emit(&cg, "; Auto-generated x86-32 assembly code\n");
    cg_emit(&cg, "; Target: x86 (32-bit) architecture\n");
    cg_emit(&cg, "; Calling convention: cdecl (caller cleans stack)\n\n");
    cg_emit(&cg, "    .386\n");
    cg_emit(&cg, "    .model flat, c\n");
    cg_emit(&cg, "    .code\n\n");

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

    cg_emit(&cg, "    end\n");
    fclose(out);
    return 0;
}

/* =====================================================
   INTERMEDIATE REPRESENTATION (3AC) GENERATION
   ===================================================== */

static void ir_emit_3ac(FILE *out, const char *op, const char *arg1, const char *arg2, const char *result) {
    if (strcmp(op, "ASSIGN") == 0) {
        fprintf(out, "    %s = %s\n", result, arg1);
    } else if (strcmp(op, "RETURN") == 0) {
        fprintf(out, "    return %s\n", arg1);
    } else if (strcmp(op, "CALL") == 0) {
        fprintf(out, "    %s = call %s(%s)\n", result, arg1, arg2 ? arg2 : "");
    } else {
        fprintf(out, "    %s = %s %s, %s\n", result, arg1, op, arg2);
    }
}

static char *ir_make_temp(CodeGenContext *cg) {
    char *temp = (char *)malloc(16);
    snprintf(temp, 16, "t%d", cg->tempCounter++);
    return temp;
}

static char *ir_generate_expr_3ac(FILE *out, CodeGenContext *cg, FunctionContext *fn, AST *expr);

static char *ir_generate_expr_3ac(FILE *out, CodeGenContext *cg, FunctionContext *fn, AST *expr) {
    if (!expr) {
        char *temp = ir_make_temp(cg);
        fprintf(out, "    %s = 0\n", temp);
        return temp;
    }

    switch (expr->kind) {
        case NODE_ID: {
            char *temp = ir_make_temp(cg);
            Symbol *sym = cg_lookup(fn, expr->name);
            if (sym) {
                if (sym->kind == SYM_PARAM) {
                    fprintf(out, "    %s = param %s\n", temp, expr->name);
                } else {
                    fprintf(out, "    %s = load %s\n", temp, expr->name);
                }
            } else {
                fprintf(out, "    %s = 0    ; undefined\n", temp);
            }
            return temp;
        }
        case NODE_INT_LITERAL: {
            char *temp = ir_make_temp(cg);
            fprintf(out, "    %s = %d\n", temp, expr->intValue);
            return temp;
        }
        case NODE_FLOAT_LITERAL: {
            char *temp = ir_make_temp(cg);
            fprintf(out, "    %s = %f\n", temp, expr->floatValue);
            return temp;
        }
        case NODE_BINARY_OP: {
            char *left = ir_generate_expr_3ac(out, cg, fn, expr->child);
            char *right = ir_generate_expr_3ac(out, cg, fn, expr->child ? expr->child->sibling : NULL);
            char *result = ir_make_temp(cg);
            const char *op = expr->name ? expr->name : "+";
            fprintf(out, "    %s = %s %s, %s\n", result, left, op, right);
            free(left);
            free(right);
            return result;
        }
        case NODE_UNARY_OP: {
            char *inner = ir_generate_expr_3ac(out, cg, fn, expr->child);
            char *result = ir_make_temp(cg);
            const char *op = expr->name ? expr->name : "not";
            fprintf(out, "    %s = %s %s\n", result, op, inner);
            free(inner);
            return result;
        }
        case NODE_FUNCTION_CALL: {
            char *result = ir_make_temp(cg);
            fprintf(out, "    %s = call %s(", result, expr->name ? expr->name : "anon");
            int first = 1;
            for (AST *arg = expr->child; arg; arg = arg->sibling) {
                if (!first) fprintf(out, ", ");
                char *argTemp = ir_generate_expr_3ac(out, cg, fn, arg);
                fprintf(out, "%s", argTemp);
                free(argTemp);
                first = 0;
            }
            fprintf(out, ")\n");
            return result;
        }
        default: {
            char *temp = ir_make_temp(cg);
            fprintf(out, "    %s = 0    ; unknown expr\n", temp);
            return temp;
        }
    }
}

int codegen_generate_ir(AST *root, SymTable *global, const char *outPath) {
    if (!root || !outPath) return 1;
    FILE *out = fopen(outPath, "w");
    if (!out) return 1;

    fprintf(out, "; Intermediate Representation (3AC - Three Address Code)\n");
    fprintf(out, "; IR is a machine-independent representation between AST and assembly\n");
    fprintf(out, "; Format: result = operand1 operator operand2\n\n");

    CodeGenContext cg;
    cg_init(&cg, out);

    FunctionContext fn = {0};
    fn.cg = &cg;
    fn.scope = global;

    for (AST *p = root->child; p; p = p->sibling) {
        if (p->kind == NODE_FUNC_DECL) {
            fprintf(out, "function %s:\n", p->name ? p->name : "anon");
            
            SymTable *fnScope = symtable_find_scope(global, p->name, global);
            if (!fnScope)
                fnScope = symtable_find_scope(global, p->name, NULL);
            fn.scope = fnScope ? fnScope : global;
            fn.funcSym = symtable_lookup(fn.scope, p->name);
            
            fprintf(out, "  prologue\n");
            
            AST *body = p->extra;
            if (body) {
                for (AST *stmt = body->child; stmt; stmt = stmt->sibling) {
                    if (stmt->kind == NODE_ASSIGN) {
                        AST *lhs = stmt->child;
                        char *rhs = ir_generate_expr_3ac(out, &cg, &fn, lhs ? lhs->sibling : NULL);
                        fprintf(out, "    store %s, %s\n", lhs ? lhs->name : "?", rhs);
                        free(rhs);
                    } else if (stmt->kind == NODE_RETURN) {
                        char *ret = ir_generate_expr_3ac(out, &cg, &fn, stmt->child);
                        fprintf(out, "    return %s\n", ret);
                        free(ret);
                    } else if (stmt->kind == NODE_IF) {
                        char *cond = ir_generate_expr_3ac(out, &cg, &fn, stmt->child);
                        fprintf(out, "    if %s goto L_then else goto L_else\n", cond);
                        free(cond);
                        fprintf(out, "  L_then:\n");
                        AST *thenBlock = stmt->child ? stmt->child->sibling : NULL;
                        if (thenBlock) {
                            for (AST *s = thenBlock->child; s; s = s->sibling) {
                                if (s->kind == NODE_ASSIGN) {
                                    AST *lhs = s->child;
                                    char *rhs = ir_generate_expr_3ac(out, &cg, &fn, lhs ? lhs->sibling : NULL);
                                    fprintf(out, "    store %s, %s\n", lhs ? lhs->name : "?", rhs);
                                    free(rhs);
                                }
                            }
                        }
                        fprintf(out, "    goto L_if_end\n");
                        fprintf(out, "  L_else:\n");
                        AST *elseBlock = thenBlock ? thenBlock->sibling : NULL;
                        if (elseBlock) {
                            for (AST *s = elseBlock->child; s; s = s->sibling) {
                                if (s->kind == NODE_ASSIGN) {
                                    AST *lhs = s->child;
                                    char *rhs = ir_generate_expr_3ac(out, &cg, &fn, lhs ? lhs->sibling : NULL);
                                    fprintf(out, "    store %s, %s\n", lhs ? lhs->name : "?", rhs);
                                    free(rhs);
                                }
                            }
                        }
                        fprintf(out, "  L_if_end:\n");
                    } else if (stmt->kind == NODE_WHILE) {
                        fprintf(out, "  L_while_top:\n");
                        char *cond = ir_generate_expr_3ac(out, &cg, &fn, stmt->child);
                        fprintf(out, "    if %s goto L_while_body else goto L_while_end\n", cond);
                        free(cond);
                        fprintf(out, "  L_while_body:\n");
                        AST *bodyBlock = stmt->child ? stmt->child->sibling : NULL;
                        if (bodyBlock) {
                            for (AST *s = bodyBlock->child; s; s = s->sibling) {
                                if (s->kind == NODE_ASSIGN) {
                                    AST *lhs = s->child;
                                    char *rhs = ir_generate_expr_3ac(out, &cg, &fn, lhs ? lhs->sibling : NULL);
                                    fprintf(out, "    store %s, %s\n", lhs ? lhs->name : "?", rhs);
                                    free(rhs);
                                }
                            }
                        }
                        fprintf(out, "    goto L_while_top\n");
                        fprintf(out, "  L_while_end:\n");
                    }
                }
            }
            fprintf(out, "  epilogue\n\n");
        }
    }

    fclose(out);
    return 0;
}

/* =====================================================
   MACHINE CODE GENERATION (Relocatable and Absolute)
   ===================================================== */

int codegen_generate_relocatable(const char *asmPath, const char *outPath) {
    FILE *in = fopen(asmPath, "r");
    if (!in) return 1;
    
    FILE *out = fopen(outPath, "w");
    if (!out) {
        fclose(in);
        return 1;
    }

    fprintf(out, "RELOCATABLE_OBJECT\n");
    fprintf(out, "FORMAT_VERSION: 1.0\n");
    fprintf(out, "ARCHITECTURE: x86-32\n");
    fprintf(out, "WORD_SIZE: 4\n\n");
    fprintf(out, "CODE_SECTION:\n");

    char line[256];
    int address = 0;
    
    /* Simple parsing: collect labels and estimate instruction sizes */
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == ';' || line[0] == '\n') continue;
        if (strstr(line, ":")) {
            fprintf(out, "  0x%04X: %s", address, line);
            continue;
        }
        if (strstr(line, "mov") || strstr(line, "add") || strstr(line, "sub"))
            address += 3;  /* Typical x86 instruction size */
        else if (strstr(line, "push") || strstr(line, "pop"))
            address += 1;
        else if (strstr(line, "call") || strstr(line, "jmp") || strstr(line, "jz"))
            address += 5;  /* Call/jump with relocatable address */
        else if (strstr(line, "ret"))
            address += 1;
        else
            address += 2;
    }
    
    fprintf(out, "\nRELOCATION_TABLE:\n");
    fprintf(out, "  ; Symbols requiring relocation\n");
    
    fprintf(out, "\nSYMBOL_TABLE:\n");
    fprintf(out, "  ; Function entry points\n");
    
    fclose(in);
    fclose(out);
    return 0;
}

int codegen_generate_absolute(const char *asmPath, const char *outPath) {
    FILE *in = fopen(asmPath, "r");
    if (!in) return 1;
    
    FILE *out = fopen(outPath, "w");
    if (!out) {
        fclose(in);
        return 1;
    }

    fprintf(out, "ABSOLUTE_OBJECT\n");
    fprintf(out, "FORMAT_VERSION: 1.0\n");
    fprintf(out, "ARCHITECTURE: x86-32\n");
    fprintf(out, "LOAD_ADDRESS: 0x00401000\n\n");
    fprintf(out, "CODE_SECTION:\n");

    char line[256];
    int baseAddress = 0x00401000;
    int address = baseAddress;
    
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == ';' || line[0] == '\n') continue;
        if (strstr(line, ":")) {
            fprintf(out, "  0x%08X: %s", address, line);
            continue;
        }
        fprintf(out, "  0x%08X: %s", address, line);
        if (strstr(line, "mov") || strstr(line, "add") || strstr(line, "sub"))
            address += 3;
        else if (strstr(line, "push") || strstr(line, "pop"))
            address += 1;
        else if (strstr(line, "call") || strstr(line, "jmp") || strstr(line, "jz"))
            address += 5;
        else if (strstr(line, "ret"))
            address += 1;
        else
            address += 2;
    }
    
    fprintf(out, "\nSYMBOL_TABLE:\n");
    fprintf(out, "  ; Resolved function addresses\n");
    
    fclose(in);
    fclose(out);
    return 0;
}
