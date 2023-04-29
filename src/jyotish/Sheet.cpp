/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Sheet.cpp
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

#include "Sheet.h"

#include "ArabicParts.h"
#include "Aspect.h"
#include "astrobase.h"
#include "Exporter.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "SheetConfig.h"
#include "SymbolProvider.h"
#include "Table.h"

#include <wx/log.h>
#include <wx/string.h>

extern Config *config;

IMPLEMENT_CLASS( SheetItem, wxObject )
IMPLEMENT_CLASS( SheetTextItem, SheetItem )
IMPLEMENT_CLASS( SheetWidgetItem, SheetItem )
IMPLEMENT_CLASS( SheetItemContainer, SheetItem )
IMPLEMENT_CLASS( SheetColumnSet, SheetItemContainer )
IMPLEMENT_CLASS( SheetRowSet, SheetItemContainer )
IMPLEMENT_CLASS( SheetWidgetGrid, SheetItemContainer )

/*****************************************************
**
**   SheetItem   ---   moveTo
**
******************************************************/
void SheetItem::moveTo( const double &x, const double &y )
{
	//printf( "SheetItem::moveTo type %d RECT x %f y %f MOVE TO x %f y %f\n", type, rect.x, rect.y, x, y );
	rect.x = x;
	rect.y = y;
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
**   SheetItemContainer   ---   Constructor
**
******************************************************/
SheetItemContainer::SheetItemContainer( const SHEET_ITEM_TYPE &type )
	: SheetItem( type )
{
	sheet = new Sheet;
}

/*****************************************************
**
**   SheetItemContainer   ---   Destructor
**
******************************************************/
SheetItemContainer::~SheetItemContainer()
{
	delete sheet;
}

/*****************************************************
**
**   SheetItemContainer   ---   addItem
**
******************************************************/
void SheetItemContainer::addItem( SheetItem *item )
{
	assert( item );
	sheet->addItem( item );
}

/*****************************************************
**
**   SheetItemContainer   ---   getSize
**
******************************************************/
uint SheetItemContainer::getSize() const
{
	assert( sheet );
	return sheet->items.size();
}

/*****************************************************
**
**   SheetItemContainer   ---   moveTo
**
******************************************************/
void SheetItemContainer::moveTo( const double &x, const double &y )
{
	const double x0 = rect.x - x;
	const double y0 = rect.y - y;
	//printf( "SheetItemContainer::moveTo p %f %f CURRENT %f %f DIFF %f %f\n", x, y, rect.x, rect.y, x0, y0 );
	rect.x = x;
	rect.y = y;
	for( std::list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
	{
		(*iter)->moveTo( (*iter)->rect.x - x0, (*iter)->rect.y - y0 );
		//printf( "   SheetItemContainer::moveTo ITEM type %d result %f %f\n", (*iter)->type, (*iter)->rect.x, (*iter)->rect.y );
	}
}

/*****************************************************
**
**   SheetWidgetGrid   ---   cloneClean
**
******************************************************/
SheetItem *SheetWidgetGrid::cloneClean()
{
	SheetWidgetGrid *grid = new SheetWidgetGrid( nb_cols, x2yratio, wmin, wmax, hmin, hmax );
	grid->sheet = sheet->cloneClean();
	return grid;
}

/*****************************************************
**
**   SheetColumnSet   ---   cloneClean
**
******************************************************/
SheetItem *SheetColumnSet::cloneClean()
{
	SheetColumnSet *colset = new SheetColumnSet( *this );
	colset->sheet = sheet->cloneClean();
	return colset;
}

/*****************************************************
**
**   SheetRowSet   ---   cloneClean
**
******************************************************/
SheetItem *SheetRowSet::cloneClean()
{
	SheetRowSet *rowset = new SheetRowSet( *this );
	rowset->sheet = sheet->cloneClean();
	return rowset;
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
	for( std::list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
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
	for( std::list<SheetItem*>::iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		SheetItem *item = *iter;
		assert( item );
		delete item;
	}
	items.clear();
}

/*****************************************************
**
**   Sheet   ---   centerItems
**
******************************************************/
void Sheet::centerItems()
{
	double xrightmax = 0;
	std::list<SheetItem*>::iterator iter;

	for( iter = items.begin(); iter != items.end(); iter++ )
	{
		//printf( "Sheet::centerItems xrightmax %f iter width %f size %ld\n", xrightmax, (*iter)->rect.width, items.size() );
		xrightmax = Max( xrightmax, (*iter)->rect.width );
	}
	for( iter = items.begin(); iter != items.end(); iter++ )
	{
		(*iter)->rect.x += .5 * xrightmax - .5 * (*iter)->rect.width;
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
	for( std::list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
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
**   SheetFormatter   ---   getObjectNameWithContext
**
******************************************************/
MString SheetFormatter::getObjectNameWithContext( const ObjectId &id, const PlanetContext &ctx, const TEXT_FORMAT format, const bool vedic ) const
{
	ASSERT_VALID_PLANET_CONTEXT( ctx );
	MString t;
	Lang lang;
	t.add( MToken( TTSE_PLANET, (int)id, format, vedic ));
	if ( ctx != PcNone ) t.add( MToken( lang.getPlanetContextSubscriptum( ctx ), TTFF_SUBSCRPTUM ));
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
	SymbolProvider sp( writercfg );

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
	for( std::list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
	{
		s << token2PlainText( *iter );
	}
	return s;
}



