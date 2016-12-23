/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ApplicationWindow.cpp
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

#include "ApplicationWindow.h"

#include <wx/choicdlg.h>
#include <wx/cmdline.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/html/htmlwin.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/sysopt.h>
#include <wx/textctrl.h>

#include "AboutDialog.h"
#include "AtlasDialog.h"
#include "ChildWindow.h"
#include "Commands.h"
#include "Conf.h"
#include "ConfigDialog.h"
#include "maitreya.h"
#include "Document.h"
#include "FileConfig.h"
#include "IdConverter.h"
#include "ImageProvider.h"
#include "ChildWindowListCtrl.h"
#include "MenuProvider.h"
#include "MultipleViewConfig.h"
#include "SingletonLifeline.h"
#include "Statusbar.h"
#include "ToolPanel.h"

Config *config;

IMPLEMENT_APP( MaitreyaApp );
IMPLEMENT_CLASS( ApplicationWindow, wxFrame )

// some builds have debug asserts enabled even in release mode
//#define SHOW_APP_DEBUG_ASSERTS

/*****************************************************
**
**   MaitreyaApp   ---   Constructor
**
******************************************************/
MaitreyaApp::MaitreyaApp()
		: wxApp()
{
	frame = (ApplicationWindow*)NULL;
	nbDebugAsserts = 0;
}

/*****************************************************
**
**   MaitreyaApp   ---   OnInit
**
******************************************************/
bool MaitreyaApp::OnInit()
{
	wxString s;
	int startarg = 1;

	::wxInitAllImageHandlers(); // maybe without gif handler

	SetAppName( APP_NAME );
	SetVendorName( VENDOR_NAME );

	s = wxT( "." );

#ifndef _WX_V2_
#ifndef SHOW_APP_DEBUG_ASSERTS
	wxDisableAsserts();
#endif
#endif

#if defined( __WXMSW__)

	if ( argc > 1 )
	{
		s = argv[1];
		startarg = 2;
	}

#elif defined( __WXMAC__ )
	wxSystemOptions::SetOption( wxT( "mac.listctrl.always_use_generic" ), 1 );
	s = MAC_RESOURCE_DIRECTORY;

#else  // Gtk, universal, expecting parameter for data directory
	if ( argc <= 1 )
	{
		printf( "WARN: missing parameter for datadir, assuming '..'\n" );
		printf( "Please call shell script wrapper 'maitreya8' from the same directory\n" );
	}
	else
	{
		s = argv[1];
		startarg = 2;
	}
#endif

	wxLog::SetLogLevel( wxLOG_Error );
	FileConfig::get()->init( s );

	wxPoint point = wxPoint( config->viewprefs->pMasterWindow.x, config->viewprefs->pMasterWindow.y );
	frame = new ApplicationWindow( GUI_APP_NAME, point, config->viewprefs->sizes.sMasterWindow );

	frame->Show( true );
	//SetTopWindow(frame);

	if ( argc <= startarg )
	{
		if ( config->openNewDocOnStart ) frame->NewFile( config->multipleView->useMultipleViews, -1 );
	}
	for ( int i = startarg; i < argc; i++ )
	{
		frame->OpenFile( argv[i] );
	}
#ifdef __WXMSW__
	frame->DragAcceptFiles( true );
#endif
	return true;
}

//#define REPORT_KEY_EVENT

