/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/EphemerisPanel.h
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


	void OnSelectSEPath( wxCommandEvent& );
	void OnChangeConfigParams( wxCommandEvent& );
	void OnCheckConstantAyanamsa( wxCommandEvent& );
	virtual void updateUi();
	void OnTextChanged( wxCommandEvent& );

	EphemConfig *ephem;

protected:
    // begin wxGlade: EphemerisPanel::attributes
    wxStaticText* label_custom_aya_t0;
    wxTextCtrl* text_t0;
    wxPanel* panel_text_t0;
    wxStaticText* text_t0_formatted;
    wxStaticText* label_custom_aya_ayat0;
    wxTextCtrl* text_ayan_t0;
    wxPanel* panel_aya_t0;
    wxStaticText* text_ayan_t0_formatted;
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
