/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChildWindow.cpp
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

#include "ChildWindow.h"
#include "BasicView.h"
#include "SplitterWidget.h"

#include <wx/string.h>

#include <wx/app.h>
#include <wx/imaglist.h>
#include <wx/notebook.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>

#include "ChartProperties.h"
#include "Commands.h"
#include "Conf.h"
#include "Document.h"
#include "guibase.h"
#include "IdConverter.h"
#include "ImageProvider.h"
#include "ViewNode.h"

extern Config *config;

IMPLEMENT_CLASS( ChildWindow, wxFrame )
IMPLEMENT_CLASS( SimpleChildWindow, ChildWindow )

DEFINE_EVENT_TYPE( CHILD_ADDTORECENT )
DEFINE_EVENT_TYPE( CHILD_SETACTIVE )
DEFINE_EVENT_TYPE( CHILD_CLOSED )
DEFINE_EVENT_TYPE( CHILD_ADDED )
DEFINE_EVENT_TYPE( SHOW_EPHEM_FILE_WARNING )

/*****************************************************
**
**   ChildWindow   ---   Constructor
**
******************************************************/
ChildWindow::ChildWindow( wxFrame *parent, Document *document, const BitmapId &bid, const wxSize &size, const bool ismain )
		: wxFrame( parent, -1, wxT( "childwindow" ), wxDefaultPosition, size ),
		doc( document ),
		bmpId( bid )
{
	isvalid = true;
	ismainwindow = false;
	setMainWindow( ismain );
	if ( doc && ! ismain ) doc->addChildWindow( this );
	SetMinSize( wxSize( 200, 200 ));
	props = new ChartProperties;
	SetIcon( ImageProvider::get()->getIcon( bmpId ));

	Connect( wxEVT_ACTIVATE, wxActivateEventHandler( ChildWindow::OnActivate ));
	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ChildWindow::OnClose ));
	Connect( wxID_ANY, SHOW_EPHEM_FILE_WARNING, wxCommandEventHandler( ChildWindow::OnEphemFileWarning ));
	//Connect( CMD_FIRST+1, CMD_GRAPHIC_STYLE+100, wxCommandEventHandler( ChildWindow::OnCommand ));
}

/*****************************************************
**
**   ChildWindow   ---   Destructor
**
******************************************************/
ChildWindow::~ChildWindow()
{
	if ( ismainwindow ) delete doc;
	delete props;
}

/*****************************************************
**
**   ChildWindow   ---   OnEphemFileWarning
**
******************************************************/
void ChildWindow::OnEphemFileWarning( wxCommandEvent& /*event*/ )
{
	// show only once a session
	static bool shown = false;

	if ( ! shown )
	{
		//if ( ! config->viewprefs->doNotShowAgainWarningEphemFiles )
		if ( config->ephem->showEphemWarning )
		{
			wxString message = 
				_( "Some items could not be calculated properly. Please check if you configured Swiss Ephemeris data files in Configuration/Ephemeris correctly." );
			DoNotShowAgainDialog dialog( this, message, &config->ephem->showEphemWarning );
			dialog.ShowModal();
		}
		shown = true;
	}
}

/*****************************************************
**
**   ChildWindow   ---   getViewListImageId
**
******************************************************/
int ChildWindow::getViewListImageId() const
{
	return ImageProvider::get()->getViewImageListIndex4BitmapId( bmpId );
}

/*****************************************************
**
**   ChildWindow   ---   OnClose
**
******************************************************/
void ChildWindow::OnClose( wxCloseEvent &event )
{
	wxLogMessage( wxT( "ChildWindow::OnClose classname %s" ), GetClassInfo()->GetClassName());

	if ( ismainwindow )
	{
		if ( ! doc->queryClose())
		{
			event.Veto();
			return;
		}
		doc->closeDependentChildWindows();
	}
	wxCommandEvent e( CHILD_CLOSED, GetId() );
	e.SetEventObject( this );

	// Issue 3.0
	//GetParent()->ProcessWindowEvent( e );
	//GetParent()->ProcessEvent( e );
	wxPostEvent( GetParent(), e );

	if ( doc ) doc->releaseChildWindow( this );
	Destroy();
	isvalid = false;
}

