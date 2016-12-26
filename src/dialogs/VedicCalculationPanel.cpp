/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicCalculationPanel.cpp
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

#include "VedicCalculationPanel.h"
#include "DialogElements.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Conf.h"
#include "mvalidator.h"
#include "VargaBase.h"

extern Config *config;
enum { CD_YL_CHOICE = wxID_HIGHEST + 1, CD_KALAVELA_CHOICE, CD_KALAVELA_LORD_CHOICE, CD_KALAVELA_PORTION_CHOICE, 
	CD_VARGA_SIGNIFICATION_TEXT, CD_VARGA_CHOICE };

IMPLEMENT_CLASS( VedicCalculationPanel, ConfigPanel )

/*****************************************************
**
**   VedicCalculationPanel   ---   Constructor
**
******************************************************/
VedicCalculationPanel::VedicCalculationPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	cconfig = new VedicCalculationConfig;
	config2model();

    // begin wxGlade: VedicCalculationPanel::VedicCalculationPanel
    notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    notebook_pane6 = new wxPanel(notebook, wxID_ANY);
    notebook_pane5 = new wxPanel(notebook, wxID_ANY);
    notebook_pane4 = new wxPanel(notebook, wxID_ANY);
    notebook_pane3 = new wxPanel(notebook, wxID_ANY);
    notebook_pane2 = new wxPanel(notebook, wxID_ANY);
    notebook_pane1 = new wxPanel(notebook, wxID_ANY);
    sizer_benefics_staticbox = new wxStaticBox(notebook_pane1, wxID_ANY, _("Benefics"));
    sizer_varga_staticbox = new wxStaticBox(notebook_pane2, wxID_ANY, _("Vargas"));
    sizer_varga_significations_staticbox = new wxStaticBox(notebook_pane2, wxID_ANY, _("Varga Significations"));
    sizer_nakshatra_staticbox = new wxStaticBox(notebook_pane3, wxID_ANY, _("Nakshatras and Dasas"));
    sizer_yl_staticbox = new wxStaticBox(notebook_pane3, wxID_ANY, _("Year Length"));
    sizer_kalavelas_staticbox = new wxStaticBox(notebook_pane4, wxID_ANY, _("Kala Velas"));
    sizer_jaimini_staticbox = new wxStaticBox(notebook_pane6, wxID_ANY, _("Jaimini Astrology"));
    sizer_basic_staticbox = new wxStaticBox(notebook_pane1, wxID_ANY, _("Basic Calculation"));
    label_ayanamsa = new wxStaticText(notebook_pane1, wxID_ANY, _("Ayanamsa"));
    const wxString choice_ayanamsa_choices[] = {
        _("None"),
        _("Lahiri"),
        _("Raman"),
        _("Krishnamurti")
    };
    choice_ayanamsa = new wxChoice(notebook_pane1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_ayanamsa_choices, 0);
    label_vcalc_node = new wxStaticText(notebook_pane1, wxID_ANY, _("Lunar Node"));
    const wxString choice_node_choices[] = {
        _("True"),
        _("Mean")
    };
    choice_node = new wxChoice(notebook_pane1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_node_choices, 0);
    label_vcalc_house = new wxStaticText(notebook_pane1, wxID_ANY, _("House System"));
    const wxString choice_house_choices[] = {
        _("Placidus"),
        _("Koch"),
        _("Regiomontanus"),
        _("Campanus"),
        _("Porphyry"),
        _("Equal"),
        _("Vehlow Equal"),
        _("Axial Rotation"),
        _("Azimutal/Horizontal"),
        _("Polich/Page"),
        _("Alcabitus"),
        _("Morinus"),
        _("Krusinski")
    };
    choice_house = new wxChoice(notebook_pane1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 13, choice_house_choices, 0);
    check_use_house_cusp = new wxCheckBox(notebook_pane1, wxID_ANY, _("K.P. Style Houses (Start at Cusp)"));
    label_moon = new wxStaticText(notebook_pane1, wxID_ANY, _("Moon"));
    const wxString choice_moon_choices[] = {
        _("Never"),
        _("Angle < 180"),
        _("120 < Angle < 240"),
        _("Always")
    };
    choice_moon = new wxChoice(notebook_pane1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_moon_choices, 0);
    label_mercury = new wxStaticText(notebook_pane1, wxID_ANY, _("Mercury"));
    const wxString choice_mercury_choices[] = {
        _("Never"),
        _("No Malefic Conjunction"),
        _("Always")
    };
    choice_mercury = new wxChoice(notebook_pane1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_mercury_choices, 0);
    label_tempfriend = new wxStaticText(notebook_pane2, wxID_ANY, _("Temporary Friends"));
    const wxString choice_tempfriend_choices[] = {
        _("Always Rasi"),
        _("Varga")
    };
    choice_tempfriend = new wxChoice(notebook_pane2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_tempfriend_choices, 0);
    label_hora = new wxStaticText(notebook_pane2, wxID_ANY, _("Hora"));
    const wxString choice_hora_choices[] = {
        _("Parasara"),
        _("Continuous")
    };
    choice_hora = new wxChoice(notebook_pane2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_hora_choices, 0);
    label_drekkana = new wxStaticText(notebook_pane2, wxID_ANY, _("Drekkana"));
    const wxString choice_drekkana_choices[] = {
        _("Parasara"),
        _("Continuous")
    };
    choice_drekkana = new wxChoice(notebook_pane2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_drekkana_choices, 0);
    label_chaturtamsa = new wxStaticText(notebook_pane2, wxID_ANY, _("Chaturtamsa"));
    const wxString choice_chaturtamsa_choices[] = {
        _("Parasara"),
        _("Continuous")
    };
    choice_chaturtamsa = new wxChoice(notebook_pane2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_chaturtamsa_choices, 0);
    const wxString choice_vargas_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_vargas = new wxChoice(notebook_pane2, CD_VARGA_CHOICE, wxDefaultPosition, wxDefaultSize, 3, choice_vargas_choices, 0);
    text_varga_signification = new wxTextCtrl(notebook_pane2, CD_VARGA_SIGNIFICATION_TEXT, wxEmptyString);
    label_nakdasa = new wxStaticText(notebook_pane3, wxID_ANY, _("Nakshatra Dasa Calculation"));
    const wxString choice_nakdasa_choices[] = {
        _("Longitude"),
        _("Entry Time")
    };
    choice_nakdasa = new wxChoice(notebook_pane3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_nakdasa_choices, 0);
    label_kalachakra = new wxStaticText(notebook_pane3, wxID_ANY, _("Kalachakra Antardasa Mode"));
    const wxString choice_kala_choices[] = {
        _("Progressive"),
        _("Cyclic"),
        _("Progr same Naks"),
        _("Portion Zero")
    };
    choice_kala = new wxChoice(notebook_pane3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_kala_choices, 0);
    label_yogini_lords = new wxStaticText(notebook_pane3, wxID_ANY, _("Display Yogini Dasa Lords"));
    const wxString choice_yogini_lords_choices[] = {
        _("Yogini"),
        _("Planet"),
        _("Both")
    };
    choice_yogini_lords = new wxChoice(notebook_pane3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_yogini_lords_choices, 0);
    label_dasa_enddate = new wxStaticText(notebook_pane3, wxID_ANY, _("Display Date for Dasas"));
    const wxString choice_dasa_show_date_mode_choices[] = {
        _("Start Date"),
        _("End Date")
    };
    choice_dasa_show_date_mode = new wxChoice(notebook_pane3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_dasa_show_date_mode_choices, 0);
    choice_yl = new YearLengthChoice(notebook_pane3, CD_YL_CHOICE, true);
    text_custom_yl = new wxTextCtrl(notebook_pane3, wxID_ANY, wxEmptyString);
    label_kalavela = new wxStaticText(notebook_pane4, wxID_ANY, _("Kala Vela"));
    const wxString choice_kalavela_choices[] = {
        _("Kala"),
        _("Mrityu"),
        _("Ardhaprahara"),
        _("Yama Ghantaka"),
        _("Gulika"),
        _("Mandi")
    };
    choice_kalavela = new wxChoice(notebook_pane4, CD_KALAVELA_CHOICE, wxDefaultPosition, wxDefaultSize, 6, choice_kalavela_choices, 0);
    label_kalalord = new wxStaticText(notebook_pane4, wxID_ANY, _("Lord"));
    const wxString choice_kalalord_choices[] = {
        _("None"),
        _("Sun"),
        _("Moon"),
        _("Mars"),
        _("Mercury"),
        _("Jupiter"),
        _("Venus"),
        _("Saturn"),
        _("Eight")
    };
    choice_kalalord = new wxChoice(notebook_pane4, CD_KALAVELA_LORD_CHOICE, wxDefaultPosition, wxDefaultSize, 9, choice_kalalord_choices, 0);
    label_kalaportion = new wxStaticText(notebook_pane4, -1, _("Part of Portion"));
    const wxString choice_kalaportion_choices[] = {
        _("Beginning"),
        _("Middle"),
        _("End")
    };
    choice_kalaportion = new wxChoice(notebook_pane4, CD_KALAVELA_PORTION_CHOICE, wxDefaultPosition, wxDefaultSize, 3, choice_kalaportion_choices, 0);
    label_av_calcrules = new wxStaticText(notebook_pane5, wxID_ANY, _("Calculation Rules"));
    const wxString choice_av_calcrules_choices[] = {
        _("Parasara"),
        _("Vahara Mihira")
    };
    choice_av_calcrules = new wxChoice(notebook_pane5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_av_calcrules_choices, 0);
    label_av_benefics = new wxStaticText(notebook_pane5, wxID_ANY, _("Benefic points"));
    const wxString choice_av_benefics_choices[] = {
        _("Number"),
        _("Line"),
        _("Dot")
    };
    choice_av_benefics = new wxChoice(notebook_pane5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_av_benefics_choices, 0);
    label_av_sarva = new wxStaticText(notebook_pane5, wxID_ANY, _("Sarvashtakavarga"));
    const wxString choice_av_sarva_choices[] = {
        _("Planets only"),
        _("With Lagna")
    };
    choice_av_sarva = new wxChoice(notebook_pane5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_av_sarva_choices, 0);
    label_av_sodhya = new wxStaticText(notebook_pane5, wxID_ANY, _("Sodhya Pinda"));
    const wxString choice_av_sodhya_choices[] = {
        _("Parasara"),
        _("Mantreswar")
    };
    choice_av_sodhya = new wxChoice(notebook_pane5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_av_sodhya_choices, 0);
    label_arudha = new wxStaticText(notebook_pane6, wxID_ANY, _("Arudhas"));
    const wxString choice_arudha_choices[] = {
        _("Parasara"),
        _("Raman")
    };
    choice_arudha = new wxChoice(notebook_pane6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_arudha_choices, 0);
    label_karaka = new wxStaticText(notebook_pane6, wxID_ANY, _("Chara Karakas"));
    const wxString choice_charakaraka_choices[] = {
        _("Parasara"),
        _("Raman")
    };
    choice_charakaraka = new wxChoice(notebook_pane6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_charakaraka_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade

	notebook->SetSelection( config->viewprefs->configDialogVCalculationPanelPage );

	// 1st panel
	choice_ayanamsa->SetValidator( MChoiceValidator( (int*)&cconfig->ayanamsa ));
	choice_house->SetValidator( MChoiceValidator( (int*)&cconfig->houseSystem ));
	check_use_house_cusp->SetValidator( MCheckValidator( &cconfig->houseUseCusps ));
	choice_node->SetValidator( MChoiceValidator( &cconfig->lunarNodeMode ));

	// 1st panel: Benefics
	choice_moon->SetValidator( MChoiceValidator( &cconfig->moonBenefic ));
	choice_mercury->SetValidator( MChoiceValidator( &cconfig->mercuryBenefic ));

	// 2nd panel: Vargas
	choice_tempfriend->SetValidator( MChoiceValidator( &cconfig->tempFriendMode ));
	choice_hora->SetValidator( MChoiceValidator( &cconfig->vargaHoraMode ));
	choice_drekkana->SetValidator( MChoiceValidator( &cconfig->vargaDrekkanaMode ));
	choice_chaturtamsa->SetValidator( MChoiceValidator( &cconfig->vargaChaturtamsaMode ));

	// 3rd panel: Nakshatras and Dasas
	choice_nakdasa->SetValidator( MChoiceValidator( &cconfig->nakshatraPortionMode ));
	choice_kala->SetValidator( MChoiceValidator( &cconfig->kalachakraMode ));
	choice_yogini_lords->SetValidator( MChoiceValidator( &cconfig->yoginiDasaLordDisplayType ));
	choice_dasa_show_date_mode->SetValidator( MChoiceValidator( &cconfig->dasaShowDateMode ));

	choice_yl->SetValidator( MChoiceValidator( &cconfig->yearLength ));
	text_custom_yl->SetValidator( MDoubleValidator( &cconfig->customYearLength, 300, 400 ));

	// Ashataka Varga
	choice_av_calcrules->SetValidator( MChoiceValidator( &cconfig->ashtakavargaMode ));
	choice_av_benefics->SetValidator( MChoiceValidator( &cconfig->ashtakavargaNumberMode ));
	choice_av_sarva->SetValidator( MChoiceValidator( &cconfig->ashtakavargaSarva));
	choice_av_sodhya->SetValidator( MChoiceValidator( &cconfig->ashtakavargaSodhyaPindaMode ));

	// Jaimini
	choice_arudha->SetValidator( MChoiceValidator( &cconfig->arudhaMode ));
	choice_charakaraka->SetValidator( MChoiceValidator( &cconfig->charaKarakaMode ));

	Connect( CD_YL_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnChoice ));
	Connect( CD_VARGA_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnVargaChoice ));
	Connect( CD_VARGA_SIGNIFICATION_TEXT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( VedicCalculationPanel::OnVargaSignificationText ));

	Connect( CD_KALAVELA_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnChoice ));
	Connect( CD_KALAVELA_LORD_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnKalaVelaLordChoice ));
	Connect( CD_KALAVELA_PORTION_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnKalaVelaPortionChoice ));
}

