/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ConfigDialog.h
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

