/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/EphemerisPanel.cpp
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

#include "EphemerisPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dir.h>
#include <wx/dirdlg.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Calculator.h"
#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"
#include "mvalidator.h"

extern Config *config;
enum { CD_SEPATH = wxID_HIGHEST + 1, CD_TIMEMODEL, CD_COORDINATES, CD_POSITIONS, CD_AYA_CONSTANT, CD_T0, CD_AYAN_T0, CD_EPHEMFILE };

IMPLEMENT_CLASS( EphemerisPanel, ConfigPanel )

/*****************************************************
**
**   EphemerisPanel   ---   Constructor
**
******************************************************/
EphemerisPanel::EphemerisPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	ephem = new EphemConfig();
	config2model();

    // begin wxGlade: EphemerisPanel::EphemerisPanel
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    wxFlexGridSizer* sizer_aya = new wxFlexGridSizer(6, 1, 0, 0);
    sizer_left->Add(sizer_aya, 1, wxALL|wxEXPAND, 3);
    wxBoxSizer* sizer_date_t0 = new wxBoxSizer(wxHORIZONTAL);
    sizer_aya->Add(sizer_date_t0, 0, wxALL|wxEXPAND, 3);
    label_custom_aya_t0 = new wxStaticText(this, wxID_ANY, _("Date t0"));
    sizer_date_t0->Add(label_custom_aya_t0, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    text_t0 = new wxTextCtrl(this, CD_T0, wxEmptyString);
    sizer_date_t0->Add(text_t0, 1, wxALL|wxEXPAND, 3);
    panel_text_t0 = new wxPanel(this, wxID_ANY);
    sizer_aya->Add(panel_text_t0, 0, wxALL|wxEXPAND, 3);
    wxBoxSizer* sizer_text_t0 = new wxBoxSizer(wxHORIZONTAL);
    text_t0_formatted = new wxStaticText(panel_text_t0, wxID_ANY, _("0000-00-00 00:00:00"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizer_text_t0->Add(text_t0_formatted, 0, wxALL|wxEXPAND, 3);
    wxBoxSizer* sizer_aya_t0 = new wxBoxSizer(wxHORIZONTAL);
    sizer_aya->Add(sizer_aya_t0, 0, wxALL|wxEXPAND, 3);
    label_custom_aya_ayat0 = new wxStaticText(this, wxID_ANY, _("Ayanamsa at t0"));
    sizer_aya_t0->Add(label_custom_aya_ayat0, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    text_ayan_t0 = new wxTextCtrl(this, CD_AYAN_T0, wxEmptyString);
    sizer_aya_t0->Add(text_ayan_t0, 1, wxALL|wxEXPAND, 3);
    panel_aya_t0 = new wxPanel(this, wxID_ANY);
    sizer_aya->Add(panel_aya_t0, 1, wxEXPAND, 0);
    wxBoxSizer* sizer_text_aya_t0 = new wxBoxSizer(wxVERTICAL);
    text_ayan_t0_formatted = new wxStaticText(panel_aya_t0, wxID_ANY, _("00:00:00.000"));
    sizer_text_aya_t0->Add(text_ayan_t0_formatted, 0, wxALL|wxEXPAND, 3);
    wxBoxSizer* sizer_constant_period = new wxBoxSizer(wxHORIZONTAL);
    sizer_aya->Add(sizer_constant_period, 1, wxEXPAND, 0);
    check_period = new wxCheckBox(this, CD_AYA_CONSTANT, _("Constant Period"));
    sizer_constant_period->Add(check_period, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    text_period = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    sizer_constant_period->Add(text_period, 1, wxALL|wxEXPAND, 3);
    wxStaticBoxSizer* sizer_ephem_path = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Path for Ephemeris Data Files")), wxVERTICAL);
    sizer_left->Add(sizer_ephem_path, 1, wxALL|wxEXPAND, 3);
    button_sepath = new wxButton(this, CD_SEPATH, _("Choose ..."));
    sizer_ephem_path->Add(button_sepath, 0, wxALL, 5);
    text_sepath = new wxTextCtrl(this, CD_EPHEMFILE, wxEmptyString);
    text_sepath->SetToolTip(_("Only required for high precision calculation"));
    sizer_ephem_path->Add(text_sepath, 0, wxALL|wxEXPAND, 5);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    wxStaticBoxSizer* sizer_calc = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Calculation")), wxVERTICAL);
    sizer_right->Add(sizer_calc, 1, wxALL|wxEXPAND, 3);
    wxFlexGridSizer* grid_sizer_calc = new wxFlexGridSizer(3, 2, 3, 3);
    sizer_calc->Add(grid_sizer_calc, 1, wxALL|wxEXPAND, 3);
    label_timemodel = new wxStaticText(this, wxID_ANY, _("Time Model"));
    grid_sizer_calc->Add(label_timemodel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    const wxString choice_timemodel_choices[] = {
        _("Universal"),
        _("Ephemeris"),
    };
    choice_timemodel = new wxChoice(this, CD_TIMEMODEL, wxDefaultPosition, wxDefaultSize, 2, choice_timemodel_choices);
    choice_timemodel->SetSelection(0);
    grid_sizer_calc->Add(choice_timemodel, 0, wxALL|wxEXPAND, 3);
    label_coord = new wxStaticText(this, wxID_ANY, _("Coordinates"));
    grid_sizer_calc->Add(label_coord, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    const wxString choice_coord_choices[] = {
        _("Geocentric"),
        _("Topocentric"),
        _("Equatorial"),
        _("Barycentric"),
        _("Heliocentric"),
    };
    choice_coord = new wxChoice(this, CD_COORDINATES, wxDefaultPosition, wxDefaultSize, 5, choice_coord_choices);
    choice_coord->SetSelection(0);
    grid_sizer_calc->Add(choice_coord, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    label_pos = new wxStaticText(this, wxID_ANY, _("Positions"));
    grid_sizer_calc->Add(label_pos, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    const wxString choice_pos_choices[] = {
        _("Apparent"),
        _("True"),
    };
    choice_pos = new wxChoice(this, CD_POSITIONS, wxDefaultPosition, wxDefaultSize, 2, choice_pos_choices);
    choice_pos->SetSelection(0);
    grid_sizer_calc->Add(choice_pos, 0, wxALL|wxEXPAND, 3);
    wxStaticBoxSizer* sizer_sunrise = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Sunrise and Sunset")), wxVERTICAL);
    sizer_right->Add(sizer_sunrise, 1, wxALL, 3);
    wxFlexGridSizer* sizer_sunrise_disk = new wxFlexGridSizer(1, 2, 3, 3);
    sizer_sunrise->Add(sizer_sunrise_disk, 1, wxALL, 3);
    label_sunrise_disk = new wxStaticText(this, wxID_ANY, _("Reference Point"));
    sizer_sunrise_disk->Add(label_sunrise_disk, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    const wxString choice_sunrise_disk_choices[] = {
        _("Center of Disk"),
        _("Edge of Disk"),
    };
    choice_sunrise_disk = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_sunrise_disk_choices);
    choice_sunrise_disk->SetSelection(0);
    sizer_sunrise_disk->Add(choice_sunrise_disk, 0, wxEXPAND, 0);
    check_sunrise_refrac = new wxCheckBox(this, wxID_ANY, _("Use Refraction"));
    check_sunrise_refrac->SetToolTip(_("Checked: apparent position will be used"));
    sizer_sunrise->Add(check_sunrise_refrac, 0, wxALL|wxEXPAND, 3);
    
    sizer_sunrise_disk->AddGrowableCol(0);
    grid_sizer_calc->AddGrowableCol(1);
    sizer_right->AddGrowableCol(0);
    panel_aya_t0->SetSizer(sizer_text_aya_t0);
    panel_text_t0->SetSizer(sizer_text_t0);
    sizer_aya->AddGrowableCol(0);
    sizer_left->AddGrowableCol(0);
    sizer_main->AddGrowableCol(0);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade

	text_t0->SetValidator( MDoubleValidator( &ephem->custom_t0, MIN_EPHEM_JD, MAX_EPHEM_JD ));
	text_ayan_t0->SetValidator( MDoubleValidator( &ephem->custom_ayan_t0, 0, 360 ));

	check_period->SetValidator( MCheckValidator( &ephem->custom_aya_constant ));
	text_period->SetValidator( MDoubleValidator( &ephem->custom_aya_period, 0, 10000000 ));

	choice_sunrise_disk->SetValidator( MChoiceValidator( &ephem->sunrise_def ));
	check_sunrise_refrac->SetValidator( MCheckValidator( &ephem->sunrise_refrac ));
	text_sepath->SetValidator( MDirectoryValidator( &ephem->sePath ));

	choice_timemodel->SetValidator( MChoiceValidator( &ephem->useEphemerisTime ));
	choice_coord->SetValidator( MChoiceValidator( &ephem->ephemCoordinateSystem ));
	choice_pos->SetValidator( MChoiceValidator( &ephem->useTruePositions ));

	Connect( CD_SEPATH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EphemerisPanel::OnSelectSEPath ));
	Connect( CD_TIMEMODEL, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemerisPanel::OnChangeConfigParams ));
	Connect( CD_COORDINATES, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemerisPanel::OnChangeConfigParams ));
	Connect( CD_POSITIONS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemerisPanel::OnChangeConfigParams ));

	Connect( CD_AYA_CONSTANT, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EphemerisPanel::OnCheckConstantAyanamsa ));

	Connect( CD_T0, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EphemerisPanel::OnTextChanged ));
	Connect( CD_EPHEMFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EphemerisPanel::OnTextChanged ));
	Connect( CD_AYAN_T0, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EphemerisPanel::OnTextChanged ));
}

