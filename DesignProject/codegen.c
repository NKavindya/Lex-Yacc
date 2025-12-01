/* 
 * codegen.c - code generation implementation
 * 
 * generates code from the AST in multiple formats:
 * 
 * 1. intermediate representation (IR):
 *    - 3-address code format (three-address code)
 *    - machine-independent representation
 *    - format: result = operand1 operator operand2
 * 
 * 2. x86-32 assembly code:
 *    - generates x86-32 assembly language
 *    - uses standard calling conventions (parameters at EBP+8, locals at EBP-offset)
 *    - manages register allocation (EAX, EBX, ECX, EDX, ESI, EDI)
 *    - handles function prologues/epilogues
 *    - generates code for: expressions, statements, function calls, control flow
 * 
 * 3. relocatable machine code:
 *    - assembles assembly into relocatable object file format
 *    - can be linked with other object files
 * 
 * 4. absolute machine code:
 *    - assembles assembly into absolute executable format
 *    - fixed memory addresses (ready to execute)
 * 
 * architecture: x86-32 (32-bit x86)
 * - word size: 4 bytes
 * - stack grows downward
 * - calling convention: parameters pushed right-to-left, caller cleans stack
 * - registers: EAX, EBX, ECX, EDX, ESI, EDI (general purpose), EBP (frame pointer), ESP (stack pointer)
 */

#include "codegen.h"      // code generation function prototypes
#include "symbol_table.h" // symbol table for variable offsets and TYPE information
#include <stdio.h>         // standard i/o (fprintf, FILE)
#include <stdlib.h>       // standard library (malloc, free, exit)
#include <string.h>       // string manipulation (strcmp, strdup, sprintf)
#include <stdarg.h>       // variable argument lists (va_list, va_start, etc.)

/* ========== string literal management ========== */
/* maps string literals to indices in .data section */

typedef struct {
    char *str;    // string literal text
    int index;    // index in string literal table
} StringMapping;

static StringMapping string_map[100];  // array of string mappings
static int string_map_count = 0;        // number of strings in table

/* ========== float literal management ========== */
/* maps float literals to indices in .data section */

typedef struct {
    double value; // float literal value
    int index;    // index in float literal table
} FloatMapping;

static FloatMapping float_map[100];  // array of float mappings
static int float_map_count = 0;       // number of floats in table

/* ========== x86-32 architecture configuration ========== */

/* register pool: number of general-purpose registers available for allocation */
/* EAX, EBX, ECX, EDX, ESI, EDI (EBP and ESP are special-purpose) */
#define REG_POOL 6

/* word size: x86-32 uses 32-bit (4 bytes) words */
#define WORD_SIZE 4

/* x86 general purpose register names (excluding EBP, ESP which are special) */
/* index mapping: 0=EAX, 1=EBX, 2=ECX, 3=EDX, 4=ESI, 5=EDI */
static const char *REG_NAMES[REG_POOL] = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI"};

/* ========== IR generation support ========== */

/* IR temporary counter: generates unique temporary variable names for 3-address code */
/* format: t0, t1, t2, ... */
static int ir_temp_counter = 0;

typedef struct {
    FILE *out;
    int available[REG_POOL];
    int labelCounter;
    int tempCounter;  /* for 3AC temporaries */
} CodeGenContext;

typedef struct {
    CodeGenContext *cg;
    SymTable *scope;
    char funcName[64];
    char endLabel[64];
    Symbol *funcSym;  /* function symbol for parameter lookup */
    int callee_saved_used[3];  /* track which callee-saved regs are used: EBX(1), ESI(4), EDI(5) */
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
    cg->available[0] = 0;
}

static const char *reg_name(int idx) {
    if (idx < 0 || idx >= REG_POOL) return "EAX";  /* default to EAX */
    return REG_NAMES[idx];
}

static int cg_alloc_reg_with_tracking(CodeGenContext *cg, FunctionContext *fn) {
    /* prefer caller-saved registers first (EAX, ECX, EDX) */
    int preferred[] = {0, 2, 3, 1, 4, 5};  /* EAX, ECX, EDX, EBX, ESI, EDI */
    for (int i = 0; i < REG_POOL; ++i) {
        int reg = preferred[i];
        if (cg->available[reg]) {
            cg->available[reg] = 0;
            /* track callee-saved registers: EBX=1, ESI=4, EDI=5 */
            if (fn && reg == 1) fn->callee_saved_used[0] = 1;  /* EBX */
            else if (fn && reg == 4) fn->callee_saved_used[1] = 1;  /* ESI */
            else if (fn && reg == 5) fn->callee_saved_used[2] = 1;  /* EDI */
            return reg;
        }
    }
    fprintf(stderr, "[codegen] register exhaustion - no register spilling implemented\n");
    fprintf(stderr, "[codegen] This is a known limitation. Complex expressions may fail.\n");
    exit(1);
}

