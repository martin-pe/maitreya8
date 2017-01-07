/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasDialog.cpp
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

#include "AtlasDialog.h"

#include <math.h>

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/grid.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>

#include "AtlasImportDialog.h"
#include "AtlasEditDialog.h"
#include "AtlasLogic.h"
#include "maitreya.h"
#include "Conf.h"
#include "guibase.h"
#include "Lang.h"

extern Config *config;

enum { ATLAS_FILTER = wxID_HIGHEST + 1, ATLAS_COUNTRY, ATLAS_MODE, ATLAS_EDIT, ATLAS_DELETE, ATLAS_ADD, ATLAS_GRID, ATLAS_TIMER };

// Interval for lookup of new location data
#define TIMER_INTERVAL_MILLISEC 50

// begin wxGlade: ::extracode
// end wxGlade

#define ATLAS_GRID_MAXCOLS 5

/*****************************************************
**
**   AtlasDialogWrapper   ---   Constructor
**
******************************************************/
AtlasDialogWrapper::AtlasDialogWrapper( wxWindow *parent, const int &callerId, wxString s  )
	: callerId( callerId ),
	searchString( s ),
	parent( parent )
{
	dialog = 0;
}

/*****************************************************
**
**   AtlasDialogWrapper   ---   Destructor
**
******************************************************/
AtlasDialogWrapper::~AtlasDialogWrapper()
{
	if ( dialog ) delete dialog;
}

/*****************************************************
**
**   AtlasDialogWrapper   ---   run
**
******************************************************/
int AtlasDialogWrapper::run()
{
	bool startdialog = false;

	bool dbok = AtlasDao().isDbStatusOkay();
	
	if ( ! dbok )
	{
		AtlasImportDialog importDialog( parent, _( "First Time Import of Atlas Data" ));
		importDialog.setDefaultFiles();
		importDialog.run();

		int ret = importDialog.ShowModal();
		if ( ret == wxID_OK )
		{
			startdialog = true;
		}
	}
	else
	{
		startdialog = true;
	}
	if ( startdialog )
	{
		dialog = new AtlasDialog( parent, callerId, searchString );
		return dialog->ShowModal();
	}
	return wxID_CANCEL;
}

/*****************************************************
**
**   AtlasDialogWrapper   ---   getSelectedEntry
**
******************************************************/
AtlasEntry *AtlasDialogWrapper::getSelectedEntry()
{
	if ( dialog ) return &dialog->getSelectedEntry();
	else return (AtlasEntry*)NULL;
}

/*****************************************************
**
**   AtlasGridTable
**
******************************************************/
class AtlasGridTable : public wxGridTableBase
{
public:
	AtlasGridTable( wxGrid *view, AtlasLogic *logic )
	{
		SetView( view );
		this->logic = logic;
	}

	virtual wxString GetValue(int row, int col)
	{
		Formatter *formatter = Formatter::get();
		AtlasEntry *entry = logic->getEntryByRowId( row );
		if ( entry )
		{
			switch ( col )
			{
			case 0:
				return entry->name;
				break;
			case 1:
				return entry->country;
				break;
			case 2:
				return entry->admin;
				break;
			case 3:
				return formatter->getLatitudeFormatted( entry->getLatitude() );
				break;
			case 4:
				return formatter->getLongitudeFormatted( entry->getLongitude() );
				break;
			default:
				return wxT( "?" );
				break;
			}
		}
		else
		{
			return col == 0 ? wxT( "..." ) : wxEmptyString;
		}
		//return wxT( "..." );
	}

	virtual bool AppendRows(size_t numRows = 1)
	{
		if ( GetView() )
		{
			wxGridTableMessage msg( this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, numRows );
			GetView()->ProcessTableMessage( msg );
		}
		return true;
	}

	virtual bool DeleteRows(size_t pos = 0, size_t numRows = 1)
	{
		if ( GetView() )
		{
			wxGridTableMessage msg( this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, pos, numRows );
			GetView()->ProcessTableMessage( msg );
		}

		return true;
	}

	virtual int GetNumberRows() {
		return ATLAS_MAX_GRID_ELEMENTS;
	}
	virtual int GetNumberCols() {
		return ATLAS_GRID_MAXCOLS;
	}

	virtual bool IsEmptyCell(int, int) {
		return false;
	}
	virtual void SetValue(int, int, const wxString&) {}

