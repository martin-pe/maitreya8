/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VedicPlanet.h
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


