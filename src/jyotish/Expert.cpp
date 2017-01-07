/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Expert.cpp
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

#include "Expert.h"
#include "astrobase.h"
#include "Horoscope.h"
#include "Varga.h"

/*****************************************************
**
**   Expert   ---   Constructor
**
******************************************************/
Expert::Expert( Horoscope *h )
		: horoscope( h )
{
}

/*****************************************************
**
**   VedicExpert   ---   Constructor
**
******************************************************/
VedicExpert::VedicExpert( Horoscope *h, const Varga &varga )
 : Expert( h ),
	varga( varga )
{
}

/*****************************************************
**
**   VedicExpert   ---   getRasi
**
******************************************************/
Rasi VedicExpert::getRasi( const ObjectId &p )
{
	return horoscope->getVargaData( p, varga )->getRasi();
}

/*****************************************************
**
**   VedicExpert   ---   getRasiLongitude
**
******************************************************/
double VedicExpert::getRasiLongitude( const ObjectId &p )
{
	const bool reverse = IS_LUNAR_NODE( p );
	return ::getRasiLen( horoscope->getVedicLongitude( p ), reverse );
}