/*****************************************************
**
**   VedicCalculationPanel   ---   Destructor
**
******************************************************/
VedicCalculationPanel::~VedicCalculationPanel()
{
	config->viewprefs->configDialogVCalculationPanelPage = notebook->GetSelection();
	delete cconfig;
}

/*****************************************************
**
**   VedicCalculationPanel   ---   config2model
**
******************************************************/
void VedicCalculationPanel::config2model()
{
	*cconfig = *config->vedicCalculation;
}

/*****************************************************
**
**   VedicCalculationPanel   ---   model2config
**
******************************************************/
void VedicCalculationPanel::model2config()
{
	*config->vedicCalculation = *cconfig;
}

/*****************************************************
**
**   VedicCalculationPanel   ---   updateUi
**
******************************************************/
void VedicCalculationPanel::updateUi()
{
	//printf( "VedicCalculationPanel::updateUi\n" );
	text_custom_yl->Enable( choice_yl->GetSelection() == 4 );
	//updateVargaSignification( choice_vargas->GetSelection());

	const uint vsel = choice_vargas->GetSelection();
	assert( vsel < cconfig->vargaSignifications.size());
	text_varga_signification->ChangeValue( cconfig->vargaSignifications[vsel] );

	// ????
	//choice_kalaportion->Enable( cconfig->kalavela.portion[0] != -1 );

	int selkala = choice_kalavela->GetSelection();
	//printf(  "VedicCalculationPanel::OnKalaVelaChoice selkala %d\n", selkala );
	assert( selkala >= 0 && selkala < 6 );
	choice_kalalord->SetSelection( cconfig->kalavela.lord[selkala] + 1 );
	choice_kalaportion->SetSelection( cconfig->kalavela.portion[selkala] );
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnChoice
**
******************************************************/
void VedicCalculationPanel::OnChoice( wxCommandEvent& )
{
	updateUi();
	setDirty();
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnVargaChoice
**
******************************************************/
void VedicCalculationPanel::OnVargaChoice( wxCommandEvent& )
{
	updateUi();
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnVargaSignificationText
**
******************************************************/
void VedicCalculationPanel::OnVargaSignificationText( wxCommandEvent& )
{
	const int selvarga = choice_vargas->GetSelection();
	if ( selvarga >= 0 && selvarga < (int)cconfig->vargaSignifications.size())
	{
		cconfig->vargaSignifications[selvarga] = text_varga_signification->GetValue();
	}
	else
	{
		printf( "ERROR VedicCalculationPanel::OnVargaSignificationText wrong selection %d\n", selvarga );
	}
	setDirty( true );
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnKalaVelaLordChoice
**
******************************************************/
void VedicCalculationPanel::OnKalaVelaLordChoice( wxCommandEvent& )
{
	int selkala = choice_kalavela->GetSelection();
	//printf(  "VedicCalculationPanel::OnKalaVelaLordChoice selkala %d\n", selkala  );
	assert( selkala >= 0 && selkala < 6 );
	cconfig->kalavela.lord[selkala] = choice_kalalord->GetSelection() - 1;
	setDirty();
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnKalaVelaPortionChoice
**
******************************************************/
void VedicCalculationPanel::OnKalaVelaPortionChoice( wxCommandEvent& )
{
	int selkala = choice_kalavela->GetSelection();
	//printf(  "VedicCalculationPanel::OnKalaVelaPortionChoice selkala %d\n", selkala  );
	assert( selkala >= 0 && selkala < 6 );
	cconfig->kalavela.portion[selkala] = choice_kalaportion->GetSelection();
	setDirty();
}

/*****************************************************
**
**   VedicCalculationPanel   ---   restoreDefaults
**
******************************************************/
void VedicCalculationPanel::restoreDefaults()
{
	*cconfig = VedicCalculationConfig();

	VargaConfigLoader *loader = VargaConfigLoader::get();

	cconfig->vargaSignifications.clear();
	for( Varga v = V_FIRST; v <= V_LAST; v++ )
	{
		cconfig->vargaSignifications.push_back( loader->getDefaultVargaSignification( v ));
	}
}

/*****************************************************
**
**   VedicCalculationPanel   ---   set_properties
**
******************************************************/
void VedicCalculationPanel::set_properties()
{
	choice_vargas->Clear();
	VargaConfigLoader *loader = VargaConfigLoader::get();

	for( Varga v = V_FIRST; v <= V_LAST; v++ )
	{
		if ( v == V_BHAVA ) choice_vargas->Append( loader->getVargaName( v ));
		choice_vargas->Append( wxString::Format( wxT( "D-%d %s" ), loader->getVargaDivision( v ), loader->getVargaName( v ).c_str()));
	}
	choice_vargas->SetSelection( 0 );

    // begin wxGlade: VedicCalculationPanel::set_properties
    choice_ayanamsa->SetSelection(0);
    choice_node->SetToolTip(_("Mean Node is most frequently used in Vedic Astrology"));
    choice_node->SetSelection(0);
    choice_house->SetSelection(0);
    check_use_house_cusp->SetToolTip(_("Checked: houses start a cusp; Unchecked: cusp is middle of house"));
    choice_moon->SetToolTip(_("Angle is Moon - Sun"));
    choice_moon->SetSelection(0);
    choice_mercury->SetSelection(0);
    choice_tempfriend->SetSelection(0);
    choice_hora->SetSelection(0);
    choice_drekkana->SetSelection(0);
    choice_chaturtamsa->SetSelection(0);
    choice_vargas->SetSelection(0);
    choice_nakdasa->SetSelection(0);
    choice_kala->SetSelection(0);
    choice_yogini_lords->SetSelection(0);
    choice_dasa_show_date_mode->SetSelection(0);
    choice_kalavela->SetSelection(0);
    choice_kalalord->SetSelection(0);
    choice_kalaportion->SetSelection(0);
    choice_av_calcrules->SetSelection(0);
    choice_av_benefics->SetSelection(0);
    choice_av_sarva->SetSelection(0);
    choice_av_sodhya->SetSelection(0);
    choice_arudha->SetToolTip(_("Parasara: special rules for planets in angles, Raman: no special rules"));
    choice_arudha->SetSelection(0);
    choice_charakaraka->SetToolTip(_("Parasara: 8 karakas, Raman: 7 karakas"));
    choice_charakaraka->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   VedicCalculationPanel   ---   do_layout
**
******************************************************/
void VedicCalculationPanel::do_layout()
{
    // begin wxGlade: VedicCalculationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 3, 3);
    wxBoxSizer* sizer_pane6 = new wxBoxSizer(wxVERTICAL);
    sizer_jaimini_staticbox->Lower();
    wxStaticBoxSizer* sizer_jaimini = new wxStaticBoxSizer(sizer_jaimini_staticbox, wxHORIZONTAL);
    wxGridSizer* grid_jaimini = new wxGridSizer(2, 2, 0, 0);
    wxBoxSizer* sizer_pane5 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_av = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid_av = new wxFlexGridSizer(4, 2, 3, 3);
    wxBoxSizer* sizer_pane4 = new wxBoxSizer(wxVERTICAL);
    sizer_kalavelas_staticbox->Lower();
    wxStaticBoxSizer* sizer_kalavelas = new wxStaticBoxSizer(sizer_kalavelas_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_kalavelas = new wxFlexGridSizer(3, 2, 3, 3);
    wxFlexGridSizer* sizer_pane3 = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_yl_staticbox->Lower();
    wxStaticBoxSizer* sizer_yl = new wxStaticBoxSizer(sizer_yl_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_yl = new wxFlexGridSizer(2, 2, 3, 3);
    sizer_nakshatra_staticbox->Lower();
    wxStaticBoxSizer* sizer_nakshatra = new wxStaticBoxSizer(sizer_nakshatra_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_nakshatra = new wxFlexGridSizer(4, 2, 3, 3);
    wxFlexGridSizer* sizer_pane2 = new wxFlexGridSizer(1, 2, 3, 3);
    sizer_varga_significations_staticbox->Lower();
    wxStaticBoxSizer* sizer_varga_significations = new wxStaticBoxSizer(sizer_varga_significations_staticbox, wxVERTICAL);
    sizer_varga_staticbox->Lower();
    wxStaticBoxSizer* sizer_varga = new wxStaticBoxSizer(sizer_varga_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_varga = new wxFlexGridSizer(4, 2, 3, 3);
    wxFlexGridSizer* sizer_pane1 = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_benefics_staticbox->Lower();
    wxStaticBoxSizer* sizer_benefics = new wxStaticBoxSizer(sizer_benefics_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_other = new wxFlexGridSizer(3, 2, 3, 3);
    sizer_basic_staticbox->Lower();
    wxStaticBoxSizer* sizer_basic = new wxStaticBoxSizer(sizer_basic_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_basic = new wxFlexGridSizer(3, 2, 3, 3);
    grid_basic->Add(label_ayanamsa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_basic->Add(choice_ayanamsa, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_basic->Add(label_vcalc_node, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_basic->Add(choice_node, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_basic->Add(label_vcalc_house, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_basic->Add(choice_house, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_basic->AddGrowableCol(1);
    sizer_basic->Add(grid_basic, 1, wxALL, 3);
    sizer_basic->Add(check_use_house_cusp, 0, wxALL|wxEXPAND, 3);
    sizer_pane1->Add(sizer_basic, 1, wxALL|wxEXPAND, 3);
    grid_sizer_other->Add(label_moon, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_other->Add(choice_moon, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_sizer_other->Add(label_mercury, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_other->Add(choice_mercury, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_sizer_other->AddGrowableCol(1);
    sizer_benefics->Add(grid_sizer_other, 0, wxEXPAND, 0);
    sizer_pane1->Add(sizer_benefics, 1, wxALL|wxEXPAND, 3);
    notebook_pane1->SetSizer(sizer_pane1);
    grid_varga->Add(label_tempfriend, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_tempfriend, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_varga->Add(label_hora, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_hora, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_varga->Add(label_drekkana, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_drekkana, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_varga->Add(label_chaturtamsa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_chaturtamsa, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_varga->AddGrowableCol(1);
    sizer_varga->Add(grid_varga, 1, wxALL|wxEXPAND, 3);
    sizer_pane2->Add(sizer_varga, 1, wxALL, 3);
    sizer_varga_significations->Add(choice_vargas, 0, wxALL|wxALIGN_RIGHT, 3);
    sizer_varga_significations->Add(text_varga_signification, 0, wxALL|wxEXPAND, 3);
    sizer_pane2->Add(sizer_varga_significations, 1, wxALL, 3);
    notebook_pane2->SetSizer(sizer_pane2);
    grid_nakshatra->Add(label_nakdasa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_nakdasa, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_nakshatra->Add(label_kalachakra, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_kala, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_nakshatra->Add(label_yogini_lords, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_yogini_lords, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(label_dasa_enddate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_dasa_show_date_mode, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    sizer_nakshatra->Add(grid_nakshatra, 1, wxALL, 3);
    sizer_pane3->Add(sizer_nakshatra, 1, wxALL|wxEXPAND, 3);
    wxStaticText* label_yl = new wxStaticText(notebook_pane3, wxID_ANY, _("Length"));
    grid_yl->Add(label_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(choice_yl, 1, wxLEFT|wxALIGN_RIGHT, 0);
    wxStaticText* label_customyl = new wxStaticText(notebook_pane3, wxID_ANY, _("Custom Length"));
    grid_yl->Add(label_customyl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(text_custom_yl, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    grid_yl->AddGrowableCol(1);
    sizer_yl->Add(grid_yl, 1, wxEXPAND, 0);
    sizer_pane3->Add(sizer_yl, 1, wxALL|wxEXPAND, 3);
    notebook_pane3->SetSizer(sizer_pane3);
    grid_kalavelas->Add(label_kalavela, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_kalavelas->Add(choice_kalavela, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_kalavelas->Add(label_kalalord, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_kalavelas->Add(choice_kalalord, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_kalavelas->Add(label_kalaportion, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_kalavelas->Add(choice_kalaportion, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_kalavelas->AddGrowableCol(1);
    sizer_kalavelas->Add(grid_kalavelas, 1, wxALL, 3);
    sizer_pane4->Add(sizer_kalavelas, 0, wxALL, 3);
    notebook_pane4->SetSizer(sizer_pane4);
    grid_av->Add(label_av_calcrules, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_calcrules, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(label_av_benefics, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_benefics, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(label_av_sarva, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_sarva, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(label_av_sodhya, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_sodhya, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->AddGrowableCol(1);
    sizer_av->Add(grid_av, 1, wxALL|wxEXPAND, 3);
    sizer_pane5->Add(sizer_av, 0, wxALL, 3);
    notebook_pane5->SetSizer(sizer_pane5);
    grid_jaimini->Add(label_arudha, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_jaimini->Add(choice_arudha, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_jaimini->Add(label_karaka, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_jaimini->Add(choice_charakaraka, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    sizer_jaimini->Add(grid_jaimini, 1, wxEXPAND, 0);
    sizer_pane6->Add(sizer_jaimini, 0, wxALL, 3);
    notebook_pane6->SetSizer(sizer_pane6);
    notebook->AddPage(notebook_pane1, _("General"));
    notebook->AddPage(notebook_pane2, _("Vargas"));
    notebook->AddPage(notebook_pane3, _("Nakshatras and Dasas"));
    notebook->AddPage(notebook_pane4, _("Kala Velas"));
    notebook->AddPage(notebook_pane5, _("Ashtakavarga"));
    notebook->AddPage(notebook_pane6, _("Jaimini"));
    sizer_main->Add(notebook, 1, wxEXPAND, 0);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createVedicCalculationPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createVedicCalculationPanel( wxWindow *parent )
{
	return new VedicCalculationPanel( parent );
}

