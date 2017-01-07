/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Expression.cpp
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

#include "Expression.h"

#include "astrobase.h"
#include "mathbase.h"
#include "MdlInterpreter.h"

extern int yylineno;

//#define _RULE_DEB

/*****************************************************
**
**   ExpressionErrorHandler   ---   Constructor
**
******************************************************/
ExpressionErrorHandler::ExpressionErrorHandler()
{
	hasErrors = false;
}

/*****************************************************
**
**   ExpressionErrorHandler   ---   appendErrors
**
******************************************************/
double ExpressionErrorHandler::appendErrors( ExpressionErrorHandler *e )
{
	assert( e );
	if ( e->hasErrors ) this->hasErrors = true;
	for ( uint i = 0; i < e->errorStack.size(); i++ ) errorStack.push_back( e->errorStack[i] );
	return 0;
}

/*****************************************************
**
**   ExpressionErrorHandler   ---   addError
**
******************************************************/
void ExpressionErrorHandler::addError( wxString s )
{
	if ( s.IsEmpty()) return;
	hasErrors = true;
	errorStack.push_back( s );
}

/*****************************************************
**
**   ExpressionErrorHandler   ---   clearErrors
**
******************************************************/
void ExpressionErrorHandler::clearErrors()
{
	hasErrors = false;
	errorStack.clear();
}

/*****************************************************
**
**   ExpressionErrorHandler   ---   printStackTrace
**
******************************************************/
void ExpressionErrorHandler::printStackTrace()
{
	if ( errorStack.size() > 0 )
	{
		printf( "Error Stack:\n" );
		for ( uint i = 0; i < errorStack.size(); i++ )
		{
			printf( "   %d: %s\n", i+1, str2char( errorStack[i] ));
		}
	}
}

/*****************************************************
**
**   Expression   ---   Constructor
**
******************************************************/
Expression::Expression( const int &type, const int &subtype )
		: lineno( yylineno ),
		type( type ),
		subtype( subtype )
{
	runtimetype = -1;
}

/*****************************************************
**
**   Expression   ---   getRuntimeTypeName
**
******************************************************/
const wxChar *Expression::getRuntimeTypeName( const int &t )
{
	switch ( t )
	{
	case ET_NUMBER:
		return wxT( "[number]" );
		break;
	case ET_HOUSE:
		return wxT( "[house]" );
		break;
	case ET_PLANET:
		return wxT( "[planet]" );
		break;
	case ET_SIGN:
		return wxT( "[sign]" );
		break;
	case ET_BOOLEAN:
		return wxT( "[boolean]" );
		break;
	case ET_NAKSHATRA:
		return wxT( "[nakshatra]" );
		break;

	default:
		return wxT( "<unknown>" );
		break;
	}
}

/*****************************************************
**
**   Expression   ---   addRuntimeTypeError
**
******************************************************/
void Expression::addRuntimeTypeError( const wxChar *item, const int &runtime_type, const int &expected_type )
{
	wxString s;
	s << wxT( "Parameter of " ) << item << wxT( " is " ) << getRuntimeTypeName( runtime_type )
	<< wxT( ", expected type was " ) << getRuntimeTypeName( expected_type );
	addError( s );
}

/*****************************************************
**
**   Expression   ---   addRuntimeTypeError
**
******************************************************/
void Expression::addRuntimeTypeError( const int &param, const wxChar *item, const int &runtime_type, const int &expected_type )
{
	wxString s;
	s << wxT( "Parameter #" ) << param << wxT( "of [" ) << item << wxT( "] is " ) << getRuntimeTypeName( runtime_type )
	<< wxT( ", expected type was " ) << getRuntimeTypeName( expected_type );
	addError( s );
}


/*****************************************************
**
**   ReturnExpression   ---   Constructor
**
******************************************************/
ReturnExpression::ReturnExpression( Expression *e )
		: Expression(  EXPRESSION_RETURN, ER_EXPRESSION )
{
	assert( e );
	this->exp1 = e;
	if ( e->hasErrors ) appendErrors( e );
	runtimetype = e->runtimetype;
}

/*****************************************************
**
**   ReturnExpression   ---   evaluate
**
******************************************************/
double ReturnExpression::evaluate( MdlInterpreter *interpreter )
{
#ifdef _RULE_DEB
	if ( this->exp1 )
		printf( "Evaluate ReturnExpression type %d subtype %d\n", exp1->type, exp1->subtype );
	else
		printf( "Evaluate ReturnExpression/ exp1 not set\n" );
#endif
	interpreter->setReturnStatus( RETURN_YES );
	if ( this->exp1 ) return exp1->evaluate( interpreter );
	else return 0;
}

