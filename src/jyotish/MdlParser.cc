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




/* Copy the first part of user declarations.  */
#line 21 "MdlParser.yy" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include <wx/string.h>

#include "Expression.h"

#define YYDEBUG 1

#define YYERROR_VERBOSE

int yyerror ( const char* s);
extern int yylex( void );
extern int yylineno;
extern char *yytext;

Rule *rule = 0;
wxString parseErrorMessage;


#line 88 "y.tab.c" /* yacc.c:339  */

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
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LPAR = 258,
    RPAR = 259,
    SEMICOLON = 260,
    COMMA = 261,
    PERCENT = 262,
    DIGIT = 263,
    OR = 264,
    AND = 265,
    NOT = 266,
    MUTUAL_KENDRA = 267,
    DESCR = 268,
    LORDOF = 269,
    ISPLANETINHOUSE = 270,
    ISPLANETINRASI = 271,
    ISEQUAL = 272,
    ISNOTEQUAL = 273,
    RETURN = 274,
    PLANET = 275,
    GETRASI = 276,
    GETBHAVA = 277,
    PLUS = 278,
    MINUS = 279,
    MULT = 280,
    DIV = 281,
    MOD = 282,
    LT = 283,
    LE = 284,
    GT = 285,
    GE = 286,
    PLANETS_INSIGN = 287,
    PLANETS_INHOUSE = 288,
    NUMBER_OF_OCCUPIED_SIGNS = 289,
    GRAHA_DRISHTI = 290,
    GETLORD = 291,
    ISBENEFIC = 292,
    ISMALEFIC = 293,
    ISPLANETINKENDRA = 294,
    ISPLANETINAPOKLIMA = 295,
    ISPLANETINPANAPHARA = 296,
    ISPLANETINDUALRASI = 297,
    ISPLANETINFIXEDRASI = 298,
    ISPLANETINMOVABLERASI = 299,
    RED12 = 300,
    GETNAKSHATRA = 301,
    RED27 = 302,
    RASIONLY = 303,
    HOUSE = 304,
    SIGN = 305,
    NAKSHATRA = 306,
    BOOLEAN = 307,
    TYPE_INT = 308,
    TYPE_DOUBLE = 309,
    TYPE_PLANET = 310,
    TYPE_SIGN = 311,
    TYPE_NAKSHATRA = 312,
    TYPE_HOUSE = 313,
    TYPE_BOOLEAN = 314,
    IDENT = 315,
    STRING = 316
  };
#endif
/* Tokens.  */
#define LPAR 258
#define RPAR 259
#define SEMICOLON 260
#define COMMA 261
#define PERCENT 262
#define DIGIT 263
#define OR 264
#define AND 265
#define NOT 266
#define MUTUAL_KENDRA 267
#define DESCR 268
#define LORDOF 269
#define ISPLANETINHOUSE 270
#define ISPLANETINRASI 271
#define ISEQUAL 272
#define ISNOTEQUAL 273
#define RETURN 274
#define PLANET 275
#define GETRASI 276
#define GETBHAVA 277
#define PLUS 278
#define MINUS 279
#define MULT 280
#define DIV 281
#define MOD 282
#define LT 283
#define LE 284
#define GT 285
#define GE 286
#define PLANETS_INSIGN 287
#define PLANETS_INHOUSE 288
#define NUMBER_OF_OCCUPIED_SIGNS 289
#define GRAHA_DRISHTI 290
#define GETLORD 291
#define ISBENEFIC 292
#define ISMALEFIC 293
#define ISPLANETINKENDRA 294
#define ISPLANETINAPOKLIMA 295
#define ISPLANETINPANAPHARA 296
#define ISPLANETINDUALRASI 297
#define ISPLANETINFIXEDRASI 298
#define ISPLANETINMOVABLERASI 299
#define RED12 300
#define GETNAKSHATRA 301
#define RED27 302
#define RASIONLY 303
#define HOUSE 304
#define SIGN 305
#define NAKSHATRA 306
#define BOOLEAN 307
#define TYPE_INT 308
#define TYPE_DOUBLE 309
#define TYPE_PLANET 310
#define TYPE_SIGN 311
#define TYPE_NAKSHATRA 312
#define TYPE_HOUSE 313
#define TYPE_BOOLEAN 314
#define IDENT 315
#define STRING 316

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 43 "MdlParser.yy" /* yacc.c:355  */

  int ival;
	char *sval;
	double dval;
	Expression *expval;

