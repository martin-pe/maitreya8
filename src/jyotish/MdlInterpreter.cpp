/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/MdlInterpreter.cpp
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

#include "astrobase.h"
#include "Conf.h"
#include "Expression.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "MdlInterpreter.h"
#include "Varga.h"
#include "VargaHoroscope.h"

extern Config *config;

//#define _RULE_DEB

/*****************************************************
**
**   MdlInterpreter   ---   Constructor
**
******************************************************/
MdlInterpreter::MdlInterpreter()
{
	return_status = RETURN_NO;
}

/*****************************************************
**
**   MdlInterpreter   ---   setChart
**
******************************************************/
void MdlInterpreter::setChart( const AbstractVargaHoroscope *chart )
{
	Rasi rasi;

	this->chart = chart;
	this->artificialLagna = chart->getRasi( OASCENDANT );

	for ( int i = 0; i < 13; i++ ) nb_planets_in_sign[i] = 0;
	for ( ObjectId p = OSUN; p <= OSATURN; p++ )
	{
		rasi = chart->getRasi( p );
		if ( rasi >= R_ARIES && rasi <= R_PISCES ) nb_planets_in_sign[rasi]++;
		else nb_planets_in_sign[12]++;
	}
}

/*****************************************************
**
**   MdlInterpreter   ---   setLagna
**
******************************************************/
void MdlInterpreter::setLagna( const int &artificialLagna )
{
	this->artificialLagna = artificialLagna;
}

/*****************************************************
**
**   MdlInterpreter   ---   evaluateYogaRule
**
******************************************************/
int MdlInterpreter::evaluateYogaRule( Rule *rule )
{
	Expression *e;
	int ret = 0;

	clearErrors();

#ifdef _RULE_DEB
	printf( "Evaluate YogaRule\n" );
#endif
	if ( rule == 0 )
	{
		addError( wxT( "Rule is NULL and cannot be evaluated" ));
		return -1;
	}
	for ( uint i = 0; i < rule->expressions.size(); i++ )
	{
#ifdef _RULE_DEB
		printf( "MdlInterpreter::evaluateYogaRule in Expression #%d\n", i );
#endif

		e = rule->expressions[i];
		if ( e->hasErrors )
		{
			appendErrors( e );
			return -3;
		}

		ret = (int)e->evaluate( this );
		if ( rule->expressions[i]->type == EXPRESSION_RETURN )
		{
			if ( e->runtimetype != ET_BOOLEAN )
			{
				addError( wxString::Format( wxT( "Wrong return type %s in return expression" ), e->getRuntimeTypeName( e->runtimetype )));
				return -4;
			}
			return ret;
		}
	}

	// must be an error
	addError( wxT( "No return expression found" ));
	return -3;
}

/*****************************************************
**
**   MdlInterpreter   ---   getRasi
**
******************************************************/
Rasi MdlInterpreter::getRasi( const double &i ) const
{
	return chart->getRasi( (ObjectId)i );
}

/*****************************************************
**
**   MdlInterpreter   ---   getNakshatra
**
******************************************************/
int MdlInterpreter::getNakshatra( const double &i ) const
{
	return chart->getNakshatra27( (ObjectId)i );
}

/*****************************************************
**
**   MdlInterpreter   ---   getBhava
**
******************************************************/
int MdlInterpreter::getBhava( const double &i ) const
{
	if ( artificialLagna != -1 )
	{
		return red12( this->getRasi( (ObjectId)i ) - this->artificialLagna );
	}
	else return chart->getBhava( (ObjectId)i );
}

/*****************************************************
**
**   MdlInterpreter   ---   isBenefic
**
******************************************************/
bool MdlInterpreter::isBenefic( const double &p ) const
{
	return chart->isBenefic( (ObjectId)p );
}

/*****************************************************
**
**   MdlInterpreter   ---   isMalefic
**
******************************************************/
bool MdlInterpreter::isMalefic( const double &p ) const
{
	return chart->isMalefic( (ObjectId)p );
}

/*****************************************************
**
**   MdlInterpreter   ---   getNumberOfOccupiedSigns
**
******************************************************/
int MdlInterpreter::getNumberOfOccupiedSigns()
{
	int count = 0;

	// all planets must be placed in the chart, otherwise runtime error.
	if ( nb_planets_in_sign[12] > 0 )
		addError( wxString::Format( wxT( "Invalid chart, %d planets not occupying valid signs" ), nb_planets_in_sign[12] ));

	for ( int i = 0; i < 12; i++ ) {
		if ( nb_planets_in_sign[i] > 0 ) count++;
	}
	return count;
}

/*****************************************************
**
**   MdlInterpreter   ---   getNumberOfObjectsInSign
**
******************************************************/
int MdlInterpreter::getNumberOfObjectsInSign( const double &i ) const
{
	return nb_planets_in_sign[red12( (int)(i+1) )];
}




