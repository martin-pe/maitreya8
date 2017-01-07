/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Transit.h
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
#ifndef _TRANSIT_H_
#define _TRANSIT_H_

#include <wx/string.h>

#include "Expert.h"
#include "Horoscope.h"

class ChartProperties;
class Sheet;

/*************************************************//**
*
* \brief Horoscope subclass providing transit calculations
*
******************************************************/
class TransitHoroscope : public Horoscope
{
public:

	TransitHoroscope();
	double calcTransitPositions( const Horoscope *hbase, const double &jd, const bool &vedic,
		const double &yl, const PlanetContext &mode );

	double getDirectionJD() const { return directionJD; }

protected:

	double directionJD;
	DECLARE_CLASS( TransitHoroscope )
};


/*************************************************//**
*
* \brief encapsulation for calculation of transits, directions, constant + Solar + Lunar arc
*
******************************************************/
class TransitExpert : public Expert
{
public:

	TransitExpert( Horoscope *h, const ChartProperties *chartprops );
	~TransitExpert();

	void init();
	void update();

	TransitHoroscope *getTransitHoroscope() { return htransit; }
	void setTransitDate( const double &jd, const double tzoffset = 0.0 );
	double getTransitDate() { return transitJD; }
	double getTzOffset() { return tzoffset; }
	double getPosDelta() const { return posdelta; }

	PlanetContext getTransitMode() const { return transitmode; }
	void setTransitMode( const PlanetContext &tmode ) { transitmode = tmode; }

	void setYearLength( const double &yl ) { yearlength = yl; }
	void writeTransitHeader( Sheet* );

private:
	const ChartProperties *chartprops;
	PlanetContext transitmode;

	double yearlength, transitJD, posdelta;
	double tzoffset;

	TransitHoroscope *htransit;
};

#endif

