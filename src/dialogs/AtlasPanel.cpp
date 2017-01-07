/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasPanel.cpp
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

#include "AtlasPanel.h"

#include "AtlasImportDialog.h"
#include "AtlasDao.h"
#include "AtlasLogic.h"
#include "Conf.h"
#include "FileConfig.h"
#include "guibase.h"
#include "mvalidator.h"

#include <list>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choicdlg.h>
#include <wx/dynarray.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

using namespace std;

// begin wxGlade: ::extracode
// end wxGlade

extern Config *config;
enum { AD_TEXTDBFILE = wxID_HIGHEST + 1, AD_COUNTRIES, AD_CHOOSEDBFILE, AD_TEXTSQLFILE, AD_CHOOSESQLFILE, AD_RUNIMPORT };

IMPLEMENT_CLASS( AtlasPanel, ConfigPanel )

/*****************************************************
**
**   AtlasPanel   ---   Constructor
**
******************************************************/
AtlasPanel::AtlasPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	aconfig = new AtlasConfig();
	config2model();

    // begin wxGlade: AtlasPanel::AtlasPanel
    panel_sqlfilestatus = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_db_staticbox = new wxStaticBox(this, wxID_ANY, _("Database File"));
    sizer_import_staticbox = new wxStaticBox(this, wxID_ANY, _("Database  Import"));
    sizer_favcountries_staticbox = new wxStaticBox(this, wxID_ANY, _("Manage Favourite Countries"));
    panel_status = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    button_dbfile = new wxButton(this, AD_CHOOSEDBFILE, _("Choose ..."));
    text_dbfile = new wxTextCtrl(this, AD_TEXTDBFILE, wxEmptyString);
    text_dbstatus = new wxStaticText(panel_status, wxID_ANY, _("Status"));
    button_sqlfile = new wxButton(this, AD_CHOOSESQLFILE, _("Choose ..."));
    text_sqlfile = new wxTextCtrl(this, AD_TEXTSQLFILE, wxEmptyString);
    text_sqlfilestatus = new wxStaticText(panel_sqlfilestatus, wxID_ANY, _("Status"));
    button_startimport = new wxButton(this, AD_RUNIMPORT, _("Start Import"));
    const wxString *list_countries_choices = NULL;
    list_countries = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, list_countries_choices, 0);
    button_countries = new wxButton(this, AD_COUNTRIES, _("Change Countries ..."));

    set_properties();
    do_layout();
    // end wxGlade

	text_dbfile->SetValidator( MFilenameValidator( &aconfig->databaseFile ));
	text_sqlfile->SetValidator( MFilenameValidator( &aconfig->sqlFile ));

	Connect( AD_CHOOSEDBFILE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasPanel::OnChooseDbFile ));
	Connect( AD_TEXTDBFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AtlasPanel::OnTextChanged ));

	Connect( AD_CHOOSESQLFILE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasPanel::OnChooseSqlFile ));
	Connect( AD_TEXTSQLFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AtlasPanel::OnTextChanged ));
	Connect( AD_RUNIMPORT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasPanel::OnStartImport ));

	Connect( AD_COUNTRIES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasPanel::OnCountries ));

	updateCountryList();
}

/*****************************************************
**
**   AtlasPanel   ---   standard overrides
**
******************************************************/
AtlasPanel::~AtlasPanel() { delete aconfig; }
void AtlasPanel::config2model() { *aconfig = *config->atlas; }
void AtlasPanel::model2config() { *config->atlas = *aconfig; }
void AtlasPanel::restoreDefaults()
{
	*aconfig = AtlasConfig();
	updateCountryList();
}

/*****************************************************
**
**   AtlasPanel   ---   OnChooseDbFile
**
******************************************************/
void AtlasPanel::OnChooseDbFile( wxCommandEvent& )
{
	static const wxString filetypes = wxT( "SQLite database (*.db)|*.db| All files (*.*)|*.*" );
	wxFileName fn( text_dbfile->GetValue() );
	wxFileDialog dialog( this, _( "Select database file"), fn.GetPath(), fn.GetFullName(), filetypes,
		wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition );

	if (dialog.ShowModal() == wxID_OK )
	{
		text_dbfile->SetValue( dialog.GetPath());
		setDirty();
	}
}

