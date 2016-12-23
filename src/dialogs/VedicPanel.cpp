/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicPanel.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "VedicPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

#include "ArabicParts.h"
#include "Conf.h"
#include "mvalidator.h"

extern Config *config;

enum { CD_ARABIC = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( VedicPanel, ConfigPanel )

/*****************************************************
**
**   VedicPanel   ---   Constructor
**
******************************************************/
VedicPanel::VedicPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	vconfig = new VedicConfig;
	config2model();

    // begin wxGlade: VedicPanel::VedicPanel
    panel_arabic_selection = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_sobjects_staticbox = new wxStaticBox(this, wxID_ANY, _("Special Points"));
    sizer_partly_staticbox = new wxStaticBox(this, wxID_ANY, _("Partly Displayed"));
    sizer_arabic_staticbox = new wxStaticBox(this, wxID_ANY, _("Arabic Parts"));
    sizer_order_lagna_staticbox = new wxStaticBox(this, wxID_ANY, _("Order of Lagna"));
    sizer_maincols_staticbox = new wxStaticBox(this, wxID_ANY, _("Columns in Main Window"));
    sizer_eobjects_staticbox = new wxStaticBox(this, wxID_ANY, _("Ephemeris Objects"));
    check_objects_outer = new wxCheckBox(this, wxID_ANY, _("3 Outer Planets"));
    check_objects_dragonhead = new wxCheckBox(this, wxID_ANY, _("Rahu"));
    check_objects_dragontail = new wxCheckBox(this, wxID_ANY, _("Ketu"));
    check_objects_ascendant = new wxCheckBox(this, wxID_ANY, _("Ascendant"));
    check_objects_meridian = new wxCheckBox(this, wxID_ANY, _("Meridian"));
    check_objects_descendant = new wxCheckBox(this, wxID_ANY, _("Descendant"));
    check_objects_imumcoeli = new wxCheckBox(this, wxID_ANY, _("Imum Coeli"));
    check_objects_uranian_inner = new wxCheckBox(this, wxID_ANY, _("4 Uranian (Cupido-Kronos)"));
    check_objects_uranian_outer = new wxCheckBox(this, wxID_ANY, _("4 Uranian (Apollon-Poseidon)"));
    check_objects_chiron = new wxCheckBox(this, wxID_ANY, _("Chiron"));
    check_objects_pholus = new wxCheckBox(this, wxID_ANY, _("Pholus"));
    check_objects_planetoids = new wxCheckBox(this, wxID_ANY, _("4 Planetoids"));
    check_objects_lilith = new wxCheckBox(this, wxID_ANY, _("Lilith (Black Moon)"));
    check_objects_house_cusp = new wxCheckBox(this, wxID_ANY, _("House Cusps/Sandhis"));
    check_objects_upagrahas = new wxCheckBox(this, wxID_ANY, _("5 Upagrahas"));
    check_objects_kalavelas = new wxCheckBox(this, wxID_ANY, _("Kalavelas"));
    check_objects_lagnas = new wxCheckBox(this, wxID_ANY, _("3 Special Lagnas"));
    check_objects_d9lagna = new wxCheckBox(this, wxID_ANY, _("Sphuta Navamsa Lagna"));
    button_arabic = new wxButton(this, CD_ARABIC, _("Select Parts ..."));
    label_arabic_selection = new wxStaticText(panel_arabic_selection, wxID_ANY, _("No parts selected"));
    const wxString choice_order_lagna_choices[] = {
        _("After Planets"),
        _("Top")
    };
    choice_order_lagna = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_order_lagna_choices, 0);
    check_main_declination = new wxCheckBox(this, wxID_ANY, _("Declination"));
    check_main_lord = new wxCheckBox(this, wxID_ANY, _("Sign Lord"));
    check_main_element = new wxCheckBox(this, wxID_ANY, _("Element"));
    check_main_quality = new wxCheckBox(this, wxID_ANY, _("Quality (Movable etc.)"));
    check_main_signification = new wxCheckBox(this, wxID_ANY, _("Basic Signification"));
    check_main_dignity = new wxCheckBox(this, wxID_ANY, _("Dignity"));
    check_main_d9 = new wxCheckBox(this, wxID_ANY, _("Navamsa D9"));
    check_main_nakshatra = new wxCheckBox(this, wxID_ANY, _("Nakshatra"));
    check_main_pada = new wxCheckBox(this, wxID_ANY, _("Nakshatra Pada"));
    check_main_kp = new wxCheckBox(this, wxID_ANY, _("Krishnamurti Lords"));
    check_main_d60 = new wxCheckBox(this, wxID_ANY, _("Shastiamsa D60"));
    check_main_d10 = new wxCheckBox(this, wxID_ANY, _("Dasa Varga"));
    check_main_housepos = new wxCheckBox(this, wxID_ANY, _("Bhava Position"));
    check_main_karaka = new wxCheckBox(this, wxID_ANY, _("Jamini Karaka"));
    check_main_av = new wxCheckBox(this, wxID_ANY, _("Ashtakavarga Points"));

    set_properties();
    do_layout();
    // end wxGlade

	check_objects_outer->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_OUTER ));
	check_objects_dragonhead->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_DRAGONHEAD ));
	check_objects_dragontail->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_DRAGONTAIL ));
	check_objects_ascendant->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_ASCENDANT ));
	check_objects_meridian->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_MERIDIAN ));
	check_objects_descendant->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_DESCENDANT ));
	check_objects_imumcoeli->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_IMUMCOELI ));
	check_objects_uranian_inner->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_URANIAN_INNER ));
	check_objects_uranian_outer->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_URANIAN_OUTER ));
	check_objects_chiron->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_CHIRON ));
	check_objects_pholus->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_PHOLUS ));
	check_objects_planetoids->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_PLANETOIDS ));
	check_objects_lilith->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_LILITH ));
	check_objects_upagrahas->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_UPAGRAHAS ));
	check_objects_kalavelas->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_KALAVELAS ));
	check_objects_lagnas->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_SPECIALLAGNAS ));
	check_objects_d9lagna->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_D9_LAGNA ));
	check_objects_house_cusp->SetValidator( MBooleanFlagValidator( (int*)&vconfig->objects, (int)OI_ALL_HOUSES ));

	choice_order_lagna->SetValidator( MChoiceValidator( &vconfig->orderLagna ));

	check_main_declination->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_LATITUDE ));
	check_main_lord->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_SIGN_LORD ));
	check_main_element->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_ELEMENT ));
	check_main_quality->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_SIGN_QUALITY ));
	check_main_dignity->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_DIGNITY ));
	check_main_d9->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_NAVAMSA ));
	check_main_nakshatra->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_NAKSHATRA ));
	check_main_pada->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_NAKSHATRA_PADA ));
	check_main_kp->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_KP_LORDS ));
	check_main_d60->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_SHASTIAMSA_LORD ));
	check_main_d10->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_DASA_VARGA ));
	check_main_housepos->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_HOUSE_POSITION ));
	check_main_karaka->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_CHARA_KARAKA ));
	check_main_av->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_AV_REKHAPOINTS ));
	check_main_signification->SetValidator( MBooleanFlagValidator( (int*)&vconfig->columnStyle, (int)TAB_CT_SIGNIFICATION ));

	Connect( CD_ARABIC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VedicPanel::OnArabicParts ));
}

