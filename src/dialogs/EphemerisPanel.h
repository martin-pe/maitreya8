/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/EphemerisPanel.h
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

#ifndef EPHEMERISPANEL_H
#define EPHEMERISPANEL_H


#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class EphemConfig;

/*************************************************//**
*
* \brief ConfigPanel for ephemeris parameters
*
******************************************************/
class EphemerisPanel: public ConfigPanel
{
	DECLARE_CLASS( EphemerisPanel )

public:
    // begin wxGlade: EphemerisPanel::ids
    // end wxGlade

	EphemerisPanel( wxWindow* );
	~EphemerisPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: EphemerisPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade


	void OnSelectSEPath( wxCommandEvent& );
	void OnChangeConfigParams( wxCommandEvent& );
	void OnCheckConstantAyanamsa( wxCommandEvent& );
	virtual void updateUi();
	void OnTextChanged( wxCommandEvent& );

	EphemConfig *ephem;

protected:
    // begin wxGlade: EphemerisPanel::attributes
    wxStaticBox* sizer_sunrise_staticbox;
    wxStaticBox* sizer_calc_staticbox;
    wxStaticBox* sizer_ephem_path_staticbox;
    wxStaticBox* sizer_aya_staticbox;
    wxStaticText* label_custom_aya_t0;
    wxTextCtrl* text_t0;
    wxStaticText* text_t0_formatted;
    wxPanel* panel_text_t0;
    wxStaticText* label_custom_aya_ayat0;
    wxTextCtrl* text_ayan_t0;
    wxStaticText* text_ayan_t0_formatted;
    wxPanel* panel_aya_t0;
    wxCheckBox* check_period;
    wxTextCtrl* text_period;
    wxButton* button_sepath;
    wxTextCtrl* text_sepath;
    wxStaticText* label_timemodel;
    wxChoice* choice_timemodel;
    wxStaticText* label_coord;
    wxChoice* choice_coord;
    wxStaticText* label_pos;
    wxChoice* choice_pos;
    wxStaticText* label_sunrise_disk;
    wxChoice* choice_sunrise_disk;
    wxCheckBox* check_sunrise_refrac;
    // end wxGlade
}; // wxGlade: end class


#endif // EPHEMERISPANEL_H
