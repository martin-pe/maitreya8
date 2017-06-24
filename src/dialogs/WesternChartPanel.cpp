/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternChartPanel.cpp
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

#include "WesternChartPanel.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "GraphicGrid.h"
#include "mvalidator.h"
#include "WesternChartConfig.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

enum { CD_SKIN_SELECT = wxID_HIGHEST + 1, CD_ASCENDANT, CD_SHOW_ASPECTS, CD_SHOW_RETRO, CD_SHOW_HOUSES, CD_USE_ASPECT_COLORS,
	CD_USE_SIGN_COLORS, CD_USE_PLANET_COLORS, CD_USE_HOUSE_COLORS, CD_SHOW_ASPECT_SYMBOLS, CD_SHOW_SECONDCHART,
	CD_SECONDCHART_STYLE, CD_HOUSE_NUMBERS, CD_PREVIEW_WINDOW };

IMPLEMENT_CLASS( WesternChartPanel, ConfigPanel )

/*****************************************************
**
**   WesternChartPanel   ---   Constructor
**
******************************************************/
WesternChartPanel::WesternChartPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	chartprops = new ChartProperties;
	config2model();

    // begin wxGlade: WesternChartPanel::WesternChartPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    notebook = new wxNotebook(this, wxID_ANY);
    notebook_pane_responsive = new wxPanel(notebook, wxID_ANY);
    notebook_pane_color = new wxPanel(notebook, wxID_ANY);
    notebook_pane_general = new wxPanel(notebook, wxID_ANY);
    const wxString choice_defaultskin_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
    };
    choice_defaultskin = new wxChoice(notebook_pane_general, CD_SKIN_SELECT, wxDefaultPosition, wxDefaultSize, 5, choice_defaultskin_choices);
    label_orientation = new wxStaticText(notebook_pane_general, wxID_ANY, _("Chart Orientation"));
    const wxString choice_leftside_choices[] = {
        _("Ascendant left"),
        _("1st House left"),
        _("Meridian top"),
        _("0 Aries top"),
        _("15 Aries top"),
        _("0 Cancer top"),
        _("15 Cancer top"),
        _("0 Leo top"),
        _("0 Libra top"),
        _("15 Libra top"),
        _("0 Capricorn top"),
        _("15 Capricorn top"),
    };
    choice_leftside = new wxChoice(notebook_pane_general, CD_ASCENDANT, wxDefaultPosition, wxDefaultSize, 12, choice_leftside_choices);
    label_transitstyle = new wxStaticText(notebook_pane_general, wxID_ANY, _("Secondary Chart"));
    const wxString choice_secondchart_style_choices[] = {
        _("Outsite"),
        _("Inside"),
    };
    choice_secondchart_style = new wxChoice(notebook_pane_general, CD_SECONDCHART_STYLE, wxDefaultPosition, wxDefaultSize, 2, choice_secondchart_style_choices);
    label_house_number_style = new wxStaticText(notebook_pane_general, wxID_ANY, _("House Number Style"));
    const wxString choice_house_number_style_choices[] = {
        _("None"),
        _("Arabic"),
        _("Roman"),
    };
    choice_house_number_style = new wxChoice(notebook_pane_general, CD_HOUSE_NUMBERS, wxDefaultPosition, wxDefaultSize, 3, choice_house_number_style_choices);
    check_retro = new wxCheckBox(notebook_pane_general, CD_SHOW_RETRO, _("Retrogression (R)"));
    check_aspects = new wxCheckBox(notebook_pane_general, CD_SHOW_ASPECTS, _("Aspects"));
    check_aspectsymbols = new wxCheckBox(notebook_pane_general, CD_SHOW_ASPECT_SYMBOLS, _("Aspect Symbols"));
    check_houses = new wxCheckBox(notebook_pane_general, CD_SHOW_HOUSES, _("Houses"));
    check_use_planet_colors = new wxCheckBox(notebook_pane_color, CD_USE_PLANET_COLORS, _("Planets"));
    check_use_sign_colors = new wxCheckBox(notebook_pane_color, CD_USE_SIGN_COLORS, _("Signs"));
    check_use_house_colors = new wxCheckBox(notebook_pane_color, CD_USE_HOUSE_COLORS, _("Houses"));
    check_use_aspect_colors = new wxCheckBox(notebook_pane_color, CD_USE_ASPECT_COLORS, _("Aspects"));
    check_zoom = new wxCheckBox(notebook_pane_responsive, wxID_ANY, _("Zoom"));
    spin_zoomfactor = new wxSpinCtrl(notebook_pane_responsive, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 50, 300);
    label_percent = new wxStaticText(notebook_pane_responsive, wxID_ANY, _("%"));
    check_magnify_aspects = new wxCheckBox(notebook_pane_responsive, wxID_ANY, _("Magnify Related Aspects"));
    window_preview = new ChartGridWidget(panel_preview, CT_RADIX, chartprops, 1, 1, CD_PREVIEW_WINDOW);
    check_secondchart = new wxCheckBox(this, CD_SHOW_SECONDCHART, _("Show Secondary Chart"));

    set_properties();
    do_layout();
    // end wxGlade

	horoscope = new Horoscope();
	horoscope->update();

	htransit = new Horoscope();
	htransit->setDate( MDate::getCurrentJD() + 36000 );
	htransit->update();

	window_preview->addWesternChart( horoscope, 0 );

	choice_defaultskin->Clear();
	choice_defaultskin->Append( WesternChartConfigLoader::get()->getNamesAsArray());

	check_secondchart->SetValue( config->viewprefs->configDialogWChartPreviewTransits );
	notebook->SetSelection( config->viewprefs->configDialogWChartPanelPage );

	{
		WesternChartDisplayConfig &style = chartprops->getWesternChartDisplayConfig();
		choice_defaultskin->SetValidator( MChoiceValidator( &style.graphicSkin ));

		check_aspects->SetValidator( MCheckValidator( &style.showAspects ));
		check_aspectsymbols->SetValidator( MCheckValidator( &style.showAspectSymbols ));
		check_retro->SetValidator( MCheckValidator( &style.showRetro ));
		check_houses->SetValidator( MCheckValidator( &style.showHouses ));

		choice_leftside->SetValidator( MChoiceValidator( &style.chartOrientation ));
		choice_secondchart_style->SetValidator( MChoiceValidator( &style.secondchartStyle ));
		choice_house_number_style->SetValidator( MChoiceValidator( &style.houseNumberStyle ));

		// color tab
		check_use_sign_colors->SetValidator( MCheckValidator( &style.showSignColors ));
		check_use_planet_colors->SetValidator( MCheckValidator( &style.showPlanetColors ));
		check_use_house_colors->SetValidator( MCheckValidator( &style.showHouseColors ));
		check_use_aspect_colors->SetValidator( MCheckValidator( &style.showAspectColors ));
	}

	{
		// responsive tab
		WesternChartBehaviorConfig &bcfg = chartprops->getWesternChartBehaviorConfig();
		check_zoom->SetValidator( MCheckValidator( &bcfg.zoomOnMouseOver ));
		spin_zoomfactor->SetValidator( MSpinValidator( &bcfg.zoomFactor ));
		check_magnify_aspects->SetValidator( MCheckValidator( &bcfg.magnifyAspects ));
	}

	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WesternChartPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WesternChartPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WesternChartPanel::OnSpin ));
	Connect( CD_PREVIEW_WINDOW, COMMAND_CHART_SKIN_CHANGED, wxCommandEventHandler( WesternChartPanel::OnChartSkinChanged ));
}

