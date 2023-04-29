/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Commands.cpp
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

#include "Commands.h"
#include "Conf.h"
#include "ImageProvider.h"
#include "MultipleViewConfig.h"
#include "VargaBase.h"

#include <wx/event.h>
#include <wx/window.h>

extern Config *config;

IMPLEMENT_CLASS( KeyEventObserver, wxObject )

/*****************************************************
**
**   MaitreyaShortcut   ---   Constructor
**
******************************************************/
MaitreyaShortcut::MaitreyaShortcut( const int &scKey, const int &scShift, const int &scCtrl )
{
	this->scKey = scKey;
	this->scShift = scShift;
	this->scCtrl = scCtrl;
	scMeta = scAlt = 0;

	/*
		-->> Not relecant any more because key events are caught on top level in app

	   MacOS workaround: simple shortcuts like '0' are caught in spin boxes, date input fields etc.
		 So these Shortcuts get additional 'Alt' modifier.

	  Relevant are all shortcuts without shift and ctrl
		letters (l, p ) are caught in the Yoga Editor text fields
	*/
/*
#if defined( __WXMAC__ )
	if ( scShift == 0 && scCtrl == 0 )
	{
		scAlt = 1;
	}
#endif
*/
}

/*****************************************************
**
**   MaitreyaShortcut   ---   toString
**
******************************************************/
wxString MaitreyaShortcut::toString() const
{
	wxString s;

	if ( scShift ) s << wxT( "Shift+" );
	if ( scCtrl ) s << wxT( "Ctrl+" );
	if ( scMeta ) s << wxT( "Meta+" );
	if ( scAlt ) s << wxT( "Alt+" );
	switch ( scKey )
	{
	case WXK_F1:
		s << wxT( "F1" );
		break;
	case WXK_UP:
		s << wxT( "Up" );
		break;
	case WXK_DOWN:
		s << wxT( "Down" );
		break;
	case WXK_ESCAPE:
		s << wxT( "Esc" );
		break;
	default:
		s << (wxChar)wxToupper( scKey );
		break;
	}
	return s;
}

/*****************************************************
**
**   MaitreyaShortcut   ---   matches
**
******************************************************/
bool MaitreyaShortcut::matches( const wxKeyEvent &event ) const
{
	return( event.GetKeyCode() == scKey
		&& event.m_shiftDown == (bool)scShift
		&& event.m_controlDown == (bool)scCtrl
		&& event.m_metaDown == (bool)scMeta
		&& event.m_altDown == (bool)scAlt
	);
}

/*****************************************************
**
**   MCommand   ---   getMenuRepresentation
**
******************************************************/
wxString MCommand::getMenuRepresentation()
{
	wxString s = getText();
	if ( shortcut != 0 ) s << wxT( "\t" ) << shortcut->toString();
	return s;
}

/*****************************************************
**
**   MCommand   ---   getShortText
**
******************************************************/
wxString MCommand::getShortText()
{
	if ( ! shortText.IsEmpty() ) return shortText;
	if ( ! text.IsEmpty() ) return text;
	return NONE;
}

/*****************************************************
**
**   NewDefaultCommand   ---   Constructor
**
******************************************************/
NewDefaultCommand::NewDefaultCommand()
	: MCommand( BM_NEW, _( "New" ), new MaitreyaShortcut( 'N', 0, 1 ))
{}

/*****************************************************
**
**   NewDefaultCommand   ---   getText
**
******************************************************/
wxString NewDefaultCommand::getText()
{
	return _( "New" );
}

IMPLEMENT_SINGLETON( CommandHandler )

/*****************************************************
**
**   CommandHandler   ---   Constructor
**
******************************************************/
CommandHandler::CommandHandler()
{
	init();
}

/*****************************************************
**
**   CommandHandler   ---   getCommand
**
******************************************************/
MCommand *CommandHandler::getCommand( const int &id )
{
	return cmds[id];
}

