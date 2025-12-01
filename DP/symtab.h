#ifndef SYMTAB_H
#define SYMTAB_H

#include "common.h"

#define MAX_SYMBOLS 1000

extern SymbolEntry symbolTable[MAX_SYMBOLS];
extern int nextSymbolIndex;

/* Install a new symbol or return the existing one. */
SymbolEntry *install(const char *lexeme, int type);

/* Look up a symbol by lexeme, or NULL if not present. */
SymbolEntry *lookup(const char *lexeme);

/* Print the entire symbol table. */
void print_symbol_table(void);

#endif /* SYMTAB_H */


