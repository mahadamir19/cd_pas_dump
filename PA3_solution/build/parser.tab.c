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
#line 1 "lib/parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);

ASTNode* root = NULL;
int syntax_errors = 0;

#define NEW_NODE(type, data, left, right) create_node(type, data, yylineno, left, right)

#line 88 "build/parser.tab.c"

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
  YYSYMBOL_T_ID = 3,                       /* T_ID  */
  YYSYMBOL_T_INT_LIT = 4,                  /* T_INT_LIT  */
  YYSYMBOL_T_FLOAT_LIT = 5,                /* T_FLOAT_LIT  */
  YYSYMBOL_T_BOOL_LIT = 6,                 /* T_BOOL_LIT  */
  YYSYMBOL_T_INT = 7,                      /* T_INT  */
  YYSYMBOL_T_FLOAT = 8,                    /* T_FLOAT  */
  YYSYMBOL_T_BOOL = 9,                     /* T_BOOL  */
  YYSYMBOL_T_VOID = 10,                    /* T_VOID  */
  YYSYMBOL_T_IF = 11,                      /* T_IF  */
  YYSYMBOL_T_ELSE = 12,                    /* T_ELSE  */
  YYSYMBOL_T_WHILE = 13,                   /* T_WHILE  */
  YYSYMBOL_T_FOR = 14,                     /* T_FOR  */
  YYSYMBOL_T_RETURN = 15,                  /* T_RETURN  */
  YYSYMBOL_T_MAIN = 16,                    /* T_MAIN  */
  YYSYMBOL_T_CIN = 17,                     /* T_CIN  */
  YYSYMBOL_T_COUT = 18,                    /* T_COUT  */
  YYSYMBOL_T_PLUS = 19,                    /* T_PLUS  */
  YYSYMBOL_T_MINUS = 20,                   /* T_MINUS  */
  YYSYMBOL_T_MULT = 21,                    /* T_MULT  */
  YYSYMBOL_T_DIV = 22,                     /* T_DIV  */
  YYSYMBOL_T_MOD = 23,                     /* T_MOD  */
  YYSYMBOL_T_ASSIGN = 24,                  /* T_ASSIGN  */
  YYSYMBOL_T_EQ = 25,                      /* T_EQ  */
  YYSYMBOL_T_NEQ = 26,                     /* T_NEQ  */
  YYSYMBOL_T_LT = 27,                      /* T_LT  */
  YYSYMBOL_T_GT = 28,                      /* T_GT  */
  YYSYMBOL_T_LE = 29,                      /* T_LE  */
  YYSYMBOL_T_GE = 30,                      /* T_GE  */
  YYSYMBOL_T_AND = 31,                     /* T_AND  */
  YYSYMBOL_T_OR = 32,                      /* T_OR  */
  YYSYMBOL_T_NOT = 33,                     /* T_NOT  */
  YYSYMBOL_T_INCREMENT = 34,               /* T_INCREMENT  */
  YYSYMBOL_T_DECREMENT = 35,               /* T_DECREMENT  */
  YYSYMBOL_T_STREAM_IN = 36,               /* T_STREAM_IN  */
  YYSYMBOL_T_STREAM_OUT = 37,              /* T_STREAM_OUT  */
  YYSYMBOL_T_LPAREN = 38,                  /* T_LPAREN  */
  YYSYMBOL_T_RPAREN = 39,                  /* T_RPAREN  */
  YYSYMBOL_T_LBRACE = 40,                  /* T_LBRACE  */
  YYSYMBOL_T_RBRACE = 41,                  /* T_RBRACE  */
  YYSYMBOL_T_LBRACKET = 42,                /* T_LBRACKET  */
  YYSYMBOL_T_RBRACKET = 43,                /* T_RBRACKET  */
  YYSYMBOL_T_SEMI = 44,                    /* T_SEMI  */
  YYSYMBOL_T_COMMA = 45,                   /* T_COMMA  */
  YYSYMBOL_T_ERROR = 46,                   /* T_ERROR  */
  YYSYMBOL_LOWER_THAN_ELSE = 47,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_program = 49,                   /* program  */
  YYSYMBOL_declaration_list = 50,          /* declaration_list  */
  YYSYMBOL_declaration = 51,               /* declaration  */
  YYSYMBOL_var_declaration = 52,           /* var_declaration  */
  YYSYMBOL_type = 53,                      /* type  */
  YYSYMBOL_func_declaration = 54,          /* func_declaration  */
  YYSYMBOL_param_list = 55,                /* param_list  */
  YYSYMBOL_param = 56,                     /* param  */
  YYSYMBOL_block = 57,                     /* block  */
  YYSYMBOL_statement_list = 58,            /* statement_list  */
  YYSYMBOL_statement = 59,                 /* statement  */
  YYSYMBOL_assignment_stmt = 60,           /* assignment_stmt  */
  YYSYMBOL_if_stmt = 61,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 62,                /* while_stmt  */
  YYSYMBOL_for_stmt = 63,                  /* for_stmt  */
  YYSYMBOL_io_stmt = 64,                   /* io_stmt  */
  YYSYMBOL_return_stmt = 65,               /* return_stmt  */
  YYSYMBOL_expression = 66,                /* expression  */
  YYSYMBOL_arg_list = 67                   /* arg_list  */
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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   543

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


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
      45,    46,    47
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    61,    61,    65,    66,    70,    71,    72,    80,    83,
      86,    95,    96,    97,    98,   102,   108,   116,   117,   118,
     122,   123,   132,   137,   138,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   159,   162,   170,   174,   182,
     187,   198,   201,   208,   213,   215,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   234,
     235,   236,   237,   238,   239,   241,   243,   244,   245,   246,
     248,   251,   254,   259,   260
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
  "\"end of file\"", "error", "\"invalid token\"", "T_ID", "T_INT_LIT",
  "T_FLOAT_LIT", "T_BOOL_LIT", "T_INT", "T_FLOAT", "T_BOOL", "T_VOID",
  "T_IF", "T_ELSE", "T_WHILE", "T_FOR", "T_RETURN", "T_MAIN", "T_CIN",
  "T_COUT", "T_PLUS", "T_MINUS", "T_MULT", "T_DIV", "T_MOD", "T_ASSIGN",
  "T_EQ", "T_NEQ", "T_LT", "T_GT", "T_LE", "T_GE", "T_AND", "T_OR",
  "T_NOT", "T_INCREMENT", "T_DECREMENT", "T_STREAM_IN", "T_STREAM_OUT",
  "T_LPAREN", "T_RPAREN", "T_LBRACE", "T_RBRACE", "T_LBRACKET",
  "T_RBRACKET", "T_SEMI", "T_COMMA", "T_ERROR", "LOWER_THAN_ELSE",
  "$accept", "program", "declaration_list", "declaration",
  "var_declaration", "type", "func_declaration", "param_list", "param",
  "block", "statement_list", "statement", "assignment_stmt", "if_stmt",
  "while_stmt", "for_stmt", "io_stmt", "return_stmt", "expression",
  "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-106)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -106,    42,   533,  -106,    -3,  -106,  -106,  -106,  -106,  -106,
    -106,    41,  -106,  -106,    21,    11,   152,    85,    62,  -106,
      85,   -35,  -106,  -106,  -106,   152,   152,   152,   152,   152,
     172,    68,   -37,  -106,    44,     7,   145,   152,   -29,   -29,
     -29,   -29,   388,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,  -106,  -106,  -106,    48,
      56,    85,    53,    56,  -106,   451,    17,   312,  -106,   147,
     147,   -29,   -29,   -29,   502,   502,   154,   154,   154,   154,
     485,   468,    55,  -106,  -106,  -106,  -106,  -106,  -106,   152,
    -106,  -106,    71,   451,    59,   -20,    70,    76,    78,    20,
      81,    82,  -106,  -106,   115,  -106,  -106,  -106,  -106,  -106,
    -106,  -106,  -106,   192,  -106,   152,   152,   152,   152,   117,
    -106,   212,   118,   152,    26,  -106,   232,   331,   409,   430,
      27,   152,  -106,   -27,   252,  -106,    98,   126,   126,   152,
     272,   152,  -106,  -106,   152,   112,  -106,   350,   117,   369,
     292,   126,    98,    87,    94,  -106,  -106,   126,  -106,  -106
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     1,     0,    11,    12,    14,    13,     3,
       5,     0,     6,     7,     0,     0,     0,    19,     0,     8,
      19,    66,    67,    68,    69,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     9,    20,
       0,     0,     0,     0,    71,    74,     0,     0,    65,    46,
      47,    48,    49,    50,    55,    56,    51,    52,    53,    54,
      57,    58,     0,    24,    15,    17,    10,    16,    72,     0,
      70,    21,     0,    73,     0,    66,     0,     0,     0,     0,
       0,     0,    22,    32,     0,    31,    23,    25,    26,    27,
      28,    30,    29,     0,    34,     0,     0,     0,     0,     0,
      45,     0,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,    44,     0,     0,    35,    70,     0,     0,     0,
       0,     0,    41,    43,     0,    37,    39,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    38,     0,    42,    40
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -106,  -106,  -106,  -106,   140,    -1,  -106,   125,    86,    50,
    -106,   -90,  -105,  -106,  -106,  -106,  -106,  -106,   -16,  -106
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     9,   103,   104,    12,    32,    33,   105,
      92,   106,   107,   108,   109,   110,   111,   112,   113,    66
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      30,    11,    60,    36,   115,    56,    57,    37,    61,    38,
      39,    40,    41,    42,   131,   141,    31,   142,    36,    31,
      65,    67,   116,    21,    22,    23,    24,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      25,    13,     3,   153,    14,    16,    63,   145,   146,    20,
      16,   115,    61,    26,    27,    28,    88,    15,    29,    17,
      31,   156,    89,    18,   120,    19,    34,   159,    18,   139,
      19,    59,    94,    93,    95,    22,    23,    24,     5,     6,
       7,     8,    96,   121,    97,    98,    99,    62,   100,   101,
      82,    25,     5,     6,     7,     8,    83,    86,    91,   126,
     127,   128,   129,   114,    26,    27,    28,   134,   117,    29,
      84,    83,   102,    87,   118,   140,   119,   122,   124,   123,
     130,   133,   144,   147,   151,   149,   157,    94,   150,    95,
      22,    23,    24,     5,     6,     7,     8,    96,   158,    97,
      98,    99,    10,   100,   101,    35,    25,    85,    21,    22,
      23,    24,     0,     0,     0,    21,    22,    23,    24,    26,
      27,    28,     0,     0,    29,    25,    83,     0,    45,    46,
      47,     0,    25,    43,    44,    45,    46,    47,    26,    27,
      28,    56,    57,    29,    64,    26,    27,    28,    56,    57,
      29,    43,    44,    45,    46,    47,     0,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,    58,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,   125,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,   132,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,   135,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,   143,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,   148,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
       0,    43,    44,    45,    46,    47,   155,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,     0,     0,
      43,    44,    45,    46,    47,    90,    48,    49,    50,    51,
      52,    53,    54,    55,     0,    56,    57,     0,     0,    43,
      44,    45,    46,    47,   136,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,    57,     0,     0,    43,    44,
      45,    46,    47,   152,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,    57,     0,     0,    43,    44,    45,
      46,    47,   154,    48,    49,    50,    51,    52,    53,    54,
      55,     0,    56,    57,     0,     0,     0,    68,    43,    44,
      45,    46,    47,     0,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,    57,     0,     0,     0,   137,    43,
      44,    45,    46,    47,     0,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,    57,     0,     0,     0,   138,
      43,    44,    45,    46,    47,     0,    48,    49,    50,    51,
      52,    53,    54,    55,     0,    56,    57,    43,    44,    45,
      46,    47,     0,    48,    49,    50,    51,    52,    53,    54,
       0,     0,    56,    57,    43,    44,    45,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,     0,     0,    56,
      57,    43,    44,    45,    46,    47,     0,     0,     0,    50,
      51,    52,    53,    -2,     4,     0,    56,    57,     0,     0,
       5,     6,     7,     8
};

