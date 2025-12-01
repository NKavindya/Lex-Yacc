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

/* Lexical element patterns */
/* Grammar: digit -> 0 ...9 */
DIGIT           [0-9]
/* Grammar: letter -> a ...z | A ...Z */
ID_START        [A-Za-z_]
/* Grammar: alphanum -> letter | digit | _ */
ID_PART         [A-Za-z0-9_]
/* Grammar: integer -> nonzero digit* | 0, where nonzero -> 1 ...9 */
INT_LITERAL     0|[1-9][0-9]*
/* Grammar: exponent part [e [+ | –] integer] */
EXP             [eE][+-]?{DIGIT}+
/* Grammar: float -> integer fraction [e [+ | –] integer]
 *   where fraction -> . digit* nonzero | .0
 */
FLOAT_LITERAL   ({INT_LITERAL}"."{DIGIT}+({EXP})?)|({INT_LITERAL}{EXP})

%%

[ \t\r]+              { /* whitespace handled in YY_USER_ACTION */ }
\n                    { current_line++; current_column = 1; }

"//".*                { /* Grammar: Inline comments start with // and end with the end of the line */ }

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

"integer"             { RETURN_KEYWORD(INTEGER_T, "INTEGER"); }  /* Reserved Words - Grammar: integer, float, class, implement, func, construct/constructor, attribute, public, private, return, read, write, if, else, while, void, self, isa, local, then, and, or, not */
"int"                 { RETURN_KEYWORD(INTEGER_T, "INTEGER"); }  /* Alias for integer */
"float"               { RETURN_KEYWORD(FLOAT_T, "FLOAT"); }      /* Grammar: float */
"class"               { RETURN_KEYWORD(CLASS, "CLASS"); }          /* Grammar: class */
"implement"           { RETURN_KEYWORD(IMPLEMENT, "IMPLEMENT"); } /* Grammar: implement */
"func"                { RETURN_KEYWORD(FUNC, "FUNC"); }          /* Grammar: func */
"construct"           { RETURN_KEYWORD(CONSTRUCT, "CONSTRUCT"); } /* Grammar: construct */
"constructor"         { RETURN_KEYWORD(CONSTRUCT, "CONSTRUCT"); }  /* Grammar: constructor */
"attribute"           { RETURN_KEYWORD(ATTRIBUTE, "ATTRIBUTE"); } /* Grammar: attribute */
"public"              { RETURN_KEYWORD(PUBLIC, "PUBLIC"); }        /* Grammar: public */
"private"             { RETURN_KEYWORD(PRIVATE, "PRIVATE"); }     /* Grammar: private */
"return"              { RETURN_KEYWORD(RETURN, "RETURN"); }       /* Grammar: return */
"read"                { RETURN_KEYWORD(READ, "READ"); }           /* Grammar: read */
"write"               { RETURN_KEYWORD(WRITE, "WRITE"); }        /* Grammar: write */
"if"                  { RETURN_KEYWORD(IF, "IF"); }                 /* Grammar: if */
"else"                { RETURN_KEYWORD(ELSE, "ELSE"); }           /* Grammar: else */
"while"               { RETURN_KEYWORD(WHILE, "WHILE"); }         /* Grammar: while */
"void"                { RETURN_KEYWORD(VOID, "VOID"); }          /* Grammar: void */
"self"                { RETURN_KEYWORD(SELF, "SELF"); }           /* Grammar: self */
"isa"                 { RETURN_KEYWORD(ISA, "ISA"); }             /* Grammar: isa */
"local"               { RETURN_KEYWORD(LOCAL, "LOCAL"); }         /* Grammar: local */
"then"                { RETURN_KEYWORD(THEN, "THEN"); }           /* Grammar: then */
"and"                 { RETURN_KEYWORD(AND, "AND"); }             /* Grammar: and */
"or"                  { RETURN_KEYWORD(OR, "OR"); }                /* Grammar: or */
"not"                 { RETURN_KEYWORD(NOT, "NOT"); }             /* Grammar: not */

