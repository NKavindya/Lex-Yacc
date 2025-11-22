#include "lexer_support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LexSymbolEntry {
    char *lexeme;
    LexSymbolKind kind;
    int first_line;
    int first_column;
    int count;
    struct LexSymbolEntry *next;
} LexSymbolEntry;

typedef struct TokenRecord {
    int tokenType;
    char *tokenName;
    char *lexeme;
    int line;
    int column;
    struct TokenRecord *next;
} TokenRecord;

typedef struct LexError {
    char *message;
    int line;
    int column;
    struct LexError *next;
} LexError;

static LexSymbolEntry *symbol_head = NULL;
static TokenRecord *token_head = NULL;
static TokenRecord *token_tail = NULL;
static LexError *error_head = NULL;
static LexError *error_tail = NULL;

static char *dup_string(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char *copy = (char*)malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

void lex_support_init(void) {
    lex_support_finalize();
}

void lex_support_finalize(void) {
    while (symbol_head) {
        LexSymbolEntry *next = symbol_head->next;
        free(symbol_head->lexeme);
        free(symbol_head);
        symbol_head = next;
    }
    symbol_head = NULL;

    while (token_head) {
        TokenRecord *next = token_head->next;
        free(token_head->tokenName);
        free(token_head->lexeme);
        free(token_head);
        token_head = next;
    }
    token_tail = NULL;

    while (error_head) {
        LexError *next = error_head->next;
        free(error_head->message);
        free(error_head);
        error_head = next;
    }
    error_tail = NULL;
}

static LexSymbolEntry *find_symbol(LexSymbolKind kind, const char *lexeme) {
    for (LexSymbolEntry *p = symbol_head; p; p = p->next) {
        if (p->kind == kind && strcmp(p->lexeme, lexeme) == 0) {
            return p;
        }
    }
    return NULL;
}

void lex_support_record_symbol(LexSymbolKind kind, const char *lexeme, int line, int column) {
    if (!lexeme) return;
    LexSymbolEntry *entry = find_symbol(kind, lexeme);
    if (entry) {
        entry->count++;
        return;
    }
    entry = (LexSymbolEntry*)malloc(sizeof(LexSymbolEntry));
    if (!entry) return;
    entry->lexeme = dup_string(lexeme);
    entry->kind = kind;
    entry->first_line = line;
    entry->first_column = column;
    entry->count = 1;
    entry->next = symbol_head;
    symbol_head = entry;
}

void lex_support_record_token(int tokenType, const char *tokenName, const char *lexeme, int line, int column) {
    (void)tokenType;
    TokenRecord *rec = (TokenRecord*)malloc(sizeof(TokenRecord));
    if (!rec) return;
    rec->tokenType = tokenType;
    rec->tokenName = dup_string(tokenName);
    rec->lexeme = lexeme ? dup_string(lexeme) : dup_string("");
    rec->line = line;
    rec->column = column;
    rec->next = NULL;
    if (!token_head) {
        token_head = token_tail = rec;
    } else {
        token_tail->next = rec;
        token_tail = rec;
    }
}

void lex_support_record_error(const char *message, int line, int column) {
    LexError *err = (LexError*)malloc(sizeof(LexError));
    if (!err) return;
    err->message = dup_string(message ? message : "");
    err->line = line;
    err->column = column;
    err->next = NULL;
    if (!error_head) {
        error_head = error_tail = err;
    } else {
        error_tail->next = err;
        error_tail = err;
    }
}

void lex_support_dump_symbols(FILE *out) {
    if (!out) return;
    fprintf(out, "Lexical Symbol Table\n");
    fprintf(out, "====================\n");
    for (LexSymbolEntry *p = symbol_head; p; p = p->next) {
        const char *kind =
            (p->kind == LEXSYM_IDENTIFIER) ? "identifier" :
            (p->kind == LEXSYM_INT_LITERAL) ? "int_literal" :
            (p->kind == LEXSYM_FLOAT_LITERAL) ? "float_literal" :
            (p->kind == LEXSYM_STRING_LITERAL) ? "string_literal" :
            "reserved";
        fprintf(out, "%-15s kind=%-15s first=%d:%d count=%d\n",
                p->lexeme ? p->lexeme : "<nil>",
                kind,
                p->first_line,
                p->first_column,
                p->count);
    }
}

void lex_support_dump_tokens(FILE *out) {
    if (!out) return;
    fprintf(out, "Token Trace\n");
    fprintf(out, "===========\n");
    for (TokenRecord *p = token_head; p; p = p->next) {
        fprintf(out, "%4d:%-4d %-15s %s\n",
                p->line,
                p->column,
                p->tokenName ? p->tokenName : "<token>",
                p->lexeme ? p->lexeme : "");
    }
}

void lex_support_dump_errors(FILE *out) {
    if (!out) return;
    if (!error_head) {
        fprintf(out, "No lexical errors detected.\n");
        return;
    }
    fprintf(out, "Lexical Errors\n");
    fprintf(out, "==============\n");
    for (LexError *p = error_head; p; p = p->next) {
        fprintf(out, "%4d:%-4d %s\n", p->line, p->column, p->message ? p->message : "");
    }
}