	virtual wxString GetColLabelValue( int col )
	{
		switch ( col )
		{
		case 0:
			return _( "City" );
			break;
		case 1:
			return _( "Country" );
			break;
		case 2:
			return _( "Admin Division" );
			break;
		case 3:
			return _( "Latitude" );
			break;
		case 4:
			return _( "Longitude" );
			break;
		default:
			return wxT( "Unknown" );
			break;
		}
	}

private:
	AtlasLogic *logic;
};

/*****************************************************
**
**   AtlasDialog   ---   Constructor
**
******************************************************/
AtlasDialog::AtlasDialog(wxWindow* parent, const int &callerId, wxString searchString )
		:  wxDialog(parent, -1, wxT( "Atlas" ),  wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE ),
		callerId( callerId )
{
	logic = new AtlasLogic( true );

    // begin wxGlade: AtlasDialog::AtlasDialog
    sizer_details_staticbox = new wxStaticBox(this, wxID_ANY, _("Location"));
    sizer_actions_staticbox = new wxStaticBox(this, wxID_ANY, _("Actions"));
    sizer_filter_staticbox = new wxStaticBox(this, wxID_ANY, _("Filter"));
    label_filtername = new wxStaticText(this, wxID_ANY, _("Name"));
    filter_edit = new wxTextCtrl(this, ATLAS_FILTER, wxEmptyString);
    label_filtercountry = new wxStaticText(this, wxID_ANY, _("Country"));
    const wxString country_choice_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    country_choice = new wxChoice(this, ATLAS_COUNTRY, wxDefaultPosition, wxDefaultSize, 4, country_choice_choices, 0);
    label_mode = new wxStaticText(this, wxID_ANY, _("Mode"));
    const wxString choice_mode_choices[] = {
        _("Name"),
        _("ASCII"),
        _("Alias")
    };
    choice_mode = new wxChoice(this, ATLAS_MODE, wxDefaultPosition, wxDefaultSize, 3, choice_mode_choices, 0);
    locgrid = new wxGrid(this, wxID_ANY);
    match_label = new wxStaticText(this, wxID_ANY, _("dummy"));
    static_line_match = new wxStaticLine(this, wxID_ANY);
    label_name = new wxStaticText(this, wxID_ANY, _("Name"));
    text_name = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_longitude = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_longitude = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_asciiname = new wxStaticText(this, wxID_ANY, _("ASCII Name"));
    text_asciiname = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_latitude = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_latitude = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_country = new wxStaticText(this, wxID_ANY, _("Country"));
    text_country = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_timezone = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    text_timezone = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_admincode = new wxStaticText(this, wxID_ANY, _("Admin Code"));
    text_admincode = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_tzoffset = new wxStaticText(this, wxID_ANY, _("TZ Hours"));
    text_tzoffset = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_population = new wxStaticText(this, wxID_ANY, _("Population"));
    text_population = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_id = new wxStaticText(this, wxID_ANY, _("Internal ID"));
    text_id = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_alias = new wxStaticText(this, wxID_ANY, _("Alias Names"));
    text_aliases = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
    button_add = new wxButton(this, ATLAS_ADD, _("New Entry"));
    button_edit = new wxButton(this, ATLAS_EDIT, _("Edit Entry"));
    button_delete = new wxButton(this, ATLAS_DELETE, _("Delete Entry"));
    static_line_button = new wxStaticLine(this, wxID_ANY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	selectedRow = -1;

	if ( callerId == ATLASDIALOG_CALLER_APPLICATIONWINDOW )
	{
		button_cancel->Show( false );
	}
	updateCountryChoice();
	choice_mode->SetSelection( config->atlas->filterMode );

	Connect( ATLAS_FILTER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AtlasDialog::OnFilterChanged ));
	Connect( ATLAS_COUNTRY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AtlasDialog::OnFilterChanged ));
	Connect( ATLAS_MODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AtlasDialog::OnFilterChanged ));

	Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( AtlasDialog::OnCellChange ));
	Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( AtlasDialog::OnCellDClick ));

  Connect( ATLAS_TIMER, wxEVT_TIMER, wxTimerEventHandler( AtlasDialog::OnTimer ));

	Connect( ATLAS_ADD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasDialog::OnAddEntry ));
	Connect( ATLAS_EDIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasDialog::OnEditEntry ));
	Connect( ATLAS_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasDialog::OnDeleteEntry ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( AtlasDialog::OnKeyDown ));

	timer = new wxTimer( this, ATLAS_TIMER );
	timer->Start( TIMER_INTERVAL_MILLISEC );

	if ( ! searchString.IsEmpty()) filter_edit->SetValue( searchString );
	filter_edit->SetFocus();

	if ( IS_VALID_SIZE( config->viewprefs->sizes.sAtlasDialog )) SetSize( config->viewprefs->sizes.sAtlasDialog );

	logic->setFilterConditions( filter_edit->GetValue(), getSelectedCountryISO(), choice_mode->GetSelection());
	updateEntryDetails();
}

