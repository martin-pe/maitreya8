/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PrintoutPanel.h
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
    wxStaticBox* sizer_save_options_staticbox;
    wxStaticBox* sizer_custom_text_staticbox;
    wxStaticBox* sizer_viewer_staticbox;
    wxStaticBox* sizer_view_options_staticbox;
    wxStaticBox* sizer_printout_staticbox;
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
