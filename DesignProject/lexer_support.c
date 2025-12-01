/* 
 * lexer_support.c - Lexical Analysis Support Implementation
 * 
 * Implements data structures and functions for tracking lexical analysis results.
 * Maintains three linked lists:
 * 1. Symbol table: All identifiers, literals, and reserved words with frequency
 * 2. Token trace: Complete sequence of tokens recognized
 * 3. Error list: All lexical errors encountered
 */

#include "lexer_support.h"  // Function prototypes and data types
#include <stdio.h>          // fprintf, FILE operations
#include <stdlib.h>         // malloc, free
#include <string.h>         // strcmp, strlen, memcpy

/**
 * LexSymbolEntry - Entry in the lexical symbol table
 * 
 * Stores information about a lexical symbol (identifier, literal, or reserved word):
 * - lexeme: The actual text of the symbol
 * - kind: Category of symbol (identifier, literal type, reserved word)
 * - first_line, first_column: Location where symbol first appeared
 * - count: Number of times this symbol appeared in source
 * - next: Pointer to next entry in linked list
 */
typedef struct LexSymbolEntry {
    char *lexeme;           // The symbol text (e.g., "x", "42", "class")
    LexSymbolKind kind;     // Category: identifier, int_literal, float_literal, etc.
    int first_line;         // Line number where symbol first appeared
    int first_column;       // Column number where symbol first appeared
    int count;              // Frequency: how many times this symbol appeared
    struct LexSymbolEntry *next;  // Next entry in linked list
} LexSymbolEntry;

/**
 * TokenRecord - Entry in the token trace
 * 
 * Records a single token recognized during lexical analysis.
 * Maintains a chronological list of all tokens for debugging.
 * 
 * - tokenType: Numeric token type (from parser.tab.h)
 * - tokenName: Human-readable name (e.g., "ID", "INT_LIT", "PLUS")
 * - lexeme: The actual text that was tokenized
 * - line, column: Source location
 * - next: Next token in sequence
 */
typedef struct TokenRecord {
    int tokenType;          // Numeric token type (from Bison-generated parser.tab.h)
    char *tokenName;        // Human-readable token name
    char *lexeme;           // The actual text that was tokenized
    int line;               // Source line number
    int column;             // Source column number
    struct TokenRecord *next;  // Next token in chronological sequence
} TokenRecord;

/**
 * LexError - Entry in the lexical error list
 * 
 * Records a lexical error encountered during analysis.
 * Errors include malformed tokens, unknown characters, unterminated comments.
 * 
 * - message: Description of the error
 * - line, column: Source location where error occurred
 * - next: Next error in list
 */
typedef struct LexError {
    char *message;          // Error message describing the problem
    int line;               // Source line number where error occurred
    int column;             // Source column number where error occurred
    struct LexError *next;  // Next error in list
} LexError;

/* Global linked list heads and tails for tracking lexical analysis data */
static LexSymbolEntry *symbol_head = NULL;  // Head of symbol table linked list
static TokenRecord *token_head = NULL;       // Head of token trace linked list
static TokenRecord *token_tail = NULL;       // Tail of token trace (for efficient appending)
static LexError *error_head = NULL;          // Head of error list linked list
static LexError *error_tail = NULL;          // Tail of error list (for efficient appending)

/**
 * dup_string - Duplicate a string
 * 
 * Helper function to create a copy of a string.
 * Allocates memory and copies the string content.
 * 
 * @param s Source string to duplicate (can be NULL)
 * @return Pointer to newly allocated string copy, or NULL if s is NULL or allocation fails
 */
static char *dup_string(const char *s) {
    if (!s) return NULL;                    // Return NULL if input is NULL
    size_t len = strlen(s);                 // Get length of source string
    char *copy = (char*)malloc(len + 1);    // Allocate memory for copy (+1 for null terminator)
    if (!copy) return NULL;                  // Return NULL if allocation fails
    memcpy(copy, s, len + 1);               // Copy string including null terminator
    return copy;                             // Return pointer to copy
}

/**
 * lex_support_init - Initialize lexical analysis support structures
 * 
 * Prepares data structures for tracking symbols, tokens, and errors.
 * Calls finalize first to ensure clean state (in case of re-initialization).
 */
void lex_support_init(void) {
    lex_support_finalize();  // Clean up any existing data first
}

/**
 * lex_support_finalize - Free all memory allocated for lexical analysis data
 * 
 * Cleans up all three linked lists:
 * 1. Symbol table: Frees all symbol entries and their lexeme strings
 * 2. Token trace: Frees all token records and their strings
 * 3. Error list: Frees all error entries and their messages
 * 
 * Should be called at the end of compilation to prevent memory leaks.
 */
