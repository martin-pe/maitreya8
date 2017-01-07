/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/BasicHoroscope.cpp
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

#include "BasicHoroscope.h"

#include "Calculator.h"
#include "Conf.h"
#include "maitreya.h"
#include "mathbase.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <wx/log.h>
#include <wx/string.h>

extern Config *config;

IMPLEMENT_CLASS( BasicHoroscope, wxEvtHandler )

/*****************************************************
**
**   ObjectPosition   ---   Constructor
**
******************************************************/
ObjectPosition::ObjectPosition( const double l, const double b, const double s, const MOVING_DIRECTION d )
 : longitude( l ),
 	latitude( b ),
	speed( s ),
	direction( d )
{
}

/*****************************************************
**
**   BasicHoroscope   ---   Constructor
**
******************************************************/
BasicHoroscope::BasicHoroscope()
{
	int i;
	TzUtil tzu;
	context = PcRadix;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		object_len[i] = 0;
		object_lat[i] = 0;
		object_speed[i] = 1;
	}
	for ( i = HOUSE1; i <= HOUSE12; i++ ) whousecusp[i] = ihousecusp[i] = ihousesandhi[0] = 0;

	TzInfo info = tzu.getCurrentTzInfo();

	dataSet = new FileDataSet;
	Location location = *config->defaultLocation;
	location.setTimeZone( info.tzhours );
	location.setDST( info.dsthours );
	dataSet->setLocation( location );
}

/*****************************************************
**
**   BasicHoroscope   ---   Destructor
**
******************************************************/
BasicHoroscope::~BasicHoroscope()
{
	delete dataSet;
}

/*****************************************************
**
**   BasicHoroscope   ---   updatePlanets
**
** update of Ayanamsa, object_len etc and house longitude
**
******************************************************/
void BasicHoroscope::updatePlanets()
{
	int i;
	double r[12], houselen;
	Calculator *calculator = CalculatorFactory().getCalculator();

	// Ayanamsa
	iayanamsa_type = config->vedicCalculation->ayanamsa;
	wayanamsa_type = config->westernCalculation->ayanamsa;
	iayanamsa = calculator->calcAyanamsa( getJD(), iayanamsa_type );
	wayanamsa = calculator->calcAyanamsa( getJD(), wayanamsa_type );

	// Planetary positions
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		switch( i )
		{
			case OMEANDESCNODE:
			case OTRUEDESCNODE:
			case OASCENDANT:
			case OMERIDIAN:
			case ODESCENDANT:
			case OIMUMCOELI:
				continue;
			break;
			default:
				calculator->calcPositionSpeed( dataSet, i, object_len[i], object_lat[i], object_speed[i] );
			break;
		}
	}
	object_len[OMEANDESCNODE] = red_deg( object_len[OMEANNODE] + 180 );
	object_len[OTRUEDESCNODE] = red_deg( object_len[OTRUENODE] + 180 );
	object_speed[OMEANDESCNODE] = object_speed[OMEANNODE];
	object_speed[OTRUEDESCNODE] = object_speed[OTRUENODE];

	// House calculation
	calculator->calcHouses( getJD(), getLongitude(), getLatitude(), config->westernCalculation->houseSystem, r );
	for ( i = HOUSE1; i <= HOUSE12; i++ ) whousecusp[i] = red_deg( r[i] - wayanamsa );

	if ( config->vedicCalculation->houseSystem != config->westernCalculation->houseSystem )
		calculator->calcHouses( getJD(), getLongitude(), getLatitude(), config->vedicCalculation->houseSystem, r );

	for ( i = HOUSE1; i <= HOUSE12; i++ ) ihousecusp[i] = red_deg( r[i] - iayanamsa );
	for ( i = HOUSE1; i <= HOUSE12; i++ )
	{
		houselen = red_deg( ihousecusp[i<11 ? i+1 : 0] - ihousecusp[i] );
		ihousesandhi[i] = red_deg( ihousecusp[i] + .5 * houselen );
	}
	object_len[OASCENDANT] = r[0];
	object_len[OMERIDIAN] = r[9];
	object_len[ODESCENDANT] = r[6];
	object_len[OIMUMCOELI] = r[3];

	siderealTime = calculator->calcSiderealTime( getJD(), getLongitude() );

	updatePositionArrays();
}

/*****************************************************
**
**   BasicHoroscope   ---   updatePositionArrays
**
** updates the arrays vpos and wpos
** integrated in updatePlanets, only once called by TransitHoroscope
**
******************************************************/
void BasicHoroscope::updatePositionArrays()
{
	// Setup Object position structs
	for ( int i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		vpos[i].longitude   = wpos[i].longitude   = object_len[i];
		vpos[i].latitude = wpos[i].latitude = object_lat[i];
		vpos[i].speed    = wpos[i].speed    = object_speed[i];
		if ( i != OMEANNODE && i != OMEANDESCNODE )
			vpos[i].direction = wpos[i].direction = ( object_speed[i] >0 ? MD_DIRECT : MD_RETROGRADE );

		// p.longitude =0 can happen for planetoids etc. without data files.
		// Do not adjust Ayanamsa in that case, so 0 Aries position will be quite obvious.
		if ( object_len[i] != 0 )
		{
			vpos[i].longitude = red_deg( vpos[i].longitude - iayanamsa );
			wpos[i].longitude = red_deg( wpos[i].longitude - wayanamsa );
		}
	}
}

