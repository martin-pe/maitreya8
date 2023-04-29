/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Commands.h
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


#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <wx/string.h>
#include <map>

#include "ImageProvider.h"
#include "maitreya.h"

class MCommand;
class wxKeyEvent;
class wxWindow;

/**************************************************************
***
**  Command and view IDs
***
****************************************************************/
enum { CMD_FIRST = wxID_HIGHEST + 1000,

	// status preferVedic, handled by all children
	CMD_VEDICMODE, CMD_WESTERNMODE,

	// western chart show
	CMD_WCS_ASPECTS, CMD_WCS_RETRO, CMD_WCS_HOUSES,
	CMD_WCS_ASPECTSYMBOLS, CMD_WCS_SECONDCHART_INSIDE,

	// western chart colors
	CMD_WCC_PLANETS, CMD_WCC_SIGNS, CMD_WCC_HOUSES, CMD_WCC_ASPECTS,

	// Status of vedic charts (graphic mode)
	CMD_VCS_RETRO, CMD_VCS_ARUDHAS, CMD_VCS_ASHTAKAVARGA,
	CMD_VCS_PLANETCOLORS, CMD_VCS_SANSKRITSYMBOLS,
	CMD_VCS_SBC_NAKSHATRA_QUALITY,

	// Vedic chart type
	CMD_VCT_SOUTH, CMD_VCT_NORTH, CMD_VCT_EAST,

	// Vedic chart center
	CMD_VCC_NOTHING, CMD_VCC_NAME, CMD_VCC_NAME_SIGNIFICATION, CMD_VCC_DIVISION, CMD_VCC_DIVISION_SIGNIFICATION,

	// Vedic chart North 
	CMD_VCN_ASC, CMD_VCN_NUMBER, CMD_VCN_SHORT, CMD_VCN_SYMBOL,

	 // 15x planetary objects (western and vedic): handled by western+vedic text+graphic
	 CMD_SHOWOUTER, CMD_SHOWDRAGONHEAD, CMD_SHOWDRAGONTAIL,
	 CMD_SHOWASCENDANT, CMD_SHOWMERIDIAN,
	 CMD_SHOWDESCENDANT, CMD_SHOWIMUMCOELI,
	 CMD_SHOWURANIAN_INNER, CMD_SHOWURANIAN_OUTER, CMD_SHOWCHIRON, CMD_SHOWPHOLUS, CMD_SHOWPLANETOIDS, CMD_SHOWLILITH,
	 CMD_SHOWUPAGRAHAS, CMD_SHOWKALAVELAS, CMD_SHOWSPECIALLAGNAS, CMD_SHOWARIES, CMD_SHOWD9LAGNA,
	 CMD_SHOWARABICPARTS,
	 CMD_SHOW_4HOUSES,
	 CMD_SHOW_HOUSES,

	 // 12 status flags for Vedic main window: handled by MainView -> TextViewBase with preferVedic = true
	 CMD_MAIN_SHOW_DECLINATION, CMD_MAIN_SHOW_90_DEGREE_LONGITUDE, CMD_MAIN_SHOW_45_DEGREE_LONGITUDE,
	 CMD_MAIN_SHOW_ANTISCIA, CMD_MAIN_SHOW_DIGNITY, CMD_MAIN_SHOW_NAVAMSA, CMD_MAIN_SHOW_LORD,
	 CMD_MAIN_SHOW_Nakshatra, CMD_MAIN_SHOW_PADA, CMD_MAIN_SHOW_KP, CMD_MAIN_SHOW_SHASTIAMSA,
	 CMD_MAIN_SHOW_DASAVARGA,
	 CMD_MAIN_SHOW_KARAKA, CMD_MAIN_SHOW_ASHTAKA,
	 CMD_MAIN_SHOW_UPA,
	 CMD_MAIN_SHOW_HOUSEPOS,
	 CMD_MAIN_SHOW_ELEMENT,
	 CMD_MAIN_SHOW_SIGNIFICATION,
	 CMD_MAIN_SHOW_SIGN_QUALITY,

	 // generic toolbar commands
	 CMD_ENTRY_CHART,
	 CMD_TOGGLE_SPLIT,
	 CMD_NOW,
	 CMD_SELECT_DASA,
	 CMD_GOTO_YEAR,
	 CMD_FILTER, CMD_CLEAR_FILTER,
	 CMD_LAUNCH_VIEWER,
	 CMD_NEW_ANNUAL_CHART,
	 CMD_TIP,
	 CMD_ZOOM_IN,
	 CMD_ZOOM_ORIGINAL,
	 CMD_ZOOM_OUT,

	 // handle by text widget
	 CMD_COPY, CMD_SELECT_ALL,

	 CMD_EXPORT_TEXT,
	 CMD_EXPORT_CSV,
	 CMD_EXPORT_HTML,
	 CMD_EXPORT_PDF,
	 CMD_EXPORT_IMAGE,
	 
	 // general commands
	CMD_KEY_ARROW_UP, CMD_KEY_ARROW_DOWN,
	CMD_KEY_LEFT, CMD_KEY_RIGHT, CMD_KEY_PAGE_DOWN, CMD_KEY_PAGE_UP,

