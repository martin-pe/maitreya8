/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/GenericTableWriter.h
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

#ifndef _GENERICTABLEWRITER_H_
#define _GENERICTABLEWRITER_H_

class Horoscope;
class Table;
class TableStyle;

#include "BasicHoroscope.h"
#include "maitreya.h"
#include "Sheet.h"

/*************************************************//**
*
*  
*
******************************************************/
class GenericTableWriter
{
public:
	GenericTableWriter( Horoscope* );
	Table *createTable( Tc* );

	Table *createObjectListTableByConfigKeys( const OBJECT_INCLUDES &objects, const TAB_COLUMN_TYPE &style,
		const bool &vedic, const TEXT_FORMAT f = TF_MEDIUM );
	void appendObjectListTcByConfigKeys( Tc&, const OBJECT_INCLUDES &objects,
		const TAB_COLUMN_TYPE &style, const bool &vedic, const TEXT_FORMAT = TF_MEDIUM );

	void setVedicObjects( const OBJECT_INCLUDES &obj ) { vobjects = obj; }
	void setWesternObjects( const OBJECT_INCLUDES &obj ) { wobjects = obj; }
	void setTextFormat( const TEXT_FORMAT &f ) { format = f; }

private:

	void writeObjectName( const uint &i0, const TcColumnSet& );
	void writeObjectLongitude( const uint &i0, const TcColumnSet& );
	void writeObjectNameAndLongitude( const uint &colid, const TcColumnSet& );
	void writeObjectLatitude( const uint &i0, const TcColumnSet& );
	void write90DegreeLongitude( const uint &i0, const TcColumnSet& );
	void write45DegreeLongitude( const uint &i0, const TcColumnSet& );
	void writeAntiscia( const uint &i0, const TcColumnSet& );

	void writeCustomKeyValueList( const uint &currentcol, const TcColumn& );

	void writeNakshatras( const uint &i0, const TcColumnSet& );
	void writeCharaKarakas( const uint &i0, const TcColumnSet& );
	void writeKpLords( const uint &i0, const TcColumnSet&, const int depth = 3 );
	void writeAvRekhaPoints( const uint &i0, const TcColumnSet& );

	void writeSignLords( const uint &i0, const TcColumnSet& );
	void writeDignities( const uint &i0, const TcColumnSet& );
	void writeNavamsa( const uint &i0, const TcColumnSet& );
	void writeNakshatraPadas( const uint &i0, const TcColumnSet& );
	void writeShastiamsaLords( const uint &i0, const TcColumnSet& );
	void writeDasaVarga( const uint &i0, const TcColumnSet& );
	void writeBhavaPositions( const uint &i0, const TcColumnSet& );

	void writeElements( const uint &i0, const TcColumnSet& );
	void writeSignification( const uint&, const TcColumnSet& );
	void writeSignQualities( const uint &i0, const TcColumnSet& );
	void writeAspectarium( const uint &i0, const TcColumnSet& );

	void writeCustomDataEntry( const uint &col, const uint &row, const TAB_CELL_TYPE &type );
	void writeHoraEntry( const int &i0, const int &row, const TAB_CELL_TYPE &rt );

	void writeErrorColumn( const uint&, const TcColumn& );

	void calcObjectList( const TcColumnSet& );

	uint calcRowCount( const TcColumnSet& );
	uint getNumCols4ColumnType( const TcColumnSet&, const TAB_COLUMN_TYPE& );
	ObjectPosition getObjectPosition( const int &id, const TcColumnSet& );
	MString getObjectName( const int &objid, const TcColumnSet& );

	Tc *tcfg;
	Horoscope *h;
	Table *table;
	ObjectArray obs;
	OBJECT_INCLUDES vobjects, wobjects;
	TEXT_FORMAT format;
};

#endif