#line 257 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 272 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   856

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  197

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   316

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    78,    78,    78,    80,    80,    82,    84,    84,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    95,    97,
      97,    97,    97,    97,    97,    97,    98,    98,    98,    98,
      98,    99,    99,    99,    99,    99,   100,   100,   100,   100,
     100,   100,   100,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   115,   116,   117,   118,   119,   120,
     122,   123,   124,   125,   126,   127,   128,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   131,   131,
     131,   131,   131,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LPAR", "RPAR", "SEMICOLON", "COMMA",
  "PERCENT", "DIGIT", "OR", "AND", "NOT", "MUTUAL_KENDRA", "DESCR",
  "LORDOF", "ISPLANETINHOUSE", "ISPLANETINRASI", "ISEQUAL", "ISNOTEQUAL",
  "RETURN", "PLANET", "GETRASI", "GETBHAVA", "PLUS", "MINUS", "MULT",
  "DIV", "MOD", "LT", "LE", "GT", "GE", "PLANETS_INSIGN",
  "PLANETS_INHOUSE", "NUMBER_OF_OCCUPIED_SIGNS", "GRAHA_DRISHTI",
  "GETLORD", "ISBENEFIC", "ISMALEFIC", "ISPLANETINKENDRA",
  "ISPLANETINAPOKLIMA", "ISPLANETINPANAPHARA", "ISPLANETINDUALRASI",
  "ISPLANETINFIXEDRASI", "ISPLANETINMOVABLERASI", "RED12", "GETNAKSHATRA",
  "RED27", "RASIONLY", "HOUSE", "SIGN", "NAKSHATRA", "BOOLEAN", "TYPE_INT",
  "TYPE_DOUBLE", "TYPE_PLANET", "TYPE_SIGN", "TYPE_NAKSHATRA",
  "TYPE_HOUSE", "TYPE_BOOLEAN", "IDENT", "STRING", "$accept", "rule_decl",
  "$@1", "exp_list", "stmt", "exp", "return_exp", "unary_exp", "not_exp",
  "red12_exp", "lordof_exp", "getrasi_exp", "getlord_exp", "getbhava_exp",
  "planetsinsign_exp", "planetsinhouse_exp", "getnakshatra_exp",
  "isbenefic_exp", "ismalefic_exp", "isplanetinkendra_exp",
  "isplanetinapoklima_exp", "isplanetinpanaphara_exp",
  "isplanetindualrasi_exp", "isplanetinfixedrasi_exp",
  "isplanetinmovablerasi_exp", "typecast_int_exp", "typecast_double_exp",
  "typecast_planet_exp", "typecast_house_exp", "typecast_nakshatra_exp",
  "typecast_boolean_exp", "typecast_sign_exp", "dual_exp", "and_exp",
  "or_exp", "isequal_exp", "isnotequal_exp", "plus_exp", "minus_exp",
  "mult_exp", "div_exp", "mod_exp", "lt_exp", "le_exp", "gt_exp", "ge_exp",
  "mutual_kendra_exp", "housematch_exp", "grahadrishti_exp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316
};
# endif

