/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/VedicChartPanel.cpp
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

#include "VedicChartPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "ChartProperties.h"
#include "Conf.h"
#include "guibase.h"
#include "GraphicGrid.h"
#include "Horoscope.h"
#include "mvalidator.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicChartConfig.h"

enum { CD_NOTEBOOK = wxID_HIGHEST + 1, CD_SKIN_SELECT, CD_USE_COLORS, CD_SELECT_SNE, CD_SHOW_ARUDHAS, CD_SHOW_RETRO,
	CD_SHOW_CENTER, CD_SELECT_NORTH, CD_SHOW_SANSKRIT_SYMBOLS, CD_SHOW_AFFLICTIONS, CD_SHOW_NAKSHATRA_QUALITIES,
	CD_HIGHLIGHT_FIELD, CD_ZOOM_FIELD, CD_MULTIPLE_ASPECTS, CD_BENEFIC_MARKUP, CD_MINIMUM_STRENGTH,
	CD_WEAKERASPECTS_STYLE, CD_GRAHADRISHTI_ASPECTING, CD_GRAHADRISHTI_ASPECTED, CD_RASIDRISHTI_ASPECTING,
	CD_RASIDRISHTI_ASPECTED, CD_TRANSIT, CD_PREVIEW_WINDOW, CD_SOUTH_ASCENDANT, CD_SHOW_ASHTAKAVARGA,
	CD_ASPECTING, CD_ASPECTED, CD_ALL_ASPECTS, CD_ASPECT_TYPE
};

IMPLEMENT_CLASS( VedicChartPanel, wxPanel )

extern Config* config;

