/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ToolPanel.cpp
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

#include "ToolPanel.h"

#include <wx/combobox.h>
#include <wx/log.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Commands.h"
#include "Conf.h"
#include "Dasa.h"
#include "DasaTool.h"
#include "DialogElements.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "MenuProvider.h"
#include "mspin.h"
#include "PrintoutConfig.h"
#include "SheetConfig.h"
#include "VargaBase.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

DEFINE_EVENT_TYPE( TOOLPANEL_CHANGED )
DEFINE_EVENT_TYPE( TOOLPANEL_CREATEDOC )

extern Config *config;

/*****************************************************
**
**   TOOLBAR DEFINITIONS
**
******************************************************/
const int tb_horawindow[] = { TBS_DATE, CMD_NOW, TBS_TZ, -1 };

const int tb_textwindow[] = { TBS_TEXTMODE, TBS_VARGA, TBS_DASA, -1 };

const int tb_ashtakavargawindow[] = { TBS_VARGA, -1 };

const int tb_eclipsewindow[] = { TBS_YEAR, TBS_NBYEARS, TBS_ECL_TYPE, TBS_TZ, -1 };

const int tb_printpreviewwindow[] = { TBS_PRINTOUTTYPE, TBS_SHEETSTYLE, TBS_VEDICCHARTSTYLE,
	TBS_WESTERNCHARTSTYLE, CMD_LAUNCH_VIEWER, -1 };

const int tb_transitwindow[] = { TBS_DATE, TBS_TIME, CMD_NOW, TBS_TZ, TBS_DATE_PROGRESS, TB_SEPARATOR,
	TBS_TRANSITMODE, -1 };

const int tb_partnerwindow[] = { TBS_PARTNER1, TB_SEPARATOR, TBS_PARTNER2, -1 };

const int tb_uranianwindow[] = { TBS_SORT, CMD_FILTER, TBS_ORBIS, TBS_GRADKREIS, TBS_YEAR, APP_CONFIGURE, -1 };

const int tb_uranianchartwindow[] = { TBS_URANIAN_CHART_REFOBJECT, TBS_ORBIS, -1 };

const int tb_gdasawindow[] = { CMD_GOTO_YEAR, CMD_NOW, CMD_SELECT_DASA, TB_SEPARATOR, TBC_DASA_ZOOM, CMD_ZOOM_ORIGINAL,
	CMD_ZOOM_IN, CMD_ZOOM_OUT, TB_SEPARATOR, TBS_ANTARDASALEVEL, TB_TEXT, -1 };

const int tb_ephemwindow[] = { TBS_YEAR, TBS_MONTH, CMD_NOW, TB_SEPARATOR, TBS_EPHEMMODE, TBS_TZ, TBS_DASA, TBS_EPHEMDEG, -1 };
const int tb_solarwindow[] = { TBS_YEAR, CMD_NOW, CMD_NEW_ANNUAL_CHART, TB_TEXT, -1 };

const int tb_dasawindow[] = { CMD_ENTRY_CHART, CMD_TOGGLE_SPLIT, TB_SEPARATOR, TBS_YOGASOURCEFILTER,-1 };

const int tb_yogawindow[] = { TBS_VARGA, TBS_YOGAGROUPFILTER, TBS_YOGASOURCEFILTER, TBS_YOGAFIRSTHOUSE, -1 };

const int tb_yogaeditor[] = { APP_NEW, APP_OPEN, CMD_SAVE, CMD_SAVEAS, CMD_CLOSE, -1 };

IMPLEMENT_SINGLETON( ToolbarFactory )

/*****************************************************
**
**   ToolbarFactory   ---    Constructor
**
******************************************************/
ToolbarFactory::ToolbarFactory()
{
	init();
}