#define YYPACT_NINF -10

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-10)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -10,     4,   144,   -10,     0,   -10,   144,    -2,     2,     6,
     144,   -10,     7,    10,    20,    26,    27,    28,    44,    45,
      59,    60,    61,    62,    63,    64,    67,    68,    69,   -10,
     -10,   -10,   -10,   144,   -10,   747,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,     3,
      79,    83,    88,   101,   102,   125,   188,    -9,   144,   144,
     144,    51,   144,   144,   144,   144,   126,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   -10,
     -10,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   -10,   773,   216,   799,   244,   272,   300,   328,   -10,
     825,   356,   384,   412,   440,   468,   496,   524,   552,   580,
     608,   636,   110,   110,     1,     1,    -9,    -9,   -10,   -10,
     -10,     1,     1,     1,     1,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,   144,   -10,   144,   -10,   -10,   -10,   -10,   144,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   664,   692,   720,   -10,   -10,   -10
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,    12,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
      15,    16,    17,     3,     4,     0,     9,     7,    19,    34,
      20,    22,    23,    21,    24,    25,    35,    27,    26,    28,
      29,    30,    31,    32,    33,    36,    37,    38,    39,    40,
      41,    42,     8,    68,    69,    70,    71,    72,    73,    75,
      74,    76,    79,    80,    81,    82,    77,    67,    78,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,     0,     0,     0,     0,     0,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,    83,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    60,    61,    62,    66,    64,
      63,    65,     0,    45,     0,    46,    48,    49,    50,     0,
      47,    52,    53,    54,    55,    56,    57,    58,    59,    44,
      51,     0,     0,     0,    96,    97,    98
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -10,   -10,   -10,   -10,    40,    -4,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      86,    88,    87,     4,     3,    89,    91,   124,     5,    90,
      92,     6,     7,    93,     8,     9,   117,   118,   119,    10,
      11,    12,    13,    94,   115,   116,   117,   118,   119,    95,
      96,    97,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    98,    99,    29,
      30,    31,    32,    79,    80,    81,    82,    83,    84,    85,
     111,   112,   100,   101,   102,   103,   104,   105,   113,   114,
     106,   107,   108,   109,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   125,   132,   133,   134,   126,   135,   136,
     137,   138,   127,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   128,   129,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   113,   114,   130,
     139,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     0,     5,     0,     0,     6,     7,     0,     8,     9,
       0,     0,     0,    10,    11,    12,    13,     0,   191,     0,
     192,     0,     0,     0,     0,   193,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,   131,    29,    30,    31,    32,   111,   112,     0,
       0,     0,     0,     0,     0,   113,   114,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     173,     0,     0,     0,     0,   111,   112,     0,     0,     0,
       0,     0,     0,   113,   114,     0,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   175,     0,
       0,     0,     0,   111,   112,     0,     0,     0,     0,     0,
       0,   113,   114,     0,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   176,     0,     0,     0,
       0,   111,   112,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   177,     0,     0,     0,     0,   111,
     112,     0,     0,     0,     0,     0,     0,   113,   114,     0,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   178,     0,     0,     0,     0,   111,   112,     0,
       0,     0,     0,     0,     0,   113,   114,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     180,     0,     0,     0,     0,   111,   112,     0,     0,     0,
       0,     0,     0,   113,   114,     0,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   181,     0,
       0,     0,     0,   111,   112,     0,     0,     0,     0,     0,
       0,   113,   114,     0,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   182,     0,     0,     0,
       0,   111,   112,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   183,     0,     0,     0,     0,   111,
     112,     0,     0,     0,     0,     0,     0,   113,   114,     0,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   184,     0,     0,     0,     0,   111,   112,     0,
       0,     0,     0,     0,     0,   113,   114,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     185,     0,     0,     0,     0,   111,   112,     0,     0,     0,
       0,     0,     0,   113,   114,     0,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   186,     0,
       0,     0,     0,   111,   112,     0,     0,     0,     0,     0,
       0,   113,   114,     0,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   187,     0,     0,     0,
       0,   111,   112,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   188,     0,     0,     0,     0,   111,
     112,     0,     0,     0,     0,     0,     0,   113,   114,     0,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   189,     0,     0,     0,     0,   111,   112,     0,
       0,     0,     0,     0,     0,   113,   114,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     190,     0,     0,     0,     0,   111,   112,     0,     0,     0,
       0,     0,     0,   113,   114,     0,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   194,     0,
       0,     0,     0,   111,   112,     0,     0,     0,     0,     0,
       0,   113,   114,     0,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   195,     0,     0,     0,
       0,   111,   112,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   196,     0,     0,     0,     0,   111,
     112,     0,     0,     0,     0,     0,     0,   113,   114,     0,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   110,     0,     0,     0,   111,   112,     0,     0,
       0,     0,     0,     0,   113,   114,     0,     0,     0,     0,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   172,
       0,     0,   111,   112,     0,     0,     0,     0,     0,     0,
     113,   114,     0,     0,     0,     0,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   174,     0,     0,   111,   112,
       0,     0,     0,     0,     0,     0,   113,   114,     0,     0,
       0,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   179,     0,     0,   111,   112,     0,     0,     0,     0,
       0,     0,   113,   114,     0,     0,     0,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123
};

