/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Q2_TAB_H_INCLUDED
# define YY_YY_Q2_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    INTLIT = 259,                  /* INTLIT  */
    FLOATLIT = 260,                /* FLOATLIT  */
    IF = 261,                      /* IF  */
    ELSE = 262,                    /* ELSE  */
    WHILE = 263,                   /* WHILE  */
    RETURN = 264,                  /* RETURN  */
    READ = 265,                    /* READ  */
    WRITE = 266,                   /* WRITE  */
    CONSTRUCTOR = 267,             /* CONSTRUCTOR  */
    FUNC = 268,                    /* FUNC  */
    IMPLEMENT = 269,               /* IMPLEMENT  */
    CLASS = 270,                   /* CLASS  */
    PUBLIC = 271,                  /* PUBLIC  */
    PRIVATE = 272,                 /* PRIVATE  */
    LOCAL = 273,                   /* LOCAL  */
    ATTRIBUTE = 274,               /* ATTRIBUTE  */
    SELF = 275,                    /* SELF  */
    ISA = 276,                     /* ISA  */
    INTEGER = 277,                 /* INTEGER  */
    FLOAT = 278,                   /* FLOAT  */
    VOID = 279,                    /* VOID  */
    PLUS = 280,                    /* PLUS  */
    MINUS = 281,                   /* MINUS  */
    MULT = 282,                    /* MULT  */
    DIV = 283,                     /* DIV  */
    EQ = 284,                      /* EQ  */
    NEQ = 285,                     /* NEQ  */
    LT = 286,                      /* LT  */
    GT = 287,                      /* GT  */
    LTE = 288,                     /* LTE  */
    GTE = 289,                     /* GTE  */
    ASSIGN = 290,                  /* ASSIGN  */
    SEMICOLON = 291,               /* SEMICOLON  */
    COMMA = 292,                   /* COMMA  */
    COLON = 293,                   /* COLON  */
    LPAREN = 294,                  /* LPAREN  */
    RPAREN = 295,                  /* RPAREN  */
    LBRACE = 296,                  /* LBRACE  */
    RBRACE = 297,                  /* RBRACE  */
    LBRACKET = 298,                /* LBRACKET  */
    RBRACKET = 299,                /* RBRACKET  */
    ARROW = 300,                   /* ARROW  */
    DOT = 301,                     /* DOT  */
    OR = 302,                      /* OR  */
    AND = 303,                     /* AND  */
    NOT = 304,                     /* NOT  */
    THEN = 305,                    /* THEN  */
    DELIM = 306                    /* DELIM  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "q2.y"

    char* str;        // For IDs, literals, etc.

#line 119 "q2.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Q2_TAB_H_INCLUDED  */
