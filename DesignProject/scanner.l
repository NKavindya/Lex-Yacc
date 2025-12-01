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

/* lexical element patterns */
/* grammar: digit -> 0 ...9 */
DIGIT           [0-9]
/* grammar: letter -> a ...z | a ...z */
ID_START        [A-Za-z]
/* grammar: alphanum -> letter | digit | _ */
ID_PART         [A-Za-z0-9_]
/* grammar: integer -> nonzero digit* | 0, where nonzero -> 1 ...9 */
INT_LITERAL     0|[1-9][0-9]*
/* grammar: exponent part [e [+ | –] integer] */
EXP             [eE][+-]?{DIGIT}+
/* grammar: float -> integer fraction [e [+ | –] integer]
 *   where fraction -> . digit* nonzero | .0
 */
FLOAT_LITERAL   ({INT_LITERAL}"."{DIGIT}+({EXP})?)|({INT_LITERAL}{EXP})

%%

[ \t\r]+              { /* whitespace handled in yy_user_action */ }
\n                    { current_line++; current_column = 1; }

"//".*                { /* grammar: inline comments start with // and end with the end of the line */ }

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

"integer"             { RETURN_KEYWORD(INTEGER_T, "INTEGER"); }  /* reserved words - grammar: integer, float, class, implement, func, construct/constructor, attribute, public, private, RETURN, READ, WRITE, IF, else, WHILE, void, self, isa, local, then, and, or, not */
"float"               { RETURN_KEYWORD(FLOAT_T, "FLOAT"); }      /* grammar: float */
"class"               { RETURN_KEYWORD(CLASS, "CLASS"); }          /* grammar: class */
"implement"           { RETURN_KEYWORD(IMPLEMENT, "IMPLEMENT"); } /* grammar: implement */
"func"                { RETURN_KEYWORD(FUNC, "FUNC"); }          /* grammar: func */
"construct"           { RETURN_KEYWORD(CONSTRUCT, "CONSTRUCT"); } /* grammar: construct */
"constructor"         { RETURN_KEYWORD(CONSTRUCT, "CONSTRUCT"); }  /* grammar: constructor */
"attribute"           { RETURN_KEYWORD(ATTRIBUTE, "ATTRIBUTE"); } /* grammar: attribute */
"public"              { RETURN_KEYWORD(PUBLIC, "PUBLIC"); }        /* grammar: public */
"private"             { RETURN_KEYWORD(PRIVATE, "PRIVATE"); }     /* grammar: private */
"return"              { RETURN_KEYWORD(RETURN, "RETURN"); }       /* grammar: RETURN */
"read"                { RETURN_KEYWORD(READ, "READ"); }           /* grammar: READ */
"write"               { RETURN_KEYWORD(WRITE, "WRITE"); }        /* grammar: WRITE */
"if"                  { RETURN_KEYWORD(IF, "IF"); }                 /* grammar: IF */
"else"                { RETURN_KEYWORD(ELSE, "ELSE"); }           /* grammar: else */
"while"               { RETURN_KEYWORD(WHILE, "WHILE"); }         /* grammar: WHILE */
"void"                { RETURN_KEYWORD(VOID, "VOID"); }          /* grammar: void */
"self"                { RETURN_KEYWORD(SELF, "SELF"); }           /* grammar: self */
"isa"                 { RETURN_KEYWORD(ISA, "ISA"); }             /* grammar: isa */
"local"               { RETURN_KEYWORD(LOCAL, "LOCAL"); }         /* grammar: local */
"then"                { RETURN_KEYWORD(THEN, "THEN"); }           /* grammar: then */
"and"                 { RETURN_KEYWORD(AND, "AND"); }             /* grammar: and */
"or"                  { RETURN_KEYWORD(OR, "OR"); }                /* grammar: or */
"not"                 { RETURN_KEYWORD(NOT, "NOT"); }             /* grammar: not */

":="                  { RETURN_TOKEN(ASSIGN, "ASSIGN"); }  /* operators - grammar: assignop -> :=, relop -> == | <> | < | > | <= | >=, addop -> + | - | or, multop -> * | / | and */
"=="                  { RETURN_TOKEN(EQ, "EQ"); }          /* grammar: relop -> == */
"<>"                  { RETURN_TOKEN(NE, "NE"); }          /* grammar: relop -> <> */
"<="                  { RETURN_TOKEN(LE, "LE"); }          /* grammar: relop -> <= */
">="                  { RETURN_TOKEN(GE, "GE"); }          /* grammar: relop -> >= */
"<"                   { RETURN_TOKEN(LT, "LT"); }          /* grammar: relop -> < */
">"                   { RETURN_TOKEN(GT, "GT"); }          /* grammar: relop -> > */
"=>"                  { RETURN_TOKEN(ARROW, "ARROW"); }    /* grammar: => (arrow operator) */

"+"                   { RETURN_TOKEN(PLUS, "PLUS"); }      /* grammar: addop -> +, sign -> + */
"-"                   { RETURN_TOKEN(MINUS, "MINUS"); }    /* grammar: addop -> -, sign -> - */
"*"                   { RETURN_TOKEN(MULT, "MULT"); }      /* grammar: multop -> * */
"/"                   { RETURN_TOKEN(DIV, "DIV"); }        /* grammar: multop -> / */

"("                   { RETURN_TOKEN(LPAREN, "LPAREN"); }  /* grammar: ( - punctuation: ( ) , { . } : [ => ] ; */
")"                   { RETURN_TOKEN(RPAREN, "RPAREN"); }  /* grammar: ) */
"{"                   { RETURN_TOKEN(LBRACE, "LBRACE"); }  /* grammar: { */
"}"                   { RETURN_TOKEN(RBRACE, "RBRACE"); }  /* grammar: } */
"["                   { RETURN_TOKEN(LBRACKET, "LBRACKET"); } /* grammar: [ */
"]"                   { RETURN_TOKEN(RBRACKET, "RBRACKET"); } /* grammar: ] */
","                   { RETURN_TOKEN(COMMA, "COMMA"); }    /* grammar: , */
";"                   { RETURN_TOKEN(SEMICOLON, "SEMICOLON"); } /* grammar: ; */
":"                   { RETURN_TOKEN(COLON, "COLON"); }     /* grammar: : */
"."                   { RETURN_TOKEN(DOT, "DOT"); }        /* grammar: . */

({INT_LITERAL}"."{DIGIT}+|{INT_LITERAL})[eE][+-]? { /* error: malformed float literal */
                        lex_support_record_error("Malformed float literal", current_line, token_start_column);
                      }

{FLOAT_LITERAL}       { /* grammar: float -> integer fraction [e [+ | –] integer] */
                        yylval.dVal = strtod(yytext, NULL);
                        lex_support_record_token(FLOAT_LIT, "FLOAT_LIT",
                                                 yytext, current_line, token_start_column);
                        lex_support_record_symbol(LEXSYM_FLOAT_LITERAL,
                                                  yytext, current_line, token_start_column);
                        return FLOAT_LIT;
                      }

{INT_LITERAL}         { /* grammar: integer -> nonzero digit* | 0 (intlit) */
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

{ID_START}{ID_PART}*  { /* grammar: ID -> letter alphanum* */
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
