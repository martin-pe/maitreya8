/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/UranianBase.cpp
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

#include "UranianBase.h"

#include <math.h>
#include <wx/log.h>

#include "astrobase.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Uranian.h"
#include "SymbolProvider.h"

extern Config *config;

IMPLEMENT_SINGLETON( UranianTool )

IMPLEMENT_CLASS( UEntity, wxObject )
IMPLEMENT_CLASS( UObject, UEntity )
IMPLEMENT_CLASS( UObjectPair, UEntity )

/*****************************************************
**
**   
**
******************************************************/
UranianTool::UranianTool()
{
}

/*****************************************************
**
**   UranianTool   ---   gradkreis2String
**
******************************************************/
wxString UranianTool::gradkreis2String( const GRADKREIS &gradkreis )
{
	switch ( gradkreis )
	{
		case GK_NONE:
			return _( "<None>" );
		break;
		case GK_360:
			//return _( "Conj" );
			return wxT( "360" );
		break;
		case GK_180:
			//return _( "Opp" );
			return wxT( "180" );
		break;
		case GK_90:
			//return _( "Square" );
			return wxT( "90" );
		break;
		case GK_45:
			return _( "1/8" );
		break;
		case GK_225:
			return _( "1/16" );
		break;
		case GK_1125:
			return _( "1/32" );
		break;
		case GK_5625:
			return _( "1/64" );
		break;
		default:
			//assert( false );
			wxLogError( wxString::Format( wxT( "invalid gradkreis id %d\n" ), gradkreis ));
			return wxT( "<invalid>" );
		break;
	}
}

