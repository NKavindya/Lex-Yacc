#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h> // For FILE*

// Enumeration for token types
typedef enum {
    // Punctuation
    TOKEN_LPAREN = 257, // Start from 257 to avoid conflict with ASCII char values
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_DOT,
    TOKEN_COLON,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,

    // Operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LT, TOKEN_GT, // <, >
    TOKEN_LE,           // <=
    TOKEN_GE,           // >=
    TOKEN_ASSIGN,       // :=
    TOKEN_EQ,           // ==
    TOKEN_NE,           // <>
    TOKEN_ARROW,        // =>
    TOKEN_OR, TOKEN_AND, TOKEN_NOT, // Logical operators (reserved words too, but operators here)

    // Reserved Words (explicitly listed for distinct token types)
    TOKEN_ELSE, TOKEN_INTEGER, TOKEN_SELF, TOKEN_FLOAT_KW, TOKEN_ISA, TOKEN_CONSTRUCT, // Renamed FLOAT to FLOAT_KW to avoid clash with TOKEN_FLOAT_LITERAL
    TOKEN_FUNC, TOKEN_PRIVATE, TOKEN_THEN, TOKEN_PUBLIC, TOKEN_LOCAL, TOKEN_IF,
    TOKEN_CLASS, TOKEN_RETURN, TOKEN_WHILE, TOKEN_IMPLEMENT, TOKEN_READ, TOKEN_VOID,
    TOKEN_ATTRIBUTE, TOKEN_WRITE,

    // Lexical Elements (identifiers, numbers)
    TOKEN_ID,
    TOKEN_INTEGER_LITERAL,
    TOKEN_FLOAT_LITERAL,

    // Special tokens
    TOKEN_EOF, // End Of File
    TOKEN_ERROR // For unrecognized characters/errors
} TokenType;

// Token data structure
typedef struct {
    TokenType type;       // Type of the token
    char *lexeme;         // The actual text of the token
    int line;             // Line number
    int column;           // Column number
} Token;

// Global variables for source code location
extern int yylineno; // Line number (provided by Lex)
extern int yycolumn; // Column number (we'll implement this)

// Function to get the next token (implemented in Lex)
Token* getNextToken();

// Function to print token information (for debugging)
void printToken(Token* token);

// Symbol table for storing identifiers and their properties (simple example)
// In a real compiler, this would be more complex (e.g., hash table)
typedef struct Symbol {
    char *name;
    // Add more fields here for type, scope, etc.
    // For this TMA, just storing the name is sufficient for demonstration.
    struct Symbol *next;
} Symbol;

extern Symbol *symbolTable;
void insertSymbol(char *name);
void printSymbolTable();


#endif // TOKEN_H