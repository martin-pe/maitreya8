/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DataDialog.cpp
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

#include "DataDialog.h"

#include <math.h>

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "AtlasDialog.h"
#include "Calculator.h"
#include "Conf.h"
#include "Document.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "mathbase.h"
#include "maitreya.h"
#include "mspin.h"
#include "mvalidator.h"

extern Config *config;

enum { DD_NOW = wxID_HIGHEST + 300, DD_DEFLOC, DD_SEARCHLOC, DD_LOCALTIME };

IMPLEMENT_CLASS( DataDialog, wxDialog )

/*************************************************//**
*
* \brief contains data for birth data dialog
*
******************************************************/
struct DataDialogModel : public LocationModel
{
	DataDialogModel() : LocationModel()
	{
		date = time = 0;
	}

	DataDialogModel( const Location &l ) : LocationModel( l )
	{
		date = time = 0;
	}

	wxString name, remark;
	double date, time;
};

/*****************************************************
**
**   DataDialog   ---   Constructor
**
******************************************************/
DataDialog::DataDialog( wxWindow* parent, Document *d )
	: wxDialog( parent, -1,wxEmptyString, wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE ),
		//: wxDialog( parent, -1, wxEmptyString ),
		doc( d )
{
	model = new DataDialogModel;
	doc2model();

    // begin wxGlade: DataDialog::DataDialog
    panel_date_formatted = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_date_staticbox = new wxStaticBox(this, wxID_ANY, _("Date"));
    sizer_location_staticbox = new wxStaticBox(this, wxID_ANY, _("Location"));
    sizer_tz_staticbox = new wxStaticBox(this, wxID_ANY, _("Time Zone"));
    sizer_remark_staticbox = new wxStaticBox(this, wxID_ANY, _("Remark"));
    sizer_name_staticbox = new wxStaticBox(this, wxID_ANY, _("Name"));
    text_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_date = new wxStaticText(this, wxID_ANY, _("Date"));
    text_date = new MDateSpin(this, wxID_ANY);
    label_datefmt = new wxStaticText(this, wxID_ANY, _("YYYY-MM-DD"));
    label_time = new wxStaticText(this, wxID_ANY, _("Local Time"));
    text_time = new MDegSpin(this, wxID_ANY);
    label_timefmt = new wxStaticText(this, wxID_ANY, _("HH:MM[:SS]"));
    label_date_formatted = new wxStaticText(panel_date_formatted, wxID_ANY, _("date"));
    label_calendar = new wxStaticText(panel_date_formatted, wxID_ANY, _("calendar"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    button_now = new wxButton(this, DD_NOW, _("Now"));
    label_locname = new wxStaticText(this, wxID_ANY, _("Name"));
    text_locname = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_long = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_longitude = new MDegSpin(this, wxID_ANY);
    label_long_fmt = new wxStaticText(this, wxID_ANY, _("DD:MM[:SS]"));
    const wxString choice_ew_choices[] = {
        _("East"),
        _("West")
    };
    choice_ew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ew_choices, 0);
    label_lat = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_latitude = new MDegSpin(this, wxID_ANY);
    label_lat_fmt = new wxStaticText(this, wxID_ANY, _("DD:MM[:SS]"));
    const wxString choice_ns_choices[] = {
        _("North"),
        _("South")
    };
    choice_ns = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ns_choices, 0);
    button_defloc = new wxButton(this, DD_DEFLOC, _("Standard Location"));
    button_searchloc = new wxButton(this, DD_SEARCHLOC, _("Search Location"));
    label_tz = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    text_tz = new MDegSpin(this, wxID_ANY);
    label_tzfmt = new wxStaticText(this, wxID_ANY, _("HH:MM[:SS]"));
    const wxString choice_tzew_choices[] = {
        _("East"),
        _("West")
    };
    choice_tzew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_tzew_choices, 0);
    label_dst = new wxStaticText(this, wxID_ANY, _("DST"));
    text_dst = new MDegSpin(this, wxID_ANY);
    label_dstfmt = new wxStaticText(this, wxID_ANY, _("HH:MM[:SS]"));
    button_localtime = new wxButton(this, DD_LOCALTIME, _("Local Time"));
    text_remark = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_apply = new wxButton(this, wxID_APPLY, _("Apply"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	text_name->SetValidator( MTextValidator( &model->name ));
	text_date->SetValidator( MDateValidator( &model->date ));
	text_time->SetValidator( MDegreeValidator( &model->time, 24 ));
	text_locname->SetValidator( MTextValidator( &model->locname ));

	text_longitude->SetValidator( MDegreeValidator( &model->longitude, 180 ));
	choice_ew->SetValidator( MChoiceValidator( &model->c_longitude ));
	text_latitude->SetValidator( MDegreeValidator( &model->latitude, 90 ));
	choice_ns->SetValidator( MChoiceValidator( &model->c_latitude ));

	text_tz->SetValidator( MDegreeValidator( &model->tz, 12 ));
	choice_tzew->SetValidator( MChoiceValidator( &model->c_tz ));

	text_dst->SetValidator( MDegreeValidator( &model->dst, 12 ));
	text_remark->SetValidator( MTextValidator( &model->remark ));

	updateDateDetailFields( true );
	SetIcon( ImageProvider::get()->getIcon( BM_EDIT ));
	text_name->SetFocus();

	Connect( DD_NOW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnSetCurrentDate ));
	Connect( DD_DEFLOC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnDefaultLocation ));
	Connect( DD_SEARCHLOC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnSearchLocation ));
	Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnApply ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnOK ));
	Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnCancel ));
	Connect( DD_LOCALTIME, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DataDialog::OnLocalTime ));
	Connect( wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DataDialog::OnInputControlChanged ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DataDialog::OnInputControlChanged ));
	Connect( wxID_ANY, COMMAND_SPIN_CHANGED, wxCommandEventHandler( DataDialog::OnInputControlChanged ));

	if ( IS_VALID_SIZE( config->viewprefs->sizes.sDataDialog )) SetSize( config->viewprefs->sizes.sDataDialog );
}

