/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AspectPanel.h
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

#ifndef ASPECTPANEL_H
#define ASPECTPANEL_H

#include "ConfigPanel.h"

#include "AspectConfig.h"
#include "Aspect.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticBox;
class wxStaticText;
class AspectPreviewPanel;

#define MAX_ASPECT_PANEL_ITEMS 19

/*************************************************//**
*
* \brief ConfigPanel for aspect parameters
*
******************************************************/
class AspectPanel: public ConfigPanel
{
	DECLARE_CLASS( AspectPanel )

public:
    // begin wxGlade: AspectPanel::ids
    // end wxGlade

	AspectPanel( wxWindow* );
	~AspectPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: AspectPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	virtual void updateUi();
	void showPenDialog();

	void OnAspectChoice( wxCommandEvent& );
	void OnSelectedAspectChanged( wxCommandEvent& );
	void OnAspectDoubleClick( wxCommandEvent& );
	void OnCheckActive( wxCommandEvent& );
	void OnSpinOrbis( wxSpinEvent& );
	void OnButtonPen( wxCommandEvent& ) { showPenDialog(); }

	vector<AspectConfig> aspects;
	uint selconfig;

protected:
    // begin wxGlade: AspectPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_options_staticbox;
    wxStaticBox* sizer_type_staticbox;
    wxChoice* choice_aspect_type;
    wxCheckBox* check_active;
    wxButton* button_pen;
    wxStaticText* label_orbis;
    wxSpinCtrl* spin_orbis;
    AspectPreviewPanel* preview_panel;
    // end wxGlade
}; // wxGlade: end class


#endif // ASPECTPANEL_H
