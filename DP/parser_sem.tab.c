/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser_sem.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* external lexer interface */
extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int current_line;

/* expose AST root */
AST *astRoot = NULL;

/* derivation file for parser output */
FILE *derivation_file = NULL;

/* Helper to write derivation steps */
static void write_derivation(const char *non_terminal, const char *production) {
    if (derivation_file) {
        fprintf(derivation_file, "%s -> %s\n", non_terminal, production);
    }
}

#line 97 "parser_sem.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser_sem.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_INT_LIT = 4,                    /* INT_LIT  */
  YYSYMBOL_FLOAT_LIT = 5,                  /* FLOAT_LIT  */
  YYSYMBOL_STRING_LIT = 6,                 /* STRING_LIT  */
  YYSYMBOL_CLASS = 7,                      /* CLASS  */
  YYSYMBOL_IMPLEMENT = 8,                  /* IMPLEMENT  */
  YYSYMBOL_FUNC = 9,                       /* FUNC  */
  YYSYMBOL_CONSTRUCT = 10,                 /* CONSTRUCT  */
  YYSYMBOL_ATTRIBUTE = 11,                 /* ATTRIBUTE  */
  YYSYMBOL_PUBLIC = 12,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 13,                   /* PRIVATE  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_READ = 15,                      /* READ  */
  YYSYMBOL_WRITE = 16,                     /* WRITE  */
  YYSYMBOL_IF = 17,                        /* IF  */
  YYSYMBOL_ELSE = 18,                      /* ELSE  */
  YYSYMBOL_WHILE = 19,                     /* WHILE  */
  YYSYMBOL_VOID = 20,                      /* VOID  */
  YYSYMBOL_SELF = 21,                      /* SELF  */
  YYSYMBOL_ISA = 22,                       /* ISA  */
  YYSYMBOL_LOCAL = 23,                     /* LOCAL  */
  YYSYMBOL_THEN = 24,                      /* THEN  */
  YYSYMBOL_COLON = 25,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 26,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 27,                     /* COMMA  */
  YYSYMBOL_DOT = 28,                       /* DOT  */
  YYSYMBOL_ASSIGN = 29,                    /* ASSIGN  */
  YYSYMBOL_ARROW = 30,                     /* ARROW  */
  YYSYMBOL_LPAREN = 31,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 32,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 33,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 34,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 35,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 36,                  /* RBRACKET  */
  YYSYMBOL_PLUS = 37,                      /* PLUS  */
  YYSYMBOL_MINUS = 38,                     /* MINUS  */
  YYSYMBOL_MULT = 39,                      /* MULT  */
  YYSYMBOL_DIV = 40,                       /* DIV  */
  YYSYMBOL_AND = 41,                       /* AND  */
  YYSYMBOL_OR = 42,                        /* OR  */
  YYSYMBOL_NOT = 43,                       /* NOT  */
  YYSYMBOL_EQ = 44,                        /* EQ  */
  YYSYMBOL_NE = 45,                        /* NE  */
  YYSYMBOL_LT = 46,                        /* LT  */
  YYSYMBOL_GT = 47,                        /* GT  */
  YYSYMBOL_LE = 48,                        /* LE  */
  YYSYMBOL_GE = 49,                        /* GE  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_prog = 51,                      /* prog  */
  YYSYMBOL_classDecl = 52,                 /* classDecl  */
  YYSYMBOL_classInherit = 53,              /* classInherit  */
  YYSYMBOL_moreIds = 54,                   /* moreIds  */
  YYSYMBOL_classBody = 55,                 /* classBody  */
  YYSYMBOL_implDef = 56,                   /* implDef  */
  YYSYMBOL_implFuncs = 57,                 /* implFuncs  */
  YYSYMBOL_funcDef = 58,                   /* funcDef  */
  YYSYMBOL_funcHead = 59,                  /* funcHead  */
  YYSYMBOL_funcBody = 60,                  /* funcBody  */
  YYSYMBOL_varDeclOrStmtList = 61,         /* varDeclOrStmtList  */
  YYSYMBOL_varDeclOrStmt = 62,             /* varDeclOrStmt  */
  YYSYMBOL_localVarDecl = 63,              /* localVarDecl  */
  YYSYMBOL_attributeDecl = 64,             /* attributeDecl  */
  YYSYMBOL_varDecl = 65,                   /* varDecl  */
  YYSYMBOL_arraySizes = 66,                /* arraySizes  */
  YYSYMBOL_arraySize = 67,                 /* arraySize  */
  YYSYMBOL_statement = 68,                 /* statement  */
  YYSYMBOL_assignStat = 69,                /* assignStat  */
  YYSYMBOL_statementList = 70,             /* statementList  */
  YYSYMBOL_statBlock = 71,                 /* statBlock  */
  YYSYMBOL_expr = 72,                      /* expr  */
  YYSYMBOL_relExpr = 73,                   /* relExpr  */
  YYSYMBOL_arithExpr = 74,                 /* arithExpr  */
  YYSYMBOL_term = 75,                      /* term  */
  YYSYMBOL_factor = 76,                    /* factor  */
  YYSYMBOL_functionCall = 77,              /* functionCall  */
  YYSYMBOL_fParams = 78,                   /* fParams  */
  YYSYMBOL_fParamsTailList = 79,           /* fParamsTailList  */
  YYSYMBOL_aParams = 80,                   /* aParams  */
  YYSYMBOL_aParamsTailList = 81,           /* aParamsTailList  */
  YYSYMBOL_type = 82                       /* type  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   177

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  167

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   304


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    55,    55,    56,    57,    58,    59,    60,    64,    74,
      82,    86,    92,    96,   102,   108,   112,   121,   122,   126,
     135,   142,   151,   160,   166,   170,   171,   175,   179,   188,
     197,   198,   202,   203,   207,   208,   216,   223,   230,   236,
     242,   246,   257,   258,   262,   263,   264,   268,   269,   270,
     274,   275,   276,   277,   278,   279,   280,   284,   285,   286,
     290,   291,   292,   296,   297,   298,   299,   300,   301,   302,
     306,   315,   322,   326,   333,   337,   338,   342,   343,   347,
     360
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "INT_LIT",
  "FLOAT_LIT", "STRING_LIT", "CLASS", "IMPLEMENT", "FUNC", "CONSTRUCT",
  "ATTRIBUTE", "PUBLIC", "PRIVATE", "RETURN", "READ", "WRITE", "IF",
  "ELSE", "WHILE", "VOID", "SELF", "ISA", "LOCAL", "THEN", "COLON",
  "SEMICOLON", "COMMA", "DOT", "ASSIGN", "ARROW", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "PLUS", "MINUS", "MULT",
  "DIV", "AND", "OR", "NOT", "EQ", "NE", "LT", "GT", "LE", "GE", "$accept",
  "prog", "classDecl", "classInherit", "moreIds", "classBody", "implDef",
  "implFuncs", "funcDef", "funcHead", "funcBody", "varDeclOrStmtList",
  "varDeclOrStmt", "localVarDecl", "attributeDecl", "varDecl",
  "arraySizes", "arraySize", "statement", "assignStat", "statementList",
  "statBlock", "expr", "relExpr", "arithExpr", "term", "factor",
  "functionCall", "fParams", "fParamsTailList", "aParams",
  "aParamsTailList", "type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-125)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      16,    26,    29,    39,    30,    87,    16,  -125,    16,    46,
      71,    76,    83,   112,  -125,  -125,  -125,    54,  -125,   113,
      84,    72,   112,    93,    88,    47,    90,    91,    92,    94,
      98,   116,    96,    54,  -125,  -125,   105,   106,    97,    79,
      99,    72,   102,   132,  -125,     8,     8,     8,   135,     8,
       8,     8,   114,  -125,  -125,  -125,  -125,  -125,   137,  -125,
     130,   130,   108,  -125,  -125,   115,  -125,   119,   117,  -125,
    -125,  -125,     8,     8,    55,  -125,    57,    59,  -125,  -125,
      33,   111,   -14,   118,   -11,    21,    48,   132,    97,   116,
      79,    79,   121,   132,   146,  -125,    27,  -125,     8,     8,
       8,     8,     8,     8,     8,     8,     8,     8,     8,     8,
       8,  -125,  -125,   125,   126,   127,   131,    19,   122,  -125,
    -125,  -125,  -125,  -125,  -125,   129,  -125,    55,    55,    59,
      59,    70,    70,    70,    70,    70,    70,  -125,  -125,    33,
    -125,  -125,  -125,    19,    69,  -125,   133,     5,   134,   122,
     132,  -125,   138,    69,   124,  -125,   128,  -125,  -125,  -125,
     119,    19,  -125,  -125,  -125,  -125,  -125
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,     0,     0,     0,     0,     0,     2,     6,     4,     0,
      10,     0,     0,    72,     1,     3,     5,    24,    19,     0,
       0,    18,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,    25,    26,     0,     0,    12,    15,
       0,    18,     0,    80,    21,     0,    76,     0,     0,     0,
       0,     0,     0,    27,    22,    23,    34,    40,     0,     9,
       0,     0,     0,    16,    17,     0,    79,    74,    64,    65,
      66,    67,     0,     0,    41,    49,    56,    57,    60,    69,
      78,     0,     0,     0,     0,     0,     0,    80,    12,     0,
      15,    15,     0,    80,     0,    71,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    70,     0,     0,     0,     0,    46,    31,    11,
      28,    13,    14,     8,    20,     0,    68,    47,    48,    58,
      59,    50,    51,    52,    53,    54,    55,    61,    62,    78,
      39,    37,    38,    46,    43,    45,     0,     0,     0,    31,
      80,    77,     0,    43,     0,    36,     0,    33,    29,    30,
      74,    46,    42,    44,    32,    73,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -125,    50,  -125,  -125,    73,    20,  -125,   136,    -1,  -125,
    -125,   139,  -125,  -125,   101,    74,    17,  -125,   -16,  -125,
      14,  -124,   -44,  -125,   -57,    12,   -65,   -17,   143,     9,
    -125,    31,   -83
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,    20,    59,    62,     7,    40,     8,     9,
      18,    32,    33,    34,    90,    53,   148,   149,   145,    36,
     154,   146,    74,    75,    76,    77,    78,    79,    24,    95,
      81,   111,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      37,    35,    80,    82,   118,    84,    85,    86,    97,   156,
     124,    68,    69,    70,    71,    96,    37,    35,   113,   152,
      41,   115,    25,     1,     2,     3,     4,    98,    99,    10,
      98,    99,    11,    26,    27,    28,    29,   166,    30,    72,
      41,   157,    12,   137,   138,   131,   132,   133,   134,   135,
     136,    73,   144,   116,   127,   128,    15,    25,    16,   126,
     110,    13,    98,    99,   100,   101,   139,   160,    26,    27,
      28,    29,    25,    30,    98,    99,    45,    31,    46,    17,
     117,     3,     4,    26,    27,    28,    29,    14,    30,    98,
      99,    60,    61,    19,   100,   101,    98,    99,   108,   109,
      37,   102,   103,   104,   105,   106,   107,   100,   101,    21,
     121,   122,   129,   130,    22,    23,    38,    39,    43,    52,
      44,    47,    48,    49,    58,    50,    37,    37,   153,    51,
      54,    56,    57,    63,    65,    66,    37,   153,    83,    87,
      88,    89,    92,   112,    37,    93,    94,   123,    46,   125,
     114,   140,   141,   142,   150,   143,   161,   147,   163,   155,
     158,   119,    91,   120,   164,    42,   159,   162,     0,   165,
     151,     0,    55,     0,     0,     0,     0,    64
};

static const yytype_int16 yycheck[] =
{
      17,    17,    46,    47,    87,    49,    50,    51,    73,     4,
      93,     3,     4,     5,     6,    72,    33,    33,    32,   143,
      21,    32,     3,     7,     8,     9,    10,    41,    42,     3,
      41,    42,     3,    14,    15,    16,    17,   161,    19,    31,
      41,    36,     3,   108,   109,   102,   103,   104,   105,   106,
     107,    43,    33,    32,    98,    99,     6,     3,     8,    32,
      27,    31,    41,    42,    37,    38,   110,   150,    14,    15,
      16,    17,     3,    19,    41,    42,    29,    23,    31,    33,
      32,     9,    10,    14,    15,    16,    17,     0,    19,    41,
      42,    12,    13,    22,    37,    38,    41,    42,    39,    40,
     117,    44,    45,    46,    47,    48,    49,    37,    38,    33,
      90,    91,   100,   101,    31,     3,     3,    33,    25,     3,
      32,    31,    31,    31,    27,    31,   143,   144,   144,    31,
      34,    26,    26,    34,    32,     3,   153,   153,     3,    25,
       3,    11,    34,    32,   161,    30,    27,    26,    31,     3,
      32,    26,    26,    26,    25,    24,    18,    35,    34,    26,
      26,    88,    61,    89,    36,    22,   149,   153,    -1,   160,
     139,    -1,    33,    -1,    -1,    -1,    -1,    41
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,     9,    10,    51,    52,    56,    58,    59,
       3,     3,     3,    31,     0,    51,    51,    33,    60,    22,
      53,    33,    31,     3,    78,     3,    14,    15,    16,    17,
      19,    23,    61,    62,    63,    68,    69,    77,     3,    33,
      57,    58,    78,    25,    32,    29,    31,    31,    31,    31,
      31,    31,     3,    65,    34,    61,    26,    26,    27,    54,
      12,    13,    55,    34,    57,    32,     3,    82,     3,     4,
       5,     6,    31,    43,    72,    73,    74,    75,    76,    77,
      72,    80,    72,     3,    72,    72,    72,    25,     3,    11,
      64,    64,    34,    30,    27,    79,    74,    76,    41,    42,
      37,    38,    44,    45,    46,    47,    48,    49,    39,    40,
      27,    81,    32,    32,    32,    32,    32,    32,    82,    54,
      65,    55,    55,    26,    82,     3,    32,    72,    72,    75,
      75,    74,    74,    74,    74,    74,    74,    76,    76,    72,
      26,    26,    26,    24,    33,    68,    71,    35,    66,    67,
      25,    81,    71,    68,    70,    26,     4,    36,    26,    66,
      82,    18,    70,    34,    36,    79,    71
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    51,    51,    51,    51,    51,    52,    53,
      53,    54,    54,    55,    55,    55,    56,    57,    57,    58,
      59,    59,    60,    61,    61,    62,    62,    63,    64,    65,
      66,    66,    67,    67,    68,    68,    68,    68,    68,    68,
      68,    69,    70,    70,    71,    71,    71,    72,    72,    72,
      73,    73,    73,    73,    73,    73,    73,    74,    74,    74,
      75,    75,    75,    76,    76,    76,    76,    76,    76,    76,
      77,    78,    78,    79,    79,    80,    80,    81,    81,    82,
      82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     0,     7,     3,
       0,     3,     0,     3,     3,     0,     5,     2,     0,     2,
       7,     4,     3,     2,     0,     1,     1,     2,     2,     5,
       2,     0,     3,     2,     2,     8,     6,     5,     5,     5,
       2,     3,     2,     0,     3,     1,     0,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     3,
       1,     3,     3,     2,     1,     1,     1,     1,     3,     1,
       4,     4,     0,     5,     0,     2,     0,     3,     0,     1,
       0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* prog: classDecl  */
