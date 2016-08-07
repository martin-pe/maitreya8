/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWidget.cpp
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

#include "SheetWidget.h"

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

IMPLEMENT_CLASS( SheetWidget, BasicWidget )

#define SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_X 40
#define SCROLLABLE_PAGE_WIDGET_PAGE_BORDER_Y 20
#define SCROLLABLE_PAGE_WIDGET_MIN_XRIGHT 800

/*****************************************************
**
**   SheetWidget   ---   Constructor
**
******************************************************/
SheetWidget::SheetWidget( wxWindow *parent, ChartProperties *props, SheetConfig *scfg, WriterConfig *wcfg, ColorConfig *ccfg )
: BasicWidget( parent, props )
{
	writercfg = wcfg ? wcfg: config->writer;

	colorcfg = ccfg ? ccfg : config->colors;
	assert( colorcfg );
	sheetcfg = (SheetConfig*)NULL;
	minxright = SCROLLABLE_PAGE_WIDGET_MIN_XRIGHT;

	if ( scfg )
	{
		sheetcfg = scfg;
		sheetConfigOverride = true;
	}
	else
	{
		sheetConfigOverride = false;
		SheetConfigLoader *loader = SheetConfigLoader::get();
		sheetcfg = loader->getConfig( config->view->sheetStyle );
	}

	sheet = new Sheet( writercfg );
	writer = new DcSheetWriter( sheet, sheetcfg, writercfg, colorcfg );
	init();
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
	delete sheet;
	delete writer;
}

/*****************************************************
**
**   SheetWidget   ---   enableFloatingLayout
**
******************************************************/
void SheetWidget::enableFloatingLayout( const bool b )
{
 printf( "SheetWidget::enableFloatingLayout %d\n", b );
 writer->enableFloatingLayout( b );
}

/*****************************************************
**
**   SheetWidget   ---   setWriterConfig
**
******************************************************/
void SheetWidget::setWriterConfig( WriterConfig *wcfg )
{
	writercfg = wcfg;
	writer->setWriterConfig( writercfg );
}

