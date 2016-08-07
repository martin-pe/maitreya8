/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternPanel.cpp
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

#include "WesternPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "ArabicParts.h"
#include "Conf.h"
#include "mvalidator.h"

extern Config *config;

enum { CD_ARABIC = wxID_HIGHEST + 1  };

IMPLEMENT_CLASS( WesternPanel, ConfigPanel )

/*****************************************************
**
**   WesternPanel   ---   Constructor
**
******************************************************/
WesternPanel::WesternPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	wconfig = new WesternConfig();
	config2model();

    // begin wxGlade: WesternPanel::WesternPanel
    panel_arabic_selection = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_partly_staticbox = new wxStaticBox(this, wxID_ANY, _("Partly Displayed"));
    sizer_arabic_staticbox = new wxStaticBox(this, wxID_ANY, _("Arabic Parts"));
    sizer_planetorder_staticbox = new wxStaticBox(this, wxID_ANY, _("Order of Planets"));
    sizer_columns_staticbox = new wxStaticBox(this, wxID_ANY, _("Columns in Main Window"));
    sizer_eobjects_staticbox = new wxStaticBox(this, wxID_ANY, _("Ephemeris Objects"));
    check_objects_outer = new wxCheckBox(this, wxID_ANY, _("3 Outer Planets"));
    check_objects_dragonhead = new wxCheckBox(this, wxID_ANY, _("Dragon Head"));
    check_objects_dragontail = new wxCheckBox(this, wxID_ANY, _("Dragon Tail"));
    check_objects_ascendant = new wxCheckBox(this, wxID_ANY, _("Ascendant"));
    check_objects_meridian = new wxCheckBox(this, wxID_ANY, _("Meridian"));
    check_objects_descendant = new wxCheckBox(this, wxID_ANY, _("Descendant"));
    check_objects_imumcoeli = new wxCheckBox(this, wxID_ANY, _("Imum Coeli"));
    check_objects_uranian = new wxCheckBox(this, wxID_ANY, _("8 Uranian"));
    check_objects_chiron = new wxCheckBox(this, wxID_ANY, _("Chiron"));
    check_objects_pholus = new wxCheckBox(this, wxID_ANY, _("Pholus"));
    check_objects_planetoids = new wxCheckBox(this, wxID_ANY, _("4 Planetoids"));
    check_objects_lilith = new wxCheckBox(this, wxID_ANY, _("Lilith (Black Moon)"));
    check_objects_house_cusp = new wxCheckBox(this, wxID_ANY, _("All House Cusps"));
    check_objects_4house_cusps = new wxCheckBox(this, wxID_ANY, _("4 Cusps (2, 3, 11, 12)"));
    check_objects_aries = new wxCheckBox(this, wxID_ANY, _("Aries"));
    button_arabic = new wxButton(this, CD_ARABIC, _("Select Parts ..."));
    label_arabic_selection = new wxStaticText(panel_arabic_selection, wxID_ANY, _("No parts selected"));
    label_planetorder = new wxStaticText(this, wxID_ANY, _("Order"));
    const wxString choice_planetorder_choices[] = {
        _("Default"),
        _("Uranian")
    };
    choice_planetorder = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_planetorder_choices, 0);
    check_main_declinations = new wxCheckBox(this, wxID_ANY, _("Declination"));
    check_main_90deg = new wxCheckBox(this, wxID_ANY, _("90 Degree Longitude"));
    check_main_45deg = new wxCheckBox(this, wxID_ANY, _("45 Degree Longitude"));
    check_main_antiscia = new wxCheckBox(this, wxID_ANY, _("Antiscia"));
    check_main_housepos = new wxCheckBox(this, wxID_ANY, _("House Position"));
    check_main_lord = new wxCheckBox(this, wxID_ANY, _("Sign Lord"));
    check_main_element = new wxCheckBox(this, wxID_ANY, _("Element"));
    check_main_quality = new wxCheckBox(this, wxID_ANY, _("Quality (Movable etc.)"));

    set_properties();
    do_layout();
    // end wxGlade

	check_objects_outer->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_OUTER ));
	check_objects_dragonhead->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_DRAGONHEAD ));
	check_objects_dragontail->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_DRAGONTAIL ));
	check_objects_ascendant->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_ASCENDANT ));
	check_objects_meridian->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_MERIDIAN ));
	check_objects_descendant->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_DESCENDANT ));
	check_objects_imumcoeli->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_IMUMCOELI ));
	check_objects_uranian->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_URANIAN ));
	check_objects_chiron->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_CHIRON ));
	check_objects_pholus->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_PHOLUS ));
	check_objects_planetoids->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_PLANETOIDS ));
	check_objects_lilith->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_LILITH ));
	check_objects_house_cusp->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_ALL_HOUSES ));
	check_objects_4house_cusps->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_4_HOUSES ));
	check_objects_aries->SetValidator( MBooleanFlagValidator( (int*)&wconfig->objects, (int)OI_ARIES ));

	choice_planetorder->SetValidator( MChoiceValidator( &wconfig->planetOrder ));

	check_main_declinations->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_LATITUDE ));
	check_main_90deg->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_90_DEGREE_LONGITUDE ));
	check_main_45deg->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_45_DEGREE_LONGITUDE ));
	check_main_antiscia->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_ANTISCIA ));
	check_main_housepos->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_HOUSE_POSITION ));
	check_main_lord->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_SIGN_LORD ));
	check_main_element->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_ELEMENT ));
	check_main_quality->SetValidator( MBooleanFlagValidator( (int*)&wconfig->columnStyle, (int)TAB_CT_SIGN_QUALITY ));

	Connect( CD_ARABIC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WesternPanel::OnArabicParts ));
}

