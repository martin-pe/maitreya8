/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VimsottariDasa.h
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

#ifndef _VIMSOTTARIDASA_H_
#define _VIMSOTTARIDASA_H_

#include "GenericDasa.h"

/*************************************************//**
*
*  basic calculation for Vimsottari related Dasas
*
******************************************************/
class VimsottariBaseDasaExpert : public GenericNakshatraDasaExpert
{
public:

	VimsottariBaseDasaExpert( const DasaId &dasa, Horoscope *h ) : GenericNakshatraDasaExpert( dasa, h ) {}

	int getTara( const int &planet, const int &nak );
	int getMahaDasaLength( const int &dasa );
	void getDasaLengthInts( int &year, int &month, int &day, const int &mahadasa, const int &antardasa );

protected:
	virtual int getNbLords() const { return 9; }
	virtual int getParamayus() const { return 120; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;

private:
};

/*************************************************//**
*
*  calculation of Vimsottari Dasa
*
******************************************************/
class VimsottariDasaExpert : public VimsottariBaseDasaExpert
{
public:

	VimsottariDasaExpert( Horoscope *h ) : VimsottariBaseDasaExpert( D_VIMSOTTARI, h ) {}

protected:
};

/*************************************************//**
*
*  calculation of Lagna Vimsottari Dasa
*
******************************************************/
class LagnaVimsottariDasaExpert : public VimsottariBaseDasaExpert
{
public:

	LagnaVimsottariDasaExpert( Horoscope *h ) : VimsottariBaseDasaExpert( D_LAGNAVIMSOTTARI, h ) {}

protected:

	// owerwrites Moon based calculation
	virtual double getPortion( Nakshatra &nak );
};

#endif

