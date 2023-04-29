/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Eclipse.cpp
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

// Workaround: compilation fails on osx 10.9
#include <wx/string.h>

#include "Eclipse.h"

#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "maitreya.h"
#include "Table.h"

extern Config *config;

/*************************************************//**
*
* \brief sorts Eclipse events according to their date
*
******************************************************/
class EclipseDataSorter
{
public:
	bool operator()( EclipseData *e1, EclipseData *e2 ) const
	{
		return ( e1->jd < e2->jd );
	}
};

/*****************************************************
**
**   EclipseExpert   ---   Constructor
**
******************************************************/
EclipseExpert::EclipseExpert()
{
}

/*****************************************************
**
**   EclipseExpert   ---   Destructor
**
******************************************************/
EclipseExpert::~EclipseExpert()
{
	std::list<EclipseData*>::iterator iter;
	for ( iter = data.begin(); iter != data.end(); iter++ )
	{
		delete (*iter);
	}
}

/*****************************************************
**
**   EclipseExpert   ---   update
**
******************************************************/
void EclipseExpert::update( const double &startjd, const double &endjd, const int &planet, const int& /* type */, const bool &vedic )
{
	Calculator *calculator = CalculatorFactory().getCalculator();
	EclipseData d;
	int count = 0;

	if ( startjd >= endjd ) return;

	data.clear();

	if ( planet == 0 || planet == 1 )
	{
		d.jd = startjd;
		while ( d.jd < endjd && count < 100 )
		{
			d = calculator->calcNextEclipse( d.jd, OSUN, 0, vedic );
			d.planet = OSUN;
			if ( d.jd <= endjd ) data.push_back( new EclipseData( d ));
			count++;
		}
	}
	if ( planet == 0 || planet == 2 )
	{
		count = 0;
		d.jd = startjd;
		while ( d.jd < endjd && count < 100 )
		{
			d = calculator->calcNextEclipse( d.jd, OMOON, 0, vedic );
			d.planet = OMOON;
			if ( d.jd <= endjd ) data.push_back( new EclipseData( d ));
			count++;
		}
	}
	data.sort( EclipseDataSorter() );
}

/*****************************************************
**
**   EclipseExpert   ---   write
**
******************************************************/
void EclipseExpert::write( Sheet *sheet, const bool &useLocaltime  )
{
	SheetFormatter fmt;

	//if ( config->view->showTextViewHeaders ) sheet->addHeader( _( "Eclipses" ));
	//sheet->addHeader( _( "Eclipses" ));

	EclipseData* e;
	std::list<EclipseData*>::iterator iter;
	wxString type, thetime;
	TzUtil tzu;

	Table *table = new Table( 5, data.size()+1 );
	table->setHeader(  _( "Eclipses" ));

	table->setHeader( 0, _( "Type" ));
	table->setHeader( 1, _( "Date" ));
	table->setHeader( 2, _( "Time" ));
	table->setHeader( 3, _( "Sun" ));
	table->setHeader( 4, _( "Moon" ));

	int i = 1;
	for ( iter = data.begin(); iter != data.end(); iter++ )
	{
		e = (*iter);
		if ( e->planet == OSUN ) type = _( "Solar"  );
		else type = _( "Lunar" );
		type << wxT( " " ) << e->description;
		table->setEntry( 0, i, type );

		TzFormattedDate fd = tzu.getDateFormatted( e->jd, useLocaltime );
		table->setEntry( 1, i, fd.weekDay + wxT( " " ) + fd.dateFormatted );

		thetime = fd.timeFormatted;
		//if ( useLocaltime ) thetime += wxT( " " ) + fd.timezoneFormatted;
		table->setEntry( 2, i, thetime );

		table->setEntry( 3, i, fmt.getPosFormatted( e->sunpos ));
		table->setEntry( 4, i, fmt.getPosFormatted( e->moonpos ));
		i++;
	}
	sheet->addItem( table );
}