/*****************************************************
**
**   MaitreyaApp   ---   FilterEvent
**
******************************************************/
int MaitreyaApp::FilterEvent( wxEvent& event )
{
#ifdef REPORT_KEY_EVENT
	static int i = 0;
#endif

	bool done = false;
	bool doProcess = false;
	//bool wantsArrowKeys = false;
	bool wantsArrowKeys = true;

	if ( event.GetEventType() == wxEVT_KEY_DOWN )
	{
		wxObject *obj = event.GetEventObject();
		if ( obj )
		{
			event.SetEventObject( obj );

#ifdef REPORT_KEY_EVENT
			wxString name = obj->GetClassInfo()->GetClassName();
			printf( "%d key event from ", i++ );
			PrintLn( name );
#endif

			if ( wxDynamicCast( obj, KeyEventObserver ))
			{
#ifdef REPORT_KEY_EVENT
				printf( "KeyEventObserver\n" );
#endif
				doProcess = true;
				//wantsArrowKeys = ((KeyEventObserver*)obj)->wantsArrowKeys();
			}

			if ( wxDynamicCast( obj, wxHtmlWindow ))
			{
#ifdef REPORT_KEY_EVENT
				printf( "wxHtmlWindow\n" );
#endif
				doProcess = true;
			}

			// Workaround: wx3 click in ChildWindowListCtrl has wxListCtrl as source
			if ( wxDynamicCast( obj, wxListCtrl ))
			{
#ifdef REPORT_KEY_EVENT
				printf( "wxListCtrl\n" );
#endif
				doProcess = true;
			}

			// Workaround: notebook proxy
			if ( wxDynamicCast( obj,  wxNotebook ))
			{
#ifdef REPORT_KEY_EVENT
				printf( "wxNotebook\n" );
#endif
				doProcess = true;
			}
		}
		else printf( "WARN: NULL object in MaitreyaApp::FilterEvent\n" );

		wxEvent *e = &event;
	  wxKeyEvent *k = (wxKeyEvent*)e;

		if ( doProcess )
		{
			done = CommandHandler::get()->processKeyEvent( k, frame, wantsArrowKeys );
			//done = CommandHandler::get()->processKeyEvent( k, (wxWindow*)obj, wantsArrowKeys );
		}
	}
	//printf( "done %d\n", done );

  //enum { Event_Skip = -1, Event_Ignore = 0, Event_Processed = 1 };
	if ( done ) return 1; // Event_Processed; 
	else return -1; //Event_Skip;
}

