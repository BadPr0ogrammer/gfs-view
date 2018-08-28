#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif
#include <stdio.h>
void my_log(const char *frmt, const char *s = NULL);

// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         configparse
#define yylex           configlex
// _bp_ #define yyerror         configerror
#define yydebug         configdebug
#define yynerrs         confignerrs

#define yylval          configlval
#define yychar          configchar

/* Copy the first part of user declarations.  */
#line 3 "config.tab.yy" /* yacc.c:339  */


////////////////////////////////////////////////////////////////////////


#define YYDEBUG 1


////////////////////////////////////////////////////////////////////////


using namespace std;

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include "vx_log.h"
#include "math_constants.h"
#include "is_bad_data.h"
#include "scanner_stuff.h"
#include "dictionary.h"
#include "threshold.h"
#include "is_number.h"
#include "concat_string.h"
#include "pwl.h"

#include "scanner_stuff.h"
#include "threshold.h"


////////////////////////////////////////////////////////////////////////


   //
   //  declarations that have external linkage
   //


extern int            yylex();

// _bp_ extern void           yyerror(const char *);

extern "C" int        configwrap();


extern char *         configtext;

extern FILE *         configin;


   //
   //  definitions that have external linkage
   //


int               LineNumber            = 1;

int               Column                = 1;

const char *      bison_input_filename  = (const char *) 0;

DictionaryStack * dict_stack            = (DictionaryStack *) 0;

bool              is_lhs                = true;

ThreshNode *      result                = 0;   //  for testing

bool              test_mode             = false;

// ConcatString   number_string;

char number_string [max_id_length];


////////////////////////////////////////////////////////////////////////


   //
   //  static definitions
   //

static PiecewiseLinear pwl;

static Dictionary DD;

static SingleThresh STH;


////////////////////////////////////////////////////////////////////////


   //
   //  static declarations
   //

static Number do_op(char op, const Number & a, const Number & b);

static Number do_integer_op(char op, const Number & a, const Number & b);

static Number do_negate(const Number &);


static void do_assign_boolean   (const char * name, bool);
static void do_assign_exp       (const char * name, const Number &);
static void do_assign_string    (const char * name, const char * text);
static void do_assign_threshold (const char * name);

static void do_assign_id      (const char * LHS, const char * RHS);

static void do_assign_dict    (const char * name);

static void do_dict();

static void do_string(const char *);

static void do_boolean(const bool &);

static void do_number(const Number &);

static void do_thresh(ThreshNode *);

static void do_na_thresh();

static void add_point(const Number &, const Number &);

static void do_pwl(const char * LHS);


static ThreshNode * do_and_thresh    (ThreshNode *, ThreshNode *);
static ThreshNode * do_or_thresh     (ThreshNode *, ThreshNode *);
static ThreshNode * do_not_thresh    (ThreshNode *);
static ThreshNode * do_paren_thresh  (ThreshNode *);
static ThreshNode * do_simple_thresh (ThreshType, const Number &);

static ThreshNode * do_fortran_thresh(const char *);


static void set_number_string();
static void set_number_string(const char *);


////////////////////////////////////////////////////////////////////////



#line 224 "config.tab.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "config.tab.h".  */
#ifndef YY_CONFIG_CONFIG_TAB_H_INCLUDED
# define YY_CONFIG_CONFIG_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int configdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    QUOTED_STRING = 259,
    INTEGER = 260,
    FLOAT = 261,
    BOOLEAN = 262,
    COMPARISON = 263,
    NA_COMPARISON = 264,
    LOGICAL_OP_NOT = 265,
    LOGICAL_OP_AND = 266,
    LOGICAL_OP_OR = 267,
    FORTRAN_THRESHOLD = 268,
    UNARY_MINUS = 269
  };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define QUOTED_STRING 259
