/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ConfigDialog.h
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <wx/dialog.h>

#include "maitreya.h"
#include "ConfigPanel.h"

class ConfigPanel;
class wxBoxSizer;
class wxButton;
class wxPanel;

class wxTreebook;

/*************************************************//**
*
* \brief base class for config dialog
*
******************************************************/
class ConfigBaseDialog: public wxDialog
{
	DECLARE_CLASS( ConfigBaseDialog )
public:
	ConfigBaseDialog( wxWindow* parent );

protected:

	virtual void saveData() = 0;

	void OnOK( wxCommandEvent& );
	void OnCancel( wxCommandEvent& );
	void OnConfigPanelChanged ( wxCommandEvent& );

	wxBoxSizer* buttonsizer;
	wxButton *ok_button, *apply_button, *restore_button, *cancel_button;
};

/*************************************************//**
*
* \brief main class for notebook based config dialog
*
******************************************************/
class SimpleConfigDialog: public ConfigBaseDialog
{
	DECLARE_CLASS( SimlpeConfigDialog )

public:

	SimpleConfigDialog( wxWindow*, const int& );
	~SimpleConfigDialog();

protected:

	void OnApply( wxCommandEvent& );
	ConfigPanel *configpanel;
	virtual void saveData();
};


/*************************************************//**
*
* \brief main class for notebook based config dialog
*
******************************************************/
class ConfigDialog: public ConfigBaseDialog
{
	DECLARE_CLASS( ConfigDialog )

public:

	ConfigDialog( wxWindow* parent );
	~ConfigDialog();

protected:

	void OnApply( wxCommandEvent& );
	void OnRestoreDefaults( wxCommandEvent& );
	void OnSize( wxSizeEvent& );
	void doApply();
	virtual void saveData();

	ConfigPanel *showPanel( const int& );

	ConfigPanel *configpanel[NB_PANELS];
	wxPanel *panel[NB_PANELS];

	wxTreebook* notebook;

#ifdef _WX_V2_
	void OnTreebookChanging( class wxTreebookEvent& );
	void OnTreebookChanged( class wxTreebookEvent& );
#else
	void OnTreebookChanging( class wxBookCtrlEvent& );
	void OnTreebookChanged( class wxBookCtrlEvent& );
#endif
};

#endif // CONFIGDIALOG_H

