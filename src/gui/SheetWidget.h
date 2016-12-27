/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWidget.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/


#ifndef _SHEETWIDGET_H_
#define _SHEETWIDGET_H_

#include "BasicSheetWidget.h"
#include "FontProvider.h"
#include "SheetWriter.h"

class ChartProperties;
class ColorConfig;
class Table;
class GuiWriter;
class Sheet;
class SheetConfig;
class WriterConfig;

/*************************************************//**
*
*  basic widget for sheet display, with a scrollable page
*
******************************************************/
class SheetWidget : public BasicSheetWidget
{
	DECLARE_CLASS( SheetWidget )

public:

	SheetWidget( wxWindow *parent, ChartProperties*, SheetConfig* = (SheetConfig*)NULL,
		WriterConfig* = (WriterConfig*)NULL, ColorConfig* = (ColorConfig*)NULL );
	~SheetWidget();

	virtual void OnDataChanged();

	GenericSheetWriter *getWriter() const { return writer; }

	void setCenterAll( const bool b = true ) { writer->setCenterAll( b ); }
	void setMinXRight( const int &xr ) { minxright = xr; }

protected:

	void paintPage( bool eraseBackground, MRect* refreshRect );
	void calculateContentSize();
	void OnSize( wxSizeEvent& );
	virtual void mouseHasMoved( const bool& );
	virtual void onNavigationKeyCommand( wxKeyEvent& );

	void init();
	void initViewPort();
	//void updateSheetConfig();

	// implementation of abstract method from BasicWidget
	virtual void doPaint( const wxRect&, const bool eraseBackground = true );

	virtual void HandleMouseWheelEvent( wxMouseEvent& );

	void OnChildCommand( wxCommandEvent& );

	GenericSheetWriter *writer;

	// total view size
	int xviewport, yviewport;

	// coordinates of the sheet
	//MRect sheetRect;

	SheetItem *lastMarkedItem;
	bool dirty;
	int minxright;
};

#endif