#define INTEGER 260
#define FLOAT 261
#define BOOLEAN 262
#define COMPARISON 263
#define NA_COMPARISON 264
#define LOGICAL_OP_NOT 265
#define LOGICAL_OP_AND 266
#define LOGICAL_OP_OR 267
#define FORTRAN_THRESHOLD 268
#define UNARY_MINUS 269

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 154 "config.tab.yy" /* yacc.c:355  */


   char text[max_id_length];

   Number nval;

   bool bval;

   ThreshType cval;

   ThreshNode * node;


#line 306 "config.tab.cc" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE configlval;

int configparse (void);

#endif /* !YY_CONFIG_CONFIG_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 321 "config.tab.cc" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   165

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  29
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  55
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  107

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   269

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      27,    28,    16,    14,    26,    15,     2,    17,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    20,
       2,    22,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    23,     2,    21,    18,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,    25,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    19
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   198,   198,   199,   200,   204,   205,   206,   207,   209,
     210,   211,   213,   214,   215,   216,   217,   218,   223,   227,
     231,   235,   236,   240,   241,   245,   246,   249,   250,   253,
     254,   255,   256,   257,   261,   262,   266,   267,   271,   272,
     276,   277,   281,   282,   286,   287,   288,   289,   290,   291,
     292,   293,   297,   301,   302,   306
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "QUOTED_STRING", "INTEGER",
  "FLOAT", "BOOLEAN", "COMPARISON", "NA_COMPARISON", "LOGICAL_OP_NOT",
  "LOGICAL_OP_AND", "LOGICAL_OP_OR", "FORTRAN_THRESHOLD", "'+'", "'-'",
  "'*'", "'/'", "'^'", "UNARY_MINUS", "';'", "']'", "'='", "'['", "'{'",
  "'}'", "','", "'('", "')'", "$accept", "assignment_list", "assignment",
  "assign_prefix", "array_prefix", "dictionary", "dictionary_list",
  "string_list", "threshold_list", "threshold", "thresh_node",
  "simple_thresh", "number", "boolean_list", "number_list", "opt_semi",
  "expression", "piecewise_linear", "point_list", "point", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    43,    45,    42,    47,    94,   269,
      59,    93,    61,    91,   123,   125,    44,    40,    41
};
# endif

#define YYPACT_NINF -17

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-17)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      58,   -16,    74,   -17,    50,   -17,    50,    69,   -17,    15,
      82,   -17,   125,   -17,   -17,   -17,   -17,   -17,   -17,    29,
     -17,   -17,   -12,    -5,    56,    21,   -17,    58,    92,   -17,
      73,   -17,   145,    79,   -17,   -17,    84,   -17,    11,    23,
      38,   -17,   -17,    52,    75,    50,    50,   -17,   -17,   -17,
     -17,    21,   -17,     8,   105,   112,     7,   -17,   -17,    21,
      21,    21,    21,    21,   -17,   -17,   -17,    88,    19,    94,
     108,    96,     4,    97,    63,   111,    74,   -17,   -17,   114,
     105,   107,   -17,    21,   -17,   -17,    66,    66,   120,   120,
     -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,    21,   132,   127,   -17
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    28,     0,    35,     0,     0,     2,     0,
       0,     4,    27,    29,    18,    36,    37,    34,    32,     0,
       1,     3,     0,     0,     0,     0,    19,     0,     0,    11,
       0,    44,     0,     0,    23,    38,     0,    21,     0,     0,
       0,    25,    40,     0,     0,     0,     0,    33,     7,    10,
       5,     0,    50,     0,     0,     0,     0,    53,     9,     0,
       0,     0,     0,     0,     6,     8,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    30,    31,    43,
       0,     0,    51,     0,    52,    54,    45,    46,    47,    48,
      49,    16,    22,    14,    24,    15,    26,    12,    39,    13,
      41,    42,    20,     0,     0,     0,    55
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -17,   124,     3,   -17,   -17,    -6,   -17,   -17,   -17,    -7,
       1,   -17,    -1,   -17,   -17,   -17,    -9,   -17,   -17,    83
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    29,    38,    39,    40,    11,
      12,    13,    31,    43,    44,   102,    55,    33,    56,    57
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      32,    17,    30,    41,    37,    18,    14,    19,    48,    42,
      21,     1,     2,     3,     4,    49,    52,     5,    22,    23,
      15,    16,    24,     2,     3,     4,    15,    16,     5,    19,
      25,     6,    67,    79,    83,    84,    25,    68,    26,    27,
      45,    46,    28,    27,    69,    81,    77,    78,    51,    70,
      86,    87,    88,    89,    90,    19,    21,    47,     2,    71,
       4,     1,    92,     5,    72,    96,     2,     3,     4,    20,
      98,     5,     1,    73,   104,   100,    50,     6,    74,    15,
      16,    19,    61,    62,    63,     6,    34,    15,    16,    35,
       2,     3,     4,    58,   105,     5,    75,    15,    16,    65,
       2,    76,     4,    36,    66,     5,    27,    25,    91,     6,
      15,    16,    94,     2,    93,     4,    95,    97,     5,    54,
      25,    59,    60,    61,    62,    63,    59,    60,    61,    62,
      63,    99,    80,   103,   101,    82,    45,    46,    63,    85,
      82,    59,    60,    61,    62,    63,    59,    60,    61,    62,
      63,    53,     0,     0,     0,   106,     0,     0,   103,    59,
      60,    61,    62,    63,     0,    64
};

