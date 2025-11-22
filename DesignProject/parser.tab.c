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
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* external lexer interface */
extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int current_line;
extern FILE *derivation_file;
static void log_production(const char *rule);

/* expose AST root */
AST *astRoot = NULL;

#line 89 "parser.tab.c"

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

#include "parser.tab.h"
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
  YYSYMBOL_INTEGER_T = 25,                 /* INTEGER_T  */
  YYSYMBOL_FLOAT_T = 26,                   /* FLOAT_T  */
  YYSYMBOL_COLON = 27,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 28,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 29,                     /* COMMA  */
  YYSYMBOL_DOT = 30,                       /* DOT  */
  YYSYMBOL_ASSIGN = 31,                    /* ASSIGN  */
  YYSYMBOL_ARROW = 32,                     /* ARROW  */
  YYSYMBOL_LPAREN = 33,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 34,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 35,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 36,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 37,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 38,                  /* RBRACKET  */
  YYSYMBOL_PLUS = 39,                      /* PLUS  */
  YYSYMBOL_MINUS = 40,                     /* MINUS  */
  YYSYMBOL_MULT = 41,                      /* MULT  */
  YYSYMBOL_DIV = 42,                       /* DIV  */
  YYSYMBOL_AND = 43,                       /* AND  */
  YYSYMBOL_OR = 44,                        /* OR  */
  YYSYMBOL_NOT = 45,                       /* NOT  */
  YYSYMBOL_EQ = 46,                        /* EQ  */
  YYSYMBOL_NE = 47,                        /* NE  */
  YYSYMBOL_LT = 48,                        /* LT  */
  YYSYMBOL_GT = 49,                        /* GT  */
  YYSYMBOL_LE = 50,                        /* LE  */
  YYSYMBOL_GE = 51,                        /* GE  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_prog = 53,                      /* prog  */
  YYSYMBOL_classDecl = 54,                 /* classDecl  */
  YYSYMBOL_classInherit = 55,              /* classInherit  */
  YYSYMBOL_moreIds = 56,                   /* moreIds  */
  YYSYMBOL_classBody = 57,                 /* classBody  */
  YYSYMBOL_implDef = 58,                   /* implDef  */
  YYSYMBOL_implFuncs = 59,                 /* implFuncs  */
  YYSYMBOL_funcDef = 60,                   /* funcDef  */
  YYSYMBOL_funcHead = 61,                  /* funcHead  */
  YYSYMBOL_funcBody = 62,                  /* funcBody  */
  YYSYMBOL_varDeclOrStmtList = 63,         /* varDeclOrStmtList  */
  YYSYMBOL_varDeclOrStmt = 64,             /* varDeclOrStmt  */
  YYSYMBOL_localVarDecl = 65,              /* localVarDecl  */
  YYSYMBOL_attributeDecl = 66,             /* attributeDecl  */
  YYSYMBOL_varDecl = 67,                   /* varDecl  */
  YYSYMBOL_arraySizes = 68,                /* arraySizes  */
  YYSYMBOL_arraySize = 69,                 /* arraySize  */
  YYSYMBOL_statement = 70,                 /* statement  */
  YYSYMBOL_assignStat = 71,                /* assignStat  */
  YYSYMBOL_statementList = 72,             /* statementList  */
  YYSYMBOL_statBlock = 73,                 /* statBlock  */
  YYSYMBOL_expr = 74,                      /* expr  */
  YYSYMBOL_relExpr = 75,                   /* relExpr  */
  YYSYMBOL_arithExpr = 76,                 /* arithExpr  */
  YYSYMBOL_term = 77,                      /* term  */
  YYSYMBOL_factor = 78,                    /* factor  */
  YYSYMBOL_functionCall = 79,              /* functionCall  */
  YYSYMBOL_fParams = 80,                   /* fParams  */
  YYSYMBOL_fParamsTailList = 81,           /* fParamsTailList  */
  YYSYMBOL_aParams = 82,                   /* aParams  */
  YYSYMBOL_aParamsTailList = 83,           /* aParamsTailList  */
  YYSYMBOL_type = 84,                      /* type  */
  YYSYMBOL_returnType = 85                 /* returnType  */
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
#define YYLAST   205

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  173

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


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
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    53,    53,    59,    65,    71,    77,    84,    92,   103,
     113,   120,   128,   136,   146,   157,   165,   175,   182,   190,
     201,   212,   223,   233,   241,   248,   253,   261,   269,   279,
     292,   298,   305,   310,   319,   324,   333,   341,   349,   356,
     363,   368,   378,   390,   398,   405,   410,   416,   424,   432,
     440,   448,   456,   464,   472,   480,   488,   496,   504,   509,
     517,   528,   533,   541,   552,   559,   564,   571,   578,   583,
     588,   597,   608,   620,   627,   639,   646,   654,   661,   669,
     677,   683,   689,   696,   703,   708
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
  "ELSE", "WHILE", "VOID", "SELF", "ISA", "LOCAL", "THEN", "INTEGER_T",
  "FLOAT_T", "COLON", "SEMICOLON", "COMMA", "DOT", "ASSIGN", "ARROW",
  "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "PLUS",
  "MINUS", "MULT", "DIV", "AND", "OR", "NOT", "EQ", "NE", "LT", "GT", "LE",
  "GE", "$accept", "prog", "classDecl", "classInherit", "moreIds",
  "classBody", "implDef", "implFuncs", "funcDef", "funcHead", "funcBody",
  "varDeclOrStmtList", "varDeclOrStmt", "localVarDecl", "attributeDecl",
  "varDecl", "arraySizes", "arraySize", "statement", "assignStat",
  "statementList", "statBlock", "expr", "relExpr", "arithExpr", "term",
  "factor", "functionCall", "fParams", "fParamsTailList", "aParams",
  "aParamsTailList", "type", "returnType", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-129)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-48)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     106,    13,    25,    56,    51,    80,   106,  -129,   106,    66,
      81,    74,    88,   114,  -129,  -129,  -129,    47,  -129,   120,
      90,    42,   114,    64,    97,   113,    -6,   122,   128,   131,
     132,   134,   146,   130,    47,  -129,  -129,   140,   141,   142,
     133,   136,    42,   139,    20,  -129,  -129,     8,     8,     8,
     167,     8,     8,     8,   147,  -129,  -129,  -129,  -129,  -129,
     172,  -129,   165,   165,   143,  -129,  -129,   145,  -129,  -129,
    -129,   149,   148,  -129,  -129,  -129,     8,     8,   104,  -129,
      39,   116,  -129,  -129,    38,   150,    68,   151,    84,    86,
     110,    20,   142,   146,   133,   133,   152,     6,   179,  -129,
     -10,  -129,     8,     8,     8,     8,     8,     8,     8,     8,
       8,     8,     8,     8,     8,  -129,  -129,   155,   158,   159,
     164,    91,   153,  -129,  -129,  -129,  -129,  -129,  -129,  -129,
    -129,   162,  -129,   104,   104,   116,   116,   112,   112,   112,
     112,   112,   112,  -129,  -129,    38,  -129,  -129,  -129,   121,
      57,  -129,   163,    11,   166,   153,    20,  -129,   174,    57,
     157,  -129,   160,  -129,  -129,  -129,   149,    19,  -129,  -129,
    -129,  -129,  -129
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,     0,     0,     0,     0,     0,     2,     6,     4,     0,
      10,     0,     0,    73,     1,     3,     5,     0,    19,     0,
       0,    18,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    25,    26,     0,     0,    12,
      15,     0,    18,     0,    83,    21,    41,     0,    77,     0,
       0,     0,     0,     0,     0,    27,    22,    23,    34,    40,
       0,     9,     0,     0,     0,    16,    17,     0,    82,    80,
      81,    75,    65,    66,    67,    68,     0,     0,    42,    50,
      57,    58,    61,    70,    79,     0,     0,     0,     0,     0,
       0,    83,    12,     0,    15,    15,     0,    83,     0,    72,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,    71,     0,     0,     0,
       0,     0,    31,    11,    28,    13,    14,     8,    85,    84,
      20,     0,    69,    48,    49,    59,    60,    51,    52,    53,
      54,    55,    56,    62,    63,    79,    39,    37,    38,     0,
       0,    46,     0,     0,     0,    31,    83,    78,     0,     0,
       0,    36,     0,    33,    29,    30,    75,     0,    43,    45,
      32,    74,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -129,    69,  -129,  -129,   103,    65,  -129,   154,    -2,  -129,
    -129,   168,  -129,  -129,   137,   108,    44,  -129,   -16,  -129,
      45,  -128,   -46,  -129,   -11,    58,   -69,   -17,   175,    37,
    -129,    60,   -87,  -129
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,    20,    61,    64,     7,    41,     8,     9,
      18,    33,    34,    35,    94,    55,   154,   155,   151,    37,
     160,   152,    78,    79,    80,    81,    82,    83,    24,    99,
      85,   115,    71,   130
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    36,    84,    86,   122,    88,    89,    90,   101,    68,
     129,    72,    73,    74,    75,   162,    10,    38,    36,    42,
      25,   158,    26,    68,   132,    47,   128,    48,    11,   104,
     105,    69,    70,    27,    28,    29,    30,   -47,    31,   172,
      42,    76,   -47,   143,   144,    69,    70,   -47,    25,   163,
      26,     3,     4,    77,   150,   -47,   133,   134,    25,    12,
      26,    27,    28,    29,    30,   100,    31,   114,   145,   166,
      32,    27,    28,    29,    30,    15,    31,    16,   104,   105,
      14,   102,   103,   -24,    13,   106,   107,   108,   109,   110,
     111,    44,    25,   -44,    26,   137,   138,   139,   140,   141,
     142,    17,   117,    19,    38,    27,    28,    29,    30,    21,
      31,   102,   103,     1,     2,     3,     4,    23,   119,   -47,
     120,    22,    25,    39,    26,    40,   150,   102,   103,   102,
     103,    45,    38,    38,   159,    27,    28,    29,    30,   -47,
      31,    46,    38,   159,   121,    62,    63,   102,   103,    54,
      38,   104,   105,   102,   103,    49,   150,   112,   113,   125,
     126,    50,   135,   136,    51,    52,    56,    53,    58,    59,
      87,    60,    65,    67,    91,    92,    93,    97,    98,    96,
     127,    48,   131,   146,   116,   118,   147,   148,   149,   156,
     153,   161,   167,   169,   164,   123,    66,    43,   170,   165,
      95,   124,    57,   171,   168,   157
};

