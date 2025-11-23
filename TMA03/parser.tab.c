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

/* expose AST root */
AST *astRoot = NULL;

#line 87 "parser.tab.c"

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
  YYSYMBOL_exprPrime = 73,                 /* exprPrime  */
  YYSYMBOL_relExpr = 74,                   /* relExpr  */
  YYSYMBOL_arithExpr = 75,                 /* arithExpr  */
  YYSYMBOL_arithExprPrime = 76,            /* arithExprPrime  */
  YYSYMBOL_addOp = 77,                     /* addOp  */
  YYSYMBOL_term = 78,                      /* term  */
  YYSYMBOL_termPrime = 79,                 /* termPrime  */
  YYSYMBOL_multOp = 80,                    /* multOp  */
  YYSYMBOL_factor = 81,                    /* factor  */
  YYSYMBOL_sign = 82,                      /* sign  */
  YYSYMBOL_variable = 83,                  /* variable  */
  YYSYMBOL_idnestChain = 84,               /* idnestChain  */
  YYSYMBOL_idnest = 85,                    /* idnest  */
  YYSYMBOL_idOrSelf = 86,                  /* idOrSelf  */
  YYSYMBOL_indiceChain = 87,               /* indiceChain  */
  YYSYMBOL_indice = 88,                    /* indice  */
  YYSYMBOL_functionCall = 89,              /* functionCall  */
  YYSYMBOL_fParams = 90,                   /* fParams  */
  YYSYMBOL_fParamsTailList = 91,           /* fParamsTailList  */
  YYSYMBOL_aParams = 92,                   /* aParams  */
  YYSYMBOL_aParamsTailList = 93,           /* aParamsTailList  */
  YYSYMBOL_type = 94                       /* type  */
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
#define YYLAST   194

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  200

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
       0,    51,    51,    52,    53,    54,    55,    56,    61,    71,
      79,    83,    89,    94,   100,   106,   111,   120,   121,   126,
     136,   143,   153,   162,   168,   172,   173,   177,   181,   190,
     199,   200,   204,   205,   210,   211,   219,   226,   232,   238,
     244,   248,   258,   259,   263,   264,   265,   270,   286,   298,
     310,   314,   315,   316,   317,   318,   319,   320,   324,   340,
     353,   357,   358,   362,   378,   391,   395,   396,   397,   401,
     402,   403,   404,   405,   406,   407,   408,   412,   413,   417,
     446,   463,   467,   481,   499,   500,   504,   521,   525,   530,
     556,   563,   567,   574,   578,   579,   583,   584,   589,   602
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
  "statBlock", "expr", "exprPrime", "relExpr", "arithExpr",
  "arithExprPrime", "addOp", "term", "termPrime", "multOp", "factor",
  "sign", "variable", "idnestChain", "idnest", "idOrSelf", "indiceChain",
  "indice", "functionCall", "fParams", "fParamsTailList", "aParams",
  "aParamsTailList", "type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-157)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      87,    32,    34,    42,    18,    54,    87,  -157,    87,    23,
      51,    48,    56,    88,  -157,  -157,  -157,    69,  -157,    90,
      66,    38,    88,    79,    74,  -157,    89,    91,    92,    94,
      96,  -157,   115,    85,    69,  -157,  -157,    95,    99,   126,
       6,    -7,   104,   105,    47,    97,    38,   101,   131,  -157,
      37,     6,    37,    37,    37,   110,  -157,  -157,  -157,  -157,
      37,    -5,  -157,    37,    37,   108,   102,  -157,   135,  -157,
     128,   128,   106,  -157,  -157,   111,  -157,   116,  -157,  -157,
    -157,    37,  -157,  -157,    37,   112,    29,    64,    39,    -8,
      37,  -157,  -157,   113,   139,   114,   117,   118,   131,  -157,
      37,  -157,   120,   119,   121,  -157,  -157,   105,   115,    47,
      47,   122,   131,   155,  -157,   127,  -157,   134,    37,    37,
    -157,    37,    37,    37,    37,    37,    37,  -157,  -157,  -157,
      37,  -157,  -157,  -157,  -157,    37,  -157,   136,   102,   137,
     140,    36,   130,   129,    37,  -157,   138,  -157,  -157,  -157,
    -157,  -157,  -157,  -157,   142,  -157,  -157,    29,    29,  -157,
    -157,  -157,  -157,  -157,  -157,    39,    -8,  -157,  -157,    36,
      86,  -157,   143,     8,   146,   130,  -157,   120,  -157,   131,
    -157,  -157,  -157,  -157,   156,    86,   141,  -157,   144,  -157,
    -157,  -157,  -157,   116,    36,  -157,  -157,  -157,  -157,  -157
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,     0,     0,     0,     0,     0,     2,     6,     4,     0,
      10,     0,     0,    91,     1,     3,     5,    24,    19,     0,
       0,    18,    91,     0,     0,    84,     0,     0,     0,     0,
       0,    85,     0,     0,    24,    25,    26,     0,     0,     0,
       0,    87,     0,    12,    15,     0,    18,     0,    99,    21,
       0,     0,     0,     0,     0,     0,    27,    22,    23,    34,
       0,    87,    80,    95,     0,     0,    87,    40,     0,     9,
       0,     0,     0,    16,    17,     0,    98,    93,    72,    73,
      74,     0,    77,    78,     0,     0,    50,    57,    60,    65,
       0,    71,    76,     0,     0,     0,     0,     0,    99,    41,
      95,    79,    97,     0,     0,    82,    86,    12,     0,    15,
      15,     0,    99,     0,    90,     0,    70,     0,     0,     0,
      47,     0,     0,     0,     0,     0,     0,    61,    62,    58,
       0,    66,    67,    68,    63,     0,    69,     0,    87,     0,
       0,    46,    31,     0,     0,    94,     0,    88,    11,    28,
      13,    14,     8,    20,     0,    75,    39,    50,    50,    51,
      52,    53,    54,    55,    56,    60,    65,    37,    38,    46,
      43,    45,     0,     0,     0,    31,    89,    97,    83,    99,
      48,    49,    59,    64,     0,    43,     0,    36,     0,    33,
      29,    30,    96,    93,    46,    42,    44,    32,    92,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -157,    17,  -157,  -157,    71,   -31,  -157,   145,   -10,  -157,
    -157,   147,  -157,  -157,   123,    65,     1,  -157,   -14,  -157,
      -3,  -156,   -46,   -43,    -2,   -59,    19,  -157,    53,    20,
    -157,   -74,  -157,   -17,   -22,  -157,  -157,   -20,  -157,   -15,
     163,    -6,    93,    11,   -97
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,    20,    69,    72,     7,    45,     8,     9,
      18,    33,    34,    35,   109,    56,   174,   175,   171,    37,
     186,   172,   102,   120,    86,    87,   129,   130,    88,   134,
     135,    89,    90,    91,    39,    40,    41,   101,    66,    92,
      24,   114,   103,   145,    77
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      38,   142,    42,    36,    85,   104,    95,    96,    97,    25,
     116,    46,   188,   184,    99,   153,   136,    38,    62,    42,
      36,    65,   115,    15,    63,    16,   100,    31,    64,    94,
      64,   131,   132,   133,    93,    10,    46,    11,   199,    25,
      25,    78,    79,    80,   189,    12,   106,     3,     4,    13,
      26,    27,    28,    29,    14,    30,    17,    31,    31,    70,
      71,   166,   159,   160,   161,   162,   163,   164,    81,   170,
     118,   119,    25,    19,    82,    83,   127,   128,   150,   151,
      84,    21,   193,    26,    27,    28,    29,    22,    30,    25,
      31,    23,    32,    43,     1,     2,     3,     4,   177,    44,
      26,    27,    28,    29,    48,    30,    49,    31,   121,   122,
     123,   124,   125,   126,   180,   181,   157,   158,    55,    57,
      50,    59,    51,    52,    38,    53,    42,    54,    60,    61,
      67,    73,    68,    75,    76,    98,   105,    64,   107,   108,
     111,   112,   138,   113,   117,   137,   139,   144,   152,   140,
     141,   146,    38,    38,    42,    42,   185,   147,   154,   155,
     156,   176,   167,   168,   169,   173,   178,   179,    38,   187,
      42,   185,   190,   149,   194,   196,   191,    38,   148,    42,
     197,    58,   195,   165,   182,    47,   183,   198,   192,     0,
       0,    74,     0,   143,   110
};

