/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasEditDialog.h
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

#ifndef ATLASEDITDIALOG_H
#define ATLASEDITDIALOG_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

#include <wx/dialog.h>
#include <list>

#include <AtlasLogic.h>

using namespace std;

class wxButton;
class wxChoice;
class wxPanel;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

class MDegSpin;
class MultiLineStaticText;

/*************************************************//**
*
* 
*
******************************************************/
class AtlasEditDialog: public wxDialog
{
public:
    // begin wxGlade: AtlasEditDialog::ids
    // end wxGlade

	AtlasEditDialog(wxWindow* parent, AtlasLogic *logic, const AtlasEntry entry );
	AtlasEntry getEntry() const { return model; }

private:
    // begin wxGlade: AtlasEditDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	AtlasLogic *logic;
	AtlasEntry model;

	list<TimezoneEntry> tzlist;
	list<AtlasCountry> countrylist;

	void OnOK( wxCommandEvent& );
	void OnChoiceCountry( wxCommandEvent& );
	void OnChoiceTzName( wxCommandEvent& );
	void OnEntryAliases( wxCommandEvent& );
	void OnTextChanged( wxCommandEvent& );

	void updateTzHours();
	void updateAdminChoice();
	void updateTzChoice();

protected:
    // begin wxGlade: AtlasEditDialog::attributes
    wxStaticBox* sizer_main_staticbox;
    wxStaticBox* sizer_bottom_staticbox;
    wxStaticBox* sizer_left_staticbox;
    wxStaticText* label_name;
    wxTextCtrl* text_name;
    wxStaticText* label_asciiname;
    wxTextCtrl* text_asciiname;
    wxStaticText* label_country;
    wxChoice* choice_country;
    wxStaticText* label_admincode;
    wxChoice* choice_admin;
    wxStaticText* label_population;
    wxTextCtrl* text_population;
    wxStaticText* label_longitude;
    MDegSpin* text_longitude;
    wxChoice* choice_ew;
    wxStaticText* label_latitude;
    MDegSpin* text_latitude;
    wxChoice* choice_ns;
    wxStaticText* label_timezone;
    wxChoice* choice_timezone;
    wxStaticText* label_tzh;
    wxStaticText* label_tzhours;
    wxStaticText* label_aliases;
    wxButton* button_aliases;
    wxTextCtrl* text_alias;
    wxButton* button_ok;
    wxButton* button_cancel_copy;
    // end wxGlade
}; // wxGlade: end class


#endif // ATLASEDITDIALOG_H