/*****************************************************
**
**   NumericExpression   ---   Constructor
**
******************************************************/
NumericExpression::NumericExpression( const int &subtype, const double value )
		: Expression( 	EXPRESSION_NUMERIC, subtype )
{
	this->value = value;

	switch ( subtype )
	{
	case ED_CONST:
		runtimetype = ET_NUMBER;
		break;
	case ED_PLANET:
		runtimetype = ET_PLANET;
		break;
	case ED_HOUSE:
		runtimetype = ET_HOUSE;
		break;
	case ED_SIGN:
		runtimetype = ET_SIGN;
		break;
	case ED_NAKSHATRA:
		runtimetype = ET_NAKSHATRA;
		break;
	case ED_BOOLEAN:
		runtimetype = ET_BOOLEAN;
		break;
	case ED_NUMBER_OF_OCCUPIED_SIGNS:
		runtimetype = ET_NUMBER;
		break;
	default:
		addError( wxString::Format( wxT( "Wrong subtype %d for NumericExpression" ), subtype ));
		break;
	}
}

/*****************************************************
**
**   NumericExpression   ---   evaluate
**
******************************************************/
double NumericExpression::evaluate( MdlInterpreter *interpreter )
{
#ifdef _RULE_DEB
	printf( "Evaluate double expression type %d subtype %d\n", type, subtype );
#endif
	if ( subtype == ED_NUMBER_OF_OCCUPIED_SIGNS )
	{
		value = interpreter->getNumberOfOccupiedSigns();
#ifdef _RULE_DEB
		printf( "Evaluate getNumberOfOccupiedSigns, interpreter returned %f\n", value );
#endif
	}
	return value;
}

/*****************************************************
**
**   UnaryExpression   ---   Constructor
**
******************************************************/
UnaryExpression::UnaryExpression( Expression *e, const int &subtype )
		: Expression( EXPRESSION_UNARY, subtype )
{
	assert( e );
	this->exp1 = e;
	if ( e->hasErrors ) appendErrors( e );

	switch ( subtype )
	{
	case E1_LORDOFHOUSE:
		runtimetype = ET_PLANET;
		if ( e->runtimetype != ET_HOUSE ) addRuntimeTypeError( wxT( "lordOfHouse" ), e->runtimetype, ET_HOUSE );
		break;
	case E1_GETRASI:
		runtimetype = ET_SIGN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "getRasi" ), e->runtimetype, ET_PLANET );
		break;
	case E1_GETBHAVA:
		runtimetype = ET_HOUSE;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "getBhava" ), e->runtimetype, ET_PLANET );
		break;
	case E1_PLANETS_INSIGN:
		runtimetype = ET_NUMBER;
		if ( e->runtimetype != ET_SIGN ) addRuntimeTypeError( wxT( "planetsInSign" ), e->runtimetype, ET_SIGN );
		break;
	case E1_PLANETS_INHOUSE:
		runtimetype = ET_NUMBER;
		if ( e->runtimetype != ET_HOUSE ) addRuntimeTypeError( wxT( "planetsInHOuse" ), e->runtimetype, ET_HOUSE );
		break;
	case E1_GETLORD:
		runtimetype = ET_PLANET;
		if ( e->runtimetype != ET_SIGN ) addRuntimeTypeError( wxT( "getLord" ), e->runtimetype, ET_SIGN );
		break;
	case E1_ISBENEFIC:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isBenefic" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISMALEFIC:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isMalefic" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISPLANETINKENDRA:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isPlanetInKendra" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISPLANETINAPOKLIMA:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isPlanetInApoklima" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISPLANETINPANAPHARA:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isPlanetInPanapara" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISPLANETINDUALRASI:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isPlanetInDualRasi" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISPLANETINFIXEDRASI:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "isPlanetInFixedRasi" ), e->runtimetype, ET_PLANET );
		break;
	case E1_ISPLANETINMOVABLERASI:
		runtimetype = ET_BOOLEAN;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "IsPlanetInMovableRasi" ), e->runtimetype, ET_PLANET );
		break;
	case E1_NOT:
		runtimetype = ET_BOOLEAN;
		break;
	case E1_RED12:
		// TODO der erbt, oder?
		runtimetype = ET_NUMBER;
		break;
	case E1_GETNAKSHATRA:
		runtimetype = ET_NAKSHATRA;
		if ( e->runtimetype != ET_PLANET ) addRuntimeTypeError( wxT( "getNakshatra" ), e->runtimetype, ET_PLANET );
		break;
	case E1_TYPECAST_INT:
		runtimetype = ET_NUMBER;
		break;
	case E1_TYPECAST_DOUBLE:
		runtimetype = ET_NUMBER;
		break;
	case E1_TYPECAST_PLANET:
		runtimetype = ET_PLANET;
		break;
	case E1_TYPECAST_SIGN:
		runtimetype = ET_SIGN;
		break;
	case E1_TYPECAST_NAKSHATRA:
		runtimetype = ET_NAKSHATRA;
		break;
	case E1_TYPECAST_HOUSE:
		runtimetype = ET_HOUSE;
		break;
	case E1_TYPECAST_BOOLEAN:
		runtimetype = ET_BOOLEAN;
		break;
	default:
		addError( wxString::Format( wxT( "Wrong subtype %d for unary expression" ), subtype ));
		break;
	}
}

