/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Partner.h
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

#ifndef _PARTNER_H_
#define _PARTNER_H_

#include <wx/string.h>
#include "Expert.h"
#include "Horoscope.h"
#include "Nakshatra.h"

class ChartProperties;
class Sheet;

/*************************************************//**
*
* 
*
******************************************************/
class AshtakootaExpert : public Expert
{
public:

	AshtakootaExpert();
	AshtakootaExpert( Horoscope*, Horoscope* );

	void setHoroscope1( Horoscope *h ) { horoscope = h; }
	void setHoroscope2( Horoscope *h ) { h2 = h; }

	int calcYoniValue( const Yoni&, const Yoni& );
	int calcRajjuValue( const Rajju&, const Rajju& );

	void update();
	void write( Sheet* );
	void writeComprehensive( Sheet* );

	double getPoints() { return ptotal; }

private:
	void init();

	Horoscope *h2;
	int pvarna, pyoni, pmaitri, prasi, pgana, pnadi, prajju;
	double pvashya, ptara, ptotal;
	Rasi rasi1, rasi2;
	Nakshatra nak1, nak2;
	FRIENDSHIP mitra1, mitra2;
	Yoni yoni1, yoni2;
	int varna1, varna2;
	int vashya1, vashya2;
	int gana1, gana2;
	int tara1, tara2;
	int nadi1, nadi2;
	Rajju rajju1, rajju2;
};

/*************************************************//**
*
* \brief composite view on 2 charts (midpoints of 2 charts)
*
******************************************************/
class CompositHoroscope : public Horoscope
{
public:
	CompositHoroscope();
	void update( Horoscope*, Horoscope* );
	void dump( Sheet*, const ChartProperties*, const bool show_header = false );

private:
	DECLARE_CLASS( CompositHoroscope )
};

#endif