/*****************************************************
**
**   VedicChartPanel   ---   Constructor
**
******************************************************/
VedicChartPanel::VedicChartPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	chartprops = new ChartProperties;
	config2model();

    // begin wxGlade: VedicChartPanel::VedicChartPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    notebook = new wxNotebook(this, CD_NOTEBOOK, wxDefaultPosition, wxDefaultSize, 0);
    notebook_pane_responsive = new wxPanel(notebook, wxID_ANY);
    notebook_pane_sbc = new wxPanel(notebook, wxID_ANY);
    notebook_pane_vargachart = new wxPanel(notebook, wxID_ANY);
    notebook_pane_general = new wxPanel(notebook, wxID_ANY);
    sizer_viewoptions_staticbox = new wxStaticBox(notebook_pane_general, wxID_ANY, _("View Options"));
    sizer_mouseover_staticbox = new wxStaticBox(notebook_pane_general, wxID_ANY, _("Field Mouse Over Behavior"));
    sizer_vargaoptions_staticbox = new wxStaticBox(notebook_pane_vargachart, wxID_ANY, _("View Options"));
    sizer_north_staticbox = new wxStaticBox(notebook_pane_vargachart, wxID_ANY, _("North Indian Chart"));
    sizer_south_staticbox = new wxStaticBox(notebook_pane_vargachart, wxID_ANY, _("South Indian Chart"));
    sizer_sbc_options_staticbox = new wxStaticBox(notebook_pane_sbc, wxID_ANY, _("View Options"));
    sizer_aspect_options_staticbox = new wxStaticBox(notebook_pane_responsive, wxID_ANY, _("Aspect Display"));
    sizer_preview_staticbox = new wxStaticBox(this, wxID_ANY, _("Preview"));
    sizer_defaultskin_staticbox = new wxStaticBox(notebook_pane_general, wxID_ANY, _("Skin"));
    const wxString choice_defaultskin_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_defaultskin = new wxChoice(notebook_pane_general, CD_SKIN_SELECT, wxDefaultPosition, wxDefaultSize, 5, choice_defaultskin_choices, 0);
    check_retro = new wxCheckBox(notebook_pane_general, CD_SHOW_RETRO, _("Show Retrogression (R)"));
    check_use_colors = new wxCheckBox(notebook_pane_general, CD_USE_COLORS, _("Use Colors for Planet Symbols"));
    check_highlight = new wxCheckBox(notebook_pane_general, CD_HIGHLIGHT_FIELD, _("Highlight"));
    check_zoom = new wxCheckBox(notebook_pane_general, CD_ZOOM_FIELD, _("Zoom"));
    spin_zoomfactor = new wxSpinCtrl(notebook_pane_general, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 50, 200);
    label_percent = new wxStaticText(notebook_pane_general, wxID_ANY, _("%"));
    label_chartstyle = new wxStaticText(notebook_pane_vargachart, wxID_ANY, _("Style"));
    const wxString choice_chart_choices[] = {
        _("South Indian"),
        _("North Indian"),
        _("East Indian")
    };
    choice_chart = new wxChoice(notebook_pane_vargachart, CD_SELECT_NORTH, wxDefaultPosition, wxDefaultSize, 3, choice_chart_choices, 0);
    label_chartcenter = new wxStaticText(notebook_pane_vargachart, wxID_ANY, _("Center of Chart"));
    const wxString choice_chart_center_choices[] = {
        _("Nothing"),
        _("Name"),
        _("Name + Signification"),
        _("Division D-N"),
        _("D-N + Signification")
    };
    choice_chart_center = new wxChoice(notebook_pane_vargachart, CD_SHOW_CENTER, wxDefaultPosition, wxDefaultSize, 5, choice_chart_center_choices, 0);
    check_arudhas = new wxCheckBox(notebook_pane_vargachart, CD_SHOW_ARUDHAS, _("Show Arudhas"));
    check_ashtakavarga = new wxCheckBox(notebook_pane_vargachart, CD_SHOW_ASHTAKAVARGA, _("Ashtakavarga in Transit Chart"));
    label_north_style = new wxStaticText(notebook_pane_vargachart, wxID_ANY, _("Sign Display"));
    const wxString choice_north_chart_choices[] = {
        _("Ascendant Only"),
        _("Number"),
        _("Short String"),
        _("Symbol")
    };
    choice_north_chart = new wxChoice(notebook_pane_vargachart, CD_SELECT_SNE, wxDefaultPosition, wxDefaultSize, 4, choice_north_chart_choices, 0);
    label_markup_ascendant = new wxStaticText(notebook_pane_vargachart, wxID_ANY, _("Ascendant Markup"));
    const wxString choice_markup_ascendant_choices[] = {
        _("None"),
        _("Solid Line"),
        _("Dotted Line")
    };
    choice_markup_ascendant = new wxChoice(notebook_pane_vargachart, CD_SOUTH_ASCENDANT, wxDefaultPosition, wxDefaultSize, 3, choice_markup_ascendant_choices, 0);
    check_sanskrit_symbols = new wxCheckBox(notebook_pane_sbc, CD_SHOW_SANSKRIT_SYMBOLS, _("Sanskrit Symbols"));
    check_nakshatra_qualities = new wxCheckBox(notebook_pane_sbc, CD_SHOW_NAKSHATRA_QUALITIES, _("Benefic/Malefic (B/M) in Nakshatras Names"));
    label_aspected = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("Show Aspected Fields"));
    const wxString choice_aspected_choices[] = {
        _("Never"),
        _("Mouse Over"),
        _("Shift + Mouse Over"),
        _("Ctrl + Mouse Over"),
        _("Alt + Mouse Over")
    };
    choice_aspected = new wxChoice(notebook_pane_responsive, CD_ASPECTED, wxDefaultPosition, wxDefaultSize, 5, choice_aspected_choices, 0);
    label_aspecting = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("Show Aspecting Objects"));
    const wxString choice_aspecting_choices[] = {
        _("Never"),
        _("Mouse Over"),
        _("Shift + Mouse Over"),
        _("Ctrl + Mouse Over"),
        _("Alt + Mouse Over")
    };
    choice_aspecting = new wxChoice(notebook_pane_responsive, CD_ASPECTING, wxDefaultPosition, wxDefaultSize, 5, choice_aspecting_choices, 0);
    label_all_aspects = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("Show All Aspects"));
    const wxString choice_all_aspects_choices[] = {
        _("Never"),
        _("Mouse Over"),
        _("Shift + Mouse Over"),
        _("Ctrl + Mouse Over"),
        _("Alt + Mouse Over"),
        _("Always")
    };
    choice_all_aspects = new wxChoice(notebook_pane_responsive, CD_ALL_ASPECTS, wxDefaultPosition, wxDefaultSize, 6, choice_all_aspects_choices, 0);
    label_aspect_type = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("Aspect Type in Varga Charts"));
    const wxString choice_aspect_type_choices[] = {
        _("Graha Drishti"),
        _("Rasi Drishti")
    };
    choice_aspect_type = new wxChoice(notebook_pane_responsive, CD_ASPECT_TYPE, wxDefaultPosition, wxDefaultSize, 2, choice_aspect_type_choices, 0);
    label_minimum_strength = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("Graha Drishti Minimum Strength"));
    const wxString choice_minimum_strength_choices[] = {
        _("100% (only full)"),
        _("75%"),
        _("50%"),
        _("25% (show all)")
    };
    choice_minimum_strength = new wxChoice(notebook_pane_responsive, CD_MINIMUM_STRENGTH, wxDefaultPosition, wxDefaultSize, 4, choice_minimum_strength_choices, 0);
    label_benefic = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("Benefic and Malefic Colors"));
    const wxString choice_benefic_choices[] = {
        _("Specific Colors (b/m)"),
        _("Background Color"),
        _("Grey")
    };
    choice_benefic = new wxChoice(notebook_pane_responsive, CD_BENEFIC_MARKUP, wxDefaultPosition, wxDefaultSize, 3, choice_benefic_choices, 0);
    check_include_asmc = new wxCheckBox(notebook_pane_responsive, wxID_ANY, _("Include Angles"));
    window_preview = new ChartGridWidget(panel_preview, CT_RADIX, chartprops, 1, 1, CD_PREVIEW_WINDOW);
    checktransit = new wxCheckBox(this, CD_TRANSIT, _("Show Transits"));

    set_properties();
    do_layout();
    // end wxGlade

	horoscope = new Horoscope();
	horoscope->update();

	htransit = new Horoscope();
	htransit->setDate( MDate::getCurrentJD() + 36000 );
	htransit->update();

	window_preview->addVedicChart( V_RASI, horoscope );

	choice_defaultskin->Clear();
	choice_defaultskin->Append( VedicChartConfigLoader::get()->getNamesAsArray());
	choice_defaultskin->SetSelection( 0 );

	notebook->SetSelection( config->viewprefs->configDialogVChartPanelPage );

	{
		VedicChartDisplayConfig &style = chartprops->getVedicChartDisplayConfig();
		choice_defaultskin->SetValidator( MChoiceValidator( &style.graphicSkin ));

		// varga chart
		choice_chart->SetValidator( MChoiceValidator( &style.indianChartType ));
		choice_north_chart->SetValidator( MChoiceValidator( &style.northIndianSignDisplayType ));
		choice_markup_ascendant->SetValidator( MChoiceValidator( &style.southIndianAscendantMarkup ));

		check_use_colors->SetValidator( MCheckValidator( &style.showPlanetColors ));
		check_arudhas->SetValidator( MCheckValidator( &style.showArudhas ));
		check_ashtakavarga->SetValidator( MCheckValidator( &style.showAshtakavarga ));
		check_retro->SetValidator( MCheckValidator( &style.showRetro ));

		choice_chart_center->SetValidator( MChoiceValidator( &style.centerInfoType ));

		// SBC
		check_nakshatra_qualities->SetValidator( MCheckValidator( &style.showSbcNakshatraQuality ));
		check_sanskrit_symbols->SetValidator( MCheckValidator( &style.showSbcSanskritSymbols ));
	}

	{
		VedicChartBehaviorConfig &bcfg = chartprops->getVedicChartBehaviorConfig();

		// mouse over
		check_highlight->SetValidator( MCheckValidator( &bcfg.highlightOnMouseOver ));
		check_zoom->SetValidator( MCheckValidator( &bcfg.zoomOnMouseOver ));
		spin_zoomfactor->SetValidator( MSpinValidator( &bcfg.zoomFactor ));

		// Aspects
		choice_aspected->SetValidator( MChoiceValidator( &bcfg.aspectedObjectsMode ));
		choice_aspecting->SetValidator( MChoiceValidator( &bcfg.aspectingObjectsMode ));
		choice_all_aspects->SetValidator( MChoiceValidator( &bcfg.allAspectsMode ));
		choice_aspect_type->SetValidator( MChoiceValidator( &bcfg.aspectType ));
		choice_minimum_strength->SetValidator( MChoiceValidator( (int*)&bcfg.grahaDrishtiMinimumStrength ));
		choice_benefic->SetValidator( MChoiceValidator( &bcfg.beneficMaleficDisplay ));
		check_include_asmc->SetValidator( MCheckValidator( &bcfg.includeAsMc ));
	}

	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicChartPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( VedicChartPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( VedicChartPanel::OnSpin ));
	Connect( CD_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( VedicChartPanel::OnNotebook ));
	Connect( CD_PREVIEW_WINDOW, COMMAND_CHART_SKIN_CHANGED, wxCommandEventHandler( VedicChartPanel::OnChartSkinChanged ));

	checktransit->SetValue( config->viewprefs->configDialogVChartPreviewTransits );
}