/*****************************************************
**
**   UnaryExpression   ---   Destructor
**
******************************************************/
UnaryExpression::~UnaryExpression()
{
	delete exp1;
}

/*****************************************************
**
**   UnaryExpression   ---   evaluate
**
******************************************************/
double UnaryExpression::evaluate( MdlInterpreter *interpreter )
{
	int val = (int)exp1->evaluate( interpreter );
	double ret = 0;

#ifdef _RULE_DEB
	printf( "Evaluate unary expression type %d subtype %d evaluated is %d\n", exp1->type, exp1->subtype, val );
#endif
	switch ( subtype )
	{
	case E1_NOT:
	{
#ifdef _RULE_DEB
		printf( "Evaluate NOT: param is %d\n", val );
#endif
		ret = ! val;
	}
	break;
	case E1_RED12:
	{
#ifdef _RULE_DEB
		printf( "Evaluate red12: param is %d result %d\n", val, red12( val ));
#endif
		ret = red12( val );
	}
	break;
	case E1_LORDOFHOUSE: // Range: house1 .. house12
	{
		if ( val < HOUSE1 || val > HOUSE12 ) return -1;
		// Bug upto 5.0-dev-3
		//int a = red12( interpreter->getRasi( OASCENDANT ) + val );
		Rasi rasi = (Rasi)red12( interpreter->getLagna() + val );
#ifdef _RULE_DEB
		printf( "Evaluate lordOfHouse: param is %d, rasi is %d and lord is %d\n", val, (int)rasi, getLord( rasi ) );
#endif
		ret = (double)getLord( rasi );
	}
	break;
	case E1_GETRASI: // Range: planet (or house cusp?)
	{
		Rasi rasi = interpreter->getRasi( val );
#ifdef _RULE_DEB
		printf( "Evaluate getRasi: param is %d, rasi is %d\n", val, (int)rasi );
#endif
		ret = (double)rasi;
	}
	break;
	case E1_GETLORD: // Range: sign Aries .. Pisces
	{
		if ( val < R_ARIES || val > R_PISCES ) return -1;
		ObjectId lord = getLord( (Rasi)val );
#ifdef _RULE_DEB
		printf( "Evaluate getLord: param is %d, lord is %d\n", val, (int)lord );
#endif
		ret = (double)lord;
	}
	break;
	case E1_GETBHAVA: // Range: planet
	{
		int a = interpreter->getBhava( val );
#ifdef _RULE_DEB
		printf( "Evaluate getBhava: param is %d, lord is %d\n", val, a );
#endif
		ret = a;
	}
	break;
	case E1_GETNAKSHATRA: // Range: planet
	{
		int a = interpreter->getNakshatra( val );
#ifdef _RULE_DEB
		printf( "Evaluate getNakshatra: param is %d, rasi is %d\n", val, a );
#endif
		ret = a;
	}
	break;
	case E1_PLANETS_INSIGN: // Range: sign Aries .. Pisces
	{
		if ( val < R_ARIES || val > R_PISCES ) return -1;
		int count = 0;
		for ( ObjectId i = OSUN; i <= OSATURN; i++ ) {
			if ( interpreter->getRasi( i ) == val ) count++;
		}
#ifdef _RULE_DEB
		printf( "Evaluae planets in Rasi: param is %d, result is %d\n", val, count );
#endif
		ret = count;
	}
	break;
	case E1_PLANETS_INHOUSE: // Range house1 .. house12
	{
		if ( val < HOUSE1 || val > HOUSE12 ) return -1;
		int count = 0;
		for ( int i = OSUN; i <= OSATURN; i++ ) {
			if ( interpreter->getBhava( i ) == val ) count++;
		}
#ifdef _RULE_DEB
		printf( "Evaluate planets in Bhava: param is %d, result is %d\n", val, count );
#endif
		ret = count;
	}
	break;
	case E1_ISBENEFIC: // Range: planet
	{
		bool b =  interpreter->isBenefic( val );
#ifdef _RULE_DEB
		printf( "Evaluate isbenefic: param is %d, result is %d\n", val, b );
#endif
		ret = b;
	}
	break;
	case E1_ISMALEFIC: // Range: planet
	{
		bool b =  interpreter->isMalefic( val );
#ifdef _RULE_DEB
		printf( "Evaluate ismalefic: param is %d, result is %d\n", val, b );
#endif
		ret = b;
	}
	break;
	case E1_ISPLANETINKENDRA: // Range: planet
	{
		int a = interpreter->getBhava( val ) % 3;
		ret = ( a == 0 );
#ifdef _RULE_DEB
		printf( "Evaluate is planet in kendra: param is %d, result is %d; ret is %d\n", val, a, a == 0 );
#endif
	}
	break;
	case E1_ISPLANETINAPOKLIMA: // Range: planet
	{
		int a = interpreter->getBhava( val ) % 3;
		ret = ( a == 1 );
#ifdef _RULE_DEB
		printf( "Evaluate is planet in apoklima: param is %d, result is %d; ret is %d\n", val, a, a == 1 );
#endif
	}
	break;
	case E1_ISPLANETINPANAPHARA: // Range: planet
	{
		int a = interpreter->getBhava( val ) % 3;
		ret = ( a == 2 );
#ifdef _RULE_DEB
		printf( "Evaluate is planet in panaphara: param is %d, result is %d; ret is %d\n", val, a, a == 2 );
#endif
	}
	break;
	case E1_ISPLANETINMOVABLERASI: // Range: planet
	{
		int a = interpreter->getRasi( val ) % 3;
		ret = ( a == 0 );
#ifdef _RULE_DEB
		printf( "Evaluate is planet in movable rasi: param is %d, result is %d; ret is %d\n", val, a, a == 2 );
#endif
	}
	break;
	case E1_ISPLANETINFIXEDRASI: // Range: planet
	{
		int a = interpreter->getRasi( val ) % 3;
		ret = ( a == 1 );
#ifdef _RULE_DEB
		printf( "Evaluate is planet in fixed rasi: param is %d, result is %d; ret is %d\n", val, a, a == 1 );
#endif
	}
	break;
	case E1_ISPLANETINDUALRASI: // Range: planet
	{
		int a = interpreter->getRasi( val ) % 3;
		ret = ( a == 2 );
#ifdef _RULE_DEB
		printf( "Evaluate is planet in dual rasi: param is %d, result is %d; ret is %d\n", val, a,  a == 0 );
#endif
	}
	break;
	case E1_TYPECAST_INT:
		ret = (int)val;
		break;
	case E1_TYPECAST_DOUBLE:
		ret = val;
		break;
	case E1_TYPECAST_PLANET:
		ret = (int)val;
		break;
	case E1_TYPECAST_SIGN:
		ret = (int)val;
		break;
	case E1_TYPECAST_NAKSHATRA:
		ret = (int)val;
		break;
	case E1_TYPECAST_HOUSE:
		ret = (int)val;
		break;
	case E1_TYPECAST_BOOLEAN:
		ret = (bool)val;
		break;

	default:
	{
		// TODO
		//ret = new ReturnExpression( ER_ERROR );
	}
	break;
	}
	return ret;
}

