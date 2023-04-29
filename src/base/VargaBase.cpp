/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VargaBase.cpp
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

#include "VargaBase.h"

#include <wx/log.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <math.h>

#include "Conf.h"
#include "maitreya.h"
#include "Lang.h"

extern Config *config;

const int VargaConfigLoader::vargaDivisions[NB_VARGAS] =
{
	1, 9, 2, 3, 4,
	6, 7, 8, 10, 12,
	16, 20, 24, 27, 30,
	40, 45, 60, 108, 144,
	0
};

IMPLEMENT_SINGLETON( VargaConfigLoader )

/*****************************************************
**
**   VargaConfigLoader   ---   init
**
******************************************************/
void VargaConfigLoader::init()
{
	for( Varga v = V_FIRST; v <= V_LAST; v++ )
	{
		if ( config->vedicCalculation->vargaSignifications.size() <= (uint)v )
		{
			config->vedicCalculation->vargaSignifications.push_back( getDefaultVargaSignification( v ));
		}
	}
	assert( config->vedicCalculation->vargaSignifications.size() == NB_VARGAS );
}

/**************************************************************
***
**  VargaConfigLoader   ---   getVargaName
***
***************************************************************/
wxString VargaConfigLoader::getVargaName( const Varga &i ) const
{
	ASSERT_VALID_VARGA( i );
	const wxString vargaNames[NB_VARGAS] =
	{
		_( "Rasi" ), _( "Navamsa" ), _( "Hora" ), _( "Drekkana" ), _( "Chaturtamsa" ), _( "Sastamsa" ),
		_( "Saptamamsa" ), _( "Ashtamsa" ), _( "Dasamsa" ), _( "Dvadasamsa" ), _( "Shodasamsa" ),
		_( "Vimsamsa" ), _( "Siddhamsa" ), _( "Bhamsa" ), _( "Trimsamsa" ),
		_( "Chatvarimsamsa" ), _( "Akshavedamsa" ), _( "Shastiamsa" ),
		_( "Ashtottaramsa" ), _( "Dvadas-Davadamsa" ), _( "Bhava" )
	};
	return vargaNames[i];
}

/**************************************************************
***
**  VargaConfigLoader   ---   getVargaSignification
***
***************************************************************/
wxString VargaConfigLoader::getVargaSignification( const Varga &i ) 
{
	ASSERT_VALID_VARGA( i );
	if( config->vedicCalculation->vargaSignifications.size() == 0 ) init();
	return config->vedicCalculation->vargaSignifications[i];
}

/**************************************************************
***
**  VargaConfigLoader   ---   getDefaultVargaSignification
***
***************************************************************/
wxString VargaConfigLoader::getDefaultVargaSignification( const Varga &i ) 
{
	ASSERT_VALID_VARGA( i );
	const wxString defaultSignifications[NB_VARGAS] =
	{
		_( "Physique" ), _( "Marriage" ), _( "Wealth" ), _( "Coborn" ), _( "Fortunes" ),
		_( "Health" ), _( "Children" ), _( "Troubles" ), _( "Profession" ), _( "Parents" ),
		_( "Conveyances" ), _( "Spirituality" ), _( "Learning" ), _( "Strength" ), _( "Evils" ),
		_( "Maternal Legacy" ), _( "Paternal Legacy" ), _( "Past Karma" ),
		wxEmptyString, // D-108
		wxEmptyString, // D-144
		wxEmptyString  // Bhava
	};
	return defaultSignifications[i];
}

/*****************************************************
**
**   VargaConfigLoader   ---   getVargaIndexByDivision
**
******************************************************/
Varga VargaConfigLoader::getVargaIndexByDivision( const int &div ) const
{
	Varga j = V_FIRST;

	// nonsense or non-initialized
	assert(( div >= 0 ) || ( div <= 144 ));

	while ( j <= V_LAST )
	{
		if ( vargaDivisions[j] == div ) return j;
		j++;
	}
	//assert( false );
	return V_NONE;
}

/*****************************************************
**
**   VargaConfigLoader   ---   getVargaIndexByDivisionString
**
******************************************************/
Varga VargaConfigLoader::getVargaIndexByDivisionString( const wxString s ) const
{
	long l;
	bool b = s.ToLong( &l );
	if ( b )
	{
		return getVargaIndexByDivision( (int)l );
	}
	for( Varga v = V_FIRST; v <= V_LAST; v++ )
	{
		if ( s.CmpNoCase( getVargaName( v )) == 0 ) return v;
	}
	return V_NONE;
}

/*****************************************************
**
**   VargaConfigLoader   ---   getVargaDivision
**
******************************************************/
int VargaConfigLoader::getVargaDivision( const Varga &i ) const
{
	ASSERT_VALID_VARGA( i );
	return vargaDivisions[i];
}



