#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    // Keywords
    KW_INT, KW_FLOAT, KW_FUNC, KW_IF, KW_ELSE, KW_WHILE, KW_RETURN,
    KW_SELF, KW_CONSTRUCT, KW_IMPL, KW_CLASS, KW_ATTRIBUTE, KW_ISA,
    KW_PUBLIC, KW_PRIVATE, KW_THEN, KW_LOCAL, KW_READ, KW_WRITE, KW_VOID,

    // Operators
    OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_ASSIGN, OP_EQUAL, OP_NOTEQ,
    OP_LT, OP_GT, OP_LE, OP_GE, OP_AND, OP_OR, OP_NOT,

    // Punctuation
    PAREN_L, PAREN_R, BRACE_L, BRACE_R, BRACKET_L, BRACKET_R,
    COMMA, SEMICOLON, COLON, DOT, ARROW,

    // Literals and identifiers
    ID, INT_LIT, FLOAT_LIT,

    // End of file
    END_OF_FILE,

    // Error token
    TOKEN_ERROR

} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
    int line_number;
} Token;

#endif