static const yytype_uint8 yycheck[] =
{
      17,    17,    48,    49,    91,    51,    52,    53,    77,     3,
      97,     3,     4,     5,     6,     4,     3,    34,    34,    21,
       1,   149,     3,     3,    34,    31,    20,    33,     3,    39,
      40,    25,    26,    14,    15,    16,    17,    18,    19,   167,
      42,    33,    23,   112,   113,    25,    26,    28,     1,    38,
       3,     9,    10,    45,    35,    36,   102,   103,     1,     3,
       3,    14,    15,    16,    17,    76,    19,    29,   114,   156,
      23,    14,    15,    16,    17,     6,    19,     8,    39,    40,
       0,    43,    44,    36,    33,    46,    47,    48,    49,    50,
      51,    27,     1,    36,     3,   106,   107,   108,   109,   110,
     111,    35,    34,    22,   121,    14,    15,    16,    17,    35,
      19,    43,    44,     7,     8,     9,    10,     3,    34,    28,
      34,    33,     1,     3,     3,    35,    35,    43,    44,    43,
      44,    34,   149,   150,   150,    14,    15,    16,    17,    18,
      19,    28,   159,   159,    34,    12,    13,    43,    44,     3,
     167,    39,    40,    43,    44,    33,    35,    41,    42,    94,
      95,    33,   104,   105,    33,    33,    36,    33,    28,    28,
       3,    29,    36,    34,    27,     3,    11,    32,    29,    36,
      28,    33,     3,    28,    34,    34,    28,    28,    24,    27,
      37,    28,    18,    36,    28,    92,    42,    22,    38,   155,
      63,    93,    34,   166,   159,   145
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,     9,    10,    53,    54,    58,    60,    61,
       3,     3,     3,    33,     0,    53,    53,    35,    62,    22,
      55,    35,    33,     3,    80,     1,     3,    14,    15,    16,
      17,    19,    23,    63,    64,    65,    70,    71,    79,     3,
      35,    59,    60,    80,    27,    34,    28,    31,    33,    33,
      33,    33,    33,    33,     3,    67,    36,    63,    28,    28,
      29,    56,    12,    13,    57,    36,    59,    34,     3,    25,
      26,    84,     3,     4,     5,     6,    33,    45,    74,    75,
      76,    77,    78,    79,    74,    82,    74,     3,    74,    74,
      74,    27,     3,    11,    66,    66,    36,    32,    29,    81,
      76,    78,    43,    44,    39,    40,    46,    47,    48,    49,
      50,    51,    41,    42,    29,    83,    34,    34,    34,    34,
      34,    34,    84,    56,    67,    57,    57,    28,    20,    84,
      85,     3,    34,    74,    74,    77,    77,    76,    76,    76,
      76,    76,    76,    78,    78,    74,    28,    28,    28,    24,
      35,    70,    73,    37,    68,    69,    27,    83,    73,    70,
      72,    28,     4,    38,    28,    68,    84,    18,    72,    36,
      38,    81,    73
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    53,    53,    53,    53,    53,    54,    55,
      55,    56,    56,    57,    57,    57,    58,    59,    59,    60,
      61,    61,    62,    63,    63,    64,    64,    65,    66,    67,
      68,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    71,    72,    72,    73,    73,    73,    74,    74,
      74,    75,    75,    75,    75,    75,    75,    75,    76,    76,
      76,    77,    77,    77,    78,    78,    78,    78,    78,    78,
      78,    79,    80,    80,    81,    81,    82,    82,    83,    83,
      84,    84,    84,    84,    85,    85
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     0,     7,     3,
       0,     3,     0,     3,     3,     0,     5,     2,     0,     2,
       7,     4,     3,     2,     0,     1,     1,     2,     2,     5,
       2,     0,     3,     2,     2,     8,     6,     5,     5,     5,
       2,     2,     3,     2,     0,     3,     1,     0,     3,     3,
       1,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       3,     1,     3,     3,     2,     1,     1,     1,     1,     3,
       1,     4,     4,     0,     5,     0,     2,     0,     3,     0,
       1,     1,     1,     0,     1,     1
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
#line 54 "parser.y"
      {
          log_production("prog -> classDecl");
          astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line);
          ast_append_child(astRoot, (yyvsp[0].node));
      }
