/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicView.cpp
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

#include "BasicView.h"

#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/layout.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>

#include "BasicWidget.h"
#include "ChartProperties.h"
#include "ChildWindow.h"
#include "Commands.h"
#include "Conf.h"
#include "Document.h"
#include "SplitterWidget.h"
#include "Statusbar.h"
#include "ToolPanel.h"
#include "ViewNode.h"

extern Config *config;

IMPLEMENT_CLASS2( BasicView, wxWindow, KeyEventObserver )
IMPLEMENT_CLASS( EmptyView, BasicView )

/*****************************************************
**
**   BasicView   ---   Constructor
**
******************************************************/
BasicView::BasicView( wxWindow *parent, ChildWindow *frame, const VIEW_ID &viewId, const bool &showToolbar, const long style )
		: wxWindow( parent, -1, wxDefaultPosition, wxDefaultSize, style ),
		frame( frame ),
		viewId( viewId ),
		showtoolbar( showToolbar ),
		props( frame->getProps() ),
		doc( frame->getDoc() )
{
	toolbar = 0;
	widget = 0;
	detailpanel = 0;
	statusbar = 0;
	if ( doc )
	{
		props->setHasDocument( true );
		//addWidgetOption( WO_MENU_EDIT_CHARTDATA );
	}

	sizer = new wxBoxSizer( wxVERTICAL );
	SetSizer( sizer );

	toolbar = ( showtoolbar ? ToolbarFactory::get()->createToolbar( this, viewId ) : 0 );

	Connect( wxEVT_SIZE, wxSizeEventHandler( BasicView::OnSize ));
	//Connect( CMD_FIRST+1, CMD_GRAPHIC_STYLE+100, wxCommandEventHandler( BasicView::OnCommand ));

	Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BasicView::OnSetFocus ));

	// toolbar events
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BasicView::OnChoiceCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler( BasicView::OnChoiceCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BasicView::OnMenuCommand ));
}

/*****************************************************
**
**   BasicView   ---   OnCommand
**
******************************************************/
void BasicView::OnCommand( wxCommandEvent &event )
{
	wxLogDebug( wxT( "BasicView::OnCommand" ));
	if ( dispatchCommand( event.GetId())) OnDataChanged();
	else event.Skip();
}

/*****************************************************
**
**   BasicView   ---   OnMenuCommand
**
******************************************************/
void BasicView::OnMenuCommand( wxCommandEvent &event )
{
	//printf( "BasicView::OnMenuCommand\n" );
	event.SetEventObject( (wxWindow*)this );
	wxPostEvent( frame->GetParent(), event );
}

/*****************************************************
**
**   BasicView   ---   DoGetBestSize
**
******************************************************/
wxSize BasicView::DoGetBestSize() const
{
	return wxSize( 200, 200 );
}

/*****************************************************
**
**   BasicView   ---   OnChoiceCommand
**
******************************************************/
void BasicView::OnChoiceCommand( wxCommandEvent& )
{
	OnToolbarCommand();
	widget->SetFocus();
}

/*****************************************************
**
**   BasicView   ---   OnEnterPressed
**
******************************************************/
void BasicView::OnEnterPressed( wxKeyEvent& )
{
	OnToolbarCommand();
}

/*****************************************************
**
**   BasicView   ---   OnSpinCommand
**
******************************************************/
void BasicView::OnSpinCommand( wxSpinEvent& )
{
	OnToolbarCommand();
}

/*****************************************************
**
**   BasicView   ---   OnGeneralCommand
**
******************************************************/
void BasicView::OnGeneralCommand( wxCommandEvent& )
{
	OnToolbarCommand();
}

/*****************************************************
**
**   BasicView   ---   OnSize
**
******************************************************/
void BasicView::OnSize( wxSizeEvent& )
{
	//printf( "BasicView::OnSize\n" );
	Layout(); // Needed because the widget will not resize
	Refresh();
}

/*****************************************************
**
**   BasicView   ---   OnSetFocus
**
******************************************************/
void BasicView::OnSetFocus( wxFocusEvent& )
{
	handleSetFocusEvent();
}

/*****************************************************
**
**   BasicView   ---   handleSetFocusEvent
**
******************************************************/
void BasicView::handleSetFocusEvent()
{
	if ( widget ) widget->SetFocus();
}

/*****************************************************
**
**   BasicView   ---   dispatchCommand
**
******************************************************/
bool BasicView::dispatchCommand( const int &command )
{
	wxLogMessage( wxString::Format( wxT( "BasicView::dispatchCommand %d ew %d" ), command, isEwFixed()));
	return props->dispatchWidgetPropertyCommand( command );
}

/*****************************************************
**
**   BasicView   ---   OnDataChanged
**
******************************************************/
void BasicView::OnDataChanged()
{
	// Nesessary on Lunar Node True/Mean Changes
	//printf( "BasicView::OnDataChanged\n" );
	props->dispatchWidgetPropertyCommand( 0 );
	Refresh();
}