/*****************************************************
**
**   DualExpression   ---   Constructor
**
******************************************************/
DualExpression::DualExpression( Expression *e1, Expression *e2, const int &subtype )
		: Expression( EXPRESSION_DUAL, subtype )
{
	assert( e1 );
	assert( e2 );

	this->exp1 = e1;
	this->exp2 = e2;

	if ( e1->hasErrors ) appendErrors( e1 );
	if ( e2->hasErrors ) appendErrors( e2 );

	switch ( subtype )
	{
	case E2_PLANETINHOUSE:
		runtimetype = ET_BOOLEAN;
		if ( e1->runtimetype != ET_PLANET ) addRuntimeTypeError( 1, wxT( "isPlanetInHouse" ), e1->runtimetype, ET_PLANET );
		if ( e2->runtimetype != ET_HOUSE ) addRuntimeTypeError( 2, wxT( "isPlanetInHouse" ), e2->runtimetype, ET_HOUSE );
		break;
	case E2_LOGICAL_AND:
		runtimetype = ET_BOOLEAN;
		if ( e1->runtimetype != ET_BOOLEAN ) addRuntimeTypeError( 1, wxT( "boolean and" ), e1->runtimetype, ET_BOOLEAN );
		if ( e2->runtimetype != ET_BOOLEAN ) addRuntimeTypeError( 2, wxT( "boolean and" ), e2->runtimetype, ET_BOOLEAN );
		break;
	case E2_LOGICAL_OR:
		runtimetype = ET_BOOLEAN;
		if ( e1->runtimetype != ET_BOOLEAN ) addRuntimeTypeError( 1, wxT( "boolean or" ), e1->runtimetype, ET_BOOLEAN );
		if ( e2->runtimetype != ET_BOOLEAN ) addRuntimeTypeError( 2, wxT( "boolean or" ), e2->runtimetype, ET_BOOLEAN );
		break;
	case E2_ISEQUAL:
		runtimetype = ET_BOOLEAN;
		/*
		if ( e1->runtimetype != e2->runtimetype ) addError(
			wxString::Format( wxT( "Expected identical types in [boolean isequal] (param #1 %s, param 2 %s)" ),
				getRuntimeTypeName( e1->runtimetype ), getRuntimeTypeName( e2->runtimetype )));
		*/
		break;
	case E2_ISNOTEQUAL:
		runtimetype = ET_BOOLEAN;
		/*
		if ( e1->runtimetype != e2->runtimetype ) addError(
			wxString::Format( wxT( "Expected identical types in [boolean isnotequal] (param #1 %s, param 2 %s)" ),
				getRuntimeTypeName( e1->runtimetype ), getRuntimeTypeName( e2->runtimetype )));
		*/
		break;
	case E2_PLUS:
		runtimetype = ET_NUMBER;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "plus" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "plus" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_MINUS:
		runtimetype = ET_NUMBER;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "minus" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "minus" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_DIV:
		runtimetype = ET_NUMBER;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "div" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "div" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_MULT:
		runtimetype = ET_NUMBER;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "mult" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "mult" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_MOD:
		runtimetype = ET_NUMBER;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "mod" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "mod" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_MUTUAL_KENDRA:
		runtimetype = ET_BOOLEAN;
		if ( e1->runtimetype != ET_PLANET ) addRuntimeTypeError( 1, wxT( "mutualKendra" ), e1->runtimetype, ET_PLANET );
		if ( e2->runtimetype != ET_PLANET ) addRuntimeTypeError( 2, wxT( "mutualKendra" ), e2->runtimetype, ET_PLANET );
		break;
	case E2_GRAHA_DRISHTI:
		runtimetype = ET_BOOLEAN;
		if ( e1->runtimetype != ET_PLANET ) addRuntimeTypeError( 1, wxT( "grahaDrishti" ), e1->runtimetype, ET_PLANET );
		if ( e2->runtimetype != ET_PLANET ) addRuntimeTypeError( 2, wxT( "grahaDrishti" ), e2->runtimetype, ET_PLANET );
		break;
	case E2_LT:
		runtimetype = ET_BOOLEAN;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "LT" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "LT" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_LE:
		runtimetype = ET_BOOLEAN;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "LE" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "LE" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_GT:
		runtimetype = ET_BOOLEAN;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "GT" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "GT" ), e2->runtimetype, ET_NUMBER );
		break;
	case E2_GE:
		runtimetype = ET_BOOLEAN;
		//if ( e1->runtimetype != ET_NUMBER ) addRuntimeTypeError( 1, wxT( "GE" ), e1->runtimetype, ET_NUMBER );
		//if ( e2->runtimetype != ET_NUMBER ) addRuntimeTypeError( 2, wxT( "GE" ), e2->runtimetype, ET_NUMBER );
		break;
	default:
		addError( wxString::Format( wxT( "Wrong subtype %d for dual expression" ), subtype ));
		break;
	}
}