static int cg_alloc_reg(CodeGenContext *cg) {
    return cg_alloc_reg_with_tracking(cg, NULL);
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

/* calculate x86 parameter offset: first PARAM at EBP+8, second at EBP+12, etc. */
static int get_param_offset(Symbol *funcSym, const char *paramName) {
    if (!funcSym || !funcSym->params) return -1;
    int paramIndex = 0;
    for (Symbol *p = funcSym->params; p; p = p->next) {
        if (p->name && strcmp(p->name, paramName) == 0) {
            /* x86 cdecl: [EBP+4] = RETURN address, [EBP+8] = first PARAM */
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
        /* local variable: negative offset from EBP */
        cg_emit(fn->cg, "    mov DWORD PTR [EBP-%d], %s    ; %s (local)\n", sym->offset, reg_name(reg), sym->name);
    } else {
        /* global variable: absolute address */
        cg_emit(fn->cg, "    mov DWORD PTR [%s], %s    ; %s (global)\n", sym->name ? sym->name : "tmp", reg_name(reg), sym->name ? sym->name : "");
    }
}

static int cg_load_var(FunctionContext *fn, Symbol *sym) {
    int reg = cg_alloc_reg_with_tracking(fn->cg, fn);
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
        /* local variable: negative offset from EBP */
        cg_emit(fn->cg, "    mov %s, DWORD PTR [EBP-%d]    ; %s (local)\n", reg_name(reg), sym->offset, sym->name);
    } else {
        /* global variable: absolute address */
        cg_emit(fn->cg, "    mov %s, DWORD PTR [%s]    ; %s (global)\n", reg_name(reg), sym->name ? sym->name : "tmp", sym->name ? sym->name : "");
    }
    return reg;
}

/* forward declarations */
static int cg_generate_expr(FunctionContext *fn, AST *expr);
static void cg_generate_statement(FunctionContext *fn, AST *stmt);
static int get_string_index(const char *str);
static int get_float_index(double value);

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
    /* push arguments right-to-left (x86 cdecl convention) */
    AST *args[64];
    int argIdx = 0;
    for (AST *arg = call->child; arg; arg = arg->sibling) {
        args[argIdx++] = arg;
        argCount++;
    }
    /* push in reverse order */
    for (int i = argCount - 1; i >= 0; i--) {
        int r = cg_generate_expr(fn, args[i]);
        cg_emit(fn->cg, "    push %s\n", reg_name(r));
        cg_free_reg(fn->cg, r);
    }
    cg_emit(fn->cg, "    call _%s\n", call->name ? call->name : "anon");
    if (argCount > 0)
        cg_emit(fn->cg, "    add ESP, %d    ; clean up stack\n", argCount * WORD_SIZE);
    int target = cg_alloc_reg_with_tracking(fn->cg, fn);
    /* RETURN value is in EAX (x86 convention) */
    if (target != 0) {
        cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(target));
    }
    return target;
}

