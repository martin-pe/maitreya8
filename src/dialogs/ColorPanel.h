/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ColorPanel.h
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

#ifndef COLORPANEL_H
#define COLORPANEL_H

#include "ConfigPanel.h"

#include <wx/colour.h>

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxNotebook;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

class AshtakavargaExpert;
class ChartProperties;
class ColorConfig;
class BarDiagramConfig;
class Horoscope;
class ObjectColorConfig;
class PlanetColorPreviewPanel;
class HouseFgColorPreviewPanel;
class HouseBgColorPreviewPanel;
class SignFgColorPreviewPanel;
class SignBgColorPreviewPanel;
class SheetWidget;

/*************************************************//**
*
* \brief ConfigPanel for color parameters
*
******************************************************/
class ColorPanel: public ConfigPanel
{
	DECLARE_CLASS( ColorPanel )

public:
    // begin wxGlade: ColorPanel::ids
    // end wxGlade

	ColorPanel( wxWindow* );
	~ColorPanel();

	void config2model();
	void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: ColorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	virtual void updateUi();

	// 1st tab
	void OnGlobalColorChanged( wxColourPickerEvent& );

	// 2nd tab
	void OnObjectColorChanged( wxColourPickerEvent& );
	void OnObjectChoice( wxCommandEvent& );
	void OnButtonSignBrush( wxCommandEvent& );
	void OnButtonHouseBrush( wxCommandEvent& );

	void updateUiObjectColors();
	int planetChoiceToIndex( const int &selection );

	// 3rd tab: bar panel
	void updateUiBarDiagram();

	void OnBarSpin( wxSpinEvent& ) { setDirty(); updateUiBarDiagram(); }
	void OnBarCheckBox( wxCommandEvent& ) { setDirty(); updateUiBarDiagram(); }
	void OnBarObjectChoice( wxCommandEvent& ) { updateUiBarDiagram(); }

	void OnButtonBarPen( wxCommandEvent& );
	void OnButtonBarBrush( wxCommandEvent& );

	ColorConfig *colorConfig;
	ObjectColorConfig *objectColors;
	BarDiagramConfig *barconfig;
	ChartProperties *props;

	Horoscope *horoscope;
  AshtakavargaExpert *expert;

protected:
    // begin wxGlade: ColorPanel::attributes
    wxStaticText* label_fg;
    wxColourPickerCtrl* button_color_fg;
    wxStaticText* label_benefic;
    wxColourPickerCtrl* button_color_benefic;
    wxStaticText* label_malefic;
    wxColourPickerCtrl* button_color_malefic;
    wxStaticText* label_transit;
    wxColourPickerCtrl* button_color_transit;
    wxStaticText* label_bg;
    wxColourPickerCtrl* button_color_bg;
    wxStaticText* label_marked;
    wxColourPickerCtrl* button_color_marked;
    wxStaticText* label_chart_markup;
    wxColourPickerCtrl* button_color_chart_markup;
    wxStaticText* label_error;
    wxColourPickerCtrl* button_color_error;
    wxPanel* notebook_pane_general;
    wxChoice* choice_planets;
    wxStaticText* label_fgplanets;
    wxColourPickerCtrl* color_fgplanets;
    PlanetColorPreviewPanel* preview_planetcolor;
    wxChoice* choice_signs;
    wxStaticText* label_fg_signs;
    wxColourPickerCtrl* color_fgsigns;
    SignFgColorPreviewPanel* preview_signfgcolor;
    wxStaticText* label_bg_signs;
    wxButton* button_signbrush;
    SignBgColorPreviewPanel* preview_signbgcolor;
    wxChoice* choice_houses;
    wxStaticText* label_fg_houses;
    wxColourPickerCtrl* color_fghouses;
    HouseFgColorPreviewPanel* preview_housefgcolor;
    wxStaticText* label_bg_houses;
    wxButton* button_housebrush;
    HouseBgColorPreviewPanel* preview_housebgcolor;
    wxPanel* notebook_pane_charts;
    wxStaticText* label_2;
    wxChoice* choice_object;
    wxButton* button_pen;
    wxButton* button_brush;
    wxCheckBox* check_zoom;
    wxSpinCtrl* spin_zoom;
    wxStaticText* label_pixel;
    wxCheckBox* check_showtoolbarmsg;
    wxStaticText* label_cornerradius;
    wxSpinCtrl* spin_cornerradius;
    wxStaticText* label_pixel2;
    SheetWidget* barwidget;
    wxPanel* notebook_pane_bar;
    wxNotebook* notebook_main;
    // end wxGlade
}; // wxGlade: end class


#endif // COLORFONTPANEL_H
