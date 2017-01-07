/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ApplicationWindow.h
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

#ifndef _APPLICATIONWINDOW_H_
#define _APPLICATIONWINDOW_H_

#include "Document.h"

#include <wx/app.h>
#include <wx/frame.h>

class ApplicationWindow;
class AppMenuBar;
class ChildWindow;
class ChildWindowListCtrl;
class MainWindowStatusbar;

class wxEvent;
class wxLog;
class wxListEvent;
class wxSplitterWindow;
class wxTextCtrl;

/*************************************************//**
*
* \brief application class managing life cycle of ApplicationWindow
*
******************************************************/
class MaitreyaApp : public wxApp
{
public:
	MaitreyaApp();

protected:
	ApplicationWindow *frame;
	virtual bool OnInit();
	virtual int OnExit();
	virtual int FilterEvent( wxEvent& );
	virtual void OnAssertFailure( const wxChar *file, int	line, const wxChar *func, const wxChar *cond, const wxChar *msg );

	int nbDebugAsserts;
};

//DECLARE_APP( MaitreyaApp )

/*************************************************//**
*
* \brief main window of appilication
*
******************************************************/
class ApplicationWindow : public wxFrame, public DocumentListener
{
	DECLARE_CLASS( ApplicationWindow )
public:

	ApplicationWindow( const wxChar *title, const wxPoint&, const wxSize& );
	~ApplicationWindow();
	void NewFile( const bool &useMultipleView, const int viewId = 0 );
	bool OpenFile( wxString );
	void CreateNewChild( Document* );

protected:
	void addChild( ChildWindow* );
	void SetActiveChild( ChildWindow* );
	void OnChildClose( ChildWindow* );
	void OnOpenRecent( wxCommandEvent& );
	void OnFileNew( wxCommandEvent& );
	void OnFileOpen( wxCommandEvent& );
	void OnFileQuit( wxCommandEvent& );
	void OnNewChild( wxCommandEvent& );
	void OnChildCommand( wxCommandEvent& );
	void OnSaveConfig( wxCommandEvent& );
	void OnConfigure( wxCommandEvent& );
	void OnLocationDialog( wxCommandEvent& );
	void OnImportLocationDialog( wxCommandEvent& );
	void OnAppAbout( wxCommandEvent& );
	void OnAppHelp( wxCommandEvent& );
	void OnClose( wxCloseEvent& );
	void showStatusBar();

	void OnToggleStatusBar( wxCommandEvent& );
	void OnToggleIcons( wxCommandEvent& );
	void OnConfigChanged( wxCommandEvent& );

	void OnToggleToolBars( wxCommandEvent& );
	void OnToolbarConfigChanged( wxCommandEvent& );
	void recreateToolbar();

	void OnDropFilesEvent( wxDropFilesEvent& );
	wxStatusBar* OnCreateStatusBar(int number, long style, wxWindowID id, const wxString& name);
	void prepareSaveConfig();
	void OnEventTest( wxCommandEvent& );
	void OnListItemSelected( wxListEvent& );
	void OnChildFocusEvent( wxCommandEvent& );
	void OnChildCloseEvent( wxCommandEvent& );
	void OnNewChildEvent( wxCommandEvent& );
	//void OnDocSavedEvent( wxCommandEvent& );
	//void OnDocUpdateEvent( wxCommandEvent& );
	void OnCreateEntryChart( wxCommandEvent& );
	void OnIdle( wxIdleEvent& );

	virtual void notifyDocumentListChanged() {}
	virtual void notifyDocumentChanged( Document* );
	virtual void notifyDocumentSaved( Document* );

	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent& );

	void echo( const wxChar* );

	MainWindowStatusbar *statusbar;
	ChildWindowListCtrl *listctrl;
	AppMenuBar *menubar;
	bool docommands, updateTools;

	wxSplitterWindow *logsplitter;
};

#endif

