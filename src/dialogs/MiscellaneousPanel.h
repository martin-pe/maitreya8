/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/MiscellaneousPanel.h
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

#ifndef MISCELANEOUSPANEL_H
#define MISCELANEOUSPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class ViewConfig;

/*************************************************//**
*
* 
*
******************************************************/
class MiscellaneousPanel: public ConfigPanel
{
	DECLARE_CLASS( MiscellaneousPanel )

public:
    // begin wxGlade: MiscellaneousPanel::ids
    // end wxGlade

	MiscellaneousPanel( wxWindow* parent );
	~MiscellaneousPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: MiscellaneousPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	ViewConfig *view;

protected:
    // begin wxGlade: MiscellaneousPanel::attributes
    wxStaticBox* sizer_export_staticbox;
    wxStaticBox* sizer_logging_staticbox;
    wxStaticBox* sizer_misc_staticbox;
    wxCheckBox* check_showstatusinfo;
    wxSpinCtrl* spin_recent_files;
    wxChoice* choice_file_backup;
    wxSpinCtrl* spin_frame_width;
    wxChoice* choice_input_field_validation;
    wxChoice* choice_logging;
    wxChoice* choice_loglevel;
    wxChoice* choice_graphic_size_dialog;
    wxCheckBox* check_overwrite;
    // end wxGlade
}; // wxGlade: end class


#endif // MISCELANEOUSPANEL_H

