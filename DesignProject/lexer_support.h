#ifndef LEXER_SUPPORT_H
#define LEXER_SUPPORT_H

#include <stdio.h>

typedef enum {
    LEXSYM_IDENTIFIER,
    LEXSYM_INT_LITERAL,
    LEXSYM_FLOAT_LITERAL,
    LEXSYM_STRING_LITERAL,
    LEXSYM_RESERVED
} LexSymbolKind;

void lex_support_init(void);
void lex_support_record_symbol(LexSymbolKind kind, const char *lexeme, int line, int column);
void lex_support_record_token(int tokenType, const char *tokenName, const char *lexeme, int line, int column);
void lex_support_record_error(const char *message, int line, int column);
void lex_support_dump_symbols(FILE *out);
void lex_support_dump_tokens(FILE *out);
void lex_support_dump_errors(FILE *out);
void lex_support_finalize(void);

#endif
