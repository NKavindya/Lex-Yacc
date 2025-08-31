#ifndef COMMON_H
#define COMMON_H

// --- Symbol Table Entry Structure ---
// Represents an entry in the symbol table, primarily for identifiers.
typedef struct {
    char *lexeme;     // The actual string of the identifier
    int type;         // The type of token (will correspond to Bison's token ID, e.g., IDENTIFIER)
    // Add more fields here for semantic analysis later (e.g., data type, scope, value)
    // int value;        // Example: for a simple variable in this context
} SymbolEntry;

#endif // COMMON_H