/*****************************************************
**
**   WesternChartPanel   ---   Destructor
**
******************************************************/
WesternChartPanel::~WesternChartPanel()
{
	config->viewprefs->configDialogWChartPreviewTransits = check_secondchart->GetValue();
	config->viewprefs->configDialogWChartPanelPage = notebook->GetSelection();
	delete chartprops;
	delete horoscope;
	delete htransit;
}

/*****************************************************
**
**   WesternChartPanel   ---   config2model
**
******************************************************/
void WesternChartPanel::config2model()
{
	*chartprops = ChartProperties();
	chartprops->setVedic( false );
}

/*****************************************************
**
**   WesternChartPanel   ---   model2config
**
******************************************************/
void WesternChartPanel::model2config()
{
	*config->westernChart = chartprops->getWesternChartDisplayConfig();
	*config->westernChartBehavior = chartprops->getWesternChartBehaviorConfig();
}

/*****************************************************
**
**   WesternChartPanel   ---   OnChartSkinChanged
**
******************************************************/
void WesternChartPanel::OnChartSkinChanged( wxCommandEvent& )
{
	int skin = window_preview->getChartProperties()->getSkin();
	if ( skin != choice_defaultskin->GetSelection())
	{
    choice_defaultskin->SetSelection( skin );
	}
	setDirty();
}

