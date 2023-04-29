/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20221106

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 22 "MdlParser.yy"

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

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 43 "MdlParser.yy"
typedef union YYSTYPE {
  int ival;
	char *sval;
	double dval;
	Expression *expval;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 58 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define LPAR 257
#define RPAR 258
#define SEMICOLON 259
#define COMMA 260
#define PERCENT 261
#define DIGIT 262
#define OR 263
#define AND 264
#define NOT 265
#define MUTUAL_KENDRA 266
#define DESCR 267
#define LORDOF 268
#define ISPLANETINHOUSE 269
#define ISPLANETINRASI 270
#define ISEQUAL 271
#define ISNOTEQUAL 272
#define RETURN 273
#define PLANET 274
#define GETRASI 275
#define GETBHAVA 276
#define PLUS 277
#define MINUS 278
#define MULT 279
#define DIV 280
#define MOD 281
#define LT 282
#define LE 283
#define GT 284
#define GE 285
#define PLANETS_INSIGN 286
#define PLANETS_INHOUSE 287
#define NUMBER_OF_OCCUPIED_SIGNS 288
#define GRAHA_DRISHTI 289
#define GETLORD 290
#define ISBENEFIC 291
#define ISMALEFIC 292
#define ISPLANETINKENDRA 293
#define ISPLANETINAPOKLIMA 294
#define ISPLANETINPANAPHARA 295
#define ISPLANETINDUALRASI 296
#define ISPLANETINFIXEDRASI 297
#define ISPLANETINMOVABLERASI 298
#define RED12 299
#define GETNAKSHATRA 300
#define RED27 301
#define RASIONLY 302
#define HOUSE 303
#define SIGN 304
#define NAKSHATRA 305
#define BOOLEAN 306
#define TYPE_INT 307
#define TYPE_DOUBLE 308
#define TYPE_PLANET 309
#define TYPE_SIGN 310
#define TYPE_NAKSHATRA 311
#define TYPE_HOUSE 312
#define TYPE_BOOLEAN 313
#define IDENT 314
#define STRING 315
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
   46,    0,   45,   45,   47,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,   35,   36,    5,    9,   22,   10,   19,   20,   37,
   23,   24,   25,   26,   27,   32,   33,   34,   38,   39,
   40,   41,   42,   43,   44,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    6,    7,   11,   12,   13,   14,   16,   15,   17,
   28,   29,   30,   31,   18,    8,   21,
};
static const YYINT yylen[] = {                            2,
    0,    2,    1,    2,    2,    1,    1,    1,    3,    3,
    1,    1,    1,    1,    1,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    6,    6,    6,
};
static const YYINT yydefred[] = {                         1,
    0,    0,    0,   11,    0,    0,    0,    0,    0,   13,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   12,   14,   15,
   16,    0,    8,    6,    7,   19,   67,   68,   66,   21,
   20,   69,   70,   71,   72,   73,   74,   75,   76,   23,
   24,   77,   22,   26,   25,   27,   28,   29,   78,   79,
   80,   81,   30,   31,   32,   18,   33,   34,   35,   36,
   37,   38,   39,   40,   41,    0,    3,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    5,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    4,    0,    0,    0,    0,    0,    0,    0,    9,
    0,    0,    0,    0,    0,    0,    0,   10,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   88,   89,   90,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   44,    0,   45,   47,   48,   49,    0,   46,   51,
   52,   53,   54,   55,   56,   57,   58,   43,   50,    0,
    0,    0,   95,   96,   97,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  317,  363,  257,  262,  265,  266,  268,  269,  273,  274,
  275,  276,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  303,  304,  305,
  306,  318,  319,  320,  321,  322,  323,  324,  325,  326,
  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,
  337,  338,  339,  340,  341,  342,  343,  344,  345,  346,
  347,  348,  349,  350,  351,  352,  353,  354,  355,  356,
  357,  358,  359,  360,  361,  362,  364,  307,  308,  309,
  310,  311,  312,  313,  318,  318,  257,  257,  257,  318,
  257,  257,  257,  257,  257,  257,  257,  257,  257,  257,
  257,  257,  257,  257,  257,  257,  257,  259,  263,  264,
  271,  272,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  364,  258,  258,  258,  258,  258,  258,  258,  258,
  318,  318,  318,  318,  318,  318,  318,  258,  318,  318,
  318,  318,  318,  318,  318,  318,  318,  318,  318,  318,
  318,  318,  318,  318,  318,  318,  318,  318,  318,  318,
  318,  318,  318,  318,  318,  318,  318,  318,  318,  318,
  260,  258,  260,  258,  258,  258,  258,  260,  258,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  318,
  318,  318,  258,  258,  258,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          1,
   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
   72,   73,   74,   75,   76,    2,   77,
};
static const YYINT yysindex[] = {                         0,
    0, -110, -254,    0, -110, -256, -253, -252, -110,    0,
 -250, -248, -247, -244, -231, -230, -227, -226, -210, -209,
 -185, -184, -174, -170, -165, -152, -138,    0,    0,    0,
    0,  744,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -110,    0, -129, -128, -127,
 -126, -109, -108, -107,  -66, -263, -110, -110, -110, -203,
 -110, -110, -110, -110, -105, -110, -110, -110, -110, -110,
 -110, -110, -110, -110, -110, -110, -110,    0, -110, -110,
 -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 -110,    0, -110, -110, -110, -110, -110, -110, -110,    0,
  770,  -38,  796,  -10,   18,   46,   74,    0,  822,  102,
  130,  158,  186,  214,  242,  270,  298,  326,  354,  382,
 -144, -144, -135, -135, -263, -263,    0,    0,    0, -135,
 -135, -135, -135, -263, -263, -263, -263, -263, -263, -263,
 -110,    0, -110,    0,    0,    0,    0, -110,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  410,
  438,  466,    0,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  148,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  494,    0,    0,    0, -196,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -235, -193,  654,  669,  510,  526,    0,    0,    0,  684,
  699,  714,  729,  542,  558,  574,  590,  606,  622,  638,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
   -3,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   78,
};
#define YYTABLESIZE 1107
static const YYINT yytable[] = {                         85,
   87,   86,    3,   88,   89,   90,   91,    4,   92,   93,
    5,    6,   94,    7,    8,  115,  116,  117,    9,   10,
   11,   12,   83,   83,   83,   95,   96,   83,   83,   97,
   98,   13,   14,   15,   16,   17,   18,   19,   20,   21,
   22,   23,   24,   25,   26,   27,   99,  100,   28,   29,
   30,   31,   78,   79,   80,   81,   82,   83,   84,  109,
  110,   17,   17,   17,   82,   82,   82,  111,  112,   82,
   82,  101,  102,  113,  114,  115,  116,  117,  118,  119,
  120,  121,  103,  131,  132,  133,  104,  134,  135,  136,
  137,  105,  139,  140,  141,  142,  143,  144,  145,  146,
  147,  148,  149,  150,  106,  151,  152,  153,  154,  155,
  156,  157,  158,  159,  160,  161,  162,  163,  107,  164,
  165,  166,  167,  168,  169,  170,  111,  112,  123,  124,
  125,  126,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  113,  114,  115,  116,  117,    3,    2,  127,  128,
  129,    4,  138,  122,    5,    6,    0,    7,    8,    0,
    0,    0,    9,   10,   11,   12,    0,  190,    0,  191,
    0,    0,    0,    0,  192,   13,   14,   15,   16,   17,
   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
    0,  130,   28,   29,   30,   31,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  172,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  174,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  175,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  176,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  177,    0,    0,    0,    0,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  179,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  180,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  181,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  182,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  183,    0,    0,    0,    0,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  184,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  185,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  186,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  187,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  188,    0,    0,    0,    0,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  189,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  193,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  194,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  195,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,   42,   42,   42,    0,    0,   42,   42,    0,    0,
    0,    0,    0,    0,   42,   42,    0,   86,   86,   86,
   42,   42,   86,   86,    0,   42,   42,   42,   42,    0,
   86,   86,    0,   87,   87,   87,   86,   86,   87,   87,
    0,   86,   86,   86,   86,    0,   87,   87,    0,   59,
   59,   59,   87,   87,   59,   59,    0,   87,   87,   87,
   87,    0,   59,   59,    0,   60,   60,   60,   59,   59,
   60,   60,    0,   59,   59,   59,   59,    0,   60,   60,
    0,   61,   61,   61,   60,   60,   61,   61,    0,   60,
   60,   60,   60,    0,   61,   61,    0,   65,   65,   65,
   61,   61,   65,   65,    0,   61,   61,   61,   61,    0,
   65,   65,    0,   63,   63,   63,   65,   65,   63,   63,
    0,   65,   65,   65,   65,    0,   63,   63,    0,   62,
   62,   62,   63,   63,   62,   62,    0,   63,   63,   63,
   63,    0,   62,   62,    0,   64,   64,   64,   62,   62,
   64,   64,    0,   62,   62,   62,   62,    0,   64,   64,
    0,   84,   84,   84,   64,   64,   84,   84,    0,   64,
   64,   64,   64,    0,   84,   84,   85,   85,   85,    0,
    0,   85,   85,    0,    0,   84,   84,   84,   84,   85,
   85,   91,   91,   91,    0,    0,   91,   91,    0,    0,
   85,   85,   85,   85,   91,   91,   92,   92,   92,    0,
    0,   92,   92,    0,    0,   91,   91,   91,   91,   92,
   92,   93,   93,   93,    0,    0,   93,   93,    0,    0,
   92,   92,   92,   92,   93,   93,   94,   94,   94,    0,
    0,   94,   94,    0,    0,   93,   93,   93,   93,   94,
   94,    0,  108,    0,    0,    0,  109,  110,    0,    0,
   94,   94,   94,   94,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  171,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  173,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  178,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,
};
static const YYINT yycheck[] = {                          3,
  257,    5,  257,  257,  257,    9,  257,  262,  257,  257,
  265,  266,  257,  268,  269,  279,  280,  281,  273,  274,
  275,  276,  258,  259,  260,  257,  257,  263,  264,  257,
  257,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  257,  257,  303,  304,
  305,  306,  307,  308,  309,  310,  311,  312,  313,  263,
  264,  258,  259,  260,  258,  259,  260,  271,  272,  263,
  264,  257,  257,  277,  278,  279,  280,  281,  282,  283,
  284,  285,  257,   87,   88,   89,  257,   91,   92,   93,
   94,  257,   96,   97,   98,   99,  100,  101,  102,  103,
  104,  105,  106,  107,  257,  109,  110,  111,  112,  113,
  114,  115,  116,  117,  118,  119,  120,  121,  257,  123,
  124,  125,  126,  127,  128,  129,  271,  272,  258,  258,
  258,  258,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  277,  278,  279,  280,  281,  257,    0,  258,  258,
  258,  262,  258,   76,  265,  266,   -1,  268,  269,   -1,
   -1,   -1,  273,  274,  275,  276,   -1,  171,   -1,  173,
   -1,   -1,   -1,   -1,  178,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
   -1,  258,  303,  304,  305,  306,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,  259,  260,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,  258,  259,  260,
  277,  278,  263,  264,   -1,  282,  283,  284,  285,   -1,
  271,  272,   -1,  258,  259,  260,  277,  278,  263,  264,
   -1,  282,  283,  284,  285,   -1,  271,  272,   -1,  258,
  259,  260,  277,  278,  263,  264,   -1,  282,  283,  284,
  285,   -1,  271,  272,   -1,  258,  259,  260,  277,  278,
  263,  264,   -1,  282,  283,  284,  285,   -1,  271,  272,
   -1,  258,  259,  260,  277,  278,  263,  264,   -1,  282,
  283,  284,  285,   -1,  271,  272,   -1,  258,  259,  260,
  277,  278,  263,  264,   -1,  282,  283,  284,  285,   -1,
  271,  272,   -1,  258,  259,  260,  277,  278,  263,  264,
   -1,  282,  283,  284,  285,   -1,  271,  272,   -1,  258,
  259,  260,  277,  278,  263,  264,   -1,  282,  283,  284,
  285,   -1,  271,  272,   -1,  258,  259,  260,  277,  278,
  263,  264,   -1,  282,  283,  284,  285,   -1,  271,  272,
   -1,  258,  259,  260,  277,  278,  263,  264,   -1,  282,
  283,  284,  285,   -1,  271,  272,  258,  259,  260,   -1,
   -1,  263,  264,   -1,   -1,  282,  283,  284,  285,  271,
  272,  258,  259,  260,   -1,   -1,  263,  264,   -1,   -1,
  282,  283,  284,  285,  271,  272,  258,  259,  260,   -1,
   -1,  263,  264,   -1,   -1,  282,  283,  284,  285,  271,
  272,  258,  259,  260,   -1,   -1,  263,  264,   -1,   -1,
  282,  283,  284,  285,  271,  272,  258,  259,  260,   -1,
   -1,  263,  264,   -1,   -1,  282,  283,  284,  285,  271,
  272,   -1,  259,   -1,   -1,   -1,  263,  264,   -1,   -1,
  282,  283,  284,  285,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  260,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  260,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  260,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 315
#define YYUNDFTOKEN 365
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","LPAR","RPAR","SEMICOLON","COMMA",
"PERCENT","DIGIT","OR","AND","NOT","MUTUAL_KENDRA","DESCR","LORDOF",
"ISPLANETINHOUSE","ISPLANETINRASI","ISEQUAL","ISNOTEQUAL","RETURN","PLANET",
"GETRASI","GETBHAVA","PLUS","MINUS","MULT","DIV","MOD","LT","LE","GT","GE",
"PLANETS_INSIGN","PLANETS_INHOUSE","NUMBER_OF_OCCUPIED_SIGNS","GRAHA_DRISHTI",
"GETLORD","ISBENEFIC","ISMALEFIC","ISPLANETINKENDRA","ISPLANETINAPOKLIMA",
"ISPLANETINPANAPHARA","ISPLANETINDUALRASI","ISPLANETINFIXEDRASI",
"ISPLANETINMOVABLERASI","RED12","GETNAKSHATRA","RED27","RASIONLY","HOUSE",
"SIGN","NAKSHATRA","BOOLEAN","TYPE_INT","TYPE_DOUBLE","TYPE_PLANET","TYPE_SIGN",
"TYPE_NAKSHATRA","TYPE_HOUSE","TYPE_BOOLEAN","IDENT","STRING","$accept",
"rule_decl","exp","return_exp","unary_exp","dual_exp","lordof_exp","and_exp",
"or_exp","housematch_exp","getrasi_exp","getbhava_exp","isequal_exp",
"isnotequal_exp","plus_exp","minus_exp","div_exp","mult_exp","mod_exp",
"mutual_kendra_exp","planetsinsign_exp","planetsinhouse_exp","grahadrishti_exp",
"getlord_exp","isbenefic_exp","ismalefic_exp","isplanetinkendra_exp",
"isplanetinapoklima_exp","isplanetinpanaphara_exp","lt_exp","le_exp","gt_exp",
"ge_exp","isplanetindualrasi_exp","isplanetinfixedrasi_exp",
"isplanetinmovablerasi_exp","not_exp","red12_exp","getnakshatra_exp",
"typecast_int_exp","typecast_double_exp","typecast_planet_exp",
"typecast_house_exp","typecast_nakshatra_exp","typecast_boolean_exp",
"typecast_sign_exp","exp_list","$$1","stmt","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : rule_decl",
"$$1 :",
"rule_decl : $$1 exp_list",
"exp_list : stmt",
"exp_list : exp_list stmt",
"stmt : exp SEMICOLON",
"exp : unary_exp",
"exp : dual_exp",
"exp : return_exp",
"exp : LPAR exp RPAR",
"exp : NUMBER_OF_OCCUPIED_SIGNS LPAR RPAR",
"exp : DIGIT",
"exp : HOUSE",
"exp : PLANET",
"exp : SIGN",
"exp : NAKSHATRA",
"exp : BOOLEAN",
"return_exp : RETURN exp",
"unary_exp : not_exp",
"unary_exp : lordof_exp",
"unary_exp : getbhava_exp",
"unary_exp : getrasi_exp",
"unary_exp : getlord_exp",
"unary_exp : planetsinsign_exp",
"unary_exp : planetsinhouse_exp",
"unary_exp : ismalefic_exp",
"unary_exp : isbenefic_exp",
"unary_exp : isplanetinkendra_exp",
"unary_exp : isplanetinapoklima_exp",
"unary_exp : isplanetinpanaphara_exp",
"unary_exp : isplanetindualrasi_exp",
"unary_exp : isplanetinfixedrasi_exp",
"unary_exp : isplanetinmovablerasi_exp",
"unary_exp : red12_exp",
"unary_exp : getnakshatra_exp",
"unary_exp : typecast_int_exp",
"unary_exp : typecast_double_exp",
"unary_exp : typecast_planet_exp",
"unary_exp : typecast_house_exp",
"unary_exp : typecast_nakshatra_exp",
"unary_exp : typecast_boolean_exp",
"unary_exp : typecast_sign_exp",
"not_exp : NOT exp",
"red12_exp : RED12 LPAR exp RPAR",
"lordof_exp : LORDOF LPAR exp RPAR",
"getrasi_exp : GETRASI LPAR exp RPAR",
"getlord_exp : GETLORD LPAR exp RPAR",
"getbhava_exp : GETBHAVA LPAR exp RPAR",
"planetsinsign_exp : PLANETS_INSIGN LPAR exp RPAR",
"planetsinhouse_exp : PLANETS_INHOUSE LPAR exp RPAR",
"getnakshatra_exp : GETNAKSHATRA LPAR exp RPAR",
"isbenefic_exp : ISBENEFIC LPAR exp RPAR",
"ismalefic_exp : ISMALEFIC LPAR exp RPAR",
"isplanetinkendra_exp : ISPLANETINKENDRA LPAR exp RPAR",
"isplanetinapoklima_exp : ISPLANETINAPOKLIMA LPAR exp RPAR",
"isplanetinpanaphara_exp : ISPLANETINPANAPHARA LPAR exp RPAR",
"isplanetindualrasi_exp : ISPLANETINDUALRASI LPAR exp RPAR",
"isplanetinfixedrasi_exp : ISPLANETINFIXEDRASI LPAR exp RPAR",
"isplanetinmovablerasi_exp : ISPLANETINMOVABLERASI LPAR exp RPAR",
"typecast_int_exp : LPAR TYPE_INT RPAR exp",
"typecast_double_exp : LPAR TYPE_DOUBLE RPAR exp",
"typecast_planet_exp : LPAR TYPE_PLANET RPAR exp",
"typecast_house_exp : LPAR TYPE_HOUSE RPAR exp",
"typecast_nakshatra_exp : LPAR TYPE_NAKSHATRA RPAR exp",
"typecast_boolean_exp : LPAR TYPE_BOOLEAN RPAR exp",
"typecast_sign_exp : LPAR TYPE_SIGN RPAR exp",
"dual_exp : housematch_exp",
"dual_exp : and_exp",
"dual_exp : or_exp",
"dual_exp : isequal_exp",
"dual_exp : isnotequal_exp",
"dual_exp : plus_exp",
"dual_exp : minus_exp",
"dual_exp : div_exp",
"dual_exp : mult_exp",
"dual_exp : mod_exp",
"dual_exp : mutual_kendra_exp",
"dual_exp : grahadrishti_exp",
"dual_exp : lt_exp",
"dual_exp : le_exp",
"dual_exp : gt_exp",
"dual_exp : ge_exp",
"and_exp : exp AND exp",
"or_exp : exp OR exp",
"isequal_exp : exp ISEQUAL exp",
"isnotequal_exp : exp ISNOTEQUAL exp",
"plus_exp : exp PLUS exp",
"minus_exp : exp MINUS exp",
"mult_exp : exp MULT exp",
"div_exp : exp DIV exp",
"mod_exp : exp MOD exp",
"lt_exp : exp LT exp",
"le_exp : exp LE exp",
"gt_exp : exp GT exp",
"ge_exp : exp GE exp",
"mutual_kendra_exp : MUTUAL_KENDRA LPAR exp COMMA exp RPAR",
"housematch_exp : ISPLANETINHOUSE LPAR exp COMMA exp RPAR",
"grahadrishti_exp : GRAHA_DRISHTI LPAR exp COMMA exp RPAR",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 153 "MdlParser.yy"
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

#line 935 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 78 "MdlParser.yy"
	{ rule = new Rule; }
#line 1608 "y.tab.c"
break;
case 5:
#line 82 "MdlParser.yy"
	{ rule->addExpression( yystack.l_mark[-1].expval ); }
#line 1613 "y.tab.c"
break;
case 9:
#line 85 "MdlParser.yy"
	{ yyval.expval = yystack.l_mark[-1].expval; }
#line 1618 "y.tab.c"
break;
case 10:
#line 86 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_NUMBER_OF_OCCUPIED_SIGNS ); }
#line 1623 "y.tab.c"
break;
case 11:
#line 87 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_CONST, yystack.l_mark[0].ival ); }
#line 1628 "y.tab.c"
break;
case 12:
#line 88 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_HOUSE, yystack.l_mark[0].ival ); }
#line 1633 "y.tab.c"
break;
case 13:
#line 89 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_PLANET, yystack.l_mark[0].ival ); }
#line 1638 "y.tab.c"
break;
case 14:
#line 90 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_SIGN, yystack.l_mark[0].ival ); }
#line 1643 "y.tab.c"
break;
case 15:
#line 91 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_NAKSHATRA, yystack.l_mark[0].ival ); }
#line 1648 "y.tab.c"
break;
case 16:
#line 92 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_BOOLEAN, yystack.l_mark[0].ival ); }
#line 1653 "y.tab.c"
break;
case 17:
#line 95 "MdlParser.yy"
	{ yyval.expval = new ReturnExpression( yystack.l_mark[0].expval ); }
#line 1658 "y.tab.c"
break;
case 42:
#line 103 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_NOT ); }
#line 1663 "y.tab.c"
break;
case 43:
#line 104 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_RED12 ); }
#line 1668 "y.tab.c"
break;
case 44:
#line 105 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_LORDOFHOUSE ); }
#line 1673 "y.tab.c"
break;
case 45:
#line 106 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_GETRASI ); }
#line 1678 "y.tab.c"
break;
case 46:
#line 107 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_GETLORD ); }
#line 1683 "y.tab.c"
break;
case 47:
#line 108 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_GETBHAVA ); }
#line 1688 "y.tab.c"
break;
case 48:
#line 109 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_PLANETS_INSIGN ); }
#line 1693 "y.tab.c"
break;
case 49:
#line 110 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_PLANETS_INHOUSE ); }
#line 1698 "y.tab.c"
break;
case 50:
#line 111 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_GETNAKSHATRA ); }
#line 1703 "y.tab.c"
break;
case 51:
#line 112 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISBENEFIC ); }
#line 1708 "y.tab.c"
break;
case 52:
#line 113 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISMALEFIC ); }
#line 1713 "y.tab.c"
break;
case 53:
#line 115 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISPLANETINKENDRA ); }
#line 1718 "y.tab.c"
break;
case 54:
#line 116 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISPLANETINAPOKLIMA ); }
#line 1723 "y.tab.c"
break;
case 55:
#line 117 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISPLANETINPANAPHARA ); }
#line 1728 "y.tab.c"
break;
case 56:
#line 118 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISPLANETINDUALRASI ); }
#line 1733 "y.tab.c"
break;
case 57:
#line 119 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISPLANETINFIXEDRASI ); }
#line 1738 "y.tab.c"
break;
case 58:
#line 120 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[-1].expval, E1_ISPLANETINMOVABLERASI ); }
#line 1743 "y.tab.c"
break;
case 59:
#line 122 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_INT ); }
#line 1748 "y.tab.c"
break;
case 60:
#line 123 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_DOUBLE ); }
#line 1753 "y.tab.c"
break;
case 61:
#line 124 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_PLANET ); }
#line 1758 "y.tab.c"
break;
case 62:
#line 125 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_HOUSE ); }
#line 1763 "y.tab.c"
break;
case 63:
#line 126 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_NAKSHATRA ); }
#line 1768 "y.tab.c"
break;
case 64:
#line 127 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_BOOLEAN ); }
#line 1773 "y.tab.c"
break;
case 65:
#line 128 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yystack.l_mark[0].expval, E1_TYPECAST_SIGN ); }
#line 1778 "y.tab.c"
break;
case 82:
#line 134 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_LOGICAL_AND ); }
#line 1783 "y.tab.c"
break;
case 83:
#line 135 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_LOGICAL_OR ); }
#line 1788 "y.tab.c"
break;
case 84:
#line 136 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_ISEQUAL ); }
#line 1793 "y.tab.c"
break;
case 85:
#line 137 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_ISNOTEQUAL ); }
#line 1798 "y.tab.c"
break;
case 86:
#line 138 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_PLUS ); }
#line 1803 "y.tab.c"
break;
case 87:
#line 139 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_MINUS ); }
#line 1808 "y.tab.c"
break;
case 88:
#line 140 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_MULT ); }
#line 1813 "y.tab.c"
break;
case 89:
#line 141 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_DIV ); }
#line 1818 "y.tab.c"
break;
case 90:
#line 142 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_MOD ); }
#line 1823 "y.tab.c"
break;
case 91:
#line 143 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_LT ); }
#line 1828 "y.tab.c"
break;
case 92:
#line 144 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_LE ); }
#line 1833 "y.tab.c"
break;
case 93:
#line 145 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_GT ); }
#line 1838 "y.tab.c"
break;
case 94:
#line 146 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-2].expval, yystack.l_mark[0].expval, E2_GE ); }
#line 1843 "y.tab.c"
break;
case 95:
#line 147 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-3].expval, yystack.l_mark[-1].expval, E2_MUTUAL_KENDRA ); }
#line 1848 "y.tab.c"
break;
case 96:
#line 148 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-3].expval, yystack.l_mark[-1].expval, E2_PLANETINHOUSE ); }
#line 1853 "y.tab.c"
break;
case 97:
#line 149 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yystack.l_mark[-3].expval, yystack.l_mark[-1].expval, E2_GRAHA_DRISHTI ); }
#line 1858 "y.tab.c"
break;
#line 1860 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