/*****************************************************
**
**   AtlasDialog   ---   Destructor
**
******************************************************/
AtlasDialog::~AtlasDialog()
{
	if ( timer->IsRunning()) timer->Stop();
	delete timer;

	// Save grid column width
	config->atlas->gridColumnSizes.clear();
	config->atlas->gridColumnSizes.push_back( locgrid->GetRowLabelSize());

	for ( int i = 0; i < ATLAS_GRID_MAXCOLS; i++ )
	{
		config->atlas->gridColumnSizes.push_back( locgrid->GetColSize( i ));
	}

	config->atlas->lastSelectedCountry = getSelectedCountryISO();
	config->atlas->filterMode = choice_mode->GetSelection();
	config->viewprefs->sizes.sAtlasDialog = GetSize();
	delete logic;
}

/*****************************************************
**
**   AtlasDialog   ---   updateCountryChoice
**
******************************************************/
void AtlasDialog::updateCountryChoice()
{
	vector<wxString> dummy;
	list<AtlasCountry> l = logic->getFavouriteCountries( dummy );

	country_choice->Clear();
	country_choice->Append( _( "All Countries" ));

	int selCountry = 0;
	int i = 1;
	for ( list<AtlasCountry>::iterator iter = l.begin(); iter != l.end(); iter++ )
	{
		wxStringClientData *clientData = new wxStringClientData( (*iter).iso );
		country_choice->Append( (*iter).name, clientData );
		if ( config->atlas->lastSelectedCountry == (*iter).iso)
		{
			selCountry = i;
		}
		i++;
	}
	country_choice->SetSelection( selCountry );
}

/*****************************************************
**
**   AtlasDialog   ---   OnFilterChanged
**
******************************************************/
void AtlasDialog::OnFilterChanged( wxCommandEvent& )
{
	logic->setFilterConditions( filter_edit->GetValue(), getSelectedCountryISO(), choice_mode->GetSelection());
	selectedRow = -1;
	updateEntryDetails();

	updateFilterMatchLabel( -1 );
	locgrid->Scroll( 0, 0 );
}

/*****************************************************
**
**   AtlasDialog   ---   OnCellChange
**
******************************************************/
void AtlasDialog::OnCellChange( wxGridEvent& event )
{
	selectedRow = event.GetRow();
	updateEntryDetails();
	event.Skip();
}

/*****************************************************
**
**   AtlasDialog   ---   OnCellDClick
**
******************************************************/
void AtlasDialog::OnCellDClick( wxGridEvent& event )
{
	selectedRow = event.GetRow();
	if ( callerId == ATLASDIALOG_CALLER_DATADIALOG || callerId == ATLASDIALOG_CALLER_DEFAULTLOCATIONPANEL )
	{
		EndModal( wxID_OK );
	}
	else editEntry();
}

/*****************************************************
**
**   AtlasDialog   ---   OnSelectLabel
**
******************************************************/
void AtlasDialog::OnSelectLabel( wxGridEvent& event )
{
	// TODO
	selectedRow = event.GetRow();
}

/*****************************************************
**
**   AtlasDialog   ---   getSelectedCountryISO
**
******************************************************/
wxString AtlasDialog::getSelectedCountryISO()
{
	wxString iso;
	int sel = country_choice->GetSelection();
	if ( sel != -1 )
	{
		wxString name = country_choice->GetStringSelection();
		wxClientData *clientData = country_choice->GetClientObject( sel );
		if ( clientData != 0 )
		{
			iso = ((wxStringClientData*)clientData)->GetData();
		}
	}
	return iso;
}

/*****************************************************
**
**   AtlasDialog   ---   updateFilterMatchLabel
**
******************************************************/
void AtlasDialog::updateFilterMatchLabel( const int &count )
{
	wxString s;

	if ( count > 0 ) s.Printf( _( "%d entries" ), count );
	else if ( count == 0 ) s = _( "No match" );

	match_label->SetLabel( s );
}

