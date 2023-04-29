/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/SolarChart.cpp
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

#include "SolarChart.h"

#include "astrobase.h"
#include "maitreya.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Sheet.h"
#include "Table.h"
#include "TextHelper.h"
#include "VimsottariDasa.h"

extern Config *config;

/*****************************************************
**
**   SolarChartExpert   ---   Constructor
**
******************************************************/
SolarChartExpert::SolarChartExpert( Horoscope *h, ChartProperties *chartprops )
		: Expert( h ),
		chartprops( chartprops )
{
	solarchart = new Horoscope();
	solarchart->setHName( _( "SolarChart" ));
	solarchart->setLocation( *horoscope->getLocation() );

	year = getCurrentYear();
}

/*****************************************************
**
**   SolarChartExpert   ---   Destructor
**
******************************************************/
SolarChartExpert::~SolarChartExpert()
{
	delete solarchart;
}

/*****************************************************
**
**   SolarChartExpert   ---   update
**
******************************************************/
void SolarChartExpert::update()
{
	Calculator *calculator = CalculatorFactory().getCalculator();
	double jd = calculator->calcTajakaJd( horoscope->getDataSet(), year, chartprops->isVedic());
	solarchart->setDate( jd );
	solarchart->update();
}

/*****************************************************
**
**   SolarChartExpert   ---   setSolarReturnYear
**
******************************************************/
void SolarChartExpert::setSolarReturnYear( const int &year )
{
	this->year = year;
}

/*****************************************************
**
**   SolarChartExpert   ---   writeSolarReturnData
**
******************************************************/
void SolarChartExpert::writeSolarReturnData( Sheet *sheet )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	Lang lang;

	writeStandardPlanetList( sheet );

	if ( chartprops->isVedic())
	{
		const int numyears = year - getYearForJD( horoscope->getJD());
		const int muntha =  a_red( numyears + getRasi( horoscope->getVedicLongitude( OASCENDANT )),  12 );
		wxString s = wxT( "Muntha: " ) + lang.getSignName( (Rasi)muntha, TF_LONG );
		sheet->addLine( s );

		VimsottariBaseDasaExpert *expert = new VimsottariDasaExpert( horoscope );
		//Location *loc = horoscope->getLocation();
		std::vector<Dasa*> v;
		
		v = expert->getSolarDasa( solarchart, year );
		Table *table = new Table( 2, 10 );
		table->setHeader( _( "Vimsottari" ));
		table->setHeader( 0, _( "Lord" ));
		table->setHeader( 1, _( "Until" ));

		for( uint i = 0; i < v.size(); i++ )
		{
			table->setEntry( 0, i + 1, lang.getObjectName( (ObjectId)v[i]->getDasaLord(), TF_MEDIUM, true ));
			table->setEntry( 1, i + 1, formatter->formatDateString( v[i]->getEndJD() ));
		}
		sheet->addItem( table );

		for( uint i = 0; i < v.size(); i++ ) delete v[i];
		v.clear();
		delete expert;
	}
}

/*****************************************************
**
**   SolarChartExpert   ---   writeStandardPlanetList
**
******************************************************/
void SolarChartExpert::writeStandardPlanetList( Sheet *sheet )
{
	if ( config->view->showTextViewHeaders ) sheet->addHeader( _( "Solar Chart" ));
	TextHelper helper( solarchart, chartprops, sheet );
	helper.writePlanets();
	//if ( ! chartprops->isVedic()) helper.writeTextAnalysis( TM_WESTERN_ASPECTARIUM_PLAIN );

}

