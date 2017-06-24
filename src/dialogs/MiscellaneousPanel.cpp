/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/MiscellaneousPanel.cpp
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

#include "MiscellaneousPanel.h"

#include "Conf.h"
#include "guibase.h"
#include "IdConverter.h"
#include "mvalidator.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

extern Config *config;

IMPLEMENT_CLASS( MiscellaneousPanel, ConfigPanel )

/*****************************************************
**
**   MiscellaneousPanel   ---   Constructor
**
******************************************************/
MiscellaneousPanel::MiscellaneousPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	view = new ViewConfig();
	config2model();

    // begin wxGlade: MiscellaneousPanel::MiscellaneousPanel
    check_showstatusinfo = new wxCheckBox(this, wxID_ANY, _("Running Objects in Status Bar"));
    label_recent = new wxStaticText(this, wxID_ANY, _("Maximum of Recent Files"));
    spin_recent_files = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 4, 30);
    label_file_backup = new wxStaticText(this, wxID_ANY, _("Create Backup Files"));
    const wxString choice_file_backup_choices[] = {
        _("Never"),
        _("One"),
        _("Infinite"),
    };
    choice_file_backup = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_file_backup_choices);
    label_frame_width = new wxStaticText(this, wxID_ANY, _("Child View Frame Width"));
    spin_frame_width = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_log_target = new wxStaticText(this, wxID_ANY, _("Log Target"));
    const wxString choice_logging_choices[] = {
        _("Message Box"),
        _("Separate Window"),
        _("Part of Main Window"),
        _("Off"),
    };
    choice_logging = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_logging_choices);
    label_loglevel = new wxStaticText(this, wxID_ANY, _("Log Level"));
    const wxString choice_loglevel_choices[] = {
        _("Fatal"),
        _("Error"),
        _("Warning"),
        _("Message"),
        _("Status"),
        _("Info"),
    };
    choice_loglevel = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 6, choice_loglevel_choices);
    label_graphic_size_dialog = new wxStaticText(this, wxID_ANY, _("Image Size"));
    const wxString choice_graphic_size_dialog_choices[] = {
        _("Show size dialog"),
        _("Use view size"),
    };
    choice_graphic_size_dialog = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_graphic_size_dialog_choices);
    check_overwrite = new wxCheckBox(this, wxID_ANY, _("Ask on overwrite"));

    set_properties();
    do_layout();
    // end wxGlade

	// Main Window
	check_showstatusinfo->SetValidator( MCheckValidator( &view->showStatusInfo ));
	spin_recent_files->SetValidator( MSpinValidator( &view->maxRecentFiles ));

	// Left: miscellaneous
	choice_file_backup->SetValidator( MChoiceValidator( &view->backupFileMode ));
	spin_frame_width->SetValidator( MSpinValidator( &view->childViewFrameWidth ));

	// logging
	choice_logging->SetValidator( MChoiceValidator( &view->logMode ));
	choice_loglevel->SetValidator( MChoiceValidator( &view->logLevel ));

	// image and text export
	choice_graphic_size_dialog->SetValidator( MChoiceValidator( &view->graphicExportSizeMode ));
	check_overwrite->SetValidator( MCheckValidator( &view->exportAskOnOverwrite ));

#if wxUSE_STD_IOSTREAM && ! defined __WXMSW__
		choice_logging->Append( _( "Console" ));
#endif

}

/*****************************************************
**
**   MiscellaneousPanel   ---   Destructor
**
******************************************************/
MiscellaneousPanel::~MiscellaneousPanel()
{
	delete view;
}

/*****************************************************
**
**   MiscellaneousPanel   ---   config2model
**
******************************************************/
void MiscellaneousPanel::config2model()
{
	*view = *config->view;
}

/*****************************************************
**
**   MiscellaneousPanel   ---   model2config
**
******************************************************/
void MiscellaneousPanel::model2config()
{
	IdConverter *idc = IdConverter::get();
	wxLog::SetLogLevel( idc->logMode2WxLogLevel( config->view->logLevel ));

	if ( view->logMode != config->view->logMode )
	{
		doMessageBox( this, _( "Log Mode will be changed after restart." ));
	}
	*config->view = *view;
}

/*****************************************************
**
**   MiscellaneousPanel   ---   restoreDefaults
**
******************************************************/
void MiscellaneousPanel::restoreDefaults()
{
	*view = ViewConfig();
}

/*****************************************************
**
**   MiscellaneousPanel   ---   set_properties
**
******************************************************/
void MiscellaneousPanel::set_properties()
{
    // begin wxGlade: MiscellaneousPanel::set_properties
    choice_file_backup->SetSelection(0);
    choice_logging->SetSelection(0);
    choice_loglevel->SetSelection(0);
    choice_graphic_size_dialog->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   MiscellaneousPanel   ---   do_layout
**
******************************************************/
void MiscellaneousPanel::do_layout()
{
    // begin wxGlade: MiscellaneousPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right_copy = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_export = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Image and Text Export")), wxVERTICAL);
    wxBoxSizer* grid_export = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_logging = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Logging")), wxHORIZONTAL);
    wxFlexGridSizer* grid_logging = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_misc = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Miscellaneous")), wxVERTICAL);
    wxFlexGridSizer* grid_misc = new wxFlexGridSizer(3, 2, 3, 3);
    sizer_misc->Add(check_showstatusinfo, 0, wxALL|wxEXPAND, 3);
    grid_misc->Add(label_recent, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_misc->Add(spin_recent_files, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3);
    grid_misc->Add(label_file_backup, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_misc->Add(choice_file_backup, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3);
    grid_misc->Add(label_frame_width, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_misc->Add(spin_frame_width, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3);
    sizer_misc->Add(grid_misc, 1, wxALL, 3);
    sizer_left->Add(sizer_misc, 1, wxEXPAND, 0);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_logging->Add(label_log_target, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_logging->Add(choice_logging, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3);
    grid_logging->Add(label_loglevel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_logging->Add(choice_loglevel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3);
    grid_logging->AddGrowableCol(1);
    sizer_logging->Add(grid_logging, 1, wxEXPAND, 0);
    sizer_right_copy->Add(sizer_logging, 1, wxEXPAND, 0);
    grid_export->Add(label_graphic_size_dialog, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_export->Add(choice_graphic_size_dialog, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3);
    sizer_export->Add(grid_export, 1, wxALL|wxEXPAND, 3);
    sizer_export->Add(check_overwrite, 0, wxALL|wxEXPAND, 3);
    sizer_right_copy->Add(sizer_export, 1, wxALL|wxEXPAND, 3);
    sizer_right_copy->AddGrowableCol(0);
    sizer_main->Add(sizer_right_copy, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createMiscellaneousPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createMiscellaneousPanel( wxWindow *parent )
{
	return new MiscellaneousPanel( parent );
}

