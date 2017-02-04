/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PrintoutPanel.h
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

#ifndef PRINTOUTPANEL_H
#define PRINTOUTPANEL_H

#include "ConfigPanel.h"

#include "PrintoutConfig.h"

// begin wxGlade: dependencies
// end wxGlade

class wxBitmapButton;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for printout parameters
*
******************************************************/
class PrintoutPanel: public ConfigPanel
{
	DECLARE_CLASS( PrintoutPanel )

public:
    // begin wxGlade: PrintoutPanel::ids
    // end wxGlade

	PrintoutPanel( wxWindow* );
	~PrintoutPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: PrintoutPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	virtual void updateUi();
	void OnCommandEvent( wxCommandEvent& );

	void OnShowCustomText( wxCommandEvent& );
	void OnCustomTextParamChange( wxCommandEvent& );
	void saveCustomTextParams( const int &sel );

	void OnTextChanged( wxCommandEvent& );
	void OnChooseCommand( wxCommandEvent& );
	void OnPrintTestPage( wxCommandEvent& );
	void OnAgenda( wxCommandEvent& );

	PdfDocumentConfig *pconfig;

protected:
    // begin wxGlade: PrintoutPanel::attributes
    wxChoice* choice_printout_type;
    wxStaticText* text_printout_description;
    wxPanel* panel_description;
    wxStaticText* label_sheetstyle;
    wxChoice* choice_sheetstyle;
    wxStaticText* label_vstyle;
    wxChoice* choice_vstyle;
    wxStaticText* label_wstyle;
    wxChoice* choice_wstyle;
    wxCheckBox* check_launchviewer;
    wxButton* button_launchcommand;
    wxTextCtrl* text_launchcommand;
    wxButton* button_test_page;
    wxChoice* choice_custom_text_type;
    wxCheckBox* check_doshow_custom_text;
    wxBitmapButton* button_agenda;
    wxTextCtrl* text_custom;
    wxStaticText* label_orientation;
    wxChoice* choice_orientation;
    wxCheckBox* check_line_separator;
    wxCheckBox* check_include_first_page;
    wxStaticText* label_paper;
    wxChoice* choice_paperformat;
    wxCheckBox* check_overwrite;
    // end wxGlade
}; // wxGlade: end class


#endif // PRINTOUTPANEL_H
