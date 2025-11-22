%{
#include "ast.h"
#include "parser.tab.h"
#include "lexer_support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int current_line = 1;
int current_column = 1;
int token_start_column = 1;
int comment_start_line = 0;
int comment_start_column = 0;

#define YY_USER_ACTION                                  \
    yylloc.first_line = current_line;                   \
    yylloc.last_line = current_line;                    \
    yylloc.first_column = current_column;               \
    yylloc.last_column = current_column + yyleng - 1;   \
    token_start_column = current_column;                \
    current_column += yyleng;

#define RETURN_KEYWORD(tok, name)                                   \
    do {                                                            \
        lex_support_record_token(tok, name, yytext,                 \
                                 current_line, token_start_column); \
        lex_support_record_symbol(LEXSYM_RESERVED, yytext,          \
                                  current_line, token_start_column);\
        return tok;                                                 \
    } while (0)

#define RETURN_TOKEN(tok, name)                                     \
    do {                                                            \
        lex_support_record_token(tok, name, yytext,                 \
                                 current_line, token_start_column); \
        return tok;                                                 \
    } while (0)

%}

%option noyywrap
%x COMMENT

DIGIT           [0-9]
ID_START        [A-Za-z_]
ID_PART         [A-Za-z0-9_]
INT_LITERAL     0|[1-9][0-9]*
EXP             [eE][+-]?{DIGIT}+
FLOAT_LITERAL   ({INT_LITERAL}"."{DIGIT}+({EXP})?)|({INT_LITERAL}{EXP})

%%

[ \t\r]+              { /* whitespace handled in YY_USER_ACTION */ }
\n                    { current_line++; current_column = 1; }

"//".*                { /* skip single-line comments */ }

"/*"                  {
                        comment_start_line = current_line;
                        comment_start_column = token_start_column;
                        BEGIN(COMMENT);
                      }

<COMMENT>"*/"         { BEGIN(INITIAL); }
<COMMENT>\n           { current_line++; current_column = 1; }
<COMMENT>.            { /* consume comment content */ }
<COMMENT><<EOF>>      {
                        char buffer[256];
                        snprintf(buffer, sizeof(buffer),
                                 "Unterminated block comment starting at %d:%d",
                                 comment_start_line, comment_start_column);
                        lex_support_record_error(buffer, current_line, token_start_column);
                        BEGIN(INITIAL);
                        return 0;
                      }

"integer"             { RETURN_KEYWORD(INTEGER_T, "INTEGER"); }
"int"                 { RETURN_KEYWORD(INTEGER_T, "INTEGER"); }
"float"               { RETURN_KEYWORD(FLOAT_T, "FLOAT"); }
"class"               { RETURN_KEYWORD(CLASS, "CLASS"); }
"implement"           { RETURN_KEYWORD(IMPLEMENT, "IMPLEMENT"); }
"func"                { RETURN_KEYWORD(FUNC, "FUNC"); }
"construct"           { RETURN_KEYWORD(CONSTRUCT, "CONSTRUCT"); }
"attribute"           { RETURN_KEYWORD(ATTRIBUTE, "ATTRIBUTE"); }
"public"              { RETURN_KEYWORD(PUBLIC, "PUBLIC"); }
"private"             { RETURN_KEYWORD(PRIVATE, "PRIVATE"); }
"return"              { RETURN_KEYWORD(RETURN, "RETURN"); }
"read"                { RETURN_KEYWORD(READ, "READ"); }
"write"               { RETURN_KEYWORD(WRITE, "WRITE"); }
"if"                  { RETURN_KEYWORD(IF, "IF"); }
"else"                { RETURN_KEYWORD(ELSE, "ELSE"); }
"while"               { RETURN_KEYWORD(WHILE, "WHILE"); }
"void"                { RETURN_KEYWORD(VOID, "VOID"); }
"self"                { RETURN_KEYWORD(SELF, "SELF"); }
"isa"                 { RETURN_KEYWORD(ISA, "ISA"); }
"local"               { RETURN_KEYWORD(LOCAL, "LOCAL"); }
"then"                { RETURN_KEYWORD(THEN, "THEN"); }
"and"                 { RETURN_KEYWORD(AND, "AND"); }
"or"                  { RETURN_KEYWORD(OR, "OR"); }
"not"                 { RETURN_KEYWORD(NOT, "NOT"); }

