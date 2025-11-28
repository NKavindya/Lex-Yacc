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

/* =====================================================
   INTERMEDIATE REPRESENTATION (IR) GENERATION
   ===================================================== */

int codegen_generate_ir(AST *root, SymTable *global, const char *outPath) {
    if (!root || !outPath) return 1;
    FILE *out = fopen(outPath, "w");
    if (!out) return 1;

    fprintf(out, "; Intermediate Representation (IR)\n");
    fprintf(out, "; IR is a machine-independent representation between AST and assembly\n");
    fprintf(out, "; Format: OPCODE [operands] [type info]\n\n");

    /* IR generation is similar to assembly but with more abstract operations */
    CodeGenContext cg;
    cg_init(&cg, out);

    FunctionContext fn = {0};
    fn.cg = &cg;
    fn.scope = global;

    for (AST *p = root->child; p; p = p->sibling) {
        if (p->kind == NODE_FUNC_DECL) {
            fprintf(out, "FUNCTION %s\n", p->name ? p->name : "anon");
            fprintf(out, "  PROLOGUE\n");
            
            SymTable *fnScope = symtable_find_scope(global, p->name, global);
            if (!fnScope)
                fnScope = symtable_find_scope(global, p->name, NULL);
            
            AST *body = p->extra;
            if (body) {
                for (AST *stmt = body->child; stmt; stmt = stmt->sibling) {
                    if (stmt->kind == NODE_ASSIGN) {
                        AST *lhs = stmt->child;
                        fprintf(out, "  ASSIGN %s\n", lhs ? lhs->name : "?");
                    } else if (stmt->kind == NODE_RETURN) {
                        fprintf(out, "  RETURN\n");
                    }
                }
            }
            fprintf(out, "  EPILOGUE\n\n");
        }
    }

    fclose(out);
    return 0;
}

/* =====================================================
   MACHINE CODE GENERATION (Relocatable and Absolute)
   ===================================================== */

/* Instruction opcodes for our register machine */
typedef enum {
    OP_NOP = 0x00,
    OP_LOAD = 0x01,
    OP_STORE = 0x02,
    OP_LOADI = 0x03,
    OP_LOADF = 0x04,
    OP_MOV = 0x05,
    OP_ADD = 0x10,
    OP_SUB = 0x11,
    OP_MUL = 0x12,
    OP_DIV = 0x13,
    OP_AND = 0x14,
    OP_OR = 0x15,
    OP_NOT = 0x16,
    OP_NEG = 0x17,
    OP_CMPEQ = 0x20,
    OP_CMPNE = 0x21,
    OP_CMPLT = 0x22,
    OP_CMPGT = 0x23,
    OP_CMPLE = 0x24,
    OP_CMPGE = 0x25,
    OP_JMP = 0x30,
    OP_JZ = 0x31,
    OP_CALL = 0x40,
    OP_RET = 0x41,
    OP_PUSH = 0x50,
    OP_POP = 0x51,
    OP_READ = 0x60,
    OP_WRITE = 0x61
} OpCode;

typedef struct {
    char *label;
    int address;
} LabelEntry;

typedef struct {
    int address;
    char *symbol;
    int type;  /* 0=absolute, 1=relative */
} RelocEntry;

static LabelEntry *labels = NULL;
static int labelCount = 0;
static RelocEntry *relocs = NULL;
static int relocCount = 0;

static int find_label_address(const char *label) {
    for (int i = 0; i < labelCount; i++) {
        if (labels[i].label && strcmp(labels[i].label, label) == 0)
            return labels[i].address;
    }
    return -1;
}

static void add_reloc(int addr, const char *sym, int type) {
    relocs = realloc(relocs, (relocCount + 1) * sizeof(RelocEntry));
    relocs[relocCount].address = addr;
    relocs[relocCount].symbol = strdup(sym);
    relocs[relocCount].type = type;
    relocCount++;
}

