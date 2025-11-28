#include "machine_code.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 512
#define BASE_ADDRESS 0x1000  /* Default base address for absolute code */

/* Instruction opcodes */
typedef enum {
    OP_LOAD = 0x01,
    OP_STORE = 0x02,
    OP_LOADI = 0x03,
    OP_LOADF = 0x04,
    OP_LOAD_STR = 0x05,
    OP_MOV = 0x06,
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
    OP_CALL = 0x32,
    OP_RET = 0x33,
    OP_PUSH = 0x40,
    OP_POP = 0x41,
    OP_READ = 0x50,
    OP_WRITE = 0x51,
    OP_NOP = 0x00
} OpCode;

/* Relocation type */
typedef enum {
    RELOC_NONE = 0,
    RELOC_ABSOLUTE = 1,    /* Absolute address */
    RELOC_RELATIVE = 2,    /* Relative address (for jumps) */
    RELOC_LABEL = 3        /* Label reference */
} RelocType;

typedef struct {
    int address;
    RelocType type;
    char label[64];
    int offset;
} RelocEntry;

static RelocEntry reloc_table[1024];
static int reloc_count = 0;
static int current_address = 0;

/* Symbol table for labels */
typedef struct {
    char name[64];
    int address;
} LabelEntry;

static LabelEntry label_table[256];
static int label_count = 0;

static int get_register_index(const char *reg) {
    if (!reg || strlen(reg) < 2) return -1;
    if (reg[0] == 'R' && isdigit(reg[1])) {
        int idx = atoi(reg + 1);
        if (idx >= 0 && idx <= 7) return idx;
    }
    return -1;
}

static int find_label_address(const char *label) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(label_table[i].name, label) == 0) {
            return label_table[i].address;
        }
    }
    return -1;
}

static void add_label(const char *name, int address) {
    if (label_count < 256) {
        strncpy(label_table[label_count].name, name, sizeof(label_table[0].name) - 1);
        label_table[label_count].address = address;
        label_count++;
    }
}

static void add_reloc(int address, RelocType type, const char *label, int offset) {
    if (reloc_count < 1024) {
        reloc_table[reloc_count].address = address;
        reloc_table[reloc_count].type = type;
        if (label) {
            strncpy(reloc_table[reloc_count].label, label, sizeof(reloc_table[0].label) - 1);
        }
        reloc_table[reloc_count].offset = offset;
        reloc_count++;
    }
}

static OpCode parse_opcode(const char *mnemonic) {
    if (strcmp(mnemonic, "LOAD") == 0) return OP_LOAD;
    if (strcmp(mnemonic, "STORE") == 0) return OP_STORE;
    if (strcmp(mnemonic, "LOADI") == 0) return OP_LOADI;
    if (strcmp(mnemonic, "LOADF") == 0) return OP_LOADF;
    if (strcmp(mnemonic, "LOAD_STR") == 0) return OP_LOAD_STR;
    if (strcmp(mnemonic, "MOV") == 0) return OP_MOV;
    if (strcmp(mnemonic, "ADD") == 0) return OP_ADD;
    if (strcmp(mnemonic, "SUB") == 0) return OP_SUB;
    if (strcmp(mnemonic, "MUL") == 0) return OP_MUL;
    if (strcmp(mnemonic, "DIV") == 0) return OP_DIV;
    if (strcmp(mnemonic, "AND") == 0) return OP_AND;
    if (strcmp(mnemonic, "OR") == 0) return OP_OR;
    if (strcmp(mnemonic, "NOT") == 0) return OP_NOT;
    if (strcmp(mnemonic, "NEG") == 0) return OP_NEG;
    if (strcmp(mnemonic, "CMPEQ") == 0) return OP_CMPEQ;
    if (strcmp(mnemonic, "CMPNE") == 0) return OP_CMPNE;
    if (strcmp(mnemonic, "CMPLT") == 0) return OP_CMPLT;
    if (strcmp(mnemonic, "CMPGT") == 0) return OP_CMPGT;
    if (strcmp(mnemonic, "CMPLE") == 0) return OP_CMPLE;
    if (strcmp(mnemonic, "CMPGE") == 0) return OP_CMPGE;
    if (strcmp(mnemonic, "JMP") == 0) return OP_JMP;
    if (strcmp(mnemonic, "JZ") == 0) return OP_JZ;
    if (strcmp(mnemonic, "CALL") == 0) return OP_CALL;
    if (strcmp(mnemonic, "RET") == 0) return OP_RET;
    if (strcmp(mnemonic, "PUSH") == 0) return OP_PUSH;
    if (strcmp(mnemonic, "POP") == 0) return OP_POP;
    if (strcmp(mnemonic, "READ") == 0) return OP_READ;
    if (strcmp(mnemonic, "WRITE") == 0) return OP_WRITE;
    return OP_NOP;
}

