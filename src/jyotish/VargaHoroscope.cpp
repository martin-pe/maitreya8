/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VargaHoroscope.cpp
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

#include "VargaHoroscope.h"

#include "astrobase.h"
#include "Conf.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "Varga.h"

extern Config *config;

/*****************************************************
**
**   AbstractVargaHoroscope   ---   AbstractVargaHoroscope
**
******************************************************/
int AbstractVargaHoroscope::getBhava( const ObjectId &planet ) const
{
	assert( planet >= 0 && planet < MAX_EPHEM_OBJECTS );
	int r1 = this->getRasi( planet );
	int r2 = this->getRasi( OASCENDANT );
	if ( r1 < 0 || r2 < 0 ) return -1;
	return red12( r1 - r2 );
}

/*****************************************************
**
**   VargaHoroscope   ---   Constructor
**
******************************************************/
VargaHoroscope::VargaHoroscope( Horoscope *h, const Varga v )
		: horoscope( h ),	varga( v ) {}

/*****************************************************
**
**   VargaHoroscope   ---   setVarga
**
******************************************************/
void VargaHoroscope::setVarga( const Varga &v )
{
	ASSERT_VALID_VARGA( v );
	varga = v;
}

/*****************************************************
**
**   VargaHoroscope   ---   getRasiLongitude
**
******************************************************/
double VargaHoroscope::getRasiLongitude( const ObjectId &planet ) const
{
	assert( planet >= 0 && planet < MAX_EPHEM_OBJECTS && horoscope );
	const int inode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const int descnode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;
	// reverse for rahu and ketu
	return ::getRasiLen( horoscope->getVedicLongitude( planet ), planet == inode || planet == descnode );
}

/*****************************************************
**
**   VargaHoroscope   ---   getRasi
**
******************************************************/
Rasi VargaHoroscope::getRasi( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	assert( horoscope );
	return horoscope->getVargaData( planet, varga )->getRasi();
}

/*****************************************************
**
**   VargaHoroscope   ---   getNakshatra27
**
******************************************************/
int VargaHoroscope::getNakshatra27( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	assert( horoscope );
	return ::getNakshatra27( horoscope->getObjectPosition( planet, true ).longitude );
}

/*****************************************************
**
**   VargaHoroscope   ---   getNakshatra28
**
******************************************************/
int VargaHoroscope::getNakshatra28( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	assert( horoscope );
	return ::getNakshatra28( horoscope->getObjectPosition( planet, true ).longitude );
}

/*****************************************************
**
**   VargaHoroscope   ---   isRetrograde
**
******************************************************/
bool VargaHoroscope::isRetrograde( const ObjectId &planet ) const
{
	return horoscope->isRetrograde( planet );
}

/*****************************************************
**
**   VargaHoroscope   ---   isBenefic
**
******************************************************/
bool VargaHoroscope::isBenefic( const ObjectId &p ) const
{
	return horoscope->isBenefic( p );
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   Constructor
**
******************************************************/
MinimalVargaHoroscope::MinimalVargaHoroscope()
{
	cleanup();
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   cleanup
**
******************************************************/
void MinimalVargaHoroscope::cleanup()
{
	int i;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) ephem_objects_rasi[i] = R_NONE;
	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ ) extended_objects_rasi[i] = R_NONE;
	for ( i = 0; i < MAX_ARABIC_OBJECTS; i++ ) arabic_objects_rasi[i] = R_NONE;
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   setRasi
**
******************************************************/
void MinimalVargaHoroscope::setRasi( const ObjectId &planet, const Rasi &rasi )
{
	ASSERT_VALID_EPHEM_OBJECT( planet );
	ASSERT_VALID_EXTENDED_RASI( rasi );
	ephem_objects_rasi[planet] = rasi;
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   getRasi
**
******************************************************/
Rasi MinimalVargaHoroscope::getRasi( const ObjectId &planet ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet );
	return ephem_objects_rasi[planet];
}