/*****************************************************
**
**   BasicView   ---   doLayout
**
******************************************************/
void BasicView::doLayout()
{
	sizer->Clear();

	// Toolbar
	if ( toolbar ) sizer->Add( toolbar, 0, wxEXPAND );

	// contents
	assert( widget );
	sizer->Add( widget, 1, wxEXPAND | wxALL, config->view->childViewFrameWidth );

	// detail panel
	if ( detailpanel ) sizer->Add( detailpanel, 0, wxEXPAND, 0 );

	// status bar
	if ( statusbar ) sizer->Add( statusbar, 0, wxEXPAND, 0 );

	sizer->Fit( this ); // Needed by Windows (view change in ephemeris and partner view)
	// TODO verify, problem in partner view obsolet
	frame->SendSizeEvent(); // Needed by GTK + Windows
	Layout();
}

/*****************************************************
**
**   BasicView   ---   getTitle
**
******************************************************/
wxString BasicView::getTitle()
{
	wxString label, s = wxT( "empty" );
	if ( doc )
	{
		label = getWindowLabel( true );
		if ( ! label.IsEmpty() ) s.Printf( wxT( "%s <%s>" ), doc->getHName().c_str(), getWindowLabel( false ).c_str() );
		else s = doc->getHName();
	}
	else {
		s = getWindowLabel();
	}
	return s;
}

/*****************************************************
**
**   BasicView   ---   echo
**
******************************************************/
void BasicView::echo( wxString /*message*/ )
{
	//if ( statusbar ) statusbar->echo( message );
}

/*****************************************************
**
**   BasicView   ---   setStatusText
**
******************************************************/
void BasicView::setStatusText( wxString message, const int &field )
{
	if ( statusbar ) statusbar->SetStatusText( message, field );
}

/*****************************************************
**
**   EmptyView   ---   Constructor
**
******************************************************/
EmptyView::EmptyView( wxWindow *parent, ChildWindow *frame )
		: BasicView( parent, frame, VIEW_EMPTY, false )
{
	widget = new EmptyWidget( this, props );
}

/**************************************************************
***
**   ViewFactory   ---   createEmptyView
***
***************************************************************/
BasicView *ViewFactory::createEmptyView( wxWindow *parent, ChildWindow *frame )
{
	return new EmptyView( parent, frame );
}

/**************************************************************
***
**   ViewFactory   ---   createView
***
***************************************************************/
BasicView *ViewFactory::createView( wxWindow *parent, ChildWindow *frame, WidgetNode *node )
{
	assert( node );
	BasicView * view = 0;
	switch ( node->viewId )
	{
	case VIEW_TEXT:
		view = createSelectableTextView( parent, frame, node->toolpanel, node->textmode );
		break;
	case VIEW_YOGA:
		view = createYogaView( parent, frame );
		break;
	case VIEW_GRAPHICALDASA:
		view = createGraphicalDasaView( parent, frame );
		break;
	case VIEW_PLANETLIST:
		view = createMainView( parent, frame );
		break;
	case VIEW_GRAPHIC:
		view = createGraphicView( parent, frame, (Varga)node->varga );
		break;
	case VIEW_DASA_TREE:
		view = createDasaTreeView( parent, frame );
		break;
	case VIEW_DASA_COMPOSITE:
		view = createDasaCompositeView( parent, frame );
		break;
	case VIEW_URANIAN:
		view = createUranianView( parent, frame );
		break;

#ifdef USE_URANIAN_CHART
	case VIEW_URANIAN_CHART:
		view = createUranianChartView( parent, frame );
		break;
#endif

	case VIEW_TRANSIT:
		view = createTransitView( parent, frame );
		break;
	case VIEW_VARGA:
		view = createVargaView( parent, frame );
		break;

#ifdef USE_PRINT_VIEW
	case VIEW_PRINT:
		view = createPrintView( parent, frame );
		break;
#endif

	case VIEW_SHADBALA:
		view = createShadbalaView( parent, frame );
		break;
	case VIEW_ASHTAKAVARGA:
		view = createAshtakaVargaView( parent, frame );
		break;
	case VIEW_VIMSOPAKABALA:
		view = createVimsopakaBalaView( parent, frame );
		break;
	case VIEW_SOLAR:
		view = createSolarView( parent, frame );
		break;
	case VIEW_SBC:
		view = createSbcView( parent, frame );
		break;
	case VIEW_HORA:
		view = createHoraView( parent, frame );
		break;
	case VIEW_ECLIPSE:
		view = createEclipseView( parent, frame );
		break;
	case VIEW_EPHEM:
		view = createEphemView( parent, frame );
		break;
	case VIEW_EMPTY:
		view = createEmptyView( parent, frame );
		break;
	case VIEW_YOGA_EDITOR:
		view = createYogaEditorView( parent, frame );
		break;
	case VIEW_GRAPHICGRID:
		view = createGraphicGridView( parent, frame, node->rows, node->cols, node->vargas );
		break;
	default:
		wxLogWarning( wxString::Format( wxT( "ViewFactory::createView unknown id %d" ), (int)node->viewId ));
		//assert( false );
		view = createEmptyView( parent, frame );
		break;
	}
	assert( view );
	return view;
}


