/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VargaBase.cpp
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

#include "VargaBase.h"

#include <wx/log.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <math.h>

#include "Conf.h"
#include "maitreya.h"
#include "Lang.h"

using namespace std;

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
wxString VargaConfigLoader::getVargaName( const Varga &i )
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
**   VargaConfigLoader   ---   getVargaDivision
**
******************************************************/
int VargaConfigLoader::getVargaDivision( const Varga &i ) const
{
	ASSERT_VALID_VARGA( i );
	return vargaDivisions[i];
}