static const yytype_int8 yycheck[] =
{
       9,     2,     9,    10,    10,     4,    22,     6,    20,    10,
       7,     3,     8,     9,    10,    20,    25,    13,     3,     4,
       5,     6,     7,     8,     9,    10,     5,     6,    13,    28,
      15,    27,    21,    25,    27,    28,    15,    26,    23,    24,
      11,    12,    27,    24,    21,    54,    45,    46,    27,    26,
      59,    60,    61,    62,    63,    54,    53,    28,     8,    21,
      10,     3,    68,    13,    26,    72,     8,     9,    10,     0,
       7,    13,     3,    21,    83,    76,    20,    27,    26,     5,
       6,    80,    16,    17,    18,    27,     4,     5,     6,     7,
       8,     9,    10,    20,   103,    13,    21,     5,     6,    20,
       8,    26,    10,    21,    20,    13,    24,    15,    20,    27,
       5,     6,     4,     8,    20,    10,    20,    20,    13,    27,
      15,    14,    15,    16,    17,    18,    14,    15,    16,    17,
      18,    20,    27,    26,    20,    28,    11,    12,    18,    56,
      28,    14,    15,    16,    17,    18,    14,    15,    16,    17,
      18,    27,    -1,    -1,    -1,    28,    -1,    -1,    26,    14,
      15,    16,    17,    18,    -1,    20
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,     9,    10,    13,    27,    30,    31,    32,
      33,    38,    39,    40,    22,     5,     6,    41,    39,    39,
       0,    31,     3,     4,     7,    15,    23,    24,    27,    34,
      38,    41,    45,    46,     4,     7,    21,    34,    35,    36,
      37,    38,    41,    42,    43,    11,    12,    28,    20,    20,
      20,    27,    45,    30,    27,    45,    47,    48,    20,    14,
      15,    16,    17,    18,    20,    20,    20,    21,    26,    21,
      26,    21,    26,    21,    26,    21,    26,    39,    39,    25,
      27,    45,    28,    27,    28,    48,    45,    45,    45,    45,
      45,    20,    34,    20,     4,    20,    38,    20,     7,    20,
      41,    20,    44,    26,    45,    45,    28
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    29,    30,    30,    30,    31,    31,    31,    31,    31,
      31,    31,    31,    31,    31,    31,    31,    31,    32,    33,
      34,    35,    35,    36,    36,    37,    37,    38,    38,    39,
      39,    39,    39,    39,    40,    40,    41,    41,    42,    42,
      43,    43,    44,    44,    45,    45,    45,    45,    45,    45,
      45,    45,    46,    47,    47,    48
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     3,     3,     3,     3,     3,
       3,     2,     4,     4,     4,     4,     4,     3,     2,     2,
       4,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       3,     3,     2,     3,     2,     1,     1,     1,     1,     3,
       1,     3,     1,     0,     1,     3,     3,     3,     3,     3,
       2,     3,     3,     1,     2,     5
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      YYERROR;                                                  \
    }                                                           \
