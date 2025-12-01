#include <stdio.h>
#include <stdlib.h>
#include "parser_rd.h"
#include "symtab.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file> [derivation_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    const char *deriv_filename = (argc >= 3) ? argv[2] : "derivation_steps.txt";
    FILE *deriv_file = fopen(deriv_filename, "w");
    if (!deriv_file) {
        perror("Error opening derivation file");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    fprintf(deriv_file, "=== Derivation Steps ===\n\n");

    /* Initialize parser */
    init_parser(input_file, deriv_file);

    /* Parse the program */
    fprintf(stderr, "Starting parse...\n");
    parse_prog();

    /* Check for remaining tokens */
    if (current_token.type != YYEOF) {
        fprintf(stderr, "Warning: Extra tokens after program end\n");
    }

    /* Close files */
    fclose(input_file);
    fclose(deriv_file);

    /* Print results */
    printf("\n=== Parsing Results ===\n");
    if (syntax_errors == 0) {
        printf("Parsing completed successfully!\n");
    } else {
        printf("Parsing completed with %d syntax error(s).\n", syntax_errors);
    }
    printf("Derivation written to: %s\n", deriv_filename);

    /* Print symbol table */
    print_symbol_table();

    return (syntax_errors == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

