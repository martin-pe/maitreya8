/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicPanel.h
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
    wxCheckBox* check_objects_outer;
    wxCheckBox* check_objects_dragonhead;
    wxCheckBox* check_objects_dragontail;
    wxCheckBox* check_objects_ascendant;
    wxCheckBox* check_objects_meridian;
    wxCheckBox* check_objects_descendant;
    wxCheckBox* check_objects_imumcoeli;
    wxCheckBox* check_objects_uranian_inner;
    wxCheckBox* check_objects_uranian_outer;
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
