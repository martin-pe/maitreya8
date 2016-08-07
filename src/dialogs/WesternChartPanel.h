/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternChartPanel.h
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
    wxStaticBox* sizer_defaultview_staticbox;
    wxChoice* choice_defaultskin;
    wxStaticText* label_orientation;
    wxChoice* choice_leftside;
    wxStaticText* label_transitstyle;
    wxChoice* choice_transitstyle;
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
    wxCheckBox* check_transit;
    // end wxGlade
}; // wxGlade: end class


#endif // WESTERNCHARTPANEL_H