#line 1405 "parser.tab.c"
    break;

  case 3: /* prog: classDecl prog  */
#line 60 "parser.y"
      {
          log_production("prog -> classDecl prog");
          if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[-1]).first_line);
          ast_append_child(astRoot, (yyvsp[-1].node));
      }
#line 1415 "parser.tab.c"
    break;

  case 4: /* prog: funcDef  */
#line 66 "parser.y"
      {
          log_production("prog -> funcDef");
          astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line);
          ast_append_child(astRoot, (yyvsp[0].node));
      }
#line 1425 "parser.tab.c"
    break;

  case 5: /* prog: funcDef prog  */
#line 72 "parser.y"
      {
          log_production("prog -> funcDef prog");
          if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[-1]).first_line);
          ast_append_child(astRoot, (yyvsp[-1].node));
      }
#line 1435 "parser.tab.c"
    break;

  case 6: /* prog: implDef  */
#line 78 "parser.y"
      {
          log_production("prog -> implDef");
          if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line);
          ast_append_child(astRoot, (yyvsp[0].node));
      }
#line 1445 "parser.tab.c"
    break;

  case 7: /* prog: %empty  */
#line 84 "parser.y"
      {
          log_production("prog -> epsilon");
          astRoot = ast_new(NODE_PROGRAM, NULL, 0);
      }
