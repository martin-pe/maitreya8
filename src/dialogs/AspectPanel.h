/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AspectPanel.h
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
    wxChoice* choice_aspect_type;
    wxCheckBox* check_active;
    wxButton* button_pen;
    wxStaticText* label_orbis;
    wxSpinCtrl* spin_orbis;
    AspectPreviewPanel* preview_panel;
    // end wxGlade
}; // wxGlade: end class


#endif // ASPECTPANEL_H
