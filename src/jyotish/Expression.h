/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Expression.h
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

#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <wx/string.h>
#include <vector>

class MdlInterpreter;

// Expression types
enum { EXPRESSION_RETURN = 0, EXPRESSION_NUMERIC, EXPRESSION_UNARY, EXPRESSION_DUAL };

// Runtime types
enum { ET_NUMBER = 10, ET_HOUSE, ET_PLANET, ET_SIGN, ET_BOOLEAN, ET_NAKSHATRA };

// Subtypes: Return expression
enum { ER_EXPRESSION = 20, ER_VOID };

// Subtypes: constant expression
enum { ED_CONST = 30, ED_PLANET, ED_HOUSE, ED_SIGN, ED_NAKSHATRA, ED_BOOLEAN, ED_NUMBER_OF_OCCUPIED_SIGNS };

// Subtypes: unary expression
enum { E1_LORDOFHOUSE = 50, E1_GETRASI, E1_GETBHAVA, E1_PLANETS_INSIGN, E1_PLANETS_INHOUSE, E1_GETLORD, E1_ISBENEFIC, E1_ISMALEFIC,
       E1_ISPLANETINKENDRA, E1_ISPLANETINAPOKLIMA, E1_ISPLANETINPANAPHARA, E1_ISPLANETINDUALRASI, E1_NOT, E1_RED12,
       E1_ISPLANETINFIXEDRASI, E1_ISPLANETINMOVABLERASI, E1_GETNAKSHATRA, E1_TYPECAST_INT, E1_TYPECAST_DOUBLE,
       E1_TYPECAST_PLANET, E1_TYPECAST_SIGN, E1_TYPECAST_NAKSHATRA, E1_TYPECAST_HOUSE, E1_TYPECAST_BOOLEAN
};

// Subtypes: dual expression
enum { E2_PLANETINHOUSE = 100, E2_LOGICAL_AND, E2_LOGICAL_OR, E2_ISEQUAL, E2_ISNOTEQUAL, E2_PLUS, E2_MINUS, E2_DIV, E2_MULT, E2_MOD,
       E2_MUTUAL_KENDRA, E2_GRAHA_DRISHTI, E2_LT, E2_LE, E2_GT, E2_GE
};

enum { RETURN_NO = 0, RETURN_YES, RETURN_ERROR };


/*************************************************//**
*
* \brief base class for expression error handling
*
******************************************************/
class ExpressionErrorHandler
{
public:
	ExpressionErrorHandler();

	double appendErrors( class ExpressionErrorHandler* );
	void addError( wxString );
	void clearErrors();
	virtual void printStackTrace();

	bool hasErrors;
	std::vector<wxString> errorStack;
};

/*************************************************//**
*
* \brief abstract class for MDL expression
*
******************************************************/
class Expression : public ExpressionErrorHandler
{
public:
	Expression( const int &type, const int &subtype );
	virtual ~Expression() {}

	const int lineno, type, subtype;

	int runtimetype;

	// must be overwritten
	virtual double evaluate( MdlInterpreter* ) = 0;

	// used for identification in error description
	const wxChar *getRuntimeTypeName( const int& );

	void addRuntimeTypeError( const wxChar *item, const int &runtime_type, const int &expected_type );
	void addRuntimeTypeError( const int &param, const wxChar *item, const int &runtime_type, const int &expected_type );
};

/*************************************************//**
*
* \brief MDL expression returning a value (including void)
*
******************************************************/
class ReturnExpression : public Expression
{
public:
	ReturnExpression( Expression* );

	virtual double evaluate( MdlInterpreter* );

	Expression *exp1;
};

/*************************************************//**
*
* \brief numeric expression in MDL
*
******************************************************/
class NumericExpression : public Expression
{
public:
	NumericExpression( const int &subtype, const double = 0 );

	virtual double evaluate( MdlInterpreter* );

	double value;
};

/*************************************************//**
*
* \brief unary expression in MDL
*
******************************************************/
class UnaryExpression : public Expression
{
public:
	UnaryExpression( Expression*, const int &subtype );
	~UnaryExpression();

	virtual double evaluate( MdlInterpreter* );

	Expression *exp1;
};

/*************************************************//**
*
* \brief dual expression in MDL (based upon 2 expresions)
*
******************************************************/
class DualExpression : public Expression
{
public:
	DualExpression( Expression*, Expression*, const int &subtype );
	~DualExpression();

	virtual double evaluate( MdlInterpreter* );

	Expression *exp1, *exp2;
};

/*************************************************//**
*
* \brief the rules class contains a list of expressions for MDL evalulation
*
******************************************************/
class Rule : public ExpressionErrorHandler
{
public:
	Rule();
	virtual ~Rule();

	void addExpression( Expression* );

	std::vector<Expression*> expressions;
};

#endif


