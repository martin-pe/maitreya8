/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/GeneralPanel.h
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
    wxStaticBox* sizer_default_location_staticbox;
    wxStaticBox* sizer_tzsettings_staticbox;
    wxStaticBox* sizer_coordinates_staticbox;
    wxStaticBox* sizer_2_staticbox;
    wxStaticBox* sizer_general_staticbox;
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
