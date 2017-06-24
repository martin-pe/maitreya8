/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/StyleManagerDialog.cpp
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

#include "StyleManagerDialog.h"

#include "FileConfig.h"
#include "guibase.h"
#include "SheetConfig.h"

#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/textdlg.h>

// begin wxGlade: ::extracode
// end wxGlade

enum { CD_LIST = wxID_HIGHEST + 300, CD_NEW, CD_COPY, CD_RENAME, CD_DELETE, CD_UP, CD_DOWN };

/*****************************************************
**
**   StyleManagerDialog   ---   Constructor
**
******************************************************/
StyleManagerDialog::StyleManagerDialog(wxWindow* parent, vector<SheetConfig*> &configs, const int &sel )
	: wxDialog( parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE ),
	v( configs )
{
    // begin wxGlade: StyleManagerDialog::StyleManagerDialog
    const wxString list_choices[] = {};
    list = new wxListBox(this, CD_LIST, wxDefaultPosition, wxDefaultSize, 0, list_choices, wxLB_SINGLE);
    button_new = new wxButton(this, CD_NEW, _("New"));
    button_copy = new wxButton(this, CD_COPY, _("Copy"));
    button_rename = new wxButton(this, CD_RENAME, _("Rename"));
    button_delete = new wxButton(this, CD_DELETE, _("Delete"));
    button_up = new wxButton(this, CD_UP, _("Up"));
    button_down = new wxButton(this, CD_DOWN, _("Down"));
    button_ok = new wxButton(this, wxID_OK, _("Close"));

    set_properties();
    do_layout();
    // end wxGlade

	populateChoice();
	list->SetSelection( sel );
	list->Refresh();

	updateUi();

	Connect( CD_NEW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StyleManagerDialog::OnNew ));
	Connect( CD_COPY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StyleManagerDialog::OnCopy ));
	Connect( CD_RENAME, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StyleManagerDialog::OnRename ));
	Connect( CD_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StyleManagerDialog::OnDelete ));
	Connect( CD_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StyleManagerDialog::OnUp ));
	Connect( CD_DOWN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StyleManagerDialog::OnDown ));
	Connect( CD_LIST, wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( StyleManagerDialog::OnListbox ));
	Connect( CD_LIST, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( StyleManagerDialog::OnListboxDoubleClick ));
}

/*****************************************************
**
**   StyleManagerDialog   ---   Destructor
**
******************************************************/
StyleManagerDialog::~StyleManagerDialog()
{
}

/*****************************************************
**
**   StyleManagerDialog   ---   populateChoice
**
******************************************************/
void StyleManagerDialog::populateChoice()
{
	//printf( "START populateChoice\n" );
	list->Clear();

	for( vector<SheetConfig*>::iterator iter = v.begin(); iter != v.end(); iter++ )
	{
		list->Append( (*iter)->name );
	}
	list->Refresh();
	//printf( "ENDE populateChoice \n" );
}

/*****************************************************
**
**   StyleManagerDialog   ---   getSelection
**
******************************************************/
int StyleManagerDialog::getSelection() const
{
	const int sel = list->GetSelection();
	/*
	if ( list->GetCount() > 0 && sel == wxNOT_FOUND )
	{
		wxLogError( wxT( "no entry selected" ));
	}
	*/
	return sel;
}

