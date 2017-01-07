/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Transit.cpp
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

#include "Transit.h"

#include "ArabicParts.h"
#include "astrobase.h"
#include "maitreya.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Table.h"
#include "VedicPlanet.h"
#include "Varga.h"

extern Config *config;

IMPLEMENT_CLASS( TransitHoroscope, Horoscope )

/*****************************************************
**
**   TransitHoroscope   ---   Constructor
**
******************************************************/
TransitHoroscope::TransitHoroscope()
		: Horoscope()
{
	context = PcTransit;
}

/*****************************************************
**
**   TransitHoroscope   ---   calcTransitPositions
**
******************************************************/
double TransitHoroscope::calcTransitPositions( const Horoscope *hbase, const double &transitJD,
	const bool &vedic, const double &yl, const PlanetContext &mode )
{
	//printf( "TransitHoroscope::calcTransitPositions mode %d\n", mode );
	ASSERT_VALID_TRANSIT_CONTEXT( mode );

	context = mode;
	double posdelta = 0.0;
	double len, dummy;
	int i;
	ObjectId p;
	Calculator *calculator = CalculatorFactory().getCalculator();

	DataSet ds( *getDataSet());
	directionJD = hbase->getJD() + ( transitJD - hbase->getJD() ) / yl;

	switch ( mode )
	{
		case PcTransit:
			setDate( transitJD );
		break;
		case PcSolarArc:
		case PcSolarArcReverse:
		case PcShiftedGravitationPoint:
		case PcShiftedMeridian:
		case PcConstantArc:
		case PcLunarArc:
			setDate( hbase->getJD() );
		break;
		case PcDirection:
			setDate( directionJD );
		break;
		default:
			assert( false );
		break;
	}
	update();

	switch ( mode )
	{
		case PcSolarArc:
			ds.setDate( directionJD );
			calculator->calcPosition( &ds, OSUN, len, dummy, true, vedic );
			posdelta = red_deg( len - getLongitude( OSUN, vedic ));
		break;
		case PcSolarArcReverse:
			ds.setDate( directionJD );
			calculator->calcPosition( &ds, OSUN, len, dummy, true, vedic );
			posdelta = red_deg( getLongitude( OSUN, vedic ) - len );
		break;
		case PcShiftedGravitationPoint:
		{
			ds.setDate( directionJD );
			//Location *loc = ds.getLocation();

			// len = progressive sun position
			calculator->calcPosition( &ds, OSUN, len, dummy, true, vedic );
			//posdelta = red_deg( 180 + len - calculator->calcMCAya( directionJD, loc->getLatitude(), loc->getLongitude(), vedic ));

			// sun + iv + arc
			// sun(r) - p(r) - IC(V) = 2 * sun(r) + sun(p) - IC(R)
			posdelta = red_deg( 2 * getLongitude( OSUN, vedic ) - len - getLongitude( OIMUMCOELI, vedic ));
		}
		break;
		case PcShiftedMeridian:
		{
			ds.setDate( directionJD );
			Location *loc = ds.getLocation();
			calculator->calcPosition( &ds, OSUN, len, dummy, true, vedic );
			posdelta = red_deg( 180 + len - calculator->calcMCAya( directionJD, loc->getLatitude(), loc->getLongitude(), vedic ));
		}
		break;
		case PcConstantArc:
			posdelta = ( transitJD - hbase->getJD() ) / yl;
		break;
		case PcLunarArc:
			ds.setDate( directionJD );
			calculator->calcPosition( &ds, OMOON, len, dummy, true, vedic );
			posdelta = red_deg( len - getLongitude( OMOON, vedic ));
		break;
		default:
		break;
	}
	if ( posdelta != 0 )
	{
		for ( i = FIRST_EPHEM_OBJECT; i <= LAST_EPHEM_OBJECT; i++ )
		{
			object_len[i] = red_deg( object_len[i] + posdelta );

			// no retrogression for most modes
			if ( mode != PcTransit ) object_speed[i] = 1;
		}

		for ( i = 0; i < NUM_LAGNA; i++ ) lagna_len[i] = red_deg( lagna_len[ i ] + posdelta );
		for ( i = 0; i < NUM_UPA; i++ ) upagraha_len[i] = red_deg( upagraha_len[ i ] + posdelta );

		for ( i = HOUSE1; i <= HOUSE12; i++ )
		{
			//printf( "HOUSE %d old %f new %f posdelta %f\n", i, whousecusp[i], red_deg( whousecusp[i] + posdelta ), posdelta );
			whousecusp[i] = red_deg( whousecusp[i] + posdelta );
			ihousecusp[i] = red_deg( ihousecusp[i] + posdelta );
			ihousesandhi[i] = red_deg( ihousesandhi[i] + posdelta );
		}
		arieslen = posdelta;
	}
	for ( p = OSUN; p < MAX_EPHEM_OBJECTS; p++ )
	{
		ephem_planets[p]->update( getVedicLongitude( p ));
	}
	arabic_expert->update();

	updatePositionArrays();
	updateObjectPositions();

	setDate( transitJD );
	return posdelta;
}