/*****************************************************
**
**   WesternPanel   ---   Life cycle
**
******************************************************/
WesternPanel::~WesternPanel() { delete wconfig; }
void WesternPanel::config2model() { *wconfig = *config->western; }
void WesternPanel::model2config() { *config->western = *wconfig; }
void WesternPanel::restoreDefaults() { *wconfig = WesternConfig(); }

/*****************************************************
**
**   WesternPanel   ---   OnArabicParts
**
******************************************************/
void WesternPanel::OnArabicParts( wxCommandEvent& )
{
	wxString parts[MAX_ARABIC_OBJECTS];
	for ( uint i = 0; i < MAX_ARABIC_OBJECTS; i++ ) parts[i] = ArabicPartsExpert::getObjectName( FIRST_ARABIC_OBJECT + i, TF_LONG );

	wxMultiChoiceDialog dialog( this,	_( "Choose Arabic Parts" ), GUI_APP_NAME, MAX_ARABIC_OBJECTS, parts );
	dialog.SetSelections( objectArray2wxArrayInt( wconfig->arabicParts ));
	if ( dialog.ShowModal() == wxID_OK )
	{
		wconfig->arabicParts = wxArrayInt2objectArray( dialog.GetSelections() );
		setDirty();
		if ( wconfig->arabicParts.size() > 0 )
		{
			wconfig->objects |= OI_ARABICPARTS;
		}
		else
		{
			wconfig->objects &= ~OI_ARABICPARTS;
		}
	}
	updateUi();
}

/*****************************************************
**
**   WesternPanel   ---   updateUi
**
******************************************************/
void WesternPanel::updateUi()
{
	wxString s;
	if ( wconfig->arabicParts.size() == 0 )
	{
		label_arabic_selection->SetLabel( _( "No part selected" ));
	}
	else if ( wconfig->arabicParts.size() == 1 )
	{
		label_arabic_selection->SetLabel( ArabicPartsExpert::getObjectName( wconfig->arabicParts[0] + FIRST_ARABIC_OBJECT, TF_LONG ));
	}
	else
	{
		s.Printf( _( "%d parts selected" ), wconfig->arabicParts.size());
		label_arabic_selection->SetLabel( s );
	}
}

