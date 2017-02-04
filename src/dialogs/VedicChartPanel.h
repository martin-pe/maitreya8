/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicChartPanel.h
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

#ifndef VEDICCHARTPANEL_H
#define VEDICCHARTPANEL_H

#include "ConfigPanel.h"
#include "Conf.h"

// begin wxGlade: ::dependencies
#include <wx/spinctrl.h>
#include <wx/notebook.h>
// end wxGlade

class ChartProperties;

class BasicVedicChart;
class ChartGridWidget;
class VedicChartBehaviorConfig;
class Horoscope;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxNotebookEvent;
class wxStaticBox;
class wxStaticBoxSizer;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for Vedic chart parameters
*
******************************************************/
class VedicChartPanel: public ConfigPanel
{
	DECLARE_CLASS( ConfVedicChartPanel )

public:
    // begin wxGlade: VedicChartPanel::ids
    // end wxGlade

	VedicChartPanel( wxWindow* );
	~VedicChartPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: VedicChartPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	ChartProperties *chartprops;
	VedicChartDisplayConfig *displayConfig;
	Horoscope *horoscope, *htransit;

	void OnCommand( wxCommandEvent& );
	void OnSpin( wxSpinEvent& );
	void OnChartSkinChanged( wxCommandEvent& );
	void OnBenefics( wxCommandEvent& );
	void OnNotebook( wxNotebookEvent& );

	virtual void updateUi();
	void calculateStyle();

protected:
    // begin wxGlade: VedicChartPanel::attributes
    wxChoice* choice_defaultskin;
    wxCheckBox* check_retro;
    wxCheckBox* check_use_colors;
    wxCheckBox* check_highlight;
    wxCheckBox* check_zoom;
    wxSpinCtrl* spin_zoomfactor;
    wxStaticText* label_percent;
    wxPanel* notebook_pane_general;
    wxStaticText* label_chartstyle;
    wxChoice* choice_chart;
    wxStaticText* label_chartcenter;
    wxChoice* choice_chart_center;
    wxCheckBox* check_arudhas;
    wxCheckBox* check_ashtakavarga;
    wxStaticText* label_north_style;
    wxChoice* choice_north_chart;
    wxStaticText* label_markup_ascendant;
    wxChoice* choice_markup_ascendant;
    wxPanel* notebook_pane_vargachart;
    wxCheckBox* check_sanskrit_symbols;
    wxCheckBox* check_nakshatra_qualities;
    wxPanel* notebook_pane_sbc;
    wxStaticText* label_aspected;
    wxChoice* choice_aspected;
    wxStaticText* label_aspecting;
    wxChoice* choice_aspecting;
    wxStaticText* label_all_aspects;
    wxChoice* choice_all_aspects;
    wxStaticText* label_aspect_type;
    wxChoice* choice_aspect_type;
    wxStaticText* label_minimum_strength;
    wxChoice* choice_minimum_strength;
    wxStaticText* label_benefic;
    wxChoice* choice_benefic;
    wxCheckBox* check_include_asmc;
    wxPanel* notebook_pane_responsive;
    wxNotebook* notebook;
    ChartGridWidget* window_preview;
    wxPanel* panel_preview;
    wxCheckBox* checktransit;
    // end wxGlade
}; // wxGlade: end class


#endif // VEDICCHARTPANEL_H
