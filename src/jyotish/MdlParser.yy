/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/MdlParser.yy
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
************************************************************************/

%{

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

%}

%union {
  int ival;
	char *sval;
	double dval;
	Expression *expval;
};

%token <ival> LPAR RPAR SEMICOLON COMMA PERCENT DIGIT OR AND NOT MUTUAL_KENDRA
%token <ival> DESCR LORDOF ISPLANETINHOUSE ISPLANETINRASI ISEQUAL ISNOTEQUAL
%token <ival> RETURN PLANET GETRASI GETBHAVA PLUS MINUS MULT DIV MOD LT LE GT GE
%token <ival> PLANETS_INSIGN PLANETS_INHOUSE NUMBER_OF_OCCUPIED_SIGNS GRAHA_DRISHTI GETLORD
%token <ival> ISBENEFIC ISMALEFIC ISPLANETINKENDRA ISPLANETINAPOKLIMA ISPLANETINPANAPHARA
%token <ival> ISPLANETINDUALRASI ISPLANETINFIXEDRASI ISPLANETINMOVABLERASI RED12 GETNAKSHATRA RED27
%token <ival> RASIONLY HOUSE SIGN NAKSHATRA BOOLEAN 
%token <ival> TYPE_INT TYPE_DOUBLE TYPE_PLANET TYPE_SIGN TYPE_NAKSHATRA TYPE_HOUSE TYPE_BOOLEAN

%token <sval> IDENT STRING

%type <expval> exp return_exp unary_exp dual_exp lordof_exp and_exp or_exp housematch_exp getrasi_exp getbhava_exp
%type <expval> isequal_exp isnotequal_exp plus_exp minus_exp div_exp mult_exp mod_exp mutual_kendra_exp 
%type <expval> planetsinsign_exp planetsinhouse_exp grahadrishti_exp getlord_exp isbenefic_exp ismalefic_exp
%type <expval> isplanetinkendra_exp isplanetinapoklima_exp isplanetinpanaphara_exp lt_exp le_exp gt_exp ge_exp
%type <expval> isplanetindualrasi_exp isplanetinfixedrasi_exp isplanetinmovablerasi_exp not_exp red12_exp getnakshatra_exp
%type <expval> typecast_int_exp typecast_double_exp typecast_planet_exp typecast_house_exp typecast_nakshatra_exp typecast_boolean_exp typecast_sign_exp


%left RETURN
%left AND OR
%left ISEQUAL ISNOTEQUAL LT LE GT GE
%left PLUS MINUS
%left LPAR RPAR
%left NOT
%left MULT DIV MOD

%%
rule_decl : { rule = new Rule; } exp_list;

exp_list: stmt | exp_list stmt;

stmt : exp SEMICOLON { rule->addExpression( $1 ); };

exp : unary_exp | dual_exp | return_exp
	| LPAR exp RPAR { $$ = $2; }
	| NUMBER_OF_OCCUPIED_SIGNS LPAR RPAR { $$ = new NumericExpression( ED_NUMBER_OF_OCCUPIED_SIGNS ); }
	| DIGIT { $$ = new NumericExpression( ED_CONST, $1 ); }
	| HOUSE { $$ = new NumericExpression( ED_HOUSE, $1 ); }
	| PLANET { $$ = new NumericExpression( ED_PLANET, $1 ); }
	| SIGN { $$ = new NumericExpression( ED_SIGN, $1 ); }
	| NAKSHATRA { $$ = new NumericExpression( ED_NAKSHATRA, $1 ); }
	| BOOLEAN { $$ = new NumericExpression( ED_BOOLEAN, $1 ); }
;

return_exp : RETURN exp { $$ = new ReturnExpression( $2 ); };

unary_exp : not_exp | lordof_exp | getbhava_exp | getrasi_exp | getlord_exp | planetsinsign_exp | planetsinhouse_exp
	| ismalefic_exp | isbenefic_exp | isplanetinkendra_exp | isplanetinapoklima_exp | isplanetinpanaphara_exp
	| isplanetindualrasi_exp | isplanetinfixedrasi_exp | isplanetinmovablerasi_exp | red12_exp | getnakshatra_exp
	| typecast_int_exp | typecast_double_exp | typecast_planet_exp | typecast_house_exp | typecast_nakshatra_exp | typecast_boolean_exp | typecast_sign_exp
;

