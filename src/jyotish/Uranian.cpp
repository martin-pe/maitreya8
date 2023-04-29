/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Uranian.cpp
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

#include "Uranian.h"

#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "maitreya.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Transit.h"
#include "UranianHelper.h"

#include <math.h>

extern Config *config;

IMPLEMENT_CLASS( UranianHoroscope, Horoscope )

/*****************************************************
**
**   UranianHoroscope   ---   Constructor
**
******************************************************/
UranianHoroscope::UranianHoroscope( const int mode )
	: Horoscope(),
	mode( mode )
{
}

/*****************************************************
**
**   UranianHoroscope   ---   Constructor
**
******************************************************/
ObjectPosition UranianHoroscope::getObjectPosition( const ObjectId &planet, const bool &vedic ) const
{
	const ObjectPosition sunpos = Horoscope::getObjectPosition( OSUN, vedic );

	ObjectPosition pos = Horoscope::getObjectPosition( planet, vedic );

	switch( mode )
	{
		case 0: // sun on Cap
			pos.longitude = red_deg( 270 - sunpos.longitude + pos.longitude );
		break;
		case 1: // sun on Cap (m)
			pos.longitude = red_deg( 270 + sunpos.longitude - pos.longitude );
		break;
		case 2: // Cap on Sun
			pos.longitude = red_deg( sunpos.longitude + pos.longitude - 270 );
		break;
		case 3: // Cap on Sun (m)
		default:
			pos.longitude = red_deg( 90 - sunpos.longitude - pos.longitude );
		break;
	}
	return pos;
}

/*************************************************//**
*
* \brief overrides getObjectPosition: set Antiscium instead of longitude
*
******************************************************/
class AntisciaHoroscope : public Horoscope
{
	virtual ObjectPosition getObjectPosition( const ObjectId &planet, const bool &vedic ) const
	{
		ObjectPosition pos = Horoscope::getObjectPosition( planet, vedic );
		//printf( "getObjectPosition lon %f new lon %f\n", pos.longitude, getAntiscium( pos.longitude ));
		pos.longitude = getAntiscium( pos.longitude );
		return pos;
	}
};

/*****************************************************
**
**   UranianExpert   ---   Constructor
**
******************************************************/
UranianExpert::UranianExpert( Horoscope *h, ChartProperties *chartprops )
		: Expert( h ),
		chartprops( chartprops )
{
	reset();
	init();
}

/*****************************************************
**
**   UranianExpert   ---   Constructor
**
******************************************************/
UranianExpert::UranianExpert( ChartProperties *chartprops )
		: Expert( (Horoscope*)NULL ),
		chartprops( chartprops )
{
	reset();
	init();
}

/*****************************************************
**
**   UranianExpert   ---   Destructor
**
******************************************************/
UranianExpert::~UranianExpert()
{
	for( int i = 0; i < MAX_PLANET_CONTEXT; i++ )
	{
		if ( result[i] ) delete result[i];
	}
	
	if ( hperiod ) delete hperiod;
	if ( hantiscia ) delete hantiscia;
}

/*****************************************************
**
**   UranianExpert   ---   reset()
**
******************************************************/
void UranianExpert::reset()
{
	for( int i = 0; i < MAX_PLANET_CONTEXT; i++ ) result[i] = (UranianCalculationResult*)NULL;

	hperiod = (TransitHoroscope*)NULL;
	hantiscia = (Horoscope*)NULL;
}

/*****************************************************
**
**   UranianExpert   ---   init
**
******************************************************/
void UranianExpert::init()
{
	calculator = CalculatorFactory().getCalculator();

	year_length = calcYearLength( config->westernCalculation->yearLength, false );

	show_header = config->view->showTextViewHeaders;
	hantiscia = new AntisciaHoroscope;
}

/*****************************************************
**
**   UranianExpert   ---   planetContext2Result
**
******************************************************/
UranianCalculationResult *UranianExpert::planetContext2Result( const PlanetContext &ctx )
{
	//printf( "Get PlanetContext %d\n", ctx );
	ASSERT_VALID_PLANET_CONTEXT( ctx );
	return result[ctx];
}

/*****************************************************
**
**   UranianExpert   ---   initRadix
**
******************************************************/
void UranianExpert::initRadix()
{
	//printf( "UranianExpert::initRadix\n" );
	if ( result[PcRadix] )
	{
		result[PcRadix]->clear();
		result[PcRadix]->horoscope = horoscope;
	}
	else initResultContext( horoscope, PcRadix );

	if ( result[PcRadixAntiscia] )
	{
		result[PcRadixAntiscia]->clear();
		result[PcRadixAntiscia]->horoscope = hantiscia;
	}
	else initResultContext( hantiscia, PcRadixAntiscia );
}

