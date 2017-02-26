/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicWidget.cpp
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

#include "BasicWidget.h"

#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/log.h>

#include "ChartProperties.h"
#include "Conf.h"
#include "ExportDialog.h"
#include "guibase.h"
#include "MenuProvider.h"
#include "Painter.h"

extern Config *config;

DEFINE_EVENT_TYPE( COMMAND_CHART_SKIN_CHANGED )

IMPLEMENT_CLASS2( BasicWidget, wxScrolledWindow, KeyEventObserver )

/*****************************************************
**
**   BasicWidget   ---   Constructor
**
******************************************************/
BasicWidget::BasicWidget( wxWindow *parent, ChartProperties *chartprops, wxWindowID id, const wxPoint& pos, const wxSize& size )
		: wxScrolledWindow( parent, id, pos, size ),
		chartprops(  chartprops )
{
	hborder = 10;
	vborder = 10;
	exportMode = false;
	dragMode = false;
	painter = 0;
	SetScrollRate( 1, 1 );
	mousePosition = wxPoint( -1, -1 );
	unscrolledMousePosition = mousePosition;

	keyMod = 0;
	mouseInside = false;

	widgetOptions = WO_NONE;

	fixedVedic = fixedWestern = false;

	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( BasicWidget::OnMouseWheelEvent ));
	Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( BasicWidget::OnMouseLeave ));
	Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( BasicWidget::OnMouseEnter ));

	Connect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BasicWidget::OnMenuCommand ));

	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( BasicWidget::OnMouseLeftDown ));
	Connect( wxEVT_LEFT_UP, wxMouseEventHandler( BasicWidget::OnMouseLeftUp ));
	Connect( wxEVT_IDLE, wxIdleEventHandler( BasicWidget::OnIdle ));

	Connect( wxEVT_PAINT, wxPaintEventHandler( BasicWidget::OnPaint ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( BasicWidget::OnKeyDown ));
	Connect( CMD_FIRST+1, CMD_GRAPHIC_STYLE+100, wxCommandEventHandler( BasicWidget::OnCommand ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BasicWidget::OnContextMenuEvent ));
}

/*****************************************************
**
**   BasicWidget   ---   Destructor
**
******************************************************/
BasicWidget::~BasicWidget()
{
}

/*****************************************************
**
**   BasicWidget   ---   OnDataChanged
**
******************************************************/
void BasicWidget::OnDataChanged()
{
	wxString cname = GetClassInfo()->GetClassName();
	//printf( "BasicWidget::OnDataChanged class %s\n", str2char( cname ));
	Refresh();
}

/*****************************************************
**
**   BasicWidget   ---   OnMenuCommand
**
******************************************************/
void BasicWidget::OnMenuCommand( wxCommandEvent &event )
{
	const int command = event.GetId();
	switch ( command )
	{
		case CMD_EXPORT_TEXT:
			exportAs( WeText );
		break;
		case CMD_EXPORT_CSV:
			exportAs( WeCsv );
		break;
		case CMD_EXPORT_HTML:
			exportAs( WeHtml );
		break;
		case CMD_EXPORT_PDF:
			exportAs( WePdf );
		break;
		case CMD_EXPORT_IMAGE:
			doImageExport();
		break;
		default:
			event.Skip();
		break;
	}	
}

/*****************************************************
**
**   BasicWidget   ---   isVedic
**
******************************************************/
bool BasicWidget::isVedic() const
{
	//static int count = 0;
	//printf( "BasicWidget::isVedic %d fixedVedic %d fixedWestern %d chartprops %d\n", count++, fixedVedic, fixedWestern, chartprops->isVedic());
	if ( fixedVedic )
	{
		assert( ! fixedWestern );
		return true;
	}
	else if ( fixedWestern )
	{
		return false;
	}
	else return chartprops->isVedic();
}

/*****************************************************
**
**   BasicWidget   ---   OnCommand
**
******************************************************/
void BasicWidget::OnCommand( wxCommandEvent& )
{
	wxLogDebug( wxT( "BasicWidget::OnCommand" ));
	//printf( "COMMAND %d\n", event.GetId());
	//if ( dispatchCommand( event.GetId())) OnDataChanged();
	//else event.Skip();
}

