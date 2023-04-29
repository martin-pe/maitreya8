/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/GenericTableWriter.cpp
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

#include "GenericTableWriter.h"

#include "Ashtakavarga.h"
#include "Aspect.h"
#include "astrobase.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Dasa.h"
#include "Hora.h"
#include "Horoscope.h"
#include "Jaimini.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "PlanetList.h"
#include "Sheet.h"
#include "SheetConfig.h"
#include "Table.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicPlanet.h"

#include <wx/log.h>

extern Config *config;

#define MAX_BASEDATA_ROWCOUNT 9
#define MAX_HOROSCOPEDATA_ROWCOUNT 7

/*****************************************************
**
**   GenericTableWriter   ---   Constructor
**
******************************************************/
GenericTableWriter::GenericTableWriter( Horoscope* horoscope )
 : h( horoscope )
{
	vobjects = config->vedic->objects;
	wobjects = config->western->objects;
	format = TF_MEDIUM;
}

/*****************************************************
**
**   GenericTableWriter   ---   getNumCols4ColumnType
**
******************************************************/
uint GenericTableWriter::getNumCols4ColumnType( const TcColumnSet &set, const TAB_COLUMN_TYPE &type )
{
	assert( type >= TAB_CT_NONE && type <= TAB_CT_ERROR );

	switch ( type )
	{
		case TAB_CT_NONE:
			return 0;
		break;
		case TAB_CT_OBJECT_NAME:
		case TAB_CT_LONGITUDE:
		case TAB_CT_LATITUDE:
		case TAB_CT_SIGN_LORD:
		case TAB_CT_DIGNITY:
		case TAB_CT_NAVAMSA:
		case TAB_CT_NAKSHATRA:
		case TAB_CT_NAKSHATRA_PADA:
		case TAB_CT_SHASTIAMSA_LORD:
		case TAB_CT_DASA_VARGA:
		case TAB_CT_HOUSE_POSITION:
		case TAB_CT_CHARA_KARAKA:
		case TAB_CT_AV_REKHAPOINTS:
		case TAB_CT_ELEMENT:
		case TAB_CT_SIGNIFICATION:
		case TAB_CT_SIGN_QUALITY:
		case TAB_CT_90_DEGREE_LONGITUDE:
		case TAB_CT_45_DEGREE_LONGITUDE:
		case TAB_CT_ANTISCIA:
		case TAB_CT_EMPTY:
			return 1;
		break;
		case TAB_CT_NAME_LONGITUDE:
		case TAB_CT_CUSTOM_KEY_VALUE:
		case TAB_CT_ERROR:
			return 2;
		break;
		case TAB_CT_KP_LORDS:
			return 3;
		break;
		case TAB_CT_WESTERN_ASPECTARIUM:
			calcObjectList( set );
			return obs.size();
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   createObjectListTableByConfigKeys
**
******************************************************/
Table *GenericTableWriter::createObjectListTableByConfigKeys( const OBJECT_INCLUDES &objects,
	const TAB_COLUMN_TYPE &style, const bool &vedic, const TEXT_FORMAT tf )
{
	TcColumnSet colset( TAB_LC_PLANETS, vedic );
	Tc tc;
	appendObjectListTcByConfigKeys( tc, objects, style, vedic, tf );
	tc.colsets.push_back( colset );
	return createTable( &tc );
}

/*****************************************************
**
**   GenericTableWriter   ---   appendObjectListTcByConfigKeys
**
******************************************************/
void GenericTableWriter::appendObjectListTcByConfigKeys( Tc &tc, const OBJECT_INCLUDES &objects,
	const TAB_COLUMN_TYPE &style, const bool &vedic, const TEXT_FORMAT )
{
	if ( vedic ) setVedicObjects( objects );
	else setWesternObjects( objects );

	TcColumnSet colset( TAB_LC_PLANETS, vedic );
	colset.cols.push_back( TcColumn( TAB_CT_OBJECT_NAME, vedic ));
	colset.cols.push_back( TcColumn( TAB_CT_LONGITUDE, vedic ));

	if ( style & TAB_CT_LATITUDE ) colset.cols.push_back( TcColumn( TAB_CT_LATITUDE, vedic ));
	if ( style & TAB_CT_90_DEGREE_LONGITUDE ) colset.cols.push_back( TcColumn( TAB_CT_90_DEGREE_LONGITUDE, vedic ));
	if ( style & TAB_CT_45_DEGREE_LONGITUDE ) colset.cols.push_back( TcColumn( TAB_CT_45_DEGREE_LONGITUDE, vedic ));
	if ( style & TAB_CT_ANTISCIA ) colset.cols.push_back( TcColumn( TAB_CT_ANTISCIA, vedic ));
	if ( style & TAB_CT_SIGN_LORD ) colset.cols.push_back( TcColumn( TAB_CT_SIGN_LORD, vedic ));
	if ( style & TAB_CT_ELEMENT ) colset.cols.push_back( TcColumn( TAB_CT_ELEMENT, vedic ));
	if ( style & TAB_CT_SIGNIFICATION ) colset.cols.push_back( TcColumn( TAB_CT_SIGNIFICATION, vedic ));
	if ( style & TAB_CT_SIGN_QUALITY ) colset.cols.push_back( TcColumn( TAB_CT_SIGN_QUALITY, vedic ));
	if ( style & TAB_CT_DIGNITY ) colset.cols.push_back( TcColumn( TAB_CT_DIGNITY, vedic ));
	if ( style & TAB_CT_NAVAMSA ) colset.cols.push_back( TcColumn( TAB_CT_NAVAMSA, vedic ));
	if ( style & TAB_CT_NAKSHATRA ) colset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA, vedic ));
	if ( style & TAB_CT_NAKSHATRA_PADA ) colset.cols.push_back( TcColumn( TAB_CT_NAKSHATRA_PADA, vedic ));
	if ( style & TAB_CT_KP_LORDS ) colset.cols.push_back( TcColumn( TAB_CT_KP_LORDS, vedic ));
	if ( style & TAB_CT_SHASTIAMSA_LORD ) colset.cols.push_back( TcColumn( TAB_CT_SHASTIAMSA_LORD, vedic ));
	if ( style & TAB_CT_CHARA_KARAKA ) colset.cols.push_back( TcColumn( TAB_CT_CHARA_KARAKA, vedic ));
	if ( style & TAB_CT_DASA_VARGA ) colset.cols.push_back( TcColumn( TAB_CT_DASA_VARGA, vedic ));
	if ( style & TAB_CT_HOUSE_POSITION ) colset.cols.push_back( TcColumn( TAB_CT_HOUSE_POSITION, vedic ));
	if ( style & TAB_CT_AV_REKHAPOINTS ) colset.cols.push_back( TcColumn( TAB_CT_AV_REKHAPOINTS, vedic ));

	tc.colsets.push_back( colset );
}