/*****************************************************
**
**   ToolbarFactory   ---    createToolbar
**
******************************************************/
wxToolBar *ToolbarFactory::createToolbar( wxWindow *parent, const int &type )
{
	wxToolBar *toolbar = new wxToolBar( parent, -1, wxDefaultPosition, wxDefaultSize, getToolbarStyle() );
	const int bmpsize = ImageProvider::get()->getDefaultBitmapSize();
	toolbar->SetToolBitmapSize( wxSize( bmpsize, bmpsize ));

	const int *a = 0;
	switch ( type )
	{
	case VIEW_URANIAN:
		a = tb_uranianwindow;
		break;
	case VIEW_URANIAN_CHART:
		a = tb_uranianchartwindow;
		break;
	case VIEW_TRANSIT:
		a = tb_transitwindow;
		break;
	case VIEW_HORA:
		a = tb_horawindow;
		break;
	case VIEW_TEXT:
		a = tb_textwindow;
		break;
	case VIEW_ASHTAKAVARGA:
		a = tb_ashtakavargawindow;
		break;
	case VIEW_DASA_TREE:
		a = tb_dasawindow;
		break;
	case VIEW_DASA_COMPOSITE:
		a = tb_dasawindow;
		break;
	case VIEW_SOLAR:
		a = tb_solarwindow;
		break;
	case VIEW_YOGA:
		a = tb_yogawindow;
		break;
	case VIEW_YOGA_EDITOR:
		a = tb_yogaeditor;
		break;
	case VIEW_GRAPHICALDASA:
		a = tb_gdasawindow;
		break;
	case VIEW_PRINT:
		a = tb_printpreviewwindow;
		break;
	case VIEW_ECLIPSE:
		a = tb_eclipsewindow;
		break;
	case VIEW_EPHEM:
		a = tb_ephemwindow;
		break;
	case VIEW_PARTNER:
		a = tb_partnerwindow;
		break;
	}
	if ( a ) create( toolbar, a );
	else wxLogWarning( wxT ( "Warning: no toolbar configuration for id #%d found" ), type );
	toolbar->Realize();
	return toolbar;
}

