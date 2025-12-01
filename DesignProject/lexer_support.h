/* 
 * lexer_support.h - lexical analysis support functions
 * 
 * provides infrastructure for tracking and reporting lexical analysis results:
 * - symbol table: tracks all identifiers, literals, and reserved words with frequency
 * - token trace: records sequence of all tokens recognized during lexing
 * - error tracking: collects lexical errors (malformed tokens, unknown characters)
 * 
 * used for debugging, error reporting, and generating lexical analysis artifacts.
 */

#ifndef LEXER_SUPPORT_H
#define LEXER_SUPPORT_H

#include <stdio.h>  // for FILE TYPE used in function signatures

/**
 * LexSymbolKind - categories of lexical symbols
 * 
 * used to classify symbols in the lexical symbol table:
 * - LEXSYM_IDENTIFIER: variable names, function names, class names, etc.
 * - LEXSYM_INT_LITERAL: integer constants (e.g., 42, 0, 100)
 * - LEXSYM_FLOAT_LITERAL: floating-point constants (e.g., 3.14, 2.5e10)
 * - LEXSYM_STRING_LITERAL: string constants (e.g., "hello")
 * - LEXSYM_RESERVED: reserved words/keywords (e.g., "class", "func", "if")
 */
typedef enum {
    LEXSYM_IDENTIFIER,      // user-defined identifiers (variables, functions, classes)
    LEXSYM_INT_LITERAL,     // integer literal constants
    LEXSYM_FLOAT_LITERAL,   // floating-point literal constants
    LEXSYM_STRING_LITERAL,  // string literal constants
    LEXSYM_RESERVED         // reserved words/keywords
} LexSymbolKind;

/* function prototypes for lexical analysis support */

/**
 * lex_support_init - initialize lexical analysis support structures
 * 
 * sets up data structures for tracking symbols, tokens, and errors.
 * should be called once at the start of compilation.
 */
void lex_support_init(void);

/**
 * lex_support_record_symbol - record a lexical symbol in the symbol table
 * 
 * adds or updates an entry in the lexical symbol table.
 * if the symbol already exists, increments its count.
 * 
 * @param kind category of the symbol (identifier, literal, reserved word)
 * @param lexeme the actual text of the symbol
 * @param line source line number where symbol first appears
 * @param column source column number where symbol first appears
 */
void lex_support_record_symbol(LexSymbolKind kind, const char *lexeme, int line, int column);

/**
 * lex_support_record_token - record a token in the token trace
 * 
 * adds an entry to the token trace, which records the sequence of all tokens
 * recognized during lexical analysis. used for debugging and generating token logs.
 * 
 * @param tokenType numeric token type (from parser.tab.h)
 * @param tokenName human-readable token name (e.g., "ID", "INT_LIT", "PLUS")
 * @param lexeme the actual text that was tokenized
 * @param line source line number
 * @param column source column number
 */
void lex_support_record_token(int tokenType, const char *tokenName, const char *lexeme, int line, int column);

/**
 * lex_support_record_error - record a lexical error
 * 
 * adds an error entry to the error list. lexical errors include:
 * - malformed tokens (e.g., invalid float literals)
 * - unknown characters
 * - unterminated comments
 * 
 * @param message error message describing the problem
 * @param line source line number where error occurred
 * @param column source column number where error occurred
 */
void lex_support_record_error(const char *message, int line, int column);

/**
 * lex_support_dump_symbols - write lexical symbol table to file
 * 
 * outputs the lexical symbol table showing:
 * - all symbols found (identifiers, literals, reserved words)
 * - symbol category (kind)
 * - first occurrence location (line:column)
 * - frequency count (how many times symbol appeared)
 * 
 * @param out output file stream (must be open for writing)
 */
void lex_support_dump_symbols(FILE *out);

/**
 * lex_support_dump_tokens - write token trace to file
 * 
 * outputs the complete sequence of tokens recognized during lexical analysis.
 * each line shows: line:column token_name lexeme
 * 
 * @param out output file stream (must be open for writing)
 */
void lex_support_dump_tokens(FILE *out);

/**
 * lex_support_dump_errors - write lexical errors to file
 * 
 * outputs all lexical errors encountered during analysis.
 * if no errors occurred, writes "no lexical errors detected."
 * 
 * @param out output file stream (must be open for writing)
 */
void lex_support_dump_errors(FILE *out);

/**
 * lex_support_finalize - clean up lexical analysis support structures
 * 
 * frees all memory allocated for symbol table, token trace, and error list.
 * should be called once at the end of compilation.
 */
void lex_support_finalize(void);

#endif  // lexer_support_h
