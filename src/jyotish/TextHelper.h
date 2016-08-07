/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/TextHelper.h
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
	int writePlanets();
	int writeVedicPlanets();
	int writeWesternPlanets();

	int writeVedicPlanetReport();
	int writeWesternPlanetReport();

	int writeSynastryReport( Horoscope *h2, AspectExpert* );
	int writeTransitReport( Horoscope *h2, AspectExpert* );

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