static int parse_immediate(const char *str) {
    if (!str) return 0;
    if (str[0] == '#') {
        return atoi(str + 1);
    }
    return atoi(str);
}

static int parse_memory_operand(const char *str, int *offset, char *label) {
    if (!str) return 0;
    if (str[0] == '[') {
        const char *p = str + 1;
        if (strncmp(p, "FP-", 3) == 0) {
            *offset = atoi(p + 3);
            return 1; /* Stack offset */
        } else if (strncmp(p, "FP+", 3) == 0) {
            *offset = atoi(p + 3);
            return 1;
        } else {
            /* Label reference */
            const char *end = strchr(p, ']');
            if (end && label) {
                int len = end - p;
                if (len < 64) {
                    strncpy(label, p, len);
                    label[len] = '\0';
                    return 2; /* Label */
                }
            }
        }
    }
    return 0;
}

/* First pass: collect labels */
static void pass1_collect_labels(FILE *asmFile) {
    char line[MAX_LINE_LEN];
    current_address = 0;
    label_count = 0;

    while (fgets(line, sizeof(line), asmFile)) {
        char *p = line;
        while (*p && isspace(*p)) p++;
        if (*p == '\0' || *p == ';' || *p == '\n') continue;

        /* Check for label (ends with ':') */
        char *colon = strchr(p, ':');
        if (colon && colon > p) {
            char label[64];
            int len = colon - p;
            if (len < 64) {
                strncpy(label, p, len);
                label[len] = '\0';
                /* Remove trailing whitespace */
                while (len > 0 && isspace(label[len-1])) len--;
                label[len] = '\0';
                add_label(label, current_address);
            }
        }

        /* Count instruction size */
        char mnemonic[32];
        if (sscanf(p, "%31s", mnemonic) == 1) {
            OpCode op = parse_opcode(mnemonic);
            if (op != OP_NOP || strcmp(mnemonic, "NOP") == 0) {
                current_address += 4; /* Each instruction is 4 bytes */
            }
        }
    }
    rewind(asmFile);
}

