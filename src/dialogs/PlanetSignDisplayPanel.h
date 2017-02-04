/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PlanetSignDisplayPanel.h
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

#ifndef PLANETSIGNDISPLAYPANEL_H
#define PLANETSIGNDISPLAYPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class ChartProperties;
class Horoscope;
class WriterConfig;
class TextWidget;

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
    TextWidget* theText;
    wxPanel* panel_preview;
    // end wxGlade
// wxGlade: end class
};

#endif // VIEWPANEL_H