/*****************************************************
**
**   DataDialog   ---   Destructor
**
******************************************************/
DataDialog::~DataDialog()
{
	config->viewprefs->sizes.sDataDialog = GetSize();
	delete model;
}

/*****************************************************
**
**   DataDialog   ---   model2gui
**
******************************************************/
void DataDialog::model2gui()
{
	printf( "DataDialog::model2gui\n" );
	TransferDataToWindow();
}

/*****************************************************
**
**   DataDialog   ---   gui2model
**
******************************************************/
void DataDialog::gui2model()
{
	printf( "DataDialog::gui2model\n" );
	TransferDataFromWindow();
}

/*****************************************************
**
**   DataDialog   ---   doc2Model
**
******************************************************/
void DataDialog::doc2model()
{
	*model = DataDialogModel( *doc->getLocation());

	model->name = doc->getHName();

	Location *loc = doc->getLocation();
	const double tmpjd = doc->getJD() + ( loc->getTimeZone() + loc->getDST() ) / 24.0;
	model->date = floor( tmpjd + .5 ) - .5;
	model->time = a_red(( tmpjd - floor( tmpjd ) + .5 ) * 24, 24 );

	model->remark = doc->getRemark();
}

/*****************************************************
**
**   DataDialog   ---   model2doc
**
******************************************************/
void DataDialog::model2doc()
{
	doc->setHName( model->name );
	doc->setDate( model->date + ( model->time - model->getTimezone() - model->dst ) / 24.0 );

	doc->setLocation( model->locname,
		model->getLongitude(),
		model->getLatitude(),
		model->getTimezone(),
		model->dst );
	doc->setRemark( model->remark );
}

/*****************************************************
**
**   DataDialog   ---   OnInputControlChanged
**
******************************************************/
void DataDialog::OnInputControlChanged( wxCommandEvent &event )
{
	printf( "1111 DataDialog :::: OnInputControlChanged\n" );
	bool b = Validate();
	printf( "2222 DataDialog :::: OnInputControlChanged b is %d\n", b );

	if ( b )
	{
		// valid
		TransferDataFromWindow();
	}
	updateDateDetailFields( b );
	event.Skip();
}

