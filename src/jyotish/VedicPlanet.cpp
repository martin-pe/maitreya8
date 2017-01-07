/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VedicPlanet.cpp
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

#include "VedicPlanet.h"

#include "astrobase.h"
#include "Conf.h"
#include "maitreya.h"
#include "Horoscope.h"
#include "Lang.h"
#include "Varga.h"

extern Config *config;

/*****************************************************
**
**   VedicPlanet   ---   Constructor
**
******************************************************/
VedicPlanet::VedicPlanet( Horoscope *h, const ObjectId &pindex )
		: horoscope( h ),
		index( pindex )
{
	longitude = 0;
	for ( int i = 0; i < 4; i++ )
	{
		vimsopakaBala[i] = 0;
		vimsopakaBalaGoodVargas[i] = 0;
	}
	averageVimsopakaBala = 0;
	//for ( Varga division = V_RASI; division < NB_VARGAS; division++ ) vargadata[(int)division] = new VargaData( this, division );
	for ( int division = (int)V_RASI; division < NB_VARGAS; division++ ) vargadata[division] = new VargaData( this, (Varga)division );
}

/*****************************************************
**
**   VedicPlanet   ---   Destructor
**
******************************************************/
VedicPlanet::~VedicPlanet()
{
	for ( int i = 0; i < NB_VARGAS; i++ ) delete vargadata[i];
}

/*****************************************************
**
**   VedicPlanet   ---   getVargaData
**
******************************************************/
VargaData *VedicPlanet::getVargaData( const Varga &v ) const
{
	ASSERT_VALID_VARGA( v );
	return vargadata[v];
}

/*****************************************************
**
**   VedicPlanet   ---   getVimsopakaBalaGoodVargas
**
******************************************************/
int VedicPlanet::getVimsopakaBalaGoodVargas( const int &i ) const
{
	assert( i >= 0 && i < 4 );
	return vimsopakaBalaGoodVargas[i];
}

/*****************************************************
**
**   VedicPlanet   ---   getRasi
**
******************************************************/
int VedicPlanet::getRasi()
{
	return ::getRasi( longitude );
}

/*****************************************************
**
**   VedicPlanet   ---   getVimsopakaBala
**
******************************************************/
double VedicPlanet::getVimsopakaBala( const int &i ) const
{
	assert( i >= 0 && i < 4 );
	return vimsopakaBala[i];
}

/*****************************************************
**
**   VedicPlanet   ---   update
**
******************************************************/
void VedicPlanet::update( const double &len )
{
	updateVargaData( len );
}

/*****************************************************
**
**   VedicPlanet   ---   updateVargaData
**
******************************************************/
void VedicPlanet::updateVargaData( const double &len )
{
	VargaExpert expert( horoscope );
	longitude = len;

	//for ( Varga division = V_RASI; division < NB_VARGAS; division++ )
	for ( int division = (int)V_RASI; division < NB_VARGAS; division++ )
	{
		vargadata[(int)division]->rasi = expert.calcVarga( longitude, (Varga)division );
	}
}

/*****************************************************
**
**   VedicPlanet   ---   calculateDignities
**
******************************************************/
void VedicPlanet::calculateDignities()
{
	if ( index > OSATURN ) return;

	assert( horoscope );
	VargaExpert expert( horoscope );
	DIGNITY dignity = DG_NONE;
	FRIENDSHIP friendship = PF_NONE;
	int vargaViswa = 0;
	int bala = 0;
	FRIENDSHIP tempFriend, planetaryFriend;
	Rasi rasi, tfrasi, tflordrasi;

	for ( Varga varga = V_FIRST; varga <= V_LAST; varga++ )
	{
		dignity = DG_NONE;
		rasi = horoscope->getVargaData( index, varga )->getRasi();

		// tempFriendMode  0 = Rasi, 1 = Varga
		tfrasi = horoscope->getVargaData( index, config->vedicCalculation->tempFriendMode == 1 ? varga : V_RASI )->getRasi();
		tflordrasi = horoscope->getVargaData( getLord( rasi ), config->vedicCalculation->tempFriendMode == 1 ? varga : V_RASI )->getRasi();
		tempFriend = getRasiTemporaryFriendship( tfrasi, tflordrasi );
		//printf( "Planet %d Varga: %d rasi %d tfrasi %d tflord %d tflordrasi %d, tempFriend %d config %d\n",
		//pindex, division, rasi, tfrasi, tflord, tflordrasi, tempFriend, config->tempFriendBasedOnVarga );
		planetaryFriend = getPlanetaryFriendship( index, getLord( rasi));

		//enum FRIENDSHIP { PF_NONE = -1, PF_ENEMY = 0, PF_NEUTRAL, PF_FRIEND, PF_SWORN_ENEMY, PF_BEST_FRIEND };

		// Adhi Mitra
		if ( planetaryFriend == PF_FRIEND && tempFriend == PF_FRIEND )
		{
			friendship = PF_BEST_FRIEND;
			dignity = DG_ADHIMITRA;
			vargaViswa = 18;
			bala = 20;
		}

		// Mitra
		else if ( planetaryFriend == PF_NEUTRAL && tempFriend == PF_FRIEND )
		{
			friendship = PF_FRIEND;
			dignity = DG_MITRA;
			vargaViswa = 15;
			bala = 15;
		}

		// Sama
		else if (( planetaryFriend == PF_FRIEND && tempFriend == PF_ENEMY ) || ( planetaryFriend == PF_ENEMY && tempFriend == PF_FRIEND ))
		{
			friendship = PF_NEUTRAL;
			dignity = DG_SAMA;
			vargaViswa = 10;
			bala = 10;
		}

		// Satru
		else if ( planetaryFriend == PF_NEUTRAL && tempFriend == PF_ENEMY )
		{
			friendship = PF_ENEMY;
			dignity = DG_SATRU;
			vargaViswa = 7;
			bala = 4;
		}

		// Adhi Satru
		else if ( planetaryFriend == PF_ENEMY && tempFriend == PF_ENEMY )
		{
			friendship = PF_SWORN_ENEMY;
			dignity = DG_ADHISATRU;
			vargaViswa = 5;
			bala = 2;
		}

		/*
		* NB: Moon in Taurus is exalted AND in Moolatrikona (Varga Chart, e.g. D-9). So exaltation will prevail because of order below.
		* Same for Saturn in Aquarius (own and Moolatrikona): Moolatrikona will prevail.
		*/
		// Own sign
		if ( index == getLord( rasi ))
		{
			dignity = DG_SVAKSHETRA;
			vargaViswa = 20;
		}

		if ( hasExaltationRasi( index, rasi )) {
			dignity = DG_EXALTED;
		}

		// Special handling for Moolatrikona: Rasi depends on exact degree, other Vargas only on sign position
		if ( varga == V_RASI )
		{
			if ( ::isInMulatrikona( index, horoscope->getVedicLongitude( index )))
			{
				dignity = DG_MOOLATRIKONA;
				bala = 45;
			}
		}
		else
		{
			if ( horoscope->getVargaData( index, varga )->getRasi() == getMulatrikonaRasi( index ))
			{
				dignity = DG_MOOLATRIKONA;
				bala = 45;
			}
		}

		// Debilation
		if ( hasDebilationRasi( index, rasi )) {
			dignity = DG_DEBILATED;
		}

		assert( dignity != DG_NONE );
		vargadata[varga]->dignity = dignity;
		vargadata[varga]->friendship = friendship;
		vargadata[varga]->vargaViswa = vargaViswa;
		vargadata[varga]->saptaVargajaBala = bala;
	}
	calculateVimsopakaBala();
}