/*****************************************************
**
**   VedicChartPanel   ---   Destructor
**
******************************************************/
VedicChartPanel::~VedicChartPanel()
{
	config->viewprefs->configDialogVChartPreviewTransits = checktransit->GetValue();
	config->viewprefs->configDialogVChartPanelPage = notebook->GetSelection();
	delete horoscope;
	delete htransit;
	delete chartprops;
}

/*****************************************************
**
**   VedicChartPanel   ---   config2model
**
******************************************************/
void VedicChartPanel::config2model()
{
	*chartprops = ChartProperties();
	chartprops->setVedic( true );
}

/*****************************************************
**
**   VedicChartPanel   ---   model2config
**
******************************************************/
void VedicChartPanel::model2config()
{
	*config->vedicChart = chartprops->getVedicChartDisplayConfig();
	*config->vedicChartBehavior = chartprops->getVedicChartBehaviorConfig();
}

/*****************************************************
**
**   VedicChartPanel   ---   calculateStyle
**
******************************************************/
void VedicChartPanel::calculateStyle()
{
	VedicChartDisplayConfig style;
	style.showArudhas = check_arudhas->GetValue();
	style.showAshtakavarga = check_ashtakavarga->GetValue();
	style.showRetro = check_retro->GetValue();
	style.showPlanetColors = check_use_colors->GetValue();
	style.showSbcSanskritSymbols = check_sanskrit_symbols->GetValue();
	style.showSbcNakshatraQuality = check_nakshatra_qualities->GetValue();

	style.indianChartType = choice_chart->GetSelection();
	style.northIndianSignDisplayType = choice_north_chart->GetSelection();
	style.southIndianAscendantMarkup = choice_markup_ascendant->GetSelection();
	style.centerInfoType = choice_chart_center->GetSelection();

	style.graphicSkin = choice_defaultskin->GetSelection();
	chartprops->setVedicChartDisplayConfig( style );
}

