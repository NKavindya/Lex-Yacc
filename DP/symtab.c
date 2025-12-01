#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

SymbolEntry symbolTable[MAX_SYMBOLS];
int nextSymbolIndex = 0;

SymbolEntry *lookup(const char *lexeme) {
    for (int i = 0; i < nextSymbolIndex; ++i) {
        if (strcmp(symbolTable[i].lexeme, lexeme) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

SymbolEntry *install(const char *lexeme, int type) {
    SymbolEntry *existing = lookup(lexeme);
    if (existing) {
        return existing;
    }

    if (nextSymbolIndex >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table overflow\n");
        exit(EXIT_FAILURE);
    }

    SymbolEntry *entry = &symbolTable[nextSymbolIndex++];
    entry->lexeme = strdup(lexeme);
    entry->type = type;
    return entry;
}

void print_symbol_table(void) {
    printf("\n--- Symbol Table ---\n");
    printf("%-5s %-20s %s\n", "#", "Lexeme", "TokenType");
    for (int i = 0; i < nextSymbolIndex; ++i) {
        printf("%-5d %-20s %d\n", i, symbolTable[i].lexeme, symbolTable[i].type);
    }
    printf("--------------------\n");
}


