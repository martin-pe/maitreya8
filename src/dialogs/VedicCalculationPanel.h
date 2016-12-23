/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicCalculationPanel.h
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

#ifndef VEDICCALCULATIONPANEL_H
#define VEDICCALCULATIONPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class AyanamsaChoice;
class YearLengthChoice;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxNotebook;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class VedicCalculationConfig;

/*************************************************//**
*
* \brief ConfigPanel for Vedic calculation parameters
*
******************************************************/
class VedicCalculationPanel: public ConfigPanel
{
	DECLARE_CLASS( VedicCalculationPanel )

public:
    // begin wxGlade: VedicCalculationPanel::ids
    // end wxGlade

	VedicCalculationPanel( wxWindow* );
	~VedicCalculationPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: VedicCalculationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	virtual void updateUi();
	void OnChoice( wxCommandEvent& );
	void OnVargaChoice( wxCommandEvent& );
	void OnVargaSignificationText( wxCommandEvent& );

	void OnKalaVelaLordChoice( wxCommandEvent& );
	void OnKalaVelaPortionChoice( wxCommandEvent& );


	VedicCalculationConfig *cconfig;

protected:
    // begin wxGlade: VedicCalculationPanel::attributes
    wxStaticBox* sizer_jaimini_staticbox;
    wxStaticBox* sizer_kalavelas_staticbox;
    wxStaticBox* sizer_yl_staticbox;
    wxStaticBox* sizer_nakshatra_staticbox;
    wxStaticBox* sizer_varga_significations_staticbox;
    wxStaticBox* sizer_varga_staticbox;
    wxStaticBox* sizer_benefics_staticbox;
    wxStaticBox* sizer_basic_staticbox;
    wxStaticText* label_ayanamsa;
    wxChoice* choice_ayanamsa;
    wxStaticText* label_vcalc_node;
    wxChoice* choice_node;
    wxStaticText* label_vcalc_house;
    wxChoice* choice_house;
    wxCheckBox* check_use_house_cusp;
    wxStaticText* label_moon;
    wxChoice* choice_moon;
    wxStaticText* label_mercury;
    wxChoice* choice_mercury;
    wxPanel* notebook_pane1;
    wxStaticText* label_tempfriend;
    wxChoice* choice_tempfriend;
    wxStaticText* label_hora;
    wxChoice* choice_hora;
    wxStaticText* label_drekkana;
    wxChoice* choice_drekkana;
    wxStaticText* label_chaturtamsa;
    wxChoice* choice_chaturtamsa;
    wxChoice* choice_vargas;
    wxTextCtrl* text_varga_signification;
    wxPanel* notebook_pane2;
    wxStaticText* label_nakdasa;
    wxChoice* choice_nakdasa;
    wxStaticText* label_kalachakra;
    wxChoice* choice_kala;
    wxStaticText* label_yogini_lords;
    wxChoice* choice_yogini_lords;
    wxStaticText* label_dasa_enddate;
    wxChoice* choice_dasa_show_date_mode;
    YearLengthChoice* choice_yl;
    wxTextCtrl* text_custom_yl;
    wxPanel* notebook_pane3;
    wxStaticText* label_kalavela;
    wxChoice* choice_kalavela;
    wxStaticText* label_kalalord;
    wxChoice* choice_kalalord;
    wxStaticText* label_kalaportion;
    wxChoice* choice_kalaportion;
    wxPanel* notebook_pane4;
    wxStaticText* label_av_calcrules;
    wxChoice* choice_av_calcrules;
    wxStaticText* label_av_benefics;
    wxChoice* choice_av_benefics;
    wxStaticText* label_av_sarva;
    wxChoice* choice_av_sarva;
    wxStaticText* label_av_sodhya;
    wxChoice* choice_av_sodhya;
    wxPanel* notebook_pane5;
    wxStaticText* label_arudha;
    wxChoice* choice_arudha;
    wxStaticText* label_karaka;
    wxChoice* choice_charakaraka;
    wxPanel* notebook_pane6;
    wxNotebook* notebook;
    // end wxGlade
}; // wxGlade: end class


#endif // VEDICCALCULATIONPANEL_H
