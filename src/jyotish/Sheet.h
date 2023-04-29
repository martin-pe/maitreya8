/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Sheet.h
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

#ifndef _SHEET_H_
#define _SHEET_H_

#include <wx/object.h>
#include <wx/string.h>
#include <list>

#include "maitreya.h"

class ChartProperties;
class Painter;
class Table;
class Sheet;
class SheetConfig;
class SheetItem;
class WriterConfig;

enum SHEET_ITEM_TYPE { WiTable = 0, WiWidget, WiText, WiBar, WiColumnSet, WiRowSet, WiWidgetGrid, WiRow, WiCell };
enum SHEET_TEXT_ITEM_SUBTYPE { WitNone = -1, WitHeader = 0, WitParagraph, WitLine, WitTableCell, WitTableHeader };

// add bold, italic etc if required
enum TT_FONT_FORMAT
{
	TTFF_NORMAL,
	TTFF_SUBSCRPTUM,
	TTFF_SUPERSCRPTUM
};

// inkl. int entityId
enum TT_SPECIAL_ENTITY
{
	TTSE_NONE,
	TTSE_PLANET,
	TTSE_SIGN,
	TTSE_ASPECT,
	TTSE_DIRECTION
};

/*************************************************//**
*
* 
*
******************************************************/
class MToken
{

public:

	MToken( const wxString s = wxEmptyString, const TT_FONT_FORMAT format = TTFF_NORMAL )
	{
		init();
		text = s;
		fontFormat = format;
	}

	MToken( const TT_SPECIAL_ENTITY &e, const int &id, const TEXT_FORMAT &tf, const bool v = false, const TT_FONT_FORMAT ff = TTFF_NORMAL )
	{
		init();
		entity = e;
		entityId = id;
		textFormat = tf;
		vedic = v;
		fontFormat = ff;
	}

	wxString text;

	TT_FONT_FORMAT fontFormat;

	TT_SPECIAL_ENTITY entity;
	int entityId;
	bool vedic;
	TEXT_FORMAT textFormat;

private:

	void init()
	{
		fontFormat = TTFF_NORMAL;
		entity = TTSE_NONE;
		entityId = -1;
		textFormat  = TF_LONG;
		vedic = false;
	}

};

/*************************************************//**
*
* 
*
******************************************************/
class MString
{
public:

	MString( wxString = wxEmptyString );
	MString( const MToken& );

	void add( const MToken& );
	void add( wxString s ) { add( MToken( s )); }
	void add( const MString& );

	// TODO auch auf ein leeres Token prüfen (String = leer oder kein Symbol)
	bool isEmpty() const { return tokens.size() == 0; }
	void clear();

	std::list<MToken> tokens;
	std::list<MString> formattedLines;
	MPoint size;

	void operator=( const wxString );
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetFormatter
{
public:
	SheetFormatter( WriterConfig* = (WriterConfig*)NULL );

	MString getPosFormatted( const double &len,
		const MOVING_DIRECTION dir = MD_DIRECT,
		const int precision = DEG_PRECISION_SECOND,
		const TEXT_FORMAT format = TF_LONG
	);

	MString getObjectName( const ObjectId&, const TEXT_FORMAT = TF_MEDIUM, const bool vedic = false );
	wxString getObjectNamePlain( const ObjectId&, const TEXT_FORMAT = TF_MEDIUM, const bool vedic = false );

	MString getSignName( const Rasi&, const TEXT_FORMAT = TF_MEDIUM );

	// with subscriptum for context
	MString getObjectNameWithContext( const ObjectId&, const PlanetContext&, const TEXT_FORMAT = TF_MEDIUM, const bool vedic = false ) const;

	wxString token2PlainText( const MToken& );

	wxString token2Html( const MToken& );

	wxString fragment2PlainText( const MString& );

protected:
	wxString escapeHtmlSymbol( const wxString &s );
	WriterConfig *writercfg;
};

class MouseEventContext
{
public:
	MouseEventContext( const bool outside = false )
		: outside( outside )
	{
		style = (SheetConfig*)NULL;
	}

	MouseEventContext( const wxPoint p, SheetConfig *style = (SheetConfig*)NULL  )
		: mousePosition( p ),
		style( style )
	{
		outside = false;
	}

