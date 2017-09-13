/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/PrintoutTextHelper.cpp
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

#include "PrintoutTextHelper.h"

#include <wx/log.h>
#include <wx/string.h>

#include "ChartProperties.h"
#include "maitreya.h"
#include "GenericTableWriter.h"
#include "Horoscope.h"
#include "Table.h"
#include "Sheet.h"

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
**   PrintoutTextHelper   ---   writeTitle
**
******************************************************/
void  PrintoutTextHelper::writeTitle()
{
	assert( horoscope );
	sheet->addHeader( horoscope->getHName());
}

/*****************************************************
**
**   PrintoutTextHelper   ---   writePrintoutHeader
**
******************************************************/
void  PrintoutTextHelper::writePrintoutHeader( const int &mode, const bool &vedic )
{
	GenericTableWriter tw( horoscope );

	//printf( "PrintoutTextHelper::writePrintoutHeader mode %d vedic %d\n", mode, vedic );

	SheetColumnSet *sc = new SheetColumnSet;
	sc->addItem( writeBaseData( mode, vedic ));

	if ( mode == 0 )
	{
		sc->addItem( tw.createObjectListTableByConfigKeys(
			vedic ?  chartprops->getVedicObjectStyle() : chartprops->getWesternObjectStyle(),
			vedic ?  chartprops->getVedicColumnStyle() : chartprops->getWesternColumnStyle(),
			vedic, TF_LONG ));
	}
	else
	{
		if ( vedic )
		{
			if ( mode == 1 ) // normal
			{
				tw.setVedicObjects( OI_INNER | OI_DRAGONTAIL | OI_DRAGONHEAD );
				Tc tc;
				TcColumnSet colset( TAB_LC_PLANETS, true );
				colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, true ));
				colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, true ));
				colset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA, true ));
				colset.cols.push_back( TcColumn( TAB_CT_CHARA_KARAKA, true ));
				tc.colsets.push_back( colset );
				sc->addItem( tw.createTable( &tc ));
			}
			else // KP
			{
				Tc tc;
				TcColumnSet colset( TAB_LC_PLANETS, true );
				colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, true ));
				colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, true ));
				colset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA, true ));
				colset.cols.push_back( TcColumn( TAB_CT_KP_LORDS, true ));
				tc.colsets.push_back( colset );
				sc->addItem( tw.createTable( &tc ));

				Tc tch;
				TcColumnSet hcolset( TAB_LC_HOUSE_CUSPS, vedic, _( "Houses" ));
				hcolset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, true ));
				hcolset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, true ));
				hcolset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA, true ));
				hcolset.cols.push_back( TcColumn( TAB_CT_KP_LORDS, true ));
				tch.colsets.push_back( hcolset );
				sc->addItem( tw.createTable( &tch ));
			}
		}
		else
		{
			if ( mode == 1 ) // normal
			{
				Tc tc;
				TcColumnSet colset( TAB_LC_PLANETS, false );
				colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, false ));
				colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, false ));
				colset.cols.push_back( TcColumn( TAB_CT_ELEMENT, false ));
				tc.colsets.push_back( colset );
				sc->addItem( tw.createTable( &tc ));

				Tc tch;
				TcColumnSet hcolset( TAB_LC_HOUSE_CUSPS, vedic, _( "Houses" ));
				hcolset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, false ));
				hcolset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, false ));
				tch.colsets.push_back( hcolset );
				sc->addItem( tw.createTable( &tch ));
			}
			else if ( mode == 2 ) // uranian
			{
				tw.setWesternObjects( OI_INNER | OI_OUTER | OI_DRAGONHEAD | OI_ASCENDANT | OI_MERIDIAN );
				Tc tc;
				TcColumnSet colset( TAB_LC_PLANETS, false );
				colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, false ));
				colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, false ));
				colset.cols.push_back( TcColumn( TAB_CT_ANTISCIA, false ));
				tc.colsets.push_back( colset );
				sc->addItem( tw.createTable( &tc ));

				Tc tcu;
				TcColumnSet ucolset( TAB_LC_URANIAN, vedic, _( "Uranian" ));
				ucolset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, false ));
				ucolset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, false ));
				tcu.colsets.push_back( ucolset );
				sc->addItem( tw.createTable( &tcu ));
			}
			else if ( mode == 3 ) // test
			{
				tw.setWesternObjects( OI_INNER | OI_OUTER | OI_DRAGONHEAD | OI_ASCENDANT | OI_MERIDIAN );
				Tc tc;
				TcColumnSet colset( TAB_LC_PLANETS, false );
				colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, false ));
				colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, false ));
				colset.cols.push_back( TcColumn( TAB_CT_ANTISCIA, false ));
				tc.colsets.push_back( colset );
				sc->addItem( tw.createTable( &tc ));

				for( int i = 0; i < 5; i++ )
				{
					Tc tcu;
					TcColumnSet ucolset( TAB_LC_URANIAN, vedic, _( "Uranian" ));
					ucolset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, false ));
					ucolset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, false ));
					tcu.colsets.push_back( ucolset );
					sc->addItem( tw.createTable( &tcu ));
				}
			}
		}
	}
	sheet->addItem( sc );
}

/*****************************************************
**
**   PrintoutTextHelper   ---   writeBaseData
**
******************************************************/
Table *PrintoutTextHelper::writeBaseData( const int& /*mode*/, const bool &vedic )
{
	Tc tc;
	GenericTableWriter tw( horoscope );

	TcColumnSet colset( TAB_LC_NONE );
	TcColumn col( TAB_CT_CUSTOM_KEY_VALUE );

	//col.cells.push_back( TcCell( TAB_CELL_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_FULL_DATE ));
	col.cells.push_back( TcCell( TAB_CELL_LOCAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_LONGITUDE ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_LATITUDE ));
	
	if ( vedic )
	{
		col.cells.push_back( TcCell( TAB_CELL_VEDIC_AYANAMSA ));
		col.cells.push_back( TcCell( TAB_CELL_VARNA ));
		col.cells.push_back( TcCell( TAB_CELL_YONI ));
		col.cells.push_back( TcCell( TAB_CELL_NADI ));
		//col.cells.push_back( TcCell( TAB_CELL_INCFLUENCE_TIME ));
	}
	else
	{
		col.cells.push_back( TcCell( TAB_CELL_TZ_NAME ));
		col.cells.push_back( TcCell( TAB_CELL_DST ));
		col.cells.push_back( TcCell( TAB_CELL_UNIVERSAL_TIME ));
		col.cells.push_back( TcCell( TAB_CELL_SIDEREAL_TIME ));
		col.cells.push_back( TcCell( TAB_CELL_JULIAN_DATE ));
		col.cells.push_back( TcCell( TAB_CELL_WESTERN_AYANAMSA ));
		col.cells.push_back( TcCell( TAB_CELL_SUNRISE ));
		col.cells.push_back( TcCell( TAB_CELL_SUNSET ));
	}
	colset.cols.push_back( col );
	tc.colsets.push_back( colset );

	return tw.createTable( &tc );
}


