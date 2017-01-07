/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/MdlLexer.ll
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2017 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

%option 8bit
%option yylineno
%option noyywrap
%option caseless

%{
#include "Conf.h"
#include "maitreya.h"
#include "Expression.h"
#include "MdlParser.hh"

extern Config *config;

%}

DIGIT   [0-9]+
IDENT   [A-Za-z0-9_.]+
WS      [ 	]+
WSE      [ \t\n]*
%s COMMENT

%%

^[ \t]*"/*" { BEGIN COMMENT; }
^[ \t]*"/*".*"*/"[ \t]*\n  {}

<COMMENT>"*/"[ \t]*\n { BEGIN 0; }
<COMMENT>"*/" { BEGIN 0;  }
<COMMENT>\n {}
<COMMENT>.*\n {}

"//".*\n {}

"lordOf" return LORDOF;
"getRasi" return GETRASI;
"getBhava" return GETBHAVA;
"getLord" return GETLORD;
"getNakshatra" return GETNAKSHATRA;

"isPlanetInHouse" return ISPLANETINHOUSE;
"isPlanetInRasi" return ISPLANETINRASI;
"isPlanetInKendra" return ISPLANETINKENDRA;
"isPlanetInApoklima" return ISPLANETINAPOKLIMA;
"isPlanetInPanaphara" return ISPLANETINPANAPHARA;
"isPlanetInDualRasi" return ISPLANETINDUALRASI;
"isPlanetInFixedRasi" return ISPLANETINFIXEDRASI;
"isPlanetInMovableRasi" return ISPLANETINMOVABLERASI;
"isBenefic" return ISBENEFIC;
"isMalefic" return ISMALEFIC;
"isMutualKendra" return MUTUAL_KENDRA;
"isGrahaDrishti" return GRAHA_DRISHTI;

"countPlanetsInSign" return PLANETS_INSIGN;
"countPlanetsInHouse" return PLANETS_INHOUSE;
"countNumberOfOccupiedSigns" return NUMBER_OF_OCCUPIED_SIGNS;

"Sun" { yylval.ival = OSUN; return PLANET; }
"Moon" { yylval.ival = OMOON; return PLANET; }
"Mars" { yylval.ival = OMARS; return PLANET; }
"Mercury" { yylval.ival = OMERCURY; return PLANET; }
"Jupiter" { yylval.ival = OJUPITER; return PLANET; }
"Venus" { yylval.ival = OVENUS; return PLANET; }
"Saturn" { yylval.ival = OSATURN; return PLANET; }
"Rahu" { yylval.ival = ( config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE ); return PLANET; }
"Ketu" { yylval.ival = ( config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE ); return PLANET; }

"Dhuma" { yylval.ival = ODHUMA; return PLANET; }
"Vyatipata" { yylval.ival = OVYATIPATA; return PLANET; }
"Parivesha" { yylval.ival = OPARIVESHA; return PLANET; }
"Chapa" { yylval.ival = OCHAPA; return PLANET; }
"Upaketu" { yylval.ival = OUPAKETU; return PLANET; }
"Kala" { yylval.ival = OKALA; return PLANET; }
"Mrityu" { yylval.ival = OMRITYU; return PLANET; }
"Ardhaprahara" { yylval.ival = OARDHAPRAHARA; return PLANET; }
"Yamaghantaka" { yylval.ival = OYAMAGHANTAKA; return PLANET; }
"Gulika" { yylval.ival = OGULIKA; return PLANET; }
"Mandi" { yylval.ival = OMANDI; return PLANET; }

"Aries" { yylval.ival = R_ARIES; return SIGN; }
"Taurus" { yylval.ival = R_TAURUS; return SIGN; }
"Gemini" { yylval.ival = R_GEMINI; return SIGN; }
"Cancer" { yylval.ival = R_CANCER; return SIGN; }
"Leo" { yylval.ival = R_LEO; return SIGN; }
"Virgo" { yylval.ival = R_VIRGO; return SIGN; }
"Libra" { yylval.ival = R_LIBRA; return SIGN; }
"Scorpio" { yylval.ival = R_SCORPIO; return SIGN; }
"Sagittarius" { yylval.ival = R_SAGITTARIUS; return SIGN; }
"Capricorn" { yylval.ival = R_CAPRICORN; return SIGN; }
"Aquarius" { yylval.ival = R_AQUARIUS; return SIGN; }
"Pisces" { yylval.ival = R_PISCES; return SIGN; }

"House1" { yylval.ival = HOUSE1; return HOUSE; }
"House2" { yylval.ival = HOUSE2; return HOUSE; }
"House3" { yylval.ival = HOUSE3; return HOUSE; }
"House4" { yylval.ival = HOUSE4; return HOUSE; }
"House5" { yylval.ival = HOUSE5; return HOUSE; }
"House6" { yylval.ival = HOUSE6; return HOUSE; }
"House7" { yylval.ival = HOUSE7; return HOUSE; }
"House8" { yylval.ival = HOUSE8; return HOUSE; }
"House9" { yylval.ival = HOUSE9; return HOUSE; }
"House10" { yylval.ival = HOUSE10; return HOUSE; }
"House11" { yylval.ival = HOUSE11; return HOUSE; }
"House12" { yylval.ival = HOUSE12; return HOUSE; }