/*****************************************************
**
**   WesternPanel   ---   set_properties
**
******************************************************/
void WesternPanel::set_properties()
{
    // begin wxGlade: WesternPanel::set_properties
    check_objects_outer->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_dragonhead->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_dragontail->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_ascendant->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_meridian->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_descendant->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_imumcoeli->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_uranian->SetToolTip(_("Fictitious planets of Uranian astrology: Cupido, Hades, Zeus, Kronos, Apollon, Admetos, Vulkanus and Poseidon "));
    check_objects_chiron->SetToolTip(_("Asteroid between Saturn and Uranus. Period 50 years"));
    check_objects_pholus->SetToolTip(_("Asteroid crossing the orbits of outer planets. Period 92 years"));
    check_objects_planetoids->SetToolTip(_("Ceres, Pallas, Juno and Vesta"));
    check_objects_house_cusp->SetToolTip(_("House positions of the objects"));
    check_objects_4house_cusps->SetToolTip(_("House positions of the objects"));
    check_objects_aries->SetToolTip(_("Used in Uranian Astrology"));
    choice_planetorder->SetSelection(0);
    check_main_declinations->SetMinSize(wxSize(180, -1));
    check_main_90deg->SetMinSize(wxSize(180, -1));
    check_main_45deg->SetMinSize(wxSize(180, -1));
    check_main_antiscia->SetMinSize(wxSize(180, -1));
    check_main_lord->SetToolTip(_("Owner of the Rasi"));
    // end wxGlade
}

/*****************************************************
**
**   WesternPanel   ---   do_layout
**
******************************************************/
void WesternPanel::do_layout()
{
    // begin wxGlade: WesternPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 3, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(3, 1, 3, 3);
    sizer_columns_staticbox->Lower();
    wxStaticBoxSizer* sizer_columns = new wxStaticBoxSizer(sizer_columns_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_columns = new wxFlexGridSizer(8, 1, 3, 3);
    wxFlexGridSizer* sizer_center = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_planetorder_staticbox->Lower();
    wxStaticBoxSizer* sizer_planetorder = new wxStaticBoxSizer(sizer_planetorder_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_planetorder2 = new wxFlexGridSizer(1, 2, 3, 3);
    sizer_arabic_staticbox->Lower();
    wxStaticBoxSizer* sizer_arabic = new wxStaticBoxSizer(sizer_arabic_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_arabic_selection = new wxBoxSizer(wxHORIZONTAL);
    sizer_partly_staticbox->Lower();
    wxStaticBoxSizer* sizer_partly = new wxStaticBoxSizer(sizer_partly_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_eobjects_staticbox->Lower();
    wxStaticBoxSizer* sizer_eobjects = new wxStaticBoxSizer(sizer_eobjects_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_eobjects = new wxFlexGridSizer(14, 1, 3, 3);
    grid_eobjects->Add(check_objects_outer, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_dragonhead, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_dragontail, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_ascendant, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_meridian, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_descendant, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_imumcoeli, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_uranian, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_chiron, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_pholus, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_planetoids, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_lilith, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_house_cusp, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_4house_cusps, 0, wxALL|wxEXPAND, 3);
    sizer_eobjects->Add(grid_eobjects, 1, wxALL, 3);
    sizer_left->Add(sizer_eobjects, 1, wxALL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    sizer_partly->Add(check_objects_aries, 0, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_partly, 1, wxALL|wxEXPAND, 3);
    sizer_arabic->Add(button_arabic, 0, wxALL, 3);
    sizer_arabic_selection->Add(label_arabic_selection, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_arabic_selection->SetSizer(sizer_arabic_selection);
    sizer_arabic->Add(panel_arabic_selection, 1, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_arabic, 1, wxEXPAND, 0);
    grid_planetorder2->Add(label_planetorder, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_planetorder2->Add(choice_planetorder, 0, wxALL|wxEXPAND, 3);
    sizer_planetorder->Add(grid_planetorder2, 1, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_planetorder, 1, wxALL|wxEXPAND, 3);
    sizer_center->AddGrowableCol(0);
    sizer_main->Add(sizer_center, 1, wxALL, 3);
    grid_columns->Add(check_main_declinations, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_columns->Add(check_main_90deg, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_columns->Add(check_main_45deg, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_columns->Add(check_main_antiscia, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_columns->Add(check_main_housepos, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_columns->Add(check_main_lord, 0, wxALL|wxEXPAND, 3);
    grid_columns->Add(check_main_element, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_columns->Add(check_main_quality, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_columns->Add(grid_columns, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_columns, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createWesternPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createWesternPanel( wxWindow *parent )
{
	return new WesternPanel( parent );
}