/*****************************************************
**
**   DualExpression   ---   Destructor
**
******************************************************/
DualExpression::~DualExpression()
{
	delete exp1;
	delete exp2;
}

/*****************************************************
**
**   DualExpression   ---   evaluate
**
******************************************************/
double DualExpression::evaluate( MdlInterpreter *interpreter )
{
	wxString err;
	double ret = 0;

	double d1 = exp1->evaluate( interpreter );
	double d2 = exp2->evaluate( interpreter );

#ifdef _RULE_DEB
	printf( "Evaluate Dual Expression type %d subtype %d, exp1 is type %d subtype, %d exp2 is type %d subtype %d result1 %f result2 %f\n",
	        type, subtype, exp1->type, exp1->subtype, exp2->type, exp2->subtype, d1, d2 );
#endif

#ifdef _RULE_DEB
#define TAG_TO_DOUBLE_EXPR( t, p ) case t: \
{ \
	printf( "Evaluate dual expression %d param1 %f param2 %f\n", t, d1, d2 ); \
	ret = ( d1 p d2 ); \
} \
break;
#else
#define TAG_TO_DOUBLE_EXPR( t, p ) case t: { return ( d1 p d2 ); } break;
#endif


	switch ( subtype )
	{
	case E2_PLANETINHOUSE:
	{
		int b = interpreter->getBhava( (int)d1 );
#ifdef _RULE_DEB
		printf( "Evaluate planetInHouse param1 is %d, param2 is %d / Bhava %d\n", (int)d1, (int)d2, b );
#endif
		ret = ( d2 == b ? 1 : 0 );
	}
	break;
	TAG_TO_DOUBLE_EXPR( E2_LOGICAL_AND, && )
	TAG_TO_DOUBLE_EXPR( E2_LOGICAL_OR, || )
	TAG_TO_DOUBLE_EXPR( E2_ISNOTEQUAL, != )
	TAG_TO_DOUBLE_EXPR( E2_PLUS, + )
	TAG_TO_DOUBLE_EXPR( E2_MINUS, - )
	TAG_TO_DOUBLE_EXPR( E2_MULT, * )
	TAG_TO_DOUBLE_EXPR( E2_LT, < )
	TAG_TO_DOUBLE_EXPR( E2_LE, <= )
	TAG_TO_DOUBLE_EXPR( E2_GT, > )
	TAG_TO_DOUBLE_EXPR( E2_GE, >= )

	case E2_DIV:
	{
		if ( d2 == 0 ) interpreter->addError( wxT( "Division by zero" ));
		return d1 / d2;
	}
	break;
	case E2_ISEQUAL:
	{
		return( d1 != -1 && d2 != -1 && d1 == d2 );
	}
	break;
	case E2_MOD:
	{
		ret = ( (int)d1 % (int)d2 );
	}
	break;
	case E2_MUTUAL_KENDRA:
	{
		int r1 = interpreter->getRasi( (int)d1 );
		int r2 = interpreter->getRasi( (int)d2 );
#ifdef _RULE_DEB
		printf( "Evaluate mututalKendra param1 is %d, param2 is %d rasi1 %d rasi2 %d return %d\n",
			(int)d1, (int)d2, r1, r2, red12( r1 - r2 ) %3 );
#endif
		ret = (red12( r1 - r2) % 3 ) ? 0 : 1 ;
	}
	break;
	case E2_GRAHA_DRISHTI:
	{
		int rasidiff = red12(  interpreter->getRasi( (int)d2 ) -  interpreter->getRasi( (int)d1 ));
#ifdef _RULE_DEB
		printf( "Evaluate grahadrishti param1 is %d, param2 is %d rasidiff %d return %d\n",
			(int)d1, (int)d2, rasidiff, getGrahaDrishti( (ObjectId)d1, rasidiff ));
#endif
		ret = getGrahaDrishti( (ObjectId)d1, rasidiff );
	}
	break;
	default:
	{
		// TODO
		//ret = new ReturnExpression( ER_ERROR );
	}
	}
#undef TAG_TO_DOUBLE_EXPR
	return ret;
}

/*****************************************************
**
**   Rule   ---   Constructor
**
******************************************************/
Rule::Rule()
{
}

/*****************************************************
**
**   Rule   ---   Destructor
**
******************************************************/
Rule::~Rule()
{
	for ( uint i = 0; i < expressions.size(); i++ ) delete expressions[i];
	expressions.clear();
}

/*****************************************************
**
**   Rule   ---   addExpression
**
******************************************************/
void Rule::addExpression( Expression *e )
{
	expressions.push_back( e );
	if ( e->hasErrors ) appendErrors( e );
}




