/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWriter.h
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

#ifndef _SHEETWRITER_H_
#define _SHEETWRITER_H_

#include "FontProvider.h"
#include "Sheet.h"

#include <wx/colour.h>

class ColorConfig;
class Painter;
class SheetConfig;
class TableStyle;
class WriterConfig;

/*************************************************//**
*
*  
*
******************************************************/
class GenericSheetWriter : public wxObject
{
	DECLARE_CLASS( GenericSheetWriter )
	
public:

	GenericSheetWriter( Sheet*, SheetConfig*, WriterConfig*, ColorConfig* );
	virtual ~GenericSheetWriter();

	void preformat( Painter *painter, const double &xrightmax );
	void drawSheet( Painter *painter, const MRect&, const bool eraseBackground = true );

	void setWriterConfig( WriterConfig *wcfg ) { writercfg = wcfg; }
	void setSheetConfig( SheetConfig *scfg ) { sheetcfg = scfg; }

	void setCenterAll( const bool b = true ) { doCenterAll = b; }

	// TODO kann man die protected machen mit friend
	MRect contentRect;
	double xSizeContents;
	double ycursor;

	MPoint pageSize;

protected:

	virtual FONT_ID getFontIdForItem( const SHEET_TEXT_ITEM_SUBTYPE& ) = 0;

	void preformatItem( Painter*, SheetItem* );
	void preformatTable( Painter*, Table* );
	void preformatWidgetGrid( Painter*, SheetWidgetGrid* );
	void preformatColset( Painter*, SheetColumnSet* );
	void preformatRowset( Painter*, SheetRowSet* );

	void drawItem( Painter*, SheetItem*, const MRect &refreshRect );
	void drawTable( Painter*, Table*, const MRect &refreshRect );

	void formatMString( Painter *painter, MString&, const double xmax = 0 );

	virtual void handlePageBreak( SheetItem* ) {}

	double table_cell_delta_x;
	double table_cell_delta_y;
	double table_widget_extra_y;
	double table_cell_align_delta;
	double table_empty_col_width;

	Sheet *sheet;
	SheetConfig *sheetcfg;
	WriterConfig *writercfg;
	ColorConfig *colorcfg;

	double xrightmax;
	bool doUniformScaling;

	bool doCenterAll;

	// corners of retangles (tables)
	int cornerRadius;


};

/*************************************************//**
*
*  
*
******************************************************/
class DcSheetWriter : public GenericSheetWriter
{
	DECLARE_CLASS( DcSheetWriter )
	
public:
	DcSheetWriter( Sheet*, SheetConfig*, WriterConfig*, ColorConfig* );

protected:

	virtual FONT_ID getFontIdForItem( const SHEET_TEXT_ITEM_SUBTYPE& );

};


#endif


