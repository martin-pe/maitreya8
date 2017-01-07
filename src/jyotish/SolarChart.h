/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/SolarChart.h
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
#ifndef _SOLARCHART_
#define _SOLARCHART_

#include <wx/string.h>

#include "Expert.h"
#include "Horoscope.h"

class ChartProperties;
class Sheet;

/*************************************************//**
*
* 
*
******************************************************/
class SolarChartExpert : public Expert
{
public:

	SolarChartExpert( Horoscope*, ChartProperties* );
	~SolarChartExpert();

	void update();
	Horoscope *getSolarChart() { return solarchart; }
	void setSolarReturnYear( const int& );
	void writeSolarReturnData( Sheet* );

private:
	ChartProperties *chartprops;
	int year;
	Horoscope *solarchart;

	void writeStandardPlanetList( Sheet* );
};

#endif