/*****************************************************
**
**   WesternChartPanel   ---   OnGuiElementChanged
**
******************************************************/
void WesternChartPanel::OnGuiElementChanged( const int id )
{
	{
		WesternChartDisplayConfig &style = chartprops->getWesternChartDisplayConfig();
		style.showAspects = check_aspects->GetValue();
		style.showAspectSymbols = check_aspectsymbols->GetValue();
		style.showRetro = check_retro->GetValue();
		style.showHouses = check_houses->GetValue();

		style.showSignColors = check_use_sign_colors->GetValue();
		style.showPlanetColors = check_use_planet_colors->GetValue();
		style.showHouseColors = check_use_house_colors->GetValue();
		style.showAspectColors = check_use_aspect_colors->GetValue();

		style.secondchartStyle = choice_secondchart_style->GetSelection();
		style.graphicSkin = choice_defaultskin->GetSelection();
		style.chartOrientation = choice_leftside->GetSelection();
		style.houseNumberStyle = choice_house_number_style->GetSelection();
	}

	{
		WesternChartBehaviorConfig &bcfg = chartprops->getWesternChartBehaviorConfig();
		bcfg.zoomOnMouseOver = check_zoom->GetValue();
		bcfg.zoomFactor = spin_zoomfactor->GetValue();
		bcfg.magnifyAspects = check_magnify_aspects->GetValue();
	}

	updateUi();
	if ( id != CD_SHOW_SECONDCHART )
	{
		setDirty();
	}
}

/*****************************************************
**
**   WesternChartPanel   ---   OnCommand
**
******************************************************/
void WesternChartPanel::OnCommand( wxCommandEvent &event )
{
	OnGuiElementChanged( event.GetId());
}

/*****************************************************
**
**   WesternChartPanel   ---   OnSpin
**
******************************************************/
void WesternChartPanel::OnSpin( wxSpinEvent& )
{
	OnGuiElementChanged();
}

/*****************************************************
**
**   WesternChartPanel   ---   updateUi
**
******************************************************/
void WesternChartPanel::updateUi()
{
	check_use_aspect_colors->Enable( chartprops->getWesternChartDisplayConfig().showAspects );
	check_aspectsymbols->Enable( chartprops->getWesternChartDisplayConfig().showAspects );
	check_use_house_colors->Enable( chartprops->getWesternChartDisplayConfig().showHouses );
	spin_zoomfactor->Enable( check_zoom->GetValue());

	window_preview->setHoroscopes( horoscope, check_secondchart->GetValue() ? htransit : (Horoscope*)NULL );
	window_preview->OnDataChanged();
	window_preview->Refresh();
}

/*****************************************************
**
**   WesternChartPanel   ---   restoreDefaults
**
******************************************************/
void WesternChartPanel::restoreDefaults()
{
	*chartprops = ChartProperties( false );
	chartprops->setVedic( false );
}

/*****************************************************
**
**   WesternChartPanel   ---   set_properties
**
******************************************************/
void WesternChartPanel::set_properties()
{
    // begin wxGlade: WesternChartPanel::set_properties
    choice_defaultskin->SetSelection(0);
    choice_leftside->SetSelection(0);
    choice_secondchart_style->SetSelection(0);
    choice_house_number_style->SetSelection(0);
    panel_preview->SetMinSize(wxSize(100, 100));
    // end wxGlade
}