while (0)
// _bp_ was in else yyerror (YY_("syntax error: cannot back up")); 

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 2:
#line 198 "config.tab.yy" /* yacc.c:1646  */
    { is_lhs = true; }
#line 1475 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 3:
#line 199 "config.tab.yy" /* yacc.c:1646  */
    { is_lhs = true; }
#line 1481 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 200 "config.tab.yy" /* yacc.c:1646  */
    {  }
#line 1487 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 204 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_boolean   ((yyvsp[-2].text), (yyvsp[-1].bval)); }
#line 1493 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 205 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_exp       ((yyvsp[-2].text), (yyvsp[-1].nval)); }
#line 1499 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 206 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_id        ((yyvsp[-2].text), (yyvsp[-1].text)); }
#line 1505 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 207 "config.tab.yy" /* yacc.c:1646  */
    { do_pwl              ((yyvsp[-2].text)); }
#line 1511 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 209 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_threshold ((yyvsp[-2].text)); }
#line 1517 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 210 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_string    ((yyvsp[-2].text), (yyvsp[-1].text)); }
#line 1523 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 211 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict      ((yyvsp[-1].text)); }
#line 1529 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 213 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict((yyvsp[-3].text)); }
#line 1535 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 214 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict((yyvsp[-3].text)); }
#line 1541 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 215 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict((yyvsp[-3].text)); }
#line 1547 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 216 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict((yyvsp[-3].text)); }
#line 1553 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 217 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict((yyvsp[-3].text)); }
#line 1559 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 218 "config.tab.yy" /* yacc.c:1646  */
    { do_assign_dict((yyvsp[-2].text)); }
#line 1565 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 223 "config.tab.yy" /* yacc.c:1646  */
    { is_lhs = false;  strcpy((yyval.text), (yyvsp[-1].text)); }
#line 1571 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 227 "config.tab.yy" /* yacc.c:1646  */
    { is_lhs = false;  strcpy((yyval.text), (yyvsp[-1].text)); }
#line 1577 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 231 "config.tab.yy" /* yacc.c:1646  */
    { do_dict(); }
#line 1583 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 240 "config.tab.yy" /* yacc.c:1646  */
    { do_string((yyvsp[0].text)); }
#line 1589 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 241 "config.tab.yy" /* yacc.c:1646  */
    { do_string((yyvsp[0].text)); }
#line 1595 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 249 "config.tab.yy" /* yacc.c:1646  */
    { do_thresh    ((yyvsp[0].node)); }
#line 1601 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 250 "config.tab.yy" /* yacc.c:1646  */
    { do_na_thresh (); }
#line 1607 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 253 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1613 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 254 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = do_and_thresh   ((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1619 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 255 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = do_or_thresh    ((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1625 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 256 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = do_not_thresh   ((yyvsp[0].node));     }
#line 1631 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 257 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = do_paren_thresh ((yyvsp[-1].node));     }
#line 1637 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 261 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = do_simple_thresh((yyvsp[-1].cval), (yyvsp[0].nval)); }
#line 1643 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 262 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.node) = do_fortran_thresh((yyvsp[0].text)); }
#line 1649 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 266 "config.tab.yy" /* yacc.c:1646  */
    { set_number_string(); }
#line 1655 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 267 "config.tab.yy" /* yacc.c:1646  */
    { set_number_string(); }
#line 1661 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 271 "config.tab.yy" /* yacc.c:1646  */
    { do_boolean((yyvsp[0].bval)); }
#line 1667 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 272 "config.tab.yy" /* yacc.c:1646  */
    { do_boolean((yyvsp[0].bval)); }