/*****************************************************
**
**   AtlasPanel   ---   OnTextChanged
**
******************************************************/
void AtlasPanel::OnTextChanged( wxCommandEvent& )
{
	updateUi();
	setDirty();
}

/*****************************************************
**
**   AtlasPanel   ---   getFileStatus
**
******************************************************/
bool AtlasPanel::getFileStatus( wxString filename, wxString &message )
{
	if ( ! wxFileName::FileExists( filename ))
	{
		message = _( "File not found" );
		return false;
	}
	else if ( ! wxFileName::IsFileReadable( filename ))
	{
		message = _( "File not readable" );
		return false;
	}
	else if ( wxFileName::GetSize( filename ) == 0 )
	{
		message = _( "File size zero" );
		return false;
	}

	message = _( "Ok" );
	return true;
}

/*****************************************************
**
**   AtlasPanel   ---   updateUi
**
******************************************************/
void AtlasPanel::updateUi()
{
	wxString message;

	wxString dbfile = AllTrim( text_dbfile->GetValue());
	bool okstatus = getFileStatus( dbfile, message );

	if ( okstatus )
	{
		AtlasDao dao( dbfile );
		okstatus = dao.isDbStatusOkay();
		if ( ! okstatus ) message = _( "Database error" );
	}
	text_dbstatus->SetLabel( message );
}

/*****************************************************
**
**   AtlasPanel   ---   OnChooseSqlFile
**
******************************************************/
void AtlasPanel::OnChooseSqlFile( wxCommandEvent& )
{
	static const wxString filetypes = wxT( "SQL file (*.sql)|*.sql| ZIP files (*.zip)|*.zip| All files (*.*)|*.*" );

	wxFileName fn( text_sqlfile->GetValue());
	wxFileDialog dialog( this, _( "Select import file"), fn.GetPath(),  fn.GetFullName(),
		filetypes, wxFD_OPEN, wxDefaultPosition );

	if (dialog.ShowModal() == wxID_OK )
	{
		text_sqlfile->SetValue( dialog.GetPath());
	}
	setDirty();
}

/*****************************************************
**
**   AtlasPanel   ---   OnStartImport
**
******************************************************/
void AtlasPanel::OnStartImport( wxCommandEvent& )
{
	AtlasImportDialog importDialog( this );
	bool ok = importDialog.setData( text_sqlfile->GetValue(), text_dbfile->GetValue());
	if ( ok )
	{
		importDialog.run();
		int result = importDialog.ShowModal();
		if ( result == wxID_OK )
		{
			doMessageBox( this, _( "Import was successfull" ));
		}
		else if ( result == wxID_CANCEL )
		{
			doMessageBox( this, _( "Import was canceled" ), wxOK | wxICON_HAND );
		}
		else if ( result == wxID_ABORT )
		{
			wxString message;
			message << _( "Import had errors" ) << Endl
				<< importDialog.getErrorMessage() << Endl;
			doMessageBox( this, message,  wxOK | wxICON_ERROR );
		}
	}
	else
	{
		printf( "Error: file status not okay\n" );
	}
	setDirty();
}

/*****************************************************
**
**   AtlasPanel   ---   updateSqlFileStatus
**
******************************************************/
void AtlasPanel::updateSqlFileStatus()
{
	wxString message;

	wxString sqlfile = AllTrim( text_sqlfile->GetValue());
	bool okstatus = getFileStatus( sqlfile, message );
	text_sqlfilestatus->SetLabel( message );
	button_startimport->Enable( okstatus );
}

/*****************************************************
**
**   AtlasPanel   ---   updateCountryList
**
******************************************************/
void AtlasPanel::updateCountryList()
{
	list_countries->Clear();

	AtlasLogic logic;
	list<AtlasCountry> l = logic.getFavouriteCountries( aconfig->favouriteCountries );
	for ( list<AtlasCountry>::iterator iter = l.begin(); iter != l.end(); iter++ )
	{
		list_countries->Append( (*iter).name );
	}
	list_countries->Refresh();
	button_countries->Enable( l.size() > 0 );
}