/*****************************************************
**
**   VedicPlanet   ---   calculateVimsopakaBala
**
******************************************************/
void VedicPlanet::calculateVimsopakaBala()
{
	int i;
	const static Varga shadvarga_vargas[6] = { V_RASI, V_HORA, V_DREKKANA, V_NAVAMSA, V_DVADASAMSA, V_TRIMSAMSA };
	const static Varga saptavarga_vargas[7] = { V_RASI, V_HORA, V_DREKKANA, V_NAVAMSA, V_DVADASAMSA, V_TRIMSAMSA, V_SAPTAMAMSA };
	const static Varga dasavarga_vargas[10] = { V_RASI, V_HORA, V_DREKKANA, V_NAVAMSA, V_DVADASAMSA, V_TRIMSAMSA,
		V_SAPTAMAMSA, V_DASAMSA, V_SHODASAMSA, V_SHASTIAMSA
	};
	const static Varga shodasavarga_vargas[16] = { V_RASI, V_HORA, V_DREKKANA, V_NAVAMSA, V_DVADASAMSA, V_TRIMSAMSA,
		V_SAPTAMAMSA, V_DASAMSA, V_SHODASAMSA, V_SHASTIAMSA, V_VIMSAMSA, V_SIDDHAMSA, V_BHAMSA,
		V_CHATURTAMSA, V_CHATVARIMSAMSA, V_AKSHAVEDAMSA
	};

	const static double shadvarga_weight[6] = { 6, 2, 4, 5, 2, 1 };
	const static double saptavarga_weight[7] = { 5, 2, 3, 2.5, 4.5, 2, 1 };
	const static double dasavarga_weight[10] = { 3, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 5 };
	const static double shodasavarga_weight[16] = { 3.5, 1, 1, 3, .5, 1, .5, .5, 2, 4, .5, .5, .5, .5, .5, .5  };

	for ( i = 0; i < 4; i++ )
	{
		vimsopakaBala[i] = 0;
		vimsopakaBalaGoodVargas[i] = 0;
	}

	for ( i = 0; i < 6; i++ )  addVimsopakaValue( shadvarga_weight[i], shadvarga_vargas[i], 0 );
	for ( i = 0; i < 7; i++ )  addVimsopakaValue( saptavarga_weight[i], saptavarga_vargas[i], 1 );
	for ( i = 0; i < 10; i++ )	addVimsopakaValue( dasavarga_weight[i], dasavarga_vargas[i], 2 );
	for ( i = 0; i < 16; i++ )	addVimsopakaValue( shodasavarga_weight[i], shodasavarga_vargas[i], 3 );

	averageVimsopakaBala = .25 * ( vimsopakaBala[0] + vimsopakaBala[1] + vimsopakaBala[2] + vimsopakaBala[3] );
}

/*****************************************************
**
**   VedicPlanet   ---   addVimsopakaValue
**
******************************************************/
void VedicPlanet::addVimsopakaValue( const double &weight, const Varga& varga, const int &vindex )
{
	double bala;
	const double minbala = 20;

	assert( vindex >= 0 && vindex < 4 );
	assert( varga >= 0 && varga < NB_VARGAS );
	assert( weight > 0 && weight < 10 );

	bala = weight * getVargaData( varga )->getVargaViswa();
	vimsopakaBala[vindex] += .05 * bala;
	if ( getVargaData( varga )->getVargaViswa() >= minbala ) vimsopakaBalaGoodVargas[vindex]++;
}