/*****************************************************
**
**   DataDialog   ---   updateDateDetailFields
**
******************************************************/
void DataDialog::updateDateDetailFields( const bool isvalid )
{
	printf( "DataDialog::updateDateDetailFields b %d\n", isvalid );
	if ( isvalid )
	{
		DateTimeFormatter *formatter = DateTimeFormatter::get();
		Calculator *calculator = CalculatorFactory().getCalculator();

		const double jd = model->date + ( model->time - model->getTimezone() - model->dst ) / 24.0;

		printf( "JD %9.9f\n", jd );
		const int format = DF_INCLUDE_YEAR_BC_AD | DF_INCLUDE_TIME | DF_INCLUDE_TZOFFSET;

		//printf( "DATA: %s\n", str2char( formatter->getFullDateStringWithTzOffset( jd , model->tz + model->dst, DF_SIGNS_STRING)));
		//label_date_formatted->SetLabel( formatter->getFullDateStringWithTzOffset( jd , model->tz + model->dst, DF_SIGNS_STRING));
		label_date_formatted->SetLabel( formatter->formatDateString( jd , model->getTimezone() + model->dst, format ));
		label_calendar->SetLabel( calculator->isGregorianCalendarDate( jd ) ? _( "Gregorian Calendar" ) : _( "Julian Calendar" ));
	}
	else
	{
		printf( "SET TO INVLAID\n" );
		label_date_formatted->SetLabel( _( "invalid value" ));
		label_calendar->SetLabel( wxEmptyString );
	}
}

/*****************************************************
**
**   DataDialog   ---   OnCancel
**
******************************************************/
void DataDialog::OnCancel( wxCommandEvent& )
{
	EndModal( wxID_CANCEL );
}

/*****************************************************
**
**   DataDialog   ---   OnOK
**
******************************************************/
void DataDialog::OnOK( wxCommandEvent& )
{
	if ( doApply() )
	{
		EndModal( wxID_OK );
	}
}

/*****************************************************
**
**   DataDialog   ---   OnSetCurrentDate
**
******************************************************/
void DataDialog::OnSetCurrentDate( wxCommandEvent& )
{
	TzUtil tzu;
	TzInfo info = tzu.getCurrentTzInfo();

	model->tz = fabs( info.tzhours );
	model->c_tz = info.tzhours < 0 ? 1 : 0;
	model->dst = info.dsthours;
	double jd = MDate::getCurrentJD();
	double tmpjd = jd + ( info.tzhours + info.dsthours ) / 24.0;
	model->date = floor( tmpjd + .5 );
	model->time = a_red(( tmpjd - floor( tmpjd ) + .5 ) * 24, 24 );

	model2gui();
	updateDateDetailFields();
}

/*****************************************************
**
**   DataDialog   ---   doApply
**
******************************************************/
bool DataDialog::doApply()
{
	if ( ! Validate() ) return false;
	gui2model();
	model2doc();
	doc->update();
	doc->updateAllChildWindows();
	doc->setDirty( true );
	doc->emitDocUpdatedEvent();

	return true;
}

/*****************************************************
**
**   DataDialog   ---   OnDefaultLocation
**
******************************************************/
void DataDialog::OnDefaultLocation( wxCommandEvent& )
{
	TzUtil tzu;

	gui2model();
	double jd = model->date + ( model->time - model->getTimezone() - model->dst ) / 24.0;
	TzInfo info = tzu.calculateTzInfoForJD( jd );

	const Location *loc = config->defaultLocation;
	model->locname = loc->getLocName();

	model->longitude = fabs( loc->getLongitude() );
	model->c_longitude = loc->getLongitude() < 0 ? 1 : 0;
	model->latitude = fabs( loc->getLatitude() );
	model->c_latitude = loc->getLatitude() < 0 ? 1 : 0;

	model->tz = fabs( info.tzhours );
	model->c_tz = info.tzhours < 0 ? 1 : 0;
	model->dst = info.dsthours;

	model2gui();
	updateDateDetailFields();
}

