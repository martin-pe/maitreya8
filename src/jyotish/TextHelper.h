/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/TextHelper.h
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

#ifndef _TEXTHELPER_H_
#define _TEXTHELPER_H_

class AspectExpert;
class ChartProperties;
class Horoscope;
class SimpleHoroscope;
class Sheet;
class TableStyle;

#include "maitreya.h"

/*************************************************//**
*
*  writes text output for text view and console output
*
******************************************************/
class TextHelper
{
public:

	TextHelper( Horoscope*, ChartProperties*, Sheet* );

	void setShowHeader( const bool b = true ) { show_header = b; }

	int writeTextAnalysis( const int &mode, const Varga varga = V_RASI, const DasaId = D_VIMSOTTARI );

	// standard lists in main view
	int writePlanets( const OBJECT_INCLUDES = OI_NONE );
	int writeVedicPlanets( const OBJECT_INCLUDES = OI_NONE );
	int writeWesternPlanets( const OBJECT_INCLUDES = OI_NONE );

	int writeVedicPlanetReport();
	int writeWesternPlanetReport();

	int writeSynastryReport( Horoscope *h2, AspectExpert* );
	int writeTransitReport( Horoscope *h2, AspectExpert* );
	int writeTransitReport( const PlanetContext& );

	// used in text view
	void writeBaseData();

	void writeHoroscopeData();

	int writeAstronomicalData();
	void writeBhavas();
	int writeKp( const DasaId& );

private:
	Horoscope *horoscope;
	ChartProperties *chartprops;
	Sheet *sheet;
	bool show_header;
};

#endif