/*****************************************************
**
**   VedicChartPanel   ---   OnNotebook
**
******************************************************/
void VedicChartPanel::OnNotebook( wxNotebookEvent& /*event*/ )
{
	//printf( "VedicChartPanel::OnNotebook id %d\n", event.GetSelection());
	updateUi();
}

/*****************************************************
**
**   VedicChartPanel   ---   OnSpin
**
******************************************************/
void VedicChartPanel::OnSpin( wxSpinEvent& )
{
	//printf( "VedicChartPanel::OnSpin\n" );
	updateUi();
	setDirty();
}

/*****************************************************
**
**   VedicChartPanel   ---   OnChartSkinChanged
**
******************************************************/
void VedicChartPanel::OnChartSkinChanged( wxCommandEvent& )
{
	int skin = window_preview->getChartProperties()->getSkin();
	//printf( "CHART SKIN CHANGED skin %d\n", skin );
	if ( skin != choice_defaultskin->GetSelection())
	{
    choice_defaultskin->SetSelection( skin );
	}
	setDirty();
}

/*****************************************************
**
**   VedicChartPanel   ---   OnCommand
**
******************************************************/
void VedicChartPanel::OnCommand( wxCommandEvent &event )
{
	//printf( "VedicChartPanel::OnCommand ID %d\n", event.GetId() );
	TransferDataFromWindow();
	if ( event.GetId() != CD_TRANSIT )
	{
		setDirty();
	}
	updateUi();
}

