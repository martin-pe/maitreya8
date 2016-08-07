/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWidget.h
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


#ifndef _SHEETWIDGET_H_
#define _SHEETWIDGET_H_

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
class SheetWidget : public BasicWidget
{
	DECLARE_CLASS( SheetWidget )

public:

	SheetWidget( wxWindow *parent, ChartProperties*, SheetConfig* = (SheetConfig*)NULL,
		WriterConfig* = (WriterConfig*)NULL, ColorConfig* = (ColorConfig*)NULL );
	~SheetWidget();

	virtual void OnDataChanged();
	virtual void exportAs( const WidgetExportType& );

	virtual int getWidgetOptions();

	GenericSheetWriter *getWriter() const { return writer; }

	Sheet *getSheet() const { return sheet; }

	void clearSheet();

	void enableFloatingLayout( const bool = true );

	void setWriterConfig( WriterConfig* );
	void setSheetConfig( SheetConfig* );
	SheetConfig *getSheetConfig() const { return sheetcfg; }

	void setCenterAll( const bool b = true ) { writer->setCenterAll( b ); }
	void setMinXRight( const int &xr ) { minxright = xr; }

protected:

	WriterConfig *writercfg;
	SheetConfig *sheetcfg;
	ColorConfig *colorcfg;
	Sheet *sheet;
	bool sheetConfigOverride;

	void paintPage( bool eraseBackground, MRect* refreshRect );
	void calculateContentSize();
	void OnSize( wxSizeEvent& );
	virtual void mouseHasMoved( const bool& );
	virtual void onNavigationKeyCommand( wxKeyEvent& );

	void init();
	void initViewPort();
	void updateSheetConfig();

	// implementation from abstract method in BAsicWidget
	virtual void doPaint( const wxRect&, const bool eraseBackground = true );
	void doPdfExport();
	void doTextExport( const WidgetExportType& );

	virtual void HandleMouseWheelEvent( wxMouseEvent& );

	void OnChildCommand( wxCommandEvent& );

	GenericSheetWriter *writer;
	//Writer *

	// total view size
	int xviewport, yviewport;

	// coordinates of the sheet
	//MRect sheetRect;

	SheetItem *lastMarkedItem;
	bool dirty;
	int minxright;
};

#endif

