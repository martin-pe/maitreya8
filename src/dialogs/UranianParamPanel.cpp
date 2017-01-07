/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/UranianParamPanel.cpp
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

#include "UranianParamPanel.h"

// begin wxGlade: ::extracode
// end wxGlade

#include "ChartProperties.h"
#include "Commands.h"
#include "Conf.h"
#include "DialogElements.h"
#include "Lang.h"
#include "mvalidator.h"
#include "ObjectFilterDialog.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

DEFINE_EVENT_TYPE( COMMAND_UPANEL_CHANGED )

#define FILTER_EXLUDE_LIST OI_DESCENDANT | OI_IMUMCOELI | OI_DRAGONTAIL | OI_4_HOUSES | OI_ALL_HOUSES 

/*****************************************************
**
**   UranianParamPanel   ---   Constructor
**
******************************************************/
UranianParamPanel::UranianParamPanel( wxWindow* parent, int id, ChartProperties *chartprops, double *orbis )
	: wxPanel( parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL),
		props( chartprops ),
		orbis( orbis )
{
	iorbis = (int)(*orbis * 60.0 );

    // begin wxGlade: UranianParamPanel::UranianParamPanel
    panel_filter = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_sort_staticbox = new wxStaticBox(this, wxID_ANY, _("Order"));
    sizer_orbis_staticbox = new wxStaticBox(this, wxID_ANY, _("Orbis"));
    sizer_include_staticbox = new wxStaticBox(this, wxID_ANY, _("Include"));
    sizer_filter_staticbox = new wxStaticBox(this, wxID_ANY, _("Filter"));
    sizer_gradkreis_staticbox = new wxStaticBox(this, wxID_ANY, _("Gradkreis"));
    choice_gradkreis = new GradkreisChoice(this, wxID_ANY);
    choice_sort = new SortChoice(this, wxID_ANY);
    spin_orbis = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB, 0, 600);
    check_include_midpoints = new wxCheckBox(this, wxID_ANY, _("Midpoints"));
    check_include_reflectionpoints = new wxCheckBox(this, wxID_ANY, _("Reflection Points"));
    check_include_sums = new wxCheckBox(this, wxID_ANY, _("Sums"));
    check_include_differences = new wxCheckBox(this, wxID_ANY, _("Differences"));
    check_include_antiscia = new wxCheckBox(this, wxID_ANY, _("Antiscia"));
    label_filter = new wxStaticText(panel_filter, wxID_ANY, _("No Filter"));
    button_filter = new wxButton(this, CMD_FILTER, _("Set Filter ..."));
    button_clear_filter = new wxButton(this, CMD_CLEAR_FILTER, _("Clear Filter"));

    set_properties();
    do_layout();
    // end wxGlade

	UranianConfig &uconfig = props->getUranianConfig();
	choice_gradkreis->SetValidator( MChoiceValidator( (int*)&uconfig.gradkreis ));
	choice_sort->SetValidator( MChoiceValidator( (int*)&uconfig.sortOrder ));
	spin_orbis->SetValidator( MSpinValidator( &iorbis ));
	//spin_orbis->SetValue( (int)( 60 * *orbis ));

	check_include_midpoints->SetValidator( MCheckValidator( &uconfig.eventsIncludeMidpoints ));
	check_include_reflectionpoints->SetValidator( MCheckValidator( &uconfig.eventsIncludeReflectionPoints ));
	check_include_sums->SetValidator( MCheckValidator( &uconfig.eventsIncludeSums ));
	check_include_differences->SetValidator( MCheckValidator( &uconfig.eventsIncludeDifferences ));
	//check_include_triples->SetValidator( MCheckValidator( &uconfig.eventsIncludeTriples ));
	check_include_antiscia->SetValidator( MCheckValidator( &uconfig.eventsIncludeAntiscia ));

	updateFilterLabel();
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( UranianParamPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( UranianParamPanel::OnSpin ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UranianParamPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( UranianParamPanel::OnCommand ));

	Connect( CMD_FILTER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UranianParamPanel::OnFilter ));
	Connect( CMD_CLEAR_FILTER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UranianParamPanel::OnClearFilter ));

	InitDialog();
}

/*****************************************************
**
**   UranianParamPanel   ---   Desstructor
**
******************************************************/
UranianParamPanel::~UranianParamPanel()
{
	*orbis = (double)iorbis / 60.0;
	//*orbis = (double)spin_orbis->GetValue() / 60.0;
	printf( "UranianParamPanel::Destructor orbis %f\n", *orbis );
}

/*****************************************************
**
**   UranianParamPanel   ---   OnCommand
**
******************************************************/
void UranianParamPanel::OnCommand( wxCommandEvent &event )
{
	event.Skip();
	*orbis = (double)iorbis / 60.0;
	//*orbis = (double)spin_orbis->GetValue() / 60.0;
	printf( "UranianParamPanel::OnCommand orbis %f iorbis %d\n", *orbis, iorbis );
	//printf( "UranianParamPanel::OnCommand\n" );
	emitChangeEvent();
}

