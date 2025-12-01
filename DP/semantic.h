#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

extern SymTable *globalTable;
extern FILE *errFile;

void semantic_passA(AST *root);
void semantic_passB(AST *root);

#endif


