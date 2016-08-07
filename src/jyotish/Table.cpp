/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Table.cpp
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

#include "Table.h"

IMPLEMENT_CLASS( Table, SheetItem )
IMPLEMENT_CLASS( Row, SheetItem )
IMPLEMENT_CLASS( TableEntry, SheetItem )

/*****************************************************
**
**   TableEntry   ---   cloneClean
**
******************************************************/
SheetItem *TableEntry::cloneClean()
{
	TableEntry *entry = new TableEntry( *this );
	entry->width = 0;
	return entry;
}

/*****************************************************
**
**   TableEntry   ---   getRefreshRect
**
******************************************************/
MRect TableEntry::getRefreshRect( const bool /*ismarked*/, const int selectionMode )
{
	switch( selectionMode )
	{
		case 1: // cell
			return rect;
		break;
		case 2: // row
			return MRect( table->rect.x, rect.y, table->rect.width, rect.height );
		break;
		case 3: // row/col
			if ( isHeader && ( rowid == 0 || rowid == table->getNbRows() - 1 ))
			{
				return MRect( rect.x, table->rect.y, rect.width, table->rect.height);
			}
			else
			{
				return MRect( table->rect.x, rect.y, table->rect.width, rect.height );
			}
		break;
		case 0: // never
		default:
			return MRect();
		break;
	}
}

/*****************************************************
**
**   Row   ---   Constructor
**
******************************************************/
Row::Row( const Table *t, const uint cols, const uint rowid ) 
	: SheetItem( WiRow ),
	table( t ),
	rowid( rowid )
{
	for ( uint c = 0; c < cols; c++ )
	{
		value.push_back( TableEntry( table, c, rowid ));
	}
}

/*****************************************************
**
**   Row   ---   isEmpty
**
******************************************************/
bool Row::isEmpty()
{
	for ( uint i = 0; i < value.size(); i++ )
	{
		if ( ! value[i].text.isEmpty() || value[i].isHeader ) return false;
	}
	return true;
}

/*****************************************************
**
**   Row   ---   cloneClean
**
******************************************************/
SheetItem *Row::cloneClean()
{
	Row *row = new Row( table, rowid, value.size() );
	for( uint c = 0; c < value.size(); c++ )
	{
		row->value[c] = value[c];
	}
	return row;
}

/*****************************************************
**
**   Row   ---   addValue
**
******************************************************/
void Row::addValue( const TableEntry &e )
{
	value.push_back( e );
}

/*****************************************************
**
**   Table   ---   Constructor
**
******************************************************/
Table::Table( const uint &cols, const uint &rows )
	: SheetItem( WiTable ),
	header( WitHeader, MString( wxEmptyString )),
	nb_cols( cols ),
	errorcount( 0 )
{
	uint r, c;

	uniformLayout = false; 
	col_width.clear();

	for ( c = 0; c < cols; c++ )
	{
		col_alignment.push_back( Align::Left );
		col_empty.push_back( true );
		col_break.push_back( false );
		col_width.push_back( 0 );
	}

	for ( r = 0; r < rows; r++ )
	{
		contents.push_back( Row( this, cols, r ));
	}
}

/*****************************************************
**
**   Table   ---   cloneClean
**
******************************************************/
SheetItem *Table::cloneClean()
{
	Table *table = new Table( nb_cols, contents.size() );
	table->header.tf = header.tf;
	TableEntry *e;

	//printf( "Start Table::cloneClean nb_rows %ld nb_cols %u\n", contents.size(), nb_cols );
	for( uint r = 0; r < contents.size(); r++ )
	{
		for( uint c = 0; c < nb_cols; c++ )
		{
			e = getEntry( c, r );
			table->setEntry( c, r, e->text, e->isHeader );
		}
  }

	for ( uint c = 0; c < nb_cols; c++ )
	{
		table->col_alignment[c] = col_alignment[c];
		table->col_empty[c] = col_empty[c];
		table->col_break[c] = col_break[c];
	}
	return table;
}

/*****************************************************
**
**   Table   ---   getSubitem4Point
**
******************************************************/
SheetItem *Table::getSubitem4Point( const wxPoint &p )
{
	//printf( "Table::getSubitem4Point %d %d\n", p.x, p.y );
	if ( ! header.rect.isNil() && pointInRect( p, header.rect )) return &header;

	//static int ccc = 0;

	for( uint r = 0; r < contents.size(); r++ )
	{
		//printf( "Table::getSubitem4Point %f %f %f %f\n", contents[r].rect.x, contents[r].rect.y, contents[r].rect.width, contents[r].rect.height );
		if ( pointInRect( p, contents[r].rect ))
		{
			for( uint c = 0; c < contents[r].value.size(); c++ )
			{
				TableEntry &e = contents[r].value[c];
				//wxRect rr = e.rect.toWxRect();
				//printf( "Col %d rect x %d y %d w %d h %d\n", (int)c, rr.x, rr.y, rr.width, rr.height );
				if ( pointInRect( p, e.rect ))
				{
					//printf( "FOUND %d row %d col %d RECT %f %f %f %f\n", ccc++, (int)r, (int)c, e.rect.x, e.rect.y, e.rect.width, e.rect.height );
					return &e;
				}
			}
			//return &contents[r];
		}
	}
	return (SheetItem*)NULL;
}

