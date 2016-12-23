/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWidget.cpp
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

#include "SheetWidget.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "Exporter.h"
#include "guibase.h"
#include "SheetWriter.h"
#include "Painter.h"
#include "PdfTools.h"
#include "SheetConfig.h"

#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/stopwatch.h>

extern Config *config;

IMPLEMENT_CLASS( SheetWidget, BasicSheetWidget )

#define SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_X 40
#define SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_Y 20
#define SCROLLABLE_PAGE_WIDGET_MIN_XRIGHT 800

/*****************************************************
**
**   SheetWidget   ---   Constructor
**
******************************************************/
SheetWidget::SheetWidget( wxWindow *parent, ChartProperties *props, SheetConfig *scfg, WriterConfig *wcfg, ColorConfig *ccfg )
: BasicSheetWidget( parent, props, scfg, wcfg, ccfg )
{
	writer = new DcSheetWriter( sheet, sheetcfg, writercfg, colorcfg );
	init();
	setWidgetOptions( WO_EXPORT_GRAPHIC );
	minxright = 300;

	Connect( wxEVT_SIZE, wxSizeEventHandler( SheetWidget::OnSize ));
	Connect( CMD_FIRST+1, CMD_GRAPHIC_STYLE+30, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SheetWidget::OnChildCommand ));
}

/*****************************************************
**
**   SheetWidget   ---   Destructor
**
******************************************************/
SheetWidget::~SheetWidget()
{
}

/*****************************************************
**
**   SheetWidget   ---   updateSheetConfig
**
******************************************************/
void SheetWidget::updateSheetConfig()
{
	SheetConfigLoader *loader = SheetConfigLoader::get();
	sheetcfg = loader->getConfig( config->view->sheetStyle );
	assert( sheetcfg );
	writer->setSheetConfig( sheetcfg );
}

/*****************************************************
**
**   SheetWidget   ---   init
**
******************************************************/
void SheetWidget::init()
{
	assert( sheet );
	//sheet->clear();

	wxSize widgetsize = GetVirtualSize();
	wxSize clientsize = GetSize();

	writer->pageSize = MPoint( clientsize.x, clientsize.y );

	printf( "CLIENT %d %d\n", clientsize.x, clientsize.y );

	xviewport = widgetsize.x;// - sb_ysize;
	yviewport = widgetsize.y;// - sb_xsize;

	assert( sheetcfg );

	// TODO das kann man sicherlich an den Writer delegieren
	writer->contentRect.x = SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_X;
	writer->contentRect.y = SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_Y;
	writer->contentRect.width = 0;
	writer->contentRect.height = 0;

	dirty = true;
	lastMarkedItem = (SheetItem*)NULL;
}

/*****************************************************
**
**   SheetWidget   ---   initViewPort
**
******************************************************/
void SheetWidget::initViewPort()
{
	wxSize oldViewport = GetVirtualSize();

	xviewport = writer->xSizeContents + 2 * SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_X;
	yviewport = writer->ycursor + 2 * SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_Y;

	if ( xviewport != oldViewport.x || yviewport != oldViewport.y )
	{
		printf( "SET VIEWPORT %d ---- %d\n", xviewport, yviewport );
		SetVirtualSize( xviewport, yviewport );
	}
}

/*****************************************************
**
**   SheetWidget   ---   OnSize
**
******************************************************/
void SheetWidget::OnSize( wxSizeEvent &event )
{
	// set dirty only if larger than minimum size
	if ( event.GetSize().x > minxright ) dirty = true;
	SetSize( event.GetSize());
	Refresh();
}

