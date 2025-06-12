%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int current_line = 1;
typedef struct {
    char lexeme[100];
    char type[50];
    int line_number;
} LexicalToken;

LexicalToken lexical_tokens[1000];
int lexical_token_count = 0;

void insert_token(const char* lexeme, const char* type) {
    LexicalToken token;
    strcpy(token.lexeme, lexeme);
    strcpy(token.type, type);
    token.line_number = current_line;
    lexical_tokens[lexical_token_count++] = token;
}
%}

%option noyywrap

%%
[ \t]+          ; /* ignore whitespace */
\n              { current_line++; } /* track line numbers */

"int"            { insert_token(yytext, "Keyword"); }
"float"         { insert_token(yytext, "Keyword"); }
"func"          { insert_token(yytext, "Keyword"); }
"if"              { insert_token(yytext, "Keyword"); }
"else"          { insert_token(yytext, "Keyword"); }
"while"         { insert_token(yytext, "Keyword"); }
"return"        { insert_token(yytext, "Keyword"); }
"self"           { insert_token(yytext, "Keyword"); }
"construct"     { insert_token(yytext, "Keyword"); }
"impl"            { insert_token(yytext, "Keyword"); }
"class"           { insert_token(yytext, "Keyword"); }
"attribute"     { insert_token(yytext, "Keyword"); }
"isa"             { insert_token(yytext, "Keyword"); }
"public"        { insert_token(yytext, "Keyword"); }
"private"       { insert_token(yytext, "Keyword"); }
"then"           { insert_token(yytext, "Keyword"); }
"local"         { insert_token(yytext, "Keyword"); }
"read"          { insert_token(yytext, "Keyword"); }
"write"         { insert_token(yytext, "Keyword"); }
"void"          { insert_token(yytext, "Keyword"); }

"("             { insert_token(yytext, "Punctuation"); }
")"             { insert_token(yytext, "Punctuation"); }
"["             { insert_token(yytext, "Punctuation"); }
"]"             { insert_token(yytext, "Punctuation"); }
"{"             { insert_token(yytext, "Punctuation"); }
"}"             { insert_token(yytext, "Punctuation"); }
","             { insert_token(yytext, "Punctuation"); }
";"             { insert_token(yytext, "Punctuation"); }
":"             { insert_token(yytext, "Punctuation"); }
"."             { insert_token(yytext, "Punctuation"); }
"=>"         { insert_token(yytext, "Punctuation"); }

"+"             { insert_token(yytext, "Operator"); }
"-"             { insert_token(yytext, "Operator"); }
"*"             { insert_token(yytext, "Operator"); }
"/"             { insert_token(yytext, "Operator"); }
":="            { insert_token(yytext, "Operator"); }
"=="            { insert_token(yytext, "Operator"); }
"<>"            { insert_token(yytext, "Operator"); }
"<"             { insert_token(yytext, "Operator"); }
">"             { insert_token(yytext, "Operator"); }
"<="            { insert_token(yytext, "Operator"); }
">="            { insert_token(yytext, "Operator"); }
"and"           { insert_token(yytext, "Operator"); }
"or"            { insert_token(yytext, "Operator"); }
"not"           { insert_token(yytext, "Operator"); }

[1-9][0-9]*     { insert_token(yytext, "Integer"); }
[0-9]+\.[0-9]+  { insert_token(yytext, "Float"); }
[a-zA-Z][a-zA-Z0-9_]* { insert_token(yytext, "Identifier"); }

"/"([^]|\+[^/])\+"/" ; /* ignore block comments */

.               { /* Ignore other characters */ }

%%

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Error: Unable to open file %s\n", argv[1]);
        return 1;
    }

    yyin = input_file;
    yylex();
    fclose(input_file);

    // Print the token information
    printf("Token\t\tType\t\tLocation\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < lexical_token_count; i++) {
        printf("%-15s\t%-15s\t%d\n", lexical_tokens[i].lexeme,
               lexical_tokens[i].type, lexical_tokens[i].line_number);
    }

    FILE* symbol_table_file = fopen("symbol_table.txt", "w");
    if (!symbol_table_file) {
        printf("Error: Unable to open symbol_table.txt for writing\n");
        return 1;
    }

    // Write the token information to the symbol table file
    fprintf(symbol_table_file, "Token\t\tType\t\tLocation\n");
    fprintf(symbol_table_file, "----------------------------------------\n");
    for (int i = 0; i < lexical_token_count; i++) {
        fprintf(symbol_table_file, "%-15s\t%-15s\t%d\n", lexical_tokens[i].lexeme,
                lexical_tokens[i].type, lexical_tokens[i].line_number);
    }
    fclose(symbol_table_file);

    printf("\nInformation about tokens has been stored in symbol_table.txt.\n");
    return 0;
}