/*****************************************************
**
**   CommandHandler   ---   processKeyEvent
**
******************************************************/
bool CommandHandler::processKeyEvent( wxKeyEvent *event, wxWindow *window, const bool isTextWidget )
{
	int i = 0;
	//printf( "CommandHandler::processKeyEvent START\n" );
	if ( event->GetKeyCode() == WXK_ESCAPE ) i = CMD_CLOSE;
	if ( ! isTextWidget )
	{
		if ( event->GetKeyCode() == WXK_UP ) i = CMD_KEY_ARROW_UP;
		if ( event->GetKeyCode() == WXK_DOWN ) i = CMD_KEY_ARROW_DOWN;
		if ( event->GetKeyCode() == WXK_LEFT ) i = CMD_KEY_LEFT;
		if ( event->GetKeyCode() == WXK_RIGHT ) i = CMD_KEY_RIGHT;
		if ( event->GetKeyCode() == WXK_PAGEDOWN ) i = CMD_KEY_PAGE_DOWN;
		if ( event->GetKeyCode() == WXK_PAGEUP ) i = CMD_KEY_PAGE_UP;
	}

	for ( std::map<int, MCommand*>::iterator it = cmds.begin(); it != cmds.end(); it++ )
	{
		if ( it->second && it->second->getShortcut() && it->second->getShortcut()->matches( *event ))
		{
			i = it->first;
			break;
		}
	}

	if ( i )
	{
		//printf( "CommandHandler::processKeyEvent: POST\n" );
		wxMenuEvent evt( wxEVT_COMMAND_MENU_SELECTED, i );
		wxPostEvent( window, evt );
	}
	return i != 0;
}