/*****************************************************
**
**   UranianExpert   ---   initResultContext
**
******************************************************/
void UranianExpert::initResultContext( Horoscope *h, const PlanetContext &ctx )
{
	//printf( "UranianExpert::initResultContext context %d\n", ctx );
	ASSERT_VALID_PLANET_CONTEXT( ctx );
	UranianCalculationResult *r = planetContext2Result( ctx );
	if ( r )
	{
		r->clear();
		r->horoscope = h;
	}
	else
	{
		result[ctx] = new UranianCalculationResult( this, h );
	}
}

/*****************************************************
**
**   UranianExpert   ---   initPeriod
**
******************************************************/
void UranianExpert::initPeriod()
{
	if ( result[PcPeriod] )
	{
		result[PcPeriod]->clear();
		assert( hperiod );
		assert( result[PcPeriod]->horoscope == hperiod );
	}
	else
	{
		hperiod = new TransitHoroscope;
		initResultContext( hperiod, PcPeriod );
	}
}

/*****************************************************
**
**   UranianExpert   ---   initPartner
**
******************************************************/
void UranianExpert::initPartner( Horoscope *h1, Horoscope *h2 )
{
	initResultContext( h1, PcPartner1 );
	initResultContext( h2, PcPartner2 );
}

/*****************************************************
**
**   UranianExpert   ---   OnDataChanged
**
******************************************************/
void UranianExpert::OnDataChanged()
{
	init();
	initRadix();
	UranianConfig &cfg = chartprops->getUranianConfig();

	findMatchingEvents( PcRadix, PcRadix, PcRadix, cfg.orbisRadix );

	/*
	if ( cfg.eventsIncludeAntiscia )
	{
		hantiscia->setLocation( *horoscope->getLocation());
		hantiscia->setDate( horoscope->getJD());
		hantiscia->update();
		findMatchingEvents( PcRadixAntiscia, PcRadix, PcRadix, cfg.orbisRadix );
	}
	*/
}

/*****************************************************
**
**   UranianExpert   ---   createClusterMatchingList
**
******************************************************/
std::map<ObjectId, ClusterMatchingList> UranianExpert::createClusterMatchingList( const PlanetContext &ctx )
{
	std::map<ObjectId, ClusterMatchingList> m;
	UranianCalculationResult *v = planetContext2Result( ctx );
	assert( v );
	const Horoscope *h = v->horoscope;
	assert( h );

	const bool skipAries = ( ctx == PcTransit || ctx == PcDirection );

	for( std::list<UEvent>::iterator iter = v->uevents.begin(); iter != v->uevents.end(); iter++ )
	{
		if ( (*iter).isDoubleObjectEvent())
		{
			UObject *uo1 = wxDynamicCast( (*iter).e1, UObject );
			assert( uo1 );
			UObject *uo2 = wxDynamicCast( (*iter).e2, UObject );
			assert( uo2 );

			if ( uo1->p == OARIES && skipAries ) continue;

			if ( m.find( uo1->p ) == m.end()) m[uo1->p] = ClusterMatchingList( uo1->context );
			m[uo1->p].matchings.push_back( uo2 );
			if ( uo1->context != uo2->context ) m[uo1->p].oneContext = false;
		}
		else if ( (*iter).isObjectAndPairEvent())
		{
			UObject *uo = iter->getObjectEntity();
			UObjectPair *up = (*iter).getObjectPairEntity();

			if ( uo->p == OARIES && skipAries ) continue;

			if ( m.find( uo->p ) == m.end())
			{
				m[uo->p] = ClusterMatchingList( uo->context );
			}

			assert( m.find( uo->p ) != m.end());
			m[uo->p].matchings.push_back( up );
			if ( uo->context != up->context ) m[uo->p].oneContext = false;
		}
		else if ( (*iter).isObjectAndTripleEvent())
		{
			// TODO
			// ignore
		}
		else if ( (*iter).isDoublePairEvent())
		{
			printf( "ERROR in UranianExpert::createClusterMatchingList: double pairs not supported\n" );
		}
		else assert( false );
	}
	return m;
}