static const yytype_int16 yycheck[] =
{
      17,    98,    17,    17,    50,    64,    52,    53,    54,     3,
      84,    21,     4,   169,    60,   112,    90,    34,    40,    34,
      34,    41,    81,     6,    31,     8,    31,    21,    35,    51,
      35,    39,    40,    41,    51,     3,    46,     3,   194,     3,
       3,     4,     5,     6,    36,     3,    66,     9,    10,    31,
      14,    15,    16,    17,     0,    19,    33,    21,    21,    12,
      13,   135,   121,   122,   123,   124,   125,   126,    31,    33,
      41,    42,     3,    22,    37,    38,    37,    38,   109,   110,
      43,    33,   179,    14,    15,    16,    17,    31,    19,     3,
      21,     3,    23,     3,     7,     8,     9,    10,   144,    33,
      14,    15,    16,    17,    25,    19,    32,    21,    44,    45,
      46,    47,    48,    49,   157,   158,   118,   119,     3,    34,
      31,    26,    31,    31,   141,    31,   141,    31,    29,     3,
      26,    34,    27,    32,     3,    25,    28,    35,     3,    11,
      34,    30,     3,    27,    32,    32,    32,    27,    26,    32,
      32,    32,   169,   170,   169,   170,   170,    36,     3,    32,
      26,    32,    26,    26,    24,    35,    28,    25,   185,    26,
     185,   185,    26,   108,    18,    34,   175,   194,   107,   194,
      36,    34,   185,   130,   165,    22,   166,   193,   177,    -1,
      -1,    46,    -1,   100,    71
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,     9,    10,    51,    52,    56,    58,    59,
       3,     3,     3,    31,     0,    51,    51,    33,    60,    22,
      53,    33,    31,     3,    90,     3,    14,    15,    16,    17,
      19,    21,    23,    61,    62,    63,    68,    69,    83,    84,
      85,    86,    89,     3,    33,    57,    58,    90,    25,    32,
      31,    31,    31,    31,    31,     3,    65,    34,    61,    26,
      29,     3,    84,    31,    35,    87,    88,    26,    27,    54,
      12,    13,    55,    34,    57,    32,     3,    94,     4,     5,
       6,    31,    37,    38,    43,    72,    74,    75,    78,    81,
      82,    83,    89,    83,    84,    72,    72,    72,    25,    72,
      31,    87,    72,    92,    75,    28,    87,     3,    11,    64,
      64,    34,    30,    27,    91,    75,    81,    32,    41,    42,
      73,    44,    45,    46,    47,    48,    49,    37,    38,    76,
      77,    39,    40,    41,    79,    80,    81,    32,     3,    32,
      32,    32,    94,    92,    27,    93,    32,    36,    54,    65,
      55,    55,    26,    94,     3,    32,    26,    74,    74,    75,
      75,    75,    75,    75,    75,    78,    81,    26,    26,    24,
      33,    68,    71,    35,    66,    67,    32,    72,    28,    25,
      73,    73,    76,    79,    71,    68,    70,    26,     4,    36,
      26,    66,    93,    94,    18,    70,    34,    36,    91,    71
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    51,    51,    51,    51,    51,    52,    53,
      53,    54,    54,    55,    55,    55,    56,    57,    57,    58,
      59,    59,    60,    61,    61,    62,    62,    63,    64,    65,
      66,    66,    67,    67,    68,    68,    68,    68,    68,    68,
      68,    69,    70,    70,    71,    71,    71,    72,    73,    73,
      73,    74,    74,    74,    74,    74,    74,    74,    75,    76,
      76,    77,    77,    78,    79,    79,    80,    80,    80,    81,
      81,    81,    81,    81,    81,    81,    81,    82,    82,    83,
      84,    84,    85,    85,    86,    86,    87,    87,    88,    89,
      90,    90,    91,    91,    92,    92,    93,    93,    94,    94
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     0,     7,     3,
       0,     3,     0,     3,     3,     0,     5,     2,     0,     2,
       7,     4,     3,     2,     0,     1,     1,     2,     2,     5,
       2,     0,     3,     2,     2,     8,     6,     5,     5,     5,
       2,     3,     2,     0,     3,     1,     0,     2,     3,     3,
       0,     3,     3,     3,     3,     3,     3,     1,     2,     3,
       0,     1,     1,     2,     3,     0,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       2,     0,     3,     5,     1,     1,     2,     0,     3,     5,
       4,     0,     5,     0,     2,     0,     3,     0,     1,     0
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
#line 51 "parser.y"
                                   { astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line); ast_append_child(astRoot, (yyvsp[0].node)); }
