/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/mathbase.cpp
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

#include "mathbase.h"
#include "Calculator.h"
#include "DataSet.h"

#include <locale.h>
#include <wx/log.h>

/**************************************************************
***
**  prints a double into a string and cuts trailing zeros (leaving at least one)
***
***************************************************************/
const wxString printfDouble( const double &d )
{
	wxString s;
	s.Printf( wxT( "%.8f" ), d );
	int i = s.Len();
	wxChar dp = (localeconv()->decimal_point)[0];
	int c = s.Find( dp );
	if ( c > -1 )
	{
		while ( i > c+2 )
		{
			if ( s.GetChar( i-1 ) == '0' ) {
				s = s.Left( i-1 );
			}
			else break;
			i--;
		}
	}
	return s;
}

/**************************************************************
***
**  prints a house number in Roman format
***
***************************************************************/
const wxString getHouseNumberFormatted( const int &house, const int style )
{
	ASSERT_VALID_HOUSE( house );
	assert( style > 0 && style <= 2 );

	const static wxChar* rnumber[12] = {
		wxT( "I" ),
		wxT( "II" ),
		wxT( "III" ),
		wxT( "IV" ),
		wxT( "V" ),
		wxT( "VI" ),
		wxT( "VII" ),
		wxT( "VIII" ),
		wxT( "IX" ),
		wxT( "X" ),
		wxT( "XI" ),
		wxT( "XII" )
	};
	if ( style == 1 )
	{
		return wxString::Format( wxT( "%d" ), house + 1 );
	}
	else if ( style == 2 )
	{
		return rnumber[house];
	}
	else return wxEmptyString;
}
	
/**************************************************************
***
**  Reduces a given double value x modulo the double a (should be positive).
**  The return value is between 0 and a.
***
***************************************************************/
double a_red( const double &x, const double &a )
{
	//assert( a > 0 );
	return( x - floor( x / a ) * a );
}

/**************************************************************
***
**   Reduces a given double value modulo 360.
**   The return value is between 0 and 360.
***
***************************************************************/
double red_deg( const double &input )
{
	return a_red( input, 360 );
}

/**************************************************************
***
**  Reduces a given integer modulo 12.
**  The return value is between 0 and 11.
***
***************************************************************/
int red12( const int &input )
{
	int ret = input %12;
	return ret >= 0 ? ret : ret + 12;
}

/**************************************************************
***
**  Reduces a given integer modulo 27.
**  The return value is between 0 and 26.
***
***************************************************************/
int red27( const int &input )
{
	int ret = input % 27;
	return ret >= 0 ? ret : ret + 27;
}

/**************************************************************
***
**     round a double
***
***************************************************************/
int a_rund( const double &v )
{
	return v >= 0 ? (int)(v+.5) : (int)(v-.5);
}

/**************************************************************
***
**    Conversion of strings to double.
**    Supports locale dependend decimal points
***
***************************************************************/
double myatof( const wxString &s )
{
	if ( s.IsEmpty() ) return 0.0;
	//assert( s );
	wxString err;
	wxChar tmp[256];
	wxStrcpy( tmp, s );
	wxChar *c = tmp;
	wxChar dp = (localeconv()->decimal_point)[0];
	//printf( "mathbase::myatof decimal point of locale is '%c'\n", dp );

	while ( *c )
	{
		if ( *c == '.' && *c != dp ) *c = ',';
		if ( *c == ',' && *c != dp ) *c = '.';
		c++;
	}
	wxString s1 = tmp;
	double a = 0;
	if ( ! s1.ToDouble( &a ) && s != wxEmptyString )
	{
		wxLogError(
		    wxT( "Can't convert string to double, string was \"%s\", local decimal point \"%c\", converted string \"%s\", double is %f\n" ),
		    s.c_str(), dp, c, a );
		return 0.0;
	}
	return a;
}

/**************************************************************
***
 * Returns the Greenwich mean time for a given julian date.
 * the return value is between 0 and 24. There's no formating like [hhmmss], etc.
***
***************************************************************/
double getTimeFromJD( const double &jd )
{
	double jjd = jd - .5;
	return a_red( ( jjd - (double)(int)jjd ) * 24, 24 );
}

/*****************************************************
**
**   year for a given jd
**
******************************************************/
int getYearForJD( const double &jd )
{
	int day, month, year;
  double ut = 0;

	CalculatorFactory().getCalculator()->getDateIntsFromJD( jd, year, month, day, ut );
	return year;
}