/*****************************************************
**
**   TransitExpert   ---   Constructor
**
******************************************************/
TransitExpert::TransitExpert( Horoscope *h, const ChartProperties *chartprops )
		: Expert( h ),
		chartprops( chartprops )
{
	transitmode = PcTransit;
	init();

	htransit = new TransitHoroscope();
	htransit->setDate( h->getJD());
	htransit->setLocation( *h->getLocation());
	transitJD = MDate::getCurrentJD();
	tzoffset = 0;
}

/*****************************************************
**
**   TransitExpert   ---   Destructor
**
******************************************************/
TransitExpert::~TransitExpert()
{
	delete htransit;
}

/*****************************************************
**
**   TransitExpert   ---   init
**
******************************************************/
void TransitExpert::init()
{
	yearlength = calcYearLength(
		chartprops->isVedic() ? config->vedicCalculation->yearLength : config->westernCalculation->yearLength, chartprops->isVedic()
	);
}

/*****************************************************
**
**   TransitExpert   ---   update
**
******************************************************/
void TransitExpert::update()
{
	posdelta = htransit->calcTransitPositions( horoscope, transitJD, chartprops->isVedic(), yearlength, transitmode );
}

/*****************************************************
**
**   TransitExpert   ---   setTransitDate
**
******************************************************/
void TransitExpert::setTransitDate( const double &jd, const double tzoffset )
{
	transitJD = jd - tzoffset / 24.0;
	this->tzoffset = tzoffset;
}

/*****************************************************
**
**   TransitExpert   ---   writeTransitHeader
**
******************************************************/
void TransitExpert::writeTransitHeader( Sheet *sheet )
{
	ASSERT_VALID_TRANSIT_CONTEXT( transitmode );
	Formatter *formatter = Formatter::get();
	SheetFormatter sf;
	Lang lang;
	DateTimeFormatter *df = DateTimeFormatter::get();
	wxString header;
	MString rule;
	bool showdate = false;
	bool showarc = false;

	switch( transitmode )
	{
		case PcTransit:
			header = _( "Transits" );
			rule.add( _( "running planets" ));
		break;
		case PcDirection:
			header = _( "Directions" );
			showdate = true;

			rule.add( wxT( "1year = 1day" ));
		break;
		case PcSolarArc:
			header = _( "Solar Arc" );
			showdate = true;
			showarc = true;

			rule.add( sf.getObjectNameWithContext( OSUN, PcDirection ));
			rule.add( wxT( " + p - " ));
			rule.add( sf.getObjectNameWithContext( OSUN, PcRadix ));
		break;
		case PcSolarArcReverse:
			header = _( "Reverse Solar Arc" );
			showdate = true;
			showarc = true;

			rule.add( sf.getObjectNameWithContext( OSUN, PcRadix ));
			rule.add( wxT( " + p - " ));
			rule.add( sf.getObjectNameWithContext( OSUN, PcDirection ));
		break;
		case PcShiftedGravitationPoint:
			header = _( "Shifted Gravitation Point" );
			showdate = true;
			showarc = true;

			rule.add( sf.getObjectNameWithContext( OSUN, PcDirection ));
			rule.add( wxT( " + p - " ));
			rule.add( sf.getObjectNameWithContext( OIMUMCOELI, PcSolarArc ));
		break;
		break;
		case PcShiftedMeridian:
			header = _( "Shifted MC" );
			showdate = true;
			showarc = true;

			rule.add( sf.getObjectNameWithContext( OSUN, PcDirection ));
			rule.add( wxT( " + p - " ));
			rule.add( sf.getObjectNameWithContext( OIMUMCOELI, PcDirection ));
		break;
		case PcLunarArc:
			header = _( "Lunar Arc" );
			showdate = true;
			showarc = true;

			rule.add( sf.getObjectNameWithContext( OMOON, PcDirection ));
			rule.add( wxT( " + p - " ));
			rule.add( sf.getObjectNameWithContext( OMOON, PcRadix ));
		break;
		case PcConstantArc:
			header = _( "Constant Arc" );
			showarc = true;

			rule.add( wxT( "1year = 1deg" ));
		break;
		default:
			assert( false );
		break;
	}
	sheet->addHeader( header );
	if ( showdate )
	{
		wxString thedate;
		thedate << _( "Date" ) << wxT( ": " ) << df->formatFullDateString( getTransitHoroscope()->getDirectionJD());
		sheet->addLine( thedate );
	}
	if ( showarc )
	{
		wxString thearc;
		thearc << _( "Arc" ) << wxT( ": " ) << formatter->getDegreesFormatted( getPosDelta());
		sheet->addLine( thearc );
	}
	if ( ! rule.isEmpty())
	{
		MString r;
		r.add( _( "Rule" ));
		r.add( wxT( ": " ));
		r.add( rule );
		sheet->addParagraph( r );
	}
}

