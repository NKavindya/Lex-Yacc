#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"
#include "lexer_support.h"

/* parser exposes astRoot and yyparse/yyin */
extern AST *astRoot;
extern int yyparse();
extern FILE *yyin;
FILE *derivation_file = NULL;

/* semantic functions */
void semantic_passA(AST *root);
void semantic_passB(AST *root);
extern SymTable *globalTable;
extern FILE *errFile;

int main(int argc, char **argv) {
    lex_support_init();
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) { perror("fopen"); return 1; }
    yyin = f;

    derivation_file = fopen("derivation_steps.txt", "w");

    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed.\n");
        fclose(f);
        if (derivation_file) fclose(derivation_file);
        lex_support_finalize();
        return 1;
    }
    fclose(f);
    if (derivation_file) fflush(derivation_file);

    if (!astRoot) {
        fprintf(stderr, "No AST produced.\n");
        if (derivation_file) fclose(derivation_file);
        lex_support_finalize();
        return 1;
    }

    printf("=== AST ===\n");
    ast_print(astRoot, 0);

    /* open semantic error file */
    errFile = fopen("semantic_errors.txt", "w");
    if (!errFile) errFile = stdout;

    /* pass A: build symbol tables */
    semantic_passA(astRoot);

    /* write symbol table */
    FILE *symout = fopen("symbol_table.txt", "w");
    if (symout) {
        symtable_print_all(globalTable, symout);
        fclose(symout);
    }

    /* pass B: semantic checks */
    semantic_passB(astRoot);

    /* lexical artifacts */
    FILE *lexsym = fopen("lexer_symbols.txt", "w");
    if (lexsym) {
        lex_support_dump_symbols(lexsym);
        fclose(lexsym);
    }
    FILE *toklog = fopen("lexer_tokens.txt", "w");
    if (toklog) {
        lex_support_dump_tokens(toklog);
        fclose(toklog);
    }
    FILE *lexerr = fopen("lexer_errors.txt", "w");
    if (lexerr) {
        lex_support_dump_errors(lexerr);
        fclose(lexerr);
    }
    lex_support_finalize();

    if (errFile && errFile != stdout) fclose(errFile);
    if (derivation_file) fclose(derivation_file);

    printf("Done. See lexer_tokens.txt, lexer_symbols.txt, semantic_errors.txt, symbol_table.txt\n");
    return 0;
}