#line 1419 "parser.tab.c"
    break;

  case 3: /* prog: classDecl prog  */
#line 52 "parser.y"
                                    { if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[-1]).first_line); ast_append_child(astRoot, (yyvsp[-1].node)); }
#line 1425 "parser.tab.c"
    break;

  case 4: /* prog: funcDef  */
#line 53 "parser.y"
                                    { astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line); ast_append_child(astRoot, (yyvsp[0].node)); }
#line 1431 "parser.tab.c"
    break;

  case 5: /* prog: funcDef prog  */
#line 54 "parser.y"
                                    { if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[-1]).first_line); ast_append_child(astRoot, (yyvsp[-1].node)); }
#line 1437 "parser.tab.c"
    break;

  case 6: /* prog: implDef  */
#line 55 "parser.y"
                                    { if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, (yylsp[0]).first_line); ast_append_child(astRoot, (yyvsp[0].node)); }
#line 1443 "parser.tab.c"
    break;

  case 7: /* prog: %empty  */
#line 56 "parser.y"
                                    { astRoot = ast_new(NODE_PROGRAM, NULL, 0); }
#line 1449 "parser.tab.c"
    break;

  case 8: /* classDecl: CLASS ID classInherit LBRACE classBody RBRACE SEMICOLON  */
#line 62 "parser.y"
      {
          AST *c = ast_new(NODE_CLASS_DECL, (yyvsp[-5].sVal), (yylsp[-5]).first_line);
          if ((yyvsp[-4].node)) ast_append_child(c, (yyvsp[-4].node));
          if ((yyvsp[-2].node)) ast_append_child(c, (yyvsp[-2].node));
          (yyval.node) = c;
      }
#line 1460 "parser.tab.c"
    break;

  case 9: /* classInherit: ISA ID moreIds  */
#line 72 "parser.y"
      {
          AST *list = ast_new(NODE_CLASS_INHERIT_LIST, NULL, (yylsp[-1]).first_line);
          AST *idnode = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          ast_append_child(list, idnode);
          if ((yyvsp[0].node)) ast_append_child(list, (yyvsp[0].node));
          (yyval.node) = list;
      }
#line 1472 "parser.tab.c"
    break;

  case 10: /* classInherit: %empty  */
#line 79 "parser.y"
                  { (yyval.node) = NULL; }
#line 1478 "parser.tab.c"
    break;

  case 11: /* moreIds: COMMA ID moreIds  */
