#ifndef COMMON_H
#define COMMON_H

/* Token structure returned by the lexical analyzer.
 * type   : token kind (matches Bison token id, e.g., IDENTIFIER)
 * lexeme : the exact text from the source
 * line   : 1-based line number
 * column : 1-based column number (approximate when using flex)
 */
typedef struct {
    int type;
    char *lexeme;
    int line;
    int column;
} Token;

/* Symbol table entry used for identifiers and reserved words. */
typedef struct {
    char *lexeme; /* actual string */
    int type;     /* token type (Bison token id) */
} SymbolEntry;

#endif /* COMMON_H */


