/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/SolarChart.cpp
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
		vector<Dasa*> v;
		
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