/*****************************************************
**
**   StyleManagerDialog   ---   updateUi
**
******************************************************/
void StyleManagerDialog::updateUi()
{
	const int sel = getSelection();
	//printf( "updateUi %d\n", sel );
	button_copy->Enable( sel != -1 );
	button_rename->Enable( sel != -1 );
	button_delete->Enable( sel != -1 );
	button_up->Enable( sel > 0 );
	button_down->Enable( sel != (int)(list->GetCount() - 1 ));
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnListbox
**
******************************************************/
void StyleManagerDialog::OnListbox( wxCommandEvent& )
{
	//printf( "StyleManagerDialog::OnListbox\n" );
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnListboxDoubleClick
**
******************************************************/
void StyleManagerDialog::OnListboxDoubleClick( wxCommandEvent& )
{
	//printf( "StyleManagerDialog::OnListboxDoubleClick %d\n", getSelection() );
	renameEntry( getSelection() );
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnNew
**
******************************************************/
void StyleManagerDialog::OnNew( wxCommandEvent& )
{
	//printf( "New\n" );
	wxTextEntryDialog dialog( this, _( "Enter Name" ), _( "New Entry" ), _( "New Entry" ));
	if ( dialog.ShowModal() == wxID_OK )
	{
		SheetConfig *cfg = new SheetConfig( dialog.GetValue() );
		v.push_back( cfg );
		populateChoice();
		list->SetSelection( list->GetCount() - 1 );
		list->Refresh();
	}
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnCopy
**
******************************************************/
void StyleManagerDialog::OnCopy( wxCommandEvent& )
{
	//printf( "StyleManagerDialog::OnCopy TODO\n" );
	SheetConfig *cfg = new SheetConfig( *v[getSelection()] );
	cfg->name << wxT( " (copy)" );
	v.push_back( cfg );
	populateChoice();
	list->SetSelection( list->GetCount() - 1 );
	list->Refresh();
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnUp
**
******************************************************/
void StyleManagerDialog::OnUp( wxCommandEvent& )
{
	const int sel = getSelection();
	const int target = sel - 1;
	assert( target >= 0 );
	//printf( "On Up selected %d target %d\n", sel, target );
	SheetConfig *cfg = v[sel];
	v[sel] = v[target];
	v[target] = cfg;
	populateChoice();
	list->SetSelection( target );
	list->Refresh();
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnDown
**
******************************************************/
void StyleManagerDialog::OnDown( wxCommandEvent& )
{
	const int sel = getSelection();
	const int target = sel + 1;
	assert( target < (int)list->GetCount() );
	//printf( "On Down selected %d target %d\n", sel, target );
	SheetConfig *cfg = v[sel];
	v[sel] = v[target];
	v[target] = cfg;
	populateChoice();
	list->SetSelection( target );
	list->Refresh();
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   renameEntry
**
******************************************************/
void StyleManagerDialog::renameEntry( const int &sel )
{
	//printf( "renameEntry\n" );
	wxTextEntryDialog dialog( this, _( "Enter New Name" ), _( "Rename Entry" ), v[sel]->name );
	if ( dialog.ShowModal() == wxID_OK )
	{
		v[sel]->name = dialog.GetValue();
		populateChoice();
		list->SetSelection( sel );
		list->Refresh();
	}
	list->Refresh();
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnRename
**
******************************************************/
void StyleManagerDialog::OnRename( wxCommandEvent& )
{
	//printf( "Rename\n" );
	renameEntry( getSelection() );
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   OnDelete
**
******************************************************/
void StyleManagerDialog::OnDelete( wxCommandEvent& )
{
	//printf( "Delete size at start %d\n", (int)v.size() );
	int sel = getSelection();
	assert( sel >= 0 && sel < (int)v.size());

	const int answer = doMessageBox( this,
		wxString::Format( _( "Do you want to delete the entry %s?" ), v[sel]->name.c_str()),
		wxYES_NO + wxNO_DEFAULT + wxICON_QUESTION );
	if ( answer == wxID_YES )
	{
		delete v[sel];
		v.erase( v.begin() + sel );

		if ( v.size() == 0 )
		{
			v.push_back( new SheetConfig );
		}
		if ( sel == (int)v.size()) sel--;
		populateChoice();
		list->SetSelection( sel );
		list->Refresh();
	}
	updateUi();
}

/*****************************************************
**
**   StyleManagerDialog   ---   set_properties
**
******************************************************/
void StyleManagerDialog::set_properties()
{
    // begin wxGlade: StyleManagerDialog::set_properties
    SetTitle(_("Manage Styles"));
    list->SetMinSize(wxSize(300, 300));
    button_ok->SetDefault();
    // end wxGlade
}

/*****************************************************
**
**   StyleManagerDialog   ---   do_layout
**
******************************************************/
void StyleManagerDialog::do_layout()
{
    // begin wxGlade: StyleManagerDialog::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_top = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_right = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Actions")), wxVERTICAL);
    wxStaticBoxSizer* sizer_list = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Styles")), wxVERTICAL);
    sizer_list->Add(list, 1, wxALL|wxEXPAND, 3);
    sizer_top->Add(sizer_list, 1, wxEXPAND, 3);
    sizer_right->Add(button_new, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_copy, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_rename, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_delete, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_up, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_down, 0, wxALL|wxEXPAND, 3);
    sizer_top->Add(sizer_right, 0, wxALIGN_BOTTOM|wxALL, 3);
    sizer_main->Add(sizer_top, 1, wxEXPAND, 0);
    wxStaticLine* static_line_2 = new wxStaticLine(this, wxID_ANY);
    sizer_main->Add(static_line_2, 0, wxALL|wxEXPAND, 3);
    sizer_bottom->Add(button_ok, 0, wxALL, 3);
    sizer_main->Add(sizer_bottom, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

