%{
#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int current_line = 1; /* keep global line number */
#define YY_USER_ACTION yylloc.first_line = current_line;
%}

%option noyywrap

%%

[ \t]+            ;                  /* skip whitespace */
\n    { current_line++; }

"class"           { return CLASS; }
"implement"       { return IMPLEMENT; }
"func"            { return FUNC; }
"construct"       { return CONSTRUCT; }
"attribute"       { return ATTRIBUTE; }
"public"          { return PUBLIC; }
"private"         { return PRIVATE; }
"return"          { return RETURN; }
"read"            { return READ; }
"write"           { return WRITE; }
"if"              { return IF; }
"else"            { return ELSE; }
"while"           { return WHILE; }
"void"            { yylval.sVal = strdup(yytext); return ID; } /* treat 'void' as ID for type rule */
"self"            { return SELF; }
"isa"             { return ISA; }
"local"           { return LOCAL; }
"then"            { return THEN; }

":="              { return ASSIGN; }
"=="              { return EQ; }
"<>"              { return NE; }
"<="              { return LE; }
">="              { return GE; }
"<"               { return LT; }
">"               { return GT; }
"and"             { return AND; }
"or"              { return OR; }
"not"             { return NOT; }
"&&"              { return AND; }
"||"              { return OR; }

"->"              { return ARROW; }
"=>"              { return ARROW; }

"+"               { return PLUS; }
"-"               { return MINUS; }
"*"               { return MULT; }
"/"               { return DIV; }

"("               { return LPAREN; }
")"               { return RPAREN; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
"["               { return LBRACKET; }
"]"               { return RBRACKET; }
","               { return COMMA; }
";"               { return SEMICOLON; }
":"               { return COLON; }
"."               { return DOT; }

[0-9]+\.[0-9]+    { yylval.dVal = atof(yytext); return FLOAT_LIT; }
0|[1-9][0-9]*     { yylval.iVal = atoi(yytext); return INT_LIT; }

\"([^\"\\]|\\.)*\"   { yylval.sVal = strdup(yytext); return STRING_LIT; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sVal = strdup(yytext); return ID; }

"//".*            ;
"/\\*"([^*]|\\*+[^*/])*"\\*/"  ;

.                 { fprintf(stderr, "Unknown character '%s' at line %d\n", yytext, current_line); }

<<EOF>>           { return 0; }

%%