/*****************************************************
**
**   CommandHandler   ---   init
**
******************************************************/
void CommandHandler::init()
{
	VargaConfigLoader *loader = VargaConfigLoader::get();
	int key, keymod, division;
	wxString shortName, longName;

	// generate Varga shortcuts
	for( Varga v = V_FIRST; v <= V_LAST; v++ )
	{
		division = loader->getVargaDivision( v );
		key = ( v + 1 ) % 10;
		keymod = (int)( ( v + 1 ) /10);
		//printf( "VARGA %d key %d keymod %d\n", (int)v, key, keymod );

		if ( v != V_BHAVA )
		{
			shortName = wxString::Format( wxT( "D-%d" ), division );
			longName = wxString::Format( wxT( "%s (D-%d)" ), loader->getVargaName( v ).c_str(), division );
		}
		else
		{
			shortName = _( "Bhava" );
			longName = _( "Bhava" );
		}

		cmds[CMD_NEW_RASI + (int)v] = new MCommand( (BitmapId)(BM_CHART + (int)v),
			shortName, longName,
			new MaitreyaShortcut( '0' + key, keymod ? 1 : 0, keymod == 2 ? 1 : 0 ));
	}

	// App File
	cmds[APP_NEW] = new NewDefaultCommand();
	cmds[APP_NEW_SINGLE] = new MCommand( BM_SVIEW, _( "New" ), _( "New Single View" ), new MaitreyaShortcut( 'N', 1, 1 ));
	//cmds[APP_NEW_SINGLE] = new MCommand( BM_SVIEW, _( "New Single View" ));
	cmds[APP_NEW_MULTIPLE_CHOICE] = new MCommand( BM_MVIEW, _( "Multiple Child Choice" ), new MaitreyaShortcut( 'M', 1, 1 ));
	cmds[APP_NEW_MULTIPLE] = new MCommand( BM_MVIEW, _( "Multiple Child" ), new MaitreyaShortcut( 'M', 0, 1 ));
	cmds[APP_OPEN] = new MCommand( BM_OPEN, _( "Open" ), new MaitreyaShortcut( 'O', 0, 1 ));
	cmds[APP_EXIT] = new MCommand( BM_QUIT, _( "Exit" ), new MaitreyaShortcut( 'Q', 0, 1 ));

	cmds[CMD_CLOSE] = new MCommand( BM_CLOSE, _( "Close" ), _( "Close Window" ), new MaitreyaShortcut( 'W', 0, 1 ));
	cmds[CMD_CLOSE_DOC] = new MCommand( BM_CLOSE, _( "Close File" ), new MaitreyaShortcut( 'W', 1, 1 ));
	cmds[CMD_SAVE] = new MCommand( BM_SAVE, _( "Save" ), new MaitreyaShortcut( 'S', 0, 1 ));
	cmds[CMD_SAVEAS] = new MCommand( BM_SAVEAS, _( "Save As" ), new MaitreyaShortcut( 'A', 0, 1 ));
	cmds[CMD_PRINT] = new MCommand( BM_PRINT, _( "PDF" ), _( "PDF Chart" ), new MaitreyaShortcut( 'P', 0, 1 ));
	cmds[CMD_QUICKPRINT] = new MCommand( BM_QUICKPRINT, _( "PDF" ), _( "Quick PDF Chart" ), new MaitreyaShortcut( 'P', 1, 1 ));

	// child new view
	cmds[CMD_NEW_TEXT] = new MCommand( BM_TEXT, _( "Text" ), _( "TextView" ), new MaitreyaShortcut( 'T', 0, 0 ));
	cmds[CMD_NEW_TRANSIT] = new MCommand( BM_TRANSIT, _( "Transit" ), new MaitreyaShortcut( 'R', 0, 0 ));
	cmds[CMD_NEW_YOGA] = new MCommand( BM_YOGA, _( "Yoga" ), new MaitreyaShortcut( 'Y', 0, 0 ));
	cmds[CMD_NEW_SBC] = new MCommand( BM_SBC, _( "SBC" ), _( "Sarvatobhadra" ), new MaitreyaShortcut( 'S', 0, 0 ));
	cmds[CMD_NEW_SOLAR] = new MCommand( BM_SOLAR, _( "Solar" ), new MaitreyaShortcut( 'T', 0, 1 ));
	cmds[CMD_NEW_VARGA] = new MCommand( BM_VARGA, _( "Varga" ), _( "Varga Summary" ), new MaitreyaShortcut( 'V', 0, 1 ));

#ifdef USE_PRINT_VIEW
	cmds[CMD_NEW_PRINTPREVIEW] = new MCommand( BM_PRINT, _( "Print Preview" ), new MaitreyaShortcut( 'X', 0, 0 ));
#endif

	cmds[CMD_NEW_SHADBALA] = new MCommand( BM_SHADBALA, _( "Shadbala" ), new MaitreyaShortcut( 'B', 0, 1 ));
	cmds[CMD_NEW_WCHART] = new MCommand( BM_WCHART, _( "WChart" ), _( "Western Chart" ), new MaitreyaShortcut( '0', 0, 0 ));
	cmds[CMD_NEW_URANIAN] = new MCommand( BM_URANIAN, _( "Uranian" ), new MaitreyaShortcut( 'U', 0, 0 ));

#ifdef USE_URANIAN_CHART
	cmds[CMD_NEW_URANIAN_CHART] = new MCommand( BM_WCHART, _( "Uranian Chart" ), new MaitreyaShortcut( 'U', 0, 1 ));
#endif

	cmds[CMD_NEW_ASHTAKAVARGA] = new MCommand( BM_ASHTAKAVARGA, _( "AV" ), _( "Ashtakavarga" ), new MaitreyaShortcut( 'A', 0, 0 ));
	cmds[CMD_NEW_DASA_TREE] = new MCommand( BM_DASA, _( "T-Dasa" ), _( "Dasa Tree" ), new MaitreyaShortcut( 'D', 0, 0 ));
	cmds[CMD_NEW_DASA_COMPOSITE] = new MCommand( BM_DASA, _( "T-Dasa" ), _( "Dasa Tree Composite" ), new MaitreyaShortcut( 'D', 0, 1 ));
	cmds[CMD_NEW_GRAPHICALDASA] = new MCommand( BM_GRAPHICALDASA, _( "G-Dasa" ), _( "Graphical Dasa" ), new MaitreyaShortcut( 'G', 0, 0 ));

	// App New View
	cmds[APP_HORA] = new MCommand( BM_HORA, _( "Hora" ), new MaitreyaShortcut( 'H', 0, 0 ));
	cmds[APP_ECLIPSE] = new MCommand( BM_ECLIPSE, _( "Eclipse" ), new MaitreyaShortcut( 'E', 0, 1 ));
	cmds[APP_PARTNER] = new MCommand( BM_PARTNER, _( "Partner" ), new MaitreyaShortcut( 'P', 0, 0 ));
	cmds[APP_EPHEMERIS] = new MCommand( BM_EPHEM, _( "Ephem" ), _( "Ephemeris" ), new MaitreyaShortcut( 'E', 0, 0 ));

// configuration on OXs has shortcut "cmd+,". This was mapped automatically on wx2.8 but is missing in wx3
#if defined(__WXMAC__)
	cmds[APP_CONFIGURE] = new MCommand( BM_CONFIG, _( "Config" ), _( "Configuration" ), new MaitreyaShortcut( ',', 0, 1 ));
#else
	cmds[APP_CONFIGURE] = new MCommand( BM_CONFIG, _( "Config" ), _( "Configuration" ), new MaitreyaShortcut( 'C', 0, 0 ));
#endif
	cmds[APP_YOGAEDITOR] = new MCommand( BM_YOGAEDITOR, _( "Yoga Editor" ), new MaitreyaShortcut( 'Y', 0, 1 ));
	cmds[APP_ATLASDIALOG] = new MCommand( BM_ATLAS, _( "Atlas" ), new MaitreyaShortcut( 'L', 0, 0 ));
	cmds[APP_ABOUT] = new MCommand( BM_ABOUT, _( "About" ), new MaitreyaShortcut( WXK_F1, 1, 0 ));
	cmds[APP_HELP] = new MCommand( BM_HELP, _( "Doc" ), _( "Documentation (Web)" ), new MaitreyaShortcut( WXK_F1, 0, 0 ));

	// child varia
	cmds[CMD_ANIMATE] = new MCommand( BM_NULL, _( "Animate Chart" ), new MaitreyaShortcut( 'A', 1, 1 ));
	cmds[CMD_VEDICMODE] = new MCommand( BM_NULL, _( "Vedic Mode" ), new MaitreyaShortcut( 'V', 0, 0 ));
	cmds[CMD_WESTERNMODE] = new MCommand( BM_NULL, _( "Western Mode" ), new MaitreyaShortcut( 'W', 0, 0 ));
	cmds[CMD_EDITDATA] = new MCommand( BM_EDIT, _( "Edit" ), _( "Edit Horoscope Data" ), new MaitreyaShortcut( 'B', 0, 0 ));

	cmds[ CMD_EXPORT_TEXT ] = new MCommand( BM_EXPORT_TEXT, _( "Plain Text" ));
	cmds[ CMD_EXPORT_CSV ] = new MCommand( BM_EXPORT_CSV, _( "CSV Text" ));
	cmds[ CMD_EXPORT_HTML ] = new MCommand( BM_EXPORT_HTML, _( "HTML" ));
	cmds[ CMD_EXPORT_PLAIN_HTML ] = new MCommand( BM_EXPORT_HTML, _( "Plain HTML" ));
	cmds[ CMD_EXPORT_PDF ] = new MCommand( BM_EXPORT_PDF, _( "PDF" ));
	cmds[ CMD_EXPORT_IMAGE ] = new MCommand( BM_EXPORT_IMAGE, _( "Image" ));

	cmds[ CMD_ENTRY_CHART ] = new MCommand( BM_NEW, _( "Entry" ), _( "Entry Chart" ));
	cmds[ CMD_TOGGLE_SPLIT ] = new MCommand( BM_SPLIT, _( "Split" ), _( "Split/Unsplit" ));
	cmds[ CMD_NOW ] = new MCommand( BM_NOW, _( "Now" ));
	cmds[ CMD_SELECT_DASA ] = new MCommand( BM_DASA, _( "Dasa" ), _( "Select Dasa" ));
	cmds[ CMD_GOTO_YEAR ] = new MCommand( BM_GOTO, _( "Goto" ), _( "Goto Year" ));
	cmds[ CMD_FILTER ] = new MCommand( BM_FILTER, _( "Filter" ));
	cmds[ CMD_CLEAR_FILTER ] = new MCommand( BM_CLOSE, _( "Clear Filter" ));
	cmds[ CMD_LAUNCH_VIEWER ] = new MCommand( BM_PRINT, _( "Launch" ), _( "Lanuch Viewer" ));
	cmds[ CMD_NEW_ANNUAL_CHART ] = new MCommand( BM_NEW, _( "Annual" ), _( "Annual Chart" ));
	cmds[ CMD_TIP ] = new MCommand( BM_TIP, _( "Tip" ));
	cmds[ CMD_ZOOM_IN ] = new MCommand( BM_ZOOM_IN, _( "Zoom In" ));
	cmds[ CMD_ZOOM_ORIGINAL ] = new MCommand( BM_ZOOM_ORIGINAL, _( "100%" ));
	cmds[ CMD_ZOOM_OUT ] = new MCommand( BM_ZOOM_OUT, _( "Zoom Out" ));

	// general commands
	cmds[CMD_KEY_ARROW_UP] = new MCommand( BM_NULL, _( "Up" ), new MaitreyaShortcut( WXK_UP, 0, 1 ));
	cmds[CMD_KEY_ARROW_DOWN] = new MCommand( BM_NULL, _( "Down" ), new MaitreyaShortcut( WXK_DOWN, 0, 1 ));

	// App Varia
	cmds[APP_SAVECONFIG] = new MCommand( BM_SAVE, _( "Save Configuration" ));
	cmds[APP_TOGGLE_MAINTOOLBAR] = new MCommand( BM_NULL, _( "Show Toolbar" ));
	cmds[APP_TOGGLE_STATUSBAR] = new MCommand( BM_NULL, _( "Show Statusbar" ));
}