":="                  { RETURN_TOKEN(ASSIGN, "ASSIGN"); }  /* Operators - Grammar: assignOp -> :=, relOp -> == | <> | < | > | <= | >=, addOp -> + | - | or, multOp -> * | / | and */
"=="                  { RETURN_TOKEN(EQ, "EQ"); }          /* Grammar: relOp -> == */
"<>"                  { RETURN_TOKEN(NE, "NE"); }          /* Grammar: relOp -> <> */
"<="                  { RETURN_TOKEN(LE, "LE"); }          /* Grammar: relOp -> <= */
">="                  { RETURN_TOKEN(GE, "GE"); }          /* Grammar: relOp -> >= */
"<"                   { RETURN_TOKEN(LT, "LT"); }          /* Grammar: relOp -> < */
">"                   { RETURN_TOKEN(GT, "GT"); }          /* Grammar: relOp -> > */
"&&"                  { RETURN_TOKEN(AND, "AND"); }         /* Alternative syntax for and */
"||"                  { RETURN_TOKEN(OR, "OR"); }          /* Alternative syntax for or */

"->"                  { RETURN_TOKEN(ARROW, "ARROW"); }     /* Alternative arrow syntax */
"=>"                  { RETURN_TOKEN(ARROW, "ARROW"); }    /* Grammar: => (arrow operator) */

"+"                   { RETURN_TOKEN(PLUS, "PLUS"); }      /* Grammar: addOp -> +, sign -> + */
"-"                   { RETURN_TOKEN(MINUS, "MINUS"); }    /* Grammar: addOp -> -, sign -> - */
"*"                   { RETURN_TOKEN(MULT, "MULT"); }      /* Grammar: multOp -> * */
"/"                   { RETURN_TOKEN(DIV, "DIV"); }        /* Grammar: multOp -> / */

"("                   { RETURN_TOKEN(LPAREN, "LPAREN"); }  /* Grammar: ( - Punctuation: ( ) , { . } : [ => ] ; */
")"                   { RETURN_TOKEN(RPAREN, "RPAREN"); }  /* Grammar: ) */
"{"                   { RETURN_TOKEN(LBRACE, "LBRACE"); }  /* Grammar: { */
"}"                   { RETURN_TOKEN(RBRACE, "RBRACE"); }  /* Grammar: } */
"["                   { RETURN_TOKEN(LBRACKET, "LBRACKET"); } /* Grammar: [ */
"]"                   { RETURN_TOKEN(RBRACKET, "RBRACKET"); } /* Grammar: ] */
","                   { RETURN_TOKEN(COMMA, "COMMA"); }    /* Grammar: , */
";"                   { RETURN_TOKEN(SEMICOLON, "SEMICOLON"); } /* Grammar: ; */
":"                   { RETURN_TOKEN(COLON, "COLON"); }     /* Grammar: : */
"."                   { RETURN_TOKEN(DOT, "DOT"); }        /* Grammar: . */

({INT_LITERAL}"."{DIGIT}+|{INT_LITERAL})[eE][+-]? { /* Error: malformed float literal */
                        lex_support_record_error("Malformed float literal", current_line, token_start_column);
                      }

{FLOAT_LITERAL}       { /* Grammar: float -> integer fraction [e [+ | –] integer] */
                        yylval.dVal = strtod(yytext, NULL);
                        lex_support_record_token(FLOAT_LIT, "FLOAT_LIT",
                                                 yytext, current_line, token_start_column);
                        lex_support_record_symbol(LEXSYM_FLOAT_LITERAL,
                                                  yytext, current_line, token_start_column);
                        return FLOAT_LIT;
                      }

{INT_LITERAL}         { /* Grammar: integer -> nonzero digit* | 0 (intLit) */
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

{ID_START}{ID_PART}*  { /* Grammar: id -> letter alphanum* */
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