/*****************************************************
**
**   UranianExpert   ---   findMatchingEvents
**
******************************************************/
void UranianExpert::findMatchingEvents( const PlanetContext &ctx, const PlanetContext &refctx,
	const PlanetContext &storectx, const double &orbis )
{
	ObjectId p, p1, p2, p3;
	uint i, j, k, k2;
	double len1, len2, len3, len4, midpoint, sum, difference, rpoint, spoint;
	UranianMatch match;
	UranianConfig &cfg = chartprops->getUranianConfig();
	const bool vedic = chartprops->isVedic();
	const ObjectArray &planets = chartprops->getPlanetList();

	//static int cc = 0;
	//printf( "UranianExpert::findMatchingEvents #%d ctx %d refctx %d storectx %d \n", cc++, (int)ctx, (int)refctx, (int)storectx );
	
	UranianCalculationResult *v = planetContext2Result( ctx );
	assert( v );
	const Horoscope *h = v->horoscope;
	assert( h );

	UranianCalculationResult *vref = planetContext2Result( refctx );
	assert( vref );
	const Horoscope *href = vref->horoscope;
	assert( href );
	const double refjd = href->getJD();

	UranianCalculationResult *vstore = planetContext2Result( storectx );
	assert( vstore );
	const Horoscope *hstore = vstore->horoscope;
	assert( hstore );


	if ( cfg.eventsIncludeTriples )
	{
		for ( i = 0; i < planets.size(); i++ )
		{
			p1 = planets[i];
			if ( p1 == OARIES ) continue;

			len1 = h->getLongitude( p1, vedic );
			for ( j = 0; j < planets.size(); j++ )
			{
				if ( i == j ) continue;

				p2 = planets[j];
				if ( p2 == OARIES ) continue;

				len2 = h->getLongitude( p2, vedic );
				for ( k = 0; k < planets.size(); k++ )
				{
					if ( k == i ||  k == j ) continue;
					p3 = planets[k];
					if ( p3 == OARIES ) continue;

					len3 = h->getLongitude( p3, vedic );
					spoint = red_deg( len1 + len2 - len3 );
					for ( k2 = 0; k2 < planets.size(); k2++ )
					{
						p = planets[k2];
						if ( p == OARIES ) continue;
						if ( ctx == refctx && ( p == p1 ||  p == p2 || p == p3 )) continue;

						len4 = href->getLongitude( p, vedic );

						match = calculateMatch( spoint, len4, orbis );
						if ( match.gradkreis != GK_NONE )
						{
							//printf( "Conjunction p1 %d p2 %d orbis %f\n", p1, p3, match.orbis );
							vstore->uevents.push_back(
								UEvent(
									new UObject( len4, refctx, p ),
									new USensitivePoint( spoint, ctx, p1, p2, p3 ),
									match, refjd )
							);
						}
					}
				}
			}
		}
	}

	/*
	*   handle conjunctions
	*/
	for ( i = 0; i < planets.size(); i++ )
	{
		p1 = planets[i];
		len1 = h->getLongitude( p1, vedic );

		for ( j = 0; j < planets.size(); j++ )
		{
			p2 = planets[j];
			len2 = href->getLongitude( p2, vedic );
			if ( len2 == 0 ) continue;

			if ( refctx != ctx || isObjectIdLessThan( p2, p1, vedic ))
			{
				// bugfix 8.0.1: other order for triple events, so orbis sign different and yearly preview wrong
				//match = calculateMatch( len2, len1, orbis );
				match = calculateMatch( len1, len2, orbis );
				if ( match.gradkreis != GK_NONE )
				{
					//printf( "Conjunction p1 %d p2 %d orbis %f\n", p1, p2, match.orbis );
					vstore->uevents.push_back(
						UEvent(
							new UObject( len2, refctx, p2 ),
							new UObject( len1, ctx, p1 ),
							match, refjd )
					);
				}
			}
		}
	}

	/*
	*  handle events with p = pair of objects, i.e. midpoints, sums etc
	*/
	for ( i = 0; i < planets.size(); i++ )
	{
		p1 = planets[i];
		len1 = h->getLongitude( p1, vedic );

		for ( j = 0; j < planets.size(); j++ )
		{
			p2 = planets[j];
			len2 = h->getLongitude( p2, vedic );

			midpoint = getMidpoint( len1, len2 );
			sum =  red_deg( len1 + len2 );
			difference =  red_deg( len1 - len2 );
			rpoint =  red_deg( 2 * len1 - len2 );

			for ( k = 0; k < planets.size(); k++ )
			{
				p3 = planets[k];
				len3 = href->getLongitude( p3, vedic );

				if ( p3 == p1 || p3 == p2 ) continue;

				/*
				*  midpoints
				*/
				if ( j > i && cfg.eventsIncludeMidpoints && p1 != p2 )
				{
					match = calculateMatch( midpoint, len3, orbis );
					if ( match.gradkreis != GK_NONE )
					{
						vstore->uevents.push_back(
							UEvent(
								new UObject( len3, refctx, p3 ),
								new UMidpoint( midpoint, ctx, p1, p2 ),
								match, refjd )
							);
					}
				} // end midpoints

				/*
				*  sums
				*/
				if ( j > i && cfg.eventsIncludeSums )
				{
					match = calculateMatch( sum, len3, orbis );
					if ( match.gradkreis != GK_NONE )
					{
						vstore->uevents.push_back(
							UEvent(
								new UObject( len3, refctx, p3 ),
								new USum( sum, ctx, p1, p2 ),
								match, refjd )
							);
					}
				} // end sums

				/*
				*  differences
				*/
				if ( cfg.eventsIncludeDifferences && p1 != p2 )
				{
					match = calculateMatch( difference, len3, orbis );
					if ( match.gradkreis != GK_NONE )
					{
						vstore->uevents.push_back(
							UEvent(
								new UObject( len3, refctx, p3 ),
								new UDifference( difference, ctx, p1, p2 ),
								match, refjd )
							);
					}
				} // end differences

				/*
				*  reflection points
				*/
				if ( cfg.eventsIncludeReflectionPoints && p1 != p2 )
				{
					match = calculateMatch( rpoint, len3, orbis );
					if ( match.gradkreis != GK_NONE )
					{
						vstore->uevents.push_back(
							UEvent(
								new UObject( len3, refctx, p3 ),
								new UReflectionPoint( rpoint, ctx, p1, p2 ),
								match, refjd )
							);
					}
				}
			}
		}
	}
	vstore->uevents.sort( UEventSorter( cfg.sortOrder, false /*vedic*/ ));
}