/*****************************************************
**
**   AtlasDialog   ---   updateEntryDetails
**
******************************************************/
void AtlasDialog::updateEntryDetails()
{
	Formatter *formatter = Formatter::get();

	AtlasEntry *e = logic->getEntryByRowId( selectedRow );

	button_edit->Enable( e != 0 );;
	button_delete->Enable( e != 0 );;

	if ( e )
	{
		selectedEntry = logic->getFullEntry( e->id );
		text_name->SetLabel( selectedEntry.name );
		text_asciiname->SetLabel( selectedEntry.asciiname );
		text_country->SetLabel( selectedEntry.country );
		text_admincode->SetLabel( selectedEntry.admin );
		text_id->SetLabel( formatter->getIntOrEmtpy( selectedEntry.id ));
		text_population->SetLabel( formatter->getIntOrEmtpy( selectedEntry.population ));

		if ( selectedEntry.getLatitude() != 0 ) text_latitude->SetLabel( formatter->getLatitudeFormatted( selectedEntry.getLatitude() ));
		else text_latitude->SetLabel( wxEmptyString );
		if ( selectedEntry.getLongitude() != 0 ) text_longitude->SetLabel( formatter->getLongitudeFormatted( selectedEntry.getLongitude() ));
		else text_longitude->SetLabel( wxEmptyString );

		text_timezone->SetLabel( selectedEntry.timezone );
		text_tzoffset->SetLabel( wxString::Format( wxT( "%.1f" ), selectedEntry.tzoffset ));
		text_aliases->SetValue( selectedEntry.aliases );
	}
	else
	{
		text_name->SetLabel( wxEmptyString );
		text_asciiname->SetLabel( wxEmptyString );
		text_country->SetLabel( wxEmptyString );
		text_admincode->SetLabel( wxEmptyString );
		text_id->SetLabel( wxEmptyString );
		text_population->SetLabel( wxEmptyString );
		text_latitude->SetLabel( wxEmptyString );
		text_longitude->SetLabel( wxEmptyString );
		text_timezone->SetLabel( wxEmptyString );
		text_tzoffset->SetLabel( wxEmptyString );
		text_aliases->SetValue( wxEmptyString );
	}
}

/*****************************************************
**
**   AtlasDialog   ---   OnIdle
**
******************************************************/
void AtlasDialog::OnIdle( wxIdleEvent& )
{
	checkNews();
}

/*****************************************************
**
**   AtlasDialog   ---   OnTimer
**
******************************************************/
void AtlasDialog::OnTimer( wxTimerEvent& )
{
	checkNews();
}

/*****************************************************
**
**   AtlasDialog   ---   checkNews
**
******************************************************/
void AtlasDialog::checkNews()
{
	if ( logic->fetchHasNews())
	{
		logic->aknowledgeFetchHasNews();
		locgrid->Refresh();
		locgrid->SelectRow( 0 );
		selectedRow = 0;
		updateEntryDetails();
	}

	if ( logic->countHasNews())
	{
		logic->aknowledgeCountHasNews();
		int oldrows = locgrid->GetNumberRows();
		int newrows = logic->getCount();
		if ( newrows > oldrows )
		{
			locgrid->AppendRows( newrows - oldrows );
		}
		else if ( newrows < oldrows )
		{
			locgrid->DeleteRows( 0, oldrows - newrows );
		}
		updateFilterMatchLabel( logic->getCount());
	}
}

/*****************************************************
**
**   AtlasDialog   ---   OnKeyDown
**
******************************************************/
void AtlasDialog::OnKeyDown( wxKeyEvent &event )
{
	switch ( event.GetKeyCode())
	{
		case WXK_ESCAPE:
			Close();
		break;
		default:
			event.Skip();
		break;
	}
}

/*****************************************************
**
**   AtlasDialog   ---   OnAddEntry
**
******************************************************/
void AtlasDialog::OnAddEntry( wxCommandEvent& )
{
	AtlasEntry entry = selectedEntry;
	entry.id = 0;
	entry.name = wxString::Format( _( "Copy of %s" ), selectedEntry.name.c_str());
	entry.asciiname = wxString::Format( _( "Copy of %s" ), selectedEntry.asciiname.c_str());
	AtlasEditDialog dialog( this,  logic, entry );
	if ( dialog.ShowModal() == wxID_OK )
	{
		entry = dialog.getEntry();
		logic->saveEntry( entry );
		logic->updateFilter();
		locgrid->Refresh();
	}
}

