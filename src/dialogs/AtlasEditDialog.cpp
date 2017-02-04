/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasEditDialog.cpp
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

#include "AtlasEditDialog.h"

// begin wxGlade: ::extracode
// end wxGlade

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "AtlasLogic.h"
#include "AtlasAliasDialog.h"
#include "DialogElements.h"
#include "Lang.h"
#include "mspin.h"
#include "mvalidator.h"

enum { AE_COUNTRY = wxID_HIGHEST + 1, AE_ADMIN, AE_ALIASES, AE_TZNAME };

/*****************************************************
**
**   AtlasEditDialog   ---   Constructor
**
******************************************************/
AtlasEditDialog::AtlasEditDialog(wxWindow* parent, AtlasLogic *logic, const AtlasEntry entry )
 : wxDialog(parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ),
 logic( logic )
{
 model = entry;

    // begin wxGlade: AtlasEditDialog::AtlasEditDialog
    label_name = new wxStaticText(this, wxID_ANY, _("Name"));
    text_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_asciiname = new wxStaticText(this, wxID_ANY, _("ASCII Name"));
    text_asciiname = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_country = new wxStaticText(this, wxID_ANY, _("Country"));
    const wxString choice_country_choices[] = {};
    choice_country = new wxChoice(this, AE_COUNTRY, wxDefaultPosition, wxDefaultSize, 0, choice_country_choices);
    label_admincode = new wxStaticText(this, wxID_ANY, _("Admin Code"));
    const wxString choice_admin_choices[] = {};
    choice_admin = new wxChoice(this, AE_ADMIN, wxDefaultPosition, wxDefaultSize, 0, choice_admin_choices);
    label_population = new wxStaticText(this, wxID_ANY, _("Population"));
    text_population = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_longitude = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_longitude = new MDegSpin(this, wxID_ANY, 180);
    const wxString choice_ew_choices[] = {
        _("East"),
        _("West"),
    };
    choice_ew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ew_choices);
    label_latitude = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_latitude = new MDegSpin(this, wxID_ANY, 90);
    const wxString choice_ns_choices[] = {
        _("North"),
        _("South"),
    };
    choice_ns = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ns_choices);
    label_timezone = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    const wxString choice_timezone_choices[] = {};
    choice_timezone = new wxChoice(this, AE_TZNAME, wxDefaultPosition, wxDefaultSize, 0, choice_timezone_choices);
    label_tzh = new wxStaticText(this, wxID_ANY, _("TZ Hours"));
    label_tzhours = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_aliases = new wxStaticText(this, wxID_ANY, _("Aliases"));
    button_aliases = new wxButton(this, AE_ALIASES, _("Edit Aliases ..."));
    text_alias = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel_copy = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	text_name->SetValidator( MTextValidator( &model.name ));
	text_asciiname->SetValidator( MTextValidator( &model.asciiname ));
	text_population->SetValidator( MIntegerValidator( &model.population ));

	text_longitude->SetValidator( MDegreeValidator( &model.a_longitude, 180 ));
	choice_ew->SetValidator( MChoiceValidator( &model.c_longitude ));
	text_latitude->SetValidator( MDegreeValidator( &model.a_latitude, 90 ));
	choice_ns->SetValidator( MChoiceValidator( &model.c_latitude ));

	choice_country->SetStringSelection( model.country );
	updateAdminChoice();
	updateTzChoice();

	text_alias->SetValidator( MTextValidator( &model.aliases ));

	Connect( AE_COUNTRY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AtlasEditDialog::OnChoiceCountry ));
	Connect( AE_TZNAME, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AtlasEditDialog::OnChoiceTzName ));
	Connect( AE_ALIASES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasEditDialog::OnEntryAliases ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasEditDialog::OnOK ));
	Connect( wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AtlasEditDialog::OnTextChanged ));

	text_name->SetFocus();
}

