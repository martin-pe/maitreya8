/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/WesternCalculationPanel.cpp
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

#include "WesternCalculationPanel.h"

#include "Conf.h"
#include "DialogElements.h"
#include "mvalidator.h"

#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

enum { CD_YL_CHOICE = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( WesternCalculationPanel, ConfigPanel )

/*****************************************************
**
**   WesternCalculationPanel   ---   Constructor
**
******************************************************/
WesternCalculationPanel::WesternCalculationPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	wconfig = new WesternCalculationConfig;
	config2model();

    // begin wxGlade: WesternCalculationPanel::WesternCalculationPanel
    sizer_yl_staticbox = new wxStaticBox(this, wxID_ANY, _("Year Length"));
    sizer_wcalc_staticbox = new wxStaticBox(this, wxID_ANY, _("Calculation Options"));
    label_wcalc_aya = new wxStaticText(this, wxID_ANY, _("Ayanamsa"));
    const wxString choice_ayanamsa_choices[] = {
        _("None"),
        _("Lahiri"),
        _("Raman"),
        _("Krishnamurti")
    };
    choice_ayanamsa = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_ayanamsa_choices, 0);
    label_wcalc_node = new wxStaticText(this, wxID_ANY, _("Lunar Node"));
    const wxString choice_wnode_choices[] = {
        _("True"),
        _("Mean")
    };
    choice_wnode = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_wnode_choices, 0);
    label_wcalc_house = new wxStaticText(this, wxID_ANY, _("House System"));
    const wxString choice_whouse_choices[] = {
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
    choice_whouse = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 13, choice_whouse_choices, 0);
    choice_yl = new YearLengthChoice(this, CD_YL_CHOICE, false);
    text_custom_yl = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

	choice_ayanamsa->SetValidator( MChoiceValidator( (int*)&wconfig->ayanamsa ));
	choice_whouse->SetValidator( MChoiceValidator( (int*)&wconfig->houseSystem ));
	choice_wnode->SetValidator( MChoiceValidator( &wconfig->lunarNodeMode ));

	choice_yl->SetValidator( MChoiceValidator( &wconfig->yearLength ));
	text_custom_yl->SetValidator( MDoubleValidator( &wconfig->customYearLength, 300, 400 ));

	Connect( CD_YL_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WesternCalculationPanel::OnChoice ));
}

/*****************************************************
**
**   WesternCalculationPanel   ---   Destructor
**
******************************************************/
WesternCalculationPanel::~WesternCalculationPanel()
{
	delete wconfig;
}

/*****************************************************
**
**   WesternCalculationPanel   ---   life cycle
**
******************************************************/
void WesternCalculationPanel::config2model() { *wconfig = *config->westernCalculation; }
void WesternCalculationPanel::model2config() { *config->westernCalculation = *wconfig; }
void WesternCalculationPanel::restoreDefaults() { *wconfig = WesternCalculationConfig(); }

/*****************************************************
**
**   WesternCalculationPanel   ---   updateUi
**
******************************************************/
void WesternCalculationPanel::updateUi()
{
	text_custom_yl->Enable( choice_yl->GetSelection() == 4 );
}

/*****************************************************
**
**   WesternCalculationPanel   ---   OnChoice
**
******************************************************/
void WesternCalculationPanel::OnChoice( wxCommandEvent& )
{
	updateUi();
	setDirty();
}

/*****************************************************
**
**   WesternCalculationPanel   ---   set_properties
**
******************************************************/
void WesternCalculationPanel::set_properties()
{
    // begin wxGlade: WesternCalculationPanel::set_properties
    choice_ayanamsa->SetSelection(0);
    choice_wnode->SetToolTip(_("Western astrologers most likely use True Node"));
    choice_wnode->SetSelection(0);
    choice_whouse->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   WesternCalculationPanel   ---   do_layout
**
******************************************************/
void WesternCalculationPanel::do_layout()
{
    // begin wxGlade: WesternCalculationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_yl_staticbox->Lower();
    wxStaticBoxSizer* sizer_yl = new wxStaticBoxSizer(sizer_yl_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_yl = new wxFlexGridSizer(5, 2, 0, 0);
    sizer_wcalc_staticbox->Lower();
    wxStaticBoxSizer* sizer_wcalc = new wxStaticBoxSizer(sizer_wcalc_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_calcoptions = new wxFlexGridSizer(3, 2, 0, 0);
    grid_calcoptions->Add(label_wcalc_aya, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_calcoptions->Add(choice_ayanamsa, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_calcoptions->Add(label_wcalc_node, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_calcoptions->Add(choice_wnode, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_calcoptions->Add(label_wcalc_house, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_calcoptions->Add(choice_whouse, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_calcoptions->AddGrowableCol(1);
    sizer_wcalc->Add(grid_calcoptions, 1, wxALL, 3);
    sizer_right->Add(sizer_wcalc, 1, wxALL, 3);
    wxStaticText* label_yl = new wxStaticText(this, wxID_ANY, _("Year Length"));
    grid_yl->Add(label_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(choice_yl, 1, wxALL|wxALIGN_RIGHT, 3);
    wxStaticText* label_custom_yl = new wxStaticText(this, wxID_ANY, _("Custom Length"));
    grid_yl->Add(label_custom_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(text_custom_yl, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->AddGrowableCol(1);
    sizer_yl->Add(grid_yl, 1, 0, 0);
    sizer_right->Add(sizer_yl, 1, wxALL, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createWesternCalculationPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createWesternCalculationPanel( wxWindow *parent )
{
	return new WesternCalculationPanel( parent );
}

