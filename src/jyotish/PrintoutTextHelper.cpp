/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/PrintoutTextHelper.cpp
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

#include "PrintoutTextHelper.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

#include "astrobase.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Dasa.h"
#include "DasaTool.h"
#include "maitreya.h"
#include "GenericTableWriter.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "PlanetList.h"
#include "Table.h"
#include "Sheet.h"
#include "SheetConfig.h"
#include "Varga.h"

extern Config *config;

/*****************************************************
**
**   PrintoutTextHelper   ---   Constructor
**
******************************************************/
PrintoutTextHelper::PrintoutTextHelper( Horoscope *h, const ChartProperties *props, Sheet *sheet )
		: horoscope( h ),
		sheet( sheet )
{
	chartprops = new ChartProperties( *props );
	show_header = true;
}

/*****************************************************
**
**   PrintoutTextHelper   ---   Destructor
**
******************************************************/
PrintoutTextHelper::~PrintoutTextHelper()
{
	delete chartprops;
}

/*****************************************************
**
**   PrintoutTextHelper   ---   writeTextAnalysis
**
******************************************************/
void  PrintoutTextHelper::writePrintoutHeader( const int& /*mode*/, const bool &vedic )
{
	Tc tc;
	GenericTableWriter tw( horoscope );
	tw.setVedicObjects( OI_INNER | OI_DRAGONTAIL | OI_DRAGONHEAD );
	tw.setTextFormat( TF_SHORT );

	writeBaseData( tc );

	printf( "VEDIC %d\n", chartprops->isVedic());
	//sleep( 19 );

	TcColumnSet colset( TAB_LC_PLANETS, vedic );
	colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, vedic ));
	colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, vedic ));
	colset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA, vedic ));
	colset.cols.push_back( TcColumn( TAB_CT_KP_LORDS, vedic ));
	colset.cols.push_back( TcColumn( TAB_CT_EMPTY ));
	tc.colsets.push_back( colset );

	TcColumnSet hcolset( TAB_LC_HOUSE_CUSPS, vedic );
	hcolset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, vedic ));
	hcolset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, vedic ));
	hcolset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA, vedic ));
	hcolset.cols.push_back( TcColumn( TAB_CT_KP_LORDS, vedic ));
	tc.colsets.push_back( hcolset );

/*
	// TODO if subtype 

	//OBJECT_INCLUDES obs = chartprops->getObjectStyle();
	TAB_COLUMN_TYPE ct = TAB_CT_NAME_LONGITUDE | TAB_CT_NAKSHATRA | TAB_CT_KP_LORDS;
	//TAB_COLUMN_TYPE ct = TAB_CT_NAME_LONGITUDE;

	// nein, keine Kopie con Chartprops, sondern separat
	chartprops->setVedicColumnStyle( ct );

	//OBJECT_INCLUDES obs = chartprops->getVedicObjectStyle();
	OBJECT_INCLUDES obs = OI_INNER | OI_DRAGONTAIL | OI_DRAGONHEAD | OI_ASCENDANT;
	tw.appendObjectListTcByConfigKeys( tc, obs, chartprops->getColumnStyle(), chartprops->isVedic(), TF_LONG );

	obs = OI_ALL_HOUSES;
	tw.appendObjectListTcByConfigKeys( tc, obs, chartprops->getColumnStyle(), chartprops->isVedic(), TF_LONG );

*/
	Table *table = tw.createTable( &tc );
	sheet->addItem( table );
}

/*****************************************************
**
**   PrintoutTextHelper   ---   writeBaseData
**
******************************************************/
void PrintoutTextHelper::writeBaseData( Tc &tc )
{
	TcColumnSet colset( TAB_LC_NONE );
	TcColumn col( TAB_CT_CUSTOM_KEY_VALUE );
	//col.title = _( "Base Data" );

	col.cells.push_back( TcCell( TAB_CELL_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_FULL_DATE ));
	col.cells.push_back( TcCell( TAB_CELL_LOCAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_LONGITUDE ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_LATITUDE ));

	col.cells.push_back( TcCell( TAB_CELL_VARNA ));
	col.cells.push_back( TcCell( TAB_CELL_YONI ));
	col.cells.push_back( TcCell( TAB_CELL_NADI ));
	col.cells.push_back( TcCell( TAB_CELL_INCFLUENCE_TIME ));

	/*
	col.cells.push_back( TcCell( TAB_CELL_TZ_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_DST ));
	col.cells.push_back( TcCell( TAB_CELL_UNIVERSAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_SIDEREAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_JULIAN_DATE ));
	col.cells.push_back( TcCell( TAB_CELL_VEDIC_AYANAMSA ));
	col.cells.push_back( TcCell( TAB_CELL_WESTERN_AYANAMSA ));
	col.cells.push_back( TcCell( TAB_CELL_SUNRISE ));
	col.cells.push_back( TcCell( TAB_CELL_SUNSET ));
	*/

	colset.cols.push_back( col );
	colset.cols.push_back( TcColumn( TAB_CT_EMPTY ));
	tc.colsets.push_back( colset );
}


