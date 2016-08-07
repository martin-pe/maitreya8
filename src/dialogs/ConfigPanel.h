/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ConfigPanel.h
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

#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <wx/panel.h>
#include "maitreya.h"

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( CONFIG_CHANGED, wxID_HIGHEST + 1300 )
DECLARE_EVENT_TYPE( CONFIG_TOOLBAR_CHANGED, wxID_HIGHEST + 1301 )
DECLARE_EVENT_TYPE( CONFIG_PANEL_CHANGED, wxID_HIGHEST + 1302 )
END_DECLARE_EVENT_TYPES()

enum {
	CONFIGPANEL_GENERAL = 0,
	CONFIGPANEL_EPHEMERIS,
	CONFIGPANEL_MULTIPLEVIEW,
	CONFIGPANEL_ATLAS,
	CONFIGPANEL_PRINTOUT,
	CONFIGPANEL_ANIMATION,

	CONFIGPANEL_SHEETSTYLE,
	CONFIGPANEL_PLANETSIGNDISPLAY,
	CONFIGPANEL_COLOR,
	CONFIGPANEL_FONT,
	CONFIGPANEL_TOOLBAR,

	CONFIGPANEL_VEDIC,
	CONFIGPANEL_VEDICCHART,
	CONFIGPANEL_VEDICCALCULATION,

	CONFIGPANEL_WESTERN,
	CONFIGPANEL_WESTERNCHART,
	CONFIGPANEL_WESTERNCALCULATION,
	CONFIGPANEL_WESTERNASPECT,

#ifdef USE_URANIAN_CHART
	CONFIGPANEL_URANIAN,
	CONFIGPANEL_URANIANCHART,
	CONFIGPANEL_URANIANEVALUATION,
#endif

	CONFIGPANEL_MISCELLANEOUS
};
#define CONFIGPANEL_FIRST CONFIGPANEL_GENERAL
#define CONFIGPANEL_LAST CONFIGPANEL_MISCELLANEOUS

#define NB_PANELS CONFIGPANEL_LAST+1

class wxSpinEvent;

/*************************************************//**
*
* \brief Base class for all panels in ConfigurationDialog
*
******************************************************/
class ConfigPanel: public wxPanel
{
	DECLARE_CLASS( ConfigPanel )

public:

	ConfigPanel( wxWindow* parent, const bool showRestoreDefaultButton = true );

	virtual void model2config() = 0;

	virtual void updateUi() {}

	virtual void onActivate();

	virtual void onPassivate();

	bool isDirty() { return dirty; }
	void setDirty( const bool b = true );

	bool hasRestoreButton() const { return showDefaultButton; }

	virtual void restoreDefaults() = 0;

protected:

	virtual void config2model() = 0;

	void OnSize( wxSizeEvent& );

	void OnCommand( wxCommandEvent& );
	void OnSpin( wxSpinEvent& );
	void OnText( wxCommandEvent& );

private:

	const bool showDefaultButton;

	bool dirty;
};

/*************************************************//**
*
*
*
******************************************************/
class ConfigPanelFactory
{
public:
	ConfigPanel *createPanel( const int&, wxWindow* );
	wxString getPanelName( const int& );

private:
	ConfigPanel *createGeneralPanel( wxWindow* );
	ConfigPanel *createMultipleViewPanel( wxWindow* );
	ConfigPanel *createPrintoutPanel( wxWindow* );
	ConfigPanel *createEphemerisPanel( wxWindow* );
	ConfigPanel *createAtlasPanel( wxWindow* );
	ConfigPanel *createToolbarPanel( wxWindow* );
	ConfigPanel *createAnimationPanel( wxWindow* );
	ConfigPanel *createMiscellaneousPanel( wxWindow* );

	ConfigPanel *createPlanetSignDisplayPanel( wxWindow* );
	ConfigPanel *createColorPanel( wxWindow* );
	ConfigPanel *createSheetStylePanel( wxWindow* );
	ConfigPanel *createFontPanel( wxWindow* );

	ConfigPanel *createVedicPanel( wxWindow* );
	ConfigPanel *createVedicChartPanel( wxWindow* );
	ConfigPanel *createVedicCalculationPanel( wxWindow* );

	ConfigPanel *createWesternPanel( wxWindow* );
	ConfigPanel *createWesternChartPanel( wxWindow* );
	ConfigPanel *createWesternCalculationPanel( wxWindow* );
	ConfigPanel *createAspectPanel( wxWindow* );

	ConfigPanel *createUranianPanel( wxWindow* );
#ifdef USE_URANIAN_CHART
	ConfigPanel *createUranianChartPanel( wxWindow* );
	ConfigPanel *createUranianEvaluationPanel( wxWindow* );
#endif
};

#endif // CONFIGPANEL_H