void lex_support_finalize(void) {
    // Free symbol table linked list
    while (symbol_head) {
        LexSymbolEntry *next = symbol_head->next;  // Save next entry
        free(symbol_head->lexeme);                  // Free lexeme string
        free(symbol_head);                          // Free entry structure
        symbol_head = next;                         // Move to next entry
    }
    symbol_head = NULL;  // Reset head pointer

    // Free token trace linked list
    while (token_head) {
        TokenRecord *next = token_head->next;  // Save next token
        free(token_head->tokenName);           // Free token name string
        free(token_head->lexeme);               // Free lexeme string
        free(token_head);                       // Free token record
        token_head = next;                      // Move to next token
    }
    token_tail = NULL;  // Reset tail pointer

    // Free error list linked list
    while (error_head) {
        LexError *next = error_head->next;  // Save next error
        free(error_head->message);          // Free error message string
        free(error_head);                   // Free error entry
        error_head = next;                  // Move to next error
    }
    error_tail = NULL;  // Reset tail pointer
}

/**
 * find_symbol - Search for existing symbol in symbol table
 * 
 * Looks up a symbol in the symbol table by kind and lexeme.
 * Used to check if a symbol already exists before adding a new entry.
 * 
 * @param kind Category of symbol to search for
 * @param lexeme Text of symbol to search for
 * @return Pointer to existing entry if found, NULL otherwise
 */
static LexSymbolEntry *find_symbol(LexSymbolKind kind, const char *lexeme) {
    // Traverse symbol table linked list
    for (LexSymbolEntry *p = symbol_head; p; p = p->next) {
        // Check if kind and lexeme match
        if (p->kind == kind && strcmp(p->lexeme, lexeme) == 0) {
            return p;  // Found matching symbol
        }
    }
    return NULL;  // Symbol not found
}

/**
 * lex_support_record_symbol - Record a lexical symbol in the symbol table
 * 
 * Adds a symbol to the lexical symbol table or increments its count if it already exists.
 * The symbol table tracks frequency and first occurrence location for each symbol.
 * 
 * @param kind Category of symbol (identifier, literal type, reserved word)
 * @param lexeme The actual text of the symbol
 * @param line Source line number where symbol appears
 * @param column Source column number where symbol appears
 */
void lex_support_record_symbol(LexSymbolKind kind, const char *lexeme, int line, int column) {
    if (!lexeme) return;  // Safety check: ignore NULL lexemes
    
    // Check if symbol already exists in table
    LexSymbolEntry *entry = find_symbol(kind, lexeme);
    if (entry) {
        entry->count++;  // Increment frequency count
        return;          // Symbol already tracked, no need to add new entry
    }
    
    // Create new symbol entry
    entry = (LexSymbolEntry*)malloc(sizeof(LexSymbolEntry));
    if (!entry) return;  // Allocation failed, silently ignore
    
    // Initialize entry fields
    entry->lexeme = dup_string(lexeme);     // Duplicate lexeme string
    entry->kind = kind;                     // Set symbol category
    entry->first_line = line;               // Record first occurrence line
    entry->first_column = column;           // Record first occurrence column
    entry->count = 1;                       // Initialize frequency to 1
    entry->next = symbol_head;              // Insert at head of list
    symbol_head = entry;                     // Update head pointer
}

/**
 * lex_support_record_token - Record a token in the token trace
 * 
 * Adds a token to the chronological token trace.
 * The token trace records the complete sequence of tokens recognized during lexing.
 * 
 * @param tokenType Numeric token type (from parser.tab.h, currently unused but stored)
 * @param tokenName Human-readable token name (e.g., "ID", "INT_LIT", "PLUS")
 * @param lexeme The actual text that was tokenized
 * @param line Source line number
 * @param column Source column number
 */
void lex_support_record_token(int tokenType, const char *tokenName, const char *lexeme, int line, int column) {
    (void)tokenType;  // Suppress unused parameter warning (tokenType stored but not currently used)
    
    // Allocate memory for new token record
    TokenRecord *rec = (TokenRecord*)malloc(sizeof(TokenRecord));
    if (!rec) return;  // Allocation failed, silently ignore
    
    // Initialize token record fields
    rec->tokenType = tokenType;                                    // Store numeric token type
    rec->tokenName = dup_string(tokenName);                        // Duplicate token name string
    rec->lexeme = lexeme ? dup_string(lexeme) : dup_string("");    // Duplicate lexeme (or empty string if NULL)
    rec->line = line;                                              // Store source line
    rec->column = column;                                          // Store source column
    rec->next = NULL;                                              // Initialize next pointer
    
    // Append to end of token trace (maintain chronological order)
    if (!token_head) {
        // First token: initialize both head and tail
        token_head = token_tail = rec;
    } else {
        // Subsequent tokens: append to tail
        token_tail->next = rec;
        token_tail = rec;  // Update tail pointer
    }
}

