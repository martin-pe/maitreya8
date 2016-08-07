/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ColorPanel.h
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
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_general_staticbox;
    wxStaticBox* sizer_mouseover_staticbox;
    wxStaticBox* sizer_object_staticbox;
    wxStaticBox* sizer_houses_staticbox;
    wxStaticBox* sizer_signs_staticbox;
    wxStaticBox* sizer_planets_staticbox;
    wxStaticBox* sizer_bgcolors_staticbox;
    wxStaticBox* sizer_fgcolors_staticbox;
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
