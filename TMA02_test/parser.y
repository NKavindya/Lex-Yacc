%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "symtab.h"
#include "parser.tab.h"

void yyerror(const char *s);
int yylex(void);

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
%token VOID_TOKEN AND_TOKEN CLASS_TOKEN OR_TOKEN RETURN_TOKEN WHILE_TOKEN ATTRIBUTE_TOKEN // FIX: Removed duplicate CLASS_TOKEN
%token WRITE_TOKEN NOT_TOKEN

%token ASSIGN_OP EQ_OP NE_OP LE_OP GE_OP ARROW

%token PLUS_OP MINUS_OP MUL_OP DIV_OP LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET SEMICOLON COMMA DOT COLON LT_OP GT_OP

%locations

%%

program:
    /* empty */
    | program statement
    ;

statement:
    declaration SEMICOLON { printf("Declaration recognized.\n"); } // Rule for declarations
    | IDENTIFIER ASSIGN_OP expression SEMICOLON { printf("Assignment: %s := ...\n", $1->lexeme); }
    | IF_TOKEN LPAREN expression RPAREN LBRACE program RBRACE { printf("If statement recognized.\n"); }
    | WHILE_TOKEN LPAREN expression RPAREN LBRACE program RBRACE { printf("While statement recognized.\n"); }
    | READ_TOKEN LPAREN IDENTIFIER RPAREN SEMICOLON { printf("Read statement recognized for %s.\n", $3->lexeme); }
    | WRITE_TOKEN LPAREN expression RPAREN SEMICOLON { printf("Write statement recognized.\n"); }
    | INTEGER_CONSTANT SEMICOLON { printf("Integer constant: %d at line %d, col %d\n", $1, @1.first_line, @1.first_column); }
    | FLOATING_POINT_CONSTANT SEMICOLON { printf("Float constant: %f at line %d, col %d\n", $1, @1.first_line, @1.first_column); }
    | IDENTIFIER SEMICOLON { printf("Identifier: %s at line %d, col %d\n", $1->lexeme, @1.first_line, @1.first_column); }
    | error SEMICOLON { yyerror("Syntax error, skipping to next statement"); }
    | SEMICOLON { } // Allow empty statements
    ;

// Rules for declarations (added in previous step, ensuring they are present)
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