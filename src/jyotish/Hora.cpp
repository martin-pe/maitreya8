/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Hora.cpp
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

#include "Hora.h"

#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "mathbase.h"
#include "Sheet.h"
#include "Table.h"

#include <wx/string.h>

extern Config *config;

/*****************************************************
**
**   HoraExpert   ---   Constructor
**
******************************************************/
HoraExpert::HoraExpert()
{
	location = config->defaultLocation;
}

/*****************************************************
**
**   HoraExpert   ---   setLocation
**
******************************************************/
void HoraExpert::setLocation( Location *loc )
{
	location = loc;
}

//#define HORA_DEB
/*****************************************************
**
**   HoraExpert   ---   update
**
******************************************************/
void HoraExpert::update( const double &jd )
{
	const ObjectId lordseq[7] = { OSATURN, OJUPITER, OMARS, OSUN, OVENUS, OMERCURY, OMOON };
	const ObjectId rev_lordseq[7] = { OVENUS, OSATURN, OMERCURY, OJUPITER, OMOON, OMARS, OSUN };

	Calculator *calculator = CalculatorFactory().getCalculator();
	DateTimeFormatter *formatter = DateTimeFormatter::get();

	int i, weekday, day, month, year;

	double sunrise = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNRISE, jd, location->getLatitude(), location->getLongitude());
	double sunset = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNSET, sunrise, location->getLatitude(), location->getLongitude());
	double sunrise_next = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNRISE, sunset, location->getLatitude(), location->getLongitude());

	weekday = getWeekDay( sunrise );
	dinaLord = lordseq[ ( rev_lordseq[weekday] ) % 7 ];



#ifdef HORA_DEB
	wxString s;
	printf( "##################  DEBUG ######################\n" );
	location->dump( s );
	s = formatter->getFullDateStringFromJD( jd );
	printf( "full date: %s\n", str2char( s ));
	printf( "jd: %s\n", str2char( formatter->getFullDateStringFromJD( jd )));
	printf( "sunrise: %s\n", str2char( formatter->getFullDateStringFromJD( sunrise )));
	printf( "sunset: %s\n", str2char( formatter->getFullDateStringFromJD( sunset )));
	printf( "sunrise NEXT: %s\n", str2char( formatter->getFullDateStringFromJD( sunrise_next )));
	printf( "WEEKDAY %d\n", weekday );
	printf( "dinalord %d\n", dinaLord );
	printf( "##################  END DEBUG  ######################\n" );
#endif

	for ( i = 0; i < 24; i++ )
	{
		horaLord[i] = lordseq[ ( rev_lordseq[weekday] + i ) % 7 ];
	}

	double daydur = sunset - sunrise;
	double daystep = daydur / 12;

	double nightdur = sunrise_next - sunset;
	double nightstep = nightdur / 12;

	currentHoraLord = horaLord[0];
	horaStart[24] = sunrise_next;

	for ( i = 0; i < 12; i++ )
	{
		horaStart[i] = sunrise + i * daystep;
		if ( horaStart[i] < jd ) currentHoraLord = horaLord[i];
	}
	for ( i = 0; i < 12; i++ )
	{
		horaStart[i+12] = sunset + i * nightstep;
		if ( horaStart[i+12] < jd ) currentHoraLord = horaLord[i+12];
	}

	// calculate lords of month and year
	formatter->calculateDateIntegers( sunrise, day, month, year );

	// lord of 1st day of year
	weekday = getWeekDay( calculator->calc_jd( year, 1, 1, 12 ));
	varshaLord = lordseq[ ( rev_lordseq[weekday] ) % 7 ];

	// lord of 1st day ov month
	weekday = getWeekDay( calculator->calc_jd( year, month, 1, 12 ));
	masaLord = lordseq[ ( rev_lordseq[weekday] ) % 7 ];
}

/*****************************************************
**
**   HoraExpert   ---   getHoraLordByIndex
**
******************************************************/
int HoraExpert::getHoraLordByIndex( const int &i ) const
{
	assert( i >= 0 && i < 24 );
	return horaLord[i];
}

/*****************************************************
**
**   HoraExpert   ---   getHoraStart
**
******************************************************/
double HoraExpert::getHoraStartByIndex( const int &i ) const
{
	assert( i >= 0 && i < 25 );
	return horaStart[i];
}

/*****************************************************
**
**   HoraExpert   ---   write
**
******************************************************/
void HoraExpert::write( Sheet *sheet, const bool isLocaltime )
{
	TzUtil tzu;
	wxString s;
	TzFormattedDate fd;
	Lang lang;
	SheetFormatter fmt;

	double corr = ( location->getTimeZone() + location->getDST()) / 24;

	Table *table = new Table( 7, 13 );
	if ( config->view->showTextViewHeaders ) table->setHeader( _( "Hora" ));

	table->setHeader( 0,  _( "Begin" ));
	table->setHeader( 1,  _( "Lord" ));
	table->setHeader( 2,  _( "End" ));
	//table->setHeader( 3,  wxEmptyString );
	table->setHeader( 4,  _( "Begin" ));
	table->setHeader( 5,  _( "Lord" ));
	table->setHeader( 6,  _( "End" ));
	int line = 1;
	int colskip = 0;
	for ( int i = 0; i < 24; i++ )
	{
		if ( i == 12 )
		{
			colskip = 4;
			line = 1;
		}

		table->setEntry( 3, line, wxEmptyString );
		fd = tzu.getDateFormatted( horaStart[i] + corr, isLocaltime );
		table->setEntry( colskip, line, fd.timeFormatted );

		table->setEntry( 1+colskip, line, fmt.getObjectName( horaLord[i], TF_LONG ));

		fd = tzu.getDateFormatted( horaStart[i+1] + corr, isLocaltime );
		table->setEntry( 2+colskip, line, fd.timeFormatted );
		line++;
	}
	sheet->addItem( table );

	sheet->addLine( wxString::Format( wxT( "%s: %s" ), _( "Lord of the day" ),
		lang.getObjectName( getDinaLord(), TF_LONG ).c_str()));
	sheet->addLine( wxString::Format( wxT( "%s: %s" ), _( "Lord of the month" ),
		lang.getObjectName( getMasaLord(), TF_LONG ).c_str()));
	sheet->addLine( wxString::Format( wxT( "%s: %s" ), _( "Lord of the year" ),
		lang.getObjectName( getVarshaLord(), TF_LONG ).c_str() ));
}