/*****************************************************
**
**   AtlasDialog   ---   OnEditEntry
**
******************************************************/
void AtlasDialog::OnEditEntry( wxCommandEvent& )
{
	editEntry();
}

/*****************************************************
**
**   AtlasDialog   ---   editEntry
**
******************************************************/
void AtlasDialog::editEntry()
{
	AtlasEditDialog dialog( this, logic, selectedEntry );
	if ( dialog.ShowModal() == wxID_OK )
	{
		selectedEntry = dialog.getEntry();
		logic->saveEntry( selectedEntry );
		logic->updateFilter();
		locgrid->Refresh();
	}
}

/*****************************************************
**
**   AtlasDialog   ---   OnDeleteEntry
**
******************************************************/
void AtlasDialog::OnDeleteEntry( wxCommandEvent& )
{
	if ( doMessageBox( this, wxString::Format( _( "Delete %s?" ), selectedEntry.name.c_str() ), wxYES_NO | wxICON_QUESTION ))
	{
		logic->deleteEntry( selectedEntry.id );
		logic->updateFilter();
		selectedRow = -1;
		updateEntryDetails();
		locgrid->Scroll( 0, 0 );
	}
}

/*****************************************************
**
**   AtlasDialog   ---   set_properties
**
******************************************************/
void AtlasDialog::set_properties()
{
	int i;

	gridTable = new AtlasGridTable( locgrid, logic );
	locgrid->SetTable( gridTable, true );

	if ( config->atlas->gridColumnSizes.size() == ATLAS_GRID_MAXCOLS + 1 )
	{
		locgrid->SetRowLabelSize( config->atlas->gridColumnSizes[0] );
		for( uint i = 0; i < ATLAS_GRID_MAXCOLS; i++ )
		{
			for ( i = 0; i < ATLAS_GRID_MAXCOLS; i++ )
			{
				locgrid->SetColSize( i, config->atlas->gridColumnSizes[i+1] );
			}
		}
	}
	else
	{
		locgrid->SetRowLabelSize( 40 );
		int totalwidth = config->viewprefs->sizes.sAtlasDialog.x - 90;
		int colw = totalwidth / 5;
		for ( i = 0; i < ATLAS_GRID_MAXCOLS; i++ )
		{
			locgrid->SetColSize( i, colw );
		}
	}

	locgrid->SetSelectionMode( wxGrid::wxGridSelectRows );
	locgrid->EnableEditing( false );
	locgrid->SetRowLabelSize( 50 );

    // begin wxGlade: AtlasDialog::set_properties
    SetTitle(_("Atlas"));
    filter_edit->SetMinSize(wxSize(200, -1));
    filter_edit->SetToolTip(_("\"*\" or \"%\" can be used for multiple character wildcards, \"?\" or \"_\" for single character wildcards"));
    country_choice->SetSelection(0);
    choice_mode->SetSelection(0);
    locgrid->SetMinSize(wxSize(-1,200));
    static_line_match->SetMinSize(wxSize(640, 2));
    label_name->SetMinSize(wxSize(100, 16));
    text_name->SetMinSize(wxSize(150, 16));
    label_longitude->SetMinSize(wxSize(100, 16));
    text_longitude->SetMinSize(wxSize(150, 16));
    label_asciiname->SetMinSize(wxSize(100, 17));
    text_asciiname->SetMinSize(wxSize(150, 17));
    label_latitude->SetMinSize(wxSize(100, 17));
    text_latitude->SetMinSize(wxSize(150, 16));
    label_country->SetMinSize(wxSize(100, 16));
    text_country->SetMinSize(wxSize(150, 16));
    label_timezone->SetMinSize(wxSize(100, 16));
    text_timezone->SetMinSize(wxSize(150, 16));
    label_admincode->SetMinSize(wxSize(100, 16));
    text_admincode->SetMinSize(wxSize(150, 16));
    label_tzoffset->SetMinSize(wxSize(100, 16));
    text_tzoffset->SetMinSize(wxSize(150, 16));
    label_population->SetMinSize(wxSize(100, 16));
    text_population->SetMinSize(wxSize(150, 16));
    label_id->SetMinSize(wxSize(100, 16));
    text_id->SetMinSize(wxSize(150, 16));
    label_alias->SetMinSize(wxSize(100, 16));
    text_aliases->SetMinSize(wxSize(-1, 100));
    // end wxGlade
}