/* Second pass: generate relocatable code */
static void pass2_generate_relocatable(FILE *asmFile, FILE *outFile) {
    char line[MAX_LINE_LEN];
    current_address = 0;
    reloc_count = 0;

    fprintf(outFile, "; Relocatable Machine Code\n");
    fprintf(outFile, "; Format: Address | Opcode | Operands | Relocation Info\n\n");

    while (fgets(line, sizeof(line), asmFile)) {
        char *p = line;
        while (*p && isspace(*p)) p++;
        if (*p == '\0' || *p == ';' || *p == '\n') continue;

        /* Skip labels (already processed) */
        char *colon = strchr(p, ':');
        if (colon && colon > p) {
            p = colon + 1;
            while (*p && isspace(*p)) p++;
            if (*p == '\0' || *p == ';' || *p == '\n') continue;
        }

        char mnemonic[32];
        if (sscanf(p, "%31s", mnemonic) != 1) continue;

        OpCode op = parse_opcode(mnemonic);
        if (op == OP_NOP && strcmp(mnemonic, "NOP") != 0) continue;

        fprintf(outFile, "%04X: ", current_address);
        fprintf(outFile, "%02X ", op);

        /* Parse operands based on instruction type */
        p += strlen(mnemonic);
        while (*p && isspace(*p)) p++;

        if (op == OP_LOADI || op == OP_LOADF) {
            /* LOADI reg, immediate */
            char reg[8];
            int imm = 0;
            if (sscanf(p, "%7s , %d", reg, &imm) == 2 || 
                sscanf(p, "%7s , #%d", reg, &imm) == 2) {
                int regIdx = get_register_index(reg);
                fprintf(outFile, "%02X %08X ", regIdx, (unsigned int)imm);
                fprintf(outFile, "| R:NONE");
            }
        } else if (op == OP_LOAD || op == OP_STORE) {
            /* LOAD/STORE reg, [address] */
            char reg[8], addr[64];
            if (sscanf(p, "%7s , %63s", reg, addr) == 2) {
                int regIdx = get_register_index(reg);
                int offset = 0;
                char label[64] = {0};
                int addrType = parse_memory_operand(addr, &offset, label);
                fprintf(outFile, "%02X ", regIdx);
                if (addrType == 1) {
                    /* Stack offset */
                    fprintf(outFile, "%08X ", (unsigned int)offset);
                    fprintf(outFile, "| R:NONE");
                } else if (addrType == 2) {
                    /* Label reference */
                    fprintf(outFile, "00000000 | R:LABEL:%s", label);
                    add_reloc(current_address + 2, RELOC_LABEL, label, 0);
                }
            }
        } else if (op == OP_MOV) {
            /* MOV reg1, reg2 */
            char reg1[8], reg2[8];
            if (sscanf(p, "%7s , %7s", reg1, reg2) == 2) {
                int idx1 = get_register_index(reg1);
                int idx2 = get_register_index(reg2);
                fprintf(outFile, "%02X %02X ", idx1, idx2);
                fprintf(outFile, "| R:NONE");
            }
        } else if (op == OP_ADD || op == OP_SUB || op == OP_MUL || op == OP_DIV ||
                   op == OP_AND || op == OP_OR || op == OP_CMPEQ || op == OP_CMPNE ||
                   op == OP_CMPLT || op == OP_CMPGT || op == OP_CMPLE || op == OP_CMPGE) {
            /* Binary op: reg1, reg1, reg2 */
            char reg1[8], reg2[8];
            if (sscanf(p, "%7s , %7s , %7s", reg1, reg1, reg2) == 2 ||
                sscanf(p, "%7s , %7s , %7s", reg1, reg1, reg2) == 3) {
                int idx1 = get_register_index(reg1);
                int idx2 = get_register_index(reg2);
                fprintf(outFile, "%02X %02X ", idx1, idx2);
                fprintf(outFile, "| R:NONE");
            }
        } else if (op == OP_NOT || op == OP_NEG) {
            /* Unary op: reg, reg */
            char reg[8];
            if (sscanf(p, "%7s , %7s", reg, reg) == 2) {
                int idx = get_register_index(reg);
                fprintf(outFile, "%02X 00 ", idx);
                fprintf(outFile, "| R:NONE");
            }
        } else if (op == OP_JMP || op == OP_JZ || op == OP_CALL) {
            /* Jump/Call: label */
            char reg[8] = {0}, label[64];
            if (op == OP_JZ) {
                if (sscanf(p, "%7s , %63s", reg, label) == 2) {
                    int regIdx = get_register_index(reg);
                    fprintf(outFile, "%02X ", regIdx);
                }
            } else {
                if (sscanf(p, "%63s", label) == 1) {
                    fprintf(outFile, "00 ");
                }
            }
            /* Remove FUNC_ prefix if present */
            if (strncmp(label, "FUNC_", 5) == 0) {
                memmove(label, label + 5, strlen(label + 5) + 1);
            }
            int targetAddr = find_label_address(label);
            if (targetAddr >= 0) {
                fprintf(outFile, "%08X ", (unsigned int)targetAddr);
                fprintf(outFile, "| R:RELATIVE");
                add_reloc(current_address + 2, RELOC_RELATIVE, label, targetAddr - current_address - 4);
            } else {
                fprintf(outFile, "00000000 | R:LABEL:%s", label);
                add_reloc(current_address + 2, RELOC_LABEL, label, 0);
            }
        } else if (op == OP_PUSH || op == OP_POP || op == OP_READ || op == OP_WRITE) {
            /* Single register operand */
            char reg[8];
            if (sscanf(p, "%7s", reg) == 1) {
                int idx = get_register_index(reg);
                fprintf(outFile, "%02X 00 00 ", idx);
                fprintf(outFile, "| R:NONE");
            }
        } else if (op == OP_RET || op == OP_NOP) {
            fprintf(outFile, "00 00 00 | R:NONE");
        }

        fprintf(outFile, "\n");
        current_address += 4; /* Each instruction is 4 bytes */
    }
}

