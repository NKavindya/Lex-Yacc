%{
#include "q2.tab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int lineno = 1;
FILE *token_output_file = NULL;
int header_printed = 0;  // To print header once

// Open token output file
void open_token_file(const char* filename) {
    token_output_file = fopen(filename, "w");
    if (!token_output_file) {
        fprintf(stderr, "Error: Could not open token output file %s\n", filename);
        exit(EXIT_FAILURE);
    }
}

// Close token output file
void close_token_file() {
    if (token_output_file) {
        fflush(token_output_file);  // flush any remaining content
        fclose(token_output_file);
        token_output_file = NULL;
    }
}

// Print header once and then token info to console and file
void print_token(const char* tokenName, const char* lexeme, int line) {
    if (!header_printed) {
        printf("%-15s\t%-10s\t%s\n", "Token", "Type", "Location");
        if (token_output_file != NULL) {
            fprintf(token_output_file, "%-15s\t%-10s\t%s\n", "Token", "Type", "Location");
        }
        header_printed = 1;
    }
    printf("%-15s\t%-10s\tLine: %d\n", tokenName, lexeme, line);
    if (token_output_file != NULL) {
        fprintf(token_output_file, "%-15s\t%-10s\tLine: %d\n", tokenName, lexeme, line);
        fflush(token_output_file);
    }
}

// Called automatically by Flex before scanning starts
int yywrap(void) {
    close_token_file();
    return 1;
}

// Initialization called at start of scan
void initialize() {
    open_token_file("tokens.txt");
}

%}

/* Removed %option noyywrap */
%option yylineno

DIGIT       [0-9]
NONZERO     [1-9]
ID          [a-zA-Z_][a-zA-Z0-9_]*
INTEGER_LIT (0|{NONZERO}{DIGIT}*)
FRACTION    \.{DIGIT}*{NONZERO}|\.0
FLOAT_LIT   {INTEGER_LIT}{FRACTION}([eE][+-]?{INTEGER_LIT})?

%%

[ \t\r]+                  ;  // Ignore whitespace
\n                        { lineno++; }

"//".*                    ;  // Ignore single-line comments
"/*"([^*]|\*+[^/])*"\*/"  ;  // Ignore block comments

"###"[ \t]*\n?            { print_token("DELIM", yytext, yylineno); return DELIM; }

"if"                      { print_token("IF", yytext, yylineno); return IF; }
"then"                    { print_token("THEN", yytext, yylineno); return THEN; }
"else"                    { print_token("ELSE", yytext, yylineno); return ELSE; }
"while"                   { print_token("WHILE", yytext, yylineno); return WHILE; }
"return"                  { print_token("RETURN", yytext, yylineno); return RETURN; }
"read"                    { print_token("READ", yytext, yylineno); return READ; }
"write"                   { print_token("WRITE", yytext, yylineno); return WRITE; }
"constructor"             { print_token("CONSTRUCTOR", yytext, yylineno); return CONSTRUCTOR; }

"implement"               { print_token("IMPLEMENT", yytext, yylineno); return IMPLEMENT; }
"func"                    { print_token("FUNC", yytext, yylineno); return FUNC; }
"class"                   { print_token("CLASS", yytext, yylineno); return CLASS; }

"public"                  { print_token("PUBLIC", yytext, yylineno); return PUBLIC; }
"private"                 { print_token("PRIVATE", yytext, yylineno); return PRIVATE; }
"local"                   { print_token("LOCAL", yytext, yylineno); return LOCAL; }

"attribute"               { print_token("ATTRIBUTE", yytext, yylineno); return ATTRIBUTE; }

"self"                    { print_token("SELF", yytext, yylineno); return SELF; }
"isa"                     { print_token("ISA", yytext, yylineno); return ISA; }

"integer"                 { print_token("INTEGER", yytext, yylineno); return INTEGER; }
"float"                   { print_token("FLOAT", yytext, yylineno); return FLOAT; }
"void"                    { print_token("VOID", yytext, yylineno); return VOID; }

"or"                      { print_token("OR", yytext, yylineno); return OR; }
"and"                     { print_token("AND", yytext, yylineno); return AND; }
"not"                     { print_token("NOT", yytext, yylineno); return NOT; }

"=="                      { print_token("EQ", yytext, yylineno); return EQ; }
"<="                      { print_token("LTE", yytext, yylineno); return LTE; }
">="                      { print_token("GTE", yytext, yylineno); return GTE; }
"<>"                      { print_token("NEQ", yytext, yylineno); return NEQ; }
":="                      { print_token("ASSIGN", yytext, yylineno); return ASSIGN; }
"<"                       { print_token("LT", yytext, yylineno); return LT; }
">"                       { print_token("GT", yytext, yylineno); return GT; }

"+"                       { print_token("PLUS", yytext, yylineno); return PLUS; }
"-"                       { print_token("MINUS", yytext, yylineno); return MINUS; }
"*"                       { print_token("MULT", yytext, yylineno); return MULT; }
"/"                       { print_token("DIV", yytext, yylineno); return DIV; }

";"                       { print_token("SEMICOLON", yytext, yylineno); return SEMICOLON; }
","                       { print_token("COMMA", yytext, yylineno); return COMMA; }
":"                       { print_token("COLON", yytext, yylineno); return COLON; }
"("                       { print_token("LPAREN", yytext, yylineno); return LPAREN; }
")"                       { print_token("RPAREN", yytext, yylineno); return RPAREN; }
"{"                       { print_token("LBRACE", yytext, yylineno); return LBRACE; }
"}"                       { print_token("RBRACE", yytext, yylineno); return RBRACE; }
"["                       { print_token("LBRACKET", yytext, yylineno); return LBRACKET; }
"]"                       { print_token("RBRACKET", yytext, yylineno); return RBRACKET; }
"=>"                      { print_token("ARROW", yytext, yylineno); return ARROW; }
"."                       { print_token("DOT", yytext, yylineno); return DOT; }

{FLOAT_LIT}               { yylval.str = strdup(yytext); print_token("FLOATLIT", yytext, yylineno); return FLOATLIT; }
{INTEGER_LIT}             { yylval.str = strdup(yytext); print_token("INTLIT", yytext, yylineno); return INTLIT; }
{ID}                      { yylval.str = strdup(yytext); print_token("ID", yytext, yylineno); return ID; }

.                         { printf("Lexical Error: Unknown character '%s' at line %d\n", yytext, yylineno); }

%%