/*****************************************************
**
**   BasicWidget   ---   OnContextMenuEvent
**
******************************************************/
void BasicWidget::OnContextMenuEvent( wxMouseEvent& event)
{
	int x, y;
	x = event.m_x;
	y = event.m_y;
	wxWindow *window = (wxWindow*)event.GetEventObject();
	window->ClientToScreen( &x, &y );
	this->ScreenToClient( &x, &y );

	wxMenu *menu = ContextMenuProvider().getWidgetMenu( this );
	PopupMenu( menu, x, y );
	delete menu;
}

/*****************************************************
**
**   BasicWidget  ---   exportAs
**
******************************************************/
void BasicWidget::exportAs( const WidgetExportType &t )
{
	/*
	*  plain Basicwidget has only image export option.
	*  there is an implementaton for other types in SheetWidget.
	*/
	if ( t == WeImage ) doImageExport();
	else wxLogError( wxT( "BasicWidget cannot export type %d" ), t );
}

/*****************************************************
**
**   BasicWidget   ---   setFixedVedic
**
******************************************************/
void BasicWidget::setFixedVedic( const bool b )
{
	fixedVedic = b;
	fixedWestern = false;
}

/*****************************************************
**
**   BasicWidget   ---   setFixedWestern
**
******************************************************/
void BasicWidget::setFixedWestern( const bool b )
{
	fixedWestern = b;
	fixedVedic = false;
}

/*****************************************************
**
**   BasicWidget   ---   OnPaint
**
******************************************************/
void BasicWidget::OnPaint( wxPaintEvent& )
{
	int vx, vy;
	wxSize size = GetClientSize();
	wxRect refreshRect( 0, 0, size.x, size.y );

	wxPaintDC context( this );
	PrepareDC( context );

	wxRegionIterator upd( GetUpdateRegion());
	if ( upd ) refreshRect = upd.GetRect();

	while ( upd )
	{
		refreshRect.Union( upd.GetRect() );
		upd++;
	}
	GetViewStart( &vx, &vy );
	if ( vx || vy )
	{
		refreshRect.x += vx;
		refreshRect.y += vy;
	}
	//printf( "BasicWidget::OnPaint -- REFRESHRECT: x %d y %d w %d h %d\n", refreshRect.x, refreshRect.y, refreshRect.width, refreshRect.height );

	assert( painter == 0 );
	painter = new DcPainter( &context );
	doPaint( refreshRect, true );
	delete painter;
	painter = 0;
}

/*****************************************************
**
**   BasicWidget   ---   OnSize
**
******************************************************/
void BasicWidget::OnSize( wxSizeEvent &event )
{
	//printf( "BasicWidget::OnSize x %d y %d\n", event.GetSize().x, event.GetSize().y );
	SetSize( event.GetSize().x, event.GetSize().y );
	//Refresh();
}