/*****************************************************
**
**   EphemerisPanel   ---   Destructor
**
******************************************************/
EphemerisPanel::~EphemerisPanel()
{
	delete ephem;
}

/*****************************************************
**
**   EphemerisPanel   ---   config2model
**
******************************************************/
void EphemerisPanel::config2model()
{
	*ephem = *config->ephem;
}

/*****************************************************
**
**   EphemerisPanel   ---   model2config
**
******************************************************/
void EphemerisPanel::model2config()
{
	*config->ephem = *ephem;
	Calculator *calculator = CalculatorFactory().getCalculator();
	calculator->init();
}

/*****************************************************
**
**   EphemerisPanel   ---   OnSelectSEPath
**
******************************************************/
void EphemerisPanel::OnSelectSEPath( wxCommandEvent& )
{
	wxDirDialog dialog( this, _( "Choose a directory" ), text_sepath->GetValue());
	if ( dialog.ShowModal() == wxID_OK )
	{
		text_sepath->SetValue( dialog.GetPath());
		setDirty();
	}
}

/*****************************************************
**
**   EphemerisPanel   ---   OnTextChanged
**
******************************************************/
void EphemerisPanel::OnTextChanged( wxCommandEvent& )
{
	updateUi();
	Validate();
	setDirty();
}