#line 1454 "parser.tab.c"
    break;

  case 8: /* classDecl: CLASS ID classInherit LBRACE classBody RBRACE SEMICOLON  */
#line 93 "parser.y"
      {
          log_production("classDecl -> CLASS id classInherit { classBody } ;");
          AST *c = ast_new(NODE_CLASS_DECL, (yyvsp[-5].sVal), (yylsp[-5]).first_line);
          if ((yyvsp[-4].node)) ast_append_child(c, (yyvsp[-4].node));
          if ((yyvsp[-2].node)) ast_append_child(c, (yyvsp[-2].node));
          (yyval.node) = c;
      }
#line 1466 "parser.tab.c"
    break;

  case 9: /* classInherit: ISA ID moreIds  */
#line 104 "parser.y"
      {
          log_production("classInherit -> ISA id moreIds");
          AST *list = ast_new(NODE_CLASS_INHERIT_LIST, NULL, (yylsp[-1]).first_line);
          AST *idnode = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          ast_append_child(list, idnode);
          if ((yyvsp[0].node)) ast_append_child(list, (yyvsp[0].node));
          (yyval.node) = list;
      }
#line 1479 "parser.tab.c"
    break;

  case 10: /* classInherit: %empty  */
#line 113 "parser.y"
      {
          log_production("classInherit -> epsilon");
          (yyval.node) = NULL;
      }
#line 1488 "parser.tab.c"
    break;

  case 11: /* moreIds: COMMA ID moreIds  */
#line 121 "parser.y"
      {
          log_production("moreIds -> , id moreIds");
          AST *idn = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          if ((yyvsp[0].node)) ast_append_child(idn, (yyvsp[0].node));
          (yyval.node) = idn;
      }
#line 1499 "parser.tab.c"
    break;

  case 12: /* moreIds: %empty  */
#line 128 "parser.y"
      {
          log_production("moreIds -> epsilon");
          (yyval.node) = NULL;
      }
#line 1508 "parser.tab.c"
    break;

  case 13: /* classBody: PUBLIC attributeDecl classBody  */
