/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/mathbase.h
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
#ifndef _MATHBASE_H_
#define _MATHBASE_H_

#include "maitreya.h"
#include <wx/string.h>

/**************************************************************
***
**  Functions for circular periods
***
***************************************************************/
double a_red( const double &x, const double &a ); // mod a
double red_deg( const double &input );            // mod 360
double red_rad( const double &input );            // mod 2*PI

int red12( const int &input );                    // mod 12 (integer)
int red27( const int &input );                    // mod 27 (integer)
#define redRasi red12
#define redNakshatra red27

/**************************************************************
***
**  Variuos mathematical funtions
***
***************************************************************/
const wxString printfDouble( const double &d );

const wxString getHouseNumberFormatted( const int &house, const int style = 0 );

// rounds a double to int
int a_rund( const double &v );

// workaround for problems around the decimal point in different languages ( "." or ",")
double myatof( const wxString& );

// Calculate the time for a given JD (0..24)
double getTimeFromJD( const double &jd );

int getYearForJD( const double& );
int getCurrentYear();
int getMonthLength( const int &year, const int &month );
int getNumberDaysInYear( const int &year );

//   weekday index for a given jd
int getWeekDay( const double &jd );

// Pen/Brush conversion
int convertInt2WxPenStyle( const int& );
int convertWxPenStyle2Int( const int& );
wxString convertWxPenStyle2String( const int& );
int convertString2WxPenStyle( const wxString );

int convertInt2MBrushStyle( const int& );
int convertMBrushStyle2Int( const int& );
int convertString2MBrushStyle( const wxString );
wxString convertMBrushStyle2String( const int& );


#endif