/*****************************************************
**
**   DataDialog   ---   OnSearchLocation
**
******************************************************/
void DataDialog::OnSearchLocation( wxCommandEvent& )
{
	AtlasDialogWrapper dialog( this, ATLASDIALOG_CALLER_DATADIALOG, text_locname->GetValue() );
	if ( dialog.run() == wxID_OK )
	{
		AtlasEntry *e = dialog.getSelectedEntry();
		printf( "HALLO %s\n", str2char( e->name ));

		gui2model();
		model->locname = e->name;
		printf( "HALLO2 %s\n", str2char( model->locname ));

		model->longitude = e->a_longitude;
		model->c_longitude = e->c_longitude;
		model->latitude = e->a_latitude;
		model->c_latitude = e->c_latitude;

		model->tz = fabs( e->tzoffset );
		model->c_tz = e->tzoffset < 0 ? 1 : 0;

		printf( "HALLO1\n" );
		model2gui();
		printf( "HALLO2\n" );
		updateDateDetailFields();
	}
}

/*****************************************************
**
**   DataDialog   ---   OnLocalTime
**
******************************************************/
void DataDialog::OnLocalTime( wxCommandEvent& )
{
	if ( !Validate() ) return;
	gui2model();
	model->tz = fabs( model->longitude ) / 15.0;
	model->c_tz = model->longitude < 0 ? 1 : 0;
	model->dst = 0;

	model2gui();
	updateDateDetailFields();
}

/*****************************************************
**
**   DataDialog   ---   set_properties
**
******************************************************/
void DataDialog::set_properties()
{
    // begin wxGlade: DataDialog::set_properties
    SetTitle(_("Birth Data"));
    text_name->SetMinSize(wxSize(300, 22));
    button_now->SetToolTip(_("Set current date"));
    choice_ew->SetSelection(0);
    choice_ns->SetSelection(0);
    button_defloc->SetToolTip(_("Set standard location"));
    button_searchloc->SetToolTip(_("Search location in database"));
    choice_tzew->SetSelection(0);
    button_localtime->SetToolTip(_("Set time zone for local time"));
    text_remark->SetMinSize(wxSize(300,80));
    button_ok->SetToolTip(_("Apply data and close dialog"));
    button_ok->SetDefault();
    button_apply->SetToolTip(_("Apply changes and recalculate chart with new data"));
    button_cancel->SetToolTip(_("Cancel changes and close dialog"));
    // end wxGlade
}


