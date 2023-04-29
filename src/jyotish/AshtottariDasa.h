/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/AshtottariDasa.h
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
#ifndef _ASHTOTTARIDASA_H_
#define _ASHTOTTARIDASA_H_

#include "maitreya.h"
#include "Dasa.h"

class Horoscope;

/*************************************************//**
*
* \brief calculation of Ashtottari Dasa
*
******************************************************/
class AshtottariDasaExpert : public DasaExpert
{
public:

	AshtottariDasaExpert( Horoscope *h ) : DasaExpert( D_ASHTOTTARI, h ) {}

	virtual std::vector<Dasa*> getFirstLevel();
	virtual std::vector<Dasa*> getNextLevel( Dasa* );

	virtual std::vector<KPEvent> getKPEventList( const double &startlen, const double &endlen, const double &startjd );
	virtual KpData getKPLords( const double& ) { return KpData(); }

	virtual int getParamayus() const { return 108; }

	virtual bool isGrahaDasaExpert() const { return true; }

private:
	const static int dasa_lord[8];
	const static int dasa_dur[8];
};

#endif