static const yytype_int16 yycheck[] =
{
       4,     3,     6,     3,     0,     3,    10,     4,     8,     3,
       3,    11,    12,     3,    14,    15,    25,    26,    27,    19,
      20,    21,    22,     3,    23,    24,    25,    26,    27,     3,
       3,     3,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,     3,     3,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
       9,    10,     3,     3,     3,     3,     3,     3,    17,    18,
       3,     3,     3,    33,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     4,    88,    89,    90,     4,    92,    93,
      94,    95,     4,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,     4,     4,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    17,    18,     4,
       4,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    -1,    11,    12,    -1,    14,    15,
      -1,    -1,    -1,    19,    20,    21,    22,    -1,   172,    -1,
     174,    -1,    -1,    -1,    -1,   179,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,     4,    49,    50,    51,    52,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       4,    -1,    -1,    -1,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     4,    -1,
      -1,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     4,    -1,    -1,    -1,
      -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     4,    -1,    -1,    -1,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     4,    -1,    -1,    -1,    -1,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       4,    -1,    -1,    -1,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     4,    -1,
      -1,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     4,    -1,    -1,    -1,
      -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     4,    -1,    -1,    -1,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     4,    -1,    -1,    -1,    -1,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       4,    -1,    -1,    -1,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     4,    -1,
      -1,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     4,    -1,    -1,    -1,
      -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     4,    -1,    -1,    -1,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     4,    -1,    -1,    -1,    -1,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       4,    -1,    -1,    -1,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     4,    -1,
      -1,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     4,    -1,    -1,    -1,
      -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     4,    -1,    -1,    -1,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     5,    -1,    -1,    -1,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    -1,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     6,
      -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     6,    -1,    -1,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     6,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    63,    64,     0,     3,     8,    11,    12,    14,    15,
      19,    20,    21,    22,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    49,
      50,    51,    52,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,    53,
      54,    55,    56,    57,    58,    59,    67,    67,     3,     3,
       3,    67,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,    66,
       5,     9,    10,    17,    18,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     4,     4,     4,     4,     4,     4,
       4,     4,    67,    67,    67,    67,    67,    67,    67,     4,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,     6,     4,     6,     4,     4,     4,     4,     6,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,    67,    67,    67,     4,     4,     4
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    62,    64,    63,    65,    65,    66,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     2,     1,     1,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     6,     6,     6
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
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

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
#line 78 "MdlParser.yy" /* yacc.c:1646  */
    { rule = new Rule; }
#line 1633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 82 "MdlParser.yy" /* yacc.c:1646  */
    { rule->addExpression( (yyvsp[-1].expval) ); }
#line 1639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 85 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = (yyvsp[-1].expval); }
#line 1645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 86 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_NUMBER_OF_OCCUPIED_SIGNS ); }
#line 1651 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 87 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_CONST, (yyvsp[0].ival) ); }
#line 1657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 88 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_HOUSE, (yyvsp[0].ival) ); }
#line 1663 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 89 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_PLANET, (yyvsp[0].ival) ); }
#line 1669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 90 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_SIGN, (yyvsp[0].ival) ); }
#line 1675 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 91 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_NAKSHATRA, (yyvsp[0].ival) ); }
#line 1681 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 92 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new NumericExpression( ED_BOOLEAN, (yyvsp[0].ival) ); }
#line 1687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 95 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new ReturnExpression( (yyvsp[0].expval) ); }
#line 1693 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 103 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_NOT ); }
#line 1699 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 104 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_RED12 ); }
#line 1705 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 105 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_LORDOFHOUSE ); }
#line 1711 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 106 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_GETRASI ); }
#line 1717 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 107 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_GETLORD ); }
#line 1723 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 108 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_GETBHAVA ); }
#line 1729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 109 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_PLANETS_INSIGN ); }
#line 1735 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 110 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_PLANETS_INHOUSE ); }
#line 1741 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 111 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_GETNAKSHATRA ); }
#line 1747 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 112 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISBENEFIC ); }
#line 1753 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 113 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISMALEFIC ); }
#line 1759 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 115 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISPLANETINKENDRA ); }
#line 1765 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 116 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISPLANETINAPOKLIMA ); }
#line 1771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 117 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISPLANETINPANAPHARA ); }
#line 1777 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 118 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISPLANETINDUALRASI ); }
#line 1783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 119 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISPLANETINFIXEDRASI ); }
#line 1789 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 120 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[-1].expval), E1_ISPLANETINMOVABLERASI ); }
#line 1795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 122 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_INT ); }
#line 1801 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 123 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_DOUBLE ); }
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 124 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_PLANET ); }
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 125 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_HOUSE ); }
#line 1819 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 126 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_NAKSHATRA ); }
#line 1825 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 127 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_BOOLEAN ); }
#line 1831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 128 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new UnaryExpression( (yyvsp[0].expval), E1_TYPECAST_SIGN ); }
#line 1837 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 134 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_LOGICAL_AND ); }
#line 1843 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 135 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_LOGICAL_OR ); }
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 136 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_ISEQUAL ); }
#line 1855 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 137 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_ISNOTEQUAL ); }
#line 1861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 138 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_PLUS ); }
#line 1867 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 139 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_MINUS ); }
#line 1873 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 140 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_MULT ); }
#line 1879 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 141 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_DIV ); }
#line 1885 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 142 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_MOD ); }
#line 1891 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 143 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_LT ); }
#line 1897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 144 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_LE ); }
#line 1903 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 145 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_GT ); }
#line 1909 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 146 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-2].expval), (yyvsp[0].expval), E2_GE ); }
#line 1915 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 147 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-3].expval), (yyvsp[-1].expval), E2_MUTUAL_KENDRA ); }
#line 1921 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 148 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-3].expval), (yyvsp[-1].expval), E2_PLANETINHOUSE ); }
#line 1927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 149 "MdlParser.yy" /* yacc.c:1646  */
    { (yyval.expval) = new DualExpression( (yyvsp[-3].expval), (yyvsp[-1].expval), E2_GRAHA_DRISHTI ); }
#line 1933 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1937 "y.tab.c" /* yacc.c:1646  */
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
      yyerror (YY_("syntax error"));
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
  yyerror (YY_("memory exhausted"));
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
#line 152 "MdlParser.yy" /* yacc.c:1906  */

/*****************************************************
**
**   yyerror
**
******************************************************/
int yyerror ( const char* s)
{
	char msg[1024];
	sprintf( msg, "Parse error at line %d near token \"%s\" (message: %s)", yylineno, yytext, s );

	parseErrorMessage = wxConvCurrent->cMB2WX( msg );
	fprintf( stderr, "%s\n", msg );
	yylineno = 1;
	return 1;
}