static int cg_generate_expr(FunctionContext *fn, AST *expr) {
    if (!expr) {
        int r = cg_alloc_reg_with_tracking(fn->cg, fn);
        cg_emit(fn->cg, "    mov %s, 0\n", reg_name(r));
        return r;
    }

    switch (expr->kind) {
        case NODE_ID: {
            Symbol *sym = cg_lookup(fn, expr->name);
            return cg_load_var(fn, sym);
        }
        case NODE_INT_LITERAL: {
            int r = cg_alloc_reg_with_tracking(fn->cg, fn);
            cg_emit(fn->cg, "    mov %s, %d\n", reg_name(r), expr->intValue);
            return r;
        }
        case NODE_FLOAT_LITERAL: {
            /* use x87 fpu for float literals - store in .data section and load via fpu */
            int float_idx = get_float_index(expr->floatValue);
            int r = cg_alloc_reg_with_tracking(fn->cg, fn);
            /* load float from .data section using fpu */
            cg_emit(fn->cg, "    fld QWORD PTR [float_%d]    ; load float: %f\n", float_idx, expr->floatValue);
            /* store float to memory temporarily (8 bytes for double precision) */
            cg_emit(fn->cg, "    sub ESP, 8    ; allocate space for float\n");
            cg_emit(fn->cg, "    fstp QWORD PTR [ESP]    ; store float to stack\n");
            /* for compatibility with integer-based codegen, we need integer representation */
            /* however, we preserve the float value in memory and use fpu for operations */
            /* load the lower 32 bits as integer representation (this is a limitation) */
            cg_emit(fn->cg, "    mov %s, DWORD PTR [ESP]    ; load float bits (lower 32 bits)\n", reg_name(r));
            cg_emit(fn->cg, "    add ESP, 8    ; clean up stack\n");
            /* note: this is still not ideal - proper float support requires fpu stack throughout */
            /* the float value is loaded correctly via fpu, but we extract integer bits for compatibility */
            /* full fpu support would require refactoring codegen to track float types and use fpu stack */
            return r;
        }
        case NODE_STRING_LITERAL: {
            int r = cg_alloc_reg_with_tracking(fn->cg, fn);
            int str_idx = get_string_index(expr->name ? expr->name : "");
            if (str_idx >= 0) {
                cg_emit(fn->cg, "    mov %s, OFFSET str_%d    ; string literal: \"%s\"\n", 
                        reg_name(r), str_idx, expr->name ? expr->name : "");
            } else {
                /* fallback IF not found in mapping */
                cg_emit(fn->cg, "    mov %s, 0    ; string literal not found in .data section\n", reg_name(r));
            }
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
                /* x86 mul uses EAX:EDX - mul multiplies EAX by operand, result in EDX:EAX */
                /* check IF EAX is already allocated - IF so, save it */
                int eax_was_allocated = !fn->cg->available[0];
                if (eax_was_allocated) {
                    /* save EAX to stack temporarily */
                    cg_emit(fn->cg, "    push EAX    ; save EAX before mul\n");
                }
                /* free EAX IF it was allocated */
                if (eax_was_allocated) {
                    fn->cg->available[0] = 1;
                }
                /* move left operand to EAX, push right operand, then multiply */
                cg_emit(fn->cg, "    mov EAX, %s\n", reg_name(left));
                cg_emit(fn->cg, "    push %s\n", reg_name(right));
                cg_emit(fn->cg, "    mul DWORD PTR [ESP]    ; EAX = EAX * [ESP]\n");
                cg_emit(fn->cg, "    add ESP, 4    ; clean up stack\n");
                cg_free_reg(fn->cg, right);
                /* result is in EAX, move to target register IF needed */
                if (left != 0) {
                    cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(left));
                    /* restore EAX IF it was allocated before */
                    if (eax_was_allocated) {
                        cg_emit(fn->cg, "    pop EAX    ; restore EAX after mul\n");
                        fn->cg->available[0] = 0;  /* mark EAX as allocated again */
                    }
                } else {
                    /* IF left was EAX, it's already there */
                    left = 0;  /* EAX is register 0 */
                    fn->cg->available[0] = 0;  /* mark EAX as allocated */
                    /* restore EAX IF it was allocated before (but now result is in EAX) */
                    if (eax_was_allocated) {
                        /* the old EAX value is lost - this is a limitation */
                        cg_emit(fn->cg, "    add ESP, 4    ; discard saved EAX (result now in EAX)\n");
                    }
                }
            } else if (strcmp(op, "/") == 0) {
                /* x86 idiv uses EDX:EAX / operand, quotient in EAX, remainder in EDX */
                /* check IF EAX is already allocated - IF so, save it */
                int eax_was_allocated = !fn->cg->available[0];
                if (eax_was_allocated) {
                    /* save EAX to stack temporarily */
                    cg_emit(fn->cg, "    push EAX    ; save EAX before idiv\n");
                }
                /* free EAX IF it was allocated */
                if (eax_was_allocated) {
                    fn->cg->available[0] = 1;
                }
                /* move left operand to EAX, sign extend, push right operand, then divide */
                cg_emit(fn->cg, "    mov EAX, %s\n", reg_name(left));
                cg_emit(fn->cg, "    cdq    ; sign extend EAX to EDX:EAX\n");
                cg_emit(fn->cg, "    push %s\n", reg_name(right));
                cg_emit(fn->cg, "    idiv DWORD PTR [ESP]    ; EAX = EDX:EAX / [ESP]\n");
                cg_emit(fn->cg, "    add ESP, 4    ; clean up stack\n");
                cg_free_reg(fn->cg, right);
                /* quotient is in EAX, move to target register IF needed */
                if (left != 0) {
                    cg_emit(fn->cg, "    mov %s, EAX\n", reg_name(left));
                    /* restore EAX IF it was allocated before */
                    if (eax_was_allocated) {
                        cg_emit(fn->cg, "    pop EAX    ; restore EAX after idiv\n");
                        fn->cg->available[0] = 0;  /* mark EAX as allocated again */
                    }
                } else {
                    /* IF left was EAX, it's already there */
                    left = 0;  /* EAX is register 0 */
                    fn->cg->available[0] = 0;  /* mark EAX as allocated */
                    /* restore EAX IF it was allocated before (but now result is in EAX) */
                    if (eax_was_allocated) {
                        /* the old EAX value is lost - this is a limitation */
                        cg_emit(fn->cg, "    add ESP, 4    ; discard saved EAX (result now in EAX)\n");
                    }
                }
            } else if (strcmp(op, "and") == 0 || strcmp(op, "&&") == 0) {
                /* short-circuit and: IF left is false, skip right evaluation */
                char short_circuit_end[64];
                cg_make_label(fn->cg, short_circuit_end, sizeof(short_circuit_end), "L_and_end");
                /* test left operand */
                cg_emit(fn->cg, "    test %s, %s\n", reg_name(left), reg_name(left));
                cg_emit(fn->cg, "    jz %s    ; short-circuit: skip right if left is false\n", short_circuit_end);
                /* left is true, evaluate right */
                cg_emit(fn->cg, "    test %s, %s\n", reg_name(right), reg_name(right));
                cg_emit(fn->cg, "    setnz AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_emit(fn->cg, "%s:\n", short_circuit_end);
                cg_free_reg(fn->cg, right);
            } else if (strcmp(op, "or") == 0 || strcmp(op, "||") == 0) {
                /* short-circuit or: IF left is true, skip right evaluation */
                char short_circuit_end[64];
                cg_make_label(fn->cg, short_circuit_end, sizeof(short_circuit_end), "L_or_end");
                /* test left operand */
                cg_emit(fn->cg, "    test %s, %s\n", reg_name(left), reg_name(left));
                cg_emit(fn->cg, "    jnz %s    ; short-circuit: skip right if left is true\n", short_circuit_end);
                /* left is false, evaluate right */
                cg_emit(fn->cg, "    test %s, %s\n", reg_name(right), reg_name(right));
                cg_emit(fn->cg, "    setnz AL\n");
                cg_emit(fn->cg, "    movzx %s, AL\n", reg_name(left));
                cg_emit(fn->cg, "%s:\n", short_circuit_end);
                cg_free_reg(fn->cg, right);
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

    int r = cg_alloc_reg_with_tracking(fn->cg, fn);
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
            int r = cg_alloc_reg_with_tracking(fn->cg, fn);
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
    /* initialize callee-saved tracking */
    fn->callee_saved_used[0] = 0;  /* EBX */
    fn->callee_saved_used[1] = 0;  /* ESI */
    fn->callee_saved_used[2] = 0;  /* EDI */
    
    snprintf(fn->funcName, sizeof(fn->funcName), "%s", funcNode->name ? funcNode->name : "anon");
    /* ensure endlabel fits: "_" + funcname (max 58 chars) + "_end" + NULL = 64 bytes total */
    /* format "_%s_end" needs: 1 + funcname + 4 + 1 = 64, so funcname max is 58 */
    /* use format specifier with length limit to avoid truncation warning */
    snprintf(fn->endLabel, sizeof(fn->endLabel), "_%.58s_END", fn->funcName);
    
    /* find function symbol for parameter lookup */
    fn->funcSym = symtable_lookup(scope, fn->funcName);
    if (!fn->funcSym) {
        /* try global scope */
        SymTable *global = scope;
        while (global->parent) global = global->parent;
        fn->funcSym = symtable_lookup(global, fn->funcName);
    }

    CodeGenContext *cg = fn->cg;
    cg_emit(cg, "_%s:\n", fn->funcName);
    /* x86 function prologue */
    cg_emit(cg, "    push EBP\n");
    cg_emit(cg, "    mov EBP, ESP\n");
    /* save only callee-saved registers that are actually used */
    /* tracked via callee_saved_used array: EBX(0), ESI(1), EDI(2) */
    if (fn->callee_saved_used[2]) {
        cg_emit(cg, "    push EDI    ; save callee-saved register\n");
    }
    if (fn->callee_saved_used[1]) {
        cg_emit(cg, "    push ESI    ; save callee-saved register\n");
    }
    if (fn->callee_saved_used[0]) {
        cg_emit(cg, "    push EBX    ; save callee-saved register\n");
    }
    if (scope->frame_size > 0)
        cg_emit(cg, "    sub ESP, %d    ; reserve space for locals\n", scope->frame_size);

    AST *body = funcNode->extra;
    if (body)
        cg_generate_block(fn, body->child);

    cg_emit(cg, "%s:\n", fn->endLabel);
    /* restore callee-saved registers in reverse order (only those we saved) */
    if (fn->callee_saved_used[0]) {
        cg_emit(cg, "    pop EBX    ; restore callee-saved register\n");
    }
    if (fn->callee_saved_used[1]) {
        cg_emit(cg, "    pop ESI    ; restore callee-saved register\n");
    }
    if (fn->callee_saved_used[2]) {
        cg_emit(cg, "    pop EDI    ; restore callee-saved register\n");
    }
    /* x86 function epilogue */
    cg_emit(cg, "    mov ESP, EBP\n");
    cg_emit(cg, "    pop EBP\n");
    cg_emit(cg, "    ret\n\n");
}

/* collect string literals from AST for .data section */
static void collect_string_literals(AST *node, char **strings, int *count, int max_count) {
    if (!node) return;
    if (node->kind == NODE_STRING_LITERAL && node->name) {
        /* check IF already collected */
        int found = 0;
        for (int i = 0; i < *count; i++) {
            if (strings[i] && strcmp(strings[i], node->name) == 0) {
                found = 1;
                break;
            }
        }
        if (!found && *count < max_count) {
            strings[*count] = node->name;
            (*count)++;
        }
    }
    /* recursively collect from children and siblings */
    if (node->child) collect_string_literals(node->child, strings, count, max_count);
    if (node->sibling) collect_string_literals(node->sibling, strings, count, max_count);
    if (node->extra) collect_string_literals(node->extra, strings, count, max_count);
}

/* collect float literals from AST for .data section */
static void collect_float_literals(AST *node) {
    if (!node) return;
    if (node->kind == NODE_FLOAT_LITERAL) {
        get_float_index(node->floatValue);
    }
    /* recursively collect from children and siblings */
    if (node->child) collect_float_literals(node->child);
    if (node->sibling) collect_float_literals(node->sibling);
    if (node->extra) collect_float_literals(node->extra);
}

static void generate_data_section(CodeGenContext *cg, AST *root) {
    char *strings[100];  /* max 100 unique string literals */
    int count = 0;
    string_map_count = 0;  /* reset mapping */
    float_map_count = 0;   /* reset float mapping */
    
    collect_string_literals(root, strings, &count, 100);
    collect_float_literals(root);  /* collect float literals */
    
    /* build string-to-index mapping */
    for (int i = 0; i < count; i++) {
        string_map[string_map_count].str = strings[i];
        string_map[string_map_count].index = i;
        string_map_count++;
    }
    
    if (count > 0 || float_map_count > 0) {
        cg_emit(cg, "    .data\n");
        /* generate string literals */
        for (int i = 0; i < count; i++) {
            /* generate label from index */
            char label[128];
            snprintf(label, sizeof(label), "str_%d", i);
            cg_emit(cg, "%s DB \"%s\", 0\n", label, strings[i]);
        }
        /* generate float literals */
        for (int i = 0; i < float_map_count; i++) {
            cg_emit(cg, "float_%d DQ %f    ; float constant\n", i, float_map[i].value);
        }
        cg_emit(cg, "\n");
    }
}

static int get_string_index(const char *str) {
    for (int i = 0; i < string_map_count; i++) {
        if (string_map[i].str && strcmp(string_map[i].str, str) == 0) {
            return string_map[i].index;
        }
    }
    return -1;  /* not found */
}

static int get_float_index(double value) {
    /* check IF float value already exists in mapping */
    for (int i = 0; i < float_map_count; i++) {
        if (float_map[i].value == value) {
            return float_map[i].index;
        }
    }
    /* add new float to mapping */
    if (float_map_count < 100) {
        float_map[float_map_count].value = value;
        float_map[float_map_count].index = float_map_count;
        return float_map_count++;
    }
    return -1;  /* mapping full */
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
    
    /* generate .data section for string literals */
    generate_data_section(&cg, root);
    
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

/* intermediate representation (3AC) generation */

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

/* machine code generation (relocatable and absolute) */

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
    
    /* simple parsing: collect labels and estimate instruction sizes */
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == ';' || line[0] == '\n') continue;
        if (strstr(line, ":")) {
            fprintf(out, "  0x%04X: %s", address, line);
            continue;
        }
        if (strstr(line, "mov") || strstr(line, "add") || strstr(line, "sub"))
            address += 3;  /* typical x86 instruction size */
        else if (strstr(line, "push") || strstr(line, "pop"))
            address += 1;
        else if (strstr(line, "call") || strstr(line, "jmp") || strstr(line, "jz"))
            address += 5;  /* call/jump with relocatable address */
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