/*****************************************************
**
**   AtlasEditDialog   ---   updateAdminChoice
**
******************************************************/
void AtlasEditDialog::updateAdminChoice()
{
	choice_admin->Clear();
	choice_admin->Append( wxEmptyString );

	const list<wxString> &l = logic->getAllAdminNamesForCountry( choice_country->GetStringSelection());
	for ( list<wxString>::const_iterator iter = l.begin(); iter != l.end(); iter++ )
	{
		choice_admin->Append( *iter );
	}
	if ( choice_admin->FindString( model.admin ) != wxNOT_FOUND ) choice_admin->SetStringSelection( model.admin );
	else choice_admin->SetSelection( 1 );
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnChoiceCountry
**
******************************************************/
void AtlasEditDialog::OnChoiceCountry( wxCommandEvent& )
{
	updateAdminChoice();
	updateTzChoice();
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnChoiceTzName
**
******************************************************/
void AtlasEditDialog::OnChoiceTzName( wxCommandEvent& )
{
	updateTzHours();
}

/*****************************************************
**
**   AtlasEditDialog   ---   updateTzHours
**
******************************************************/
void AtlasEditDialog::updateTzHours()
{
	wxString tzfullname = choice_timezone->GetStringSelection();

	for ( list<TimezoneEntry>::iterator iter = tzlist.begin(); iter != tzlist.end(); iter++ )
	{
		if ( (*iter).name == tzfullname )
		{
			label_tzhours->SetLabel( wxString::Format( wxT( "%02.1f" ), (*iter).tzoffset));
			return;
		}
	}
	printf( "ERROR: no time zone found for entry %s\n", str2char( tzfullname ));
}

/*****************************************************
**
**   AtlasEditDialog   ---   updateTzChoice
**
******************************************************/
void AtlasEditDialog::updateTzChoice()
{
	wxString country_code;
	wxString country = choice_country->GetStringSelection();

	// get country
	for ( list<AtlasCountry>::iterator iter = countrylist.begin(); iter != countrylist.end(); iter++ )
	{
		if ( (*iter).name == country )
		{
			country_code = (*iter).iso;
			break;
		}
	}

	choice_timezone->Clear();
	for ( list<TimezoneEntry>::iterator iter = tzlist.begin(); iter != tzlist.end(); iter++ )
	{
		if ( (*iter).country_code != country_code ) continue;

		wxString tzname = (*iter).name;
		choice_timezone->Append( tzname );
	}
	if ( choice_timezone->FindString( model.timezone ) != wxNOT_FOUND ) choice_timezone->SetStringSelection( model.timezone );
	else choice_timezone->SetSelection( 0 );

	updateTzHours();
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnEntryAliases
**
******************************************************/
void AtlasEditDialog::OnEntryAliases( wxCommandEvent& )
{
	AtlasAliasDialog dialog( this, model );
	if ( dialog.ShowModal() == wxID_OK )
	{
		text_alias->SetValue( model.aliases );
	}
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnOk
**
******************************************************/
void AtlasEditDialog::OnOK( wxCommandEvent& )
{
	TransferDataFromWindow();
	model.country = choice_country->GetStringSelection();
	model.country_code = logic->getCountryCodeForName( choice_country->GetStringSelection());

	model.admin = choice_admin->GetStringSelection();
	model.admin1_code = logic->getAdminCodeForCountryAndName( model.country_code, choice_admin->GetStringSelection());

	printf( "POP %d\n", model.population );
	EndModal( wxID_OK );
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnTextChanged
**
******************************************************/
void AtlasEditDialog::OnTextChanged( wxCommandEvent &event )
{
	Validate();
	event.Skip();
}

/*****************************************************
**
**   AtlasEditDialog   ---   set_properties
**
******************************************************/
void AtlasEditDialog::set_properties()
{
	countrylist = logic->getAllCountries();
	for ( list<AtlasCountry>::iterator iter = countrylist.begin(); iter != countrylist.end(); iter++ )
	{
		choice_country->Append( (*iter).name );
	}
	tzlist = logic->getAllTimezones();

    // begin wxGlade: AtlasEditDialog::set_properties
    SetTitle(_("Edit Atlas Entry"));
    choice_ew->SetMinSize(wxSize(100, -1));
    choice_ew->SetSelection(0);
    choice_ns->SetMinSize(wxSize(100, -1));
    choice_ns->SetSelection(0);
    text_alias->SetMinSize(wxSize(-1, 100));
    // end wxGlade
}

/*****************************************************
**
**   AtlasEditDialog   ---   do_layout
**
******************************************************/
void AtlasEditDialog::do_layout()
{
    // begin wxGlade: AtlasEditDialog::do_layout
    wxStaticBoxSizer* sizer_main = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxEmptyString), wxVERTICAL);
    wxStaticBoxSizer* sizer_bottom = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxEmptyString), wxHORIZONTAL);
    wxStaticBoxSizer* sizer_left = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Attributes")), wxVERTICAL);
    wxFlexGridSizer* grid_attributes = new wxFlexGridSizer(11, 2, 0, 0);
    wxBoxSizer* sizer_latitude = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_longitude = new wxBoxSizer(wxHORIZONTAL);
    grid_attributes->Add(label_name, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(text_name, 0, wxALL|wxEXPAND, 3);
    grid_attributes->Add(label_asciiname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(text_asciiname, 0, wxALL|wxEXPAND, 3);
    grid_attributes->Add(label_country, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(choice_country, 0, wxALL, 3);
    grid_attributes->Add(label_admincode, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(choice_admin, 0, wxALL, 3);
    grid_attributes->Add(label_population, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(text_population, 0, wxALL|wxEXPAND, 3);
    grid_attributes->Add(label_longitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    sizer_longitude->Add(text_longitude, 1, wxEXPAND, 0);
    sizer_longitude->Add(choice_ew, 0, wxALL, 3);
    grid_attributes->Add(sizer_longitude, 1, wxALL|wxEXPAND, 0);
    grid_attributes->Add(label_latitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    sizer_latitude->Add(text_latitude, 1, wxEXPAND, 0);
    sizer_latitude->Add(choice_ns, 0, wxALL, 3);
    grid_attributes->Add(sizer_latitude, 1, wxALL|wxEXPAND, 0);
    grid_attributes->Add(label_timezone, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(choice_timezone, 1, wxALL, 3);
    grid_attributes->Add(label_tzh, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(label_tzhours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(label_aliases, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_attributes->Add(button_aliases, 0, wxALIGN_RIGHT|wxALL, 3);
    grid_attributes->AddGrowableCol(1);
    sizer_left->Add(grid_attributes, 0, wxEXPAND, 0);
    sizer_left->Add(text_alias, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_left, 1, wxEXPAND, 0);
    sizer_bottom->Add(button_ok, 0, wxALL, 3);
    sizer_bottom->Add(button_cancel_copy, 0, wxALL, 3);
    sizer_main->Add(sizer_bottom, 0, wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

