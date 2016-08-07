/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/mathbase.h
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