/*****************************************************
**
**   VedicChartPanel   ---   updateUi
**
******************************************************/
void VedicChartPanel::updateUi()
{
	VedicChartBehaviorConfig &bcfg = chartprops->getVedicChartBehaviorConfig();

	choice_markup_ascendant->Enable( choice_chart->GetSelection() == 0 );
	label_markup_ascendant->Enable( choice_chart->GetSelection() == 0 );

	label_north_style->Enable( choice_chart->GetSelection() == 1 );
	choice_north_chart->Enable( choice_chart->GetSelection() == 1 );

	chartprops->setVedicSkin( choice_defaultskin->GetSelection());

	label_aspected->Enable( bcfg.aspectingObjectsMode != 5 );
	choice_aspected->Enable( bcfg.aspectingObjectsMode != 5 );

	label_minimum_strength->Enable( bcfg.aspectType == 0 );
	choice_minimum_strength->Enable( bcfg.aspectType == 0 );

	window_preview->deleteCharts();
	Horoscope *ht = ( checktransit->GetValue() ? htransit : (Horoscope*)NULL );
	window_preview->changeChartType( ht ? CT_TRANSIT : CT_RADIX );

	//printf( "REFRESH CHART\n" );

	if ( notebook->GetSelection() == 2 )
	{
		window_preview->addSarvatobhadraChart( horoscope, ht );
	}
	else
	{
		window_preview->addVedicChart( V_RASI, horoscope, ht );
	}
	window_preview->OnDataChanged();
	window_preview->Refresh();
}

/*****************************************************
**
**   VedicChartPanel   ---   restoreDefaults
**
******************************************************/
void VedicChartPanel::restoreDefaults()
{
	*chartprops = ChartProperties( false );
	chartprops->setVedic( true );
}

/*****************************************************
**
**   VedicChartPanel   ---   set_properties
**
******************************************************/
void VedicChartPanel::set_properties()
{
#if defined(__WXMAC__)
// change Ctrl to Cmd on OSx
    choice_aspected->SetString( 3, _("Cmd + Mouse Over"));
    choice_aspecting->SetString( 3, _("Cmd + Mouse Over"));
#endif

    // begin wxGlade: VedicChartPanel::set_properties
    choice_defaultskin->SetSelection(0);
    choice_chart->SetSelection(0);
    choice_chart_center->SetSelection(0);
    choice_north_chart->SetSelection(0);
    choice_markup_ascendant->SetSelection(0);
    choice_aspected->SetSelection(0);
    choice_aspecting->SetSelection(0);
    choice_all_aspects->SetSelection(0);
    choice_aspect_type->SetSelection(0);
    choice_minimum_strength->SetSelection(0);
    choice_benefic->SetSelection(0);
    panel_preview->SetMinSize(wxSize(100, 100));
    // end wxGlade
}

