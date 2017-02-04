/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/StyleManagerDialog.h
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

#ifndef STYLEMANAGERDIALOG_H
#define STYLEMANAGERDIALOG_H

#include <wx/dialog.h>
#include<vector>

using namespace std;

// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class SheetConfig;
class wxButton;
class wxListBox;
class wxStaticBox;

/*************************************************//**
*
*    CLASS    ---   StyleManagerDialog
*
******************************************************/
class StyleManagerDialog: public wxDialog
{
public:
    // begin wxGlade: StyleManagerDialog::ids
    // end wxGlade

	StyleManagerDialog(wxWindow* parent, vector<SheetConfig*>&, const int &sel );
	~StyleManagerDialog();
	int getSelection() const;

private:
    // begin wxGlade: StyleManagerDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void populateChoice();
	void updateUi();
	void renameEntry( const int& );

	void OnListbox( wxCommandEvent& );
	void OnListboxDoubleClick( wxCommandEvent& );
	void OnNew( wxCommandEvent& );
	void OnCopy( wxCommandEvent& );
	void OnRename( wxCommandEvent& );
	void OnDelete( wxCommandEvent& );
	void OnUp( wxCommandEvent& );
	void OnDown( wxCommandEvent& );

	vector<SheetConfig*> &v;

protected:
    // begin wxGlade: StyleManagerDialog::attributes
    wxListBox* list;
    wxButton* button_new;
    wxButton* button_copy;
    wxButton* button_rename;
    wxButton* button_delete;
    wxButton* button_up;
    wxButton* button_down;
    wxButton* button_ok;
    // end wxGlade
}; // wxGlade: end class


#endif // STYLEMANAGERDIALOG_H
