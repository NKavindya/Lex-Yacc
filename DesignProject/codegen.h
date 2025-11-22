#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symbol_table.h"

int codegen_generate(AST *root, SymTable *global, const char *outPath);

#endif