static const yytype_int16 yycheck[] =
{
      16,     2,    39,    38,    24,    34,    35,    42,    45,    25,
      26,    27,    28,    29,   119,    42,    17,    44,    38,    20,
      36,    37,    42,     3,     4,     5,     6,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      20,    44,     0,   148,     3,    24,    39,   137,   138,    38,
      24,    24,    45,    33,    34,    35,    39,    16,    38,    38,
      61,   151,    45,    42,    44,    44,     4,   157,    42,    42,
      44,     3,     1,    89,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    99,    13,    14,    15,    43,    17,    18,
      42,    20,     7,     8,     9,    10,    40,    44,    43,   115,
     116,   117,   118,    44,    33,    34,    35,   123,    38,    38,
      60,    40,    41,    63,    38,   131,    38,    36,     3,    37,
       3,     3,    24,   139,    12,   141,    39,     1,   144,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    44,    13,
      14,    15,     2,    17,    18,    20,    20,    61,     3,     4,
       5,     6,    -1,    -1,    -1,     3,     4,     5,     6,    33,
      34,    35,    -1,    -1,    38,    20,    40,    -1,    21,    22,
      23,    -1,    20,    19,    20,    21,    22,    23,    33,    34,
      35,    34,    35,    38,    39,    33,    34,    35,    34,    35,
      38,    19,    20,    21,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    44,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    -1,    -1,
      19,    20,    21,    22,    23,    43,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    35,    -1,    -1,    19,
      20,    21,    22,    23,    43,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    -1,    -1,    19,    20,
      21,    22,    23,    43,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    -1,    19,    20,    21,
      22,    23,    43,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    34,    35,    -1,    -1,    -1,    39,    19,    20,
      21,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    -1,    -1,    39,    19,
      20,    21,    22,    23,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    -1,    -1,    -1,    39,
      19,    20,    21,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    35,    19,    20,    21,
      22,    23,    -1,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    34,    35,    19,    20,    21,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    34,
      35,    19,    20,    21,    22,    23,    -1,    -1,    -1,    27,
      28,    29,    30,     0,     1,    -1,    34,    35,    -1,    -1,
       7,     8,     9,    10
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    49,    50,     0,     1,     7,     8,     9,    10,    51,
      52,    53,    54,    44,     3,    16,    24,    38,    42,    44,
      38,     3,     4,     5,     6,    20,    33,    34,    35,    38,
      66,    53,    55,    56,     4,    55,    38,    42,    66,    66,
      66,    66,    66,    19,    20,    21,    22,    23,    25,    26,
      27,    28,    29,    30,    31,    32,    34,    35,    44,     3,
      39,    45,    43,    39,    39,    66,    67,    66,    39,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    42,    40,    57,    56,    44,    57,    39,    45,
      43,    43,    58,    66,     1,     3,    11,    13,    14,    15,
      17,    18,    41,    52,    53,    57,    59,    60,    61,    62,
      63,    64,    65,    66,    44,    24,    42,    38,    38,    38,
      44,    66,    36,    37,     3,    44,    66,    66,    66,    66,
       3,    60,    44,     3,    66,    44,    43,    39,    39,    42,
      66,    42,    44,    44,    24,    59,    59,    66,    44,    66,
      66,    12,    43,    60,    43,    44,    59,    39,    44,    59
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    51,    52,    52,
      52,    53,    53,    53,    53,    54,    54,    55,    55,    55,
      56,    56,    57,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    60,    60,    61,    61,    62,
      63,    64,    64,    64,    65,    65,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    67,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     2,     3,     5,
       6,     1,     1,     1,     1,     6,     6,     3,     1,     0,
       2,     4,     3,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     4,     7,     5,     7,     5,
       8,     4,     7,     4,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     3,     1,     1,     1,     1,
       4,     3,     4,     3,     1
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: declaration_list  */
#line 61 "lib/parser.y"
                       { root = NEW_NODE(NODE_PROGRAM, "ROOT", (yyvsp[0].node), NULL); }
#line 1320 "build/parser.tab.c"
    break;

  case 3: /* declaration_list: declaration_list declaration  */
#line 65 "lib/parser.y"
                                   { (yyval.node) = append_node((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1326 "build/parser.tab.c"
    break;

  case 4: /* declaration_list: %empty  */
#line 66 "lib/parser.y"
      { (yyval.node) = NULL; }
#line 1332 "build/parser.tab.c"
    break;

  case 5: /* declaration: var_declaration  */
#line 70 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1338 "build/parser.tab.c"
    break;

  case 6: /* declaration: func_declaration  */
#line 71 "lib/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1344 "build/parser.tab.c"
    break;

  case 7: /* declaration: error T_SEMI  */
#line 72 "lib/parser.y"
                   {
        fprintf(stderr, "Panic Mode: Invalid declaration at line %d.\n", yylineno);
        yyerrok;
        (yyval.node) = NULL; 
    }
#line 1354 "build/parser.tab.c"
    break;

  case 8: /* var_declaration: type T_ID T_SEMI  */
#line 81 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_VAR_DECL, (yyvsp[-1].sval), (yyvsp[-2].node), NULL); }
#line 1360 "build/parser.tab.c"
    break;

  case 9: /* var_declaration: type T_ID T_ASSIGN expression T_SEMI  */
#line 84 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_VAR_DECL, (yyvsp[-3].sval), (yyvsp[-4].node), (yyvsp[-1].node)); }
#line 1366 "build/parser.tab.c"
    break;

  case 10: /* var_declaration: type T_ID T_LBRACKET T_INT_LIT T_RBRACKET T_SEMI  */
#line 87 "lib/parser.y"
      { 
          char buffer[50];
          sprintf(buffer, "%s[%s]", (yyvsp[-4].sval), (yyvsp[-2].sval));
          (yyval.node) = NEW_NODE(NODE_VAR_DECL, buffer, (yyvsp[-5].node), NULL); 
      }
#line 1376 "build/parser.tab.c"
    break;

  case 11: /* type: T_INT  */
#line 95 "lib/parser.y"
              { (yyval.node) = NEW_NODE(NODE_LITERAL, "int", NULL, NULL); }
#line 1382 "build/parser.tab.c"
    break;

  case 12: /* type: T_FLOAT  */
#line 96 "lib/parser.y"
              { (yyval.node) = NEW_NODE(NODE_LITERAL, "float", NULL, NULL); }
#line 1388 "build/parser.tab.c"
    break;

  case 13: /* type: T_VOID  */
#line 97 "lib/parser.y"
              { (yyval.node) = NEW_NODE(NODE_LITERAL, "void", NULL, NULL); }
#line 1394 "build/parser.tab.c"
    break;

  case 14: /* type: T_BOOL  */
#line 98 "lib/parser.y"
              { (yyval.node) = NEW_NODE(NODE_LITERAL, "bool", NULL, NULL); }
#line 1400 "build/parser.tab.c"
    break;

  case 15: /* func_declaration: type T_ID T_LPAREN param_list T_RPAREN block  */
#line 103 "lib/parser.y"
      { 
        ASTNode* signature = NEW_NODE(NODE_BLOCK, "Signature", (yyvsp[-5].node), (yyvsp[-2].node));
        (yyval.node) = create_node(NODE_FUNC_DECL, (yyvsp[-4].sval), (yyvsp[-5].node)->line, signature, (yyvsp[0].node));
      }
#line 1409 "build/parser.tab.c"
    break;

  case 16: /* func_declaration: type T_MAIN T_LPAREN param_list T_RPAREN block  */
#line 109 "lib/parser.y"
      { 
        ASTNode* signature = NEW_NODE(NODE_BLOCK, "Signature", (yyvsp[-5].node), (yyvsp[-2].node));
        (yyval.node) = create_node(NODE_FUNC_DECL, "main", (yyvsp[-5].node)->line, signature, (yyvsp[0].node));
      }
#line 1418 "build/parser.tab.c"
    break;

  case 17: /* param_list: param_list T_COMMA param  */
#line 116 "lib/parser.y"
                               { (yyval.node) = append_node((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1424 "build/parser.tab.c"
    break;

  case 18: /* param_list: param  */
#line 117 "lib/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1430 "build/parser.tab.c"
    break;

  case 19: /* param_list: %empty  */
#line 118 "lib/parser.y"
      { (yyval.node) = NULL; }
#line 1436 "build/parser.tab.c"
    break;

  case 20: /* param: type T_ID  */
#line 122 "lib/parser.y"
                { (yyval.node) = NEW_NODE(NODE_VAR_DECL, (yyvsp[0].sval), (yyvsp[-1].node), NULL); }
#line 1442 "build/parser.tab.c"
    break;

  case 21: /* param: type T_ID T_LBRACKET T_RBRACKET  */
#line 124 "lib/parser.y"
      { 
          char buffer[50];
          sprintf(buffer, "%s[]", (yyvsp[-2].sval));
          (yyval.node) = NEW_NODE(NODE_VAR_DECL, buffer, (yyvsp[-3].node), NULL); 
      }
#line 1452 "build/parser.tab.c"
    break;

  case 22: /* block: T_LBRACE statement_list T_RBRACE  */
#line 133 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_BLOCK, "{}", (yyvsp[-1].node), NULL); }
#line 1458 "build/parser.tab.c"
    break;

  case 23: /* statement_list: statement_list statement  */
#line 137 "lib/parser.y"
                               { (yyval.node) = append_node((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1464 "build/parser.tab.c"
    break;

  case 24: /* statement_list: %empty  */
#line 138 "lib/parser.y"
      { (yyval.node) = NULL; }
#line 1470 "build/parser.tab.c"
    break;

  case 25: /* statement: assignment_stmt  */
#line 142 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1476 "build/parser.tab.c"
    break;

  case 26: /* statement: if_stmt  */
#line 143 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1482 "build/parser.tab.c"
    break;

  case 27: /* statement: while_stmt  */
#line 144 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1488 "build/parser.tab.c"
    break;

  case 28: /* statement: for_stmt  */
#line 145 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1494 "build/parser.tab.c"
    break;

  case 29: /* statement: return_stmt  */
#line 146 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1500 "build/parser.tab.c"
    break;

  case 30: /* statement: io_stmt  */
#line 147 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1506 "build/parser.tab.c"
    break;

  case 31: /* statement: block  */
#line 148 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1512 "build/parser.tab.c"
    break;

  case 32: /* statement: var_declaration  */
#line 149 "lib/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1518 "build/parser.tab.c"
    break;

  case 33: /* statement: expression T_SEMI  */
#line 150 "lib/parser.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1524 "build/parser.tab.c"
    break;

  case 34: /* statement: error T_SEMI  */
#line 151 "lib/parser.y"
                   { 
        fprintf(stderr, "Panic Mode: Invalid statement at line %d.\n", yylineno);
        yyerrok; 
        (yyval.node) = NULL; 
    }
#line 1534 "build/parser.tab.c"
    break;

  case 35: /* assignment_stmt: T_ID T_ASSIGN expression T_SEMI  */
#line 160 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_ASSIGN, "=", NEW_NODE(NODE_VAR_USE, (yyvsp[-3].sval), NULL, NULL), (yyvsp[-1].node)); }
#line 1540 "build/parser.tab.c"
    break;

  case 36: /* assignment_stmt: T_ID T_LBRACKET expression T_RBRACKET T_ASSIGN expression T_SEMI  */
#line 163 "lib/parser.y"
      {
         ASTNode* arrAccess = NEW_NODE(NODE_VAR_USE, (yyvsp[-6].sval), (yyvsp[-4].node), NULL);
         (yyval.node) = NEW_NODE(NODE_ASSIGN, "[]=", arrAccess, (yyvsp[-1].node));
      }
#line 1549 "build/parser.tab.c"
    break;

  case 37: /* if_stmt: T_IF T_LPAREN expression T_RPAREN statement  */
#line 171 "lib/parser.y"
      { 
        (yyval.node) = NEW_NODE(NODE_IF, "if", (yyvsp[-2].node), (yyvsp[0].node));
      }
#line 1557 "build/parser.tab.c"
    break;

  case 38: /* if_stmt: T_IF T_LPAREN expression T_RPAREN statement T_ELSE statement  */
#line 175 "lib/parser.y"
      { 
        ASTNode* branches = NEW_NODE(NODE_BLOCK, "IfElseBranches", (yyvsp[-2].node), (yyvsp[0].node));
        (yyval.node) = NEW_NODE(NODE_IF, "if-else", (yyvsp[-4].node), branches);
      }
#line 1566 "build/parser.tab.c"
    break;

  case 39: /* while_stmt: T_WHILE T_LPAREN expression T_RPAREN statement  */
#line 183 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_WHILE, "while", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1572 "build/parser.tab.c"
    break;

  case 40: /* for_stmt: T_FOR T_LPAREN assignment_stmt expression T_SEMI assignment_stmt T_RPAREN statement  */
#line 188 "lib/parser.y"
      { 
          ASTNode* loopScope = NEW_NODE(NODE_BLOCK, "LoopScope", (yyvsp[-2].node), (yyvsp[0].node));
          
          ASTNode* loopRest = NEW_NODE(NODE_BLOCK, "LoopRest", (yyvsp[-4].node), loopScope);

          (yyval.node) = NEW_NODE(NODE_FOR, "for", (yyvsp[-5].node), loopRest);
      }
#line 1584 "build/parser.tab.c"
    break;

  case 41: /* io_stmt: T_CIN T_STREAM_IN T_ID T_SEMI  */
#line 199 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_IO, "read", NEW_NODE(NODE_VAR_USE, (yyvsp[-1].sval), NULL, NULL), NULL); }
#line 1590 "build/parser.tab.c"
    break;

  case 42: /* io_stmt: T_CIN T_STREAM_IN T_ID T_LBRACKET expression T_RBRACKET T_SEMI  */
#line 202 "lib/parser.y"
      { 
          // Create a VAR_USE node that has the index (expression) as its Left child
          ASTNode* arrAccess = NEW_NODE(NODE_VAR_USE, (yyvsp[-4].sval), (yyvsp[-2].node), NULL);
          (yyval.node) = NEW_NODE(NODE_IO, "read", arrAccess, NULL); 
      }
#line 1600 "build/parser.tab.c"
    break;

  case 43: /* io_stmt: T_COUT T_STREAM_OUT expression T_SEMI  */
#line 209 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_IO, "print", (yyvsp[-1].node), NULL); }
#line 1606 "build/parser.tab.c"
    break;

  case 44: /* return_stmt: T_RETURN expression T_SEMI  */
#line 214 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_RETURN, "return", (yyvsp[-1].node), NULL); }
#line 1612 "build/parser.tab.c"
    break;

  case 45: /* return_stmt: T_RETURN T_SEMI  */
#line 216 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_RETURN, "return", NULL, NULL); }
#line 1618 "build/parser.tab.c"
    break;

  case 46: /* expression: expression T_PLUS expression  */
#line 220 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "+", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1624 "build/parser.tab.c"
    break;

  case 47: /* expression: expression T_MINUS expression  */
#line 221 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "-", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1630 "build/parser.tab.c"
    break;

  case 48: /* expression: expression T_MULT expression  */
#line 222 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "*", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1636 "build/parser.tab.c"
    break;

  case 49: /* expression: expression T_DIV expression  */
#line 223 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "/", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1642 "build/parser.tab.c"
    break;

  case 50: /* expression: expression T_MOD expression  */
#line 224 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "%", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1648 "build/parser.tab.c"
    break;

  case 51: /* expression: expression T_LT expression  */
#line 225 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "<", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1654 "build/parser.tab.c"
    break;

  case 52: /* expression: expression T_GT expression  */
#line 226 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, ">", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1660 "build/parser.tab.c"
    break;

  case 53: /* expression: expression T_LE expression  */
#line 227 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "<=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1666 "build/parser.tab.c"
    break;

  case 54: /* expression: expression T_GE expression  */
#line 228 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, ">=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1672 "build/parser.tab.c"
    break;

  case 55: /* expression: expression T_EQ expression  */
#line 229 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "==", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1678 "build/parser.tab.c"
    break;

  case 56: /* expression: expression T_NEQ expression  */
#line 230 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "!=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1684 "build/parser.tab.c"
    break;

  case 57: /* expression: expression T_AND expression  */
#line 231 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "&&", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1690 "build/parser.tab.c"
    break;

  case 58: /* expression: expression T_OR expression  */
#line 232 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_BIN_OP, "||", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1696 "build/parser.tab.c"
    break;

  case 59: /* expression: T_MINUS expression  */
#line 234 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_UNARY_OP, "-", (yyvsp[0].node), NULL); }
#line 1702 "build/parser.tab.c"
    break;

  case 60: /* expression: T_NOT expression  */
#line 235 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_UNARY_OP, "!", (yyvsp[0].node), NULL); }
#line 1708 "build/parser.tab.c"
    break;

  case 61: /* expression: T_INCREMENT expression  */
#line 236 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_UNARY_OP, "++", (yyvsp[0].node), NULL); }
#line 1714 "build/parser.tab.c"
    break;

  case 62: /* expression: T_DECREMENT expression  */
#line 237 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_UNARY_OP, "--", (yyvsp[0].node), NULL); }
#line 1720 "build/parser.tab.c"
    break;

  case 63: /* expression: expression T_INCREMENT  */
#line 238 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_UNARY_OP, "POST++", (yyvsp[-1].node), NULL); }
#line 1726 "build/parser.tab.c"
    break;

  case 64: /* expression: expression T_DECREMENT  */
#line 239 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_UNARY_OP, "POST--", (yyvsp[-1].node), NULL); }
#line 1732 "build/parser.tab.c"
    break;

  case 65: /* expression: T_LPAREN expression T_RPAREN  */
#line 241 "lib/parser.y"
                                     { (yyval.node) = (yyvsp[-1].node); }
#line 1738 "build/parser.tab.c"
    break;

  case 66: /* expression: T_ID  */
#line 243 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_VAR_USE, (yyvsp[0].sval), NULL, NULL); }
#line 1744 "build/parser.tab.c"
    break;

  case 67: /* expression: T_INT_LIT  */
#line 244 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_LITERAL, (yyvsp[0].sval), NULL, NULL); }
#line 1750 "build/parser.tab.c"
    break;

  case 68: /* expression: T_FLOAT_LIT  */
#line 245 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_LITERAL, (yyvsp[0].sval), NULL, NULL); }
#line 1756 "build/parser.tab.c"
    break;

  case 69: /* expression: T_BOOL_LIT  */
#line 246 "lib/parser.y"
                                     { (yyval.node) = NEW_NODE(NODE_LITERAL, (yyvsp[0].sval), NULL, NULL); }
#line 1762 "build/parser.tab.c"
    break;

  case 70: /* expression: T_ID T_LBRACKET expression T_RBRACKET  */
#line 249 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_VAR_USE, (yyvsp[-3].sval), (yyvsp[-1].node), NULL); }
#line 1768 "build/parser.tab.c"
    break;

  case 71: /* expression: T_ID T_LPAREN T_RPAREN  */
#line 252 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_FUNC_CALL, (yyvsp[-2].sval), NULL, NULL); }
#line 1774 "build/parser.tab.c"
    break;

  case 72: /* expression: T_ID T_LPAREN arg_list T_RPAREN  */
#line 255 "lib/parser.y"
      { (yyval.node) = NEW_NODE(NODE_FUNC_CALL, (yyvsp[-3].sval), (yyvsp[-1].node), NULL); }
#line 1780 "build/parser.tab.c"
    break;

  case 73: /* arg_list: arg_list T_COMMA expression  */
#line 259 "lib/parser.y"
                                  { (yyval.node) = append_node((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1786 "build/parser.tab.c"
    break;

  case 74: /* arg_list: expression  */
#line 260 "lib/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1792 "build/parser.tab.c"
    break;


#line 1796 "build/parser.tab.c"

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 263 "lib/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s (Token: %s)\n", yylineno, s, yytext);
    syntax_errors++;
}
