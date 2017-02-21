/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasAliasDialog.cpp
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

#include "AtlasAliasDialog.h"

// begin wxGlade: ::extracode
// end wxGlade

#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textdlg.h>
#include <wx/tokenzr.h>

#include "AtlasDao.h"
#include "maitreya.h"
#include "guibase.h"

enum { AL_LIST = wxID_HIGHEST + 1, AL_ADD, AL_EDIT, AL_DELETE };

/*****************************************************
**
**   AtlasAliasDialog   ---   Constructor
**
******************************************************/
AtlasAliasDialog::AtlasAliasDialog(wxWindow* parent, AtlasEntry &entry )
 : wxDialog(parent, -1, _( "Edit Aliases" ), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
 entry( entry )
{
    // begin wxGlade: AtlasAliasDialog::AtlasAliasDialog
    sizer_list_staticbox = new wxStaticBox(this, wxID_ANY, _("Aliases"));
    label_name = new wxStaticText(this, wxID_ANY, _("Name"));
    const wxString *list_choices = NULL;
    list = new wxListBox(this, AL_LIST, wxDefaultPosition, wxDefaultSize, 0, list_choices, wxLB_SINGLE|wxLB_SORT);
    button_add = new wxButton(this, AL_ADD, _("Add Entry"));
    button_edit = new wxButton(this, AL_EDIT, _("Edit Entry"));
    button_delete = new wxButton(this, AL_DELETE, _("Delete Entry"));
    static_line_4 = new wxStaticLine(this, wxID_ANY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( AL_ADD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasAliasDialog::OnAdd ));
	Connect( AL_EDIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasAliasDialog::OnEdit ));
	Connect( AL_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasAliasDialog::OnDelete ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasAliasDialog::OnOK ));
}

/*****************************************************
**
**   AtlasAliasDialog   ---   OnAdd
**
******************************************************/
void AtlasAliasDialog::OnAdd( wxCommandEvent& )
{
	wxTextEntryDialog dialog( this, _( "Add Entry" ), _( "Alias" ));
	if ( dialog.ShowModal() == wxID_OK )
	{
		list->Append( dialog.GetValue());

		// select new entry
		const int index = list->FindString( dialog.GetValue());
		if ( index != wxNOT_FOUND )
		{
			list->SetSelection( index );
		}
	}
}

/*****************************************************
**
**   AtlasAliasDialog   ---   OnEdit
**
******************************************************/
void AtlasAliasDialog::OnEdit( wxCommandEvent& )
{
	const int sel = list->GetSelection();
	if ( sel != wxNOT_FOUND )
	{
		wxTextEntryDialog dialog( this, _( "Edit Entry" ), _( "Alias" ), list->GetString( sel ) );
		if ( dialog.ShowModal() == wxID_OK )
		{
			list->SetString( sel, dialog.GetValue());
		}
	}
	else
	{
		doMessageBox( this, wxT( "No entry selected" ));
	}
}

/*****************************************************
**
**   AtlasAliasDialog   ---   OnDelete
**
******************************************************/
void AtlasAliasDialog::OnDelete( wxCommandEvent& )
{
	int sel = list->GetSelection();
	if ( sel != wxNOT_FOUND )
	{
		wxString message = wxString::Format( _( "Do you want to delete the item %s?" ), list->GetString( sel ).c_str());
		list->Delete( sel );
	}
	else
	{
		doMessageBox( this, wxT( "No entry selected" ));
	}
}

/*****************************************************
**
**   AtlasAliasDialog   ---   OnOK
**
******************************************************/
void AtlasAliasDialog::OnOK( wxCommandEvent& )
{
	const int size = list->GetCount();

	entry.aliases.Clear();
	for( int i = 0; i < size; i++ )
	{
		if ( i > 0 )
		{
			entry.aliases << wxT( ", " );
		}
		entry.aliases << list->GetString( i );
	}
	EndModal( wxID_OK );
}

/*****************************************************
**
**   AtlasAliasDialog   ---   set_properties
**
******************************************************/
void AtlasAliasDialog::set_properties()
{
    // begin wxGlade: AtlasAliasDialog::set_properties
    SetTitle(_("Edit Aliases"));
    list->SetMinSize(wxSize(200, 300));
    // end wxGlade

	wxString label;
	label << _( "Name" ) << wxT( ": " ) << entry.name;
	label_name->SetLabel( label );

	wxStringTokenizer tk( entry.aliases, wxT( "," ));
	wxString token;

	while ( tk.HasMoreTokens() )
	{
		token = tk.GetNextToken().Trim( true ).Trim( false );
		list->Append( token );
	}
	if ( list->GetCount() > 0 ) list->Select( 0 );
}

/*****************************************************
**
**   AtlasAliasDialog   ---   do_layout
**
******************************************************/
void AtlasAliasDialog::do_layout()
{
    // begin wxGlade: AtlasAliasDialog::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_attributes = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_actions = new wxBoxSizer(wxVERTICAL);
    sizer_list_staticbox->Lower();
    wxStaticBoxSizer* sizer_list = new wxStaticBoxSizer(sizer_list_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_list = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_main->Add(label_name, 0, wxALL, 3);
    grid_list->Add(list, 0, wxALL|wxEXPAND, 3);
    grid_list->AddGrowableRow(0);
    grid_list->AddGrowableCol(0);
    sizer_list->Add(grid_list, 1, wxEXPAND, 0);
    sizer_attributes->Add(sizer_list, 1, wxEXPAND, 0);
    sizer_actions->Add(button_add, 0, wxALL|wxEXPAND, 3);
    sizer_actions->Add(button_edit, 0, wxALL|wxEXPAND, 3);
    sizer_actions->Add(button_delete, 0, wxALL|wxEXPAND, 3);
    sizer_actions->Add(static_line_4, 0, wxALL|wxEXPAND, 3);
    sizer_attributes->Add(sizer_actions, 0, 0, 3);
    sizer_main->Add(sizer_attributes, 1, wxEXPAND, 0);
    sizer_bottom->Add(button_ok, 0, wxALL|wxEXPAND, 3);
    sizer_bottom->Add(button_cancel, 0, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_bottom, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

