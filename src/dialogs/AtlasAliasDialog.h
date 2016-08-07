/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasAliasDialog.h
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
