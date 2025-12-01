%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "symtab.h"

void yyerror(const char *s);
int yylex(void);
extern int yylineno;

%}

%union {
    int iValue;
    double fValue;
    char *sValue;
    SymbolEntry *sEntry;
}

%token YYEOF ERROR_TOKEN

%token <iValue> INTEGER_CONSTANT
%token <fValue> FLOATING_POINT_CONSTANT
%token <sEntry> IDENTIFIER

%token ELSE_TOKEN INTEGER_TOKEN SELF_TOKEN FLOAT_TOKEN ISA_TOKEN CONSTRUCT_TOKEN FUNC_TOKEN
%token PRIVATE_TOKEN THEN_TOKEN IF_TOKEN PUBLIC_TOKEN LOCAL_TOKEN IMPLEMENT_TOKEN READ_TOKEN
%token VOID_TOKEN AND_TOKEN CLASS_TOKEN OR_TOKEN RETURN_TOKEN WHILE_TOKEN ATTRIBUTE_TOKEN
%token WRITE_TOKEN NOT_TOKEN

%token ASSIGN_OP EQ_OP NE_OP LE_OP GE_OP ARROW

%token PLUS_OP MINUS_OP MUL_OP DIV_OP LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA DOT COLON LT_OP GT_OP

%locations

%%

program:
      /* empty */
    | program statement
    ;

statement:
      declaration SEMICOLON        { printf("Declaration.\n"); }
    | IDENTIFIER ASSIGN_OP expression SEMICOLON
        { printf("Assignment to %s.\n", $1->lexeme); }
    | IF_TOKEN LPAREN expression RPAREN LBRACE program RBRACE
        { printf("If-statement.\n"); }
    | WHILE_TOKEN LPAREN expression RPAREN LBRACE program RBRACE
        { printf("While-statement.\n"); }
    | READ_TOKEN LPAREN IDENTIFIER RPAREN SEMICOLON
        { printf("Read %s.\n", $3->lexeme); }
    | WRITE_TOKEN LPAREN expression RPAREN SEMICOLON
        { printf("Write.\n"); }
    | INTEGER_CONSTANT SEMICOLON
        { printf("Integer constant: %d at line %d\n", $1, @1.first_line); }
    | FLOATING_POINT_CONSTANT SEMICOLON
        { printf("Float constant: %f at line %d\n", $1, @1.first_line); }
    | IDENTIFIER SEMICOLON
        { printf("Identifier: %s at line %d\n", $1->lexeme, @1.first_line); }
    | error SEMICOLON
        { yyerror("Syntax error, skipping to next statement"); yyclearin; }
    | SEMICOLON
        { /* empty statement */ }
    ;

declaration:
      INTEGER_TOKEN IDENTIFIER
    | INTEGER_TOKEN IDENTIFIER ASSIGN_OP expression
    | FLOAT_TOKEN IDENTIFIER
    | FLOAT_TOKEN IDENTIFIER ASSIGN_OP expression
    ;

expression:
      INTEGER_CONSTANT
    | FLOATING_POINT_CONSTANT
    | IDENTIFIER
    | expression PLUS_OP expression
    | expression MINUS_OP expression
    | expression MUL_OP expression
    | expression DIV_OP expression
    | expression EQ_OP expression
    | expression NE_OP expression
    | expression LE_OP expression
    | expression GE_OP expression
    | expression LT_OP expression
    | expression GT_OP expression
    | LPAREN expression RPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error near line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    extern FILE *yyin;
    yyin = f;

    if (yyparse() == 0) {
        printf("\nParsing completed successfully.\n");
    }

    fclose(f);

    print_symbol_table();
    return EXIT_SUCCESS;
}


