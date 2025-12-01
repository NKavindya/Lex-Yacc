/* 
 * lexer_support.h - Lexical Analysis Support Functions
 * 
 * Provides infrastructure for tracking and reporting lexical analysis results:
 * - Symbol table: Tracks all identifiers, literals, and reserved words with frequency
 * - Token trace: Records sequence of all tokens recognized during lexing
 * - Error tracking: Collects lexical errors (malformed tokens, unknown characters)
 * 
 * Used for debugging, error reporting, and generating lexical analysis artifacts.
 */

#ifndef LEXER_SUPPORT_H
#define LEXER_SUPPORT_H

#include <stdio.h>  // For FILE type used in function signatures

/**
 * LexSymbolKind - Categories of lexical symbols
 * 
 * Used to classify symbols in the lexical symbol table:
 * - LEXSYM_IDENTIFIER: Variable names, function names, class names, etc.
 * - LEXSYM_INT_LITERAL: Integer constants (e.g., 42, 0, 100)
 * - LEXSYM_FLOAT_LITERAL: Floating-point constants (e.g., 3.14, 2.5e10)
 * - LEXSYM_STRING_LITERAL: String constants (e.g., "hello")
 * - LEXSYM_RESERVED: Reserved words/keywords (e.g., "class", "func", "if")
 */
typedef enum {
    LEXSYM_IDENTIFIER,      // User-defined identifiers (variables, functions, classes)
    LEXSYM_INT_LITERAL,     // Integer literal constants
    LEXSYM_FLOAT_LITERAL,   // Floating-point literal constants
    LEXSYM_STRING_LITERAL,  // String literal constants
    LEXSYM_RESERVED         // Reserved words/keywords
} LexSymbolKind;

/* Function prototypes for lexical analysis support */

/**
 * lex_support_init - Initialize lexical analysis support structures
 * 
 * Sets up data structures for tracking symbols, tokens, and errors.
 * Should be called once at the start of compilation.
 */
void lex_support_init(void);

/**
 * lex_support_record_symbol - Record a lexical symbol in the symbol table
 * 
 * Adds or updates an entry in the lexical symbol table.
 * If the symbol already exists, increments its count.
 * 
 * @param kind Category of the symbol (identifier, literal, reserved word)
 * @param lexeme The actual text of the symbol
 * @param line Source line number where symbol first appears
 * @param column Source column number where symbol first appears
 */
void lex_support_record_symbol(LexSymbolKind kind, const char *lexeme, int line, int column);

/**
 * lex_support_record_token - Record a token in the token trace
 * 
 * Adds an entry to the token trace, which records the sequence of all tokens
 * recognized during lexical analysis. Used for debugging and generating token logs.
 * 
 * @param tokenType Numeric token type (from parser.tab.h)
 * @param tokenName Human-readable token name (e.g., "ID", "INT_LIT", "PLUS")
 * @param lexeme The actual text that was tokenized
 * @param line Source line number
 * @param column Source column number
 */
void lex_support_record_token(int tokenType, const char *tokenName, const char *lexeme, int line, int column);

/**
 * lex_support_record_error - Record a lexical error
 * 
 * Adds an error entry to the error list. Lexical errors include:
 * - Malformed tokens (e.g., invalid float literals)
 * - Unknown characters
 * - Unterminated comments
 * 
 * @param message Error message describing the problem
 * @param line Source line number where error occurred
 * @param column Source column number where error occurred
 */
void lex_support_record_error(const char *message, int line, int column);

/**
 * lex_support_dump_symbols - Write lexical symbol table to file
 * 
 * Outputs the lexical symbol table showing:
 * - All symbols found (identifiers, literals, reserved words)
 * - Symbol category (kind)
 * - First occurrence location (line:column)
 * - Frequency count (how many times symbol appeared)
 * 
 * @param out Output file stream (must be open for writing)
 */
void lex_support_dump_symbols(FILE *out);

/**
 * lex_support_dump_tokens - Write token trace to file
 * 
 * Outputs the complete sequence of tokens recognized during lexical analysis.
 * Each line shows: line:column token_name lexeme
 * 
 * @param out Output file stream (must be open for writing)
 */
void lex_support_dump_tokens(FILE *out);

/**
 * lex_support_dump_errors - Write lexical errors to file
 * 
 * Outputs all lexical errors encountered during analysis.
 * If no errors occurred, writes "No lexical errors detected."
 * 
 * @param out Output file stream (must be open for writing)
 */
void lex_support_dump_errors(FILE *out);

/**
 * lex_support_finalize - Clean up lexical analysis support structures
 * 
 * Frees all memory allocated for symbol table, token trace, and error list.
 * Should be called once at the end of compilation.
 */
void lex_support_finalize(void);

#endif  // LEXER_SUPPORT_H
