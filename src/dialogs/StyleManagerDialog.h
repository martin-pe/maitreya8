/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/StyleManagerDialog.h
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
    wxStaticBox* sizer_right_staticbox;
    wxStaticBox* sizer_list_staticbox;
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
