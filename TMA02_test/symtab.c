#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h" // Include symbol table definitions

SymbolEntry symbolTable[MAX_SYMBOLS];
int nextSymbolIndex = 0;

// FIX: Changed TokenType to int
SymbolEntry* install(const char *lexeme, int type) {
    // First, check if the symbol already exists
    SymbolEntry *entry = lookup(lexeme);
    if (entry != NULL) {
        return entry; // Already exists, return existing entry
    }

    // If not, add new symbol
    if (nextSymbolIndex >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table overflow!\n");
        exit(1);
    }

    SymbolEntry *newEntry = &symbolTable[nextSymbolIndex];
    newEntry->lexeme = strdup(lexeme); // Duplicate the string
    newEntry->type = type;
    // Initialize other fields if necessary
    nextSymbolIndex++;
    return newEntry;
}

// Function to look up a symbol in the symbol table
SymbolEntry* lookup(const char *lexeme) {
    for (int i = 0; i < nextSymbolIndex; i++) {
        if (strcmp(symbolTable[i].lexeme, lexeme) == 0) {
            return &symbolTable[i]; // Found
        }
    }
    return NULL; // Not found
}

// Function to print the contents of the symbol table
void print_symbol_table(void) {
    printf("\n--- Symbol Table Contents ---\n");
    printf("%-20s %s\n", "Lexeme", "Type (Bison ID)");
    printf("----------------------------------\n");
    for (int i = 0; i < nextSymbolIndex; i++) {
        printf("%-20s %d\n", symbolTable[i].lexeme, symbolTable[i].type);
    }
    printf("----------------------------------\n");
}