/*****************************************************
**
**   GenericTableWriter   ---   createTable
**
******************************************************/
Table *GenericTableWriter::createTable( Tc *conf )
{
	tcfg = conf;
	uint i, j;

	// calculate number of cols and rows
	uint cols = 0;
	uint rows = 0;
	for( i = 0; i < tcfg->colsets.size(); i++ )
	{
		rows = Max( rows, calcRowCount( tcfg->colsets[i] ) + 1 );
		for( j = 0; j < tcfg->colsets[i].cols.size(); j++ )
		{
			cols +=  getNumCols4ColumnType( tcfg->colsets[i], tcfg->colsets[i].cols[j].type );
		}
	}
	if ( cols == 0 )
	{
		wxLogError( wxT( "ERROR: table has no cols" ));
		assert( false );
	}

	wxLogDebug( wxString::Format( wxT( "Create table cols %u rows %u" ), cols, rows ));
	table = new Table( cols, rows );
	//for( uint i = 0; i < cols; i++ ) for( uint j = 0; j < rows; j++ ) table->setEntry( i, j, wxT( "a" ));

	uint currentcol = 0;
	for( i = 0; i < tcfg->colsets.size(); i++ )
	{
		calcObjectList( tcfg->colsets[i] );
		for( j = 0; j < tcfg->colsets[i].cols.size(); j++ )
		{
			switch( tcfg->colsets[i].cols[j].type )
			{
				case TAB_CT_NONE:
				break;
				case TAB_CT_CUSTOM_KEY_VALUE:
					//printf( "HEADER tcfg->colsets[i].cols[j]size %ld rows %u\n", tcfg->colsets[i].cols[j].cells.size(), rows );
					writeCustomKeyValueList( currentcol, tcfg->colsets[i].cols[j] );
				break;
				case TAB_CT_OBJECT_NAME:
					writeObjectName( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_LONGITUDE:
					writeObjectLongitude( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_LATITUDE:
					writeObjectLatitude( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_NAME_LONGITUDE:
					writeObjectNameAndLongitude( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_90_DEGREE_LONGITUDE:
					write90DegreeLongitude( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_45_DEGREE_LONGITUDE:
					write45DegreeLongitude( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_ANTISCIA:
					writeAntiscia( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_SIGN_LORD:
					writeSignLords( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_DIGNITY:
					writeDignities( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_NAVAMSA:
					writeNavamsa( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_NAKSHATRA:
					writeNakshatras( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_NAKSHATRA_PADA:
					writeNakshatraPadas( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_KP_LORDS:
					writeKpLords( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_SHASTIAMSA_LORD:
					writeShastiamsaLords( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_DASA_VARGA:
					writeDasaVarga( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_HOUSE_POSITION:
					writeBhavaPositions( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_CHARA_KARAKA:
					writeCharaKarakas( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_AV_REKHAPOINTS:
					writeAvRekhaPoints( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_ELEMENT:
					writeElements( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_SIGNIFICATION:
					writeSignification( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_SIGN_QUALITY:
					writeSignQualities( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_WESTERN_ASPECTARIUM:
					writeAspectarium( currentcol, tcfg->colsets[i] );
				break;
				case TAB_CT_ERROR:
					writeErrorColumn( currentcol, tcfg->colsets[i].cols[j] );
				break;
				case TAB_CT_EMPTY:
					//printf( "TODO EMPTY COL\n" );
				break;
				default:
					assert( false );
				break;
			}
			currentcol += getNumCols4ColumnType( tcfg->colsets[i], tcfg->colsets[i].cols[j].type );
		}
	}
	return table;
}

/*****************************************************
**
**   GenericTableWriter   ---   getObjectPosition
**
******************************************************/
ObjectPosition GenericTableWriter::getObjectPosition( const int &id, const TcColumnSet &set )
{
	switch( set.listcontext )
	{
		case TAB_LC_PLANETS:
		case TAB_LC_URANIAN:
		case TAB_LC_HOUSE_CUSPS:
		{
			return h->getObjectPosition( (ObjectId)id, set.vedic );
		}
		break;
		//case TAB_LC_HOUSE_CUSPS:
			//return ObjectPosition( h->getHouse( id, set.vedic ));
		break;
		default:
		break;
	}
	return ObjectPosition();
}

/*****************************************************
**
**   GenericTableWriter   ---   calcRowCount
**
******************************************************/
uint GenericTableWriter::calcRowCount( const TcColumnSet &set )
{
	switch( set.listcontext )
	{
		case TAB_LC_PLANETS:
			obs = set.vedic ?  PlanetList().getVedicObjectList( vobjects ) : PlanetList().getWesternObjectList( wobjects );
			return obs.size();
		break;
		case TAB_LC_URANIAN:
			return 8;
		break;
		case TAB_LC_HOUSE_CUSPS:
			return 12;
		break;
		case TAB_LC_NONE:
		{
			uint maxrow = 0;
			for( uint i = 0; i < set.cols.size(); i++ )
			{
				maxrow = Max( maxrow, set.cols[i].cells.size());
			}
			return maxrow;
		}
		break;
		case TAB_LC_ERROR:
			return 1;
		break;
		case TAB_LC_EMPTY:
			return 0;
		break;
		default:
			wxLogError( wxString::Format( wxT( "ERROR GenericTableWriter::calcRowCount: wrong list context %d" ), set.listcontext ));
			return 12;
			//assert( false );
		break;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   calcObjectList
**
******************************************************/
void GenericTableWriter::calcObjectList( const TcColumnSet& set )
{
	obs.clear();
	switch( set.listcontext )
	{
		case TAB_LC_PLANETS:
			obs = set.vedic ?  PlanetList().getVedicObjectList( vobjects ) : PlanetList().getWesternObjectList( wobjects );
		break;
		case TAB_LC_HOUSE_CUSPS:
			for( ObjectId i = OHOUSE1; i <= OHOUSE12; i++ ) obs.push_back( i );
		break;
		case TAB_LC_URANIAN:
			for( ObjectId i = OCUPIDO; i <= OPOSEIDON; i++ ) obs.push_back( i );
		break;
		default:
		break;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeCustomKeyValueList
**
******************************************************/
void GenericTableWriter::writeCustomKeyValueList( const uint &i0, const TcColumn &c )
{
	int row = 1;
	/*
	wxString title = wxT( "Untitled" );
	if ( ! c.title.IsEmpty()) title = c.title;
	
	//table->setHeader( i0,  title, 1);
	table->setHeader( i0,  title );
	table->setHeader( i0 + 1, wxEmptyString );
	table->setHeader( i0, wxEmptyString );
	table->setHeader( i0 + 1, wxEmptyString );
	*/
	table->setHeader( i0, wxT( " " ));
	table->setHeader( i0 + 1, wxT( " " ));

	for( uint i = 0; i < c.cells.size(); i++ )
	{
		if ( c.cells[i].type != TAB_CELL_ERROR )
		{
			writeCustomDataEntry( i0, row, c.cells[i]. type );
		}
		else
		{
			table->setEntry( i0, i + 1, wxT( "Error" ));
			table->setEntry( i0 + 1, i + 1, c.cells[i].errorMsg );
		}
		row++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeObjectName
**
******************************************************/
void GenericTableWriter::writeObjectName( const uint &i0, const TcColumnSet &set )
{
	//printf( "GenericTableWriter::writeObjectName %d, listcontext %d rows %d size %d\n", i0, (int)set.listcontext,  (int)table->getNbRows(), (int)obs.size());
	assert( table->getNbCols() >= i0 + 1 );
	assert( table->getNbRows() > obs.size());

	table->setHeader( i0,  wxT( "" ));

	for ( uint p = 0; p < obs.size(); p++ )
	{
		table->setEntry( i0, p + 1, getObjectName( obs[p], set ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   getObjectName
**
******************************************************/
MString GenericTableWriter::getObjectName( const int &objid, const TcColumnSet &set )
{
	SheetFormatter fmt;

	switch( set.listcontext )
	{
		case TAB_LC_PLANETS:
		case TAB_LC_URANIAN:
			return fmt.getObjectName( (ObjectId)objid, format, set.vedic );
		break;
		case TAB_LC_HOUSE_CUSPS:
			return fmt.getObjectNamePlain( (ObjectId)objid, format, set.vedic );
		break;
		break;
		default:
			return wxString::Format( wxT ( "ERROR GenericTableWriter::getObjectName: invalid list context %u\n" ), (int)set.listcontext );
		break;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeObjectLongitude
**
******************************************************/
void GenericTableWriter::writeObjectLongitude( const uint &colid, const TcColumnSet &set )
{
	assert( table->getNbCols() >= colid + 1 );
	assert( table->getNbRows() > obs.size());

	ObjectPosition pos;
	SheetFormatter fmt;

	if ( set.header.IsEmpty())
	table->setHeader( colid,  _( "Longitude" ));
	else
	table->setHeader( colid,  set.header );
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( colid, p + 1, fmt.getPosFormatted( pos.longitude, pos.direction, DEG_PRECISION_SECOND, format ));
		if ( IS_EPHEM_OBJECT( obs[p] ) && h->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   write90DegreeLongitude
**
******************************************************/
void GenericTableWriter::write90DegreeLongitude( const uint &colid, const TcColumnSet &set )
{
	assert( table->getNbCols() >= colid + 1 );
	assert( table->getNbRows() > obs.size());

	Formatter *formatter = Formatter::get();

	table->setHeader( colid,  _( "90 Degree" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		table->setEntry( colid, p + 1, formatter->getLenFormatted( a_red( getObjectPosition( obs[p], set ).longitude, 90.0 )));
		if ( IS_EPHEM_OBJECT( obs[p] ) && h->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   write45DegreeLongitude
**
******************************************************/
void GenericTableWriter::write45DegreeLongitude( const uint &colid, const TcColumnSet &set )
{
	assert( table->getNbCols() >= colid + 1 );
	assert( table->getNbRows() > obs.size());
	Formatter *formatter = Formatter::get();

	table->setHeader( colid,  _( "45 Degree" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		table->setEntry( colid, p + 1, formatter->getLenFormatted( a_red( getObjectPosition( obs[p], set ).longitude, 45.0 )));
		if ( IS_EPHEM_OBJECT( obs[p] ) && h->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeAntiscia
**
******************************************************/
void GenericTableWriter::writeAntiscia( const uint &colid, const TcColumnSet &set )
{
	assert( table->getNbCols() >= colid + 1 );
	assert( table->getNbRows() > obs.size());

	ObjectPosition pos;
	SheetFormatter fmt;

	table->setHeader( colid,  _( "Antiscium" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( colid, p + 1, fmt.getPosFormatted( getAntiscium( pos.longitude ),
			pos.direction, DEG_PRECISION_SECOND, TF_SHORT ));
		if ( IS_EPHEM_OBJECT( obs[p] ) && h->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeObjectNameAndLongitude
**
******************************************************/
void GenericTableWriter::writeObjectNameAndLongitude( const uint &colid, const TcColumnSet &set )
{
	assert( table->getNbCols() >= colid + 2 );
	assert( table->getNbRows() > obs.size());
	SheetFormatter fmt;

	ObjectPosition pos;
	wxString s;

	switch( set.listcontext )
	{
		case TAB_LC_PLANETS:
			s = _( "Planet" );
		break;
		case TAB_LC_HOUSE_CUSPS:
			s = _( "House Cusp" );
		break;
		case TAB_LC_URANIAN:
			s = _( "Uranian" );
		break;
		default:
			s = wxT( "ERROR" );
		break;
	}
	table->setHeader( colid,  s );

	for ( uint p = 0; p < obs.size(); p++ )
	{
		table->setEntry( colid, p + 1, getObjectName( obs[p], set ));
		pos = getObjectPosition( obs[p], set );
		table->setEntry( colid + 1, p + 1, fmt.getPosFormatted( pos.longitude, pos.direction, DEG_PRECISION_SECOND ));
		if ( h->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeObjectLatitude
**
******************************************************/
void GenericTableWriter::writeObjectLatitude( const uint &i0, const TcColumnSet &set )
{
  table->setHeader( i0,  _( "Declination" ));
	Formatter *f = Formatter::get();
	ObjectPosition pos;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( i0, p + 1, f->getLatitudeFormatted( pos.latitude, DEG_PRECISION_SECOND ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeNakshatras
**
******************************************************/
void GenericTableWriter::writeNakshatras( const uint &i0, const TcColumnSet &set )
{
	Lang lang;
  //table->setHeader( i0, _( "Nakshatra" ));
  wxString header = _( "Nakshatra" );
	if ( format == TF_SHORT ) header = header.Left( 3 );
  table->setHeader( i0, header );
	assert( table->getNbRows() > obs.size() );

	for ( uint p = 0; p < obs.size(); p++ )
	{
		table->setEntry( i0, p + 1,
			lang.getNakshatra27Name( ::getNakshatra27( getObjectPosition( obs[p], set ).longitude ), format ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeSignLords
**
******************************************************/
void GenericTableWriter::writeSignLords( const uint &i0, const TcColumnSet &set )
{
	ObjectPosition pos;
	SheetFormatter fmt;

  table->setHeader( i0,  _( "Lord" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( i0, p + 1, fmt.getObjectName( getLord( getRasi( pos.longitude ), set.vedic ), format, set.vedic ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeDignities
**
******************************************************/
void GenericTableWriter::writeDignities( const uint &i0, const TcColumnSet& )
{
	Lang lang;
  table->setHeader( i0,  _( "Dignity" ));

	for ( uint p = 0; p < obs.size(); p++ )
	{
		if ( obs[p] >= OSUN && obs[p] <= OSATURN )
		{
			table->setEntry( i0, p + 1, lang.getDignityName( h->getVargaData( obs[p], V_RASI )->getDignity(), format ));
		}
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeNavamsa
**
******************************************************/
void GenericTableWriter::writeNavamsa( const uint &i0, const TcColumnSet &set )
{
	ObjectPosition pos;
	SheetFormatter fmt;

  table->setHeader( i0,  _( "D-9" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( i0, p + 1, fmt.getSignName( getRasi( pos.longitude * 9 ), format ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeElements
**
******************************************************/
void GenericTableWriter::writeElements( const uint &i0, const TcColumnSet &set )
{
  table->setHeader( i0,  _( "Element" ));
	Lang lang;
	ObjectPosition pos;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( i0, p + 1, lang.getElementName( getRasi( pos.longitude )));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeSignification
**
******************************************************/
void GenericTableWriter::writeSignification( const uint &i0, const TcColumnSet &set )
{
  table->setHeader( i0,  _( "Signification" ));
	Lang lang;
	for ( uint i = 0; i < obs.size(); i++ )
	{
		table->setEntry( i0, i + 1, lang.getObjectSignification( obs[i], TF_LONG, set.vedic ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeSignQualities
**
******************************************************/
void GenericTableWriter::writeSignQualities( const uint &i0, const TcColumnSet &set )
{
	Lang lang;
  table->setHeader( i0,  _( "Quality" ));
	ObjectPosition pos;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( i0, p + 1, lang.getSignQualityName( getRasi( pos.longitude )));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeAspectarium
**
******************************************************/
void GenericTableWriter::writeAspectarium( const uint &i0, const TcColumnSet &set )
{
	AspectExpert expert( h, h );
	std::list<AspectEvent>::iterator iter;
	TableEntry *e;

	ChartProperties props;
	props.setVedic( set.vedic );

	std::list<AspectEvent> al = expert.updateWesternAspects( &props, CT_RADIX );

	for ( uint p = 0; p < obs.size(); p++ )
	{
		table->setHeader( i0 + p,  getObjectName( obs[p], set ));
		for ( uint q = 0; q < obs.size(); q++ )
		{
			if ( p == q ) table->setEntry( i0 + p, q + 1, wxT( "-" ));
			else
			{
				for ( iter = al.begin(); iter != al.end(); iter++ )
				{
					if ( (*iter).planet1 == obs[p] && (*iter).planet2 == obs[q] )
					{
						e = table->getEntry( i0 + p , q + 1 );
						e->text.add( MToken( TTSE_ASPECT, (*iter).aspectType, TF_MEDIUM ));

						e = table->getEntry( i0 + q , p + 1 );
						e->text.add( MToken( TTSE_ASPECT, (*iter).aspectType, TF_MEDIUM ));
						//s << t.getAspectSymbol( (*iter).aspectType );
						//table->setEntry( i0 + p , q + 1, s );
						//table->setEntry( i0 + q , p + 1, s );
					}
				}
			}
		}
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeNakshatraPadas
**
******************************************************/
void GenericTableWriter::writeNakshatraPadas( const uint &i0, const TcColumnSet &set )
{
  table->setHeader( i0,  _( "Pada" ));
	ObjectPosition pos;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		table->setEntry( i0, p + 1, wxString::Format( wxT( "%d" ), (int)(getNakshatraLongitude27( pos.longitude ) / 3.3333333333 ) + 1 ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeShastiamsaLords
**
******************************************************/
void GenericTableWriter::writeShastiamsaLords( const uint &i0, const TcColumnSet &set )
{
	int shast;
	Lang lang;
	ObjectPosition pos;

	const static bool k_shastiamsa_benefic[60] = {
		false, false, true, true, true, true, false, false, false, false,
		false, false, true, true, false, false, true, true, true, true,
		true, true, true, true, true, true, true, true, true, false,
		false, false, false, false, false, false, true, true, true, false,
		false, false, false, false, true, true, true, false, true, true,
		false, false, true, true, false, true, true, true, false, true,
	};

// Dignities

  table->setHeader( i0,  _( "D-60 Lords" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		pos = getObjectPosition( obs[p], set );
		shast = (int)( a_red( pos.longitude, 30 ) * 2 );
		if ( isEvenRasi( pos.longitude )) shast = 59 - shast;
		assert( shast >= 0 && shast < 60 );
		table->setEntry( i0, p + 1, wxString::Format( wxT( "%s (%c)" ), lang.getShastiamsaName( shast ).c_str(),
			( k_shastiamsa_benefic[(int)shast] ? 'B' : 'M' )));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeDasaVarga
**
******************************************************/
void GenericTableWriter::writeDasaVarga( const uint &i0, const TcColumnSet& )
{
	VargaExpert expert;
	Lang lang;
	VedicPlanet *planet;
	int dignity;

  table->setHeader( i0,  _( "D-10" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		if ( obs[p] <= OSATURN )
		{
			planet = h->getPlanet( obs[p] );
			dignity = planet->getVimsopakaBalaGoodVargas( 2 );

			if ( dignity <= 1 ) table->setEntry( i0, p + 1, wxString::Format( wxT( "%d" ), dignity ));
			else table->setEntry( i0, p + 1, wxString::Format( wxT( "%s (%d)" ), lang.getVimsopakaDignityName( 2, dignity ).c_str(), dignity ));
		}
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeBhavaPositions
**
******************************************************/
void GenericTableWriter::writeBhavaPositions( const uint &i0, const TcColumnSet &set )
{
	VargaHoroscope bchart( h, V_BHAVA );
  table->setHeader( i0,  set.vedic ? _( "Bhava" ) : _( "House" ));
	for ( uint p = 0; p < obs.size(); p++ )
	{
		if ( ! IS_HOUSE_OBJECT( obs[p] ))	
			table->setEntry( i0, p + 1, wxString::Format( wxT( "%d" ), red12( bchart.getRasi( obs[p] ) - bchart.getRasi( OASCENDANT )) + 1 ));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeKpLords
**
******************************************************/
void GenericTableWriter::writeKpLords( const uint &i0, const TcColumnSet &set, const int depth )
{
	assert( table->getNbRows() > obs.size() );
	SheetFormatter fmt;

	//printf( "GenericTableWriter::writeKpLords format %d\n", format );

  table->setHeader( i0,  _( "L" ));
  if ( depth > 1 ) table->setHeader( i0 + 1,  _( "SL" ));
  if ( depth > 2 ) table->setHeader( i0 + 2,  _( "SSL" ));

	h->updateKP( D_VIMSOTTARI );

	if ( set.listcontext == TAB_LC_PLANETS )
	{
		for ( uint p = 0; p < obs.size(); p++ )
		{
			KpData kp = h->getKPLords( obs[p] );
			table->setEntry( i0, p + 1, fmt.getObjectName( (ObjectId)kp.lord, format, true ));
			if ( depth > 1 ) table->setEntry( i0 + 1, p + 1, fmt.getObjectName( (ObjectId)kp.sublord, format, true ));
			if ( depth > 2 ) table->setEntry( i0 + 2, p + 1, fmt.getObjectName( (ObjectId)kp.subsublord, format, true ));
		}
	}
	else if ( set.listcontext == TAB_LC_HOUSE_CUSPS )
	{
		for ( uint p = HOUSE1; p <= HOUSE12; p++ )
		{
			KpData kp = h->getHouseKPLords( p );
			table->setEntry( i0, p + 1, fmt.getObjectName( (ObjectId)kp.lord, format, true ));
			if ( depth > 1 ) table->setEntry( i0 + 1, p + 1, fmt.getObjectName( (ObjectId)kp.sublord, format, true ));
			if ( depth > 2 ) table->setEntry( i0 + 2, p + 1, fmt.getObjectName( (ObjectId)kp.subsublord, format, true ));
		}
	}
	else wxLogError( wxString::Format( wxT( "wrong list context %d in GenericTableWriter::writeKpLords" ), (int)set.listcontext ));
}

/*****************************************************
**
**   GenericTableWriter   ---   writeAvRekhaPoints
**
******************************************************/
void GenericTableWriter::writeAvRekhaPoints( const uint &i0, const TcColumnSet &set )
{
  table->setHeader( i0,  _( "AV" ));
	if ( set.listcontext != TAB_LC_PLANETS || ! set.vedic ) return;

	AshtakavargaExpert expert( h, V_RASI );
	expert.update();

	uint line = 1;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		assert( table->getNbRows() > line );

		if (( obs[p] >= OSUN && obs[p] <= OSATURN ) || obs[p] == OASCENDANT )
		{
			table->setEntry( i0, line, wxString::Format( wxT( "%d" ), expert.getItem( REKHA, obs[p], getRasi( h->getVedicLongitude( obs[p] )))));
		}
		line++;
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeCharaKarakas
**
******************************************************/
void GenericTableWriter::writeCharaKarakas( const uint &i0, const TcColumnSet &set )
{
  table->setHeader( i0,  _( "Karaka" ));
	if ( set.listcontext != TAB_LC_PLANETS || ! set.vedic ) return;

	Lang lang;
	ObjectId p;
	const bool cmode = config->vedicCalculation->charaKarakaMode; // 0: Parasara, 1: Raman

	JaiminiExpert jexpert( h, V_RASI );
	jexpert.calcCharaKarakas();

	for ( uint i = 0; i < obs.size(); i++ )
	{
		assert( table->getNbRows() > i );

		// map planet id to karaka index - quite ugly
		p = obs[i];
		if ( cmode ) // Raman
		{
			if ( p > OSATURN ) continue;
		}
		else // Parasara
		{
			if ( p == OURANUS ) continue;
			if ( p == OMEANNODE || p == OTRUENODE ) p = (ObjectId)7;
			if ( p > 7 ) continue;
		}
		table->setEntry( i0, i + 1, lang.getKarakaName( jexpert.getCharaKarakaProperty( p )));
	}
}

/*****************************************************
**
**   GenericTableWriter   ---   writeCustomDataEntry
**
******************************************************/
void GenericTableWriter::writeCustomDataEntry( const uint &col, const uint &row, const TAB_CELL_TYPE &type )
{
	assert( table->getNbCols() > col );
	assert( table->getNbRows() > row );
	Formatter *f = Formatter::get();
	DateTimeFormatter *df = DateTimeFormatter::get();
	Lang lang;
	wxString s;
	
	switch( type )
	{
		case TAB_CELL_NAME:
			table->setEntry( col, row,  _( "Name" ));
			table->setEntry( col + 1, row, h->getHName() );
		break;
		case TAB_CELL_FULL_DATE:
		{
			table->setEntry( col, row,  _( "Date" ));
			Location *loc = h->getDataSet()->getLocation();
			table->setEntry( col + 1, row, df->formatDateString( h->getJD(), loc->getTimeZone() + loc->getDST()));
		}
		break;
		case TAB_CELL_LOCAL_TIME:
		{
			table->setEntry( col, row,  _( "Local Time" ));
			Location *loc = h->getDataSet()->getLocation();
			table->setEntry( col + 1, row, f->getTimeFormatted( a_red( getTimeFromJD( h->getDataSet()->getDate()->getJD()) + loc->getTimeZone() + loc->getDST(), 24 )));
		}
		break;
		case TAB_CELL_LOCATION_NAME:
			table->setEntry( col, row,  _( "Location" ));
			table->setEntry( col + 1, row, h->getDataSet()->getLocation()->getLocName() );
		break;
		case TAB_CELL_LOCATION_LONGITUDE:
		{
			table->setEntry( col, row,  _( "Longitude" ));
			Location *loc = h->getDataSet()->getLocation();
			table->setEntry( col + 1, row, f->getLongitudeFormatted( loc->getLongitude()));
		}
		break;
		case TAB_CELL_LOCATION_LATITUDE:
		{
			table->setEntry( col, row,  _( "Latitude" ));
			Location *loc = h->getDataSet()->getLocation();
			table->setEntry( col + 1, row, f->getLatitudeFormatted( loc->getLatitude()));
		}
		break;
		case TAB_CELL_TZ_NAME:
		{
			table->setEntry( col, row,  _( "Time Zone" ));
			Location *loc = h->getDataSet()->getLocation();
			const double tz = loc->getTimeZone();
			table->setEntry( col + 1, row, wxString::Format( wxT( "%s %c%.1f" ), _( "UT" ), ( tz >= 0 ? '+' : ' ' ), tz ));
		}
		break;
		case TAB_CELL_DST:
		{
			table->setEntry( col, row,  _( "Daylight Saving" ));
			Location *loc = h->getDataSet()->getLocation();
			table->setEntry( col + 1, row, wxString::Format( wxT( "%.1f" ), loc->getDST()));
		}
		break;
		case TAB_CELL_UNIVERSAL_TIME:
			table->setEntry( col, row,  _( "Universal Time" ));
			table->setEntry( col + 1, row, f->getTimeFormatted( getTimeFromJD( h->getJD() )));
		break;
		case TAB_CELL_SIDEREAL_TIME:
			table->setEntry( col, row,  _( "Sidereal Time" ));
			table->setEntry( col + 1, row, f->getTimeFormatted( h->getSiderealTime()));
		break;
		case TAB_CELL_JULIAN_DATE:
			table->setEntry( col, row,  _( "Julian Date" ));
			table->setEntry( col + 1, row, wxString::Format( wxT( "%8.5f" ), h->getJD()));
		break;
		case TAB_CELL_SUNRISE:
		{
			table->setEntry( col, row,  _( "Sunrise" ));
			Location *loc = h->getLocation();
			if ( h->getSunrise() != 0 )
			{
				s = f->getTimeFormatted( getTimeFromJD( h->getSunrise() + ( loc->getTimeZone() + loc->getDST()) / 24.0 ));
			}
			else s = _( "n.a." );
			table->setEntry( col + 1, row, s );
		}
		break;
		case TAB_CELL_SUNSET:
		{
			table->setEntry( col, row,  _( "Sunset" ));
			Location *loc = h->getLocation();
			if ( h->getSunset() != 0 )
			{
				s = f->getTimeFormatted( getTimeFromJD( h->getSunset() + ( loc->getTimeZone() + loc->getDST()) / 24.0 ));
			}
			else s = _( "n.a." );
			table->setEntry( col + 1, row, s );
		}
		break;
		case TAB_CELL_VEDIC_AYANAMSA:
			table->setEntry( col, row,  _( "Vedic Ayanamsa" ));
			table->setEntry( col + 1, row, f->getAyanamsaNameAndValue( h->getAyanamsaType( true ), h->getAyanamsa( true )));
		break;
		case TAB_CELL_WESTERN_AYANAMSA:
			table->setEntry( col, row,  _( "Western Ayanamsa" ));
			table->setEntry( col + 1, row, f->getAyanamsaNameAndValue( h->getAyanamsaType( false ), h->getAyanamsa( false )));
		break;
		case TAB_CELL_VARNA:
			table->setEntry( col, row,  _( "Varna" ));
			table->setEntry( col + 1, row, lang.getVarnaName( getVarna( h->getVedicLongitude( OMOON ))));
		break;
		case TAB_CELL_GANA:
			table->setEntry( col, row,  _( "Gana" ));
			table->setEntry( col + 1, row, lang.getGanaName( NakshatraExpert().getGana( h->getVedicLongitude( OMOON ))));
		break;
		case TAB_CELL_YONI:
		{
			Yoni yoni = NakshatraExpert().getYoni( h->getVedicLongitude( OMOON ));
			table->setEntry( col, row,  _( "Yoni" ));
			table->setEntry( col + 1, row, yoni.name );
		}
		break;
		case TAB_CELL_NADI:
			table->setEntry( col, row,  _( "Nadi" ));
			table->setEntry( col + 1, row, lang.getNadiName( NakshatraExpert().getNadi( h->getVedicLongitude( OMOON ))));
		break;
		case TAB_CELL_RAJJU:
		{
			Rajju rajju = NakshatraExpert().getRajju( getNakshatra27( h->getVedicLongitude( OMOON )));
			table->setEntry( col, row,  _( "Rajju" ));
			table->setEntry( col + 1, row, lang.getRajjuName( rajju.aroha, rajju.type ));
		}
		break;
		case TAB_CELL_INCFLUENCE_TIME:
		{
			DasaExpert *expert = DasaExpertFactory().getDasaExpert( D_VIMSOTTARI, h );
			table->setEntry( col, row,  _( "Influence" ));
			table->setEntry( col + 1, row, expert->getDasaLordNameF( expert->getBirthMahaDasaLord(), TF_LONG ));
		}
		break;

		case TAB_CELL_LUNAR_TITHI:
		{
			double angle = red_deg( h->getVedicLongitude( OMOON ) - h->getVedicLongitude( OSUN ));
			table->setEntry( col, row,  _( "Lunar Tithi" ));
			table->setEntry( col + 1, row,  lang.getTithiName( angle / 12 ));
		}
		break;
		case TAB_CELL_WEEKDAY:
			table->setEntry( col, row,  _( "Weekday" ));
			table->setEntry( col + 1, row,  lang.getWeekdayName( h->getDataSet()->getWeekDay() ));
		break;

		case TAB_CELL_HORA_LORD:
		case TAB_CELL_DINA_LORD:
		case TAB_CELL_MASA_LORD:
		case TAB_CELL_VARSHA_LORD:
			writeHoraEntry( col, row, type );
		break;
		default:
			table->setEntry( col, row,  wxString::Format( wxT( "Error, wrong custom data entry id %d" ), (int)type ));
		break;
	};
}

/*****************************************************
**
**   GenericTableWriter   ---   writeHoraEntry
**
******************************************************/
void GenericTableWriter::writeHoraEntry( const int &col, const int &row, const TAB_CELL_TYPE &rt )
{
	HoraExpert expert;
	expert.setLocation( h->getLocation() );
	expert.update( h->getJD() );
	ObjectId lord = ONONE;
	wxString title;
	SheetFormatter fmt;
	
	switch( rt )
	{
		case TAB_CELL_HORA_LORD:
			title = _( "Hora Lord" );
			lord = expert.getHoraLord();
		break;
		case TAB_CELL_DINA_LORD:
			title = _( "Dina Lord" );
			lord = expert.getDinaLord();
		break;
		case TAB_CELL_MASA_LORD:
			title = _( "Masa Lord" );
			lord = expert.getMasaLord();
		break;
		case TAB_CELL_VARSHA_LORD:
			title = _( "Varsha Lord" );
			lord = expert.getVarshaLord();
		break;
		default:
			title = wxString::Format( wxT( "Error, wrong Hora lord id %d" ), (int)rt );
		break;
	}
	table->setEntry( col, row,  title );
	table->setEntry( col + 1, row, fmt.getObjectName( lord, format ));
}

/*****************************************************
**
**   GenericTableWriter   ---   writeErrorColumn
**
******************************************************/
void GenericTableWriter::writeErrorColumn( const uint &i0, const TcColumn &col )
{
  table->setHeader( i0,  _( "Error" ));
	table->setEntry( i0, 1, col.errorMsg );
}



