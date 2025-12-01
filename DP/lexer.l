%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "symtab.h"

/* Include parser header - try parser_rd.h first, then parser.tab.h */
#ifdef USE_RD_PARSER
#include "parser_rd.h"
/* For recursive descent, we don't use yylval, but need to avoid errors */
typedef union {
    int iValue;
    double fValue;
    char *sValue;
    SymbolEntry *sEntry;
} YYSTYPE;
static YYSTYPE yylval;
#else
#include "parser.tab.h"
extern YYSTYPE yylval;
#endif

/* Track current line/column for location reporting. */
int current_line = 1;
int current_column = 1;
extern int yylineno;

/* Last token produced by the scanner (for the Token API). */
static Token lastToken;

static void start_token(int token_type, const char *text);

%}

%option noyywrap
%option nounput
%option noinput
%option yylineno

%%

/* Whitespace */
[ \t]+            { current_column += yyleng; /* skip spaces/tabs */ }
\n                { current_line++; current_column = 1; }

/* Comments */
"//".*            { current_column += yyleng; /* skip inline comment */ }
"/*"([^*]|\*+[^*/])*\*+"/"  { current_column += yyleng; /* skip block comment */ }

/* Reserved words */
"else"            { start_token(ELSE_TOKEN, yytext);     return ELSE_TOKEN; }
"integer"         { start_token(INTEGER_TOKEN, yytext);  return INTEGER_TOKEN; }
"self"            { start_token(SELF_TOKEN, yytext);     return SELF_TOKEN; }
"float"           { start_token(FLOAT_TOKEN, yytext);    return FLOAT_TOKEN; }
"isa"             { start_token(ISA_TOKEN, yytext);      return ISA_TOKEN; }
"construct"       { start_token(CONSTRUCT_TOKEN, yytext);return CONSTRUCT_TOKEN; }
"func"            { start_token(FUNC_TOKEN, yytext);     return FUNC_TOKEN; }
"private"         { start_token(PRIVATE_TOKEN, yytext);  return PRIVATE_TOKEN; }
"then"            { start_token(THEN_TOKEN, yytext);     return THEN_TOKEN; }
"if"              { start_token(IF_TOKEN, yytext);       return IF_TOKEN; }
"public"          { start_token(PUBLIC_TOKEN, yytext);   return PUBLIC_TOKEN; }
"local"           { start_token(LOCAL_TOKEN, yytext);    return LOCAL_TOKEN; }
"implement"       { start_token(IMPLEMENT_TOKEN, yytext);return IMPLEMENT_TOKEN; }
"read"            { start_token(READ_TOKEN, yytext);     return READ_TOKEN; }
"void"            { start_token(VOID_TOKEN, yytext);     return VOID_TOKEN; }
"and"             { start_token(AND_TOKEN, yytext);      return AND_TOKEN; }
"class"           { start_token(CLASS_TOKEN, yytext);    return CLASS_TOKEN; }
"or"              { start_token(OR_TOKEN, yytext);       return OR_TOKEN; }
"return"          { start_token(RETURN_TOKEN, yytext);   return RETURN_TOKEN; }
"while"           { start_token(WHILE_TOKEN, yytext);    return WHILE_TOKEN; }
"attribute"       { start_token(ATTRIBUTE_TOKEN, yytext);return ATTRIBUTE_TOKEN; }
"write"           { start_token(WRITE_TOKEN, yytext);    return WRITE_TOKEN; }
"not"             { start_token(NOT_TOKEN, yytext);      return NOT_TOKEN; }

/* Operators and punctuation with multiple characters */
":="              { start_token(ASSIGN_OP, yytext);      return ASSIGN_OP; }
"=="              { start_token(EQ_OP, yytext);          return EQ_OP; }
"<>"              { start_token(NE_OP, yytext);          return NE_OP; }
"<="              { start_token(LE_OP, yytext);          return LE_OP; }
">="              { start_token(GE_OP, yytext);          return GE_OP; }
"=>"              { start_token(ARROW, yytext);          return ARROW; }

/* Numeric literals (integer and float) based on the given spec (integer fraction [e ...]) */
([0-9]+)\.([0-9]*[1-9]|0)([eE][+-]?[0-9]+)? {
                    start_token(FLOATING_POINT_CONSTANT, yytext);
                    yylval.fValue = atof(yytext);
                    return FLOATING_POINT_CONSTANT;
                 }

0|[1-9][0-9]*     {
                    start_token(INTEGER_CONSTANT, yytext);
                    yylval.iValue = atoi(yytext);
                    return INTEGER_CONSTANT;
                 }

/* Identifiers (id -> letter alphanum*) and installation in symbol table */
[a-zA-Z][a-zA-Z0-9_]* {
                    start_token(IDENTIFIER, yytext);
                    yylval.sEntry = install(yytext, IDENTIFIER);
                    return IDENTIFIER;
                 }

/* Single-character operators and punctuation */
"+"               { start_token(PLUS_OP, yytext);        return PLUS_OP; }
"-"               { start_token(MINUS_OP, yytext);       return MINUS_OP; }
"*"               { start_token(MUL_OP, yytext);         return MUL_OP; }
"/"               { start_token(DIV_OP, yytext);         return DIV_OP; }

"("               { start_token(LPAREN, yytext);         return LPAREN; }
")"               { start_token(RPAREN, yytext);         return RPAREN; }
"{"               { start_token(LBRACE, yytext);         return LBRACE; }
"}"               { start_token(RBRACE, yytext);         return RBRACE; }
"["               { start_token(LBRACKET, yytext);       return LBRACKET; }
"]"               { start_token(RBRACKET, yytext);       return RBRACKET; }
";"               { start_token(SEMICOLON, yytext);      return SEMICOLON; }
","               { start_token(COMMA, yytext);          return COMMA; }
"."               { start_token(DOT, yytext);            return DOT; }
":"               { start_token(COLON, yytext);          return COLON; }
"<"               { start_token(LT_OP, yytext);          return LT_OP; }
">"               { start_token(GT_OP, yytext);          return GT_OP; }

/* Any other character is a lexical error. */
.                 {
                    fprintf(stderr, "Lexical error: unrecognized character '%s' at line %d, column %d\n",
                            yytext, current_line, current_column);
                    current_column += yyleng;
                    return ERROR_TOKEN;
                 }

<<EOF>>           { return YYEOF; }

%%

/* Helper to initialize the lastToken structure and advance the current_column. */
static void start_token(int token_type, const char *text) {
    lastToken.type = token_type;
    lastToken.lexeme = strdup(text);
    lastToken.line = current_line;
    lastToken.column = current_column;
    current_column += strlen(text);
}

/* Public API for manually driving the scanner if needed. */
Token next_token(void) {
    int t = yylex();
    lastToken.type = t;
    return lastToken;
}

/* Get the last token produced (for parser use) */
Token get_last_token(void) {
    return lastToken;
}


