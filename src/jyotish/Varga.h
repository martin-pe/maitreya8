/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Varga.h
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

#ifndef _VARGA_H_
#define _VARGA_H_

#include <wx/string.h>
#include <vector>

#include "Expert.h"
#include "maitreya.h"

class Horoscope;
class Sheet;
class VedicPlanet;

using namespace std;

/*************************************************//**
*
* \brief Varga properties of a single planet
*
******************************************************/
class VargaData
{
	friend class VedicPlanet;

public:
	VargaData( const VedicPlanet*, const Varga& );

	int getDivision() const { return division; }

	Rasi getRasi() const { return rasi; }

	int getVargaViswa() const { return vargaViswa; }

	int getSaptaVargajaBala() const { return saptaVargajaBala; }

	DIGNITY getDignity() const { return dignity; }

	FRIENDSHIP getFriendship() const { return friendship; }

private:

	const VedicPlanet *planet;
	const int division;

	Rasi rasi;
	int vargaViswa, saptaVargajaBala;
	DIGNITY dignity;
	FRIENDSHIP friendship;
};

/*************************************************//**
*
* \brief encapsulates calculation of Vargas
*
******************************************************/
class VargaExpert : public Expert
{
public:

	VargaExpert() : Expert() {}
	VargaExpert( Horoscope *h ) : Expert( h ) {}

	Rasi calcVarga( const double& len, const Varga& );

	virtual void write( Sheet*, const OBJECT_INCLUDES &style, const bool show_header = true );

private:

	// Dvadasamsa calculation plays a special role an is used by D-12, D-108 and D-144
	double getDvadasamsaLongitude( const double& ) const;
};

#endif