/*****************************************************
**
**   VedicChartPanel   ---   do_layout
**
******************************************************/
void VedicChartPanel::do_layout()
{
    // begin wxGlade: VedicChartPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_preview_staticbox->Lower();
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_preview = new wxFlexGridSizer(1, 1, 3, 3);
    wxBoxSizer* sizer_left = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* sizer_aspect = new wxFlexGridSizer(1, 1, 3, 3);
    sizer_aspect_options_staticbox->Lower();
    wxStaticBoxSizer* sizer_aspect_options = new wxStaticBoxSizer(sizer_aspect_options_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_aspects = new wxFlexGridSizer(6, 2, 3, 3);
    wxFlexGridSizer* grid_sbc = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_sbc_options_staticbox->Lower();
    wxStaticBoxSizer* sizer_sbc_options = new wxStaticBoxSizer(sizer_sbc_options_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_vargachart = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_south_staticbox->Lower();
    wxStaticBoxSizer* sizer_south = new wxStaticBoxSizer(sizer_south_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_south = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_north_staticbox->Lower();
    wxStaticBoxSizer* sizer_north = new wxStaticBoxSizer(sizer_north_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_north = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_vargaoptions_staticbox->Lower();
    wxStaticBoxSizer* sizer_vargaoptions = new wxStaticBoxSizer(sizer_vargaoptions_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_varga_options = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* sizer_general = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_mouseover_staticbox->Lower();
    wxStaticBoxSizer* sizer_mouseover = new wxStaticBoxSizer(sizer_mouseover_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_zoom = new wxFlexGridSizer(1, 3, 0, 0);
    sizer_viewoptions_staticbox->Lower();
    wxStaticBoxSizer* sizer_viewoptions = new wxStaticBoxSizer(sizer_viewoptions_staticbox, wxVERTICAL);
    sizer_defaultskin_staticbox->Lower();
    wxStaticBoxSizer* sizer_defaultskin = new wxStaticBoxSizer(sizer_defaultskin_staticbox, wxVERTICAL);
    sizer_defaultskin->Add(choice_defaultskin, 0, wxALL, 3);
    sizer_general->Add(sizer_defaultskin, 1, wxALL|wxEXPAND, 3);
    sizer_viewoptions->Add(check_retro, 0, wxALL|wxEXPAND, 3);
    sizer_viewoptions->Add(check_use_colors, 0, wxALL|wxEXPAND, 3);
    sizer_general->Add(sizer_viewoptions, 1, wxEXPAND, 0);
    sizer_mouseover->Add(check_highlight, 0, wxALL|wxEXPAND, 3);
    grid_zoom->Add(check_zoom, 1, wxALL|wxEXPAND, 3);
    grid_zoom->Add(spin_zoomfactor, 0, wxALIGN_RIGHT, 0);
    grid_zoom->Add(label_percent, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_zoom->AddGrowableCol(0);
    sizer_mouseover->Add(grid_zoom, 1, wxEXPAND, 0);
    sizer_general->Add(sizer_mouseover, 1, wxEXPAND, 3);
    notebook_pane_general->SetSizer(sizer_general);
    sizer_general->AddGrowableCol(0);
    grid_varga_options->Add(label_chartstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga_options->Add(choice_chart, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_varga_options->Add(label_chartcenter, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga_options->Add(choice_chart_center, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_varga_options->AddGrowableCol(0);
    sizer_vargaoptions->Add(grid_varga_options, 1, wxALL|wxEXPAND, 3);
    sizer_vargaoptions->Add(check_arudhas, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_vargaoptions->Add(check_ashtakavarga, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_vargachart->Add(sizer_vargaoptions, 1, wxEXPAND, 0);
    grid_north->Add(label_north_style, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_north->Add(choice_north_chart, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_north->AddGrowableCol(0);
    sizer_north->Add(grid_north, 1, wxEXPAND, 0);
    grid_vargachart->Add(sizer_north, 1, wxEXPAND, 0);
    grid_south->Add(label_markup_ascendant, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_south->Add(choice_markup_ascendant, 0, wxALIGN_RIGHT, 0);
    grid_south->AddGrowableCol(0);
    sizer_south->Add(grid_south, 1, wxEXPAND, 0);
    grid_vargachart->Add(sizer_south, 1, wxEXPAND, 0);
    notebook_pane_vargachart->SetSizer(grid_vargachart);
    grid_vargachart->AddGrowableCol(0);
    sizer_sbc_options->Add(check_sanskrit_symbols, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_sbc_options->Add(check_nakshatra_qualities, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sbc->Add(sizer_sbc_options, 1, wxEXPAND, 0);
    notebook_pane_sbc->SetSizer(grid_sbc);
    grid_sbc->AddGrowableCol(0);
    grid_aspects->Add(label_aspected, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_aspects->Add(choice_aspected, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_aspects->Add(label_aspecting, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_aspects->Add(choice_aspecting, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_aspects->Add(label_all_aspects, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_aspects->Add(choice_all_aspects, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_aspects->Add(label_aspect_type, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_aspects->Add(choice_aspect_type, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_aspects->Add(label_minimum_strength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_aspects->Add(choice_minimum_strength, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_aspects->Add(label_benefic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_aspects->Add(choice_benefic, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_aspects->AddGrowableCol(1);
    sizer_aspect_options->Add(grid_aspects, 1, wxALL|wxEXPAND, 3);
    sizer_aspect_options->Add(check_include_asmc, 0, wxALL, 3);
    sizer_aspect->Add(sizer_aspect_options, 1, wxEXPAND, 3);
    notebook_pane_responsive->SetSizer(sizer_aspect);
    notebook->AddPage(notebook_pane_general, _("General"));
    notebook->AddPage(notebook_pane_vargachart, _("Varga Chart"));
    notebook->AddPage(notebook_pane_sbc, _("SBC"));
    notebook->AddPage(notebook_pane_responsive, _("Aspects"));
    sizer_left->Add(notebook, 0, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_left, 0, wxEXPAND, 0);
    grid_preview->Add(window_preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_preview);
    grid_preview->AddGrowableRow(0);
    grid_preview->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_preview->Add(checktransit, 0, 0, 0);
    sizer_right->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(1);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createVedicChartPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createVedicChartPanel( wxWindow *parent )
{
	return new VedicChartPanel( parent );
}

