/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VimsottariDasa.h
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

