/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternCalculationPanel.h
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
    wxStaticBox* sizer_yl_staticbox;
    wxStaticBox* sizer_wcalc_staticbox;
    wxStaticText* label_wcalc_aya;
    wxChoice* choice_ayanamsa;
    wxStaticText* label_wcalc_node;
    wxChoice* choice_wnode;
    wxStaticText* label_wcalc_house;
    wxChoice* choice_whouse;
    YearLengthChoice* choice_yl;
    wxTextCtrl* text_custom_yl;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNCALCULATIONPANEL_H