"Lagna" { yylval.ival = OASCENDANT; return PLANET; }
"Ascendant" { yylval.ival = OASCENDANT; return PLANET; }
"MC" { yylval.ival = HOUSE10; return PLANET; }
"MediumCoeli" { yylval.ival = HOUSE10; return PLANET; }
"Descendant" { yylval.ival = HOUSE7; return PLANET; }
"IC" { yylval.ival = HOUSE4; return PLANET; }
"ImumCoeli" { yylval.ival = HOUSE4; return PLANET; }

"Aswini" { yylval.ival = N27_ASWINI; return NAKSHATRA; }
"Bharani" { yylval.ival = N27_BHARANI; return NAKSHATRA; }
"Krittika" { yylval.ival = N27_KRITTIKA; return NAKSHATRA; }
"Rohini" { yylval.ival = N27_ROHINI; return NAKSHATRA; }
"Mrigasira" { yylval.ival = N27_MRIGASIRA; return NAKSHATRA; }
"Arudra" { yylval.ival = N27_ARUDRA; return NAKSHATRA; }
"Punarvasu" { yylval.ival = N27_PUNARVASU; return NAKSHATRA; }
"Pushyami" { yylval.ival = N27_PUSHYAMI; return NAKSHATRA; }
"Aslesha" { yylval.ival = N27_ASLESHA; return NAKSHATRA; }
"Makha" { yylval.ival = N27_MAKHA; return NAKSHATRA; }
"PurvaPhalgini" { yylval.ival = N27_PPHALGUNI; return NAKSHATRA; }
"UttaraPhalguni" { yylval.ival = N27_UPHALGUNI; return NAKSHATRA; }
"Hashta" { yylval.ival = N27_HASTHA; return NAKSHATRA; }
"Chitta" { yylval.ival = N27_CHITTA; return NAKSHATRA; }
"Swati" { yylval.ival = N27_SWATI; return NAKSHATRA; }
"Vishakha" { yylval.ival = N27_VISAKHA; return NAKSHATRA; }
"Anuradha" { yylval.ival = N27_ANURADHA; return NAKSHATRA; }
"Jeshta" { yylval.ival = N27_JYESHTA; return NAKSHATRA; }
"Moola" { yylval.ival = N27_MULA; return NAKSHATRA; }
"PurvaShadha" { yylval.ival = N27_PSHADHA; return NAKSHATRA; }
"UttaraShadha" { yylval.ival = N27_USHADHA; return NAKSHATRA; }
"Sravana" { yylval.ival = N27_SRAVANA; return NAKSHATRA; }
"Dhanishta" { yylval.ival = N27_DHANISHTA; return NAKSHATRA; }
"Satabishak" { yylval.ival = N27_SATHABISHAK; return NAKSHATRA; }
"PurvaBhadra" { yylval.ival = N27_PBHADRA; return NAKSHATRA; }
"UttaraBhadra" { yylval.ival = N27_UBHADRA; return NAKSHATRA; }
"Revati" { yylval.ival = N27_REVATI; return NAKSHATRA; }

"true" { yylval.ival = 1; return BOOLEAN; }
"yes" { yylval.ival = 1; return BOOLEAN; }
"false" { yylval.ival = 0; return BOOLEAN; }
"no" { yylval.ival = 0; return BOOLEAN; }

"and" return AND;
"or" return OR;
"not" return NOT;

"int" return TYPE_INT;
"double" return TYPE_DOUBLE;
"Planet" return TYPE_PLANET;
"Sign" return TYPE_SIGN;
"Nakshatra" return TYPE_NAKSHATRA;
"House" return TYPE_HOUSE;
"Boolean" return TYPE_BOOLEAN;
"Bool" return TYPE_BOOLEAN;

"(" return LPAR;
")" return RPAR;

"return" return RETURN;
";" return SEMICOLON;
"," return COMMA;

"==" return ISEQUAL;
"!=" return ISNOTEQUAL;

"+" return PLUS;
"-" return MINUS;
"*" return MULT;
"/" return DIV;
"%" return MOD;

"<" return LT;
"<=" return LE;
">" return GT;
">=" return GE;

"red12" return RED12;
"red27" return RED27;

{DIGIT} {
		yylval.ival = atoi( yytext );
		return DIGIT;
	}

\"[^"]*["] { yylval.sval = strdup( yytext+1 ); yylval.sval[ strlen( yylval.sval )-1] = 0; return STRING; }


{IDENT} { yylval.sval = strdup( yytext ); return IDENT; }

"#"[^\n]*"\n" {}

"\n"      {}

{WS}     {}


. {
		fprintf( stderr, "Lexical error at line %d near token \"%s\"", yylineno, yytext );
	}

%%