/*****************************************************
**
**   AtlasPanel   ---   OnCountries
**
******************************************************/
void AtlasPanel::OnCountries( wxCommandEvent& )
{
	AtlasLogic logic;
	wxArrayInt fav;

	list<AtlasCountry> l = logic.getAllCountries();
	wxArrayString countrylabels;
	countrylabels.Add( wxEmptyString, l.size());

	int i = 0;
	for ( list<AtlasCountry>::iterator iter = l.begin(); iter != l.end(); iter++ )
	{
		countrylabels[i++] = (*iter).name;
	}
	wxMultiChoiceDialog dialog( this,	_( "Choose Favourite Countries" ), GUI_APP_NAME, countrylabels );

	for( uint i = 0; i < aconfig->favouriteCountries.size(); i++ )
	{
		wxString iso = aconfig->favouriteCountries[i];
		int count = 0;
		for ( list<AtlasCountry>::iterator iter = l.begin(); iter != l.end(); iter++ )
		{
			if ( (*iter).iso.CmpNoCase( iso ) == 0 )
			{
				fav.Add( count );
				break;
			}
			count++;
		}

	}
	dialog.SetSelections( fav );

	if ( dialog.ShowModal() == wxID_OK )
	{
		wxArrayInt a = dialog.GetSelections();
		aconfig->favouriteCountries.clear();
		for ( uint i = 0; i < a.Count(); i++ )
		{
			int count = 0;
			for ( list<AtlasCountry>::iterator iter = l.begin(); iter != l.end(); iter++ )
			{
				if ( count++ == a[i] )
				{
					aconfig->favouriteCountries.push_back( iter->iso );
				}
			}
		}
		setDirty();
	}
	updateCountryList();
}

/*****************************************************
**
**   AtlasPanel   ---   set_properties
**
******************************************************/
void AtlasPanel::set_properties()
{
    // begin wxGlade: AtlasPanel::set_properties
    text_dbstatus->SetMinSize(wxSize(-1,-1));
    text_sqlfilestatus->SetMinSize(wxSize(-1,-1));
    list_countries->SetMinSize(wxSize(-1, 200));
    // end wxGlade
}

/*****************************************************
**
**   AtlasPanel   ---   do_layout
**
******************************************************/
void AtlasPanel::do_layout()
{
    // begin wxGlade: AtlasPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_favcountries_staticbox->Lower();
    wxStaticBoxSizer* sizer_favcountries = new wxStaticBoxSizer(sizer_favcountries_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_import_staticbox->Lower();
    wxStaticBoxSizer* sizer_import = new wxStaticBoxSizer(sizer_import_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_sqlfilestatus = new wxBoxSizer(wxHORIZONTAL);
    sizer_db_staticbox->Lower();
    wxStaticBoxSizer* sizer_db = new wxStaticBoxSizer(sizer_db_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_dbstatus = new wxBoxSizer(wxHORIZONTAL);
    sizer_db->Add(button_dbfile, 0, wxALL, 3);
    sizer_db->Add(text_dbfile, 0, wxALL|wxEXPAND, 3);
    sizer_dbstatus->Add(text_dbstatus, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_status->SetSizer(sizer_dbstatus);
    sizer_db->Add(panel_status, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_db, 1, wxALL|wxEXPAND, 3);
    sizer_import->Add(button_sqlfile, 0, wxALL, 3);
    sizer_import->Add(text_sqlfile, 0, wxALL|wxEXPAND, 3);
    sizer_sqlfilestatus->Add(text_sqlfilestatus, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_sqlfilestatus->SetSizer(sizer_sqlfilestatus);
    sizer_import->Add(panel_sqlfilestatus, 1, wxALL|wxEXPAND, 3);
    sizer_import->Add(button_startimport, 0, wxALL, 3);
    sizer_left->Add(sizer_import, 1, wxEXPAND, 0);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    sizer_favcountries->Add(list_countries, 1, wxALL|wxEXPAND, 3);
    sizer_favcountries->Add(button_countries, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_right->Add(sizer_favcountries, 1, wxEXPAND, 0);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    sizer_main->AddGrowableCol(1);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createAtlasPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createAtlasPanel( wxWindow *parent )
{
	return new AtlasPanel( parent );
}


