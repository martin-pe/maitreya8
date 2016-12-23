/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternChartPanel.h
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

#ifndef WESTERNCHARTPANEL_H
#define WESTERNCHARTPANEL_H

#include "ConfigPanel.h"
#include "Conf.h"

// begin wxGlade: dependencies
// end wxGlade

class ChartGridWidget;
class ChartProperties;
class Horoscope;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxNotebook;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class WesternChartBehaviorConfig;

/*************************************************//**
*
* \brief Config panel for western chart parameters
*
******************************************************/
class WesternChartPanel: public ConfigPanel
{
	DECLARE_CLASS( ConfWesternChartPanel )

public:
    // begin wxGlade: WesternChartPanel::ids
    // end wxGlade

	WesternChartPanel( wxWindow* );
	~WesternChartPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: WesternChartPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	ChartProperties *chartprops;
	Horoscope *horoscope, *htransit;

	void OnCommand( wxCommandEvent& );
	void OnGuiElementChanged( const int id = -1 );
	void OnSpin( wxSpinEvent& );
	void OnChartSkinChanged( wxCommandEvent& );
	virtual void updateUi();

protected:
    // begin wxGlade: WesternChartPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_mouseover_staticbox;
    wxStaticBox* sizer_colors_staticbox;
    wxStaticBox* sizer_show_staticbox;
    wxStaticBox* sizer_chartoptions_staticbox;
    wxStaticBox* sizer_defaultskin_staticbox;
    wxChoice* choice_defaultskin;
    wxStaticText* label_orientation;
    wxChoice* choice_leftside;
    wxStaticText* label_transitstyle;
    wxChoice* choice_secondchart_style;
    wxStaticText* label_house_number_style;
    wxChoice* choice_house_number_style;
    wxCheckBox* check_retro;
    wxCheckBox* check_aspects;
    wxCheckBox* check_aspectsymbols;
    wxCheckBox* check_houses;
    wxPanel* notebook_pane_general;
    wxCheckBox* check_use_planet_colors;
    wxCheckBox* check_use_sign_colors;
    wxCheckBox* check_use_house_colors;
    wxCheckBox* check_use_aspect_colors;
    wxPanel* notebook_pane_color;
    wxCheckBox* check_zoom;
    wxSpinCtrl* spin_zoomfactor;
    wxStaticText* label_percent;
    wxCheckBox* check_magnify_aspects;
    wxPanel* notebook_pane_responsive;
    wxNotebook* notebook;
    ChartGridWidget* window_preview;
    wxPanel* panel_preview;
    wxCheckBox* check_secondchart;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNCHARTPANEL_H