/*****************************************************
**
**   ToolbarFactory   ---    create
**
******************************************************/
void ToolbarFactory::create( wxToolBar *toolbar, const int *a )
{
	const int *i = a; // a is int array
	while ( *i != -1 )
	{
		//printf( "ToolbarFactory::create I is %d\n", *i ); 
		if ( IS_TB_CHOICE_ITEM( *i )) createChoiceItem( toolbar, *i );
		else if ( IS_TB_TEXT_ITEM( *i )) createTextField( toolbar );
		else if ( IS_TB_COMBO_ITEM( *i )) createComboItem( toolbar, *i );
		else if ( *i == TB_SEPARATOR ) toolbar->AddSeparator();
		else createByCommandId( toolbar, *i );

		i++;
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    createByCommandId
**
******************************************************/
void ToolbarFactory::createByCommandId( wxToolBar *toolbar, const int &item )
{
	//printf( "ToolbarFactory::createByCommandId %d\n", item );

#ifndef _WX_V2_
	// tip is right aligned, but method only on wx3 available
	if ( item == CMD_TIP ) toolbar->AddStretchableSpace();
#endif

	const wxItemKind kind = ( item == CMD_TOGGLE_SPLIT ? wxITEM_CHECK : wxITEM_NORMAL );
	addTool( toolbar, item, kind );
}

/*****************************************************
**
**   ToolbarFactory   ---    createTextField
**
******************************************************/
void ToolbarFactory::createTextField( wxToolBar *toolbar )
{
	//printf( "ToolbarFactory::createTextField\n" );
	wxControl *control = 0;
	const int bmpsize = ImageProvider::get()->getDefaultBitmapSize();

	const int textHeight = bmpsize + toolbar->GetMargins().x;
	control = (wxControl*) new ToolbarLabel( toolbar, TB_TEXT, wxDefaultPosition, wxSize( 600, textHeight ));
	toolbar->AddControl( control );
}

/*****************************************************
**
**   ToolbarFactory   ---    createComboItem
**
******************************************************/
void ToolbarFactory::createComboItem( wxToolBar *toolbar, const int &item )
{
	//printf( "ToolbarFactory::createComboItem %d\n", item );

	const static wxString zoom_choices[] = { wxT("10"), wxT("25"), wxT("50"), wxT("100"),
		wxT("150"), wxT("200"), wxT("250"), wxT("300"), wxT("400"), wxT("500"), wxT( "1000" ), wxT( "3000" ) };
	wxComboBox *combo = 0;
	wxString label;
	
	if ( item == TBC_DASA_ZOOM )
	{
		//NumericValidator val( 10, 100000 );
		combo = new wxComboBox( toolbar, item, wxT( "100" ), wxDefaultPosition, wxSize( 120, -1 ), 12, zoom_choices,
			wxTE_PROCESS_ENTER );
		label = _( "Change Zoom (Ctrl+MouseWheel)" );
		//combo->SetValidator( MIntegerValidator( &model.population ));
	}
	assert( combo );
	addControl( toolbar, combo, label );
}

/*****************************************************
**
**   ToolbarFactory   ---    createChoiceItem
**
******************************************************/
void ToolbarFactory::createChoiceItem( wxToolBar *toolbar, const int &item )
{
	//printf( "ToolbarFactory::createChoiceItem %d\n", item );

	const static wxString eclipse_types[] = { _("All"), _("Solar"), _("Lunar") };

	const static wxString antardasa_choices[] = { wxT("Mahadasa"), wxT("Antardasa"), wxT("Pratyantardasa"),
		wxT( "Sookshmantardasa" ), wxT( "Pranadasa" ) };

	const static wxString ephem_mode_choices[] = { _("Longitudes"), _("Graphical"), _("Sunrise/Tithi"),
		_("Ingress"), _( "Lunar" ), _("KP") };

	const static wxString tz_choices[] = { wxT("UT"), _("Local") };

	const static wxString ephemdeg_choices[] = { wxT("360"), wxT("180"), wxT("90"), wxT("45"), wxT("30"),
		wxT("22:30"), wxT("13:20"), wxT("11:15"), wxT("5:07:30") };

	const static wxString first_house_choices[] = { _("Ascendant"), _("Moon"), _("Arudha Lagna"),
		_( "Aries" ), _( "Taurus" ), _( "Gemini" ), _( "Cancer" ), _( "Leo" ), _( "Virgo" ), _( "Libra" ),
		_( "Scorpio" ), _( "Sagittarius" ), _( "Capricorn" ), _( "Aquarius" ), _( "Pisces" ) };

	wxChoice *choice = 0;
	wxControl *control = 0;
	wxString label;
	wxString s;

	switch ( item )
	{
	case TBS_URANIAN_CHART_REFOBJECT:
		control = new UranianReferenceObjectChoice( toolbar, item );
		label = _( "Reference Object" );
	break;
	case TBS_TEXTMODE:
		control = new TextViewModeChoice( toolbar, item );
		label = _( "Mode" );
		break;
	case TBS_VARGA:
	{
		VargaConfigLoader *loader = VargaConfigLoader::get();

		wxChoice *c = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		for ( Varga i = V_FIRST; i <= V_LAST; i++ )
		{
			if ( loader->getVargaDivision(i) > 0 )
				s.Printf( wxT( "%s (D%d)" ), loader->getVargaName(i).c_str(), loader->getVargaDivision(i) );
			else s = loader->getVargaName(i);
			c->Append( s );
		}
		label = _( "Varga" );
		control = c;
	}
	break;
	case TBS_DASA:
	{
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		DasaTool *tool = DasaTool::get();
		for ( uint i = 0; i < MAX_DASAEXPERTS; i++ )
		{
			choice->Append( tool->getDasaName( (DasaId)i ));
		}
		label = _( "Dasa" );
	}
	break;
	case TBS_YEAR:
		control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize,
			wxSP_ARROW_KEYS | wxTAB_TRAVERSAL, -3000, 5000 );
		control->SetSize( 80, control->GetBestSize().GetY());
		label = _( "Year" );
		break;
	case TBS_PRINTOUTTYPE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->Append( PrintoutConfigLoader::get()->getNamesAsArray());
		label = _( "Printout Type" );
		break;
	case TBS_WESTERNCHARTSTYLE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->Append( WesternChartConfigLoader::get()->getNamesAsArray());
		label = _( "Western Style" );
		break;
	case TBS_VEDICCHARTSTYLE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->Append( VedicChartConfigLoader::get()->getNamesAsArray());
		label = _( "Vedic Style" );
		break;
	case TBS_SHEETSTYLE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->Append( SheetConfigLoader::get()->getNamesAsArray());
		label = _( "Sheet Style" );
		break;
	case TBS_NBYEARS:
		control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10);
		control->SetSize( 60, control->GetBestSize().GetY());
		label = _( "Years" );
		break;
	case TBS_ECL_TYPE:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 3, eclipse_types, 0);
		label = _( "Type" );
		break;
	case TBS_DATE_PROGRESS:
		control = new wxSlider( toolbar, item, 0, -7, 7, wxDefaultPosition, wxSize( 100, -1 ), wxSL_HORIZONTAL );
		control->SetToolTip( _( "Progress Transit Date" ));
		label = _( "Progress" );
		break;
	case TBS_DATE:
		control = new MDateSpin( toolbar, item );
		//control = new MSpinDate( toolbar, item, wxDefaultPosition, wxSize( 150, -1 )); 
		label = _( "Date" );
		break;
	case TBS_TIME:
		control = new MDegSpin( toolbar, item, 24 );
		label = _( "Time" );
		break;
	case TBS_TRANSITMODE:
		control = new TransitModeChoice( toolbar, item );
		label = _( "Mode" );
		break;
	case TBS_ORBIS:
		control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 300 );
		control->SetSize( 60, control->GetBestSize().GetY());
		label = _( "Orbis" );
		break;
	case TBS_GRADKREIS:
		control = new GradkreisChoice( toolbar, item );
		label = _( "Gradkreis" );
		break;
	case TBS_SORT:
		control = new SortChoice( toolbar, item );
		label = _( "Sort order" );
		break;
	case TBS_YOGAGROUPFILTER:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		control->SetToolTip( wxT( "Filter for Yoga Types" ));
		label = _( "Groups" );
		break;
	case TBS_YOGASOURCEFILTER:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		control->SetToolTip( wxT( "Filter for Yoga Sources" ));
		label = _( "Sources" );
		break;
	case TBS_YOGAFIRSTHOUSE:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 120, -1 ), 15, first_house_choices, wxFIXED_MINSIZE );
		label = _( "Ascendant" );
		break;
	case TBS_ANTARDASALEVEL:
	{
		wxChoice *c = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 5, antardasa_choices, 0);
		c->SetSelection( 1 );
		control = c;
		label = _( "Dasa Level (Shift+MouseWheel)" );
	}
	break;
	case TBS_MONTH:
		control =  new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize,
		                           wxSP_ARROW_KEYS|wxSP_WRAP|wxTE_PROCESS_ENTER, 1, 12 );
		control->SetSize( 50, control->GetBestSize().GetY());
		label = _( "Month" );
		break;
	case TBS_EPHEMMODE:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 120, -1 ), 6, ephem_mode_choices, 0);
		label = _( "Mode" );
		break;
	case TBS_TZ:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 2, tz_choices, 0);
		label = _( "Time Zone" );
		break;
	case TBS_EPHEMDEG:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 120, -1 ), 9, ephemdeg_choices, 0);
		label = _( "Circle Type" );
		break;
	case TBS_PARTNER1:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		label = _( "Partner 1" );
		break;
	case TBS_PARTNER2:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		label = _( "Partner 2" );
		break;
	default:
		assert( 0 );
		break;
	}
	assert( choice || control );
	if ( choice )
	{
		choice->SetSelection( 0 );
		addControl( toolbar, choice, label );
	}
	else
	{
		if ( item == TBS_PARTNER1 || item == TBS_PARTNER2 )
		{
			toolbar->AddControl( new wxStaticText( toolbar, -1, label ));
		}
		assert( control );
		addControl( toolbar, control, label );
	}
}

