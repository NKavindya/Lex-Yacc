#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ast.h"
#include "symbol_table.h"
#include "codegen.h"

/* Parser interfaces */
extern AST *astRoot;
extern int yyparse();
extern FILE *yyin;
extern int current_line;
extern FILE *derivation_file;

/* Semantic functions */
void semantic_passA(AST *root);
void semantic_passB(AST *root);
extern SymTable *globalTable;
extern FILE *errFile;

/* Lexer interfaces for token/symbol output */
extern int yylex(void);
extern int current_column;

/* Symbol table from lexer (if using symtab.c) */
extern SymbolEntry symbolTable[];
extern int nextSymbolIndex;

/* Forward declarations */
void generate_lexer_outputs(const char *inputFile);
void generate_derivation_output(void);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    FILE *f = fopen(inputFile, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }
    yyin = f;

    printf("=== Phase 1: Lexical Analysis ===\n");
    generate_lexer_outputs(inputFile);
    fclose(f);

    /* Reopen for parsing */
    f = fopen(inputFile, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }
    yyin = f;

    printf("=== Phase 2: Syntax Analysis ===\n");
    
    /* Open derivation file */
    extern FILE *derivation_file;
    derivation_file = fopen("derivation_steps.txt", "w");
    if (!derivation_file) {
        fprintf(stderr, "Warning: Could not open derivation_steps.txt\n");
    }
    
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed.\n");
        fclose(f);
        if (derivation_file) fclose(derivation_file);
        return 1;
    }
    fclose(f);
    if (derivation_file) {
        fclose(derivation_file);
        printf("Derivation steps written to derivation_steps.txt\n");
    }

    if (!astRoot) {
        fprintf(stderr, "No AST produced.\n");
        return 1;
    }

    printf("=== Phase 3: Semantic Analysis ===\n");
    errFile = fopen("semantic_errors.txt", "w");
    if (!errFile) errFile = stdout;

    /* Pass A: Build symbol tables */
    semantic_passA(astRoot);

    /* Write symbol table */
    FILE *symout = fopen("symbol_table.txt", "w");
    if (symout) {
        symtable_print_all(globalTable, symout);
        fclose(symout);
        printf("Symbol table written to symbol_table.txt\n");
    }

    /* Pass B: Semantic checks */
    semantic_passB(astRoot);

    if (errFile && errFile != stdout) {
        fclose(errFile);
    }

    /* Check if there are semantic errors */
    FILE *errCheck = fopen("semantic_errors.txt", "r");
    int hasErrors = 0;
    if (errCheck) {
        char line[256];
        if (fgets(line, sizeof(line), errCheck)) {
            hasErrors = 1;  /* File has content = errors exist */
        }
        fclose(errCheck);
    }

    if (hasErrors) {
        printf("Semantic errors found. See semantic_errors.txt\n");
        printf("Code generation skipped due to semantic errors.\n");
        return 1;
    }

    printf("=== Phase 4: Code Generation ===\n");

    /* Generate Intermediate Representation */
    if (codegen_generate_ir(astRoot, globalTable, "codegen.ir") == 0) {
        printf("IR written to codegen.ir\n");
    }

    /* Generate Assembly */
    if (codegen_generate(astRoot, globalTable, "codegen.asm") == 0) {
        printf("Assembly written to codegen.asm\n");

        /* Generate Relocatable Code */
        if (codegen_generate_relocatable("codegen.asm", "codegen.reloc") == 0) {
            printf("Relocatable code written to codegen.reloc\n");
        }

        /* Generate Absolute Code */
        if (codegen_generate_absolute("codegen.asm", "codegen.abs") == 0) {
            printf("Absolute code written to codegen.abs\n");
        }
    }

    printf("\n=== Compilation Complete ===\n");
    printf("Generated files:\n");
    printf("  - lexer_tokens.txt\n");
    printf("  - lexer_symbols.txt\n");
    printf("  - lexer_errors.txt\n");
    printf("  - derivation_steps.txt\n");
    printf("  - symbol_table.txt\n");
    printf("  - semantic_errors.txt\n");
    printf("  - codegen.ir\n");
    printf("  - codegen.asm\n");
    printf("  - codegen.reloc\n");
    printf("  - codegen.abs\n");

    return 0;
}

/* Generate lexer output files */
void generate_lexer_outputs(const char *inputFile) {
    FILE *tokenFile = fopen("lexer_tokens.txt", "w");
    FILE *symbolFile = fopen("lexer_symbols.txt", "w");
    FILE *errorFile = fopen("lexer_errors.txt", "w");

    if (!tokenFile || !symbolFile || !errorFile) {
        perror("Failed to open lexer output files");
        return;
    }

    FILE *f = fopen(inputFile, "r");
    if (!f) {
        fclose(tokenFile);
        fclose(symbolFile);
        fclose(errorFile);
        return;
    }
    yyin = f;

    extern int current_line;
    extern int current_column;
    current_line = 1;
    if (current_column == 0) current_column = 1;  /* initialize if not already set */

    int token;
    int tokenCount = 0;
    while ((token = yylex()) != 0) {
        tokenCount++;
        if (tokenFile) {
            fprintf(tokenFile, "%d:%d %d\n", current_line, current_column, token);
        }
        if (token == 256) {  /* ERROR_TOKEN */
            if (errorFile) {
                fprintf(errorFile, "Lexical error at line %d, column %d\n",
                        current_line, current_column);
            }
        }
    }

    /* Write symbol table to lexer_symbols.txt */
    if (nextSymbolIndex > 0) {
        for (int i = 0; i < nextSymbolIndex; i++) {
            fprintf(symbolFile, "%s %d first=1:1 count=1\n",
                    symbolTable[i].lexeme, symbolTable[i].type);
        }
    } else {
        fprintf(symbolFile, "; No symbols found\n");
    }

    fclose(f);
    if (tokenFile) fclose(tokenFile);
    if (symbolFile) fclose(symbolFile);
    if (errorFile) fclose(errorFile);
    
    printf("Lexer output files generated:\n");
    printf("  - lexer_tokens.txt (%d tokens)\n", tokenCount);
    printf("  - lexer_symbols.txt (%d symbols)\n", nextSymbolIndex);
}

/* Generate derivation steps (if using recursive descent parser) */
void generate_derivation_output(void) {
    /* This would be called if using recursive descent parser */
    /* For now, derivation is handled by parser.y actions */
}