/*****************************************************
**
**   ChildWindow   ---   OnActivate
**
******************************************************/
void ChildWindow::OnActivate( wxActivateEvent& )
{
	setTitle();
	wxCommandEvent e( CHILD_SETACTIVE, GetId() );
	e.SetEventObject( this );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ChildWindow   ---   dispatchCommand
**
******************************************************/
bool ChildWindow::dispatchCommand( const int& command )
{
	switch ( command )
	{
	case CMD_CLOSE:
		Close();
		break;
	case CMD_CLOSE_DOC:
		if ( doc ) doc->getMainWindow()->Close();
		else Close();
		break;
	case CMD_SAVE:
		if ( doc ) doc->save();
		break;
	case CMD_SAVEAS:
		if ( doc ) doc->saveAs();
		break;
	case CMD_QUICKPRINT:
		if ( doc ) doc->doQuickPrint();
		break;
	case CMD_PRINT:
		if ( doc ) doc->doPrint();
		break;
	case CMD_EDITDATA:
		if ( doc ) doc->editData();
		break;
	case CMD_EXPORT_TEXT:
	case CMD_EXPORT_HTML:
	case CMD_EXPORT_PDF:
	case CMD_EXPORT_IMAGE:
			printf( "EXPORT\n" );
	break;
	case CMD_ANIMATE:
		if ( doc )
		{
			props->setAnimated( ! doc->isAnimated());
			doc->StartTimer( props->isAnimated());
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

/*****************************************************
**
**   ChildWindow   ---   OnCommand
**
******************************************************/
void ChildWindow::OnCommand( wxCommandEvent &event )
{
	if ( ! isvalid ) return; // nothing to do for closing views

	const int command = event.GetId();
	//printf( "ChildWindow::OnCommand command is %d\n", command );

	/*
	wxObject *obj = event.GetEventObject();
	if ( obj )
	{
		wxString cname = obj->GetClassInfo()->GetClassName();
		printf( "CLASSNAME %s\n", str2char( cname ));
	}
	else printf( "CLASS NADA\n" );
	*/

	//if (IS_NAVIGATION_KEY_COMMAND( event.GetId())) printf( "NAVIGATION KAY COMMAND\n" );

	if ( command >= CMD_NEW_TEXT && command <= CMD_NEW_RASI+30 )
	{
		event.Skip(); // Delegate to parent
	}
	else
	{
		bool ret = props->dispatchWidgetPropertyCommand( command );
		//printf( "ChildWindow::dispatchCommand dispatchWidgetPropertyCommand return is %d\n", ret );
		if ( ret ) OnDataChanged();
		else dispatchCommand( event.GetId() );
	}
}

/*****************************************************
**
**   ChildWindow   ---   setMainWindow
**
******************************************************/
void ChildWindow::setMainWindow( const bool &b )
{
	ismainwindow = b;
	if ( b ) doc->setMainWindow( this );
}

/*****************************************************
**
**   SimpleChildWindow   ---   Constructor
**
******************************************************/
SimpleChildWindow::SimpleChildWindow( wxFrame *parent, Document *doc, const BitmapId &bmpId, const wxSize &size, const bool ismain )
		: ChildWindow( parent, doc, bmpId, size, ismain )
{
	view = 0;
	Connect( wxEVT_ACTIVATE, wxActivateEventHandler( SimpleChildWindow::OnActivate ));
	Connect( wxEVT_SIZE, wxSizeEventHandler( SimpleChildWindow::OnSize ));
	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SimpleChildWindow::OnClose ));
}

/*****************************************************
**
**   SimpleChildWindow   ---   Destructor
**
******************************************************/
SimpleChildWindow::~SimpleChildWindow()
{
}

/*****************************************************
**
**   SimpleChildWindow   ---   setTitle
**
******************************************************/
void SimpleChildWindow::setTitle()
{
	if ( view ) SetTitle( view->getTitle() );
	else SetTitle( wxT( "empty" ));
}

/*****************************************************
**
**   SimpleChildWindow   ---   dispatchCommand
**
******************************************************/
bool SimpleChildWindow::dispatchCommand( const int& command )
{
	bool ret = false;
	assert( view );

	//if ( view )
	//printf( "SimpleChildWindow::dispatchCommand VIEWID %d\n", view->getViewId() );
	//else assert( false );

	if ( view ) ret = view->dispatchCommand( command );
	else wxLogError( wxT( "SimpleChildWindow::dispatchCommand - view not set" ));

	if ( ret )
	{
		// required e.g. for ew toggle because text views do not update otherwise
		view->OnDataChanged();
		view->Refresh();
		return ret;
	}
	else return ChildWindow::dispatchCommand( command );
}

/*****************************************************
**
**   SimpleChildWindow   ---   insertView
**
******************************************************/
void SimpleChildWindow::insertView( BasicView *v )
{
	view = v;
	setTitle();
}

/*****************************************************
**
**   SimpleChildWindow   ---   postCreate
**
******************************************************/
void SimpleChildWindow::postCreate()
{
	assert( view );
	view->postCreate();
	//view->Layout();
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnClose
**
******************************************************/
void SimpleChildWindow::OnClose( wxCloseEvent &event )
{
	assert( view );
	if ( ! view->queryClose() )
	{
		event.Veto();
	}
	else ChildWindow::OnClose( event );
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnDataChanged
**
******************************************************/
void SimpleChildWindow::OnDataChanged()
{
	view->OnDataChanged();
	setTitle();
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnActivate
**
******************************************************/
void SimpleChildWindow::OnActivate( wxActivateEvent &event )
{
	// attention: wx3.2 GTK will cause infinite loop of child focus events if the line below is uncommented
	//if ( view )	view->SetFocus();
	ChildWindow::OnActivate( event );
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnSize
**
******************************************************/
void SimpleChildWindow::OnSize( wxSizeEvent &event )
{
	wxSize size = event.GetSize();
	if ( view ) IdConverter::get()->setConfigSizeForViewId( view->getViewId(), size, props->isVedic());
	event.Skip();
}

/**************************************************************
***
**   ChildWindowFactory   ---   createPartnerWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createPartnerWindow( wxFrame *parent )
{
	IdConverter *idc = IdConverter::get();
	const wxSize size = idc->getSizeForViewId( VIEW_PARTNER, config->preferVedic );
	const BitmapId bmpId = idc->viewId2BitmapId( VIEW_PARTNER );

	SimpleChildWindow *child = new SimpleChildWindow( parent, 0, bmpId, size , false );
	BasicView *mview = ViewFactory().createPartnerView( child, child );
	mview->doLayout();
	child->insertView( mview );
	return child;
}

/**************************************************************
***
**   ChildWindowFactory   ---   createSingleMainWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createSingleMainWindow( wxFrame *parent, Document *doc )
{
	WidgetNode node( CMD_NEW_MAINVIEW );

	IdConverter *idc = IdConverter::get();
	const wxSize size = idc->getSizeForViewId( node.viewId, node.vedic );
	const BitmapId bmpId = idc->viewId2BitmapId( node.viewId, node.vedic );
	SimpleChildWindow *child = new SimpleChildWindow( parent, doc, bmpId, size, true );

	BasicView *view = ViewFactory().createView( child, child, &node );
	assert( view );
	child->insertView( view );
	view->doLayout();
	doc->setMainWindow( child );
	return child;
}

/**************************************************************
***
**   ChildWindowFactory   ---   createChild
***
***************************************************************/
ChildWindow *ChildWindowFactory::createChild( wxFrame *parent, Document *doc, const int id )
{
	WidgetNode node( id );

	IdConverter *idc = IdConverter::get();
	const wxSize size = idc->getSizeForViewId( node.viewId, node.vedic );
	const BitmapId bmpId = idc->viewId2BitmapId( node.viewId, node.vedic, (Varga)node.varga );

	SimpleChildWindow *child = new SimpleChildWindow( parent, doc, bmpId, size, false );
	child->getProps()->setVedic( node.vedic );
	assert( child );

	BasicView *view = ViewFactory().createView( child, child, &node );
	child->insertView( view );
	view->doLayout();

	return child;
}