static int encode_instruction(FILE *out, OpCode op, int reg1, int reg2, int reg3, int imm, int is_relocatable, int is_absolute) {
    int addr = ftell(out);
    char opname[16];
    
    /* Map opcode to name */
    switch (op) {
        case OP_LOADI: strcpy(opname, "LOADI"); break;
        case OP_ADD: strcpy(opname, "ADD"); break;
        case OP_RET: strcpy(opname, "RET"); break;
        case OP_JMP: strcpy(opname, "JMP"); break;
        default: strcpy(opname, "UNKNOWN"); break;
    }
    
    /* Write instruction in readable format */
    fprintf(out, "  0x%04X: %s", addr, opname);
    if (reg1 >= 0) fprintf(out, " R%d", reg1);
    if (reg2 >= 0) fprintf(out, " R%d", reg2);
    if (reg3 >= 0) fprintf(out, " R%d", reg3);
    if (imm >= 0) {
        if (is_relocatable) {
            fprintf(out, " <RELOC>");
            add_reloc(addr, "SYMBOL", 1);
        } else {
            if (is_absolute)
                fprintf(out, " 0x%04X", imm);
            else
                fprintf(out, " %d", imm);
        }
    }
    fprintf(out, "\n");
    
    return 4; /* Estimated instruction size */
}

