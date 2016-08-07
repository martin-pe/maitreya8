/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Sheet.cpp
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

#include "Sheet.h"

#include "ArabicParts.h"
#include "Aspect.h"
#include "astrobase.h"
#include "Exporter.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"
#include "SheetConfig.h"
#include "SymbolProvider.h"
#include "Table.h"

#include <wx/log.h>
#include <wx/string.h>

extern Config *config;

IMPLEMENT_CLASS( SheetItem, wxObject )
IMPLEMENT_CLASS( SheetTextItem, SheetItem )
IMPLEMENT_CLASS( SheetWidgetItem, SheetItem )
IMPLEMENT_CLASS( SheetGridItem, SheetItem )

/*****************************************************
**
**   SheetItem   ---   getSubitem4Point
**
******************************************************/
SheetItem *SheetItem::getSubitem4Point( const wxPoint &p )
{
	return pointInRect( p, rect ) ? (SheetItem*)this : (SheetItem*)NULL;
}

/*****************************************************
**
**   SheetTextItem   ---   cloneClean
**
******************************************************/
SheetItem *SheetTextItem::cloneClean()
{
	SheetTextItem *item = new SheetTextItem( subtype, tf, align );
	return item;
}

/*****************************************************
**
**   SheetTableItem   ---   Constructor
**
******************************************************/
/*
SheetTableItem::SheetTableItem( Table *t, const int align )
	: SheetItem( WiTable, align ),
	table( t )
{
	headerItem = (SheetTextItem*)NULL;
}

SheetTableItem::~SheetTableItem()
{
	if ( table ) delete table;
	if ( headerItem ) delete headerItem;
	//for( vector<SheetItem*>::iterator iter = rowitems.begin(); rowitems.end(); iter++ ) delete *iter;
	for( uint i = 0; i < rowitems.size(); i++ ) delete rowitems[i];
	rowitems.clear();
}

SheetItem *SheetTableItem::cloneClean()
{
	// TODO deep copy of table
	SheetTableItem *item = new SheetTableItem( table, align );
	item->table = table->cloneClean();
	return item;
}

MRect SheetTableItem::getHeaderRect() const
{
	MRect rect;
	return rect;
}
uint SheetTableItem::getItem4Point( const wxPoint &p ) const
{
	if ( ! pointInRect( p, rect ))
	{
		return NO_ITEM;
	}

	assert( table );

	double y = rect.y;
	if ( p.y >= y && p.y <= y + table->headerHeight )
	{
		//printf( "YES HEADER\n" );
		return HEADER_ROW;
	}

	y += table->headerHeight;
	for( uint r = 0; r < table->getNbRows(); r++ )
	{
		if ( p.y >= y && p.y <= y + table->row_height[r] )
		{
			//printf( "YES ROW %d\n", r );
			return r;
		}
		y += table->row_height[r];
	}
	//printf( "NO ROW\n" );
	return NO_ITEM;
}

MRect SheetTableItem::getRowRect( const unsigned &row ) const
{
	assert( table );
	MRect rowrect = rect;

	if( row == HEADER_ROW )
	{
		rowrect.height = table->headerHeight;
	}
	else
	{
		assert( row < table->getNbRows());

		rowrect.y += table->headerHeight;
		for( uint r = 0; r < row; r++ )
		{
			//printf( "ROW %d\n", r );
			rowrect.y += table->row_height[r];
		}
		rowrect.height = table->row_height[row];
	}
	return rowrect;
}

*/

/*****************************************************
**
**   SheetGridItem   ---   Constructor
**
******************************************************/
SheetGridItem::SheetGridItem( const uint &cols, const uint &rows, const int align )
	: SheetItem( WiGrid, align ),
	cols( cols ),
	rows( rows )
{
}

/*****************************************************
**
**   SheetGridItem   ---   Destructor
**
******************************************************/
SheetGridItem::~SheetGridItem()
{
	for( list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		delete *iter;
	}
}

/*****************************************************
**
**   SheetGridItem   ---   cloneClean
**
******************************************************/
SheetItem *SheetGridItem::cloneClean()
{
	SheetGridItem *grid = new SheetGridItem( cols, rows, align );
		
	for( list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		grid->addItem( (*iter)->cloneClean());
	}

	return grid;
}

/*****************************************************
**
**   SheetGridItem   ---   addItem
**
******************************************************/
void SheetGridItem::addItem( SheetItem *item )
{
	items.push_back( item );
}

/*****************************************************
**
**   Sheet   ---   Constructor
**
******************************************************/
Sheet::Sheet( WriterConfig *wcfg )
{
	writercfg = wcfg ? wcfg: config->writer;
}

/*****************************************************
**
**   Sheet   ---   Destructor
**
******************************************************/
Sheet::~Sheet()
{
	clear();
}

/*****************************************************
**
**   Sheet   ---   cloneClean
**
******************************************************/
Sheet *Sheet::cloneClean()
{
	Sheet *sheet = new Sheet( writercfg );
	for( list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		sheet->addItem( (*iter)->cloneClean() );
	}
	return sheet;
}