/*************************************************//**
*
* 
*
******************************************************/
static const struct ToolbarCommandId
{
	int cmdId;
	bool isDefault;
}
theIdList[] =
{
	{ APP_NEW, true },
	{ APP_OPEN, true },
	{ CMD_SAVE, true },
	{ CMD_SAVEAS, false },
	{ CMD_PRINT, true },
	{ CMD_QUICKPRINT, false },
	{ CMD_EDITDATA, true },
	{ 0, true },

	{ CMD_NEW_RASI, true },
	{ CMD_NEW_RASI+1, true },
	{ CMD_NEW_RASI+2, false },
	{ CMD_NEW_RASI+3, false },
	{ CMD_NEW_RASI+4, false },
	{ CMD_NEW_RASI+5, false },
	{ CMD_NEW_RASI+6, false },
	{ CMD_NEW_RASI+7, false },
	{ CMD_NEW_RASI+8, false },
	{ CMD_NEW_RASI+9, false },
	{ CMD_NEW_RASI+10, false },
	{ CMD_NEW_RASI+11, false },
	{ CMD_NEW_RASI+12, false },
	{ CMD_NEW_RASI+13, false },
	{ CMD_NEW_RASI+14, false },
	{ CMD_NEW_RASI+15, false },
	{ CMD_NEW_RASI+16, false },
	{ CMD_NEW_RASI+17, false },
	{ CMD_NEW_RASI+18, false },
	{ CMD_NEW_RASI+19, false },
	{ CMD_NEW_RASI+20, false },
	{ CMD_NEW_WCHART, true },
	{ CMD_NEW_SBC, true },

	{ CMD_NEW_VARGA, true },
	{ CMD_NEW_TEXT, true },
	{ CMD_NEW_DASA_COMPOSITE, true },
	{ CMD_NEW_GRAPHICALDASA, true },
	{ CMD_NEW_ASHTAKAVARGA, false },
	{ CMD_NEW_TRANSIT, true },
	{ CMD_NEW_YOGA, true },
	{ CMD_NEW_SOLAR, false },
	{ CMD_NEW_URANIAN, false },
	{ 0, true },

	{ APP_HORA, false },
	{ APP_PARTNER, true },
	{ APP_EPHEMERIS, true },
	{ APP_ECLIPSE, false },
	{ APP_ATLASDIALOG, true },
	{ APP_CONFIGURE, true },
	{ APP_HELP, false },
	{ APP_ABOUT, false }
};


