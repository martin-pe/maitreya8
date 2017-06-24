/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/GeneralPanel.h
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

#ifndef GENERALPANEL_H
#define GENERALPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class Location;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class wxTimer;
class wxTimerEvent;
struct GeneralPanelModel;

/*************************************************//**
*
* \brief ConfigPanel for general parameters
*
******************************************************/
class GeneralPanel: public ConfigPanel
{
	DECLARE_CLASS( GeneralPanel )

public:
    // begin wxGlade: GeneralPanel::ids
    // end wxGlade

	GeneralPanel( wxWindow* );
	~GeneralPanel();

	virtual void restoreDefaults();

private:
    // begin wxGlade: GeneralPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnLanguageChoice( wxCommandEvent& );
	void OnSearchLocation( wxCommandEvent& );
	void OnTimer( wxTimerEvent& );
	void updateTzItems();

	void config2model();
	void model2config();

	virtual void onActivate();
	virtual void onPassivate();

	wxTimer *timer;
	GeneralPanelModel *model;

protected:
    // begin wxGlade: GeneralPanel::attributes
    wxStaticText* label_defstyle;
    wxChoice* choice_defstyle;
    wxStaticText* label_lang;
    wxChoice* choice_lang;
    wxCheckBox* check_opennewdoc;
    wxCheckBox* check_askonquit;
    wxTextCtrl* text_name;
    wxButton* button_search;
    wxStaticText* label_longitude;
    wxTextCtrl* text_longitude;
    wxChoice* choice_ew;
    wxStaticText* label_latitude;
    wxTextCtrl* text_latitude;
    wxChoice* choice_ns;
    wxStaticText* label_localtime;
    wxStaticText* text_localtime;
    wxStaticText* label_uttime;
    wxStaticText* text_uttime;
    wxStaticText* label_tzoffset;
    wxStaticText* text_tzoffset;
    wxStaticText* label_systemdst;
    wxStaticText* text_systemdst;
    // end wxGlade
}; // wxGlade: end class


#endif // GENERALPANEL_H