	// delegated to application object
	CMD_CLOSE, CMD_CLOSE_DOC, CMD_EDITDATA, CMD_ANIMATE,
	CMD_SAVE, CMD_SAVEAS, CMD_PRINT, CMD_QUICKPRINT, CMD_NEW_ANNUAL,

	// creation of new child views -> delegate to application object
	CMD_NEW_TEXT, CMD_NEW_TRANSIT, CMD_NEW_SBC, CMD_NEW_SOLAR, CMD_NEW_YOGA, CMD_NEW_YOGAEDITOR,
	CMD_NEW_DASA_TREE, CMD_NEW_DASA_COMPOSITE, CMD_NEW_GRAPHICALDASA,
	CMD_NEW_GRAPHICGRID, CMD_NEW_VARGA, CMD_NEW_ASHTAKAVARGA, CMD_NEW_PRINTPREVIEW,
	CMD_NEW_VIMSOPAKABALA, CMD_NEW_WCHART, CMD_NEW_EMPTY,

	CMD_NEW_URANIAN, CMD_NEW_URANIAN_CHART,
	CMD_NEW_SHADBALA,

	CMD_NEW_MAINVIEW,

	// final commands
	CMD_NEW_RASI,
	CMD_GRAPHIC_STYLE = CMD_NEW_RASI + 30,

	// careful: this id was introduced after release, so do not put it in the middle
	// because the ids are stored for toolbar buttons
	CMD_EXPORT_PLAIN_HTML,
	CMD_LAST
};

enum {
	APP_CONFIGURE = wxID_PREFERENCES,
	APP_ABOUT = wxID_ABOUT,
	APP_EXIT = wxID_EXIT,
	APP_HELP = wxID_HELP,
	APP_OPEN = wxID_OPEN,
	//APP_NEW = wxID_NEW,

	APP_LIST_LARGEICONS = wxID_VIEW_LARGEICONS,
	APP_LIST_SMALLICONS = wxID_VIEW_SMALLICONS,
	APP_LIST_TABLE = wxID_VIEW_LIST,


/*
    xID_CLOSE,
    wxID_SAVE,
    wxID_SAVEAS,
*/

	APP_FIRST = wxID_HIGHEST + 2000,
	/*
	APP_FILE_NEW_SIMPLE,
	APP_FILE_NEW_MULTIPLE_DEFAULT,
	APP_FILE_NEW_MULTIPLE,
	*/

	APP_NEW,
	APP_NEW_SINGLE,
	APP_NEW_MULTIPLE_CHOICE,
	APP_NEW_MULTIPLE,

	APP_ATLASDIALOG = APP_NEW_MULTIPLE + 100,

	APP_HORA,
	APP_EPHEMERIS,
	APP_ECLIPSE,
	APP_PARTNER,
	APP_YOGAEDITOR,

	APP_TOGGLE_STATUSBAR,
	APP_TOGGLE_MAINTOOLBAR,
	APP_SAVECONFIG,

	APP_FILERECENT
};

#define IS_COMMAND_ID( id ) ( id > CMD_FIRST && id <= APP_FILERECENT + 30 )

/*************************************************//**
*
* \brief application will filter key events for this class and its subclasses
*
******************************************************/
class KeyEventObserver : public wxObject
{
	DECLARE_CLASS( KeyEventObserver )
};

/*************************************************//**
*
* \brief keyboard shortcut
*
******************************************************/
class MaitreyaShortcut
{
public:

	MaitreyaShortcut( const int &scKey, const int &scShift, const int &scCtrl );

	wxString toString() const;

	bool matches( const wxKeyEvent& ) const;

private:
	int scKey, scShift, scCtrl, scMeta, scAlt;
};

/*************************************************//**
*
* \brief encapsulation of a command, used for construction of menu and toolbar items
*
******************************************************/
class MCommand
{
public:
	MCommand( const BitmapId &bmpId, wxString text, const MaitreyaShortcut *shortcut = 0 )
			: bmpId( bmpId ),
			text( text ),
			shortcut( shortcut ) {}

	MCommand( const BitmapId &bmpId, wxString shortText, wxString text, const MaitreyaShortcut *shortcut = 0 )
			: bmpId( bmpId ),
			shortText( shortText ),
			text( text ),
			shortcut( shortcut ) {}

	~MCommand() { if ( shortcut ) delete shortcut; }

	wxString getMenuRepresentation();

	BitmapId getBitmapId() const { return bmpId; }

	const MaitreyaShortcut *getShortcut() const { return shortcut; }

	wxString getText() { return text; }

	wxString getShortText();

private:
	const BitmapId bmpId;
	wxString shortText, text;
	const MaitreyaShortcut *shortcut;
};

class NewDefaultCommand : public MCommand
{
public:
	NewDefaultCommand();
	virtual wxString getText();
};


/*************************************************//**
*
* \brief holds the list of commands and handles lookups for key events
*
******************************************************/
class CommandHandler
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( CommandHandler )

public:

	MCommand *getCommand( const int &id );

	bool processKeyEvent( wxKeyEvent*, wxWindow *window, const bool isTextWidget );

	const std::map<int, MCommand*> &getCommands() const { return cmds; }

protected:
	void init();

	std::map<int, MCommand*> cmds;

private:
	CommandHandler();
};

#endif