#line 55 "parser_sem.y"
                                   { write_derivation("prog", "classDecl"); astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line); ast_append_child(astRoot, (yyvsp[0].node)); }
#line 1397 "parser_sem.tab.c"
    break;

  case 3: /* prog: classDecl prog  */
#line 56 "parser_sem.y"
                                   { write_derivation("prog", "classDecl prog"); if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[-1]).first_line); ast_append_child(astRoot, (yyvsp[-1].node)); }
#line 1403 "parser_sem.tab.c"
    break;

  case 4: /* prog: funcDef  */
#line 57 "parser_sem.y"
                                   { write_derivation("prog", "funcDef"); astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line); ast_append_child(astRoot, (yyvsp[0].node)); }
#line 1409 "parser_sem.tab.c"
    break;

  case 5: /* prog: funcDef prog  */
#line 58 "parser_sem.y"
                                   { write_derivation("prog", "funcDef prog"); if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[-1]).first_line); ast_append_child(astRoot, (yyvsp[-1].node)); }
#line 1415 "parser_sem.tab.c"
    break;

  case 6: /* prog: implDef  */
#line 59 "parser_sem.y"
                                   { write_derivation("prog", "implDef"); if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line); ast_append_child(astRoot, (yyvsp[0].node)); }
#line 1421 "parser_sem.tab.c"
    break;

  case 7: /* prog: %empty  */
