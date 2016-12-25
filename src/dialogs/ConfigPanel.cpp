/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ConfigPanel.cpp
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

#include "ConfigPanel.h"
#include "Conf.h"

#include <wx/clrpicker.h>
#include <wx/spinctrl.h>

IMPLEMENT_CLASS( ConfigPanel, wxPanel )

DEFINE_EVENT_TYPE( CONFIG_CHANGED )
DEFINE_EVENT_TYPE( CONFIG_TOOLBAR_CHANGED )
DEFINE_EVENT_TYPE( CONFIG_PANEL_CHANGED )

/*****************************************************
**
**   ConfigPanel   ---   Constructor
**
******************************************************/
ConfigPanel::ConfigPanel( wxWindow* parent, const bool showRestoreDefaultButton )
	: wxPanel( parent, -1 , wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWS_EX_VALIDATE_RECURSIVELY ),
	showDefaultButton( showRestoreDefaultButton )
{
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	dirty = false;
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ConfigPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigPanel::OnSpin ));
	//Connect( wxID_ANY, wxEVT_SPINCTRL, wxSpinEventHandler( ConfigPanel::OnSpin ));
	Connect( wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigPanel::OnText ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxCommandEventHandler( ConfigPanel::OnCommand ));
}

/*****************************************************
**
**   ConfigPanel   ---   setDirty
**
******************************************************/
void ConfigPanel::setDirty( const bool b )
{
	//printf( "ConfigPanel::setDirty %d\n", b );
	if ( dirty != b )
	{
		wxCommandEvent e( CONFIG_PANEL_CHANGED, GetId() );
		e.SetInt( b );
		wxPostEvent( GetParent(), e );
	}
	dirty = b;
}

/*****************************************************
**
**   ConfigPanel   ---   OnSize
**
******************************************************/
void ConfigPanel::OnSize( wxSizeEvent &event )
{
	//int a = event.GetSize().GetWidth();
	//int b = event.GetSize().GetHeight();
	event.Skip();
}

/*****************************************************
**
**   ConfigPanel   ---   OnCommand
**
******************************************************/
void ConfigPanel::OnCommand( wxCommandEvent &event )
{
	//printf( "ConfigPanel::OnCommand\n" );
	setDirty();
	event.Skip();
}

/*****************************************************
**
**   ConfigPanel   ---   OnSpin
**
******************************************************/
void ConfigPanel::OnSpin( wxSpinEvent &event )
{
	//printf( "ConfigPanel::OnSpin\n" );
	setDirty();
	event.Skip();
}

/*****************************************************
**
**   ConfigPanel   ---   OnText
**
******************************************************/
void ConfigPanel::OnText( wxCommandEvent &event )
{
	//printf( "ConfigPanel::OnText\n" );
	setDirty();
	event.Skip();
}

/*****************************************************
**
**   ConfigPanel   ---   onActivate
**
******************************************************/
void ConfigPanel::onActivate()
{
	//printf( "ConfigPanel::onActivate\n" );
	updateUi();
}

/*****************************************************
**
**   ConfigPanel   ---   onPassivate
**
******************************************************/
void ConfigPanel::onPassivate()
{
	//printf( "ConfigPanel::onPassivate\n" );
}

/*****************************************************
**
**   ConfigPanelFactory   ---   getPanelName
**
******************************************************/
wxString ConfigPanelFactory::getPanelName( const int &id )
{
	assert( id >= 0 && id < NB_PANELS );
	const wxString notebook_title[NB_PANELS] =
	{
		// General section
		_( "General"),
		_( "Ephemeris" ),
		_( "Multiple Views" ),
		_( "Atlas" ),
		_( "Print" ),
		_( "Animation" ),

		// User Interface
		_( "User Interface" ),  // = SheetStylePanel
		_( "Planets and Signs" ),
		_( "Colors" ),
		_( "Fonts" ),
		_( "Toolbar" ),

		// Vedic 
		_( "Vedic Astrology" ),
		_( "Chart"),
		_( "Calculation" ),

		// Western 
		_( "Western Astrology" ),
		_( "Chart" ),
		_( "Calculation" ),
		_( "Aspects" ),

		_( "Miscellaneous" )
	};

	return notebook_title[id];
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createPanel( const int &id, wxWindow *parent )
{
	assert( id >= 0 && id < NB_PANELS );
	ConfigPanel *panel;
	switch ( id )
	{
	case CONFIGPANEL_GENERAL:
		panel = createGeneralPanel( parent );
		break;
	case CONFIGPANEL_MULTIPLEVIEW:
		panel = createMultipleViewPanel( parent );
		break;
	case CONFIGPANEL_ATLAS:
		panel = createAtlasPanel( parent );
		break;
	case CONFIGPANEL_EPHEMERIS:
		panel = createEphemerisPanel( parent );
		break;
	case CONFIGPANEL_PRINTOUT:
		panel = createPrintoutPanel( parent );
		break;
	case CONFIGPANEL_ANIMATION:
		panel = createAnimationPanel( parent );
		break;
	case CONFIGPANEL_MISCELLANEOUS:
		panel = createMiscellaneousPanel( parent );
		break;
	case CONFIGPANEL_PLANETSIGNDISPLAY:
		panel = createPlanetSignDisplayPanel( parent );
		break;
	case CONFIGPANEL_FONT:
		panel = createFontPanel( parent );
		break;
	case CONFIGPANEL_SHEETSTYLE:
		panel = createSheetStylePanel( parent );
		break;
	case CONFIGPANEL_COLOR:
		panel = createColorPanel( parent );
		break;
	case CONFIGPANEL_TOOLBAR:
		panel = createToolbarPanel( parent );
		break;
	case CONFIGPANEL_VEDIC:
		panel = createVedicPanel( parent );
		break;
	case CONFIGPANEL_VEDICCHART:
		panel = createVedicChartPanel( parent );
		break;
	case CONFIGPANEL_VEDICCALCULATION:
		panel = createVedicCalculationPanel( parent );
		break;
	case CONFIGPANEL_WESTERN:
		panel = createWesternPanel( parent );
		break;
	case CONFIGPANEL_WESTERNCHART:
		panel = createWesternChartPanel( parent );
		break;
	case CONFIGPANEL_WESTERNCALCULATION:
		panel = createWesternCalculationPanel( parent );
		break;
	case CONFIGPANEL_WESTERNASPECT:
		panel = createAspectPanel( parent );
		break;
	default:
		assert( false );
		break;
	}
	assert( panel );
	return panel;
}


