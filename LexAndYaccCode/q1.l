%{
#include "q1.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

[0-9]+      { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* {
                    if (strcmp(yytext, "while") == 0) return WHILE;
                    else if (strcmp(yytext, "if") == 0) return IF;
                    else if (strcmp(yytext, "then") == 0) return THEN;
                    else if (strcmp(yytext, "else") == 0) return ELSE;
                    else if (strcmp(yytext, "print") == 0) return PRINT;
                    yylval.sval = strdup(yytext);
                    return IDENTIFIER;
                  }

"="         { return ASSIGN; }
">"         { return GT; }
"<"         { return LT; }
"=="        { return EQ; }
"+"         { return PLUS; }
"-"         { return MINUS; }
";"         { return SEMI; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }

[ \t\n]+    ;  // skip whitespace

.           { printf("Unknown character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