int generate_relocatable_code(const char *asmPath, const char *outPath) {
    FILE *asmFile = fopen(asmPath, "r");
    if (!asmFile) {
        fprintf(stderr, "Error: Cannot open assembly file %s\n", asmPath);
        return 1;
    }

    FILE *outFile = fopen(outPath, "w");
    if (!outFile) {
        fprintf(stderr, "Error: Cannot create output file %s\n", outPath);
        fclose(asmFile);
        return 1;
    }

    /* Pass 1: Collect labels */
    pass1_collect_labels(asmFile);

    /* Pass 2: Generate relocatable code */
    pass2_generate_relocatable(asmFile, outFile);

    /* Write relocation table */
    fprintf(outFile, "\n; Relocation Table\n");
    fprintf(outFile, "; Address | Type | Label | Offset\n");
    for (int i = 0; i < reloc_count; i++) {
        fprintf(outFile, "%04X | ", reloc_table[i].address);
        switch (reloc_table[i].type) {
            case RELOC_ABSOLUTE: fprintf(outFile, "ABS"); break;
            case RELOC_RELATIVE: fprintf(outFile, "REL"); break;
            case RELOC_LABEL: fprintf(outFile, "LABEL"); break;
            default: fprintf(outFile, "NONE"); break;
        }
        fprintf(outFile, " | %s | %d\n", reloc_table[i].label, reloc_table[i].offset);
    }

    fclose(asmFile);
    fclose(outFile);
    return 0;
}

int generate_absolute_code(const char *relocPath, const char *outPath, int baseAddress) {
    FILE *relocFile = fopen(relocPath, "r");
    if (!relocFile) {
        fprintf(stderr, "Error: Cannot open relocatable file %s\n", relocPath);
        return 1;
    }

    FILE *outFile = fopen(outPath, "w");
    if (!outFile) {
        fprintf(stderr, "Error: Cannot create output file %s\n", outPath);
        fclose(relocFile);
        return 1;
    }

    fprintf(outFile, "; Absolute Machine Code\n");
    fprintf(outFile, "; Base Address: 0x%04X\n", baseAddress);
    fprintf(outFile, "; Format: Address | Opcode | Operands (hex)\n\n");

    char line[MAX_LINE_LEN];
    int address = baseAddress;

    while (fgets(line, sizeof(line), relocFile)) {
        if (line[0] == ';' || line[0] == '\n') {
            if (strstr(line, "Relocation Table")) {
                /* Skip relocation table section */
                while (fgets(line, sizeof(line), relocFile)) {
                    if (line[0] == '\n') break;
                }
                break;
            }
            continue;
        }

        /* Parse relocatable code line */
        unsigned int addr, opcode, op1, op2, op3;
        char reloc[128];
        if (sscanf(line, "%X: %X %X %X %X | %127s", &addr, &opcode, &op1, &op2, &op3, reloc) >= 5) {
            fprintf(outFile, "%04X: ", address);
            fprintf(outFile, "%02X %02X %02X %02X", opcode, op1, op2, op3);

            /* Apply relocation if needed */
            if (strncmp(reloc, "R:RELATIVE", 10) == 0 || strncmp(reloc, "R:LABEL", 7) == 0) {
                /* Extract target address from relocation info */
                char *labelStart = strstr(reloc, ":");
                if (labelStart) {
                    /* For now, keep relative offset - in real system would resolve */
                    fprintf(outFile, " ; reloc: %s", reloc);
                }
            }

            fprintf(outFile, "\n");
            address += 4;
        }
    }

    fclose(relocFile);
    fclose(outFile);
    return 0;
}