/*****************************************************
**
**   Sheet   ---   clear
**
******************************************************/
void Sheet::clear()
{
	//printf( "SHEET CLEAR\n" );
	for( list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		SheetItem *item = *iter;
		assert( item );
		delete item;
	}
	items.clear();
}

/*****************************************************
**
**   Sheet   ---   resetMarkup
**
******************************************************/
void Sheet::resetMarkup()
{
	for( list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		(*iter)->resetMarkup();
	}
}

/*****************************************************
**
**   Sheet   ---   add* methods
**
******************************************************/
void Sheet::addItem( SheetItem *item ) { items.push_back( item ); }
void Sheet::addHeader( const wxString &s, const int align ) { addItem( new SheetTextItem( WitHeader, s, align )); }
void Sheet::addParagraph( const wxString &s, const int align ) { addItem( new SheetTextItem( WitParagraph, s, align )); }
void Sheet::addLine( const wxString &s, const int align ) { addItem( new SheetTextItem( WitLine, s, align )); }

void Sheet::addHeader( const MString &s, const int align ) { addItem( new SheetTextItem( WitHeader, s, align )); }
void Sheet::addParagraph( const MString &s, const int align ) { addItem( new SheetTextItem( WitParagraph, s, align )); }
void Sheet::addLine( const MString &s, const int align ) { addItem( new SheetTextItem( WitLine, s, align )); }

/*****************************************************
**
**   MString   ---   Constructor
**
******************************************************/
MString::MString( wxString s )
{
	if ( ! s.IsEmpty() ) tokens.push_back( MToken( s ));
	size = 0;
}

/*****************************************************
**
**   MString   ---   Constructor
**
******************************************************/
MString::MString( const MToken &token )
{
	add( token );
}

/*****************************************************
**
**   MString   ---   operator=
**
******************************************************/
void MString::operator=( const wxString s )
{
	add( MToken( s ));
}

/*****************************************************
**
**   MString   ---   clear()
**
******************************************************/
void MString::clear()
{
	tokens.clear();
	formattedLines.clear();
	size = MPoint( 0, 0 );
}

/*****************************************************
**
**   MString   ---   add
**
******************************************************/
void MString::add( const MToken &token )
{
	tokens.push_back( token );
}

