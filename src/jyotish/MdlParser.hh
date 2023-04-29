#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
  int ival;
	char *sval;
	double dval;
	Expression *expval;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
