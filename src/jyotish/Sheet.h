/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Sheet.h
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

#ifndef _SHEET_H_
#define _SHEET_H_

#include <wx/object.h>
#include <wx/string.h>
#include <list>

#include "maitreya.h"

using namespace std;

class ChartProperties;
class Painter;
class Table;
class Sheet;
class SheetConfig;
class SheetItem;
class WriterConfig;

enum SHEET_ITEM_TYPE { WiTable, WiWidget, WiText, WiBar, WiGrid, WiRow, WiCell };
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

	list<MToken> tokens;
	list<MString> formattedLines;
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

	MString getObjectName( const ObjectId &num, const TEXT_FORMAT format = TF_MEDIUM, const bool vedic = false );
	wxString getObjectNamePlain( const ObjectId &id, const TEXT_FORMAT format = TF_MEDIUM, const bool vedic = false );

	MString getSignName( const Rasi&, const TEXT_FORMAT format = TF_MEDIUM );

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
		resetMarkup();
	}

	virtual ~SheetItem() {}

	virtual SheetItem *cloneClean() = 0;

	virtual void resetMarkup() { markedItem = (SheetItem*)NULL;; }

	virtual void markItem( SheetItem *item, const bool b = true ) { markedItem = b ? item : (SheetItem*)NULL; }

	virtual SheetItem *getSubitem4Point( const wxPoint& );

	virtual bool hasMarkup() const { return markedItem != (SheetItem*)NULL; }

	virtual MRect getRefreshRect( const bool /*ismarked*/ = false, const int /*selectionMode*/ = 0 ) { return rect; }

	SheetItem *markedItem;

	SHEET_ITEM_TYPE type;
	int align;

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
/*
class SheetTableItem : public SheetItem
{
	DECLARE_CLASS( SheetTableItem )

public:
	SheetTableItem( Table *t, const int align = Align::Center );

	virtual ~SheetTableItem();

	virtual SheetItem *cloneClean();

	virtual SheetItem *getSubitem4Point( const wxPoint& );

	Table *table;
	vector<SheetItem*> rowitems;
	SheetTextItem *headerItem;
};
*/

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
	 : SheetItem( WiWidget, align ), props( props )
	{
		ratio = .7;
		xmin = xmax = yfixed = 0;
		minDecreaseRatio = .8;
	}

	virtual void preformat() {}

	virtual void doPaint( Painter *painter, const MRect &refreshRect ) = 0;

	/*
	*  Size hints
	*  ----------
	*  ratio: y = ratio * x
	*  xmin = 0 will be maximum width
	*  xbreak will cause column break at that width, zero means no break
	*/
	double ratio, xmin, xmax, yfixed;
	
	// item size can be decrease if page break required (pdf only)
	double minDecreaseRatio;


protected:

	ChartProperties *props;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetGridItem : public SheetItem
{
	DECLARE_CLASS( SheetGridItem )

public:

	//virtual bool handleMouseMoveEvent( MouseEventContext& );
	SheetGridItem( const uint &cols, const uint &rows, const int align = Align::Center );
	~SheetGridItem();

	virtual SheetItem *cloneClean();

	virtual void resetMarkup() { /* TODO */ }

	//virtual void doPaint( Painter *painter, const MRect &refreshRect ) = 0;

	void addItem( SheetItem* );

protected:

	const uint cols;
	const uint rows;
	list<SheetItem*> items;

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
	void resetMarkup();

	void addHeader( const wxString&, const int align = Align::Left + Align::VCenter );
	void addParagraph( const wxString&, const int align = Align::Left + Align::VCenter );
	void addLine( const wxString&, const int align = Align::Left + Align::VCenter );

	void addHeader( const MString&, const int align = Align::Left + Align::VCenter );
	void addParagraph( const MString&, const int align = Align::Left + Align::VCenter );
	void addLine( const MString&, const int align = Align::Left + Align::VCenter );

	list<SheetItem*> items;

	WriterConfig *writercfg;
};


#endif


