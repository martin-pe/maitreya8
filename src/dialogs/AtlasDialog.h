/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasDialog.h
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

#ifndef ATLASDIALOG_H
#define ATLASDIALOG_H

// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class AtlasDialog;
class AtlasGridTable;
class AtlasLogic;
class wxButton;
class wxChoice;
class wxGridEvent;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;
class wxTimer;
class wxGrid;
class MultiLineStaticText;

#include <wx/dialog.h>

#include "AtlasDao.h"

using namespace std;

enum { ATLASDIALOG_CALLER_APPLICATIONWINDOW, ATLASDIALOG_CALLER_DATADIALOG, ATLASDIALOG_CALLER_DEFAULTLOCATIONPANEL };

/*************************************************//**
*
*  
*
******************************************************/
class AtlasDialogWrapper
{
public:
	AtlasDialogWrapper( wxWindow *parent, const int &callerId, wxString searchString = wxEmptyString );
	~AtlasDialogWrapper();

	int run();

	AtlasEntry *getSelectedEntry();

private:
	const int callerId;
	wxString searchString;
	wxWindow *parent;
	AtlasDialog *dialog;

};

/*************************************************//**
*
*  GUI access to atlas database
*
******************************************************/
class AtlasDialog: public wxDialog
{
public:
    // begin wxGlade: AtlasDialog::ids
    // end wxGlade

	AtlasDialog(wxWindow* parent, const int &callerId, wxString searchString = wxEmptyString );
	~AtlasDialog();

	AtlasEntry &getSelectedEntry() { return selectedEntry; }

private:
    // begin wxGlade: AtlasDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnFilterChanged( wxCommandEvent& );
	void OnCellChange( wxGridEvent& );
	void OnCellDClick( wxGridEvent& );
	void OnSelectLabel( wxGridEvent& );

	void OnIdle( wxIdleEvent& );
	void OnAddEntry( wxCommandEvent& );
	void OnEditEntry( wxCommandEvent& );
	void OnDeleteEntry( wxCommandEvent& );
	void OnKeyDown( wxKeyEvent& );
	void OnTimer( class wxTimerEvent& );

	void editEntry();
	void updateFilterMatchLabel( const int& );
	void updateEntryDetails();
	wxString getSelectedCountryISO();
	void updateCountryChoice();
	void checkNews();

	const int callerId;

	int selectedRow;
	AtlasEntry selectedEntry;

	AtlasLogic *logic;
	AtlasGridTable *gridTable;
	wxTimer *timer;

protected:
    // begin wxGlade: AtlasDialog::attributes
    wxStaticBox* sizer_actions_staticbox;
    wxStaticBox* sizer_details_staticbox;
    wxStaticBox* sizer_filter_staticbox;
    wxStaticText* label_filtername;
    wxTextCtrl* filter_edit;
    wxStaticText* label_filtercountry;
    wxChoice* country_choice;
    wxStaticText* label_mode;
    wxChoice* choice_mode;
    wxGrid* locgrid;
    wxStaticText* match_label;
    wxStaticLine* static_line_match;
    wxStaticText* label_name;
    wxStaticText* text_name;
    wxStaticText* label_longitude;
    wxStaticText* text_longitude;
    wxStaticText* label_asciiname;
    wxStaticText* text_asciiname;
    wxStaticText* label_latitude;
    wxStaticText* text_latitude;
    wxStaticText* label_country;
    wxStaticText* text_country;
    wxStaticText* label_timezone;
    wxStaticText* text_timezone;
    wxStaticText* label_admincode;
    wxStaticText* text_admincode;
    wxStaticText* label_tzoffset;
    wxStaticText* text_tzoffset;
    wxStaticText* label_population;
    wxStaticText* text_population;
    wxStaticText* label_id;
    wxStaticText* text_id;
    wxStaticText* label_alias;
    wxTextCtrl* text_aliases;
    wxButton* button_add;
    wxButton* button_edit;
    wxButton* button_delete;
    wxStaticLine* static_line_button;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
}; // wxGlade: end class


#endif // ATLASDIALOG_H
