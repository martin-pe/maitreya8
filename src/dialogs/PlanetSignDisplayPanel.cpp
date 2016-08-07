/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PlanetSignDisplayPanel.cpp
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

#include "PlanetSignDisplayPanel.h"

#include "Conf.h"
#include "ChartProperties.h"
#include "SheetWidget.h"
#include "guibase.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mvalidator.h"
#include "PlanetList.h"
#include "Table.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

IMPLEMENT_CLASS( PlanetSignDisplayPanel, ConfigPanel )

extern Config *config;

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   Constructor
**
******************************************************/
PlanetSignDisplayPanel::PlanetSignDisplayPanel( wxWindow* parent ) : ConfigPanel( parent )
{
		cfg = new WriterConfig( *config->writer );
		props = new ChartProperties;
		horoscope = new Horoscope;

    // begin wxGlade: PlanetSignDisplayPanel::PlanetSignDisplayPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_positions_staticbox = new wxStaticBox(this, wxID_ANY, _("Display of Positions"));
    sizer_signs_staticbox = new wxStaticBox(this, wxID_ANY, _("Display of Signs"));
    sizer_preview_staticbox = new wxStaticBox(this, wxID_ANY, _("Preview"));
    sizer_planets_staticbox = new wxStaticBox(this, wxID_ANY, _("Display of Planets"));
    check_planet_symbols = new wxCheckBox(this, wxID_ANY, _("Use Symbols for Planets"));
    check_vedic_planet_names = new wxCheckBox(this, wxID_ANY, _("Vedic Planet Names"));
    label_uranus = new wxStaticText(this, wxID_ANY, _("Uranus"));
    const wxString choice_uranus_choices[] = {
        _("Symbol 1"),
        _("Symbol 2")
    };
    choice_uranus = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_uranus_choices, 0);
    label_pluto = new wxStaticText(this, wxID_ANY, _("Pluto"));
    const wxString choice_pluto_choices[] = {
        _("Symbol 1"),
        _("Symbol 2"),
        _("Symbol 3")
    };
    choice_pluto = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_pluto_choices, 0);
    check_vedic_positions = new wxCheckBox(this, wxID_ANY, _("Vedic Position Format"));
    check_sign_symbols = new wxCheckBox(this, wxID_ANY, _("Use Symbols for Signs"));
    check_vedic_sign_names = new wxCheckBox(this, wxID_ANY, _("Vedic Sign Names"));
    label_capricorn = new wxStaticText(this, wxID_ANY, _("Capricorn"));
    const wxString choice_capricorn_choices[] = {
        _("Symbol 1"),
        _("Symbol 2")
    };
    choice_capricorn = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_capricorn_choices, 0);
    theText = new SheetWidget(panel_preview, props, 0, cfg);

    set_properties();
    do_layout();
    // end wxGlade

	check_planet_symbols->SetValidator( MCheckValidator( &cfg->planetSymbols ));
	check_vedic_planet_names->SetValidator( MCheckValidator( &cfg->vedicPlanetNames ));
	choice_uranus->SetValidator( MChoiceValidator( &cfg->uranusSymbol ));
	choice_pluto->SetValidator( MChoiceValidator( &cfg->plutoSymbol ));

	check_sign_symbols->SetValidator( MCheckValidator( &(cfg->signSymbols) ));
	check_vedic_sign_names->SetValidator( MCheckValidator( &cfg->vedicSignNames ));
	choice_capricorn->SetValidator( MChoiceValidator( &cfg->capricornSymbol ));

	check_vedic_positions->SetValidator( MCheckValidator( &cfg->vedicPositions ));

	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlanetSignDisplayPanel::OnChoice ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PlanetSignDisplayPanel::OnChoice ));
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   Destructor
**
******************************************************/
PlanetSignDisplayPanel::~PlanetSignDisplayPanel()
{
	delete cfg;
	delete props;
	delete horoscope;
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   config2model
**
******************************************************/
void PlanetSignDisplayPanel::config2model()
{
	*cfg = WriterConfig( *config->writer );
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   model2config
**
******************************************************/
void PlanetSignDisplayPanel::model2config()
{
	*config->writer = *cfg;
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   writeTextContents
**
******************************************************/
void PlanetSignDisplayPanel::writeTextContents()
{
	printf( "WRITE\n" );
	wxString s;
	Lang lang( cfg );
	SheetFormatter fmt( cfg );

	Sheet *sheet = theText->getSheet();
	sheet->clear();

	horoscope->setCurrentDate();
	horoscope->update();
	ObjectArray obs = PlanetList().getVedicObjectList( OI_OUTER );
	Table *table = new Table( 2, 10 );

	int line = 0;
	for ( uint i = 0; i < obs.size(); i++ )
	{
		if ( obs[i] == OJUPITER ) continue;
		if ( obs[i] == OSATURN ) continue;
		if ( obs[i] == ONEPTUNE ) continue;
		table->setEntry( 0, line, fmt.getObjectName( obs[i], TF_LONG, true ));
		table->setEntry( 1, line++,
			fmt.getPosFormatted( horoscope->getVedicLongitude( obs[i] ), horoscope->getMovingDirection( obs[i] ), DEG_PRECISION_SECOND ));
	}
	for ( uint i = 8; i < 11; i++ )
	{
		if ( cfg->vedicPlanetNames ) s = lang.getBhavaName( i );
		else s.Printf( wxT( "%02d" ), i+1 );
		table->setEntry( 0, line, s );
		table->setEntry( 1, line++, fmt.getPosFormatted( horoscope->getHouse( i, true ), MD_DIRECT, DEG_PRECISION_SECOND ));
	}
	sheet->addItem( table );
	theText->OnDataChanged();
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   OnChoice
**
******************************************************/
void PlanetSignDisplayPanel::OnChoice( wxCommandEvent& )
{
	cfg->planetSymbols = check_planet_symbols->GetValue();
	cfg->vedicPlanetNames = check_vedic_planet_names->GetValue();
	cfg->uranusSymbol = choice_uranus->GetSelection();
	cfg->plutoSymbol = choice_pluto->GetSelection();

	cfg->signSymbols = check_sign_symbols->GetValue();
	cfg->vedicSignNames = check_vedic_sign_names->GetValue();
	cfg->capricornSymbol = choice_capricorn->GetSelection();

	cfg->vedicPositions = check_vedic_positions->GetValue();

	updateUi();
	setDirty();
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   updateUi
**
******************************************************/
void PlanetSignDisplayPanel::updateUi()
{
	const bool psymbols = check_planet_symbols->GetValue();
	check_vedic_planet_names->Enable( ! psymbols );
	choice_uranus->Enable( psymbols );
	choice_pluto->Enable( psymbols );

	const bool vpositions = check_vedic_positions->GetValue();
	const bool ssymbols = check_sign_symbols->GetValue();

	check_sign_symbols->Enable( ! vpositions );
	check_vedic_sign_names->Enable(! vpositions && ! ssymbols );
	choice_capricorn->Enable( ! vpositions && ssymbols );

	writeTextContents();
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   restoreDefaults
**
******************************************************/
void PlanetSignDisplayPanel::restoreDefaults()
{
	*cfg = WriterConfig();
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   set_properties
**
******************************************************/
void PlanetSignDisplayPanel::set_properties()
{
    // begin wxGlade: PlanetSignDisplayPanel::set_properties
    label_uranus->SetMinSize(wxSize(80, -1));
    choice_uranus->SetSelection(0);
    choice_pluto->SetSelection(0);
    choice_capricorn->SetSelection(0);
    theText->SetMinSize(wxSize(300, 300));
    // end wxGlade
}

/*****************************************************
**
**   PlanetSignDisplayPanel   ---   do_layout
**
******************************************************/
void PlanetSignDisplayPanel::do_layout()
{
    // begin wxGlade: PlanetSignDisplayPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_preview_staticbox->Lower();
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_signs_staticbox->Lower();
    wxStaticBoxSizer* sizer_signs = new wxStaticBoxSizer(sizer_signs_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_signs = new wxFlexGridSizer(1, 2, 3, 3);
    sizer_positions_staticbox->Lower();
    wxStaticBoxSizer* sizer_positions = new wxStaticBoxSizer(sizer_positions_staticbox, wxVERTICAL);
    sizer_planets_staticbox->Lower();
    wxStaticBoxSizer* sizer_planets = new wxStaticBoxSizer(sizer_planets_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_planets = new wxFlexGridSizer(2, 2, 3, 3);
    sizer_planets->Add(check_planet_symbols, 0, wxALL, 3);
    sizer_planets->Add(check_vedic_planet_names, 0, wxALL, 3);
    grid_planets->Add(label_uranus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_planets->Add(choice_uranus, 0, wxALL|wxEXPAND, 3);
    grid_planets->Add(label_pluto, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_planets->Add(choice_pluto, 0, wxALL|wxEXPAND, 3);
    grid_planets->AddGrowableCol(1);
    sizer_planets->Add(grid_planets, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_planets, 1, wxALL|wxEXPAND, 3);
    sizer_positions->Add(check_vedic_positions, 0, wxALL, 3);
    sizer_left->Add(sizer_positions, 1, wxALL|wxEXPAND, 3);
    sizer_signs->Add(check_sign_symbols, 0, wxALL, 3);
    sizer_signs->Add(check_vedic_sign_names, 0, wxALL, 3);
    grid_signs->Add(label_capricorn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(choice_capricorn, 0, wxALL|wxEXPAND, 3);
    grid_signs->AddGrowableCol(1);
    sizer_signs->Add(grid_signs, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_signs, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_previewpanel->Add(theText, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
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
**   ConfigPanelFactory   ---   createPlanetSignDisplayPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createPlanetSignDisplayPanel( wxWindow *parent )
{
	return new PlanetSignDisplayPanel( parent );
}