/*****************************************************
**
**   BasicHoroscope   ---   getObjectPosition
**
******************************************************/
ObjectPosition BasicHoroscope::getObjectPosition( const ObjectId &planet, const bool &vedic ) const
{
	if ( IS_EPHEM_OBJECT( planet )) return( vedic ? vpos[planet] : wpos[planet] );
	else if ( IS_HOUSE_OBJECT( planet )) return ObjectPosition( getHouse( planet-OHOUSE1, vedic, ! config->vedicCalculation->houseUseCusps ));
	else assert( false );
}

/*****************************************************
**
**   BasicHoroscope   ---   getLongitude
**
******************************************************/
double BasicHoroscope::getLongitude( const ObjectId &planet, const bool &vedic ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, vedic ).longitude;
}

/*****************************************************
**
**   BasicHoroscope   ---   isRetrograde
**
******************************************************/
bool BasicHoroscope::isRetrograde( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, true ).direction == MD_RETROGRADE;
}

/*****************************************************
**
**   BasicHoroscope   ---   getMovingDirection
**
******************************************************/
MOVING_DIRECTION BasicHoroscope::getMovingDirection( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, true ).direction;
}

/*****************************************************
**
**   BasicHoroscope   ---   getTropicalLongitude
**
******************************************************/
double BasicHoroscope::getTropicalLongitude( const ObjectId &planet ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet )
	return object_len[planet];
}

/*****************************************************
**
**   BasicHoroscope   ---   getLatitude
**
******************************************************/
double BasicHoroscope::getLatitude( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, true ).latitude;
}

/*****************************************************
**
**   BasicHoroscope   ---   getSpeed
**
******************************************************/
double BasicHoroscope::getSpeed( const ObjectId &planet ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet )
	return object_speed[planet];
}

/*****************************************************
**
**   BasicHoroscope   ---   getHouse
**
******************************************************/
double BasicHoroscope::getHouse( const int& housenb, const bool& vedic, const bool getsandhi ) const
{
	ASSERT_VALID_HOUSE( housenb )
	if ( vedic ) {
		return( getsandhi ? ihousesandhi[housenb] : ihousecusp[housenb] );
	}
	else {
		//if ( getsandhi ) wxLogError( wxT( "Sandhis for western houses not supported, using cusp" ));
		return whousecusp[housenb];
	}
}

/*****************************************************
**
**   BasicHoroscope   ---   getHouseForLongitude
**
******************************************************/
double BasicHoroscope::getHouse4Longitude( const double &len, const bool &vedic ) const
{
	//printf( "BasicHoroscope::getHouse4Longitude len %f\n", len );
	double hstart, hend, hlen, hpart, ret;
	for ( int i = HOUSE1; i <= HOUSE12; i++ )
	{
		if ( ! vedic )
		{
			hstart = whousecusp[i];
			hend = whousecusp[red12(i+1)];
		}
		else
		{
			if ( config->vedicCalculation->houseUseCusps )
			{
				hstart = ihousecusp[i];
				hend = ihousecusp[red12(i+1)];
			}
			else
			{
				hstart = ihousesandhi[red12( i -1)];
				hend = ihousesandhi[red12(i)];
			}
		}
		//printf( "hstart %f end %f\n", hstart, hend );
		if (( hstart <= len && len < hend ) || ( hstart > hend && ( len >= hstart || len < hend )))
		{
			hlen = red_deg( hend - hstart );
			hpart = red_deg( len - hstart );
			ret = i + hpart / hlen;
			//printf( "BasicHoroscope::getHouse4Longitude len %f vedic %d hstart %f hend %f hlen %f hpart %f ret %f\n", len, vedic, hstart, hend, hlen, hpart, ret );
			assert( ret >= 0 && ret < 12 );
			return ret;
		}
	}
	assert( 0 );

	return 0;
}

/*****************************************************
**
**   BasicHoroscope   ---   getHousePos
**
******************************************************/
int BasicHoroscope::getHousePos( const ObjectId &planet, const bool &vedic ) const
{
	// TODO store house longitudes on update() and return that instead of repeated calculation
	return (int)getHouse4Longitude( getLongitude( planet, vedic), vedic );
}

/*****************************************************
**
**   BasicHoroscope   ---   openFile
**
******************************************************/
bool BasicHoroscope::openFile( wxString filename )
{
	return dataSet->openFile( filename );
}

/*****************************************************
**
**   BasicHoroscope   ---   saveAs
**
******************************************************/
bool BasicHoroscope::saveAs( wxString filename )
{
	return dataSet->saveAs( filename );
}

/*****************************************************
**
**   BasicHoroscope   ---   save
**
******************************************************/
bool BasicHoroscope::save()
{
	return dataSet->save();
}