#line 60 "parser_sem.y"
                                   { write_derivation("prog", "epsilon"); astRoot = ast_new(NODE_PROGRAM, NULL, 0); }
#line 1427 "parser_sem.tab.c"
    break;

  case 8: /* classDecl: CLASS ID classInherit LBRACE classBody RBRACE SEMICOLON  */
#line 65 "parser_sem.y"
      {
          AST *c = ast_new(NODE_CLASS_DECL, (yyvsp[-5].sVal), (yylsp[-5]).first_line);
          if ((yyvsp[-4].node)) ast_append_child(c, (yyvsp[-4].node));
          if ((yyvsp[-2].node)) ast_append_child(c, (yyvsp[-2].node));
          (yyval.node) = c;
      }
#line 1438 "parser_sem.tab.c"
    break;

  case 9: /* classInherit: ISA ID moreIds  */
#line 75 "parser_sem.y"
      {
          AST *list = ast_new(NODE_CLASS_INHERIT_LIST, NULL, (yylsp[-1]).first_line);
          AST *idnode = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          ast_append_child(list, idnode);
          if ((yyvsp[0].node)) ast_append_child(list, (yyvsp[0].node));
          (yyval.node) = list;
      }
#line 1450 "parser_sem.tab.c"
    break;

  case 10: /* classInherit: %empty  */