/*****************************************************
**
**   current year
**
******************************************************/
int getCurrentYear()
{
	return getYearForJD( MDate().getJD());
}

/*****************************************************
**
**   number of days in a given month
**
******************************************************/
int getMonthLength( const int &year, const int &month )
{
	switch ( month )
	{
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	break;
	case 2: // Feb
		if ( year % 4 ) return 28;
		else
		{
			int nb_days = 29;
			if ( year % 100 == 0 ) nb_days = 28; // 1900, 1800 etc. have 28 days
			if ( year % 400 == 0 ) nb_days = 29; // BUT Feb 2000 has 29 days
			return nb_days;
		}
	break;
	default:
		return 31;
	break;
	}
}

/*****************************************************
**
**   number of days in a given year
**
******************************************************/
int getNumberDaysInYear( const int &year )
{
	if ( year % 4 ) return 365;
	if ( year % 400 == 0 ) return 366; // BUT Feb 2000 has 29 days
	if ( year % 100 == 0 ) return 365; // 1900, 1800 etc. have 28 days
	return 366;
}

/*****************************************************
**
**   weekday index for a given jd
**
******************************************************/
int getWeekDay( const double &jd )
{
	return (int)(jd + 1.5 ) % 7;
}

/*****************************************************
**
**   convertInt2WxPenStyle
**
******************************************************/
int convertInt2WxPenStyle( const int &istyle )
{
	int style;
	switch ( istyle )
	{
	case 1:
		style = wxDOT;
		break;
	case 2:
		style = wxLONG_DASH;
		break;
	case 3:
		style = wxSHORT_DASH;
		break;
	case 4:
		style = wxDOT_DASH;
		break;
	case 5:
		style = wxTRANSPARENT;
		break;
	default:
		style = wxSOLID;
		break;
	}
	return style;
}

/*****************************************************
**
**   convertString2WxPenStyle
**
******************************************************/
int convertString2WxPenStyle( const wxString s )
{
	if ( ! s.CmpNoCase( wxT( "wxSOLID" ))) return wxSOLID;
	else if ( ! s.CmpNoCase( wxT( "wxDOT" ))) return wxDOT;
	else if ( ! s.CmpNoCase( wxT( "wxLONG_DASH" ))) return wxLONG_DASH;
	else if ( ! s.CmpNoCase( wxT( "wxSHORT_DASH" ))) return wxSHORT_DASH;
	else if ( ! s.CmpNoCase( wxT( "wxDOT_DASH" ))) return wxDOT_DASH;
	else if ( ! s.CmpNoCase( wxT( "wxTRANSPARENT" ))) return wxTRANSPARENT;

	wxString errstr = wxT( "invalid pen style " );
	errstr << s;
	wxLogError( s );
	return wxSOLID;
}

/*****************************************************
**
**   convertWxPenStyle2String
**
******************************************************/
wxString convertWxPenStyle2String( const int &style )
{
	wxString s;
	switch ( style )
	{
	case wxSOLID:
		s = wxT( "wxSOLID" );
		break;
	case wxDOT:
		s = wxT( "wxDOT" );
		break;
	case wxLONG_DASH:
		s = wxT( "wxLONG_DASH" );
		break;
	case wxSHORT_DASH:
		s = wxT( "wxSHORT_DASH" );
		break;
	case wxDOT_DASH:
		s = wxT( "wxDOT_DASH" );
		break;
	case wxTRANSPARENT:
		s = wxT( "wxTRANSPARENT" );
		break;
	default:
	{
		wxString errstr = wxT( "invalid pen style " );
		errstr << style;
		wxLogError( s );
		s = wxT( "wxSOLID" );
	}
	break;
	}
	return s;
}

/*****************************************************
**
**   convertWxPenStyle2Int
**
******************************************************/
int convertWxPenStyle2Int( const int &style )
{
	int istyle;
	switch ( style )
	{
	case wxDOT:
		istyle = 1;
		break;
	case wxLONG_DASH:
		istyle = 2;
		break;
	case wxSHORT_DASH:
		istyle = 3;
		break;
	case wxDOT_DASH:
		istyle = 4;
		break;
	case wxTRANSPARENT:
		istyle = 5;
		break;
	default:
		istyle = 0;
		break;
	}
	return istyle;
}