/*****************************************************
**
**   UranianExpert   ---   calculateTransit
**
******************************************************/
void UranianExpert::calculateTransit( Horoscope *htransit, const PlanetContext &ctx )
{
	//printf( "UranianExpert::calculateTransit ctx %d\n", ctx );
	initResultContext( htransit, ctx );
	UranianCalculationResult *v = planetContext2Result( ctx );
	assert( v );
	const Horoscope *h = v->horoscope;
	assert( h );
	UranianConfig &cfg = chartprops->getUranianConfig();

	findMatchingEvents( PcRadix, ctx, ctx, cfg.orbisTransit );

	/*
	if (  cfg.eventsIncludeAntiscia )
	{
		findMatchingEvents( PcRadixAntiscia, ctx, ctx, cfg.orbisTransit );
	}
*/
}

/*****************************************************
**
**   UranianExpert   ---   calculatePartner
**
******************************************************/
void UranianExpert::calculatePartner( Horoscope *h1, Horoscope *h2 )
{
	UranianConfig &cfg = chartprops->getUranianConfig();
  initPartner( h1, h2 );

	//printf( "UranianExpert::calculatePartner orbis %f\n", cfg.orbisPartner );

	findMatchingEvents( PcPartner1, PcPartner2, PcPartner2, cfg.orbisPartner );
	findMatchingEvents( PcPartner2, PcPartner1, PcPartner1, cfg.orbisPartner );
}

/*****************************************************
**
**   UranianExpert   ---   calculateMatch
**
******************************************************/
UranianMatch UranianExpert::calculateMatch( const double &len1, const double &len2, const double &orbis )
{
	double circledeg = 360.0;
	double dist;
	UranianConfig &cfg = chartprops->getUranianConfig();

	for ( GRADKREIS gk = GK_360; gk <= cfg.gradkreis; gk++ )
	{
		dist = a_red( len2 - len1, circledeg );
		//printf( "UranianExpert::calculateMatch len1 %f len2 %f dist %f\n", len1, len2, *dist );
		if ( dist > .5 * circledeg ) dist -= circledeg;

		if ( fabs ( dist ) <= orbis )
		{
			return UranianMatch( gk, dist, fabs( len1 - len2 - dist ));
		}
		circledeg /= 2.0;
	}
	return UranianMatch( GK_NONE );
}

