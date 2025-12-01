#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symbol_table.h"

/* Generate Intermediate Representation (IR) */
int codegen_generate_ir(AST *root, SymTable *global, const char *outPath);

/* Generate x86-32 assembly code */
int codegen_generate(AST *root, SymTable *global, const char *outPath);

/* Generate relocatable machine code */
int codegen_generate_relocatable(const char *asmPath, const char *outPath);

/* Generate absolute machine code */
int codegen_generate_absolute(const char *asmPath, const char *outPath);

#endif /* CODEGEN_H */

