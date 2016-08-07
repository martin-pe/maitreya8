/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasImportDialog.cpp
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

#include "AtlasImportDialog.h"

#include "AtlasImporter.h"
#include "FileConfig.h"
#include "guibase.h"

#include <wx/button.h>
#include <wx/filename.h>
#include <wx/gauge.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>

// begin wxGlade: ::extracode
// end wxGlade

enum { AI_SQLFILE = wxID_HIGHEST + 1, AI_DBFILE, AI_RUN, AI_ABORT, AI_TIMER };

#define TIMER_PERIOD_MILLISEC  100

/*************************************************//**
*
*  AtlasImportDialog   ---   Constructor
*
******************************************************/
AtlasImportDialog::AtlasImportDialog(wxWindow* parent, wxString title )
 : wxDialog( parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
    // begin wxGlade: AtlasImportDialog::AtlasImportDialog
    panel_gauge = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxRAISED_BORDER|wxTAB_TRAVERSAL);
    gauge = new wxGauge(panel_gauge, wxID_ANY, 100);
    text_status = new wxStaticText(panel_gauge, wxID_ANY, _("dummy"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	close_called = false;
	timer = new wxTimer( this, AI_TIMER );
	timer->Start( TIMER_PERIOD_MILLISEC );

	importer = new AtlasImporter;

	if ( ! title.IsEmpty() ) SetTitle( title );
	SetSize( 600, -1 );

	Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasImportDialog::OnCancel ));
	Connect( AI_TIMER, wxEVT_TIMER, wxTimerEventHandler( AtlasImportDialog::OnTimer ));
}

/*************************************************//**
*
*  AtlasImportDialog   ---   Destructor
*
******************************************************/
AtlasImportDialog::~AtlasImportDialog()
{
	if ( timer->IsRunning()) timer->Stop();
	delete importer;
}

/*****************************************************
**
**   AtlasImportDialog   ---   run
**
******************************************************/
bool AtlasImportDialog::run()
{
	// testen, ob Dateien ok
	if ( ! checkFiles()) return false;

	importer->setImportFile( sqlfile );
	importer->setDatabaseFile( dbfile );
	importer->run();
	return true;
}

/*****************************************************
**
**   AtlasImportDialog   ---   getErrorMessage
**
******************************************************/
wxString AtlasImportDialog::getErrorMessage()
{
	return importer->writeErrorStatus();
}

/*****************************************************
**
**   AtlasImportDialog   ---   checkFiles
**
******************************************************/
bool AtlasImportDialog::checkFiles()
{
	return true;
}

/*****************************************************
**
**   AtlasImportDialog   ---   setDefaultFiles
**
******************************************************/
bool AtlasImportDialog::setDefaultFiles()
{
	FileConfig *fc = FileConfig::get();
	return setData( fc->getAtlasSqlFile(), importer->getDefaultDbFilename());
}

/*****************************************************
**
**   AtlasImportDialog   ---   setData
**
******************************************************/
bool AtlasImportDialog::setData( wxString sqlfile, wxString dbfile )
{
	this->sqlfile = sqlfile;
	this->dbfile = dbfile;
	return checkFiles();
	return true;
}

/*****************************************************
**
**   AtlasImportDialog   ---   OnTimer
**
******************************************************/
void AtlasImportDialog::OnTimer( wxTimerEvent& )
{
	gauge->Pulse();

	if ( importer->hasErrors())
	{
		if ( ! close_called ) EndModal( wxID_ABORT );
		close_called = true;
	}
	else if ( importer->isCanceled())
	{
		if ( ! close_called ) EndModal( wxID_CANCEL );
		close_called = true;
	}
	else if ( importer->isFinished())
	{
		if ( ! close_called ) EndModal( wxID_OK );
		close_called = true;
	}
	else
	{
		text_status->SetLabel( importer->writeRunStatus());
	}
}

/*****************************************************
**
**   AtlasImportDialog   ---   OnRun
**
******************************************************/
void AtlasImportDialog::OnRun( wxCommandEvent& )
{
	importer->run();
}

/*****************************************************
**
**   AtlasImportDialog   ---   OnCancel
**
******************************************************/
void AtlasImportDialog::OnCancel( wxCommandEvent& )
{
	importer->abort();
	if ( ! close_called ) EndModal( wxID_CANCEL );
	close_called = true;
}

/*************************************************//**
*
*  AtlasImportDialog   ---   set_properties
*
******************************************************/
void AtlasImportDialog::set_properties()
{
    // begin wxGlade: AtlasImportDialog::set_properties
    SetTitle(_("Atlas Data Import"));
    gauge->SetMinSize(wxSize(300, -1));
    // end wxGlade
}

/*************************************************//**
*
*  AtlasImportDialog   ---   do_layout
*
******************************************************/
void AtlasImportDialog::do_layout()
{
    // begin wxGlade: AtlasImportDialog::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_gauge = new wxBoxSizer(wxVERTICAL);
    sizer_gauge->Add(gauge, 0, wxALL|wxEXPAND, 3);
    sizer_gauge->Add(text_status, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    panel_gauge->SetSizer(sizer_gauge);
    sizer_main->Add(panel_gauge, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(button_cancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

