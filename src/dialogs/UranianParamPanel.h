/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/UranianParamPanel.h
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

#ifndef URANIANPARAMPANEL_H
#define URANIANPARAMPANEL_H

#include <wx/panel.h>

#include "maitreya.h"

#ifndef APP_CATALOG
#define APP_CATALOG "app"  // replace with the appropriate catalog name
#endif

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( COMMAND_UPANEL_CHANGED, wxID_HIGHEST + 7641 )
END_DECLARE_EVENT_TYPES()

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinEvent;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticText;

class ChartProperties;
class GradkreisChoice;
class SortChoice;
class UranianConfig;

/*************************************************//**
*
* 
*
******************************************************/
class UranianParamPanel: public wxPanel
{
public:
    // begin wxGlade: UranianParamPanel::ids
    // end wxGlade

	UranianParamPanel( wxWindow* parent, int id, ChartProperties*, double *orbis );

private:
    // begin wxGlade: UranianParamPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnCommand( wxCommandEvent &event );
	void OnSpin( wxSpinEvent &event );
	void OnFilter( wxCommandEvent& );
	void OnClearFilter( wxCommandEvent& );

	void emitChangeEvent();
	void updateFilterLabel();

	ChartProperties *props;
	double *orbis;
	int iorbis;

protected:
    // begin wxGlade: UranianParamPanel::attributes
    wxStaticBox* sizer_filter_staticbox;
    wxStaticBox* sizer_include_staticbox;
    wxStaticBox* sizer_orbis_staticbox;
    wxStaticBox* sizer_sort_staticbox;
    wxStaticBox* sizer_gradkreis_staticbox;
    GradkreisChoice* choice_gradkreis;
    SortChoice* choice_sort;
    wxSpinCtrl* spin_orbis;
    wxCheckBox* check_include_midpoints;
    wxCheckBox* check_include_reflectionpoints;
    wxCheckBox* check_include_sums;
    wxCheckBox* check_include_differences;
    wxCheckBox* check_include_antiscia;
    wxStaticText* label_filter;
    wxPanel* panel_filter;
    wxButton* button_filter;
    wxButton* button_clear_filter;
    // end wxGlade
}; // wxGlade: end class


#endif // URANIANPARAMPANEL_H
