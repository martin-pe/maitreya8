/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Lang.h
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

#ifndef _LANG_H_
#define _LANG_H_

#include "maitreya.h"

class WriterConfig;

// Date formats
#define DF_INCLUDE_YEAR_SIGN   0x00000001
#define DF_INCLUDE_YEAR_BC_AD  0x00000002
#define DF_INCLUDE_TIME        0x00000010
#define DF_INCLUDE_TZOFFSET    0x00000020

/*************************************************//**
*
* \brief encapsulation of language specific names for astrological entities
*
******************************************************/
class Lang
{
public:

	Lang( WriterConfig* = (WriterConfig*)NULL );

	// get weekday name (0..6, sunday is 0)
	const wxString getWeekdayName( const int& ) const;
	const wxString getMonthName( const int &i ) const;

	const wxString getGanaName( const int& ) const;
	const wxString getNadiName( const int& ) const;
	const wxString getVarnaName( const int& ) const;
	const wxString getTaraName( const int& ) const;
	const wxString getTithiName( const int& ) const;
	const wxString getVashyaName( const int& ) const;

	const wxString getRajjuName( const int &aroha, const int &type ) const;

	const wxString getBhavaName( const int &i ) const;
	const wxString getFriendshipName( const FRIENDSHIP &index, const TEXT_FORMAT = TF_LONG ) const;
	const wxString getDignityName( const DIGNITY&, const TEXT_FORMAT = TF_LONG ) const;

	const wxString getVimsopakaDignityName( const int &index, const int &dignity ) const;

	wxString getNakshatra27Name( const Nakshatra&, const TEXT_FORMAT = TF_LONG ) const;
	wxString getNakshatra28Name( const NakshatraId_28&, const TEXT_FORMAT = TF_LONG ) const;

	const wxString getKarakaName( const int &i, const TEXT_FORMAT format = TF_LONG ) const;
	const wxString getShastiamsaName( const int &i ) const;

	const wxString getObjectName( const ObjectId&, const TEXT_FORMAT&, const bool vedic = false ) const;

	const wxString getObjectSignification( const ObjectId&, const TEXT_FORMAT&, const bool vedic = false );

	const wxString getSignName( const Rasi&, const TEXT_FORMAT format = TF_LONG ) const;

	const wxString getElementName( const int &sign ) const;
	const wxString getSignQualityName( const int &sign ) const;

	wxChar getPlanetContextSubscriptum( const PlanetContext &ctx ) const;

	// name of ayanamsa
	const wxString getAyanamsaName( const AYANAMSA& );

	void setConfig( WriterConfig *c ) { writercfg = c; }

private:
	wxString getNakshatraName( const int &nak, const bool &use28, const TEXT_FORMAT = TF_LONG ) const;
	WriterConfig *writercfg;
};

/*************************************************//**
*
*  \brief parses and formats date and time
*
******************************************************/
class DateTimeFormatter
{
	DECLARE_SINGLETON( DateTimeFormatter )
public:

	bool parseDateString( wxString value, int &day, int &month, int &year );

	bool parseDateString( wxString value, double &jd );

	wxString formatDateString( const double &jd, const double tzoffset = 0, const int format = DF_INCLUDE_YEAR_SIGN );

	wxString formatFullDateString( const double &jd, const double tzoffset = 0 )
	{
		return formatDateString( jd, tzoffset, DF_INCLUDE_YEAR_SIGN | DF_INCLUDE_TIME );
	}

	void calculateDateIntegers( const double &jd, int &day, int &month, int &year );
};

/*************************************************//**
*
*  \brief Formatting of date, time and degree values
*
******************************************************/
class Formatter
{
	DECLARE_SINGLETON( Formatter )

public:

	// calculates degrees, minutes and seconds from a given length from a (double) degree value
	void getDegMinSecInts( const double &len, int &deg, int &min, int &sec );
	void getDegMinSecInts2( const double &len, int &deg, int &min, double &sec );

	// deprecated
	const wxString getDegMinSecFormatted( double t, const int &format, const double &minvalue, const double &maxvalue );

	// neu
	const wxString formatDegreeString( double t, const int &format );

	// formats a length like DD:MM:SS
	const wxString getLenFormatted( const double &len, const int format = DEG_PRECISION_FLEXIBLE );

	// formats a latitude like DD:MM:SS  [N|S]
	const wxString getLatitudeFormatted( const double&, const int format = DEG_PRECISION_FLEXIBLE );

	// formats a longitude like DD:MM:SS  [E|W]
	const wxString getLongitudeFormatted( const double&, const int format = DEG_PRECISION_FLEXIBLE );

	// formats a time value like HH:MM:SS
	const wxString getTimeFormatted( const double &t, const int format = DEG_PRECISION_FLEXIBLE )
		{ return getDegMinSecFormatted( t, format, 0.0, 24.0 ); }

	// formats a degree value like DD:MM:SS
	const wxString getDegreesFormatted( const double &t, const int format = DEG_PRECISION_FLEXIBLE )
		{ return getDegMinSecFormatted( t, format, 0.0, 360.0 ); }

	wxString parseDegMinSecAndFormat( wxString, const int = DEG_PRECISION_FLEXIBLE );

	// parses a degree string DD:MM:SS
	// dep
	bool getDegreeIntsFromString( wxString value, int &deg, int &minute, int &second, int format = 0 );


	// neu
	bool parseDegreeString( const wxString &value, int &deg, int &minute, int &second, const int &maxvalue );
	bool parseDegreeString( const wxString &s, double &value, const int &maxvalue );

	// Calculates a length string
	const wxString getLenString( const double &len, const int format = 0, const int dir = 0 );

	bool parseDoubleString( const wxString &s, double &value, const double &minvalue, const double &maxvalue );
	wxString formatDoubleString( const double &t, const int format = DEG_PRECISION_FLEXIBLE );

	const wxString getAyanamsaNameAndValue( const AYANAMSA&, const double &value );

	wxString getIntOrEmtpy( const int &a );
};

#endif

