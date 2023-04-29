/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Varga.h
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

#ifndef _VARGA_H_
#define _VARGA_H_

#include <wx/string.h>

#include "Expert.h"
#include "maitreya.h"

class Horoscope;
class Sheet;
class VedicPlanet;

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


