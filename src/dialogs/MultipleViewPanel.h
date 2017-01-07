/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/MultipleViewPanel.h
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

#ifndef MULTIPLEVIEWPANEL_H
#define MULTIPLEVIEWPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;
class MultipleViewConfiguration;

/*************************************************//**
*
* \brief ConfigPanel for multiple view parameters
*
******************************************************/
class MultipleViewPanel: public ConfigPanel
{
	DECLARE_CLASS( MultipleViewPanel )

public:
    // begin wxGlade: MultipleViewPanel::ids
    // end wxGlade

	MultipleViewPanel( wxWindow* );
	~MultipleViewPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: MultipleViewPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChoice( wxCommandEvent& );
	virtual void updateUi();

	MultipleViewConfiguration *mconfig;

protected:
    // begin wxGlade: MultipleViewPanel::attributes
    wxStaticBox* sizer_nbstyle_staticbox;
    wxStaticBox* sizer_mview_staticbox;
    wxCheckBox* check_use_mviews;
    wxStaticText* label_defview;
    wxChoice* choice_viewlist;
    wxStaticText* text_view_description;
    wxPanel* panel_description;
    wxStaticText* label_nbstyle;
    wxChoice* choice_nbstyle;
    wxStaticText* label_orientation;
    wxChoice* choice_orientation;
    // end wxGlade
}; // wxGlade: end class


#endif // MULTIPLEVIEWPANEL_H
