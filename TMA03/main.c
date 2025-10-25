#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

/* parser exposes astRoot and yyparse/yyin */
extern AST *astRoot;
extern int yyparse();
extern FILE *yyin;

/* semantic functions */
void semantic_passA(AST *root);
void semantic_passB(AST *root);
extern SymTable *globalTable;
extern FILE *errFile;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) { perror("fopen"); return 1; }
    yyin = f;

    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed.\n");
        fclose(f);
        return 1;
    }
    fclose(f);

    if (!astRoot) {
        fprintf(stderr, "No AST produced.\n");
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

    if (errFile && errFile != stdout) fclose(errFile);

    printf("Done. See symbol_table.txt and semantic_errors.txt\n");
    return 0;
}
