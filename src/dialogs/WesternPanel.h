/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternPanel.h
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


#ifndef WESTERNPANEL_H
#define WESTERNPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticText;
class WesternConfig;

/*************************************************//**
*
* \brief ConfigPanel for western objects
*
******************************************************/
class WesternPanel: public ConfigPanel
{
	DECLARE_CLASS( WesternPanel )

public:
    // begin wxGlade: WesternPanel::ids
    // end wxGlade

	WesternPanel( wxWindow* );
	~WesternPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: WesternPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnArabicParts( wxCommandEvent& );
	virtual void updateUi();

	WesternConfig *wconfig;

protected:
    // begin wxGlade: WesternPanel::attributes
    wxStaticBox* sizer_columns_staticbox;
    wxStaticBox* sizer_planetorder_staticbox;
    wxStaticBox* sizer_arabic_staticbox;
    wxStaticBox* sizer_partly_staticbox;
    wxStaticBox* sizer_eobjects_staticbox;
    wxCheckBox* check_objects_outer;
    wxCheckBox* check_objects_dragonhead;
    wxCheckBox* check_objects_dragontail;
    wxCheckBox* check_objects_ascendant;
    wxCheckBox* check_objects_meridian;
    wxCheckBox* check_objects_descendant;
    wxCheckBox* check_objects_imumcoeli;
    wxCheckBox* check_objects_uranian;
    wxCheckBox* check_objects_chiron;
    wxCheckBox* check_objects_pholus;
    wxCheckBox* check_objects_planetoids;
    wxCheckBox* check_objects_lilith;
    wxCheckBox* check_objects_house_cusp;
    wxCheckBox* check_objects_4house_cusps;
    wxCheckBox* check_objects_aries;
    wxButton* button_arabic;
    wxStaticText* label_arabic_selection;
    wxPanel* panel_arabic_selection;
    wxStaticText* label_planetorder;
    wxChoice* choice_planetorder;
    wxCheckBox* check_main_declinations;
    wxCheckBox* check_main_90deg;
    wxCheckBox* check_main_45deg;
    wxCheckBox* check_main_antiscia;
    wxCheckBox* check_main_housepos;
    wxCheckBox* check_main_lord;
    wxCheckBox* check_main_element;
    wxCheckBox* check_main_quality;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNPANEL_H