/*****************************************************
**
**   WesternChartPanel   ---   do_layout
**
******************************************************/
void WesternChartPanel::do_layout()
{
    // begin wxGlade: WesternChartPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Preview")), wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* sizer_responsive = new wxFlexGridSizer(1, 1, 3, 3);
    wxStaticBoxSizer* sizer_mouseover = new wxStaticBoxSizer(new wxStaticBox(notebook_pane_responsive, wxID_ANY, _("Object Mouse Over Behavior")), wxVERTICAL);
    wxFlexGridSizer* grid_zoom = new wxFlexGridSizer(1, 3, 0, 0);
    wxFlexGridSizer* sizer_color = new wxFlexGridSizer(1, 1, 3, 3);
    wxStaticBoxSizer* sizer_colors = new wxStaticBoxSizer(new wxStaticBox(notebook_pane_color, wxID_ANY, _("Foreground Colors")), wxVERTICAL);
    wxFlexGridSizer* sizer_general = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_show = new wxStaticBoxSizer(new wxStaticBox(notebook_pane_general, wxID_ANY, _("Show in Chart")), wxVERTICAL);
    wxStaticBoxSizer* sizer_chartoptions = new wxStaticBoxSizer(new wxStaticBox(notebook_pane_general, wxID_ANY, _("Chart Options")), wxVERTICAL);
    wxGridSizer* grid_options = new wxGridSizer(3, 2, 0, 0);
    wxStaticBoxSizer* sizer_defaultskin = new wxStaticBoxSizer(new wxStaticBox(notebook_pane_general, wxID_ANY, _("Skin")), wxVERTICAL);
    wxFlexGridSizer* grid_defaultview = new wxFlexGridSizer(1, 1, 3, 3);
    grid_defaultview->Add(choice_defaultskin, 0, wxALL|wxEXPAND, 3);
    grid_defaultview->AddGrowableCol(0);
    sizer_defaultskin->Add(grid_defaultview, 1, wxEXPAND, 0);
    sizer_general->Add(sizer_defaultskin, 0, wxALL|wxEXPAND, 3);
    grid_options->Add(label_orientation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_options->Add(choice_leftside, 0, wxALIGN_RIGHT|wxALL, 3);
    grid_options->Add(label_transitstyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_options->Add(choice_secondchart_style, 0, wxALIGN_RIGHT|wxALL, 3);
    grid_options->Add(label_house_number_style, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_options->Add(choice_house_number_style, 0, wxALIGN_RIGHT|wxALL, 3);
    sizer_chartoptions->Add(grid_options, 1, 0, 0);
    sizer_general->Add(sizer_chartoptions, 0, wxALL|wxEXPAND, 3);
    sizer_show->Add(check_retro, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_show->Add(check_aspects, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_show->Add(check_aspectsymbols, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_show->Add(check_houses, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_general->Add(sizer_show, 1, wxEXPAND, 0);
    notebook_pane_general->SetSizer(sizer_general);
    sizer_general->AddGrowableCol(0);
    sizer_colors->Add(check_use_planet_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_sign_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_house_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_aspect_colors, 0, wxALL|wxEXPAND, 3);
    sizer_color->Add(sizer_colors, 1, wxALL|wxEXPAND, 3);
    notebook_pane_color->SetSizer(sizer_color);
    sizer_color->AddGrowableCol(0);
    grid_zoom->Add(check_zoom, 1, wxALL|wxEXPAND, 3);
    grid_zoom->Add(spin_zoomfactor, 0, wxALIGN_RIGHT|wxALL, 3);
    grid_zoom->Add(label_percent, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_zoom->AddGrowableCol(0);
    sizer_mouseover->Add(grid_zoom, 1, wxEXPAND, 0);
    sizer_mouseover->Add(check_magnify_aspects, 0, wxALL|wxEXPAND, 3);
    sizer_responsive->Add(sizer_mouseover, 1, wxALL|wxEXPAND, 3);
    notebook_pane_responsive->SetSizer(sizer_responsive);
    sizer_responsive->AddGrowableCol(0);
    notebook->AddPage(notebook_pane_general, _("General"));
    notebook->AddPage(notebook_pane_color, _("Colors"));
    notebook->AddPage(notebook_pane_responsive, _("Mouse Over"));
    sizer_main->Add(notebook, 1, wxEXPAND, 0);
    grid_previewpanel->Add(window_preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_preview->Add(check_secondchart, 0, wxALL, 3);
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
**   ConfigPanelFactory   ---   createWesternChartPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createWesternChartPanel( wxWindow *parent )
{
	return new WesternChartPanel( parent );
}