#line 1673 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 276 "config.tab.yy" /* yacc.c:1646  */
    { do_number((yyvsp[0].nval)); }
#line 1679 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 277 "config.tab.yy" /* yacc.c:1646  */
    { do_number((yyvsp[0].nval)); }
#line 1685 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 286 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = (yyvsp[0].nval); }
#line 1691 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 287 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = do_op('+', (yyvsp[-2].nval), (yyvsp[0].nval)); }
#line 1697 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 288 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = do_op('-', (yyvsp[-2].nval), (yyvsp[0].nval)); }
#line 1703 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 289 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = do_op('*', (yyvsp[-2].nval), (yyvsp[0].nval)); }
#line 1709 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 290 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = do_op('/', (yyvsp[-2].nval), (yyvsp[0].nval)); }
#line 1715 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 291 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = do_op('^', (yyvsp[-2].nval), (yyvsp[0].nval)); }
#line 1721 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 292 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = do_negate((yyvsp[0].nval)); }
#line 1727 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 293 "config.tab.yy" /* yacc.c:1646  */
    { (yyval.nval) = (yyvsp[-1].nval); }
#line 1733 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 297 "config.tab.yy" /* yacc.c:1646  */
    { }
#line 1739 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 301 "config.tab.yy" /* yacc.c:1646  */
    { }
#line 1745 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 302 "config.tab.yy" /* yacc.c:1646  */
    { }
#line 1751 "config.tab.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 306 "config.tab.yy" /* yacc.c:1646  */
    { add_point((yyvsp[-3].nval), (yyvsp[-1].nval)); }
#line 1757 "config.tab.cc" /* yacc.c:1646  */
    break;


#line 1761 "config.tab.cc" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      // _bp_ yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  // _bp_ yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 310 "config.tab.yy" /* yacc.c:1906  */



////////////////////////////////////////////////////////////////////////


   //
   //  standard yacc stuff
   //


////////////////////////////////////////////////////////////////////////

/*
void yyerror(const char * s)

{

int j, j1, j2;
int line_len, text_len;
int c;
char line[max_id_length + 1];
ifstream in;
ConcatString msg;


c = (int) (Column - strlen(configtext));

mlog << Error
     << "\n"
     << "yyerror() -> syntax error in file \"" << bison_input_filename << "\"\n\n"
     << "   line   = " << LineNumber << "\n\n"
     << "   column = " << c << "\n\n"
     << "   text   = \"" << configtext << "\"\n\n";

in.open(bison_input_filename);

for (j=1; j<LineNumber; ++j)  {   //  j starts at one here, not zero

   in.getline(line, sizeof(line));

}

in.getline(line, sizeof(line));

in.close();




mlog << Error
     << "\n" << line << "\n";

line_len = strlen(line);

text_len = strlen(configtext);

j1 = c;
j2 = c + text_len - 1;

msg.erase();
for (j=1; j<=line_len; ++j)  {   //  j starts at one here, not zero

   if ( (j >= j1) && (j <= j2) )  msg << '^';
   else                           msg << '_';

}

mlog << Error
     << msg << "\n\n";


///exit ( 1 );
return;

}
*/

////////////////////////////////////////////////////////////////////////


int configwrap()

{

return ( 1 );

}


////////////////////////////////////////////////////////////////////////


Number do_op(char op, const Number & a, const Number & b)

{

Number c;
double A, B, C;

   //
   //  treat ^ as a special case
   //

if ( op == '^' )  {

   A = as_double(a);   
   B = as_double(b);   

   C = pow(A, B);

   set_double(c, C);

   return ( c );

}

   //
   //  for all other operators, see if we're doing integer arithmetic
   //

if ( a.is_int && b.is_int )  {

   c = do_integer_op(op, a, b);

   return ( c );

}

   //
   //  do floating-point arithmetic
   //

A = as_double(a);   
B = as_double(b);   

switch ( op )  {

   case '+':  C = A + B;  break;
   case '-':  C = A - B;  break;
   case '*':  C = A * B;  break;
   case '/':
      if ( is_eq(B, 0.0) )  {

        return Number();
      }
      C = A / B;
      break;

   default:
     my_log("err #%s\n", "0x6cafb99f");

     return Number();
}

set_double(c, C);

   //
   //  done
   //

return ( c );

}


