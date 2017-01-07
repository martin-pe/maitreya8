/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasAliasDialog.h
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

#ifndef ATLASALIASDIALOG_H
#define ATLASALIASDIALOG_H

// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

#include <wx/dialog.h>

class wxButton;
class wxListBox;
class wxListEvent;
class wxStaticBox;
class wxStaticText;

struct AtlasEntry;

/*************************************************//**
*
*  
*
******************************************************/
class AtlasAliasDialog: public wxDialog
{
public:
    // begin wxGlade: AtlasAliasDialog::ids
    // end wxGlade

	AtlasAliasDialog(wxWindow* parent, AtlasEntry& );

private:
    // begin wxGlade: AtlasAliasDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnAdd( wxCommandEvent& );
	void OnEdit( wxCommandEvent& );
	void OnDelete( wxCommandEvent& );
	void OnOK( wxCommandEvent& );

	AtlasEntry &entry;

protected:
    // begin wxGlade: AtlasAliasDialog::attributes
    wxStaticBox* sizer_list_staticbox;
    wxStaticText* label_name;
    wxListBox* list;
    wxButton* button_add;
    wxButton* button_edit;
    wxButton* button_delete;
    wxStaticLine* static_line_4;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
}; // wxGlade: end class


#endif // ATLASALIASDIALOG_H