#line 82 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1456 "parser_sem.tab.c"
    break;

  case 11: /* moreIds: COMMA ID moreIds  */
#line 87 "parser_sem.y"
      {
          AST *idn = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          if ((yyvsp[0].node)) ast_append_child(idn, (yyvsp[0].node));
          (yyval.node) = idn;
      }
#line 1466 "parser_sem.tab.c"
    break;

  case 12: /* moreIds: %empty  */
#line 92 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1472 "parser_sem.tab.c"
    break;

  case 13: /* classBody: PUBLIC attributeDecl classBody  */
#line 97 "parser_sem.y"
      {
          if ((yyvsp[-1].node) && (yyvsp[-1].node)->child) (yyvsp[-1].node)->child->typeName = strdup("PUBLIC");
          if ((yyvsp[0].node)) ast_append_sibling(&(yyvsp[-1].node), (yyvsp[0].node));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1482 "parser_sem.tab.c"
    break;

  case 14: /* classBody: PRIVATE attributeDecl classBody  */
#line 103 "parser_sem.y"
      {
          if ((yyvsp[-1].node) && (yyvsp[-1].node)->child) (yyvsp[-1].node)->child->typeName = strdup("PRIVATE");
          if ((yyvsp[0].node)) ast_append_sibling(&(yyvsp[-1].node), (yyvsp[0].node));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1492 "parser_sem.tab.c"
    break;

  case 15: /* classBody: %empty  */
#line 108 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1498 "parser_sem.tab.c"
    break;

  case 16: /* implDef: IMPLEMENT ID LBRACE implFuncs RBRACE  */
#line 113 "parser_sem.y"
      {
          AST *n = ast_new(NODE_EMPTY, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) n->child = (yyvsp[-1].node);
          (yyval.node) = n;
      }
#line 1508 "parser_sem.tab.c"
    break;

  case 17: /* implFuncs: funcDef implFuncs  */
#line 121 "parser_sem.y"
                        { (yyval.node) = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&(yyval.node), (yyvsp[0].node)); }
#line 1514 "parser_sem.tab.c"
    break;

  case 18: /* implFuncs: %empty  */
#line 122 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1520 "parser_sem.tab.c"
    break;

  case 19: /* funcDef: funcHead funcBody  */
#line 127 "parser_sem.y"
      {
          AST *f = (yyvsp[-1].node);
          if ((yyvsp[0].node)) f->extra = (yyvsp[0].node);
          (yyval.node) = f;
      }
#line 1530 "parser_sem.tab.c"
    break;

  case 20: /* funcHead: FUNC ID LPAREN fParams RPAREN ARROW type  */
#line 136 "parser_sem.y"
      {
          AST *fn = ast_new(NODE_FUNC_DECL, (yyvsp[-5].sVal), (yylsp[-5]).first_line);
          fn->typeName = (yyvsp[0].node) ? strdup((yyvsp[0].node)->name) : NULL;
          if ((yyvsp[-3].node)) fn->child = (yyvsp[-3].node);
          (yyval.node) = fn;
      }
#line 1541 "parser_sem.tab.c"
    break;

  case 21: /* funcHead: CONSTRUCT LPAREN fParams RPAREN  */
#line 143 "parser_sem.y"
      {
          AST *fn = ast_new(NODE_FUNC_DECL, "constructor", (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) fn->child = (yyvsp[-1].node);
          (yyval.node) = fn;
      }
#line 1551 "parser_sem.tab.c"
    break;

  case 22: /* funcBody: LBRACE varDeclOrStmtList RBRACE  */
#line 152 "parser_sem.y"
      {
          AST *b = ast_new(NODE_FUNC_BODY, NULL, (yylsp[-2]).first_line);
          if ((yyvsp[-1].node)) b->child = (yyvsp[-1].node);
          (yyval.node) = b;
      }
#line 1561 "parser_sem.tab.c"
    break;

  case 23: /* varDeclOrStmtList: varDeclOrStmt varDeclOrStmtList  */
#line 161 "parser_sem.y"
      {
          AST *head = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&head, (yyvsp[0].node));
          (yyval.node) = head;
      }
#line 1571 "parser_sem.tab.c"
    break;

  case 24: /* varDeclOrStmtList: %empty  */
#line 166 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1577 "parser_sem.tab.c"
    break;

  case 25: /* varDeclOrStmt: localVarDecl  */
#line 170 "parser_sem.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1583 "parser_sem.tab.c"
    break;

  case 26: /* varDeclOrStmt: statement  */
#line 171 "parser_sem.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1589 "parser_sem.tab.c"
    break;

  case 27: /* localVarDecl: LOCAL varDecl  */
#line 175 "parser_sem.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1595 "parser_sem.tab.c"
    break;

  case 28: /* attributeDecl: ATTRIBUTE varDecl  */
#line 180 "parser_sem.y"
      {
          AST *attr = ast_new(NODE_ATTRIBUTE, NULL, (yylsp[-1]).first_line);
          if ((yyvsp[0].node)) ast_append_child(attr, (yyvsp[0].node));
          (yyval.node) = attr;
      }
#line 1605 "parser_sem.tab.c"
    break;

  case 29: /* varDecl: ID COLON type arraySizes SEMICOLON  */
#line 189 "parser_sem.y"
      {
          AST *v = ast_new(NODE_VAR_DECL, (yyvsp[-4].sVal), (yylsp[-4]).first_line);
          if ((yyvsp[-2].node)) v->typeName = (yyvsp[-2].node)->name ? strdup((yyvsp[-2].node)->name) : NULL;
          (yyval.node) = v;
      }
#line 1615 "parser_sem.tab.c"
    break;

  case 30: /* arraySizes: arraySize arraySizes  */
#line 197 "parser_sem.y"
                           { (yyval.node) = NULL; }
#line 1621 "parser_sem.tab.c"
    break;

  case 31: /* arraySizes: %empty  */
#line 198 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1627 "parser_sem.tab.c"
    break;

  case 32: /* arraySize: LBRACKET INT_LIT RBRACKET  */
#line 202 "parser_sem.y"
                                { (yyval.node) = ast_new_int((yyvsp[-1].iVal), (yylsp[-1]).first_line); }
#line 1633 "parser_sem.tab.c"
    break;

  case 33: /* arraySize: LBRACKET RBRACKET  */
#line 203 "parser_sem.y"
                        { (yyval.node) = NULL; }
#line 1639 "parser_sem.tab.c"
    break;

  case 34: /* statement: assignStat SEMICOLON  */
#line 207 "parser_sem.y"
                           { (yyval.node) = (yyvsp[-1].node); }
#line 1645 "parser_sem.tab.c"
    break;

  case 35: /* statement: IF LPAREN expr RPAREN THEN statBlock ELSE statBlock  */
#line 209 "parser_sem.y"
      {
          AST *node = ast_new(NODE_IF, NULL, (yylsp[-7]).first_line);
          ast_append_child(node, (yyvsp[-5].node));
          ast_append_child(node, (yyvsp[-2].node));
          ast_append_child(node, (yyvsp[0].node));
          (yyval.node) = node;
      }
#line 1657 "parser_sem.tab.c"
    break;

  case 36: /* statement: WHILE LPAREN expr RPAREN statBlock SEMICOLON  */
#line 217 "parser_sem.y"
      {
          AST *node = ast_new(NODE_WHILE, NULL, (yylsp[-5]).first_line);
          ast_append_child(node, (yyvsp[-3].node));
          ast_append_child(node, (yyvsp[-1].node));
          (yyval.node) = node;
      }
#line 1668 "parser_sem.tab.c"
    break;

  case 37: /* statement: READ LPAREN ID RPAREN SEMICOLON  */
#line 224 "parser_sem.y"
      {
          AST *n = ast_new(NODE_READ, NULL, (yylsp[-4]).first_line);
          AST *idn = ast_new(NODE_ID, (yyvsp[-2].sVal), (yylsp[-2]).first_line);
          ast_append_child(n, idn);
          (yyval.node) = n;
      }
#line 1679 "parser_sem.tab.c"
    break;

  case 38: /* statement: WRITE LPAREN expr RPAREN SEMICOLON  */
#line 231 "parser_sem.y"
      {
          AST *n = ast_new(NODE_WRITE, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1689 "parser_sem.tab.c"
    break;

  case 39: /* statement: RETURN LPAREN expr RPAREN SEMICOLON  */
#line 237 "parser_sem.y"
      {
          AST *n = ast_new(NODE_RETURN, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1699 "parser_sem.tab.c"
    break;

  case 40: /* statement: functionCall SEMICOLON  */
#line 242 "parser_sem.y"
                             { (yyval.node) = (yyvsp[-1].node); }
#line 1705 "parser_sem.tab.c"
    break;

  case 41: /* assignStat: ID ASSIGN expr  */
#line 247 "parser_sem.y"
      {
          AST *lhs = ast_new(NODE_ID, (yyvsp[-2].sVal), (yylsp[-2]).first_line);
          AST *assign = ast_new(NODE_ASSIGN, NULL, (yylsp[-2]).first_line);
          ast_append_child(assign, lhs);
          ast_append_child(assign, (yyvsp[0].node));
          (yyval.node) = assign;
      }
#line 1717 "parser_sem.tab.c"
    break;

  case 42: /* statementList: statement statementList  */
#line 257 "parser_sem.y"
                              { AST *h = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&h, (yyvsp[0].node)); (yyval.node) = h; }
#line 1723 "parser_sem.tab.c"
    break;

  case 43: /* statementList: %empty  */
#line 258 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1729 "parser_sem.tab.c"
    break;

  case 44: /* statBlock: LBRACE statementList RBRACE  */
#line 262 "parser_sem.y"
                                  { (yyval.node) = (yyvsp[-1].node); }
#line 1735 "parser_sem.tab.c"
    break;

  case 45: /* statBlock: statement  */
#line 263 "parser_sem.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1741 "parser_sem.tab.c"
    break;

  case 46: /* statBlock: %empty  */
#line 264 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1747 "parser_sem.tab.c"
    break;

  case 47: /* expr: expr AND expr  */
#line 268 "parser_sem.y"
                      { AST *n = ast_new(NODE_BINARY_OP, "and", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1753 "parser_sem.tab.c"
    break;

  case 48: /* expr: expr OR expr  */
#line 269 "parser_sem.y"
                      { AST *n = ast_new(NODE_BINARY_OP, "or", (yylsp[-1]).first_line);  ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1759 "parser_sem.tab.c"
    break;

  case 49: /* expr: relExpr  */
#line 270 "parser_sem.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1765 "parser_sem.tab.c"
    break;

  case 50: /* relExpr: arithExpr EQ arithExpr  */
#line 274 "parser_sem.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "==", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1771 "parser_sem.tab.c"
    break;

  case 51: /* relExpr: arithExpr NE arithExpr  */
#line 275 "parser_sem.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "<>", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1777 "parser_sem.tab.c"
    break;

  case 52: /* relExpr: arithExpr LT arithExpr  */
#line 276 "parser_sem.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "<", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1783 "parser_sem.tab.c"
    break;

  case 53: /* relExpr: arithExpr GT arithExpr  */
#line 277 "parser_sem.y"
                             { AST *n = ast_new(NODE_BINARY_OP, ">", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1789 "parser_sem.tab.c"
    break;

  case 54: /* relExpr: arithExpr LE arithExpr  */
#line 278 "parser_sem.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "<=", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1795 "parser_sem.tab.c"
    break;

  case 55: /* relExpr: arithExpr GE arithExpr  */
#line 279 "parser_sem.y"
                             { AST *n = ast_new(NODE_BINARY_OP, ">=", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1801 "parser_sem.tab.c"
    break;

  case 56: /* relExpr: arithExpr  */
#line 280 "parser_sem.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1807 "parser_sem.tab.c"
    break;

  case 57: /* arithExpr: term  */
#line 284 "parser_sem.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1813 "parser_sem.tab.c"
    break;

  case 58: /* arithExpr: arithExpr PLUS term  */
#line 285 "parser_sem.y"
                           { AST *n = ast_new(NODE_BINARY_OP, "+", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1819 "parser_sem.tab.c"
    break;

  case 59: /* arithExpr: arithExpr MINUS term  */
#line 286 "parser_sem.y"
                           { AST *n = ast_new(NODE_BINARY_OP, "-", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1825 "parser_sem.tab.c"
    break;

  case 60: /* term: factor  */
#line 290 "parser_sem.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1831 "parser_sem.tab.c"
    break;

  case 61: /* term: term MULT factor  */
#line 291 "parser_sem.y"
                       { AST *n = ast_new(NODE_BINARY_OP, "*", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1837 "parser_sem.tab.c"
    break;

  case 62: /* term: term DIV factor  */
#line 292 "parser_sem.y"
                       { AST *n = ast_new(NODE_BINARY_OP, "/", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1843 "parser_sem.tab.c"
    break;

  case 63: /* factor: NOT factor  */
#line 296 "parser_sem.y"
                 { AST *n = ast_new(NODE_UNARY_OP, "not", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1849 "parser_sem.tab.c"
    break;

  case 64: /* factor: ID  */
#line 297 "parser_sem.y"
                { (yyval.node) = ast_new(NODE_ID, (yyvsp[0].sVal), (yylsp[0]).first_line); }
#line 1855 "parser_sem.tab.c"
    break;

  case 65: /* factor: INT_LIT  */
#line 298 "parser_sem.y"
                { (yyval.node) = ast_new(NODE_INT_LITERAL, NULL, (yylsp[0]).first_line); (yyval.node)->intValue = (yyvsp[0].iVal); }
#line 1861 "parser_sem.tab.c"
    break;

  case 66: /* factor: FLOAT_LIT  */
#line 299 "parser_sem.y"
                { (yyval.node) = ast_new(NODE_FLOAT_LITERAL, NULL, (yylsp[0]).first_line); (yyval.node)->floatValue = (yyvsp[0].dVal); }
#line 1867 "parser_sem.tab.c"
    break;

  case 67: /* factor: STRING_LIT  */
#line 300 "parser_sem.y"
                 { (yyval.node) = ast_new(NODE_STRING_LITERAL, (yyvsp[0].sVal), (yylsp[0]).first_line); }
#line 1873 "parser_sem.tab.c"
    break;

  case 68: /* factor: LPAREN arithExpr RPAREN  */
#line 301 "parser_sem.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1879 "parser_sem.tab.c"
    break;

  case 69: /* factor: functionCall  */
#line 302 "parser_sem.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1885 "parser_sem.tab.c"
    break;

  case 70: /* functionCall: ID LPAREN aParams RPAREN  */
#line 307 "parser_sem.y"
      {
          AST *c = ast_new(NODE_FUNCTION_CALL, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) c->child = (yyvsp[-1].node);
          (yyval.node) = c;
      }
#line 1895 "parser_sem.tab.c"
    break;

  case 71: /* fParams: ID COLON type fParamsTailList  */
#line 316 "parser_sem.y"
      {
          AST *p = ast_new(NODE_PARAM, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) p->typeName = (yyvsp[-1].node)->name ? strdup((yyvsp[-1].node)->name) : NULL;
          if ((yyvsp[0].node)) ast_append_sibling(&p, (yyvsp[0].node));
          (yyval.node) = p;
      }
#line 1906 "parser_sem.tab.c"
    break;

  case 72: /* fParams: %empty  */
#line 322 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1912 "parser_sem.tab.c"
    break;

  case 73: /* fParamsTailList: COMMA ID COLON type fParamsTailList  */
#line 327 "parser_sem.y"
      {
          AST *p = ast_new(NODE_PARAM, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) p->typeName = (yyvsp[-1].node)->name ? strdup((yyvsp[-1].node)->name) : NULL;
          if ((yyvsp[0].node)) ast_append_sibling(&p, (yyvsp[0].node));
          (yyval.node) = p;
      }
#line 1923 "parser_sem.tab.c"
    break;

  case 74: /* fParamsTailList: %empty  */
#line 333 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1929 "parser_sem.tab.c"
    break;

  case 75: /* aParams: expr aParamsTailList  */
#line 337 "parser_sem.y"
                           { AST *h = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&h, (yyvsp[0].node)); (yyval.node) = h; }
#line 1935 "parser_sem.tab.c"
    break;

  case 76: /* aParams: %empty  */
#line 338 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1941 "parser_sem.tab.c"
    break;

  case 77: /* aParamsTailList: COMMA expr aParamsTailList  */
#line 342 "parser_sem.y"
                                 { AST *n = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1947 "parser_sem.tab.c"
    break;

  case 78: /* aParamsTailList: %empty  */
#line 343 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1953 "parser_sem.tab.c"
    break;

  case 79: /* type: ID  */
#line 348 "parser_sem.y"
      {
          const char *raw = (yyvsp[0].sVal);
          char buf[64] = {0};
          if (raw) {
              if (strcmp(raw, "int") == 0 || strcmp(raw, "integer") == 0) strncpy(buf, "integer", sizeof(buf)-1);
              else if (strcmp(raw, "float") == 0) strncpy(buf, "float", sizeof(buf)-1);
              else if (strcmp(raw, "void") == 0) strncpy(buf, "void", sizeof(buf)-1);
              else strncpy(buf, raw, sizeof(buf)-1);
          }
          AST *t = ast_new(NODE_TYPE, buf[0] ? strdup(buf) : NULL, (yylsp[0]).first_line);
          (yyval.node) = t;
      }
#line 1970 "parser_sem.tab.c"
    break;

  case 80: /* type: %empty  */
#line 360 "parser_sem.y"
                  { (yyval.node) = NULL; }
#line 1976 "parser_sem.tab.c"
    break;


#line 1980 "parser_sem.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 363 "parser_sem.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", current_line, s);
}


