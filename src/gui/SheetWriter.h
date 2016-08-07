/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWriter.h
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

	void enableFloatingLayout( const bool b = true ) { doFloatingLayout = b; }

	void setWriterConfig( WriterConfig *wcfg ) { writercfg = wcfg; }
	void setSheetConfig( SheetConfig *scfg ) { sheetcfg = scfg; }

	void setCenterAll( const bool b = true ) { doCenterAll = b; }

	// TODO kann man die protected machen mit friend
	MRect contentRect;
	double xSizeContents;
	double ycursor;

protected:

	virtual FONT_ID getFontIdForItem( const SHEET_TEXT_ITEM_SUBTYPE& ) = 0;

	void preformatTable( Painter*, Table* );
	void drawTable( Painter*, Table*, const MRect &refreshRect );
	void drawTableFrame( Painter*, TableStyle*, MRect &r );
	void drawTableShadow( Painter*, TableStyle*, MRect &r );

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

	/*
	* contents will be arranged in multiple columns if possible.
	* is enabled by default. 
	*/
	bool doFloatingLayout;

	bool doCenterAll;

	// number of colums in floating layout
	int numcols;

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