":="                  { RETURN_TOKEN(ASSIGN, "ASSIGN"); }
"=="                  { RETURN_TOKEN(EQ, "EQ"); }
"<>"                  { RETURN_TOKEN(NE, "NE"); }
"<="                  { RETURN_TOKEN(LE, "LE"); }
">="                  { RETURN_TOKEN(GE, "GE"); }
"<"                   { RETURN_TOKEN(LT, "LT"); }
">"                   { RETURN_TOKEN(GT, "GT"); }
"&&"                  { RETURN_TOKEN(AND, "AND"); }
"||"                  { RETURN_TOKEN(OR, "OR"); }

"->"                  { RETURN_TOKEN(ARROW, "ARROW"); }
"=>"                  { RETURN_TOKEN(ARROW, "ARROW"); }

"+"                   { RETURN_TOKEN(PLUS, "PLUS"); }
"-"                   { RETURN_TOKEN(MINUS, "MINUS"); }
"*"                   { RETURN_TOKEN(MULT, "MULT"); }
"/"                   { RETURN_TOKEN(DIV, "DIV"); }

"("                   { RETURN_TOKEN(LPAREN, "LPAREN"); }
")"                   { RETURN_TOKEN(RPAREN, "RPAREN"); }
"{"                   { RETURN_TOKEN(LBRACE, "LBRACE"); }
"}"                   { RETURN_TOKEN(RBRACE, "RBRACE"); }
"["                   { RETURN_TOKEN(LBRACKET, "LBRACKET"); }
"]"                   { RETURN_TOKEN(RBRACKET, "RBRACKET"); }
","                   { RETURN_TOKEN(COMMA, "COMMA"); }
";"                   { RETURN_TOKEN(SEMICOLON, "SEMICOLON"); }
":"                   { RETURN_TOKEN(COLON, "COLON"); }
"."                   { RETURN_TOKEN(DOT, "DOT"); }

({INT_LITERAL}"."{DIGIT}+|{INT_LITERAL})[eE][+-]? {
                        lex_support_record_error("Malformed float literal", current_line, token_start_column);
                      }

{FLOAT_LITERAL}       {
                        yylval.dVal = strtod(yytext, NULL);
                        lex_support_record_token(FLOAT_LIT, "FLOAT_LIT",
                                                 yytext, current_line, token_start_column);
                        lex_support_record_symbol(LEXSYM_FLOAT_LITERAL,
                                                  yytext, current_line, token_start_column);
                        return FLOAT_LIT;
                      }

{INT_LITERAL}         {
                        yylval.iVal = atoi(yytext);
                        lex_support_record_token(INT_LIT, "INT_LIT",
                                                 yytext, current_line, token_start_column);
                        lex_support_record_symbol(LEXSYM_INT_LITERAL,
                                                  yytext, current_line, token_start_column);
                        return INT_LIT;
                      }

\"([^\"\\]|\\.)*\"    {
                        yylval.sVal = strdup(yytext);
                        lex_support_record_token(STRING_LIT, "STRING_LIT",
                                                 yytext, current_line, token_start_column);
                        lex_support_record_symbol(LEXSYM_STRING_LITERAL,
                                                  yytext, current_line, token_start_column);
                        return STRING_LIT;
                      }

{ID_START}{ID_PART}*  {
                        yylval.sVal = strdup(yytext);
                        lex_support_record_token(ID, "ID",
                                                 yytext, current_line, token_start_column);
                        lex_support_record_symbol(LEXSYM_IDENTIFIER,
                                                  yytext, current_line, token_start_column);
                        return ID;
                      }

.                     {
                        char buffer[128];
                        snprintf(buffer, sizeof(buffer), "Unknown character '%s'", yytext);
                        lex_support_record_error(buffer, current_line, token_start_column);
                      }

<<EOF>>               { return 0; }

%%