/*****************************************************
**
**   BasicWidget   ---   doImageExport
**
******************************************************/
void BasicWidget::doImageExport()
{
	int exporttype;
	wxString filename, e;
	int xsize, ysize;

	GetVirtualSize( &xsize, &ysize );

	if ( config->view->graphicExportSizeMode == 0 )
	{
		ExportDialog dialog( this, xsize, ysize );
		if ( dialog.ShowModal() != wxID_OK ) return;
		dialog.getSizes( xsize, ysize );
		config->view->graphicExportSizeMode = dialog.getCheckShowDialog();
	}

	const static wxString filetypes =
	    wxT( "PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp|PCX (*pcx)|*pcx|PNM (*.pnm)|*.pnm|TIFF (*.tif)|*.tif|All files (*)| *.*" );

	switch ( config->view->defGraphicExportType )
	{
	case 1:
		filename = wxT( "out.jpg"  );
		break;
	case 2:
		filename = wxT( "out.bmp"  );
		break;
	case 3:
		filename = wxT( "out.pcx"  );
		break;
	case 4:
		filename = wxT( "out.pnm"  );
		break;
	case 5:
		filename = wxT( "out.tif"  );
		break;
	default:
		filename = wxT( "out.png"  );
		break;
	}
	int style = wxFD_SAVE;
	if ( config->view->exportAskOnOverwrite ) style |= wxFD_OVERWRITE_PROMPT;

	wxFileDialog exportFileDialog( this, _("Export Picture" ), config->viewprefs->defExportPath, filename, filetypes, style, wxDefaultPosition );
	exportFileDialog.SetFilterIndex( config->view->defGraphicExportType );

	if ( exportFileDialog.ShowModal() == wxID_OK )
	{
		filename = exportFileDialog.GetPath();
		e = filename.Right(4).MakeLower();
		if ( e == wxT( ".png" ))
		{
			exporttype = wxBITMAP_TYPE_PNG;
			config->view->defGraphicExportType = 0;
		}
		else if ( e == wxT( ".jpg" ))
		{
			exporttype = wxBITMAP_TYPE_JPEG;
			config->view->defGraphicExportType = 1;
		}
		else if ( e == wxT( "jpeg" ))
		{
			exporttype = wxBITMAP_TYPE_JPEG;
			config->view->defGraphicExportType = 1;
		}
		else if ( e == wxT( ".bmp" ))
		{
			exporttype = wxBITMAP_TYPE_BMP;
			config->view->defGraphicExportType = 2;
		}
		else if ( e == wxT( ".pcx" ))
		{
			exporttype = wxBITMAP_TYPE_PCX;
			config->view->defGraphicExportType = 3;
		}
		else if ( e == wxT( ".pnm" ))
		{
			exporttype = wxBITMAP_TYPE_PNM;
			config->view->defGraphicExportType = 4;
		}
		else if ( e == wxT( ".tif" ))
		{
			exporttype = wxBITMAP_TYPE_TIF;
			config->view->defGraphicExportType = 5;
		}
		else
		{
			doMessageBox( this, wxString::Format( _( "Can't determine image handler for extension \"%s\", using default (PNG)" ), e.c_str()));
			filename.Append( wxT ( ".png" ));
			exporttype = wxBITMAP_TYPE_PNG;
			config->view->defGraphicExportType = 0;
		}
		config->viewprefs->defExportPath = exportFileDialog.GetDirectory();
		wxBitmap bitmap( xsize, ysize );
		wxMemoryDC dc;
		dc.SelectObject( bitmap );
		painter = new DcPainter( &dc );
		exportMode = true;
		wxRect refreshRect = wxRect( 0, 0, xsize, ysize );
		doPaint( refreshRect, true );
		exportMode = false;
		delete painter;
		painter = 0;
		if ( bitmap.SaveFile( filename, (wxBitmapType)exporttype ))
		{
			doMessageBox( this, wxString::Format( _("Picture exported to %s"), filename.c_str()));
		}
	}
}

/*****************************************************
**
**   BasicWidget   ---   OnIdle
**
******************************************************/
void BasicWidget::OnIdle( wxIdleEvent& )
{
	if ( ! mouseInside ) return;

	wxMouseState mst = wxGetMouseState();
	const wxPoint p = ScreenToClient( wxPoint( mst.GetX(), mst.GetY()));

	int ks = 0;
	if ( wxGetKeyState( WXK_CONTROL )) ks |= wxMOD_CONTROL;
	if ( wxGetKeyState( WXK_SHIFT )) ks |= wxMOD_SHIFT;
	if ( wxGetKeyState( WXK_ALT )) ks |= wxMOD_ALT;

	//static int cc = 0;
	//printf( "BasicWidget::OnIdle %d key %d mouse pos x %d y %d old pos %d %d\n", cc++, ks, p.x, p.y, unscrolledMousePosition.x, unscrolledMousePosition.y  );

	if ( p == unscrolledMousePosition && ks == keyMod ) return;

	keyMod = ks;


	if ( dragMode )
	{
		//printf( "DRAG %d %d\n", mousePosition.x - p.x , mousePosition.y - p.y );
		Scroll( mousePosition.x - p.x , mousePosition.y - p.y );
	}

	int vx, vy;
	unscrolledMousePosition = mousePosition = p;
	GetViewStart( &vx, &vy );
	mousePosition.x += vx;
	mousePosition.y += vy;

	mouseHasMoved( false );
}

#define IS_KEY_CODE( c ) c == WXK_UP || c == WXK_DOWN || c == WXK_LEFT || c == WXK_RIGHT || c == WXK_PAGEDOWN || c == WXK_PAGEUP

/*****************************************************
**
**   BasicWidget   ---   OnKeyDown
**
******************************************************/
void BasicWidget::OnKeyDown( wxKeyEvent &event )
{
	//printf( "KEY DOWN in BasicWidget::OnKeyDown MODIFIERS %d ctrl %d\n", event.GetModifiers(), event.m_controlDown );

	if ( IS_KEY_CODE( event.GetKeyCode() ))
	{
		onNavigationKeyCommand( event );
	}
}