/*****************************************************
**
**   VedicPanel   ---   life cycle
**
******************************************************/
VedicPanel::~VedicPanel() { delete vconfig; } 
void VedicPanel::config2model() { *vconfig = *config->vedic; }
void VedicPanel::model2config() { *config->vedic = *vconfig; }
void VedicPanel::restoreDefaults() { *vconfig = VedicConfig(); } 

/*****************************************************
**
**   VedicPanel   ---   OnArabicParts
**
******************************************************/
void VedicPanel::OnArabicParts( wxCommandEvent& )
{
	wxString parts[MAX_ARABIC_OBJECTS];
	for ( uint i = 0; i < MAX_ARABIC_OBJECTS; i++ ) parts[i] = ArabicPartsExpert::getObjectName( FIRST_ARABIC_OBJECT + i, TF_LONG );

	wxMultiChoiceDialog dialog( this,	_( "Choose Arabic Parts" ), GUI_APP_NAME, MAX_ARABIC_OBJECTS, parts );
	dialog.SetSelections( objectArray2wxArrayInt( vconfig->arabicParts ));
	if ( dialog.ShowModal() == wxID_OK )
	{
		vconfig->arabicParts = wxArrayInt2objectArray( dialog.GetSelections());
		if ( vconfig->arabicParts.size() > 0 )
		{
			vconfig->objects |= OI_ARABICPARTS;
		}
		else
		{
			vconfig->objects &= ~OI_ARABICPARTS;
		}
		setDirty();
	}
	updateUi();
}