#line 137 "parser.y"
      {
          log_production("classBody -> PUBLIC attributeDecl classBody");
          if ((yyvsp[-1].node)) {
              if ((yyvsp[-1].node)->typeName) free((yyvsp[-1].node)->typeName);
              (yyvsp[-1].node)->typeName = strdup("public");
          }
          if ((yyvsp[0].node)) ast_append_sibling(&(yyvsp[-1].node), (yyvsp[0].node));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1522 "parser.tab.c"
    break;

  case 14: /* classBody: PRIVATE attributeDecl classBody  */
#line 147 "parser.y"
      {
          log_production("classBody -> PRIVATE attributeDecl classBody");
          if ((yyvsp[-1].node)) {
              if ((yyvsp[-1].node)->typeName) free((yyvsp[-1].node)->typeName);
              (yyvsp[-1].node)->typeName = strdup("private");
          }
          if ((yyvsp[0].node)) ast_append_sibling(&(yyvsp[-1].node), (yyvsp[0].node));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1536 "parser.tab.c"
    break;

  case 15: /* classBody: %empty  */
#line 157 "parser.y"
      {
          log_production("classBody -> epsilon");
          (yyval.node) = NULL;
      }
#line 1545 "parser.tab.c"
    break;

  case 16: /* implDef: IMPLEMENT ID LBRACE implFuncs RBRACE  */
#line 166 "parser.y"
      {
          log_production("implDef -> IMPLEMENT id { implFuncs }");
          AST *n = ast_new(NODE_EMPTY, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) n->child = (yyvsp[-1].node);
          (yyval.node) = n;
      }
#line 1556 "parser.tab.c"
    break;

  case 17: /* implFuncs: funcDef implFuncs  */
#line 176 "parser.y"
      {
          log_production("implFuncs -> funcDef implFuncs");
          (yyval.node) = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&(yyval.node), (yyvsp[0].node));
      }
#line 1566 "parser.tab.c"
    break;

  case 18: /* implFuncs: %empty  */
#line 182 "parser.y"
      {
          log_production("implFuncs -> epsilon");
          (yyval.node) = NULL;
      }
#line 1575 "parser.tab.c"
    break;

  case 19: /* funcDef: funcHead funcBody  */
#line 191 "parser.y"
      {
          log_production("funcDef -> funcHead funcBody");
          AST *f = (yyvsp[-1].node);
          if ((yyvsp[0].node)) f->extra = (yyvsp[0].node);
          (yyval.node) = f;
      }
#line 1586 "parser.tab.c"
    break;

  case 20: /* funcHead: FUNC ID LPAREN fParams RPAREN ARROW returnType  */
#line 202 "parser.y"
      {
          log_production("funcHead -> FUNC id ( fParams ) ARROW returnType");
          AST *fn = ast_new(NODE_FUNC_DECL, (yyvsp[-5].sVal), (yylsp[-5]).first_line);
          if ((yyvsp[0].node)) {
              fn->typeName = (yyvsp[0].node)->name ? strdup((yyvsp[0].node)->name) : NULL;
              ast_free((yyvsp[0].node));
          }
          if ((yyvsp[-3].node)) fn->child = (yyvsp[-3].node);
          (yyval.node) = fn;
      }
#line 1601 "parser.tab.c"
    break;

  case 21: /* funcHead: CONSTRUCT LPAREN fParams RPAREN  */
#line 213 "parser.y"
      {
          log_production("funcHead -> CONSTRUCT ( fParams )");
          AST *fn = ast_new(NODE_FUNC_DECL, "constructor", (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) fn->child = (yyvsp[-1].node);
          (yyval.node) = fn;
      }
#line 1612 "parser.tab.c"
    break;

  case 22: /* funcBody: LBRACE varDeclOrStmtList RBRACE  */
#line 224 "parser.y"
      {
          log_production("funcBody -> { varDeclOrStmtList }");
          AST *b = ast_new(NODE_FUNC_BODY, NULL, (yylsp[-2]).first_line);
          if ((yyvsp[-1].node)) b->child = (yyvsp[-1].node);
          (yyval.node) = b;
      }
#line 1623 "parser.tab.c"
    break;

  case 23: /* varDeclOrStmtList: varDeclOrStmt varDeclOrStmtList  */
#line 234 "parser.y"
      {
          log_production("varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList");
          AST *head = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&head, (yyvsp[0].node));
          (yyval.node) = head;
      }
#line 1634 "parser.tab.c"
    break;

  case 24: /* varDeclOrStmtList: %empty  */
#line 241 "parser.y"
      {
          log_production("varDeclOrStmtList -> epsilon");
          (yyval.node) = NULL;
      }
#line 1643 "parser.tab.c"
    break;

  case 25: /* varDeclOrStmt: localVarDecl  */
#line 249 "parser.y"
      {
          log_production("varDeclOrStmt -> localVarDecl");
          (yyval.node) = (yyvsp[0].node);
      }
#line 1652 "parser.tab.c"
    break;

  case 26: /* varDeclOrStmt: statement  */
#line 254 "parser.y"
      {
          log_production("varDeclOrStmt -> statement");
          (yyval.node) = (yyvsp[0].node);
      }
#line 1661 "parser.tab.c"
    break;

  case 27: /* localVarDecl: LOCAL varDecl  */
#line 262 "parser.y"
      {
          log_production("localVarDecl -> LOCAL varDecl");
          (yyval.node) = (yyvsp[0].node);
      }
#line 1670 "parser.tab.c"
    break;

  case 28: /* attributeDecl: ATTRIBUTE varDecl  */
#line 270 "parser.y"
      {
          log_production("attributeDecl -> ATTRIBUTE varDecl");
          AST *attr = ast_new(NODE_ATTRIBUTE, NULL, (yylsp[-1]).first_line);
          if ((yyvsp[0].node)) ast_append_child(attr, (yyvsp[0].node));
          (yyval.node) = attr;
      }
#line 1681 "parser.tab.c"
    break;

  case 29: /* varDecl: ID COLON type arraySizes SEMICOLON  */
#line 280 "parser.y"
      {
          log_production("varDecl -> id : type arraySizes ;");
          AST *v = ast_new(NODE_VAR_DECL, (yyvsp[-4].sVal), (yylsp[-4]).first_line);
          if ((yyvsp[-2].node)) {
              v->typeName = (yyvsp[-2].node)->name ? strdup((yyvsp[-2].node)->name) : NULL;
              ast_free((yyvsp[-2].node));
          }
          (yyval.node) = v;
      }
#line 1695 "parser.tab.c"
    break;

  case 30: /* arraySizes: arraySize arraySizes  */
#line 293 "parser.y"
      {
          log_production("arraySizes -> arraySize arraySizes");
          (yyval.node) = NULL;
      }
#line 1704 "parser.tab.c"
    break;

  case 31: /* arraySizes: %empty  */
#line 298 "parser.y"
      {
          log_production("arraySizes -> epsilon");
          (yyval.node) = NULL;
      }
#line 1713 "parser.tab.c"
    break;

  case 32: /* arraySize: LBRACKET INT_LIT RBRACKET  */
#line 306 "parser.y"
      {
          log_production("arraySize -> [ INT ]");
          (yyval.node) = ast_new_int((yyvsp[-1].iVal), (yylsp[-1]).first_line);
      }
#line 1722 "parser.tab.c"
    break;

  case 33: /* arraySize: LBRACKET RBRACKET  */
#line 311 "parser.y"
      {
          log_production("arraySize -> [ ]");
          (yyval.node) = NULL;
      }
#line 1731 "parser.tab.c"
    break;

  case 34: /* statement: assignStat SEMICOLON  */
#line 320 "parser.y"
      {
          log_production("statement -> assignStat ;");
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1740 "parser.tab.c"
    break;

  case 35: /* statement: IF LPAREN expr RPAREN THEN statBlock ELSE statBlock  */
#line 325 "parser.y"
      {
          log_production("statement -> IF ( expr ) THEN statBlock ELSE statBlock");
          AST *node = ast_new(NODE_IF, NULL, (yylsp[-7]).first_line);
          ast_append_child(node, (yyvsp[-5].node));
          ast_append_child(node, (yyvsp[-2].node));
          ast_append_child(node, (yyvsp[0].node));
          (yyval.node) = node;
      }
#line 1753 "parser.tab.c"
    break;

  case 36: /* statement: WHILE LPAREN expr RPAREN statBlock SEMICOLON  */
#line 334 "parser.y"
      {
          log_production("statement -> WHILE ( expr ) statBlock ;");
          AST *node = ast_new(NODE_WHILE, NULL, (yylsp[-5]).first_line);
          ast_append_child(node, (yyvsp[-3].node));
          ast_append_child(node, (yyvsp[-1].node));
          (yyval.node) = node;
      }
#line 1765 "parser.tab.c"
    break;

  case 37: /* statement: READ LPAREN ID RPAREN SEMICOLON  */
#line 342 "parser.y"
      {
          log_production("statement -> READ ( id ) ;");
          AST *n = ast_new(NODE_READ, NULL, (yylsp[-4]).first_line);
          AST *idn = ast_new(NODE_ID, (yyvsp[-2].sVal), (yylsp[-2]).first_line);
          ast_append_child(n, idn);
          (yyval.node) = n;
      }
#line 1777 "parser.tab.c"
    break;

  case 38: /* statement: WRITE LPAREN expr RPAREN SEMICOLON  */
#line 350 "parser.y"
      {
          log_production("statement -> WRITE ( expr ) ;");
          AST *n = ast_new(NODE_WRITE, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1788 "parser.tab.c"
    break;

  case 39: /* statement: RETURN LPAREN expr RPAREN SEMICOLON  */
#line 357 "parser.y"
      {
          log_production("statement -> RETURN ( expr ) ;");
          AST *n = ast_new(NODE_RETURN, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1799 "parser.tab.c"
    break;

  case 40: /* statement: functionCall SEMICOLON  */
#line 364 "parser.y"
      {
          log_production("statement -> functionCall ;");
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1808 "parser.tab.c"
    break;

  case 41: /* statement: error SEMICOLON  */
#line 369 "parser.y"
      {
          log_production("statement -> error ;");
          yyerror("Recovering from statement error");
          yyerrok;
          (yyval.node) = NULL;
      }
#line 1819 "parser.tab.c"
    break;

  case 42: /* assignStat: ID ASSIGN expr  */
#line 379 "parser.y"
      {
          log_production("assignStat -> id ASSIGN expr");
          AST *lhs = ast_new(NODE_ID, (yyvsp[-2].sVal), (yylsp[-2]).first_line);
          AST *assign = ast_new(NODE_ASSIGN, NULL, (yylsp[-2]).first_line);
          ast_append_child(assign, lhs);
          ast_append_child(assign, (yyvsp[0].node));
          (yyval.node) = assign;
      }
#line 1832 "parser.tab.c"
    break;

  case 43: /* statementList: statement statementList  */
#line 391 "parser.y"
      {
          log_production("statementList -> statement statementList");
          AST *h = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&h, (yyvsp[0].node));
          (yyval.node) = h;
      }
#line 1843 "parser.tab.c"
    break;

  case 44: /* statementList: %empty  */
#line 398 "parser.y"
      {
          log_production("statementList -> epsilon");
          (yyval.node) = NULL;
      }
#line 1852 "parser.tab.c"
    break;

  case 45: /* statBlock: LBRACE statementList RBRACE  */
#line 406 "parser.y"
      {
          log_production("statBlock -> { statementList }");
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1861 "parser.tab.c"
    break;

  case 46: /* statBlock: statement  */
#line 411 "parser.y"
      {
          log_production("statBlock -> statement");
          (yyval.node) = (yyvsp[0].node);
      }
#line 1870 "parser.tab.c"
    break;

  case 47: /* statBlock: %empty  */
#line 416 "parser.y"
      {
          log_production("statBlock -> epsilon");
          (yyval.node) = NULL;
      }
#line 1879 "parser.tab.c"
    break;

  case 48: /* expr: expr AND expr  */
#line 425 "parser.y"
      {
          log_production("expr -> expr AND expr");
          AST *n = ast_new(NODE_BINARY_OP, "and", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1891 "parser.tab.c"
    break;

  case 49: /* expr: expr OR expr  */
#line 433 "parser.y"
      {
          log_production("expr -> expr OR expr");
          AST *n = ast_new(NODE_BINARY_OP, "or", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1903 "parser.tab.c"
    break;

  case 50: /* expr: relExpr  */
#line 441 "parser.y"
      {
          log_production("expr -> relExpr");
          (yyval.node) = (yyvsp[0].node);
      }
#line 1912 "parser.tab.c"
    break;

  case 51: /* relExpr: arithExpr EQ arithExpr  */
#line 449 "parser.y"
      {
          log_production("relExpr -> arithExpr == arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "==", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1924 "parser.tab.c"
    break;

  case 52: /* relExpr: arithExpr NE arithExpr  */
#line 457 "parser.y"
      {
          log_production("relExpr -> arithExpr <> arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "<>", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1936 "parser.tab.c"
    break;

  case 53: /* relExpr: arithExpr LT arithExpr  */
#line 465 "parser.y"
      {
          log_production("relExpr -> arithExpr < arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "<", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1948 "parser.tab.c"
    break;

  case 54: /* relExpr: arithExpr GT arithExpr  */
#line 473 "parser.y"
      {
          log_production("relExpr -> arithExpr > arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, ">", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1960 "parser.tab.c"
    break;

  case 55: /* relExpr: arithExpr LE arithExpr  */
#line 481 "parser.y"
      {
          log_production("relExpr -> arithExpr <= arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "<=", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1972 "parser.tab.c"
    break;

  case 56: /* relExpr: arithExpr GE arithExpr  */
#line 489 "parser.y"
      {
          log_production("relExpr -> arithExpr >= arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, ">=", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 1984 "parser.tab.c"
    break;

  case 57: /* relExpr: arithExpr  */
#line 497 "parser.y"
      {
          log_production("relExpr -> arithExpr");
          (yyval.node) = (yyvsp[0].node);
      }
#line 1993 "parser.tab.c"
    break;

  case 58: /* arithExpr: term  */
#line 505 "parser.y"
      {
          log_production("arithExpr -> term");
          (yyval.node) = (yyvsp[0].node);
      }
#line 2002 "parser.tab.c"
    break;

  case 59: /* arithExpr: arithExpr PLUS term  */
#line 510 "parser.y"
      {
          log_production("arithExpr -> arithExpr + term");
          AST *n = ast_new(NODE_BINARY_OP, "+", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 2014 "parser.tab.c"
    break;

  case 60: /* arithExpr: arithExpr MINUS term  */
#line 518 "parser.y"
      {
          log_production("arithExpr -> arithExpr - term");
          AST *n = ast_new(NODE_BINARY_OP, "-", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 2026 "parser.tab.c"
    break;

  case 61: /* term: factor  */
#line 529 "parser.y"
      {
          log_production("term -> factor");
          (yyval.node) = (yyvsp[0].node);
      }
#line 2035 "parser.tab.c"
    break;

  case 62: /* term: term MULT factor  */
#line 534 "parser.y"
      {
          log_production("term -> term * factor");
          AST *n = ast_new(NODE_BINARY_OP, "*", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 2047 "parser.tab.c"
    break;

  case 63: /* term: term DIV factor  */
#line 542 "parser.y"
      {
          log_production("term -> term / factor");
          AST *n = ast_new(NODE_BINARY_OP, "/", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 2059 "parser.tab.c"
    break;

  case 64: /* factor: NOT factor  */
#line 553 "parser.y"
      {
          log_production("factor -> NOT factor");
          AST *n = ast_new(NODE_UNARY_OP, "not", (yylsp[-1]).first_line);
          ast_append_child(n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 2070 "parser.tab.c"
    break;

  case 65: /* factor: ID  */
#line 560 "parser.y"
      {
          log_production("factor -> id");
          (yyval.node) = ast_new(NODE_ID, (yyvsp[0].sVal), (yylsp[0]).first_line);
      }
#line 2079 "parser.tab.c"
    break;

  case 66: /* factor: INT_LIT  */
#line 565 "parser.y"
      {
          log_production("factor -> INT_LIT");
          AST *n = ast_new(NODE_INT_LITERAL, NULL, (yylsp[0]).first_line);
          n->intValue = (yyvsp[0].iVal);
          (yyval.node) = n;
      }
#line 2090 "parser.tab.c"
    break;

  case 67: /* factor: FLOAT_LIT  */
#line 572 "parser.y"
      {
          log_production("factor -> FLOAT_LIT");
          AST *n = ast_new(NODE_FLOAT_LITERAL, NULL, (yylsp[0]).first_line);
          n->floatValue = (yyvsp[0].dVal);
          (yyval.node) = n;
      }
#line 2101 "parser.tab.c"
    break;

  case 68: /* factor: STRING_LIT  */
#line 579 "parser.y"
      {
          log_production("factor -> STRING_LIT");
          (yyval.node) = ast_new(NODE_STRING_LITERAL, (yyvsp[0].sVal), (yylsp[0]).first_line);
      }
#line 2110 "parser.tab.c"
    break;

  case 69: /* factor: LPAREN arithExpr RPAREN  */
#line 584 "parser.y"
      {
          log_production("factor -> ( arithExpr )");
          (yyval.node) = (yyvsp[-1].node);
      }
#line 2119 "parser.tab.c"
    break;

  case 70: /* factor: functionCall  */
#line 589 "parser.y"
      {
          log_production("factor -> functionCall");
          (yyval.node) = (yyvsp[0].node);
      }
#line 2128 "parser.tab.c"
    break;

  case 71: /* functionCall: ID LPAREN aParams RPAREN  */
#line 598 "parser.y"
      {
          log_production("functionCall -> id ( aParams )");
          AST *c = ast_new(NODE_FUNCTION_CALL, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) c->child = (yyvsp[-1].node);
          (yyval.node) = c;
      }
#line 2139 "parser.tab.c"
    break;

  case 72: /* fParams: ID COLON type fParamsTailList  */
#line 609 "parser.y"
      {
          log_production("fParams -> id : type fParamsTailList");
          AST *p = ast_new(NODE_PARAM, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) {
              p->typeName = (yyvsp[-1].node)->name ? strdup((yyvsp[-1].node)->name) : NULL;
              ast_free((yyvsp[-1].node));
          }
          if ((yyvsp[0].node)) ast_append_sibling(&p, (yyvsp[0].node));
          (yyval.node) = p;
      }
#line 2154 "parser.tab.c"
    break;

  case 73: /* fParams: %empty  */
#line 620 "parser.y"
      {
          log_production("fParams -> epsilon");
          (yyval.node) = NULL;
      }
#line 2163 "parser.tab.c"
    break;

  case 74: /* fParamsTailList: COMMA ID COLON type fParamsTailList  */
#line 628 "parser.y"
      {
          log_production("fParamsTailList -> , id : type fParamsTailList");
          AST *p = ast_new(NODE_PARAM, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) {
              p->typeName = (yyvsp[-1].node)->name ? strdup((yyvsp[-1].node)->name) : NULL;
              ast_free((yyvsp[-1].node));
          }
          if ((yyvsp[0].node)) ast_append_sibling(&p, (yyvsp[0].node));
          (yyval.node) = p;
      }
#line 2178 "parser.tab.c"
    break;

  case 75: /* fParamsTailList: %empty  */
#line 639 "parser.y"
      {
          log_production("fParamsTailList -> epsilon");
          (yyval.node) = NULL;
      }
#line 2187 "parser.tab.c"
    break;

  case 76: /* aParams: expr aParamsTailList  */
#line 647 "parser.y"
      {
          log_production("aParams -> expr aParamsTailList");
          AST *h = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&h, (yyvsp[0].node));
          (yyval.node) = h;
      }
#line 2198 "parser.tab.c"
    break;

  case 77: /* aParams: %empty  */
#line 654 "parser.y"
      {
          log_production("aParams -> epsilon");
          (yyval.node) = NULL;
      }
#line 2207 "parser.tab.c"
    break;

  case 78: /* aParamsTailList: COMMA expr aParamsTailList  */
#line 662 "parser.y"
      {
          log_production("aParamsTailList -> , expr aParamsTailList");
          AST *n = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&n, (yyvsp[0].node));
          (yyval.node) = n;
      }
#line 2218 "parser.tab.c"
    break;

  case 79: /* aParamsTailList: %empty  */
#line 669 "parser.y"
      {
          log_production("aParamsTailList -> epsilon");
          (yyval.node) = NULL;
      }
#line 2227 "parser.tab.c"
    break;

  case 80: /* type: INTEGER_T  */
#line 678 "parser.y"
      {
          log_production("type -> INTEGER");
          AST *t = ast_new(NODE_TYPE, "int", (yylsp[0]).first_line);
          (yyval.node) = t;
      }
#line 2237 "parser.tab.c"
    break;

  case 81: /* type: FLOAT_T  */
#line 684 "parser.y"
      {
          log_production("type -> FLOAT");
          AST *t = ast_new(NODE_TYPE, "float", (yylsp[0]).first_line);
          (yyval.node) = t;
      }
#line 2247 "parser.tab.c"
    break;

  case 82: /* type: ID  */
#line 690 "parser.y"
      {
          log_production("type -> id");
          AST *t = ast_new(NODE_TYPE, (yyvsp[0].sVal), (yylsp[0]).first_line);
          (yyval.node) = t;
      }
#line 2257 "parser.tab.c"
    break;

  case 83: /* type: %empty  */
#line 696 "parser.y"
      {
          log_production("type -> epsilon");
          (yyval.node) = NULL;
      }
#line 2266 "parser.tab.c"
    break;

  case 84: /* returnType: type  */
#line 704 "parser.y"
      {
          log_production("returnType -> type");
          (yyval.node) = (yyvsp[0].node);
      }
#line 2275 "parser.tab.c"
    break;

  case 85: /* returnType: VOID  */
#line 709 "parser.y"
      {
          log_production("returnType -> VOID");
          AST *t = ast_new(NODE_TYPE, "void", (yylsp[0]).first_line);
          (yyval.node) = t;
      }
#line 2285 "parser.tab.c"
    break;


#line 2289 "parser.tab.c"

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

#line 716 "parser.y"


static void log_production(const char *rule) {
    if (derivation_file && rule) {
        fprintf(derivation_file, "%s\n", rule);
        fflush(derivation_file);
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", current_line, s);
}