/*****************************************************
**
**   AtlasDialog   ---   do_layout
**
******************************************************/
void AtlasDialog::do_layout()
{
    // begin wxGlade: AtlasDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(7, 1, 0, 0);
    wxFlexGridSizer* sizer_button = new wxFlexGridSizer(1, 5, 0, 0);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer_actions_staticbox->Lower();
    wxStaticBoxSizer* sizer_actions = new wxStaticBoxSizer(sizer_actions_staticbox, wxVERTICAL);
    sizer_details_staticbox->Lower();
    wxStaticBoxSizer* sizer_details = new wxStaticBoxSizer(sizer_details_staticbox, wxHORIZONTAL);
    wxBoxSizer* sizer_details2 = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid_details = new wxFlexGridSizer(6, 4, 0, 3);
    wxFlexGridSizer* sizer_top = new wxFlexGridSizer(1, 3, 0, 0);
    sizer_filter_staticbox->Lower();
    wxStaticBoxSizer* sizer_filter = new wxStaticBoxSizer(sizer_filter_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_filter = new wxFlexGridSizer(1, 6, 3, 3);
    grid_filter->Add(label_filtername, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_filter->Add(filter_edit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_filter->Add(label_filtercountry, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_filter->Add(country_choice, 0, wxALL, 3);
    grid_filter->Add(label_mode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_filter->Add(choice_mode, 0, wxALL, 3);
    grid_filter->AddGrowableCol(1);
    grid_filter->AddGrowableCol(3);
    grid_filter->AddGrowableCol(5);
    sizer_filter->Add(grid_filter, 1, wxALL|wxEXPAND, 3);
    sizer_top->Add(sizer_filter, 1, wxALL|wxEXPAND, 5);
    sizer_top->AddGrowableCol(0);
    sizer_main->Add(sizer_top, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 6);
    sizer_main->Add(locgrid, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(match_label, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 6);
    sizer_main->Add(static_line_match, 0, wxTOP|wxBOTTOM, 5);
    grid_details->Add(label_name, 0, wxALL, 3);
    grid_details->Add(text_name, 0, wxALL, 3);
    grid_details->Add(label_longitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_details->Add(text_longitude, 0, wxALL, 3);
    grid_details->Add(label_asciiname, 0, wxALL, 3);
    grid_details->Add(text_asciiname, 0, wxALL, 3);
    grid_details->Add(label_latitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_details->Add(text_latitude, 0, wxALL, 3);
    grid_details->Add(label_country, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_details->Add(text_country, 0, wxALL, 3);
    grid_details->Add(label_timezone, 0, wxALL, 3);
    grid_details->Add(text_timezone, 0, wxALL, 3);
    grid_details->Add(label_admincode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_details->Add(text_admincode, 0, wxALL, 3);
    grid_details->Add(label_tzoffset, 0, wxALL, 3);
    grid_details->Add(text_tzoffset, 0, wxALL, 3);
    grid_details->Add(label_population, 0, wxALL, 3);
    grid_details->Add(text_population, 0, wxALL, 3);
    grid_details->Add(label_id, 0, wxALL, 3);
    grid_details->Add(text_id, 0, wxALL, 3);
    grid_details->AddGrowableCol(1);
    grid_details->AddGrowableCol(3);
    sizer_details2->Add(grid_details, 1, wxEXPAND, 0);
    sizer_details2->Add(label_alias, 0, wxALL, 3);
    sizer_details2->Add(text_aliases, 0, wxALL|wxEXPAND, 3);
    sizer_details->Add(sizer_details2, 1, wxEXPAND, 0);
    sizer_4->Add(sizer_details, 1, wxEXPAND, 0);
    sizer_actions->Add(button_add, 0, wxALL|wxEXPAND, 3);
    sizer_actions->Add(button_edit, 0, wxALL|wxEXPAND, 3);
    sizer_actions->Add(button_delete, 0, wxALL|wxEXPAND, 3);
    sizer_4->Add(sizer_actions, 0, wxEXPAND, 0);
    sizer_main->Add(sizer_4, 1, wxEXPAND, 0);
    sizer_main->Add(static_line_button, 0, wxALL|wxEXPAND, 5);
    sizer_button->Add(button_ok, 0, wxALL, 3);
    sizer_button->Add(button_cancel, 0, wxALL, 3);
    sizer_main->Add(sizer_button, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(1);
    sizer_main->AddGrowableCol(0);
    Layout();
    // end wxGlade
}