/*****************************************************
**
**   VedicPanel   ---   updateUi
**
******************************************************/
void VedicPanel::updateUi()
{
	wxString s;
	if ( vconfig->arabicParts.size() == 0 )
	{
		label_arabic_selection->SetLabel( _( "No part selected" ));
	}
	else if ( vconfig->arabicParts.size() == 1 )
	{
		label_arabic_selection->SetLabel( ArabicPartsExpert::getObjectName( vconfig->arabicParts[0] + FIRST_ARABIC_OBJECT, TF_LONG ));
	}
	else
	{
		label_arabic_selection->SetLabel( wxString::Format( _( "%d parts selected" ), vconfig->arabicParts.size()));
	}
}

/*****************************************************
**
**   VedicPanel   ---   set_properties
**
******************************************************/
void VedicPanel::set_properties()
{
    // begin wxGlade: VedicPanel::set_properties
    check_objects_outer->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_dragonhead->SetToolTip(_("Dragon Head"));
    check_objects_dragontail->SetToolTip(_("Dragon Tail"));
    check_objects_uranian_inner->SetToolTip(_("Fictitious planets of Uranian astrology: Cupido, Hades, Zeus, Kronos, Apollon, Admetos, Vulkanus and Poseidon "));
    check_objects_uranian_outer->SetToolTip(_("Fictitious planets of Uranian astrology: Cupido, Hades, Zeus, Kronos, Apollon, Admetos, Vulkanus and Poseidon "));
    check_objects_chiron->SetToolTip(_("Asteroid between Saturn and Uranus. Period 50 years"));
    check_objects_pholus->SetToolTip(_("Asteroid crossing the orbits of outer planets. Period 92 years"));
    check_objects_planetoids->SetToolTip(_("Ceres, Pallas, Juno and Vesta"));
    check_objects_house_cusp->SetToolTip(_("House positions of the objects"));
    check_objects_upagrahas->SetToolTip(_("Dhuma, Vyatipata, Parivesha, Chapa and Upaketu"));
    check_objects_kalavelas->SetToolTip(_("Kala, Mrityu, Ardhaprahara, Yamaghantaka, Gulika and Mandi"));
    check_objects_lagnas->SetToolTip(_("Bhava Lagna, Hora Lagna and Ghatika Lagna"));
    check_objects_d9lagna->SetToolTip(_("Length of Navamsa Lagna"));
    choice_order_lagna->SetSelection(0);
    check_main_declination->SetToolTip(_("House positions of the objects"));
    check_main_lord->SetToolTip(_("Owner of the Rasi"));
    check_main_dignity->SetToolTip(_("Exaltation, Moolatrikona, friendship etc."));
    check_main_d9->SetToolTip(_("D-9 chart for marriage"));
    check_main_nakshatra->SetToolTip(_("Lunar mansions or constellations. See next tab for 27 resp. 28 Nakshatra zodiac"));
    check_main_pada->SetToolTip(_("Quarters of Nakshatras. 1 Pada = 1 Navamsa"));
    check_main_kp->SetToolTip(_("Lords and Sublords according to Krishnamurti Paddhati system"));
    check_main_d60->SetToolTip(_("Display of Shastiamsa Lords. Can be used for general indications"));
    check_main_d10->SetToolTip(_("Names of dignities according to Dasa Varga: Parijata, Uttama, Gopura etc."));
    check_main_housepos->SetToolTip(_("Bhava according to house cusps (K.P.) or Bhava Sandhis"));
    check_main_karaka->SetToolTip(_("Atma Karaka, Amatya Karaka etc. See next tab for inclusion of Rahu"));
    check_main_av->SetToolTip(_("Total number of Rekha points"));
    // end wxGlade
}

