/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternCalculationPanel.h
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

#ifndef WESTERNCALCULATIONPANEL_H
#define WESTERNCALCULATIONPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class AyanamsaChoice;
class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;
class WesternCalculationConfig;
class YearLengthChoice;

/*************************************************//**
*
* \brief ConfigPanel for western calculation parameters
*
******************************************************/
class WesternCalculationPanel: public ConfigPanel
{
	DECLARE_CLASS( WesternCalculationPanel )

public:
    // begin wxGlade: WesternCalculationPanel::ids
    // end wxGlade

	WesternCalculationPanel( wxWindow* );
	~WesternCalculationPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: WesternCalculationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChoice( wxCommandEvent& );
	virtual void updateUi();

	WesternCalculationConfig *wconfig;

protected:
    // begin wxGlade: WesternCalculationPanel::attributes
    wxStaticText* label_wcalc_aya;
    wxChoice* choice_ayanamsa;
    wxStaticText* label_wcalc_node;
    wxChoice* choice_wnode;
    wxStaticText* label_wcalc_house;
    wxChoice* choice_whouse;
    wxStaticText* label_yl;
    YearLengthChoice* choice_yl;
    wxStaticText* label_custom_yl;
    wxTextCtrl* text_custom_yl;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNCALCULATIONPANEL_H