/*****************************************************
**
**   SheetWidget   ---   setSheetConfig
**
******************************************************/
void SheetWidget::setSheetConfig( SheetConfig *scfg )
{
	sheetcfg = scfg;
	sheetConfigOverride = true;
	writer->setSheetConfig( sheetcfg );
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
**   SheetWidget   ---   clearSheet
**
******************************************************/
void SheetWidget::clearSheet()
{
	sheet->clear();
}

/*****************************************************
**
**   SheetWidget   ---   getWidgetOptions
**
******************************************************/
int SheetWidget::getWidgetOptions()
{
	int wo = BasicWidget::getWidgetOptions();

	if ( sheet->items.size() > 0 ) wo |= WO_EXPORT_GRAFIC;

	for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
	{
		SheetItem *item = *iter;
		switch( item->type )
		{
			case WiTable:
			case WiText:
				//return WO_EXPORT_ALL;
				wo |= WO_EXPORT_PLAINTEXT;
				wo |= WO_EXPORT_CSVTEXT;
				wo |= WO_EXPORT_HTMLTEXT;
				wo |= WO_EXPORT_PDF;
			break;
			default:
			break;
		}
	}
	return wo;
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

	painter->setBrush( sheetcfg->brush );
	if ( sheetcfg->brush.color.IsOk())
	{
		painter->setPen( wxPen( sheetcfg->brush.color, 1, wxSOLID ));
	}
	else painter->setTransparentPen();
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
**   SheetWidget   ---   exportAs
**
******************************************************/
void SheetWidget::exportAs( const WidgetExportType &type )
{
	printf( "SheetWidget::exportAs %d\n", type );

	switch( type  )
	{
		case WeText:
		case WeCsv:
		case WeHtml:
			doTextExport( type );
		break;
		case WePdf:
			doPdfExport();
		break;
		case WeImage:
			doImageExport();
		break;
		default:
			assert( false );
		break;
	}
	OnDataChanged();
}

/*****************************************************
**
**   SheetWidget   ---   doTextExport
**
******************************************************/
void SheetWidget::doTextExport( const WidgetExportType &type )
{
	wxString filename, filetypes, title;

	switch( type  )
	{
		case WeText:
			filename = wxT( "out.txt" );
			filetypes = wxT( "Text (*.txt)|*.txt|All files (*)|*.*" );
			title = _( "Export as Plain Text" );
		break;
		case WeCsv:
			filename = wxT( "out.csv" );
			filetypes = wxT( "Text (*.csv)|*.csv|All files (*)|*.*" );
			title = _( "Export as Csv Text" );
		break;
		case WeHtml:
			filename = wxT( "out.html" );
			filetypes = wxT( "Html (*.html)|*.html|All files (*)|*.*" );
			title = _( "Export as HTML Text" );
		break;
		default:
			assert( false );
		break;
	}
	int style = wxFD_SAVE;
	if ( config->view->exportAskOnOverwrite ) style |= wxFD_OVERWRITE_PROMPT;

	wxFileDialog exportFileDialog( this, title, config->viewprefs->defExportPath, filename, filetypes, style, wxDefaultPosition );
	if ( exportFileDialog.ShowModal() == wxID_OK )
	{
		filename = exportFileDialog.GetPath();
		config->viewprefs->defExportPath = exportFileDialog.GetDirectory();
		Exporter *exporter = ExporterFactory().getExporter( type );
		wxString s =  exporter->exportSheet( sheet );
		//Cout( s );
		wxFile file;
		file.Create( filename, true );
		file.Write( s );
		file.Close();
		doMessageBox( this, wxString::Format( _("Text exported to %s"), filename.c_str()));
	}
}

/*****************************************************
**
**   SheetWidget   ---   doPdfExport
**
******************************************************/
void SheetWidget::doPdfExport()
{
	sheet->resetMarkup();
	Sheet *pdfsheet = sheet->cloneClean();

	PdfTool tool( (PdfDocumentConfig*)NULL );
	tool.doSheetExport( pdfsheet );
	delete pdfsheet;
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
void SheetWidget::mouseHasMoved( const bool &mouseOutside )
{
	int vx, vy;
	GetViewStart( &vx, &vy );
	SheetItem *matchitem = (SheetItem*)NULL;

	// what about bar diagrams
	if ( ! sheetcfg->selectionMode ) return;

	if ( ! mouseOutside )
	{
		for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
		{
			SheetItem *item = (SheetItem*)*iter;
			assert( item );

			//printf( "item rect %f %f %f %f mouse %d %d\n", item->rect.x, item->rect.y, item->rect.width, item->rect.height, mousePosition.x, mousePosition.y );
			if ( pointInRect( mousePosition, item->rect ))
			{
				matchitem = item;
				break;
			}
		}
	}

	//printf( "SheetWidget::mouseHasMoved %d mouse x %d y %d lastItem %ld matchitem %ld\n", 
		//mouseOutside, mousePosition.x, mousePosition.y, (long)lastMarkedItem, (long) matchitem );

	if ( matchitem )
	{
		SheetItem *subitem = matchitem->getSubitem4Point( mousePosition );
		if ( subitem != matchitem->markedItem )
		{
			wxRect therect;
			if ( matchitem->markedItem )
			{
				therect = matchitem->markedItem->getRefreshRect( true, sheetcfg->selectionMode ).toWxRect();
			}
			if ( subitem )
			{
				therect.Union( subitem->getRefreshRect( true, sheetcfg->selectionMode ).toWxRect());
			}
			matchitem->markItem( subitem );
			therect.Offset( -vx, -vy );
			//printf( "REFRESH %d %d %d %d\n", therect.x, therect.y, therect.width, therect.height );
			RefreshRect( therect );
			//Refresh();
		}
	}

	if ( lastMarkedItem  && lastMarkedItem != matchitem ) // reset old marked item
	{
		lastMarkedItem->resetMarkup();
		wxRect therect = lastMarkedItem->getRefreshRect( true, sheetcfg->selectionMode ).toWxRect();
		therect.Offset( -vx, -vy );
		RefreshRect( therect );
	}
	lastMarkedItem = matchitem;
}



