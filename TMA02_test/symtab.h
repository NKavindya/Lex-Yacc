#ifndef SYMTAB_H
#define SYMTAB_H

#include "common.h" // Include common definitions

// --- Global Symbol Table ---
#define MAX_SYMBOLS 1000
extern SymbolEntry symbolTable[MAX_SYMBOLS];
extern int nextSymbolIndex;

// --- Function Prototypes ---
// FIX: Changed TokenType to int
SymbolEntry* install(const char *lexeme, int type);
SymbolEntry* lookup(const char *lexeme);
void print_symbol_table(void);

#endif // SYMTAB_H