/**
 * lex_support_record_error - Record a lexical error
 * 
 * Adds an error entry to the error list.
 * Lexical errors include malformed tokens, unknown characters, unterminated comments.
 * 
 * @param message Error message describing the problem (can be NULL, will use empty string)
 * @param line Source line number where error occurred
 * @param column Source column number where error occurred
 */
void lex_support_record_error(const char *message, int line, int column) {
    // Allocate memory for new error entry
    LexError *err = (LexError*)malloc(sizeof(LexError));
    if (!err) return;  // Allocation failed, silently ignore
    
    // Initialize error entry fields
    err->message = dup_string(message ? message : "");  // Duplicate message (or empty string if NULL)
    err->line = line;                                     // Store error line
    err->column = column;                                 // Store error column
    err->next = NULL;                                     // Initialize next pointer
    
    // Append to end of error list
    if (!error_head) {
        // First error: initialize both head and tail
        error_head = error_tail = err;
    } else {
        // Subsequent errors: append to tail
        error_tail->next = err;
        error_tail = err;  // Update tail pointer
    }
}

/**
 * lex_support_dump_symbols - Write lexical symbol table to file
 * 
 * Outputs the complete lexical symbol table showing:
 * - All symbols found (identifiers, literals, reserved words)
 * - Symbol category (kind)
 * - First occurrence location (line:column)
 * - Frequency count (how many times each symbol appeared)
 * 
 * Format: lexeme kind=category first=line:column count=frequency
 * 
 * @param out Output file stream (must be open for writing, checked for NULL)
 */
void lex_support_dump_symbols(FILE *out) {
    if (!out) return;  // Safety check: ignore NULL file pointer
    
    // Print header
    fprintf(out, "Lexical Symbol Table\n");
    fprintf(out, "====================\n");
    
    // Traverse symbol table linked list and print each entry
    for (LexSymbolEntry *p = symbol_head; p; p = p->next) {
        // Convert symbol kind enum to human-readable string
        const char *kind =
            (p->kind == LEXSYM_IDENTIFIER) ? "identifier" :
            (p->kind == LEXSYM_INT_LITERAL) ? "int_literal" :
            (p->kind == LEXSYM_FLOAT_LITERAL) ? "float_literal" :
            (p->kind == LEXSYM_STRING_LITERAL) ? "string_literal" :
            "reserved";
        
        // Print symbol entry: lexeme, kind, first occurrence, frequency
        fprintf(out, "%-15s kind=%-15s first=%d:%d count=%d\n",
                p->lexeme ? p->lexeme : "<nil>",  // Lexeme text (or <nil> if NULL)
                kind,                              // Symbol category
                p->first_line,                     // First occurrence line
                p->first_column,                   // First occurrence column
                p->count);                         // Frequency count
    }
}

/**
 * lex_support_dump_tokens - Write token trace to file
 * 
 * Outputs the complete sequence of tokens recognized during lexical analysis.
 * Each line shows: line:column token_name lexeme
 * 
 * This provides a chronological record of all tokens, useful for debugging
 * lexical analysis and understanding how the source was tokenized.
 * 
 * @param out Output file stream (must be open for writing, checked for NULL)
 */
void lex_support_dump_tokens(FILE *out) {
    if (!out) return;  // Safety check: ignore NULL file pointer
    
    // Print header
    fprintf(out, "Token Trace\n");
    fprintf(out, "===========\n");
    
    // Traverse token trace linked list and print each token
    for (TokenRecord *p = token_head; p; p = p->next) {
        // Print token: line:column token_name lexeme
        fprintf(out, "%4d:%-4d %-15s %s\n",
                p->line,                          // Source line number
                p->column,                        // Source column number
                p->tokenName ? p->tokenName : "<token>",  // Token name (or <token> if NULL)
                p->lexeme ? p->lexeme : "");      // Lexeme text (or empty string if NULL)
    }
}

/**
 * lex_support_dump_errors - Write lexical errors to file
 * 
 * Outputs all lexical errors encountered during analysis.
 * If no errors occurred, writes "No lexical errors detected."
 * 
 * Each error shows: line:column error_message
 * 
 * @param out Output file stream (must be open for writing, checked for NULL)
 */
void lex_support_dump_errors(FILE *out) {
    if (!out) return;  // Safety check: ignore NULL file pointer
    
    // Check if any errors were recorded
    if (!error_head) {
        fprintf(out, "No lexical errors detected.\n");
        return;  // No errors to report
    }
    
    // Print header
    fprintf(out, "Lexical Errors\n");
    fprintf(out, "==============\n");
    
    // Traverse error list linked list and print each error
    for (LexError *p = error_head; p; p = p->next) {
        // Print error: line:column error_message
        fprintf(out, "%4d:%-4d %s\n", 
                p->line,                                    // Error line number
                p->column,                                  // Error column number
                p->message ? p->message : "");              // Error message (or empty string if NULL)
    }
}
