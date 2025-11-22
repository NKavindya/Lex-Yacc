#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symbol_table.h"

/* generate assembly code */
int codegen_generate(AST *root, SymTable *global, const char *outPath);

/* generate Intermediate Representation (IR) */
int codegen_generate_ir(AST *root, SymTable *global, const char *outPath);

/* generate relocatable machine code */
int codegen_generate_relocatable(const char *asmPath, const char *outPath);

/* generate absolute machine code */
int codegen_generate_absolute(const char *asmPath, const char *outPath);

#endif
