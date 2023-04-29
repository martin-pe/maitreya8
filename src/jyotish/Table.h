/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Table.h
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

#ifndef _TABLE_H_
#define _TABLE_H_

#include <vector>

#include "Sheet.h"

class Table;

/*************************************************//**
*
* \brief represents a single entry of a Table
*
******************************************************/
class TableEntry : public SheetItem
{
	DECLARE_CLASS( TableEntry )
public:

	TableEntry( const Table *t, const uint &colid, const uint &rowid )
		: SheetItem( WiCell ),
		table( t ),
		colid( colid ),
		rowid( rowid )
	{
		isHeader = false;
	}

	TableEntry( const Table *t, const uint &colid, const uint &rowid, const wxString &v, const bool header = false )
		: SheetItem( WiCell ),
		table( t ),
		colid( colid ),
		rowid( rowid )
	{
		text = MString( v );
		isHeader = header;
	}

	TableEntry( const Table *t, const uint &colid, const uint &rowid, const MString &f, const bool header = false )
		: SheetItem( WiCell ),
		table( t ),
		colid( colid ),
		rowid( rowid )
	{
		text = f;
		isHeader = header;
	}

	SheetItem *cloneClean();

	virtual MRect getRefreshRect( const bool = false, const int selectionMode = 0 );

	MString text;
	wxString plainText;
	bool isHeader;

	const Table *table;
	uint colid, rowid;
};

/*************************************************//**
*
* \brief  represents a row in a Table
*
******************************************************/
class Row : public SheetItem
{
	DECLARE_CLASS( Row )
public:

	Row( const Table*, const uint nb_cols = 0, const uint rowid = NO_ROW);

	SheetItem *cloneClean();

	void addValue( const TableEntry& );

	void resetRectangles();

	std::vector<TableEntry> value;
	bool isEmpty();

	const Table *table;
	uint rowid;
};

/*************************************************//**
*
*
*
******************************************************/
class Table : public SheetItem
{
	DECLARE_CLASS( Table )
public:

	Table( const uint &cols, const uint &rows );
	SheetItem *cloneClean();

	void setHeader( const wxString &s ) { setHeader( MToken( s )); }
	void setHeader( const MString& );
	void setHeader( const uint &col, const wxString& );
	void setHeader( const uint &col, const MString& );
	void setHeaderEntry( const uint&col, const uint &row, const wxString& );
	void setHeaderEntry( const uint&col, const uint &row, const MString& );

	void setEntry( const uint &col, const uint &row, const wxString&, const bool isHeader = false );
	void setEntry( const uint &col, const uint &row, const MString&, const bool isHeader = false );

	TableEntry *getEntry( const uint &col, const uint &row );
	std::vector<Row> contents;

	void addRow( Row );
	void resetRectangles();
	void setCenterAll();

	uint getNbRows() const { return contents.size(); }
	uint getNbCols() const { return nb_cols; }

	std::vector<double> col_width;
	std::vector<int> col_alignment;
  std::vector<bool> col_break;

	bool isEmptyCol( uint &c ) { return col_empty[c]; }
	bool uniformLayout; 

	SheetTextItem header;

private:
	uint nb_cols;
	std::vector<bool> col_empty;

public:
	int errorcount;
};

#endif


