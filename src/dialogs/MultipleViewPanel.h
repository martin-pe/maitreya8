/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/MultipleViewPanel.h
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

#ifndef MULTIPLEVIEWPANEL_H
#define MULTIPLEVIEWPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;
class MultipleViewConfiguration;

/*************************************************//**
*
* \brief ConfigPanel for multiple view parameters
*
******************************************************/
class MultipleViewPanel: public ConfigPanel
{
	DECLARE_CLASS( MultipleViewPanel )

public:
    // begin wxGlade: MultipleViewPanel::ids
    // end wxGlade

	MultipleViewPanel( wxWindow* );
	~MultipleViewPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: MultipleViewPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChoice( wxCommandEvent& );
	virtual void updateUi();

	MultipleViewConfiguration *mconfig;

protected:
    // begin wxGlade: MultipleViewPanel::attributes
    wxStaticBox* sizer_nbstyle_staticbox;
    wxStaticBox* sizer_mview_staticbox;
    wxCheckBox* check_use_mviews;
    wxStaticText* label_defview;
    wxChoice* choice_viewlist;
    wxStaticText* text_view_description;
    wxPanel* panel_description;
    wxStaticText* label_nbstyle;
    wxChoice* choice_nbstyle;
    wxStaticText* label_orientation;
    wxChoice* choice_orientation;
    // end wxGlade
}; // wxGlade: end class


#endif // MULTIPLEVIEWPANEL_H