/*****************************************************
**
**   MaitreyaApp   ---   OnAssertFailure
**
******************************************************/
// avoid compiler warnings about unsed params
#ifdef SHOW_APP_DEBUG_ASSERTS
void MaitreyaApp::OnAssertFailure( const wxChar *file, int	line, const wxChar *func, const wxChar *cond, const wxChar *msg )
{
	wxApp::OnAssertFailure( file, line, func, cond, msg );
#else
void MaitreyaApp::OnAssertFailure( const wxChar*, int, const wxChar*, const wxChar*, const wxChar* )
{
#endif

	nbDebugAsserts++;

	/*
	static int count = 0;
	printf( "TODO: debug handler disabled %d\n", count++ );

	wxString s;
	s = wxString::Format( wxT( "File %s line %d func %s cond %s msg %s" ), file, line, func, cond, msg );
	PrintLn( s );
*/
}

/*****************************************************
**
**   MaitreyaApp   ---   OnExit
**
******************************************************/
int MaitreyaApp::OnExit()
{
	if ( nbDebugAsserts > 0 ) printf( "%d debug asserts\n", nbDebugAsserts );
	SingletonLifeline().cleanup();
	config->writeAll();
	delete config;
	return 0;
}

/*****************************************************
**
**   ApplicationWindow   ---   Constructor
**
******************************************************/
ApplicationWindow::ApplicationWindow(const wxChar *title, const wxPoint &pos, const wxSize &size )
		: wxFrame( 0, -1, title, pos, size )
{
	IdConverter *idc = IdConverter::get();
	wxString rfiles, s;
	listctrl = 0;
	logsplitter = (wxSplitterWindow*)NULL;

	docommands = true;

	DocumentManager::get()->subscribe( this );

	switch ( config->view->logMode )
	{
		case 1:
			// wxSize ds = wxGetDisplaySize();
			wxLog::SetActiveTarget( new wxLogWindow( this, wxT( "Log Window" )));
		break;
		case 2:
		{
			logsplitter = new wxSplitterWindow( this );
			wxTextCtrl * logText = new wxTextCtrl( logsplitter, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
			wxLog::SetActiveTarget( new wxLogTextCtrl( logText ));
			listctrl = new ChildWindowListCtrl( logsplitter, -1 );
			logsplitter->SplitHorizontally( listctrl, logText, config->viewprefs->sashMasterWindowLog );
			wxLogMessage( wxT( "Log Window" ));
		}
		break;
		case 3:
			(void)new wxLogNull();
		break;
#if wxUSE_STD_IOSTREAM && ! defined __WXMSW__
		case 4:
			wxLog::SetActiveTarget( new wxLogStream( &cout ));
		break;
#endif
		case 0:
		default:
			// Log GUI
		break;
	}
	wxLog::SetLogLevel( idc->logMode2WxLogLevel( config->view->logLevel ));

	if ( ! listctrl ) listctrl = new ChildWindowListCtrl( this, -1 );
	SetIcon( ImageProvider::get()->getIcon( BM_MAITREYA  ));

	menubar = new AppMenuBar( wxMB_DOCKABLE );
	SetMenuBar( menubar );

	if ( config->viewprefs->showMainToolbar ) recreateToolbar();

	statusbar = (MainWindowStatusbar*)CreateStatusBar();
	showStatusBar();

#if defined( __WXMAC__ )
	// Otherwise window will grow on each startup
	SetSize( size );
#endif

	Connect( CMD_FIRST+1, CMD_GRAPHIC_STYLE+30, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnChildCommand ));
	Connect( APP_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileNew ));
	Connect( APP_NEW_SINGLE, APP_NEW_MULTIPLE + 100, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileNew ));
	Connect( APP_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileOpen ));
	Connect( APP_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileQuit ));

	Connect( APP_TOGGLE_STATUSBAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnToggleStatusBar ));
	Connect( APP_TOGGLE_MAINTOOLBAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnToggleToolBars ));
	Connect( APP_SAVECONFIG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnSaveConfig ));
	Connect( APP_CONFIGURE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnConfigure ));
	Connect( APP_ATLASDIALOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnLocationDialog ));
	Connect( APP_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnAppAbout ));
	Connect( APP_HELP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnAppHelp ));

	Connect( APP_LIST_LARGEICONS, APP_LIST_TABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnToggleIcons ));
	Connect( APP_HORA, APP_YOGAEDITOR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnNewChild ));
	Connect( APP_FILERECENT, APP_FILERECENT+35, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnOpenRecent ));

	Connect( wxID_ANY, CONFIG_CHANGED, wxCommandEventHandler( ApplicationWindow::OnConfigChanged ));
	Connect( wxID_ANY, CONFIG_TOOLBAR_CHANGED, wxCommandEventHandler( ApplicationWindow::OnToolbarConfigChanged ));
	Connect( wxID_ANY, CHILD_SETACTIVE, wxCommandEventHandler( ApplicationWindow::OnChildFocusEvent ));
	Connect( wxID_ANY, CHILD_CLOSED, wxCommandEventHandler( ApplicationWindow::OnChildCloseEvent ));
	Connect( wxID_ANY, CREATE_ENTRY_CHART, wxCommandEventHandler( ApplicationWindow::OnCreateEntryChart ));

	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ApplicationWindow::OnClose ));
	Connect( wxEVT_DROP_FILES, wxDropFilesEventHandler( ApplicationWindow::OnDropFilesEvent ));
	Connect( LISTCTRL_ID, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ApplicationWindow::OnListItemSelected ));
	Connect( wxEVT_IDLE, wxIdleEventHandler( ApplicationWindow::OnIdle ));

	updateTools = true;

}