	wxPoint mousePosition;
	bool outside;
	wxRect refreshRect;
	SheetConfig *style;
};

/*************************************************//**
*
*  
*
******************************************************/
class SheetItem : public wxObject
{
	DECLARE_CLASS( SheetItem )

public:
	SheetItem( const SHEET_ITEM_TYPE &type, const int align = Align::Center )
	 : type( type ),
	 align( align )
	{
		shrinkrate = 0;
	}

	virtual ~SheetItem() {}

	virtual SheetItem *cloneClean() = 0;

	virtual void moveTo( const double &x, const double &y );

	SHEET_ITEM_TYPE type;
	int align;
	int shrinkrate;

	MRect rect;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetTextItem : public SheetItem
{
	DECLARE_CLASS( SheetTextItem )

public:
	SheetTextItem( const SHEET_TEXT_ITEM_SUBTYPE &st, wxString t, const int align = Align::Center )
	 : SheetItem( WiText, align ), subtype( st )
	{
		tf = MString( t );
		linecount = 0;
	}

	SheetTextItem( const SHEET_TEXT_ITEM_SUBTYPE &st, const MString &f, const int align = Align::Center )
	 : SheetItem( WiText, align ), tf( f ), subtype( st )
	{
		tf = f;
		linecount = 0;
	}

	virtual SheetItem *cloneClean();

	bool isEmpty() const { return tf.isEmpty(); }

	MString tf;
	const SHEET_TEXT_ITEM_SUBTYPE subtype;

	wxString formattedText;
	int linecount;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetWidgetItem : public SheetItem
{
	DECLARE_CLASS( SheetWidgetItem )

public:

	SheetWidgetItem( ChartProperties *props, const int align = Align::Center )
	 : SheetItem( WiWidget, align ), props( props ) {}

	virtual void doPaint( Painter *painter, const MRect &refreshRect ) = 0;

protected:

	ChartProperties *props;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetItemContainer : public SheetItem
{
	DECLARE_CLASS( SheetItemContainer )

public:

	SheetItemContainer( const SHEET_ITEM_TYPE &type );
	~SheetItemContainer();

	void addItem( SheetItem* );
	uint getSize() const;

	virtual void moveTo( const double &x, const double &y );

	Sheet *sheet;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetWidgetGrid : public SheetItemContainer
{
	DECLARE_CLASS( SheetWidgetGrid )

public:

	SheetWidgetGrid( const uint &ncols, const double x2yratio = 0,
			const double wmin = 0, const double hmin = 0,
			const double wmax = 0, const double hmax = 0 )
		: SheetItemContainer( WiWidgetGrid ),
			nb_cols( ncols ),
			x2yratio( x2yratio ),
			wmin( wmin ),
			hmin( hmin ),
			wmax( wmax ),
			hmax( hmax )
		{
		}

	virtual SheetItem *cloneClean();

	const uint nb_cols;
	double x2yratio;
	const double wmin, hmin, wmax, hmax;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetColumnSet : public SheetItemContainer
{
	DECLARE_CLASS( SheetColumnSet )

public:

	SheetColumnSet() : SheetItemContainer( WiColumnSet ) {}
	virtual SheetItem *cloneClean();
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetRowSet : public SheetItemContainer
{
	DECLARE_CLASS( SheetRowSet )

public:

	SheetRowSet() : SheetItemContainer( WiRowSet ) {}
	virtual SheetItem *cloneClean();
};

/*************************************************//**
*
* 
*
******************************************************/
class Sheet
{
public:

	Sheet( WriterConfig* = (WriterConfig*)NULL );
	~Sheet();

	Sheet *cloneClean();

	bool isEmpty() { return items.size() == 0; }

	void clear();
	void addItem( SheetItem* );
	void centerItems();

	void addHeader( const wxString&, const int align = Align::Left + Align::VCenter );
	void addParagraph( const wxString&, const int align = Align::Left + Align::VCenter );
	void addLine( const wxString&, const int align = Align::Left + Align::VCenter );

	void addHeader( const MString&, const int align = Align::Left + Align::VCenter );
	void addParagraph( const MString&, const int align = Align::Left + Align::VCenter );
	void addLine( const MString&, const int align = Align::Left + Align::VCenter );

	std::list<SheetItem*> items;

	WriterConfig *writercfg;
};


#endif