/*****************************************************
**
**   UranianParamPanel   ---   OnSpin
**
******************************************************/
void UranianParamPanel::OnSpin( wxSpinEvent &event )
{
	event.Skip();
	*orbis = (double)iorbis / 60.0;
	//*orbis = (double)spin_orbis->GetValue() / 60.0;
	printf( "UranianParamPanel::OnSpin orbis %f iorbis %d\n", *orbis, iorbis );
	//printf( "UranianParamPanel::OnSpin orbis %f iorbis %d\n", *orbis, iorbis );
	emitChangeEvent();
}

/*****************************************************
**
**   UranianParamPanel   ---   OnFilter
**
******************************************************/
void UranianParamPanel::OnFilter( wxCommandEvent& )
{
	//printf( "UranianParamPanel::OnFilter\n" );
	vector<ObjectId> planets = props->getPlanetList( FILTER_EXLUDE_LIST );

	ObjectFilterDialog dialog( this, planets, props->getObjectFilter() );
	if ( dialog.ShowModal() == wxID_OK )
	{
		ObjectFilter filter = dialog.getFilter();
		props->setObjectFilter( filter );
		updateFilterLabel();
		emitChangeEvent();
	}
}

/*****************************************************
**
**   UranianParamPanel   ---   OnClearFilter
**
******************************************************/
void UranianParamPanel::OnClearFilter( wxCommandEvent& )
{
	//printf( "UranianParamPanel::OnClearFilter\n" );
	props->setObjectFilter( ObjectFilter());
	updateFilterLabel();
	emitChangeEvent();
}

/*****************************************************
**
**   UranianParamPanel   ---   emitChangeEvent
**
******************************************************/
void UranianParamPanel::emitChangeEvent()
{
	//printf( "EMIT\n" );
	wxCommandEvent e( COMMAND_UPANEL_CHANGED, GetId());
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   UranianParamPanel   ---   updateFilterLabel
**
******************************************************/
void UranianParamPanel::updateFilterLabel()
{
	Lang lang;
	ObjectFilter filter = props->getObjectFilter();
	const uint fmax = props->getPlanetList( FILTER_EXLUDE_LIST ).size();

	button_clear_filter->Enable( filter.size() != 0 && filter.size() != fmax ) ;

	if ( filter.size() == 0 || filter.size() == fmax ) 
	{
		label_filter->SetLabel( _( "No Filter" ));
	}
	else if ( filter.size() == 1 )
	{
		label_filter->SetLabel( lang.getObjectName( *filter.begin(), TF_LONG ));
	}
	else if ( filter.size() <  6 )
	{
		wxString s;
		uint i = 0;
		for( set<ObjectId>::iterator iter = filter.begin(); iter != filter.end(); iter++ )
		{
			s << lang.getObjectName( *iter, TF_MEDIUM );
			if ( ++i < filter.size()) s << wxT( "," );
		}
		label_filter->SetLabel( s );
	}
	else
	{
		label_filter->SetLabel( wxString::Format( wxT( "%u/%u" ), filter.size(), fmax ));
	}
}

/*****************************************************
**
**   UranianParamPanel   ---   set_properties
**
******************************************************/
void UranianParamPanel::set_properties()
{
    // begin wxGlade: UranianParamPanel::set_properties
    // end wxGlade
}

/*****************************************************
**
**   UranianParamPanel   ---   do_layout
**
******************************************************/
void UranianParamPanel::do_layout()
{
    // begin wxGlade: UranianParamPanel::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    sizer_filter_staticbox->Lower();
    wxStaticBoxSizer* sizer_filter = new wxStaticBoxSizer(sizer_filter_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    sizer_include_staticbox->Lower();
    wxStaticBoxSizer* sizer_include = new wxStaticBoxSizer(sizer_include_staticbox, wxVERTICAL);
    sizer_orbis_staticbox->Lower();
    wxStaticBoxSizer* sizer_orbis = new wxStaticBoxSizer(sizer_orbis_staticbox, wxVERTICAL);
    sizer_sort_staticbox->Lower();
    wxStaticBoxSizer* sizer_sort = new wxStaticBoxSizer(sizer_sort_staticbox, wxVERTICAL);
    sizer_gradkreis_staticbox->Lower();
    wxStaticBoxSizer* sizer_gradkreis = new wxStaticBoxSizer(sizer_gradkreis_staticbox, wxVERTICAL);
    sizer_gradkreis->Add(choice_gradkreis, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_gradkreis, 0, wxALL|wxEXPAND, 3);
    sizer_sort->Add(choice_sort, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_sort, 0, wxALL|wxEXPAND, 3);
    sizer_orbis->Add(spin_orbis, 0, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_orbis, 0, wxALL|wxEXPAND, 3);
    sizer_include->Add(check_include_midpoints, 0, wxALL, 3);
    sizer_include->Add(check_include_reflectionpoints, 0, wxALL, 3);
    sizer_include->Add(check_include_sums, 0, wxALL, 3);
    sizer_include->Add(check_include_differences, 0, wxALL, 3);
    sizer_include->Add(check_include_antiscia, 0, wxALL, 3);
    sizer_main->Add(sizer_include, 0, wxALL|wxEXPAND, 3);
    sizer_1->Add(label_filter, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_filter->SetSizer(sizer_1);
    sizer_filter->Add(panel_filter, 1, wxALL|wxEXPAND, 3);
    sizer_filter->Add(button_filter, 0, wxALL|wxEXPAND, 3);
    sizer_filter->Add(button_clear_filter, 0, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_filter, 0, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

