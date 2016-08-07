/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ApplicationWindow.h
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

#ifndef _APPLICATIONWINDOW_H_
#define _APPLICATIONWINDOW_H_

#include <wx/app.h>
#include <wx/frame.h>

class ApplicationWindow;
class AppMenuBar;
class ChildWindow;
class ChildWindowListCtrl;
class Document;
class DocumentManager;
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
class ApplicationWindow : public wxFrame
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
	void OnDocSavedEvent( wxCommandEvent& );
	void OnDocUpdateEvent( wxCommandEvent& );
	void OnCreateEntryChart( wxCommandEvent& );
	void OnIdle( wxIdleEvent& );

	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent& );

	void echo( const wxChar* );

	DocumentManager *docmanager;
	MainWindowStatusbar *statusbar;
	ChildWindowListCtrl *listctrl;
	AppMenuBar *menubar;
	bool docommands, updateTools;

	wxSplitterWindow *logsplitter;
};

#endif

