/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Table.h
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

#ifndef _TABLE_H_
#define _TABLE_H_

#include <vector>

#include "Sheet.h"

class Table;

using namespace std;

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
		width = 0;
	}

	TableEntry( const Table *t, const uint &colid, const uint &rowid, const wxString &v, const bool header = false )
		: SheetItem( WiCell ),
		table( t ),
		colid( colid ),
		rowid( rowid )
	{
		text = MString( v );
		isHeader = header;
		width = 0;
	}

	TableEntry( const Table *t, const uint &colid, const uint &rowid, const MString &f, const bool header = false )
		: SheetItem( WiCell ),
		table( t ),
		colid( colid ),
		rowid( rowid )
	{
		text = f;
		isHeader = header;
		width = 0;
	}

	SheetItem *cloneClean();

	virtual MRect getRefreshRect( const bool = false, const int selectionMode = 0 );

	MString text;
	wxString plainText;
	bool isHeader;
	double width;

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

	vector<TableEntry> value;
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

	virtual SheetItem *getSubitem4Point( const wxPoint& );
	void calculateSubitemRectangles();

	void setHeader( const wxString &s ) { setHeader( MToken( s )); }
	void setHeader( const MString& );
	void setHeader( const uint &col, const wxString& );
	void setHeader( const uint &col, const MString& );
	void setHeaderEntry( const uint&col, const uint &row, const wxString& );
	void setHeaderEntry( const uint&col, const uint &row, const MString& );

	void setEntry( const uint &col, const uint &row, const wxString&, const bool isHeader = false );
	void setEntry( const uint &col, const uint &row, const MString&, const bool isHeader = false );

	TableEntry *getEntry( const uint &col, const uint &row );
	vector<Row> contents;

	void addRow( Row );
	void resetRectangles();
	void setCenterAll();

	uint getNbRows() const { return contents.size(); }
	uint getNbCols() const { return nb_cols; }

	vector<double> col_width;
	vector<int> col_alignment;
  vector<bool> col_break;

	bool isEmptyCol( uint &c ) { return col_empty[c]; }
	bool uniformLayout; 

	SheetTextItem header;

private:
	uint nb_cols;
	vector<bool> col_empty;

public:
	int errorcount;
};

#endif