/*****************************************************
**
**   UranianExpert   ---   calculateUEventDueDate
**
******************************************************/
void UranianExpert::calculateUEventDueDate( UEvent &e, const double &radixsun )
{
	double targetpos, solarjd;
	const bool vedic = chartprops->isVedic();
	//printf( "len1 %f len2 %f orbis %f residuum %f\n", e.e1->longitude, e.e2->longitude, e.orbis, e.residuum );
	//printf( "DUEDATE date %s\n", str2char( formatter->getDateStringFromJD(  e.duedate )));

	const Horoscope *hradix = result[PcRadix]->horoscope;

	DataSet workingDs = *horoscope->getDataSet();
	workingDs.setDate( e.duedate );

	if ( e.e1->context == PcRadix && e.e2->context == PcPeriod )
	{
		targetpos = radixsun - e.orbis;

		//printf( "P1 radix, p2 ist targetpos %f \n", targetpos );
		solarjd = calculator->calcPlanetaryEvent( &workingDs, targetpos, OSUN, vedic );
		//printf( "DUEDATE 2 date %s\n", str2char( formatter->getDateStringFromJD(  solarjd )));
		e.duedate = hradix->getJD() + year_length * ( solarjd - hradix->getJD() );
		//printf( "DUEDATE 3 date %s\n", str2char( formatter->getDateStringFromJD(  e.duedate )));
	}
	else if ( e.e2->context == PcRadix && e.e1->context == PcPeriod )
	{
		targetpos = radixsun - e.orbis;

		//printf( "P1 radix, p2 ist targetpos %f \n", targetpos );
		solarjd = calculator->calcPlanetaryEvent( &workingDs, targetpos, OSUN, vedic );
		//printf( "DUEDATE 2 date %s\n", str2char( formatter->getDateStringFromJD(  solarjd )));
		e.duedate = hradix->getJD() + year_length * ( solarjd - hradix->getJD() );
		//printf( "DUEDATE 3 date %s\n", str2char( formatter->getDateStringFromJD(  e.duedate )));
	}
	//else printf( "Other: %d - %d\n", e.e1->context, e.e2->context );
}

/*****************************************************
**
**   UranianExpert   ---   calculateYearlyPreview
**
******************************************************/
void UranianExpert::calculateYearlyPreview( const int &year )
{
	previewYear = year;
	const double first_jd = calculator->calc_jd( year, 1, 1, 0.0 );
	const double last_jd = calculator->calc_jd( year + 1, 1, 1, 0.0 );

	calculatePeriodPreview( first_jd, last_jd );
}

/*****************************************************
**
**   UranianExpert   ---   calculatePeriodPreview
**
******************************************************/
void UranianExpert::calculatePeriodPreview( const double &first_jd, const double &last_jd )
{
	double first_sunpos, last_sunpos, dummy;
	const bool vedic = chartprops->isVedic();

	DataSet workingDs = *horoscope->getDataSet();

	const double birth_jd = horoscope->getJD();
	const double first_sjd = birth_jd + ( first_jd - birth_jd ) / year_length;
	workingDs.setDate( first_sjd );
	calculator->calcPosition( &workingDs, OSUN, first_sunpos, dummy, true, vedic );

	const double last_sjd = birth_jd + ( last_jd - birth_jd ) / year_length;
	workingDs.setDate( last_sjd );
	calculator->calcPosition( &workingDs, OSUN, last_sunpos, dummy, true, vedic );
	const double bogen = red_deg( last_sunpos - first_sunpos );

	const double mid_sunpos = red_deg( first_sunpos + .5 * bogen );
	const double orbis = .5 * ( last_sunpos - first_sunpos );

	workingDs.setDate( 0.5 * ( first_sjd + last_sjd ));
	const double mid_sjd = calculator->calcPlanetaryEvent( &workingDs, mid_sunpos, OSUN, vedic );
	const double mid_jd = birth_jd + year_length * ( mid_sjd - birth_jd );

	initPeriod();
	hperiod->setLocation( *horoscope->getDataSet()->getLocation() );

	hperiod->setDate( mid_jd );
	hperiod->calcTransitPositions( horoscope, mid_jd, vedic, year_length, PcSolarArc );
	hperiod->setDate( mid_sjd );

	findMatchingEvents( PcRadix, PcPeriod, PcPeriod, orbis );

	for( std::list<UEvent>::iterator iter = result[PcPeriod]->uevents.begin(); iter != result[PcPeriod]->uevents.end(); iter++ )
	{
		calculateUEventDueDate( *iter, hperiod->getLongitude( OSUN, vedic ));
	}
	result[PcPeriod]->uevents.sort( UEventSorter( AS_DATE, vedic ));
}