////////////////////////////////////////////////////////////////////////


Number do_integer_op(char op, const Number & a, const Number & b)

{

Number c;
int A, B, C;

A = a.i;
B = b.i;

switch ( op )  {

   case '+':  C = A + B;  break;
   case '-':  C = A - B;  break;
   case '*':  C = A * B;  break;
   case '/':
      if ( B == 0 )  {
        my_log("err #%s\n", "0xd48c14d2");

        return Number();
      }
      C = A / B;
      break;

   default:
     my_log("err #%s\n","0xf8a1b044");

     return Number();
}

set_int(c, C);

   //
   //  done
   //

return ( c );

}


////////////////////////////////////////////////////////////////////////


Number do_negate(const Number & a)

{

Number b;

if ( a.is_int )  set_int    (b, -(a.i));
else             set_double (b, -(a.d));

return ( b );

}


////////////////////////////////////////////////////////////////////////


void do_assign_boolean(const char * name, bool tf)

{

DictionaryEntry entry;

entry.set_boolean(name, tf);

dict_stack->store(entry);

return;

}


////////////////////////////////////////////////////////////////////////


void do_assign_threshold(const char * name)

{

DictionaryEntry e;

e.set_threshold(name, STH);

dict_stack->store(e);

STH.clear();

return;

}


////////////////////////////////////////////////////////////////////////


void do_assign_exp(const char * name, const Number & n)

{

DictionaryEntry entry;

if ( n.is_int )  entry.set_int    (name, n.i);
else             entry.set_double (name, n.d);

dict_stack->store(entry);


return;

}


////////////////////////////////////////////////////////////////////////


void do_assign_string(const char * name, const char * text)

{

DictionaryEntry entry;

entry.set_string(name, text);

dict_stack->store(entry);

return;

}


////////////////////////////////////////////////////////////////////////


void do_assign_id(const char * LHS, const char * RHS)

{

const DictionaryEntry * e = dict_stack->lookup(RHS);

if ( !e )  {
  my_log("err #%s\n", "0x984a6f77");

  return;
}

DictionaryEntry ee = *e;

ee.set_name(LHS);

dict_stack->store(ee);

return;

}


////////////////////////////////////////////////////////////////////////


void do_dict()

{

DD = *(dict_stack->top());

dict_stack->erase_top();

if ( ! dict_stack->top_is_array() )  return;

DictionaryEntry e;

e.set_dict(0, DD);

dict_stack->store(e);

DD.clear();

return;

}


////////////////////////////////////////////////////////////////////////


void do_assign_dict (const char * name)

{

if ( DD.n_entries() > 0 )  {

   DictionaryEntry e;

   e.set_dict(name, DD);

   dict_stack->store(e);

   DD.clear();

} else {

   dict_stack->pop_dict(name);

}

return;

}


////////////////////////////////////////////////////////////////////////


void do_string(const char * text)

{

DictionaryEntry e;

e.set_string(0, text);

dict_stack->store(e);


return;

}


////////////////////////////////////////////////////////////////////////


void do_boolean(const bool & boolean)

{

DictionaryEntry e;

e.set_boolean(0, boolean);

dict_stack->store(e);


return;

}


////////////////////////////////////////////////////////////////////////


void do_number(const Number & number)

{

DictionaryEntry e;

if ( number.is_int )  e.set_int    (0, number.i);
else                  e.set_double (0, number.d);

dict_stack->store(e);


return;

}


////////////////////////////////////////////////////////////////////////


void do_thresh(ThreshNode * node)

{

if ( test_mode )  {

   result = node;

} 

else {

   if ( dict_stack->top_is_array() )  {

      DictionaryEntry e;
      SingleThresh T;

      T.set(node);

      e.set_threshold(0, T);

      dict_stack->store(e);

   }  else STH.set(node);

}

return;

}