/*****************************************************
**
**   BasicWidget   ---   onNavigationKeyCommand
**
******************************************************/
void BasicWidget::onNavigationKeyCommand( wxKeyEvent &event )
{
	//printf( "BasicWidget::onNavigationKeyCommand %d\n", event.GetKeyCode() );
	chartprops->processNavigationKey( event.GetKeyCode() );
	//printf( "FERTIG BasicWidget::onNavigationKeyCommand %d\n", event.GetKeyCode() );
	OnDataChanged();
	Refresh();
}

/*****************************************************
**
**   BasicWidget   ---   OnChar
**
******************************************************/
void BasicWidget::OnChar( wxKeyEvent &event )
{
	wxLogDebug( wxT( "CHAR in BasicWidget::OnChar" ));
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   BasicWidget   ---   OnMouseWheelEvent
**
******************************************************/
void BasicWidget::OnMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "WHEEL in BasicWidget::onMouseWheelEvent %d - %d\n", event.GetX(), event.GetY() );
	HandleMouseWheelEvent( event );
}

/*****************************************************
**
**   BasicWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void BasicWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	//static int i = 0;
	//printf( "HANDLE WHEEL in BasicWidget::onMouseWheelEvent %d x %d - y %d rotation %d\n", i++, event.GetX(), event.GetY(), event.GetWheelRotation() );

	bool vedic = isVedic();

#if defined( __WXMAC__ )
	/*
	bool shiftpressed = wxGetKeyState( WXK_SHIFT );
	if ( shiftpressed )
	{
		chartprops->changeSkin( event.GetWheelRotation() > 0 );
	}
	*/
	if ( fabs( event.GetWheelRotation() ) < 3 ) return;
	chartprops->changeSkin( event.GetWheelRotation() > 0, vedic );
#else
	chartprops->changeSkin( event.GetWheelRotation() > 0, vedic );
#endif
	Refresh();
	//OnDataChanged();

	// needed by vedic/WesternChartPanel to change selection in skin choice
  wxCommandEvent e( COMMAND_CHART_SKIN_CHANGED, GetId());
	wxPostEvent( GetParent(), e );
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseLeftDown
***
***************************************************************/
void BasicWidget::OnMouseLeftDown( wxMouseEvent& )
{
	SetFocus();
	dragMode = true;
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseLeftUp
***
***************************************************************/
void BasicWidget::OnMouseLeftUp( wxMouseEvent& )
{
	dragMode = false;
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseLeave
***
***************************************************************/
void BasicWidget::OnMouseLeave( wxMouseEvent& )
{
	//printf( "BasicWidget::OnMouseLeeve x %d y %d\n", event.GetX(), event.GetY() );
	dragMode = false;
	mouseInside = false;

	unscrolledMousePosition = mousePosition = wxPoint( -1, -1 );
	mouseHasMoved( true );
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseEnter
***
***************************************************************/
void BasicWidget::OnMouseEnter( wxMouseEvent &event )
{
	int vx, vy;
	//printf( "BasicWidget::OnMouseEnter x %d y %d\n", event.GetX(), event.GetY() );
	unscrolledMousePosition = mousePosition = event.GetPosition();
	GetViewStart( &vx, &vy );
	mousePosition.x += vx;
	mousePosition.y += vy;
	mouseInside = true;

	mouseHasMoved( false );
}

/**************************************************************
***
**   BasicWidget   ---   mouseHasMoved
***
***************************************************************/
void BasicWidget::mouseHasMoved( const bool& /*outside */ )
{
	//printf( "BasicWidget::mouseHasMoved b %d\n", b );
}

/**************************************************************
***
**   BasicWidget   ---   echo
***
***************************************************************/
void BasicWidget::echo( wxString )
{
	// TODO
}

/**************************************************************
***
**   EmptyWidget   ---   Constructor
***
***************************************************************/
EmptyWidget::EmptyWidget( wxWindow *parent, ChartProperties *props )
 : BasicWidget( parent, props )
{
}

/**************************************************************
***
**   EmptyWidget   ---   doPaint
***
***************************************************************/
void EmptyWidget::doPaint( const wxRect&, const bool /*eraseBackground*/ )
{
	wxSize size = GetVirtualSize();
	painter->setPen( *wxRED );
	painter->drawLine( 0, 0, size.x, size.y );
	painter->drawLine( 0, size.y, size.x, 0 );
	painter->drawTextFormatted( wxRect( 0, 0, size.x, .5 * size.y ), isVedic() ? wxT( "vedic" ) : wxT( "western" ), Align::Center );
}



