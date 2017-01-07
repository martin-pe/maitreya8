/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VedicPlanet.h
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
#ifndef _VEDICPLANET_H_
#define _VEDICPLANET_H_

#include "Varga.h"

class Horoscope;
class VargaData;

/*************************************************//**
*
* \brief holds additional data like dignity and Vimsopaka Bala for a single planet
*
******************************************************/
class VedicPlanet
{
public:
	VedicPlanet( Horoscope*, const ObjectId &index );
	~VedicPlanet();

	void update( const double &len );
	void calculateDignities();

	ObjectId getIndex() const { return index; }
	double getLongitude() const { return longitude; }
	int getRasi();

	VargaData *getVargaData( const Varga& ) const;
	int getVimsopakaBalaGoodVargas( const int& ) const;

	double getVimsopakaBala( const int& ) const;
	double getAverageVimsopakaBala() const { return averageVimsopakaBala; }

private:

	void calculateVimsopakaBala();
	void addVimsopakaValue( const double &weight, const Varga&, const int &vindex );
	void updateVargaData( const double &len );

	Horoscope *horoscope;
	const ObjectId index;

	double longitude;
	VargaData *vargadata[NB_VARGAS];
	double vimsopakaBala[4];
	int vimsopakaBalaGoodVargas[4];
	double averageVimsopakaBala;
};

#endif