not_exp            : NOT exp { $$ = new UnaryExpression( $2, E1_NOT ); };
red12_exp          : RED12 LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_RED12 ); };
lordof_exp         : LORDOF LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_LORDOFHOUSE ); };
getrasi_exp        : GETRASI LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_GETRASI ); };
getlord_exp        : GETLORD LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_GETLORD ); };
getbhava_exp       : GETBHAVA LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_GETBHAVA ); };
planetsinsign_exp  : PLANETS_INSIGN LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_PLANETS_INSIGN ); };
planetsinhouse_exp : PLANETS_INHOUSE LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_PLANETS_INHOUSE ); };
getnakshatra_exp   : GETNAKSHATRA LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_GETNAKSHATRA ); };
isbenefic_exp      : ISBENEFIC LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISBENEFIC ); };
ismalefic_exp      : ISMALEFIC LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISMALEFIC ); };

isplanetinkendra_exp      : ISPLANETINKENDRA LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISPLANETINKENDRA ); };
isplanetinapoklima_exp    : ISPLANETINAPOKLIMA LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISPLANETINAPOKLIMA ); };
isplanetinpanaphara_exp   : ISPLANETINPANAPHARA LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISPLANETINPANAPHARA ); };
isplanetindualrasi_exp    : ISPLANETINDUALRASI LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISPLANETINDUALRASI ); };
isplanetinfixedrasi_exp   : ISPLANETINFIXEDRASI LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISPLANETINFIXEDRASI ); };
isplanetinmovablerasi_exp : ISPLANETINMOVABLERASI LPAR exp RPAR { $$ = new UnaryExpression( $3, E1_ISPLANETINMOVABLERASI ); };

typecast_int_exp       : LPAR TYPE_INT       RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_INT ); };
typecast_double_exp    : LPAR TYPE_DOUBLE    RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_DOUBLE ); };
typecast_planet_exp    : LPAR TYPE_PLANET    RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_PLANET ); };
typecast_house_exp     : LPAR TYPE_HOUSE     RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_HOUSE ); };
typecast_nakshatra_exp : LPAR TYPE_NAKSHATRA RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_NAKSHATRA ); };
typecast_boolean_exp   : LPAR TYPE_BOOLEAN   RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_BOOLEAN ); };
typecast_sign_exp      : LPAR TYPE_SIGN      RPAR exp { $$ = new UnaryExpression( $4, E1_TYPECAST_SIGN ); };

dual_exp: housematch_exp | and_exp | or_exp | isequal_exp | isnotequal_exp | plus_exp | minus_exp | div_exp | mult_exp
	| mod_exp | mutual_kendra_exp | grahadrishti_exp | lt_exp | le_exp | gt_exp | ge_exp
;

and_exp            : exp AND exp { $$ = new DualExpression( $1, $3, E2_LOGICAL_AND ); } ;
or_exp             : exp OR exp { $$ = new DualExpression( $1, $3, E2_LOGICAL_OR ); } ;
isequal_exp        : exp ISEQUAL exp { $$ = new DualExpression( $1, $3, E2_ISEQUAL ); } ;
isnotequal_exp     : exp ISNOTEQUAL exp { $$ = new DualExpression( $1, $3, E2_ISNOTEQUAL ); } ;
plus_exp           : exp PLUS exp { $$ = new DualExpression( $1, $3, E2_PLUS ); } ;
minus_exp          : exp MINUS exp { $$ = new DualExpression( $1, $3, E2_MINUS ); } ;
mult_exp           : exp MULT exp { $$ = new DualExpression( $1, $3, E2_MULT ); } ;
div_exp            : exp DIV exp { $$ = new DualExpression( $1, $3, E2_DIV ); } ;
mod_exp            : exp MOD exp { $$ = new DualExpression( $1, $3, E2_MOD ); } ;
lt_exp             : exp LT exp { $$ = new DualExpression( $1, $3, E2_LT ); } ;
le_exp             : exp LE exp { $$ = new DualExpression( $1, $3, E2_LE ); } ;
gt_exp             : exp GT exp { $$ = new DualExpression( $1, $3, E2_GT ); } ;
ge_exp             : exp GE exp { $$ = new DualExpression( $1, $3, E2_GE ); } ;
mutual_kendra_exp  : MUTUAL_KENDRA LPAR exp COMMA exp RPAR { $$ = new DualExpression( $3, $5, E2_MUTUAL_KENDRA ); };
housematch_exp     : ISPLANETINHOUSE LPAR exp COMMA exp RPAR { $$ = new DualExpression( $3, $5, E2_PLANETINHOUSE ); } ;
grahadrishti_exp   : GRAHA_DRISHTI LPAR exp COMMA exp RPAR { $$ = new DualExpression( $3, $5, E2_GRAHA_DRISHTI ); } ;


%%
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