/*****************************************************
**
**   EphemerisPanel   ---   updateUi
**
******************************************************/
void EphemerisPanel::updateUi()
{
	//printf( "EphemerisPanel::updateUi\n" );
	Formatter *formatter = Formatter::get();
	DateTimeFormatter *df = DateTimeFormatter::get();

	text_period->Enable( check_period->GetValue() );

	if ( text_t0 != 0 && ! text_t0->GetValue().IsEmpty())
	{
		double custom_t0 = myatof( text_t0->GetValue() );
		wxString t0 = df->formatFullDateString( custom_t0 );
		if ( text_t0_formatted ) text_t0_formatted->SetLabel( t0 );
	}
	if ( text_ayan_t0  != 0 && ! text_ayan_t0->GetValue().IsEmpty())
	{
		double custom_ayan_t0 = myatof( text_ayan_t0->GetValue() );
		wxString ayan_t0 = formatter->getLenFormatted( custom_ayan_t0, DEG_PRECISION_MORE );
		if ( text_ayan_t0_formatted ) text_ayan_t0_formatted->SetLabel( ayan_t0 );
	}
	//Validate();
	//setDirty();
}

/*****************************************************
**
**   EphemerisPanel   ---   OnCheckConstantAyanamsa
**
******************************************************/
void EphemerisPanel::OnCheckConstantAyanamsa( wxCommandEvent& )
{
	//printf( "EphemerisPanel::OnCheckConstantAyanamsa\n" );
	updateUi();
	setDirty();
}

/*****************************************************
**
**   EphemerisPanel   ---   OnChangeConfigParams
**
******************************************************/
void EphemerisPanel::OnChangeConfigParams( wxCommandEvent& )
{
	static bool shown = false;
	if ( ! config->viewprefs->doNotShowAgainEphemConfigChange && ! shown )
	{
		DoNotShowAgainDialog dialog( this,
			_( "Warning: changes in calculation configuration may result in wrong calculation results!"),
			&config->viewprefs->doNotShowAgainEphemConfigChange );
		dialog.ShowModal();
		shown = true;
	}
	setDirty();
}

/*****************************************************
**
**   EphemerisPanel   ---   restoreDefaults
**
******************************************************/
void EphemerisPanel::restoreDefaults()
{
	*ephem = EphemConfig();
}

/*****************************************************
**
**   EphemerisPanel   ---   set_properties
**
******************************************************/
/*****************************************************
**
**   EphemerisPanel   ---   do_layout
**
******************************************************/
/*****************************************************
**
**   ConfigPanelFactory   ---   createEphemerisPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createEphemerisPanel( wxWindow *parent )
{
	return new EphemerisPanel( parent );
}

