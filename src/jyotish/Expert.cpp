/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Expert.cpp
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

