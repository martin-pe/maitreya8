/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PlanetSignDisplayPanel.h
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

#ifndef PLANETSIGNDISPLAYPANEL_H
#define PLANETSIGNDISPLAYPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class ChartProperties;
class Horoscope;
class WriterConfig;
class SheetWidget;

class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticText;

/*************************************************//**
*
* \brief Config panel for view parameters
*
******************************************************/
class PlanetSignDisplayPanel: public ConfigPanel
{
	DECLARE_CLASS( PlanetSignDisplayPanel )

public:
    // begin wxGlade: PlanetSignDisplayPanel::ids
    // end wxGlade

	PlanetSignDisplayPanel( wxWindow* );
	~PlanetSignDisplayPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: PlanetSignDisplayPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChoice( wxCommandEvent& );
	virtual void updateUi();
	void writeTextContents();

	WriterConfig *cfg;
	ChartProperties *props;
	Horoscope *horoscope;

protected:
    // begin wxGlade: PlanetSignDisplayPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_signs_staticbox;
    wxStaticBox* sizer_positions_staticbox;
    wxStaticBox* sizer_planets_staticbox;
    wxCheckBox* check_planet_symbols;
    wxCheckBox* check_vedic_planet_names;
    wxStaticText* label_uranus;
    wxChoice* choice_uranus;
    wxStaticText* label_pluto;
    wxChoice* choice_pluto;
    wxCheckBox* check_vedic_positions;
    wxCheckBox* check_sign_symbols;
    wxCheckBox* check_vedic_sign_names;
    wxStaticText* label_capricorn;
    wxChoice* choice_capricorn;
    SheetWidget* theText;
    wxPanel* panel_preview;
    // end wxGlade
// wxGlade: end class
};

#endif // VIEWPANEL_H
