/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicPanel.h
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

#ifndef VEDICPANEL_H
#define VEDICPANEL_H

#include "ConfigPanel.h"
#include "maitreya.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class VedicConfig;

/*************************************************//**
*
* \brief ConfigPanel for Vedic objects
*
******************************************************/
class VedicPanel: public ConfigPanel
{
	DECLARE_CLASS( VedicPanel )

public:
    // begin wxGlade: VedicPanel::ids
    // end wxGlade

	VedicPanel( wxWindow* );
	~VedicPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: VedicPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnArabicParts( wxCommandEvent& );
	virtual void updateUi();

	VedicConfig *vconfig;

protected:
    // begin wxGlade: VedicPanel::attributes
    wxStaticBox* sizer_maincols_staticbox;
    wxStaticBox* sizer_order_lagna_staticbox;
    wxStaticBox* sizer_arabic_staticbox;
    wxStaticBox* sizer_partly_staticbox;
    wxStaticBox* sizer_sobjects_staticbox;
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
    wxCheckBox* check_objects_upagrahas;
    wxCheckBox* check_objects_kalavelas;
    wxCheckBox* check_objects_lagnas;
    wxCheckBox* check_objects_d9lagna;
    wxButton* button_arabic;
    wxStaticText* label_arabic_selection;
    wxPanel* panel_arabic_selection;
    wxChoice* choice_order_lagna;
    wxCheckBox* check_main_declination;
    wxCheckBox* check_main_lord;
    wxCheckBox* check_main_element;
    wxCheckBox* check_main_quality;
    wxCheckBox* check_main_signification;
    wxCheckBox* check_main_dignity;
    wxCheckBox* check_main_d9;
    wxCheckBox* check_main_nakshatra;
    wxCheckBox* check_main_pada;
    wxCheckBox* check_main_kp;
    wxCheckBox* check_main_d60;
    wxCheckBox* check_main_d10;
    wxCheckBox* check_main_housepos;
    wxCheckBox* check_main_karaka;
    wxCheckBox* check_main_av;
    // end wxGlade
}; // wxGlade: end class


#endif // VEDICPANEL_H
