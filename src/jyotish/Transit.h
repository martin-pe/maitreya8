/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Transit.h
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
		const double &yl, const TRANSIT_MODE &mode );

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

	Horoscope *getTransitHoroscope() { return htransit; }
	void setTransitDate( const double &jd, const double tzoffset = 0.0 );
	double getTransitDate() { return transitJD; }
	double getTzOffset() { return tzoffset; }

	TRANSIT_MODE getTransitMode() const { return transitmode; }
	void setTransitMode( const TRANSIT_MODE &tmode ) { transitmode = tmode; }

	void setYearLength( const double &yl ) { yearlength = yl; }
	void writeTransitHeader( Sheet* );

private:
	const ChartProperties *chartprops;
	TRANSIT_MODE transitmode;

	double yearlength, transitJD, posdelta;
	double tzoffset;

	TransitHoroscope *htransit;
};

#endif