/*****************************************************
**
**   Table   ---   resetRectangles
**
******************************************************/
void Table::resetRectangles()
{
  //printf( "Table::resetRectangles( NOW \n" );
  rect = MRect();
  header.rect = MRect();
  for( uint r = 0; r < contents.size(); r++ )
  {
    contents[r].rect = MRect();
  }
}

/*****************************************************
**
**   Table   ---   setCenterAll
**
******************************************************/
void Table::setCenterAll()
{
	for ( uint col = 0; col < getNbCols(); col++ )
	{
		col_alignment[col] = Align::Center;
	}
}

/*****************************************************
**
**   Table   ---   calculateSubitemRectangles
**
******************************************************/
void Table::calculateSubitemRectangles()
{
	header.rect.x = rect.x;
	header.rect.width = rect.width;
	header.rect.y = rect.y;

	double y = rect.y + header.rect.height;
	for( uint r = 0; r < contents.size(); r++ )
	{
		contents[r].rect.x = rect.x;
		contents[r].rect.width = rect.width;
		contents[r].rect.y = y;

		double x = rect.x;
		for ( uint col = 0; col < getNbCols(); col++ )
		{
			TableEntry &e = contents[r].value[col];

			e.rect.x = x;
			x += col_width[col];
			e.rect.width = col_width[col];

			e.rect.y = y;
			e.rect.height = contents[r].rect.height;

			e.colid = col;
			e.rowid = r;
		}

		y += contents[r].rect.height;
	}
}

/*****************************************************
**
**   Table   ---   setHeader
**
******************************************************/
void Table::setHeader( const MString &s )
{
	header.tf = s;
}

/*****************************************************
**
**   Table   ---   setHeader
**
******************************************************/
void Table::setHeader( const uint &col, const wxString &s1 )
{
	contents[0].value[col].text = MString( s1 );
	contents[0].value[col].isHeader = true;
}

/*****************************************************
**
**   Table   ---   setHeader
**
******************************************************/
void Table::setHeader( const uint &col, const MString &f )
{
	assert( contents.size() > 0 );
	assert( col < nb_cols );
	contents[0].value[col].text = f;
	contents[0].value[col].isHeader = true;
}

/*****************************************************
**
**   Table   ---   setHeaderEntry
**
******************************************************/
void Table::setHeaderEntry( const uint &col, const uint &row, const wxString &s1 )
{
	assert( row < contents.size() );
	assert( col < nb_cols );

	setEntry( col, row, s1, true );
}

/*****************************************************
**
**   Table   ---   setHeaderEntry
**
******************************************************/
void Table::setHeaderEntry( const uint&col, const uint &row, const MString &f )
{
	assert( row < contents.size() );
	assert( col < nb_cols );

	setEntry( col, row, f, true );
}

/*****************************************************
**
**   Table   ---   setEntry
**
******************************************************/
void Table::setEntry( const uint &col, const uint &row, const MString &f, const bool isHeader )
{
	assert( row < contents.size() );
	assert( col < nb_cols );

	if ( col_empty[col] &&  ! f.isEmpty() ) col_empty[col] = false;
	contents[row].value[col] = TableEntry( this, col, row, f, isHeader );
}

/*****************************************************
**
**   Table   ---   setEntry
**
******************************************************/
void Table::setEntry( const uint &col, const uint &row, const wxString &s1, const bool isHeader )
{
	assert( row < contents.size() );
	assert( col < nb_cols );

	if ( col_empty[col] &&  ! s1.IsEmpty() ) col_empty[col] = false;
	contents[row].value[col] = TableEntry( this, col, row, s1, isHeader );
}

/*****************************************************
**
**   Table   ---   getEntry
**
******************************************************/
TableEntry *Table::getEntry( const uint &col, const uint &row )
{
	assert( row < contents.size() );
	assert( col < nb_cols );
	return &contents[row].value[col];
}

/*****************************************************
**
**   Table   ---   addRow
**
******************************************************/
void Table::addRow( Row r )
{
	assert( r.value.size() == nb_cols );
	r.rowid = contents.size();
	contents.push_back( r );

	for( uint c = 0; c < nb_cols; c++ )
	{
		if ( col_empty[c] &&  ( ! r.value[c].text.isEmpty())) col_empty[c] = false;
	}
}