/*****************************************************
**
**   DataDialog   ---   do_layout
**
******************************************************/
void DataDialog::do_layout()
{
    // begin wxGlade: DataDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(4, 1, 0, 0);
    wxFlexGridSizer* sizer_buttons = new wxFlexGridSizer(1, 9, 0, 0);
    sizer_remark_staticbox->Lower();
    wxStaticBoxSizer* sizer_remark = new wxStaticBoxSizer(sizer_remark_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_remark = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* grid_input_fields = new wxFlexGridSizer(3, 2, 3, 3);
    wxBoxSizer* sizer_tz_actions = new wxBoxSizer(wxVERTICAL);
    sizer_tz_staticbox->Lower();
    wxStaticBoxSizer* sizer_tz = new wxStaticBoxSizer(sizer_tz_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_tz = new wxFlexGridSizer(2, 4, 3, 3);
    wxBoxSizer* sizer_location_actions = new wxBoxSizer(wxVERTICAL);
    sizer_location_staticbox->Lower();
    wxStaticBoxSizer* sizer_location = new wxStaticBoxSizer(sizer_location_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_locatino_params = new wxFlexGridSizer(2, 4, 0, 0);
    wxFlexGridSizer* sizer_locname = new wxFlexGridSizer(1, 2, 0, 0);
    wxBoxSizer* sizer_date_actions = new wxBoxSizer(wxVERTICAL);
    sizer_date_staticbox->Lower();
    wxStaticBoxSizer* sizer_date = new wxStaticBoxSizer(sizer_date_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_date_formatted = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* grid_datetime = new wxFlexGridSizer(2, 3, 3, 3);
    sizer_name_staticbox->Lower();
    wxStaticBoxSizer* sizer_name = new wxStaticBoxSizer(sizer_name_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_name = new wxFlexGridSizer(1, 1, 0, 0);
    grid_sizer_name->Add(text_name, 0, wxALL|wxEXPAND, 3);
    grid_sizer_name->AddGrowableCol(0);
    sizer_name->Add(grid_sizer_name, 1, wxEXPAND, 0);
    sizer_main->Add(sizer_name, 1, wxALL|wxEXPAND, 3);
    grid_datetime->Add(label_date, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(text_date, 1, wxALL|wxEXPAND, 3);
    grid_datetime->Add(label_datefmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(label_time, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->Add(text_time, 1, wxALL|wxEXPAND, 3);
    grid_datetime->Add(label_timefmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_datetime->AddGrowableCol(1);
    sizer_date->Add(grid_datetime, 1, wxALL|wxEXPAND, 3);
    sizer_date_formatted->Add(label_date_formatted, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_date_formatted->Add(label_calendar, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    panel_date_formatted->SetSizer(sizer_date_formatted);
    sizer_date_formatted->AddGrowableCol(0);
    sizer_date_formatted->AddGrowableCol(1);
    sizer_date->Add(panel_date_formatted, 0, wxALL|wxEXPAND, 3);
    grid_input_fields->Add(sizer_date, 1, wxALL|wxEXPAND, 3);
    sizer_date_actions->Add(button_now, 0, wxALL|wxEXPAND, 3);
    grid_input_fields->Add(sizer_date_actions, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_locname->Add(label_locname, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_locname->Add(text_locname, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_locname->AddGrowableCol(1);
    sizer_location->Add(sizer_locname, 0, wxALL|wxEXPAND, 3);
    grid_locatino_params->Add(label_long, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_locatino_params->Add(text_longitude, 1, wxALL|wxEXPAND, 3);
    grid_locatino_params->Add(label_long_fmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_locatino_params->Add(choice_ew, 0, wxALL|wxEXPAND, 3);
    grid_locatino_params->Add(label_lat, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_locatino_params->Add(text_latitude, 1, wxALL|wxEXPAND, 3);
    grid_locatino_params->Add(label_lat_fmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_locatino_params->Add(choice_ns, 0, wxALL|wxEXPAND, 3);
    grid_locatino_params->AddGrowableCol(1);
    sizer_location->Add(grid_locatino_params, 1, wxALL|wxEXPAND, 3);
    grid_input_fields->Add(sizer_location, 0, wxALL|wxEXPAND, 3);
    sizer_location_actions->Add(button_defloc, 0, wxALL|wxEXPAND, 3);
    sizer_location_actions->Add(button_searchloc, 0, wxALL|wxEXPAND, 3);
    grid_input_fields->Add(sizer_location_actions, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(label_tz, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(text_tz, 1, wxALL|wxEXPAND, 3);
    grid_tz->Add(label_tzfmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(choice_tzew, 0, wxALL, 3);
    grid_tz->Add(label_dst, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(text_dst, 1, wxALL|wxEXPAND, 3);
    grid_tz->Add(label_dstfmt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_tz->Add(20, 20, 0, 0, 0);
    grid_tz->AddGrowableCol(1);
    sizer_tz->Add(grid_tz, 1, wxALL|wxEXPAND, 3);
    grid_input_fields->Add(sizer_tz, 1, wxALL|wxEXPAND, 3);
    sizer_tz_actions->Add(button_localtime, 0, wxALL|wxEXPAND, 3);
    grid_input_fields->Add(sizer_tz_actions, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    grid_input_fields->AddGrowableCol(0);
    sizer_main->Add(grid_input_fields, 1, wxALL|wxEXPAND, 3);
    grid_sizer_remark->Add(text_remark, 0, wxALL|wxEXPAND, 3);
    grid_sizer_remark->AddGrowableRow(0);
    grid_sizer_remark->AddGrowableCol(0);
    sizer_remark->Add(grid_sizer_remark, 1, wxEXPAND, 0);
    sizer_main->Add(sizer_remark, 1, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_ok, 0, wxALL, 5);
    sizer_buttons->Add(button_apply, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_buttons->Add(button_cancel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_buttons->AddGrowableCol(0);
    sizer_buttons->AddGrowableCol(1);
    sizer_buttons->AddGrowableCol(2);
    sizer_buttons->AddGrowableCol(3);
    sizer_main->Add(sizer_buttons, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(2);
    sizer_main->AddGrowableCol(0);
    Layout();
    Centre();
    // end wxGlade
}