/*****************************************************
**
**   ApplicationWindow   ---   Destructor
**
******************************************************/
ApplicationWindow::~ApplicationWindow()
{
	DocumentManager::get()->unsubscribe( this );
	//delete config;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnOpenRecent
**
******************************************************/
void ApplicationWindow::OnOpenRecent( wxCommandEvent &event )
{
	wxLogMessage( wxT( "ApplicationWindow::OnOpenRecent eventid %d" ), event.GetId() );
	OpenFile( menubar->getRecentFilesEntry( event.GetId()) );
	updateTools = true;
}


/*****************************************************
**
**   ApplicationWindow   ---   SetActiveChild
**
******************************************************/
void ApplicationWindow::SetActiveChild( ChildWindow *child )
{
	updateTools = true;
	if ( ! listctrl->getNumberOfEntries() ) return;
	for ( long i = 0; i < listctrl->getNumberOfEntries(); i++ )
	{
		if ( listctrl->getChild(i) == child )
		{
			listctrl->selectItem(i );
			return;
		}
	}
//	assert( 0 );
}

/*****************************************************
**
**   ApplicationWindow   ---   OnIdle
**
******************************************************/
void ApplicationWindow::OnIdle( wxIdleEvent& )
{
	if ( ! updateTools ) return;

	bool isDoc = false;
	docommands = false;

	ChildWindow *child = listctrl->getActiveChild();
	menubar->updateMenus( child );

	if ( child )
	{
		if ( child->getDoc() ) isDoc = true;
	}

	wxToolBarBase *toolbar = GetToolBar();
	if ( toolbar )
	{
		for ( int i = CMD_FIRST + 1; i < CMD_LAST; i++ ) toolbar->EnableTool( i, isDoc );

		/* does not work, all item disable of isDoc is false
		for( uint i = 0; i < config->toolbar->mainToolbarItems.size(); i++ )
		{
			printf( "Toolbar item %d command %d\n", i, config->toolbar->mainToolbarItems[i] );
			toolbar->EnableTool( config->toolbar->mainToolbarItems[i], isDoc );
		}
		*/

		/* GetToolByPos not supported by wx2.8
		for ( size_t i = 0 + 1; i < toolbar->GetToolsCount(); i++ )
		{
			const wxToolBarToolBase *item = toolbar->GetToolByPos( i );
			if ( item ) toolbar->EnableTool( item->GetId(), isDoc );
		}
		*/
	}

	docommands = true;
	updateTools = false;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildClose
**
******************************************************/
void ApplicationWindow::OnChildClose( ChildWindow *child )
{
	listctrl->deleteEntry( child );
	Document *doc = child->getDoc();
	DocumentManager *docmanager = DocumentManager::get();

	//printf( "ApplicationWindow::OnChildClose\n" );

	if ( doc && child == doc->getMainWindow() ) docmanager->deleteDoc( doc );

	if ( listctrl->getNumberOfEntries() > 0 ) listctrl->selectItem( 0 );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnFileNew
**
******************************************************/
void ApplicationWindow::OnFileNew( wxCommandEvent &event )
{
	int viewId = -1;
	printf( "File NEW id %d viewID %d\n", event.GetId(), event.GetId() - APP_NEW_MULTIPLE );

	bool useMultipleView = config->multipleView->useMultipleViews;
	switch ( event.GetId() )
	{
	case APP_NEW:
		break;
	case APP_NEW_SINGLE:
		useMultipleView = false;
		break;
	case APP_NEW_MULTIPLE_CHOICE:
	{
		wxSingleChoiceDialog dialog( this, _( "Select View" ), GUI_APP_NAME, MultipleViewConfigLoader::get()->getNamesAsArray() );
		dialog.SetSize( 300, 600 );
		if ( dialog.ShowModal() != wxID_OK ) return;
		viewId = dialog.GetSelection();
		useMultipleView = true;
	}
		break;
	case APP_NEW_MULTIPLE:
		useMultipleView = true;
		viewId = config->multipleView->defaultView;
		break;
	default:
		useMultipleView = true;
		
		//printf( "ApplicationWindow::OnFileNew NEW Default\n" );
		viewId = event.GetId() - APP_NEW_MULTIPLE;
		break;
	}
	NewFile( useMultipleView, viewId );
}

/*****************************************************
**
**   ApplicationWindow   ---   NewFile
**
******************************************************/
void ApplicationWindow::NewFile( const bool &useMultipleView, const int viewId )
{
	ChildWindowFactory factory;
	DocumentManager *docmanager = DocumentManager::get();

	Document *doc = new Document();
	doc->update();
	docmanager->addDoc( doc );
	ChildWindow *child = factory.createMainWindow( this, doc, useMultipleView, viewId );
	addChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnFileOpen
**
******************************************************/
void ApplicationWindow::OnFileOpen( wxCommandEvent& )
{
	wxString filetypes = wxT( "Maitreya (*.mtx)|*.mtx| Maitreya 2.x (*.atx)|*.atx|Jagannatha Hora (*.jhd)|*.jhd| All files (*.*)|*.*" );
	wxFileDialog openFileDialog( this, _( "Open file"), config->viewprefs->defOpenPath, wxT( "" ), filetypes, wxFD_OPEN, wxDefaultPosition );

	openFileDialog.SetFilterIndex( config->view->defOpenFiletype );
	if (openFileDialog.ShowModal() == wxID_OK )
	{
		// Bugfix 7.0.3 GetDirectory returns home directory instead of selected dir
		//if ( OpenFile( openFileDialog.GetDirectory() + wxFileName::GetPathSeparator()  + openFileDialog.GetFilename() ))
		if ( OpenFile( openFileDialog.GetPath() ))
		{
			config->viewprefs->defOpenPath = openFileDialog.GetDirectory();
			config->view->defOpenFiletype = openFileDialog.GetFilterIndex();
		}
	}
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OpenFile
**
******************************************************/
bool ApplicationWindow::OpenFile( wxString filename )
{
	wxLogMessage( wxT( "ApplicationWindow::OpenFile %s" ), filename.c_str() );

	Document *doc = new Document();
	if ( ! doc->openFile( filename ))
	{
		delete doc;
		doMessageBox( this,
			wxString::Format( wxT( "%s %s" ), _( "Cannot open file" ), filename.c_str() ),
			wxOK | wxICON_ERROR );
		return false;
	}
	menubar->addToRecentFiles( filename );
	CreateNewChild( doc );
	updateTools = true;
	return true;
}

/*****************************************************
**
**   ApplicationWindow   ---   CreateNewChild
**
******************************************************/
void ApplicationWindow::CreateNewChild( Document *doc )
{
	DocumentManager *docmanager = DocumentManager::get();

	doc->update();
	docmanager->addDoc( doc );
	ChildWindow *child = ChildWindowFactory().createMainWindow( this, doc, config->multipleView->useMultipleViews, -1 );
	addChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnFileQuit
**
******************************************************/
void ApplicationWindow::OnFileQuit( wxCommandEvent& )
{
	Close( true );
}

/*****************************************************
**
**   ApplicationWindow   ---   addChild
**
******************************************************/
void ApplicationWindow::addChild( ChildWindow *child )
{
	if ( child == 0 )
	{
		printf( "Error: child is null!\n" );
		return;
	}
	listctrl->addEntry( child );
	SetActiveChild( child );
	child->Show( true );
	child->postCreate();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnNewChild
**
******************************************************/
void ApplicationWindow::OnNewChild( wxCommandEvent &event )
{
	ChildWindow *child = 0;
	//printf( "ApplicationWindow::OnNewChild %d\n", event.GetId() );
	ChildWindowFactory factory;

	switch ( event.GetId() )
	{
	case APP_PARTNER:
		child = factory.createPartnerWindow( this );
		break;
	default:
		child = factory.createChild( this, 0, event.GetId() );
		break;
	}
	addChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildCommand
**
******************************************************/
void ApplicationWindow::OnChildCommand( wxCommandEvent &event )
{
	updateTools = true;
	int id = event.GetId();
	wxObject *obj = event.GetEventObject();
	if ( obj )
	{
		wxString classname = obj->GetClassInfo()->GetClassName();
		printf( "ApplicationWindow::OnChildCommand %d object %s\n", id, str2char( classname ));
	}
	if ( ! docommands ) return;
	ChildWindow *child = listctrl->getActiveChild();

	if ( ! child )
	{
		wxLogMessage( wxT( "ApplicationWindow::OnChildCommand No active child" ));
		return;
	}
	if ( id < CMD_NEW_TEXT || id >= CMD_GRAPHIC_STYLE )
	{
		child->OnCommand( event );
		if ( id == CMD_EDITDATA ) listctrl->updateItemLabels();
	}
	else
	{
		// avoid that document dependent view is opened based upon non doc view, ugly but works
		if ( child->getDoc() == 0 &&
		        ( id == CMD_NEW_TEXT || id == CMD_NEW_TRANSIT || id == CMD_NEW_SBC || id == CMD_NEW_SOLAR
		          || id == CMD_NEW_YOGA || id == CMD_NEW_GRAPHICALDASA
							|| id == CMD_NEW_DASA_TREE || id == CMD_NEW_DASA_COMPOSITE
							|| id == CMD_NEW_WCHART || id == CMD_NEW_VARGA || id == CMD_NEW_URANIAN
							|| id == CMD_NEW_URANIAN_CHART || id == CMD_NEW_MAINVIEW || id >= CMD_NEW_RASI )) return;

		// may be done now
		addChild( ChildWindowFactory().createChild( this, child->getDoc(), id ));
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   prepareSaveConfig
**
******************************************************/
void ApplicationWindow::prepareSaveConfig()
{
	wxLogMessage( wxT( "ApplicationWindow::prepareSaveConfig" ));

	if ( logsplitter ) config->viewprefs->sashMasterWindowLog = logsplitter->GetSashPosition();

	wxRect rect = GetRect();
	config->viewprefs->pMasterWindow = wxSize( rect.x, rect.y );
	config->viewprefs->sizes.sMasterWindow = wxSize( rect.width, rect.height );
	menubar->saveRecentFiles();
	//config->writeAll();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnClose
**
******************************************************/
void ApplicationWindow::OnClose( wxCloseEvent& )
{
	wxLogMessage( wxT( "Start ApplicationWindow::OnClose" ));
	long i;
	DocumentManager *docmanager = DocumentManager::get();

	updateTools = true;
	if ( config->askOnQuit )
	{
		if ( doMessageBox( this, _( "Do you want to quit?" ), wxYES_NO | wxCENTRE | wxICON_QUESTION ) == wxID_NO ) return;
	}
	wxLogMessage( wxT( "ApplicationWindow::OnClose -- Closing" ));

	if ( docmanager->queryClose())
	{
		for ( i = 0; i < listctrl->getNumberOfEntries(); i++ )
		{
			if ( ! listctrl->getChild( i )->Close()) return;
		}
		prepareSaveConfig();
		Destroy();
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   OnSaveConfig
**
******************************************************/
void ApplicationWindow::OnSaveConfig( wxCommandEvent& )
{
	prepareSaveConfig();
	wxString filename = FileConfig::get()->getConfigFilename();

	config->writeAll();
	doMessageBox( this, wxString::Format( _( "Configuration saved to file %s." ), filename.c_str()));
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToggleIcons
**
******************************************************/
void ApplicationWindow::OnToggleIcons( wxCommandEvent &event )
{
	config->viewprefs->listStyle = event.GetId() - APP_LIST_LARGEICONS;
	listctrl->recreate();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnConfigure
**
******************************************************/
void ApplicationWindow::OnConfigure( wxCommandEvent& )
{
	wxLogMessage( wxT( "ApplicationWindow::OnConfigure" ));
	ConfigDialog( this ).ShowModal();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnLocationDialog
**
******************************************************/
void ApplicationWindow::OnLocationDialog( wxCommandEvent& )
{
	AtlasDialogWrapper wrapper( this, ATLASDIALOG_CALLER_APPLICATIONWINDOW );
	wrapper.run();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnAppHelp
**
******************************************************/
void ApplicationWindow::OnAppHelp( wxCommandEvent& )
{
	if ( ! wxLaunchDefaultBrowser( SARAVALI_HELP_URL )) doMessageBox( this, _( "Cannot launch Browser." ) );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnAppAbout
**
******************************************************/
void ApplicationWindow::OnAppAbout( wxCommandEvent& )
{
	AboutDialog( this ).ShowModal();
	updateTools = true;
}

/**************************************************************
***
**  ApplicationWindow   ---   OnCreateStatusBar
***
***************************************************************/
// this method is internally used by wxWidgets
wxStatusBar *ApplicationWindow::OnCreateStatusBar( int, long, wxWindowID, const wxString& )
{
	return new MainWindowStatusbar( this );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   showStatusBar
**
******************************************************/
void ApplicationWindow::showStatusBar()
{
	//wxCoord w, h;
	statusbar->StartPlanetTimer( config->viewprefs->showStatusBar && config->view->showStatusInfo );
	statusbar->Show( config->viewprefs->showStatusBar );
	// Workaround
	/*
	GetSize( &w, &h );
	SetSize( w+1, h );
	SetSize( w, h );
	*/
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToggleStatusBar
**
******************************************************/
void ApplicationWindow::OnToggleStatusBar( wxCommandEvent &event )
{
	config->viewprefs->showStatusBar = event.IsChecked();
	showStatusBar();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToggleToolBars
**
******************************************************/
void ApplicationWindow::OnToggleToolBars( wxCommandEvent &event )
{
	config->viewprefs->showMainToolbar = event.IsChecked();
	if ( config->viewprefs->showMainToolbar )
	{
		recreateToolbar();
	}
	else
	{
		wxToolBar *toolbar = GetToolBar();
		delete toolbar;
		SetToolBar( NULL );
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   recreateToolbar
**
******************************************************/
void ApplicationWindow::recreateToolbar()
{
	ToolbarFactory *factory = ToolbarFactory::get();

	wxToolBar *toolbar = GetToolBar();
	delete toolbar;
	SetToolBar( NULL );

	printf( "ApplicationWindow::recreateToolbar style i %d\n", factory->getToolbarStyle());
	toolbar = CreateToolBar( factory->getToolbarStyle());
	factory->populateMainToolbar( toolbar );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToolbarConfigChanged
**
******************************************************/
void ApplicationWindow::OnToolbarConfigChanged( wxCommandEvent& )
{
	if ( config->viewprefs->showMainToolbar )
	{
		recreateToolbar();
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   OnDropFilesEvent
**
******************************************************/
void ApplicationWindow::OnDropFilesEvent( wxDropFilesEvent &event )
{
	wxLogMessage( wxT( "ApplicationWindow::OnDropFilesEvent" ));
	for ( int i = 0; i < event.GetNumberOfFiles(); i++ )
	{
		wxLogMessage( wxT( "ApplicationWindow::OnDropFilesEvent open %s"), (event.GetFiles())[i].c_str() );
		OpenFile( (event.GetFiles())[i] );
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   OnConfigChanged
**
******************************************************/
void ApplicationWindow::OnConfigChanged( wxCommandEvent& )
{
	wxLogMessage( wxT( "ApplicationWindow::OnConfigChanged" ));

	DocumentManager *docmanager = DocumentManager::get();
	docmanager->updateDocsAndChildren();

	listctrl->onConfigChanged();
	if ( config->viewprefs->showStatusBar )
		((MainWindowStatusbar*)GetStatusBar())->StartPlanetTimer( config->view->showStatusInfo );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildFocusEvent
**
******************************************************/
void ApplicationWindow::OnChildFocusEvent( wxCommandEvent &event )
{
	ChildWindow *child = (ChildWindow*)event.GetEventObject();
	SetActiveChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildCloseEvent
**
******************************************************/
void ApplicationWindow::OnChildCloseEvent( wxCommandEvent &event )
{
	ChildWindow *child = (ChildWindow*)event.GetEventObject();
	OnChildClose( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnNewChildEvent
**
******************************************************/
void ApplicationWindow::OnNewChildEvent( wxCommandEvent &event )
{
	assert( 0 );
	ChildWindow *child = (ChildWindow*)event.GetEventObject();
	addChild( child );
	echo( wxT( "New child created" ));
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnListItemSelected
**
******************************************************/
void ApplicationWindow::OnListItemSelected( wxListEvent& event )
{
	listctrl->setActiveItem( event.GetIndex() );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   DocumentListener im implementations
**
******************************************************/
void ApplicationWindow::notifyDocumentChanged( Document* )
{
	listctrl->updateItemLabels();
	updateTools = true;
}

void ApplicationWindow::notifyDocumentSaved( Document *doc )
{
	printf( "ApplicationWindow::notifyDocumentSaved\n" );
	menubar->addToRecentFiles( ((FileDataSet*)doc->getDataSet())->getFilename() );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnCreateEntryChart
**
******************************************************/
void ApplicationWindow::OnCreateEntryChart( wxCommandEvent &event )
{
	Document *d = (Document*)event.GetEventObject();
	CreateNewChild( d );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnEventTest
**
******************************************************/
void ApplicationWindow::OnEventTest( wxCommandEvent& )
{
	//wxString *s = (wxString*)event.GetEventObject();
}

/*****************************************************
**
**   ApplicationWindow   ---   echo
**
******************************************************/
void ApplicationWindow::echo( const wxChar* )
{
	//((MainWindowStatusbar*)GetStatusBar())->echo( message );
	updateTools = true;
}