/*****************************************************
**
**   MString   ---   add
**
******************************************************/
void MString::add( const MString &f )
{
	for( list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
	{
		tokens.push_back( *iter );
	}
}

// TODO maybe wrong in Ansi build
#define DEGREE_SYMBOL wxT( "\u00B0" );
#define MINUTE_SYMBOL wxT( "\u2032" )
#define SECOND_SYMBOL wxT( "\u2033" )

/*****************************************************
**
**   SheetFormatter   ---   Constructor
**
******************************************************/
SheetFormatter::SheetFormatter( WriterConfig *c )
{
	writercfg = c ? c : config->writer;
}

/*****************************************************
**
**   SheetFormatter   ---   getPosFormatted
**
******************************************************/
MString SheetFormatter::getPosFormatted( const double &len, const MOVING_DIRECTION dir, const int precision, const TEXT_FORMAT format )
{
	MString t;
	int deg, min;
	double sec;
	wxString s;

	if ( dir == MD_RETROGRADE )
	{
		t.add( MToken( TTSE_DIRECTION, dir, format ));
	}

	double mylen = red_deg( len );
	const Rasi sign = (Rasi)( mylen / 30 );
	mylen -= 30 * sign;

	getDegMinSecInts2( mylen, deg, min, sec );
	const wxString sdeg = wxString::Format( wxT( "%02d" ), deg );
	const wxString smin = wxString::Format( wxT( "%02d" ), min );
	const wxString ssec = wxString::Format( wxT( "%02d" ), (int)sec );

	if ( writercfg->vedicPositions )
	{
		s = wxString::Format( wxT( "%02d-%02d-%02d" ), sign, deg, min );
		if ( precision == DEG_PRECISION_SECOND )
		{
			s << wxT( "-" ) << ssec;
		}
		else if ( precision == DEG_PRECISION_MORE )
		{
			s << wxT( "-" ) << sec;
		}
		t.add( MToken( s ));
	}
	else
	{
		if ( precision == DEG_PRECISION_MINUTE )
		{
			t.add( MToken( sdeg ));
			t.add( MToken( TTSE_SIGN, sign, format ));
			t.add( MToken( smin ));
		}
		else
		{
			s << sdeg << DEGREE_SYMBOL;
			s << smin << MINUTE_SYMBOL;
			if ( precision == DEG_PRECISION_SECOND )
			{
				s << ssec << SECOND_SYMBOL;
			}
			else if ( precision == DEG_PRECISION_MORE )
			{
				s << sec << SECOND_SYMBOL;
			}
			s << SPACE;
			t.add( MToken( s ));
			t.add( MToken( TTSE_SIGN, sign, format ));
		}
	}

	return t;
}

/*****************************************************
**
**   SheetFormatter   ---   getObjectName
**
******************************************************/
MString SheetFormatter::getObjectName( const ObjectId &id, const TEXT_FORMAT format, const bool vedic )
{
	MString t;
	t.add( MToken( TTSE_PLANET, (int)id, format, vedic ));
	return t;
}

/*****************************************************
**
**   SheetFormatter   ---   getSignName
**
******************************************************/
MString SheetFormatter::getSignName( const Rasi &sign, const TEXT_FORMAT format )
{
	MString t;
	t.add( MToken( TTSE_SIGN, (int)sign, format ));
	return t;
}

/*****************************************************
**
**   SheetFormatter   ---   getObjectNamePlain
**
******************************************************/
wxString SheetFormatter::getObjectNamePlain( const ObjectId &id, const TEXT_FORMAT format, const bool vedic )
{
	return token2PlainText( MToken( TTSE_PLANET, (int)id, format, vedic ));
}

/*****************************************************
**
**   SheetFormatter   ---   token2PlainText
**
******************************************************/
wxString SheetFormatter::token2PlainText( const MToken &token )
{
	Lang lang( writercfg );
	wxString t;

	if ( token.entity != TTSE_NONE )
	{
		switch( token.entity )
		{
			case TTSE_PLANET:
				if ( token.entityId >= OFORTUNE ) t << ArabicPartsExpert::getObjectName( token.entityId, token.textFormat );
				else t << lang.getObjectName( (ObjectId)token.entityId, token.textFormat, token.vedic );
			break;
			case TTSE_SIGN:
				t << lang.getSignName( (Rasi)token.entityId, token.textFormat ); // TODO, writercfg->vedicSignNames );
			break;
			case TTSE_DIRECTION:
				switch( (MOVING_DIRECTION)token.entityId )
				{
					case MD_NONE:
						t << SPACE;
					break;
					case MD_DIRECT:
						t << wxT( "D" );
					break;
					case MD_RETROGRADE:
						t << wxT( "R" );
					break;
					case MD_STATIONARY:
						t << wxT( "S" );
					break;
				}
			break;
			case TTSE_ASPECT:
				return AspectExpert::getAspectName( token.entityId );
			break;
			default:
				assert( false );
			break;
		}
	}
	else
	{
		t << token.text;
	}
	//printf( "KUNO %s\n", str2char( t ));
	return t;
}

/*****************************************************
**
**   SheetFormatter   ---   token2Html
**
******************************************************/
wxString SheetFormatter::token2Html( const MToken &token )
{
	Lang lang;
	wxString t;
	SymbolProvider sp;

	if ( token.entity != TTSE_NONE )
	{
		switch( token.entity )
		{
			case TTSE_PLANET:
				if ( token.entityId >= OFORTUNE ) t << ArabicPartsExpert::getObjectName( token.entityId, token.textFormat );
				else
				{
					if ( writercfg->planetSymbols )
					{
						t << escapeHtmlSymbol( sp.getPlanetCode( (ObjectId)token.entityId ));
					}
					else
					{
						t << lang.getObjectName( (ObjectId)token.entityId, token.textFormat, token.vedic );
					}
				}
			break;
			case TTSE_SIGN:
				if ( writercfg->signSymbols )
				{
					t << escapeHtmlSymbol( sp.getSignCode( (Rasi)token.entityId ));
				}
				else
				{
					t << lang.getSignName( (Rasi)token.entityId, token.textFormat ); // TODO, writercfg->vedicSignNames );
				}
			break;
			case TTSE_DIRECTION:
				t << escapeHtmlSymbol( sp.getRetroCode( (MOVING_DIRECTION)token.entityId ));
			break;
			case TTSE_ASPECT:
			{
				wxChar symbolCode = sp.getAspectCode( (ASPECT_TYPE)token.entityId );
				if ( symbolCode != SYMBOL_CODE_ERROR )
				{
					t << escapeHtmlSymbol( symbolCode );
				}
				else
				{
					t << AspectExpert::getAspectName( token.entityId );
				}
			}
			break;
			default:
				assert( false );
			break;
		}
	}
	else
	{
		t << token.text;
	}
	//printf( "KUNO %s\n", str2char( t ));
	return t;
}

/*****************************************************
**
**   SheetFormatter   ---   escapeHtmlSymbol
**
******************************************************/
wxString SheetFormatter::escapeHtmlSymbol( const wxString &t )
{
	//WriterConfig *writercfg;
	wxString s;
	s << wxT( "<font face=\"" ) << SYMBOL_FONT_NAME << wxT( "\">" ) << t << wxT( "</font>" );
	return s;
}

/*****************************************************
**
**   SheetFormatter   ---   fragment2PlainText
**
******************************************************/
wxString SheetFormatter::fragment2PlainText( const MString &f )
{
	wxString s;
	for( list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
	{
		s << token2PlainText( *iter );
	}
	return s;
}