/*****************************************************
**
**   VedicPanel   ---   do_layout
**
******************************************************/
void VedicPanel::do_layout()
{
    // begin wxGlade: VedicPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 4, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_maincols_staticbox->Lower();
    wxStaticBoxSizer* sizer_maincols = new wxStaticBoxSizer(sizer_maincols_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_maincols = new wxFlexGridSizer(15, 1, 3, 0);
    wxFlexGridSizer* sizer_center = new wxFlexGridSizer(4, 1, 0, 0);
    sizer_order_lagna_staticbox->Lower();
    wxStaticBoxSizer* sizer_order_lagna = new wxStaticBoxSizer(sizer_order_lagna_staticbox, wxVERTICAL);
    sizer_arabic_staticbox->Lower();
    wxStaticBoxSizer* sizer_arabic = new wxStaticBoxSizer(sizer_arabic_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_arabic_selection = new wxBoxSizer(wxHORIZONTAL);
    sizer_partly_staticbox->Lower();
    wxStaticBoxSizer* sizer_partly = new wxStaticBoxSizer(sizer_partly_staticbox, wxVERTICAL);
    sizer_sobjects_staticbox->Lower();
    wxStaticBoxSizer* sizer_sobjects = new wxStaticBoxSizer(sizer_sobjects_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sobjects = new wxFlexGridSizer(8, 1, 3, 3);
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
    grid_eobjects->Add(check_objects_uranian_inner, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_uranian_outer, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_chiron, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_pholus, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_planetoids, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_lilith, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_house_cusp, 0, wxALL|wxEXPAND, 3);
    sizer_eobjects->Add(grid_eobjects, 1, wxALL, 3);
    sizer_left->Add(sizer_eobjects, 1, wxALL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_sobjects->Add(check_objects_upagrahas, 0, wxALL|wxEXPAND, 3);
    grid_sobjects->Add(check_objects_kalavelas, 0, wxALL|wxEXPAND, 3);
    grid_sobjects->Add(check_objects_lagnas, 0, wxALL|wxEXPAND, 3);
    sizer_sobjects->Add(grid_sobjects, 1, wxALL, 3);
    sizer_center->Add(sizer_sobjects, 1, wxALL, 3);
    sizer_partly->Add(check_objects_d9lagna, 0, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_partly, 1, wxALL|wxEXPAND, 3);
    sizer_arabic->Add(button_arabic, 0, wxALL, 3);
    sizer_arabic_selection->Add(label_arabic_selection, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_arabic_selection->SetSizer(sizer_arabic_selection);
    sizer_arabic->Add(panel_arabic_selection, 1, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_arabic, 1, wxEXPAND, 0);
    sizer_order_lagna->Add(choice_order_lagna, 0, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_order_lagna, 1, wxEXPAND, 0);
    sizer_center->AddGrowableCol(0);
    sizer_main->Add(sizer_center, 1, wxALL, 3);
    grid_maincols->Add(check_main_declination, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_lord, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_element, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_maincols->Add(check_main_quality, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_maincols->Add(check_main_signification, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_maincols->Add(check_main_dignity, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_d9, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_nakshatra, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_pada, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_kp, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_d60, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_d10, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_housepos, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_karaka, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_av, 0, wxALL|wxEXPAND, 3);
    grid_maincols->AddGrowableCol(0);
    sizer_maincols->Add(grid_maincols, 1, wxALL, 3);
    sizer_right->Add(sizer_maincols, 1, wxALL, 3);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableRow(1);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createVedicPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createVedicPanel( wxWindow *parent )
{
	return new VedicPanel( parent );
}

