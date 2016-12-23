/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicSheetWidget.h
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


#ifndef _BASICSHEETWIDGET_H_
#define _BASICSHEETWIDGET_H_

#include "BasicWidget.h"
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
class BasicSheetWidget : public BasicWidget
{
	DECLARE_CLASS( BasicSheetWidget )

public:

	BasicSheetWidget( wxWindow *parent, ChartProperties*, SheetConfig* = (SheetConfig*)NULL,
		WriterConfig* = (WriterConfig*)NULL, ColorConfig* = (ColorConfig*)NULL );
	~BasicSheetWidget();

	virtual void exportAs( const WidgetExportType& );

	Sheet *getSheet() const { return sheet; }

	void clearSheet();

	void setWriterConfig( WriterConfig* );
	void setSheetConfig( SheetConfig* );
	SheetConfig *getSheetConfig() const { return sheetcfg; }

protected:

	WriterConfig *writercfg;
	SheetConfig *sheetcfg;
	ColorConfig *colorcfg;
	Sheet *sheet;

	// TODO REMOVE
	bool sheetConfigOverride;

	void doPdfExport();
	void doTextExport( const WidgetExportType& );
};

#endif

