%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern YYSTYPE yylval;
int current_line = 1;
%}

%option noyywrap

%%

[ \t]+            ;  // ignore whitespace
\n                { current_line++; }

"int"             { yylval.sVal = strdup(yytext); return ID; }
"integer"         { yylval.sVal = strdup(yytext); return ID; }
"float"           { yylval.sVal = strdup(yytext); return ID; }
"func"            { return FUNC; }
"if"              { return IF; }
"else"            { return ELSE; }
"while"           { return WHILE; }
"return"          { return RETURN; }
"self"            { return SELF; }
"construct"       { return CONSTRUCT; }
"implement"       { return IMPLEMENT; }
"class"           { return CLASS; }
"attribute"       { return ATTRIBUTE; }
"isa"             { return ISA; }
"public"          { return PUBLIC; }
"private"         { return PRIVATE; }
"then"            { return THEN; }
"local"           { return LOCAL; }
"read"            { return READ; }
"write"           { return WRITE; }
"void"            { return VOID; }

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
"=>"              { return ARROW; }

[0-9]+\.[0-9]+    { yylval.dVal = atof(yytext); return FLOAT_LIT; }
[1-9][0-9]*       { yylval.iVal = atoi(yytext); return INT_LIT; }
0                 { yylval.iVal = 0; return INT_LIT; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.sVal = strdup(yytext); return ID; }

"//".*            ; // ignore inline comments
"/\\*"([^*]|\\*+[^*/])*"\\*/"  ; // ignore block comments

.                 { /* ignore any other characters */ }

<<EOF>>           { return 0; }

%%