/*****************************************************
**
**   ToolbarFactory   ---    init
**
******************************************************/
void ToolbarFactory::init()
{
	if ( config->toolbar->mainToolbarItems.size() == 0)
	{
		for ( size_t i = 0; i < WXSIZEOF( theIdList ); i++ )
		{
			if ( theIdList[i].isDefault )
			{
				config->toolbar->mainToolbarItems.push_back( theIdList[i].cmdId );
			}
		}
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    getDefaultCommandIdList
**
******************************************************/
vector<int> ToolbarFactory::getDefaultCommandIdList()
{
	vector<int> l;
	for ( size_t i = 0; i < WXSIZEOF( theIdList ); i++ )
	{
		if ( theIdList[i].isDefault )
		{
			l.push_back( theIdList[i].cmdId );
		}
	}
	return l;
}

/*****************************************************
**
**   ToolbarFactory   ---    getFullCommandIdList
**
******************************************************/
vector<int> ToolbarFactory::getFullCommandIdList()
{
	vector<int> l;
	for ( size_t i = 0; i < WXSIZEOF( theIdList ); i++ )
	{
		if ( theIdList[i].cmdId )
		{
			l.push_back( theIdList[i].cmdId );
		}
	}
	return l;
}

/*****************************************************
**
**   ToolbarFactory   ---    populateMainToolbar
**
******************************************************/
void ToolbarFactory::populateMainToolbar( wxToolBar *tb )
{
	assert( tb );
	//const int bmpsize = ImageProvider::get()->getToolBitmapSize();
	//printf( "bmpsize %d\n", bmpsize );

	tb->ClearTools();
	//tb->SetToolBitmapSize( wxSize( bmpsize, bmpsize ));
	tb->SetToolBitmapSize( wxSize( 24, 24 ));

	for( uint i = 0; i < config->toolbar->mainToolbarItems.size(); i++ )
	{
		if ( config->toolbar->mainToolbarItems[i] == APP_NEW )
		{

#ifdef _WX_V2_
			addTool( tb, config->toolbar->mainToolbarItems[i] );
#else
			addTool( tb, config->toolbar->mainToolbarItems[i], wxITEM_DROPDOWN );
			tb->SetDropdownMenu( APP_NEW, ContextMenuProvider().getToolbarNewMenu());
#endif

		}
		else if ( config->toolbar->mainToolbarItems[i] )
		{
			addTool( tb, config->toolbar->mainToolbarItems[i] );
		}
		else tb->AddSeparator();
	}
	tb->Realize();
}

/*****************************************************
**
**   ToolbarFactory   ---    getToolbarStyle
**
******************************************************/
int ToolbarFactory::getToolbarStyle()
{
	int style = wxTB_FLAT | wxTB_HORIZONTAL | wxTB_DOCKABLE;
	switch( config->toolbar->toolbarStyle )
	{
		case 0: // text
			style |= wxTB_TEXT;
			style |= wxTB_NOICONS;
		break;
		case 1: // icon
		break;
		case 2: // text and icon
		default:
			style |= wxTB_TEXT;
		break;
	}
	//printf( "Toolbar style is %d\n", style );
	return style;
}

/*****************************************************
**
**   ToolbarFactory   ---    addTool
**
******************************************************/
void ToolbarFactory::addTool( wxToolBar *tb, const int &id,  const wxItemKind kind )
{
	ImageProvider *ip = ImageProvider::get();
	CommandHandler *handler = CommandHandler::get();
	MCommand *command = handler->getCommand( id );

	if ( ! command )
	{
		wxLogError( wxT( "tool with id %d not found" ), id );
	}
	else
	{
		tb->AddTool( id, command->getShortText(), ip->getBitmap( command->getBitmapId() ), command->getText(), kind );
		/*
		if ( config->toolbar->toolbarIconSize )
		{
			// 32x32
			tb->AddTool( id, command->getShortText(), ip->getBitmap( command->getBitmapId() ), command->getText(), kind );
		}
		else if ( id )
		{
			// 24x24
			tb->AddTool( id, command->getShortText(), ip->getScaledBitmap( command->getBitmapId(), 24 ), command->getText(), kind );
		}
		*/
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    addControl
**
******************************************************/
void ToolbarFactory::addControl( wxToolBar *toolbar, wxControl *control, const wxString &label )
{
	control->SetToolTip( label );

#ifndef _WX_V2_
		toolbar->AddControl( control, label );
#else
		toolbar->AddControl( control );
#endif
	
}