////////////////////////////////////////////////////////////////////////


void do_na_thresh()

{

if ( !dict_stack->top_is_array() )  return;

DictionaryEntry e;
SingleThresh T;

T.set(na_str);

e.set_threshold(0, T);

dict_stack->store(e);

   //
   //  done
   //

return;

}


////////////////////////////////////////////////////////////////////////


void add_point(const Number & x, const Number & y)

{

pwl.add_point(as_double(x), as_double(y));

return;

}


////////////////////////////////////////////////////////////////////////


void do_pwl(const char * LHS)

{

DictionaryEntry e;

e.set_pwl(LHS, pwl);

dict_stack->store(e);


   //
   //  done
   //

pwl.clear();

return;

}


////////////////////////////////////////////////////////////////////////


ThreshNode * do_and_thresh    (ThreshNode * a, ThreshNode * b)

{

And_Node * n = new And_Node;

n->left_child  = a;
n->right_child = b;

n->s << a->s << "&&" << b->s;

n->abbr_s << a->abbr_s << ".and." << b->abbr_s;

return ( n );

}


////////////////////////////////////////////////////////////////////////


ThreshNode * do_or_thresh (ThreshNode * a, ThreshNode * b)

{

Or_Node * n = new Or_Node;

n->left_child  = a;
n->right_child = b;

n->s << a->s << "||" << b->s;

n->abbr_s << a->abbr_s << ".or." << b->abbr_s;

return ( n );

}


////////////////////////////////////////////////////////////////////////


ThreshNode * do_not_thresh    (ThreshNode * n)

{

Not_Node * nn = new Not_Node;

nn->child = n;

nn->s << '!' << n->s;

nn->abbr_s << ".not." << n->abbr_s;

return ( nn );

}


////////////////////////////////////////////////////////////////////////


ThreshNode * do_paren_thresh  (ThreshNode * n)

{

ConcatString b;

b.erase();

b << '(' << n->s << ')';

n->s = b;

b.erase();

b << '(' << n->abbr_s << ')';

n->abbr_s = b;

return ( n );

}


////////////////////////////////////////////////////////////////////////


ThreshNode * do_simple_thresh (ThreshType op, const Number & n)

{

Simple_Node * s = new Simple_Node;

s->op = op;

s->T = as_double(n);

if ( op >= 0 )  {

   s->s      << thresh_type_str[op] << number_string;
   s->abbr_s << thresh_abbr_str[op] << number_string;

}

return ( s );

}


////////////////////////////////////////////////////////////////////////


ThreshNode * do_fortran_thresh(const char * text)

{

ThreshType op  = no_thresh_type;
const char * p = text + 2;         //  we know that all the prefixes
                                   //  (like "le" or "gt") are two  
                                   //  characters long

     if ( strncmp(text, "le", 2) == 0 )  op = thresh_le;
else if ( strncmp(text, "lt", 2) == 0 )  op = thresh_lt;

else if ( strncmp(text, "gt", 2) == 0 )  op = thresh_gt;
else if ( strncmp(text, "ge", 2) == 0 )  op = thresh_ge;

else if ( strncmp(text, "eq", 2) == 0 )  op = thresh_eq;
else if ( strncmp(text, "ne", 2) == 0 )  op = thresh_ne;

else {
  my_log("err #%s\n", "0x7d6f8ac0");

  return NULL;
}

Number n;
const double value = atof(p);

n.is_int = 0;

n.d = value;

set_number_string(p);

return ( do_simple_thresh (op, n) );

}


////////////////////////////////////////////////////////////////////////


void set_number_string()

{

set_number_string(configtext);

return;

}


////////////////////////////////////////////////////////////////////////


void set_number_string(const char * text)

{

const int k = (int) (sizeof(number_string));

strncpy(number_string, text, k);

number_string[k - 1] = (char) 0;

return;

}


////////////////////////////////////////////////////////////////////////