/*****************************************************
**
**   convertInt2MBrushStyle
**
******************************************************/
int convertInt2MBrushStyle( const int &istyle )
{
	int style;
	switch ( istyle )
	{
	case 1 :
		style = wxTRANSPARENT;
		break;
	case 2 :
		style = wxBDIAGONAL_HATCH;
		break;
	case 3 :
		style = wxCROSSDIAG_HATCH;
		break;
	case 4 :
		style = wxFDIAGONAL_HATCH;
		break;
	case 5 :
		style = wxCROSS_HATCH;
		break;
	case 6 :
		style = wxHORIZONTAL_HATCH;
		break;
	case 7 :
		style = wxVERTICAL_HATCH;
		break;
	case 8 :
		style = wxSTIPPLE;
		break;
	default:
		style = wxSOLID;
		break;
	}
	printf( "convertInt2MBrushStyle in %d out %d\n", istyle, style );
	return style;
}

/*****************************************************
**
**   convertString2MBrushStyle
**
******************************************************/
int convertString2MBrushStyle( const wxString s )
{
	if ( ! s.CmpNoCase( wxT( "wxSOLID" ))) return wxSOLID;
	else if ( ! s.CmpNoCase( wxT( "wxTRANSPARENT" ))) return wxTRANSPARENT;
	else if ( ! s.CmpNoCase( wxT( "wxBDIAGONAL_HATCH" ))) return wxBDIAGONAL_HATCH;
	else if ( ! s.CmpNoCase( wxT( "wxCROSSDIAG_HATCH" ))) return wxCROSSDIAG_HATCH;
	else if ( ! s.CmpNoCase( wxT( "wxFDIAGONAL_HATCH" ))) return wxFDIAGONAL_HATCH;
	else if ( ! s.CmpNoCase( wxT( "wxCROSS_HATCH" ))) return wxCROSS_HATCH;
	else if ( ! s.CmpNoCase( wxT( "wxHORIZONTAL_HATCH" ))) return wxHORIZONTAL_HATCH;
	else if ( ! s.CmpNoCase( wxT( "wxVERTICAL_HATCH" ))) return wxVERTICAL_HATCH;
	else if ( ! s.CmpNoCase( wxT( "wxSTIPPLE" ))) return wxSTIPPLE;

	wxLogError( wxT( "convertString2MBrushStyle: invalid brush style '%s'" ), s.c_str() );
	return wxSOLID;
}

/*****************************************************
**
**   convertMBrushStyle2Int
**
******************************************************/
int convertMBrushStyle2Int( const int &style )
{
	int istyle;
	switch ( style )
	{
	case wxTRANSPARENT:
		istyle = 1;
		break;
	case wxBDIAGONAL_HATCH:
		istyle = 2;
		break;
	case wxCROSSDIAG_HATCH:
		istyle = 3;
		break;
	case wxFDIAGONAL_HATCH:
		istyle = 4;
		break;
	case wxCROSS_HATCH:
		istyle = 5;
		break;
	case wxHORIZONTAL_HATCH:
		istyle = 6;
		break;
	case wxVERTICAL_HATCH:
		istyle = 7;
		break;
	case wxSTIPPLE:
		istyle = 8;
		break;
	default:
		istyle = 0;
		break;
	}
	return istyle;
}

/*****************************************************
**
**   convertMBrushStyle2String
**
******************************************************/
wxString convertMBrushStyle2String( const int &style )
{
	switch ( style )
	{
	case wxSOLID:
		return wxT( "wxSOLID" );
		break;
	case wxTRANSPARENT:
		return wxT( "wxTRANSPARENT" );
		break;
	case wxBDIAGONAL_HATCH:
		return wxT( "wxBDIAGONAL_HATCH" );
		break;
	case wxCROSSDIAG_HATCH:
		return wxT( "wxCROSSDIAG_HATCH" );
		break;
	case wxFDIAGONAL_HATCH:
		return wxT( "wxFDIAGONAL_HATCH" );
		break;
	case wxCROSS_HATCH:
		return wxT( "wxCROSS_HATCH" );
		break;
	case wxHORIZONTAL_HATCH:
		return wxT( "wxHORIZONTAL_HATCH" );
		break;
	case wxVERTICAL_HATCH:
		return wxT( "wxVERTICAL_HATCH" );
		break;
	case wxSTIPPLE:
		return wxT( "wxSTIPPLE" );
		break;
	default:
		wxLogError( wxT( "convertMBrushStyle2String: invalid brush style %d" ), style );
		return wxT( "wxSOLID" );
	break;
	}
}