#line 84 "parser.y"
      {
          AST *idn = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          if ((yyvsp[0].node)) ast_append_child(idn, (yyvsp[0].node));
          (yyval.node) = idn;
      }
#line 1488 "parser.tab.c"
    break;

  case 12: /* moreIds: %empty  */
#line 89 "parser.y"
                  { (yyval.node) = NULL; }
#line 1494 "parser.tab.c"
    break;

  case 13: /* classBody: PUBLIC attributeDecl classBody  */
#line 95 "parser.y"
      {
          if ((yyvsp[-1].node) && (yyvsp[-1].node)->child) (yyvsp[-1].node)->child->typeName = strdup("PUBLIC");
          if ((yyvsp[0].node)) ast_append_sibling(&(yyvsp[-1].node), (yyvsp[0].node));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1504 "parser.tab.c"
    break;

  case 14: /* classBody: PRIVATE attributeDecl classBody  */
#line 101 "parser.y"
      {
          if ((yyvsp[-1].node) && (yyvsp[-1].node)->child) (yyvsp[-1].node)->child->typeName = strdup("PRIVATE");
          if ((yyvsp[0].node)) ast_append_sibling(&(yyvsp[-1].node), (yyvsp[0].node));
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1514 "parser.tab.c"
    break;

  case 15: /* classBody: %empty  */
#line 106 "parser.y"
                  { (yyval.node) = NULL; }
#line 1520 "parser.tab.c"
    break;

  case 16: /* implDef: IMPLEMENT ID LBRACE implFuncs RBRACE  */
#line 112 "parser.y"
      {
          AST *n = ast_new(NODE_EMPTY, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) n->child = (yyvsp[-1].node);
          (yyval.node) = n;
      }
#line 1530 "parser.tab.c"
    break;

  case 17: /* implFuncs: funcDef implFuncs  */
#line 120 "parser.y"
                        { (yyval.node) = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&(yyval.node), (yyvsp[0].node)); }
#line 1536 "parser.tab.c"
    break;

  case 18: /* implFuncs: %empty  */
#line 121 "parser.y"
                  { (yyval.node) = NULL; }
#line 1542 "parser.tab.c"
    break;

  case 19: /* funcDef: funcHead funcBody  */
#line 127 "parser.y"
      {
          AST *f = (yyvsp[-1].node);
          if ((yyvsp[0].node)) f->extra = (yyvsp[0].node);
          (yyval.node) = f;
      }
#line 1552 "parser.tab.c"
    break;

  case 20: /* funcHead: FUNC ID LPAREN fParams RPAREN ARROW type  */
#line 137 "parser.y"
      {
          AST *fn = ast_new(NODE_FUNC_DECL, (yyvsp[-5].sVal), (yylsp[-5]).first_line);
          fn->typeName = (yyvsp[0].node) ? strdup((yyvsp[0].node)->name) : NULL;
          if ((yyvsp[-3].node)) fn->child = (yyvsp[-3].node);
          (yyval.node) = fn;
      }
#line 1563 "parser.tab.c"
    break;

  case 21: /* funcHead: CONSTRUCT LPAREN fParams RPAREN  */
#line 144 "parser.y"
      {
          AST *fn = ast_new(NODE_FUNC_DECL, "constructor", (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) fn->child = (yyvsp[-1].node);
          (yyval.node) = fn;
      }
#line 1573 "parser.tab.c"
    break;

  case 22: /* funcBody: LBRACE varDeclOrStmtList RBRACE  */
#line 154 "parser.y"
      {
          AST *b = ast_new(NODE_FUNC_BODY, NULL, (yylsp[-2]).first_line);
          if ((yyvsp[-1].node)) b->child = (yyvsp[-1].node);
          (yyval.node) = b;
      }
#line 1583 "parser.tab.c"
    break;

  case 23: /* varDeclOrStmtList: varDeclOrStmt varDeclOrStmtList  */
#line 163 "parser.y"
      {
          AST *head = (yyvsp[-1].node);
          if ((yyvsp[0].node)) ast_append_sibling(&head, (yyvsp[0].node));
          (yyval.node) = head;
      }
#line 1593 "parser.tab.c"
    break;

  case 24: /* varDeclOrStmtList: %empty  */
#line 168 "parser.y"
                  { (yyval.node) = NULL; }
#line 1599 "parser.tab.c"
    break;

  case 25: /* varDeclOrStmt: localVarDecl  */
#line 172 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1605 "parser.tab.c"
    break;

  case 26: /* varDeclOrStmt: statement  */
#line 173 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1611 "parser.tab.c"
    break;

  case 27: /* localVarDecl: LOCAL varDecl  */
#line 177 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1617 "parser.tab.c"
    break;

  case 28: /* attributeDecl: ATTRIBUTE varDecl  */
#line 182 "parser.y"
      {
          AST *attr = ast_new(NODE_ATTRIBUTE, NULL, (yylsp[-1]).first_line);
          if ((yyvsp[0].node)) ast_append_child(attr, (yyvsp[0].node));
          (yyval.node) = attr;
      }
#line 1627 "parser.tab.c"
    break;

  case 29: /* varDecl: ID COLON type arraySizes SEMICOLON  */
#line 191 "parser.y"
      {
          AST *v = ast_new(NODE_VAR_DECL, (yyvsp[-4].sVal), (yylsp[-4]).first_line);
          if ((yyvsp[-2].node)) v->typeName = (yyvsp[-2].node)->name ? strdup((yyvsp[-2].node)->name) : NULL;
          (yyval.node) = v;
      }
#line 1637 "parser.tab.c"
    break;

  case 30: /* arraySizes: arraySize arraySizes  */
#line 199 "parser.y"
                           { (yyval.node) = NULL; }
#line 1643 "parser.tab.c"
    break;

  case 31: /* arraySizes: %empty  */
#line 200 "parser.y"
                  { (yyval.node) = NULL; }
#line 1649 "parser.tab.c"
    break;

  case 32: /* arraySize: LBRACKET INT_LIT RBRACKET  */
#line 204 "parser.y"
                                { (yyval.node) = ast_new_int((yyvsp[-1].iVal), (yylsp[-1]).first_line); }
#line 1655 "parser.tab.c"
    break;

  case 33: /* arraySize: LBRACKET RBRACKET  */
#line 205 "parser.y"
                        { (yyval.node) = NULL; }
#line 1661 "parser.tab.c"
    break;

  case 34: /* statement: assignStat SEMICOLON  */
#line 210 "parser.y"
                           { (yyval.node) = (yyvsp[-1].node); }
#line 1667 "parser.tab.c"
    break;

  case 35: /* statement: IF LPAREN expr RPAREN THEN statBlock ELSE statBlock  */
#line 212 "parser.y"
      {
          AST *node = ast_new(NODE_IF, NULL, (yylsp[-7]).first_line);
          ast_append_child(node, (yyvsp[-5].node));
          ast_append_child(node, (yyvsp[-2].node));
          ast_append_child(node, (yyvsp[0].node));
          (yyval.node) = node;
      }
#line 1679 "parser.tab.c"
    break;

  case 36: /* statement: WHILE LPAREN expr RPAREN statBlock SEMICOLON  */
#line 220 "parser.y"
      {
          AST *node = ast_new(NODE_WHILE, NULL, (yylsp[-5]).first_line);
          ast_append_child(node, (yyvsp[-3].node));
          ast_append_child(node, (yyvsp[-1].node));
          (yyval.node) = node;
      }
#line 1690 "parser.tab.c"
    break;

  case 37: /* statement: READ LPAREN variable RPAREN SEMICOLON  */
#line 227 "parser.y"
      {
          AST *n = ast_new(NODE_READ, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1700 "parser.tab.c"
    break;

  case 38: /* statement: WRITE LPAREN expr RPAREN SEMICOLON  */
#line 233 "parser.y"
      {
          AST *n = ast_new(NODE_WRITE, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1710 "parser.tab.c"
    break;

  case 39: /* statement: RETURN LPAREN expr RPAREN SEMICOLON  */
#line 239 "parser.y"
      {
          AST *n = ast_new(NODE_RETURN, NULL, (yylsp[-4]).first_line);
          ast_append_child(n, (yyvsp[-2].node));
          (yyval.node) = n;
      }
#line 1720 "parser.tab.c"
    break;

  case 40: /* statement: functionCall SEMICOLON  */
#line 244 "parser.y"
                             { (yyval.node) = (yyvsp[-1].node); }
#line 1726 "parser.tab.c"
    break;

  case 41: /* assignStat: variable ASSIGN expr  */
#line 249 "parser.y"
      {
          AST *assign = ast_new(NODE_ASSIGN, NULL, (yylsp[-2]).first_line);
          ast_append_child(assign, (yyvsp[-2].node));
          ast_append_child(assign, (yyvsp[0].node));
          (yyval.node) = assign;
      }
#line 1737 "parser.tab.c"
    break;

  case 42: /* statementList: statement statementList  */
#line 258 "parser.y"
                              { AST *h = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&h, (yyvsp[0].node)); (yyval.node) = h; }
#line 1743 "parser.tab.c"
    break;

  case 43: /* statementList: %empty  */
#line 259 "parser.y"
                  { (yyval.node) = NULL; }
#line 1749 "parser.tab.c"
    break;

  case 44: /* statBlock: LBRACE statementList RBRACE  */
#line 263 "parser.y"
                                  { (yyval.node) = (yyvsp[-1].node); }
#line 1755 "parser.tab.c"
    break;

  case 45: /* statBlock: statement  */
#line 264 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1761 "parser.tab.c"
    break;

  case 46: /* statBlock: %empty  */
#line 265 "parser.y"
                  { (yyval.node) = NULL; }
#line 1767 "parser.tab.c"
    break;

  case 47: /* expr: relExpr exprPrime  */
#line 271 "parser.y"
      {
          if ((yyvsp[0].node)) {
              AST *op = (yyvsp[0].node);
              AST *left = (yyvsp[-1].node);
              AST *right = op->child ? op->child->sibling : NULL;
              op->child = left;
              if (right) op->child->sibling = right;
              (yyval.node) = op;
          } else {
              (yyval.node) = (yyvsp[-1].node);
          }
      }
#line 1784 "parser.tab.c"
    break;

  case 48: /* exprPrime: AND relExpr exprPrime  */
#line 287 "parser.y"
      {
          AST *op = ast_new(NODE_BINARY_OP, "and", (yylsp[-2]).first_line);
          op->child = (yyvsp[-1].node);
          if ((yyvsp[0].node)) {
              AST *chain = (yyvsp[0].node);
              chain->child->sibling = op;
              (yyval.node) = chain;
          } else {
              (yyval.node) = op;
          }
      }
#line 1800 "parser.tab.c"
    break;

  case 49: /* exprPrime: OR relExpr exprPrime  */
#line 299 "parser.y"
      {
          AST *op = ast_new(NODE_BINARY_OP, "or", (yylsp[-2]).first_line);
          op->child = (yyvsp[-1].node);
          if ((yyvsp[0].node)) {
              AST *chain = (yyvsp[0].node);
              chain->child->sibling = op;
              (yyval.node) = chain;
          } else {
              (yyval.node) = op;
          }
      }
#line 1816 "parser.tab.c"
    break;

  case 50: /* exprPrime: %empty  */
#line 310 "parser.y"
                  { (yyval.node) = NULL; }
#line 1822 "parser.tab.c"
    break;

  case 51: /* relExpr: arithExpr EQ arithExpr  */
#line 314 "parser.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "==", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1828 "parser.tab.c"
    break;

  case 52: /* relExpr: arithExpr NE arithExpr  */
#line 315 "parser.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "<>", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1834 "parser.tab.c"
    break;

  case 53: /* relExpr: arithExpr LT arithExpr  */
#line 316 "parser.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "<", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1840 "parser.tab.c"
    break;

  case 54: /* relExpr: arithExpr GT arithExpr  */
#line 317 "parser.y"
                             { AST *n = ast_new(NODE_BINARY_OP, ">", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1846 "parser.tab.c"
    break;

  case 55: /* relExpr: arithExpr LE arithExpr  */
#line 318 "parser.y"
                             { AST *n = ast_new(NODE_BINARY_OP, "<=", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1852 "parser.tab.c"
    break;

  case 56: /* relExpr: arithExpr GE arithExpr  */
#line 319 "parser.y"
                             { AST *n = ast_new(NODE_BINARY_OP, ">=", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[-2].node)); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1858 "parser.tab.c"
    break;

  case 57: /* relExpr: arithExpr  */
#line 320 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1864 "parser.tab.c"
    break;

  case 58: /* arithExpr: term arithExprPrime  */
#line 325 "parser.y"
      {
          if ((yyvsp[0].node)) {
              AST *op = (yyvsp[0].node);
              AST *left = (yyvsp[-1].node);
              AST *right = op->child;
              op->child = left;
              if (right) op->child->sibling = right;
              (yyval.node) = op;
          } else {
              (yyval.node) = (yyvsp[-1].node);
          }
      }
#line 1881 "parser.tab.c"
    break;

  case 59: /* arithExprPrime: addOp term arithExprPrime  */
#line 341 "parser.y"
      {
          AST *op = ast_new(NODE_BINARY_OP, (yyvsp[-2].node), (yylsp[-2]).first_line);
          op->child = (yyvsp[-1].node);
          if ((yyvsp[0].node)) {
              AST *chain = (yyvsp[0].node);
              chain->child->sibling = op;
              (yyval.node) = chain;
          } else {
              (yyval.node) = op;
          }
          free((yyvsp[-2].node));
      }
#line 1898 "parser.tab.c"
    break;

  case 60: /* arithExprPrime: %empty  */
#line 353 "parser.y"
                  { (yyval.node) = NULL; }
#line 1904 "parser.tab.c"
    break;

  case 61: /* addOp: PLUS  */
#line 357 "parser.y"
           { (yyval.node) = strdup("+"); }
#line 1910 "parser.tab.c"
    break;

  case 62: /* addOp: MINUS  */
#line 358 "parser.y"
            { (yyval.node) = strdup("-"); }
#line 1916 "parser.tab.c"
    break;

  case 63: /* term: factor termPrime  */
#line 363 "parser.y"
      {
          if ((yyvsp[0].node)) {
              AST *op = (yyvsp[0].node);
              AST *left = (yyvsp[-1].node);
              AST *right = op->child;
              op->child = left;
              if (right) op->child->sibling = right;
              (yyval.node) = op;
          } else {
              (yyval.node) = (yyvsp[-1].node);
          }
      }
#line 1933 "parser.tab.c"
    break;

  case 64: /* termPrime: multOp factor termPrime  */
#line 379 "parser.y"
      {
          AST *op = ast_new(NODE_BINARY_OP, (yyvsp[-2].node), (yylsp[-2]).first_line);
          op->child = (yyvsp[-1].node);
          if ((yyvsp[0].node)) {
              AST *chain = (yyvsp[0].node);
              chain->child->sibling = op;
              (yyval.node) = chain;
          } else {
              (yyval.node) = op;
          }
          free((yyvsp[-2].node));
      }
#line 1950 "parser.tab.c"
    break;

  case 65: /* termPrime: %empty  */
#line 391 "parser.y"
                  { (yyval.node) = NULL; }
#line 1956 "parser.tab.c"
    break;

  case 66: /* multOp: MULT  */
#line 395 "parser.y"
           { (yyval.node) = strdup("*"); }
#line 1962 "parser.tab.c"
    break;

  case 67: /* multOp: DIV  */
#line 396 "parser.y"
          { (yyval.node) = strdup("/"); }
#line 1968 "parser.tab.c"
    break;

  case 68: /* multOp: AND  */
#line 397 "parser.y"
          { (yyval.node) = strdup("and"); }
#line 1974 "parser.tab.c"
    break;

  case 69: /* factor: sign factor  */
#line 401 "parser.y"
                  { AST *n = ast_new(NODE_UNARY_OP, (yyvsp[-1].node), (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; free((yyvsp[-1].node)); }
#line 1980 "parser.tab.c"
    break;

  case 70: /* factor: NOT factor  */
#line 402 "parser.y"
                 { AST *n = ast_new(NODE_UNARY_OP, "not", (yylsp[-1]).first_line); ast_append_child(n, (yyvsp[0].node)); (yyval.node) = n; }
#line 1986 "parser.tab.c"
    break;

  case 71: /* factor: variable  */
#line 403 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1992 "parser.tab.c"
    break;

  case 72: /* factor: INT_LIT  */
#line 404 "parser.y"
                { (yyval.node) = ast_new(NODE_INT_LITERAL, NULL, (yylsp[0]).first_line); (yyval.node)->intValue = (yyvsp[0].iVal); }
#line 1998 "parser.tab.c"
    break;

  case 73: /* factor: FLOAT_LIT  */
#line 405 "parser.y"
                { (yyval.node) = ast_new(NODE_FLOAT_LITERAL, NULL, (yylsp[0]).first_line); (yyval.node)->floatValue = (yyvsp[0].dVal); }
#line 2004 "parser.tab.c"
    break;

  case 74: /* factor: STRING_LIT  */
#line 406 "parser.y"
                 { (yyval.node) = ast_new(NODE_STRING_LITERAL, (yyvsp[0].sVal), (yylsp[0]).first_line); }
#line 2010 "parser.tab.c"
    break;

  case 75: /* factor: LPAREN arithExpr RPAREN  */
#line 407 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 2016 "parser.tab.c"
    break;

  case 76: /* factor: functionCall  */
#line 408 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2022 "parser.tab.c"
    break;

  case 77: /* sign: PLUS  */
#line 412 "parser.y"
           { (yyval.node) = strdup("+"); }
#line 2028 "parser.tab.c"
    break;

  case 78: /* sign: MINUS  */
#line 413 "parser.y"
            { (yyval.node) = strdup("-"); }
#line 2034 "parser.tab.c"
    break;

  case 79: /* variable: idnestChain ID indiceChain  */
#line 418 "parser.y"
      {
          AST *var = ast_new(NODE_ID, (yyvsp[-1].sVal), (yylsp[-1]).first_line);
          if ((yyvsp[-2].node)) {
              AST *nested = (yyvsp[-2].node);
              if (nested->child) {
                  AST *last = nested->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = var;
              } else {
                  nested->child = var;
              }
              var = nested;
          }
          if ((yyvsp[0].node)) {
              AST *indices = (yyvsp[0].node);
              if (var->child) {
                  AST *last = var->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = indices;
              } else {
                  var->child = indices;
              }
          }
          (yyval.node) = var;
      }
#line 2064 "parser.tab.c"
    break;

  case 80: /* idnestChain: idnest idnestChain  */
#line 447 "parser.y"
      {
          if ((yyvsp[0].node)) {
              if ((yyvsp[0].node)->child) {
                  AST *last = (yyvsp[0].node)->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = (yyvsp[-1].node);
              } else {
                  (yyvsp[0].node)->child = (yyvsp[-1].node);
              }
              (yyval.node) = (yyvsp[0].node);
          } else {
              AST *chain = ast_new(NODE_ID, NULL, (yylsp[-1]).first_line);
              chain->child = (yyvsp[-1].node);
              (yyval.node) = chain;
          }
      }
#line 2085 "parser.tab.c"
    break;

  case 81: /* idnestChain: %empty  */
#line 463 "parser.y"
                  { (yyval.node) = NULL; }
#line 2091 "parser.tab.c"
    break;

  case 82: /* idnest: idOrSelf indiceChain DOT  */
#line 468 "parser.y"
      {
          AST *access = (yyvsp[-2].node);
          if ((yyvsp[-1].node)) {
              if (access->child) {
                  AST *last = access->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = (yyvsp[-1].node);
              } else {
                  access->child = (yyvsp[-1].node);
              }
          }
          (yyval.node) = access;
      }
#line 2109 "parser.tab.c"
    break;

  case 83: /* idnest: idOrSelf LPAREN aParams RPAREN DOT  */
#line 482 "parser.y"
      {
          AST *call = ast_new(NODE_FUNCTION_CALL, (yyvsp[-4].node)->name, (yylsp[-4]).first_line);
          call->child = (yyvsp[-4].node);
          if ((yyvsp[-2].node)) {
              if (call->child->sibling) {
                  AST *last = call->child->sibling;
                  while (last->sibling) last = last->sibling;
                  last->sibling = (yyvsp[-2].node);
              } else {
                  call->child->sibling = (yyvsp[-2].node);
              }
          }
          (yyval.node) = call;
      }
#line 2128 "parser.tab.c"
    break;

  case 84: /* idOrSelf: ID  */
#line 499 "parser.y"
         { (yyval.node) = ast_new(NODE_ID, (yyvsp[0].sVal), (yylsp[0]).first_line); }
#line 2134 "parser.tab.c"
    break;

  case 85: /* idOrSelf: SELF  */
#line 500 "parser.y"
           { (yyval.node) = ast_new(NODE_ID, "self", (yylsp[0]).first_line); }
#line 2140 "parser.tab.c"
    break;

  case 86: /* indiceChain: indice indiceChain  */
#line 505 "parser.y"
      {
          if ((yyvsp[0].node)) {
              AST *list = ast_new(NODE_ID, NULL, (yylsp[-1]).first_line);
              list->child = (yyvsp[-1].node);
              if ((yyvsp[0].node)->child) {
                  AST *last = list->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = (yyvsp[0].node)->child;
              }
              (yyval.node) = list;
          } else {
              AST *list = ast_new(NODE_ID, NULL, (yylsp[-1]).first_line);
              list->child = (yyvsp[-1].node);
              (yyval.node) = list;
          }
      }
#line 2161 "parser.tab.c"
    break;

  case 87: /* indiceChain: %empty  */
#line 521 "parser.y"
                  { (yyval.node) = NULL; }
#line 2167 "parser.tab.c"
    break;

  case 88: /* indice: LBRACKET arithExpr RBRACKET  */
#line 525 "parser.y"
                                  { (yyval.node) = (yyvsp[-1].node); }
#line 2173 "parser.tab.c"
    break;

  case 89: /* functionCall: idnestChain ID LPAREN aParams RPAREN  */
#line 531 "parser.y"
      {
          AST *c = ast_new(NODE_FUNCTION_CALL, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-4].node)) {
              AST *obj = (yyvsp[-4].node)->child;
              if (obj) {
                  c->child = obj;
                  if ((yyvsp[-1].node)) {
                      if (c->child->sibling) {
                          AST *last = c->child->sibling;
                          while (last->sibling) last = last->sibling;
                          last->sibling = (yyvsp[-1].node);
                      } else {
                          c->child->sibling = (yyvsp[-1].node);
                      }
                  }
              }
          } else {
              if ((yyvsp[-1].node)) c->child = (yyvsp[-1].node);
          }
          (yyval.node) = c;
      }
#line 2199 "parser.tab.c"
    break;

  case 90: /* fParams: ID COLON type fParamsTailList  */
#line 557 "parser.y"
      {
          AST *p = ast_new(NODE_PARAM, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) p->typeName = (yyvsp[-1].node)->name ? strdup((yyvsp[-1].node)->name) : NULL;
          if ((yyvsp[0].node)) ast_append_sibling(&p, (yyvsp[0].node));
          (yyval.node) = p;
      }
#line 2210 "parser.tab.c"
    break;

  case 91: /* fParams: %empty  */
#line 563 "parser.y"
                  { (yyval.node) = NULL; }
#line 2216 "parser.tab.c"
    break;

  case 92: /* fParamsTailList: COMMA ID COLON type fParamsTailList  */
#line 568 "parser.y"
      {
          AST *p = ast_new(NODE_PARAM, (yyvsp[-3].sVal), (yylsp[-3]).first_line);
          if ((yyvsp[-1].node)) p->typeName = (yyvsp[-1].node)->name ? strdup((yyvsp[-1].node)->name) : NULL;
          if ((yyvsp[0].node)) ast_append_sibling(&p, (yyvsp[0].node));
          (yyval.node) = p;
      }
#line 2227 "parser.tab.c"
    break;

  case 93: /* fParamsTailList: %empty  */
#line 574 "parser.y"
                  { (yyval.node) = NULL; }
#line 2233 "parser.tab.c"
    break;

  case 94: /* aParams: expr aParamsTailList  */
#line 578 "parser.y"
                           { AST *h = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&h, (yyvsp[0].node)); (yyval.node) = h; }
#line 2239 "parser.tab.c"
    break;

  case 95: /* aParams: %empty  */
#line 579 "parser.y"
                  { (yyval.node) = NULL; }
#line 2245 "parser.tab.c"
    break;

  case 96: /* aParamsTailList: COMMA expr aParamsTailList  */
#line 583 "parser.y"
                                 { AST *n = (yyvsp[-1].node); if ((yyvsp[0].node)) ast_append_sibling(&n, (yyvsp[0].node)); (yyval.node) = n; }
#line 2251 "parser.tab.c"
    break;

  case 97: /* aParamsTailList: %empty  */
#line 584 "parser.y"
                  { (yyval.node) = NULL; }
#line 2257 "parser.tab.c"
    break;

  case 98: /* type: ID  */
#line 590 "parser.y"
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
#line 2274 "parser.tab.c"
    break;

  case 99: /* type: %empty  */
#line 602 "parser.y"
                  { (yyval.node) = NULL; }
#line 2280 "parser.tab.c"
    break;


#line 2284 "parser.tab.c"

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

#line 605 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", current_line, s);
}