/*****************************************************
**
**   UranianTool   ---   gradkreis2AspectCode
**
******************************************************/
ASPECT_TYPE UranianTool::gradkreis2AspectCode( const GRADKREIS &gk ) const
{
	ASSERT_VALID_GRADKREIS( gk )
	switch( gk )
	{
		case GK_360:
			return AT_CONJUNCTION;
		break;
		case GK_180:
			return AT_OPPOSITION;
		break;
		case GK_90:
			return AT_SQUARE;
		break;
		case GK_45:
			return AT_SEMISQUARE;
		break;
		case GK_225:
			return AT_QUARTERSQUARE;
		break;
		case GK_1125:
			return AT_QUAVER;
		break;
		case GK_5625:
			return AT_SEMIQUAVER;
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   UranianTool   ---   getUranianAspectCode
**
******************************************************/
wxChar UranianTool::getUranianAspectCode( const GRADKREIS &gradkreis ) const
{
	ASSERT_VALID_GRADKREIS( gradkreis )
	return SymbolProvider().getAspectCode( gradkreis2AspectCode( gradkreis ));
}

/**************************************************************
***
**   UranianTool   ---   getDegrees4Gradkreis
***
***************************************************************/
double UranianTool::getDegrees4Gradkreis( const GRADKREIS &gradkreis )
{
	ASSERT_VALID_GRADKREIS( gradkreis )
	return 360.0 / getMultiplicator4Gradkreis( gradkreis );
}

/**************************************************************
***
**   UranianTool   ---   getMultiplicator4Gradkreis
***
***************************************************************/
double UranianTool::getMultiplicator4Gradkreis( const GRADKREIS &gradkreis )
{
	ASSERT_VALID_GRADKREIS( gradkreis )

	switch ( gradkreis )
	{
		case GK_360:  return 1.0;  break;
		case GK_180:  return 2.0;  break;
		case GK_90:   return 4.0;  break;
		case GK_45:   return 8.0;  break;
		case GK_225:  return 16.0; break;
		case GK_1125: return 32.0; break;
		case GK_5625: return 64.0; break;
		default:
			assert( 0 );
		break;
	}
	return 1;
}

/*****************************************************
**
**   UObject   ---   Constructor
**
******************************************************/
UObject::UObject( const double &longitude, const PlanetContext& ctx, const ObjectId& p, const LONGITUDE_TYPE lttype )
	: UEntity( longitude, ctx, lttype ), p( p )
{
}

/*****************************************************
**
**   UObject   ---   clone
**
******************************************************/
UEntity *UObject::clone()
{
	return new UObject( longitude, context, p, lttype );
}

/*****************************************************
**
**   UObject   ---   write
**
******************************************************/
MString UObject::write( const UWRITE_FORMAT ef ) const
{
	Lang lang;

	MString t;
	t.add( MToken( TTSE_PLANET, (int)p, TF_MEDIUM ));
	if ( lttype == LT_ANTISCIA ) t.add( MToken( wxT( "a" ), TTFF_SUPERSCRPTUM ));
	if ( ef == UF_CONTEXT ) t.add( MToken( lang.getPlanetContextSubscriptum( context ), TTFF_SUBSCRPTUM ));

	return t;
}

/*****************************************************
**
**   UObjectPair   ---   Constructor
**
******************************************************/
UObjectPair::UObjectPair( const double &longitude, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2,
	const wxChar operatorSymbol, const LONGITUDE_TYPE lttype )
	 : UEntity( longitude, ctx, lttype ), p1( p1 ), p2( p2 ),
	 operatorSymbol( operatorSymbol )
{
}

/*****************************************************
**
**   UObjectPair   ---   write
**
******************************************************/
MString UObjectPair::write( const UWRITE_FORMAT ef ) const
{
	wxString s;
	Lang lang;

	MString t;
	t.add( MToken( TTSE_PLANET, (int)p1, TF_MEDIUM ));
	t.add( MToken( operatorSymbol ));
	t.add( MToken( TTSE_PLANET, (int)p2, TF_MEDIUM ));
	if ( ef == UF_CONTEXT ) t.add( MToken( lang.getPlanetContextSubscriptum( context ), TTFF_SUBSCRPTUM ));

	return t;
}

/*****************************************************
**
**   UMidpoint   ---   clone
**
******************************************************/
UEntity *UMidpoint::clone()
{
	return new UMidpoint( longitude, context, p1, p2, lttype );
}

/*****************************************************
**
**   USum   ---   clone
**
******************************************************/
UEntity *USum::clone()
{
	return new USum( longitude, context, p1, p2, lttype );
}

/*****************************************************
**
**   UDifference   ---   clone
**
******************************************************/
UEntity *UDifference::clone()
{
	return new UDifference( longitude, context, p1, p2, lttype );
}

/*****************************************************
**
**   UReflectionPoint   ---   clone
**
******************************************************/
UEntity *UReflectionPoint::clone()
{
	return new UReflectionPoint( longitude, context, p1, p2, lttype );
}

/*****************************************************
**
**   UReflectionPoint   ---   write
**
******************************************************/
MString UReflectionPoint::write( const UWRITE_FORMAT ef ) const
{
	wxString s;
	Lang lang;

	MString t;
	MToken token( TTSE_PLANET, (int)p1, TF_MEDIUM );
	t.add( token );
	t.add( wxT( " + " ));
	t.add( token );
	t.add( wxT( " - " ));
	t.add( MToken( TTSE_PLANET, (int)p2, TF_MEDIUM ));
	if ( ef == UF_CONTEXT ) t.add( MToken( lang.getPlanetContextSubscriptum( context ), TTFF_SUBSCRPTUM ));

	return t;
}

/*****************************************************
**
**   UObjectTriple   ---   Constructor
**
******************************************************/
UObjectTriple::UObjectTriple( const double &longitude, const PlanetContext& ctx, const ObjectId& p1,
	const ObjectId& p2, const ObjectId& p3, const LONGITUDE_TYPE lttype )
 : UObjectPair( longitude, ctx, p1, p2, lttype ), p3( p3 )
{
}

/*****************************************************
**
**   USensitivePoint   ---   clone
**
******************************************************/
UEntity *USensitivePoint::clone()
{
	return new USensitivePoint( longitude, context, p1, p2, p3, lttype );
}

/*****************************************************
**
**   USensitivePoint   ---   write
**
******************************************************/
MString USensitivePoint::write( const UWRITE_FORMAT ef ) const
{
	MString t;
	Lang lang;

	t.add( MToken( TTSE_PLANET, (int)p1, TF_MEDIUM ));
	t.add( MToken( wxT( " + "  )));
	t.add( MToken( TTSE_PLANET, (int)p2, TF_MEDIUM ));
	t.add( MToken( wxT( " - "  )));
	t.add( MToken( TTSE_PLANET, (int)p3, TF_MEDIUM ));
	if ( ef == UF_CONTEXT ) t.add( MToken( lang.getPlanetContextSubscriptum( context ), TTFF_SUBSCRPTUM ));

	return t;
}

/*****************************************************
**
**   UEvent   ---   Constructor
**
******************************************************/
UEvent::UEvent( UEntity *e1, UEntity *e2, const GRADKREIS &gradkreis, const double &orbis, const double &residuum, const double jd )
	: e1( e1 ), e2( e2 ), gradkreis( gradkreis ), orbis( orbis ), residuum( residuum ), duedate( jd )
{
	assert( e1 );
	assert( e2 );
}

/*****************************************************
**
**   UEvent   ---   Constructor
**
******************************************************/
UEvent::UEvent( UEntity *e1, UEntity *e2, const UranianMatch &m, const double jd )
	: e1( e1 ), e2( e2 ), gradkreis( m.gradkreis ), orbis( m.orbis ), residuum( m.residuum ), duedate( jd )
{
	assert( e1 );
	assert( e2 );
}

/*****************************************************
**
**   UEvent   ---   Copy Constructor
**
******************************************************/
UEvent::UEvent( const UEvent &e )
	: gradkreis( e.gradkreis ),
	orbis( e.orbis ),
	residuum( e.residuum )
{
	assert( e.e1 );
	this->e1 = e.e1->clone();
	assert( e.e2 );
	this->e2 = e.e2->clone();

	duedate = e.duedate;
}

/*****************************************************
**
**   UEvent   ---   Destructor
**
******************************************************/
UEvent::~UEvent()
{
	delete e1;
	delete e2;
}

/*****************************************************
**
**   UEvent   ---   write
**
******************************************************/
MString UEvent::write( const UWRITE_FORMAT ef ) const
{
	assert( e1 );
	assert( e2 );
	MString t = e1->write( ef );
	t.add( MToken( wxT( " = "  )));
	t.add( e2->write( ef ));
	return t;
}

/*****************************************************
**
**   UEventSorter   ---   operator()
**
******************************************************/
bool UEventSorter::operator()( const UEvent& v1, const UEvent& v2 ) const
{
	//printf( "UEventSorter::operator order is %d\n", order );
	switch ( order )
	{
		case AS_RPLANET:
			assert( v1.e2 != (UEntity*)NULL );
			assert( v2.e2 != (UEntity*)NULL );
			return isObjectIdLessThan( v1.e2->getFirstObject(), v2.e2->getFirstObject(), vedic );
		break;
		case AS_ORBIS:
			return v1.orbis < v2.orbis;
		break;
		case AS_ORBIS_REVERSE:
			return v1.orbis > v2.orbis;
		break;
		case AS_ORBIS_ABSOLUTE:
			return fabs( v1.orbis ) < fabs( v2.orbis );
		break;
		case AS_DATE:
			return v1.duedate < v2.duedate;
		break;
		case AS_LPLANET:
			assert( v1.e1 != (UEntity*)NULL );
			assert( v2.e1 != (UEntity*)NULL );
			return isObjectIdLessThan( v1.e1->getFirstObject(), v2.e1->getFirstObject(), vedic );
		break;
		case AS_TYPE: // sort according to gradkreis. 
			return v1.gradkreis < v2.gradkreis;
		break;
		case AS_SCORE:
			// fallback
			return fabs( v1.orbis ) < fabs( v2.orbis );
		break;
		default:
			printf( "Invalid sort order %d in UEventSorter\n", order );
		break;
	}
	return false;
}

/*****************************************************
**
**   UranianCalculationResult   ---   Constructor
**
******************************************************/
UranianCalculationResult::UranianCalculationResult( UranianExpert *expert, Horoscope *horoscope )
 : expert( expert ),
 horoscope( horoscope )
{
	assert( expert );
	assert( horoscope );
}

/*****************************************************
**
**   UranianCalculationResult   ---   clear
**
******************************************************/
void UranianCalculationResult::clear()
{
	uevents.clear();
}


