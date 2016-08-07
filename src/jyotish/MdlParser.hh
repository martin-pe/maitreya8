/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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
#line 45 "MdlParser.yy" /* yacc.c:1909  */

  int ival;
	char *sval;
	double dval;
	Expression *expval;

#line 183 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