int codegen_generate_relocatable(const char *asmPath, const char *outPath) {
    FILE *in = fopen(asmPath, "r");
    if (!in) return 1;
    
    FILE *out = fopen(outPath, "w");
    if (!out) {
        fclose(in);
        return 1;
    }

    /* Write relocatable object file header (text format for readability) */
    fprintf(out, "RELOCATABLE_OBJECT\n");
    fprintf(out, "FORMAT_VERSION: 1.0\n");
    fprintf(out, "ARCHITECTURE: SimpleRegisterMachine\n");
    fprintf(out, "WORD_SIZE: 8\n\n");
    fprintf(out, "CODE_SECTION:\n");

    char line[256];
    int address = 0;
    
    /* First pass: collect labels */
    while (fgets(line, sizeof(line), in)) {
        if (strstr(line, "FUNC_") && strstr(line, ":")) {
            char label[64];
            if (sscanf(line, "%63[^:]:", label) == 1) {
                labels = realloc(labels, (labelCount + 1) * sizeof(LabelEntry));
                labels[labelCount].label = strdup(label);
                labels[labelCount].address = address;
                labelCount++;
            }
        }
        /* Estimate instruction size */
        if (strstr(line, "LOAD") || strstr(line, "STORE") || strstr(line, "ADD") || strstr(line, "SUB"))
            address += 4;
        else if (strstr(line, "JMP") || strstr(line, "JZ") || strstr(line, "CALL"))
            address += 4;
        else if (strstr(line, "RET") || strstr(line, "PUSH") || strstr(line, "POP"))
            address += 2;
    }
    
    rewind(in);
    address = 0;
    
    /* Second pass: generate code */
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == ';' || line[0] == '\n') continue;
        if (strstr(line, ":")) continue; /* Skip labels in binary output */
        
        int reg1 = -1, reg2 = -1, reg3 = -1, imm = -1;
        
        if (strstr(line, "LOADI")) {
            if (sscanf(line, " LOADI R%d, %d", &reg1, &imm) == 2) {
                encode_instruction(out, OP_LOADI, reg1, -1, -1, imm, 0, 0);
                address += 4;
            }
        } else if (strstr(line, "ADD")) {
            if (sscanf(line, " ADD R%d, R%d, R%d", &reg1, &reg2, &reg3) == 3) {
                encode_instruction(out, OP_ADD, reg1, reg2, reg3, -1, 0, 0);
                address += 4;
            }
        } else if (strstr(line, "RET")) {
            encode_instruction(out, OP_RET, -1, -1, -1, -1, 0, 0);
            address += 2;
        } else if (strstr(line, "JMP")) {
            char label[64];
            if (sscanf(line, " JMP %63s", label) == 1) {
                encode_instruction(out, OP_JMP, -1, -1, -1, 0, 1, 0);
                add_reloc(address, label, 1);
                address += 4;
            }
        }
    }
    
    fprintf(out, "\nRELOCATION_TABLE:\n");
    for (int i = 0; i < relocCount; i++) {
        fprintf(out, "  %d: %s (type=%d)\n", relocs[i].address, relocs[i].symbol, relocs[i].type);
    }
    
    fprintf(out, "\nSYMBOL_TABLE:\n");
    for (int i = 0; i < labelCount; i++) {
        fprintf(out, "  %s: %d\n", labels[i].label, labels[i].address);
    }
    
    fclose(in);
    fclose(out);
    
    /* Free memory */
    for (int i = 0; i < labelCount; i++) free(labels[i].label);
    for (int i = 0; i < relocCount; i++) free(relocs[i].symbol);
    free(labels);
    free(relocs);
    labels = NULL;
    relocs = NULL;
    labelCount = relocCount = 0;
    
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
    fprintf(out, "ARCHITECTURE: SimpleRegisterMachine\n");
    fprintf(out, "LOAD_ADDRESS: 0x1000\n\n");
    fprintf(out, "CODE_SECTION:\n");

    char line[256];
    int baseAddress = 0x1000;
    int address = baseAddress;
    
    /* Collect labels first */
    while (fgets(line, sizeof(line), in)) {
        if (strstr(line, "FUNC_") && strstr(line, ":")) {
            char label[64];
            if (sscanf(line, "%63[^:]:", label) == 1) {
                labels = realloc(labels, (labelCount + 1) * sizeof(LabelEntry));
                labels[labelCount].label = strdup(label);
                labels[labelCount].address = address;
                labelCount++;
            }
        }
        if (strstr(line, "LOAD") || strstr(line, "STORE") || strstr(line, "ADD") || strstr(line, "SUB"))
            address += 4;
        else if (strstr(line, "JMP") || strstr(line, "JZ") || strstr(line, "CALL"))
            address += 4;
        else if (strstr(line, "RET") || strstr(line, "PUSH") || strstr(line, "POP"))
            address += 2;
    }
    
    rewind(in);
    address = baseAddress;
    
    /* Generate absolute code with resolved addresses */
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == ';' || line[0] == '\n') continue;
        if (strstr(line, ":")) continue;
        
        int reg1 = -1, reg2 = -1, reg3 = -1, imm = -1;
        
        if (strstr(line, "LOADI")) {
            if (sscanf(line, " LOADI R%d, %d", &reg1, &imm) == 2) {
                encode_instruction(out, OP_LOADI, reg1, -1, -1, imm, 0, 1);
                address += 4;
            }
        } else if (strstr(line, "ADD")) {
            if (sscanf(line, " ADD R%d, R%d, R%d", &reg1, &reg2, &reg3) == 3) {
                encode_instruction(out, OP_ADD, reg1, reg2, reg3, -1, 0, 1);
                address += 4;
            }
        } else if (strstr(line, "RET")) {
            encode_instruction(out, OP_RET, -1, -1, -1, -1, 0, 1);
            address += 2;
        } else if (strstr(line, "JMP")) {
            char label[64];
            if (sscanf(line, " JMP %63s", label) == 1) {
                int targetAddr = find_label_address(label);
                if (targetAddr < 0) targetAddr = address + 4; /* Default: next instruction */
                encode_instruction(out, OP_JMP, -1, -1, -1, targetAddr, 0, 1);
                address += 4;
            }
        }
    }
    
    fprintf(out, "\nSYMBOL_TABLE:\n");
    for (int i = 0; i < labelCount; i++) {
        fprintf(out, "  %s: 0x%04X\n", labels[i].label, labels[i].address);
    }
    
    fclose(in);
    fclose(out);
    
    /* Free memory */
    for (int i = 0; i < labelCount; i++) free(labels[i].label);
    free(labels);
    labels = NULL;
    labelCount = 0;
    
    return 0;
}