/*****************************************************
**
**   SheetWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void SheetWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	const bool modpressed = event.m_controlDown;
	const bool shiftpressed = event.m_shiftDown;
	//printf( "SheetWidget::HandleMouseWheelEvent rotation is %d modpressed %d shiftpressed %d\n", event.GetWheelRotation(), modpressed, shiftpressed );

  if ( modpressed || shiftpressed )
	{
		event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
		event.Skip();
	}
	else
	{
		int x, y, x0, y0, xs, ys;
		const int offset = 60;

		GetViewStart( &x, &y );      // scroll position
		GetSize( &xs, &ys );         // size of visible widget only
		GetVirtualSize( &x0, &y0 );  // complete size of widget

		//printf( "SheetWidget::HandleMouseWheelEvent x %d y %d xs %d ys %d x0 %d y0 %d\n", x, y, xs, ys, x0, y0 );

		// Wheel scolls in vertical direction, but horizontal if nothing left to scroll
    if ( event.GetWheelRotation() < 0 )
    {
      if ( y + ys >= y0 ) x = Min( x0, x + offset );
      else
			{
				y += offset;
				mousePosition.y += offset;
			}
    }
    else
    {
      if ( y > 0 )
			{
				y = Max ( 0, y - offset );
				if ( mousePosition.y > offset ) mousePosition.y -= offset;
				else mousePosition.y = 0;
			}
      else x -= offset;
    }

		Scroll( x, y );

		// marked cols (if any) must move, too
		mouseHasMoved( false );
	}
}

/*****************************************************
**
**   SheetWidget   ---   doPaint
**
******************************************************/
void SheetWidget::doPaint( const wxRect &rect, const bool eraseBackground )
{
#ifdef SCROLLABLE_PAGE_WIDGET_SHOW_STOP_WATCH
	const wxLongLong starttime = wxGetLocalTimeMillis();
#endif

	assert( sheetcfg );
	
	if ( ! sheetConfigOverride ) updateSheetConfig();
	assert( painter );
	painter->writercfg = sheet->writercfg;
	painter->colorcfg = colorcfg;

	//printf( "RECT x %d y %d w %d h %d\n", rect.x, rect.y, rect.width, rect.height );
	MRect therect( rect );

	//printf( "SheetWidget::doPaint xviewport %d yviewport %d erase = %d DIRTY %d\n", xviewport, yviewport, eraseBackground, dirty );

	// dirty ist okay. kommt bei echten Updates und Resize. nicht bei Scroll, mouse over usw.
	if ( dirty )
	{
		init();
		calculateContentSize();
		initViewPort();
		dirty = false;
	}

	painter->setBrush( config->colors->bgColor );
	painter->setTransparentPen();
	painter->drawRectangle( rect );

	painter->setTransparentPen();

	writer->drawSheet( painter, therect, eraseBackground );

#ifdef SCROLLABLE_PAGE_WIDGET_SHOW_STOP_WATCH
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "SheetWidget::doPaint in %ld millisec eraseBackground %d" ),
		totaltime.ToLong(), eraseBackground ));
#endif
}

/*****************************************************
**
**   SheetWidget   ---   OnDataChanged
**
******************************************************/
void SheetWidget::OnDataChanged()
{
	//printf( "SheetWidget::OnDataChanged\n" );
	init();
	Refresh();
}

/*****************************************************
**
**   SheetWidget   ---   calculateContentSize
**
******************************************************/
void SheetWidget::calculateContentSize()
{
	wxSize p = GetSize();

	const double xrightmax = Max( p.x - 4.0 * SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_X, minxright );
	printf( "SheetWidget::calculateContentSize size x %d xrightmax %f minxright %d\n", p.x, xrightmax, minxright );
	writer->preformat( painter, xrightmax );
}

/*****************************************************
**
**   SheetWidget   ---   OnChildCommand
**
******************************************************/
void SheetWidget::OnChildCommand( wxCommandEvent &event )
{
	printf( "SheetWidget::OnChildCommand\n" );
	event.Skip();
}

/*****************************************************
**
**   SheetWidget   ---   onNavigationKeyCommand
**
******************************************************/
void SheetWidget::onNavigationKeyCommand( wxKeyEvent &event )
{
	printf( "SheetWidget::onNavigationKeyCommand\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/**************************************************************
***
**   SheetWidget   ---   mouseHasMoved
***
***************************************************************/
void SheetWidget::mouseHasMoved( const bool& /*mouseOutside*/ )
{
}



