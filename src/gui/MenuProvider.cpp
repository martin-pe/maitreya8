/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/MenuProvider.cpp
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

#include "MenuProvider.h"

#include "BasicView.h"
#include "BasicWidget.h"
#include "Conf.h"
#include "Commands.h"
#include "ChildWindow.h"
#include "Document.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "MultipleViewConfig.h"
#include "Varga.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

extern Config *config;

IMPLEMENT_CLASS( MyMenu, wxMenu )

/**************************************************************
***
**    MyMenu   ---   Constructor
***
***************************************************************/
MyMenu::MyMenu( const wxString title )
	: wxMenu( title )
{
	handler = CommandHandler::get();
}

/**************************************************************
***
**    MyMenu   ---   addItem
***
***************************************************************/
wxMenuItem *MyMenu::addItem( const int &id )
{
	wxMenuItem *item = (wxMenuItem*)NULL;
	MCommand *command = handler->getCommand( id );
	if ( command )
	{
		item = new wxMenuItem( this, id, command->getMenuRepresentation());
		if ( command->getBitmapId() != BM_NULL ) item->SetBitmap( ImageProvider::get()->getMenuBitmap( command->getBitmapId() ));
		Append( item );
	}
	else assert( 0 );
	return item;
}

/**************************************************************
***
**    MyMenu   ---   addCheckItem
***
***************************************************************/
void MyMenu::addCheckItem( const int &id, const bool checked )
{
	MCommand *command = handler->getCommand( id );
	if ( command )
	{
		AppendCheckItem( id, command->getMenuRepresentation());
		Check( id, checked );
	}
	else assert( 0 );
}

/**************************************************************
***
**    MyMenu   ---   addCheckItem
***
***************************************************************/
void MyMenu::addCheckItem( const int &id, const wxString title, const bool checked )
{
	AppendCheckItem( id, title );
	Check( id, checked );
}

/**************************************************************
***
**    MyMenu   ---   addArray
***
***************************************************************/
void MyMenu::addArray( const int a[] )
{
	int i = 0;

	while ( a[i] != -1 )
	{
		if ( a[i] ) addItem( a[i] );
		else AppendSeparator();
		i++;
	}
}

/**************************************************************
***
**    MyMenu   ---   addWindowListMenu
***
***************************************************************/
void MyMenu::addWindowListMenu()
{
	wxMenu *wlmenu = new wxMenu();
	wlmenu->AppendRadioItem( APP_LIST_LARGEICONS, _( "Big Icons" ) );
	wlmenu->AppendRadioItem( APP_LIST_SMALLICONS, _( "Small Icons" ) );
	wlmenu->AppendRadioItem( APP_LIST_TABLE, _( "Table" ) );
	switch ( config->viewprefs->listStyle )
	{
	case 1:
		wlmenu->Check( APP_LIST_SMALLICONS, true );
		break;
	case 2:
		wlmenu->Check( APP_LIST_TABLE, true );
		break;
	default:
		wlmenu->Check( APP_LIST_LARGEICONS, true );
		break;
	}
	Append( -1, _("Window List"), wlmenu );
}

/**************************************************************
***
**    MyMenu   ---   addVargaMenu
***
***************************************************************/
void MyMenu::addVargaMenu()
{
	MyMenu *menu = new MyMenu( wxEmptyString ); //, view );
	for ( int i = 0; i < NB_VARGAS; i++ ) menu->addItem( CMD_NEW_RASI + i );
	Append( -1, _( "Varga"), menu );
}

/*****************************************************
**
**   AppMenuBar   ---   Constructor
**
******************************************************/
AppMenuBar::AppMenuBar( int style )
		: wxMenuBar( style )
{
	const static int fileitems[] = { 0, CMD_SAVE, CMD_SAVEAS, CMD_CLOSE_DOC, 0, APP_EXIT, -1 };
	const static int vedicitems[] = { CMD_NEW_VARGA, CMD_NEW_SBC, CMD_NEW_TEXT, 0,
		CMD_NEW_DASA_TREE, CMD_NEW_DASA_COMPOSITE,
		CMD_NEW_GRAPHICALDASA, CMD_NEW_TRANSIT, 0, CMD_NEW_YOGA,
		CMD_NEW_ASHTAKAVARGA, CMD_NEW_SHADBALA, CMD_NEW_SOLAR, -1 };

#ifdef USE_URANIAN_CHART
	const static int westernitems[9] = { CMD_NEW_WCHART, CMD_NEW_TEXT, 0, CMD_NEW_URANIAN, CMD_NEW_URANIAN_CHART,
		CMD_NEW_TRANSIT, 0, CMD_NEW_SOLAR, -1 };
#else
	const static int westernitems[8] = { CMD_NEW_WCHART, CMD_NEW_TEXT, 0, CMD_NEW_URANIAN, 
		CMD_NEW_TRANSIT, 0, CMD_NEW_SOLAR, -1 };
#endif

	filemenu = new MyMenu;
	newDefaultItem = filemenu->addItem( APP_NEW );

	newmenu = new MyMenu;
	newmenu->addItem( APP_NEW_SINGLE );
	newmenu->AppendSeparator();
	vector<MultipleViewConfig*> &v = MultipleViewConfigLoader::get()->getConfigs();

	for ( uint i = 0; i < v.size(); i++ ) newmenu->Append( APP_NEW_MULTIPLE + i, v[i]->name );
	filemenu->Append( -1, _( "New" ), newmenu );

	filemenu->addItem( APP_OPEN );

	recentmenu = new MyMenu;
	for( uint i = 0; i < config->viewprefs->recentFiles.size(); i++ )
  {
		recentfiles.Add( config->viewprefs->recentFiles[i] );
	}
	for ( size_t n = 0; n < recentfiles.GetCount(); n++ ) recentmenu->Append( APP_FILERECENT + n, recentfiles[n] );
	filemenu->Append( -1, _("Open Recent"), recentmenu );

	filemenu->addArray( fileitems );
	Append( filemenu, _( "&File" ));

	vedicmenu = new MyMenu;
	vedicmenu->addVargaMenu();
	vedicmenu->addArray( vedicitems );
	Append( vedicmenu, _( "&Vedic"));

	westernmenu = new MyMenu;
	westernmenu->addArray( westernitems );
	Append( westernmenu, _( "&Western"));

#ifdef USE_PRINT_VIEW
	const static int windowitems[] = { 0, CMD_NEW_PRINTPREVIEW, CMD_PRINT, CMD_QUICKPRINT, 0, CMD_CLOSE, -1 };
#else
	const static int windowitems[] = { 0, CMD_PRINT, CMD_QUICKPRINT, 0, CMD_CLOSE, -1 };
#endif

	windowmenu = new MyMenu;
	windowmenu->addItem( CMD_EDITDATA );
	windowmenu->addCheckItem( CMD_ANIMATE );
	windowmenu->AppendSeparator();

	windowmenu->addCheckItem( CMD_VEDICMODE );
	windowmenu->addCheckItem( CMD_WESTERNMODE );

	windowmenu->addArray( windowitems );
	Append( windowmenu, _( "&Window"));

	const static int extraitems[11] = { APP_EPHEMERIS, APP_ECLIPSE, APP_HORA, APP_PARTNER, 0,
		APP_CONFIGURE, APP_ATLASDIALOG, APP_YOGAEDITOR, APP_SAVECONFIG, 0, -1 };
	extramenu = new MyMenu;
	extramenu->addArray( extraitems );
	extramenu->addWindowListMenu();
	extramenu->addCheckItem( APP_TOGGLE_MAINTOOLBAR, config->viewprefs->showMainToolbar );
	extramenu->addCheckItem( APP_TOGGLE_STATUSBAR, config->viewprefs->showStatusBar );
	Append( extramenu, _( "&Extras"));

	helpmenu = new MyMenu;
	helpmenu->addItem( APP_HELP );
	helpmenu->addItem( APP_ABOUT );
	Append( helpmenu, _( "&Help"));
}

/*****************************************************
**
**   AppMenuBar   ---   updateMenus
**
******************************************************/
void AppMenuBar::updateMenus( ChildWindow *child )
{
	bool isDoc = false;
	bool isAnimated = false;
	bool hasEwOption = false;
	bool isVedic = false;

	if ( child )
	{
		if ( child->getDoc() ) isDoc = true;
		if ( isDoc && child->getDoc()->isAnimated() ) isAnimated = true;

		ChartProperties *props = child->getProps();
		if ( props )
		{
			if ( ! props->isFixedVedic() && ! props->isFixedWestern()) hasEwOption = true;
			isVedic = props->isVedic();
		}
	}

	MCommand *command = CommandHandler::get()->getCommand( APP_NEW );
	newDefaultItem->SetItemLabel( command->getMenuRepresentation());

	filemenu->Enable( APP_FILERECENT, recentfiles.GetCount() > 0  );
	filemenu->Enable( CMD_CLOSE_DOC, isDoc );
	filemenu->Enable( CMD_SAVE, isDoc );
	filemenu->Enable( CMD_SAVEAS, isDoc );

	// Vedic and western menus
	EnableTop( 1, isDoc );
	EnableTop( 2, isDoc );

	windowmenu->Enable( CMD_WESTERNMODE, hasEwOption );
	windowmenu->Check( CMD_WESTERNMODE, ! isVedic );
	windowmenu->Enable( CMD_VEDICMODE, hasEwOption );
	windowmenu->Check( CMD_VEDICMODE, isVedic );

#ifdef USE_PRINT_VIEW
	windowmenu->Enable( CMD_NEW_PRINTPREVIEW, isDoc );
#endif

	windowmenu->Enable( CMD_PRINT, isDoc );
	windowmenu->Enable( CMD_QUICKPRINT, isDoc );

	windowmenu->Enable( CMD_EDITDATA, isDoc );
	windowmenu->Check( CMD_ANIMATE, isAnimated );

	windowmenu->Enable( CMD_ANIMATE, isDoc );
	EnableTop( 3, child );
}

/*****************************************************
**
**   AppMenuBar   ---   addToRecentFiles
**
******************************************************/
void AppMenuBar::addToRecentFiles( const wxString &filename )
{
	uint maxrecent = config->view->maxRecentFiles;
	uint recentcount = recentmenu->GetMenuItemCount();
	size_t n;

	// clear recent file menu
	for ( n = 0; n < recentcount; n++ ) recentmenu->Delete( APP_FILERECENT + n );

	// Clear obsolete items ( if number of files has changed via ConfigDialog)
	if ( recentfiles.GetCount() > maxrecent )
	{
		for ( n = 0; n < recentfiles.GetCount() - maxrecent; n++ )
			recentfiles.RemoveAt( maxrecent + n );
	}

	// update file list
	for ( n = 0; n < recentfiles.GetCount(); n++ )
	{
		if ( recentfiles[n] == filename )
		{
			recentfiles.RemoveAt( n );
			break;
		}
	}
	if ( recentfiles.GetCount() == (uint)config->view->maxRecentFiles ) recentfiles.RemoveAt( recentfiles.GetCount() - 1 );
	recentfiles.Insert( filename, 0 );
	for ( n = 0; n < recentfiles.GetCount(); n++ ) recentmenu->Append( APP_FILERECENT + n, recentfiles[n] );
}

/*****************************************************
**
**   AppMenuBar   ---   getRecentFilesEntry
**
******************************************************/
wxString AppMenuBar::getRecentFilesEntry( const int &commandId )
{
	const int fileindex = commandId - APP_FILERECENT;
	assert( fileindex >= 0 && fileindex < (int)recentfiles.GetCount());
	return recentfiles[ fileindex ];
}

/*****************************************************
**
**   AppMenuBar   ---   saveRecentFiles
**
******************************************************/
void AppMenuBar::saveRecentFiles()
{
	config->viewprefs->recentFiles.clear();
	for ( size_t n = 0; n < recentfiles.GetCount(); n++ )
	{
		config->viewprefs->recentFiles.push_back( recentfiles[n] );
	}
}

/**************************************************************
***
**   ContextMenuProvider   ---   getChildWindowListCtrlMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getChildWindowListCtrlMenu( ChildWindow *child )
{
	assert( child );
	ChartProperties *props = child->getProps();

	MyMenu *menu = new MyMenu( child->GetTitle());
	menu->AppendSubMenu( getNewViewMenu( props ), _( "New View"));
	menu->AppendSeparator();

	if ( child->getDoc())
	{
		menu->addItem( CMD_EDITDATA );
		menu->addCheckItem( CMD_ANIMATE, props->isAnimated() );
		menu->AppendSeparator();
	}
	menu->addItem( CMD_CLOSE );

	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getWidgetMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getWidgetMenu( BasicWidget *widget )
{
	assert( widget );
	return getWidgetMenu( widget->getChartProperties(), widget->getWidgetOptions(), _( "Context Menu" ));
}

/**************************************************************
***
**   ContextMenuProvider   ---   getWidgetMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getWidgetMenu( ChartProperties *props, const int &wo, wxString title )
{
	assert( props );
	//printf( "WO %d & WO_SUPPORTS_EW_TOGGLE %d\n", wo, wo & WO_SUPPORTS_EW_TOGGLE );

	MyMenu *menu = new MyMenu( title );

	menu->AppendSubMenu( getNewViewMenu( props ), _( "New View"));
	menu->AppendSeparator();

	if ( props->hasDocument() )
	{
		menu->addItem( CMD_EDITDATA );
		menu->addCheckItem( CMD_ANIMATE, props->isAnimated() );
		menu->AppendSeparator();
	}

	if ( ! props->isEwFixed() && ( wo & WO_SUPPORTS_EW_TOGGLE ))
	{
		menu->addCheckItem( CMD_VEDICMODE, props->isVedic());
		menu->addCheckItem( CMD_WESTERNMODE, ! props->isVedic());
		menu->AppendSeparator();
	}

	if (( wo & WO_MENU_FULL_OBJECT ) | ( wo & WO_MENU_RESTRICTED_OBJECT ))
	{
		menu->AppendSubMenu( getObjectSubMenu( wo, props ), _( "Objects"));
	}
	if (( wo & WO_MENU_FULL_GRAPHIC_STYLE ) | ( wo & WO_MENU_RESTRICTED_GRAPHIC_STYLE ))
	{
		menu->AppendSubMenu( getGraphicStyleMenu( wo, props ), _( "Graphic Options"));
	}
	if ( wo & WO_MENU_SBC_STYLE )
	{
		menu->AppendSubMenu( getSarvatobhadraMenu( props ), _( "Sarvatobhadra"));
	}
	if ( wo & WO_MENU_SKIN )
	{
		menu->AppendSubMenu( getSkinMenu( props ), _( "Skin" ));
	}
	if ( wo & WO_IS_MAIN_VIEW )
	{
		menu->AppendSubMenu( getMainViewColumnMenu( props ), _( "Columns"));
	}

	if (
		( wo & WO_EXPORT_PLAINTEXT )
		| ( wo & WO_EXPORT_CSVTEXT )
		| ( wo & WO_EXPORT_HTMLTEXT )
		| ( wo & WO_EXPORT_PDF )
		| ( wo & WO_EXPORT_GRAPHIC ))
	{
		menu->AppendSubMenu( getExportMenu( wo ), _( "Export As ..."));
	}

	menu->addItem( CMD_CLOSE );
	return menu;
}

/**************************************************************
***
**    ContextMenuProvider   ---   getExportMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getExportMenu( const int &wo )
{
	MyMenu *exportmenu = new MyMenu( _( "Export As" ));
	if ( wo & WO_EXPORT_PLAINTEXT ) exportmenu->addItem( CMD_EXPORT_TEXT );
	if ( wo & WO_EXPORT_CSVTEXT ) exportmenu->addItem( CMD_EXPORT_CSV );
	if ( wo & WO_EXPORT_HTMLTEXT )
	{
		exportmenu->addItem( CMD_EXPORT_HTML );
		exportmenu->addItem( CMD_EXPORT_PLAIN_HTML );
	}

#ifdef USE_PDF_EXPORT
	if ( wo & WO_EXPORT_PDF ) exportmenu->addItem( CMD_EXPORT_PDF );
#endif

	if ( wo & WO_EXPORT_GRAPHIC ) exportmenu->addItem( CMD_EXPORT_IMAGE );
	return exportmenu;
}

/**************************************************************
***
**    ContextMenuProvider   ---   getSkinMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getSkinMenu( ChartProperties *props )
{
	uint i = 0;
	MyMenu *menu = new MyMenu( _( "Skin" ));

	if ( props->isVedic())
	{
		vector<VedicChartConfig*> list =  VedicChartConfigLoader::get()->getConfigs();
		for ( unsigned i = 0; i < list.size(); i++ )
		{
			menu->addCheckItem( CMD_GRAPHIC_STYLE + i, list[i]->name, i == (uint)props->getSkin());
		}
	}
	else
	{
		vector<WesternChartConfig*> list =  WesternChartConfigLoader::get()->getConfigs();
		for ( i = 0; i < list.size(); i++ )
		{
			menu->addCheckItem( CMD_GRAPHIC_STYLE + i, list[i]->name, i == (uint)props->getSkin());
		}
	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getObjectSubMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getObjectSubMenu( const int &wo, ChartProperties *props )
{
	Lang lang;
	const TEXT_FORMAT format = TF_LONG;

	MyMenu *menu = new MyMenu( _( "Objects" ));

	menu->addCheckItem( CMD_SHOWOUTER, _( "3 Outer Planets"), props->getObjectStyle() & OI_OUTER );

	menu->addCheckItem( CMD_SHOWDRAGONHEAD, lang.getObjectName( OMEANNODE, format, props->isVedic() ),
		props->getObjectStyle() & OI_DRAGONHEAD );
	menu->addCheckItem( CMD_SHOWDRAGONTAIL, lang.getObjectName( OMEANDESCNODE, format, props->isVedic() ),
		props->getObjectStyle() & OI_DRAGONTAIL );

	if ( wo & WO_MENU_FULL_OBJECT )
	{
		menu->addCheckItem( CMD_SHOWASCENDANT, lang.getObjectName( OASCENDANT, format, props->isVedic() ),
			props->getObjectStyle() & OI_ASCENDANT );
		menu->addCheckItem( CMD_SHOWMERIDIAN, lang.getObjectName( OMERIDIAN, format, props->isVedic() ),
			props->getObjectStyle() & OI_MERIDIAN );
		menu->addCheckItem( CMD_SHOWDESCENDANT, lang.getObjectName( ODESCENDANT, format, props->isVedic() ),
			props->getObjectStyle() & OI_DESCENDANT );
		menu->addCheckItem( CMD_SHOWIMUMCOELI, lang.getObjectName( OIMUMCOELI, format, props->isVedic() ),
			props->getObjectStyle() & OI_IMUMCOELI );
	}

	menu->addCheckItem( CMD_SHOWURANIAN_INNER, _( "4 Uranian (Cupido-Kronos)"), props->getObjectStyle() & OI_URANIAN_INNER );
	menu->addCheckItem( CMD_SHOWURANIAN_OUTER, _( "4 Uranian (Apollon-Poseidon)"), props->getObjectStyle() & OI_URANIAN_OUTER );
	menu->addCheckItem( CMD_SHOWCHIRON, lang.getObjectName( OCHIRON, format, props->isVedic() ),
		props->getObjectStyle() & OI_CHIRON );
	menu->addCheckItem( CMD_SHOWPHOLUS, lang.getObjectName( OPHOLUS, format, props->isVedic() ),
		props->getObjectStyle() & OI_PHOLUS );
	menu->addCheckItem( CMD_SHOWPLANETOIDS, _( "4 Planetoids"), props->getObjectStyle() & OI_PLANETOIDS );
	menu->addCheckItem( CMD_SHOWLILITH, lang.getObjectName( OLILITH, format, props->isVedic() ),
		props->getObjectStyle() & OI_LILITH );

	if ( wo & WO_MENU_FULL_OBJECT )
	{
		if ( props->isVedic())
		{
			menu->addCheckItem( CMD_SHOWUPAGRAHAS, _( "5 Upagrahas"), props->getObjectStyle() & OI_UPAGRAHAS );
			menu->addCheckItem( CMD_SHOWKALAVELAS, _( "Kalavelas"), props->getObjectStyle() & OI_KALAVELAS );
			menu->addCheckItem( CMD_SHOWSPECIALLAGNAS, _( "3 Special Lagnas"), props->getObjectStyle() & OI_SPECIALLAGNAS );
			menu->addCheckItem( CMD_SHOWD9LAGNA, _( "Sphuta Navamsa Lagna"), props->getObjectStyle() & OI_D9_LAGNA );
		}
		else menu->addCheckItem( CMD_SHOWARIES, _( "Aries"), props->getObjectStyle() & OI_ARIES );

		menu->addCheckItem( CMD_SHOW_HOUSES, _( "All Houses"), props->getObjectStyle() & OI_ALL_HOUSES );
		menu->addCheckItem( CMD_SHOW_4HOUSES, _( "4 Houses (2, 3, 11, 12)"), props->getObjectStyle() & OI_4_HOUSES );

		size_t size = ( props->isVedic() ? config->vedic->arabicParts.size() : config->western->arabicParts.size() );
		if ( size > 0 )
		{
			menu->addCheckItem( CMD_SHOWARABICPARTS, _( "Arabic Parts"), props->getObjectStyle() & OI_ARABICPARTS );
		}
	}
	return menu;
}

/**************************************************************
***
**    ContextMenuProvider   ---   getMainViewColumnMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getMainViewColumnMenu( ChartProperties *props )
{
	wxMenu *menu = new wxMenu( _( "Columns" ));

	if ( props->isVedic())
	{
		menu->Append( CMD_MAIN_SHOW_DECLINATION, _( "Declinations" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_DECLINATION, props->getVedicColumnStyle() & TAB_CT_LATITUDE );

		menu->Append( CMD_MAIN_SHOW_LORD, _( "Sign Lord" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_LORD, props->getVedicColumnStyle() & TAB_CT_SIGN_LORD );

		menu->Append( CMD_MAIN_SHOW_ELEMENT, _( "Element" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_ELEMENT, props->getVedicColumnStyle() & TAB_CT_ELEMENT );

		menu->Append( CMD_MAIN_SHOW_SIGN_QUALITY, _( "Sign Quality" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_SIGN_QUALITY, props->getVedicColumnStyle() & TAB_CT_SIGN_QUALITY );

		menu->Append( CMD_MAIN_SHOW_SIGNIFICATION, _( "Basic Signification" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_SIGNIFICATION, props->getVedicColumnStyle() & TAB_CT_SIGNIFICATION );

		menu->Append( CMD_MAIN_SHOW_DIGNITY, _( "Dignity" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_DIGNITY, props->getVedicColumnStyle() & TAB_CT_DIGNITY );
		menu->Append( CMD_MAIN_SHOW_NAVAMSA, _( "Navamsa (D-9)" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_NAVAMSA, props->getVedicColumnStyle() & TAB_CT_NAVAMSA );
		menu->Append( CMD_MAIN_SHOW_Nakshatra, _( "Nakshatra" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_Nakshatra, props->getVedicColumnStyle() & TAB_CT_NAKSHATRA );
		menu->Append( CMD_MAIN_SHOW_PADA, _( "Nakshatra Padas" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_PADA, props->getVedicColumnStyle() & TAB_CT_NAKSHATRA_PADA );

		menu->Append( CMD_MAIN_SHOW_KARAKA, _( "Jaimini Karaka" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_KARAKA, props->getVedicColumnStyle() & TAB_CT_CHARA_KARAKA );
		menu->Append( CMD_MAIN_SHOW_KP, _( "Krishnamurti Lords" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_KP, props->getVedicColumnStyle() & TAB_CT_KP_LORDS );

		menu->Append( CMD_MAIN_SHOW_SHASTIAMSA, _( "Shastiamsa (D-60)" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_SHASTIAMSA, props->getVedicColumnStyle() & TAB_CT_SHASTIAMSA_LORD );
		menu->Append( CMD_MAIN_SHOW_DASAVARGA, _( "Dasa Varga Dignity" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_DASAVARGA, props->getVedicColumnStyle() & TAB_CT_DASA_VARGA );
		menu->Append( CMD_MAIN_SHOW_HOUSEPOS, _( "Bhava" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_HOUSEPOS, props->getVedicColumnStyle() & TAB_CT_HOUSE_POSITION );
		menu->Append( CMD_MAIN_SHOW_ASHTAKA, _( "Ashtakavarga" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_ASHTAKA, props->getVedicColumnStyle() & TAB_CT_AV_REKHAPOINTS );
	}
	else
	{
		menu->Append( CMD_MAIN_SHOW_DECLINATION, _( "Declinations" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_DECLINATION, props->getWesternColumnStyle() & TAB_CT_LATITUDE );

		menu->Append( CMD_MAIN_SHOW_90_DEGREE_LONGITUDE, _( "90 Degrees" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_90_DEGREE_LONGITUDE, props->getWesternColumnStyle() & TAB_CT_90_DEGREE_LONGITUDE );

		menu->Append( CMD_MAIN_SHOW_45_DEGREE_LONGITUDE, _( "45 Degrees" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_45_DEGREE_LONGITUDE, props->getWesternColumnStyle() & TAB_CT_45_DEGREE_LONGITUDE );

		menu->Append( CMD_MAIN_SHOW_ANTISCIA, _( "Antiscia" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_ANTISCIA, props->getWesternColumnStyle() & TAB_CT_ANTISCIA );

		menu->Append( CMD_MAIN_SHOW_LORD, _( "Sign Lord" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_LORD, props->getWesternColumnStyle() & TAB_CT_SIGN_LORD );

		menu->Append( CMD_MAIN_SHOW_ELEMENT, _( "Element" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_ELEMENT, props->getWesternColumnStyle() & TAB_CT_ELEMENT );

		menu->Append( CMD_MAIN_SHOW_SIGN_QUALITY, _( "Sign Quality" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_SIGN_QUALITY, props->getWesternColumnStyle() & TAB_CT_SIGN_QUALITY );

		menu->Append( CMD_MAIN_SHOW_HOUSEPOS, _( "House Positions" ), wxT( "" ), true );
		menu->Check( CMD_MAIN_SHOW_HOUSEPOS, props->getWesternColumnStyle() & TAB_CT_HOUSE_POSITION );
	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getGraphicStyleMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getGraphicStyleMenu( const int &wo, ChartProperties *props )
{
	wxMenu *menu = new MyMenu( _( "Chart Options" ));

	if ( props->isVedic())
	{
		menu->Append( CMD_VCT_SOUTH, _("South Indian Style"), wxT( "" ), true );
		menu->Check( CMD_VCT_SOUTH, props->getVedicChartDisplayConfig().indianChartType == VCT_SOUTH );
		menu->Append( CMD_VCT_NORTH, _("North Indian Style"), wxT( "" ), true );
		menu->Check( CMD_VCT_NORTH, props->getVedicChartDisplayConfig().indianChartType == VCT_NORTH );
		menu->Append( CMD_VCT_EAST, _("East Indian Style"), wxT( "" ), true );
		menu->Check( CMD_VCT_EAST, props->getVedicChartDisplayConfig().indianChartType == VCT_EAST );

		if ( props->getVedicChartDisplayConfig().indianChartType == VCT_NORTH )
		{
			wxMenu *northmenu = new wxMenu();
			northmenu->Append( CMD_VCN_ASC, _("Ascendant Only"), wxT( "" ), true );
			northmenu->Check( CMD_VCN_ASC, props->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_ASC );
			northmenu->Append( CMD_VCN_NUMBER, _("Number"), wxT( "" ), true );
			northmenu->Check( CMD_VCN_NUMBER, props->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_NUMBER );
			northmenu->Append( CMD_VCN_SHORT, _("Short String"), wxT( "" ), true );
			northmenu->Check( CMD_VCN_SHORT, props->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_SHORT );
			northmenu->Append( CMD_VCN_SYMBOL, _("Symbol"), wxT( "" ), true );
			northmenu->Check( CMD_VCN_SYMBOL, props->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_SYMBOL );
			menu->Append( -1, _( "Display Signs" ), northmenu );
		}
		if ( wo & WO_MENU_FULL_GRAPHIC_STYLE )
		{
			MyMenu *centermenu = new MyMenu( _( "Chart Center" ));
			centermenu->Append( CMD_VCC_NOTHING, _("Nothing"), wxT( "" ), true );
			centermenu->Check( CMD_VCC_NOTHING, props->getVedicChartDisplayConfig().centerInfoType == VCC_NOTHING );
			centermenu->Append( CMD_VCC_NAME, _("Name"), wxT( "" ), true );
			centermenu->Check( CMD_VCC_NAME, props->getVedicChartDisplayConfig().centerInfoType == VCC_NAME );
			centermenu->Append( CMD_VCC_NAME_SIGNIFICATION, _("Name + Signification"), wxT( "" ), true );
			centermenu->Check( CMD_VCC_NAME_SIGNIFICATION, props->getVedicChartDisplayConfig().centerInfoType == VCC_NAME_SIGNIFICATION );
			centermenu->Append( CMD_VCC_DIVISION, _("Division"), wxT( "" ), true );
			centermenu->Check( CMD_VCC_DIVISION, props->getVedicChartDisplayConfig().centerInfoType == VCC_DIVISION );
			centermenu->Append( CMD_VCC_DIVISION_SIGNIFICATION, _("Division + Signification"), wxT( "" ), true );
			centermenu->Check( CMD_VCC_DIVISION_SIGNIFICATION, props->getVedicChartDisplayConfig().centerInfoType == VCC_DIVISION_SIGNIFICATION );
			menu->Append( -1, _( "Chart Center" ), centermenu );
		}
		menu->AppendSeparator();

		if ( wo & WO_MENU_FULL_GRAPHIC_STYLE )
		{
			menu->Append( CMD_VCS_RETRO, _( "Retrograde Planets"), wxT( "" ), true );
			menu->Check( CMD_VCS_RETRO, props->getVedicChartDisplayConfig().showRetro );
			menu->Append( CMD_VCS_ARUDHAS, _( "Arudhas"), wxT( "" ), true );
			menu->Check( CMD_VCS_ARUDHAS, props->getVedicChartDisplayConfig().showArudhas );
			
			if ( wo & WO_MENU_TRANSIT )
			{
				menu->Append( CMD_VCS_ASHTAKAVARGA, _( "Ashtakavarga"), wxT( "" ), true );
				menu->Check( CMD_VCS_ASHTAKAVARGA, props->getVedicChartDisplayConfig().showAshtakavarga );
			}
			menu->Append( CMD_VCS_PLANETCOLORS, _( "Colors for Planet Symbols"), wxT( "" ), true );
			menu->Check( CMD_VCS_PLANETCOLORS, props->getVedicChartDisplayConfig().showPlanetColors );
		}
	}
	else // western
	{
		menu->Append( CMD_WCS_RETRO, _( "Retrograde Planets"), wxT( "" ), true );
		menu->Check( CMD_WCS_RETRO, props->getWesternChartDisplayConfig().showRetro );
		menu->Append( CMD_WCS_ASPECTS, _( "Aspects"), wxT( "" ), true );
		menu->Check( CMD_WCS_ASPECTS, props->getWesternChartDisplayConfig().showAspects );
		menu->Append( CMD_WCS_ASPECTSYMBOLS, _( "Aspect Symbols"), wxT( "" ), true );
		menu->Check( CMD_WCS_ASPECTSYMBOLS, props->getWesternChartDisplayConfig().showAspectSymbols );
		menu->Append( CMD_WCS_HOUSES, _( "Houses"), wxT( "" ), true );
		menu->Check( CMD_WCS_HOUSES, props->getWesternChartDisplayConfig().showHouses );

		if ( wo & WO_MENU_TRANSIT )
		{
			menu->Append( CMD_WCS_SECONDCHART_INSIDE, _( "Transits Inside"), wxT( "" ), true );
			menu->Check( CMD_WCS_SECONDCHART_INSIDE, props->getWesternChartDisplayConfig().secondchartStyle );
		}

	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getSarvatobhadraMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getSarvatobhadraMenu( ChartProperties *props )
{
	MyMenu *menu = new MyMenu( _( "Sarvatobhadra" ));

	menu->Append( CMD_VCS_RETRO, _( "Retrograde Planets"), wxT( "" ), true );
	menu->Check( CMD_VCS_RETRO, props->getVedicChartDisplayConfig().showRetro );
	menu->Append( CMD_VCS_PLANETCOLORS, _( "Colors for Planet Symbols"), wxT( "" ), true );
	menu->Check( CMD_VCS_PLANETCOLORS, props->getVedicChartDisplayConfig().showPlanetColors );
	menu->Append( CMD_VCS_SANSKRITSYMBOLS, _( "Sanskrit Symbols"), wxT( "" ), true );
	menu->Check( CMD_VCS_SANSKRITSYMBOLS, props->getVedicChartDisplayConfig().showSbcSanskritSymbols );
	menu->Append( CMD_VCS_SBC_NAKSHATRA_QUALITY, _( "Benefic/Malefic Nakshatras"), wxT( "" ), true );
	menu->Check( CMD_VCS_SBC_NAKSHATRA_QUALITY, props->getVedicChartDisplayConfig().showSbcNakshatraQuality );
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getAppWindowContextMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getAppWindowContextMenu()
{
	const int appmenuitems[] = { APP_NEW, APP_OPEN, 0, APP_EPHEMERIS,
		APP_ECLIPSE, APP_HORA, APP_PARTNER, 0, -1 };

	MyMenu *menu = new MyMenu;
	menu->addArray( appmenuitems );
	menu->addWindowListMenu();
	menu->addItem( APP_CONFIGURE );
	menu->addItem( APP_ATLASDIALOG );
	menu->AppendSeparator();
	menu->addItem( APP_EXIT );
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getToolbarNewMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getToolbarNewMenu()
{
	MyMenu *newmenu = new MyMenu;
	newmenu->addItem( APP_NEW_SINGLE );

	newmenu->AppendSeparator();
	vector<MultipleViewConfig*> &v = MultipleViewConfigLoader::get()->getConfigs();
	for ( uint i = 0; i < v.size(); i++ ) newmenu->Append( APP_NEW_MULTIPLE + i, v[i]->name );
	return newmenu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getChartMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getChartMenu( ChartProperties *props, const bool &ismainview )
{
	MyMenu *menu = new MyMenu( _( "Horoscope" ));
	menu->addItem( CMD_CLOSE );
	if ( props->hasDocument())
	{
		if( ismainview ) menu->addItem( CMD_CLOSE_DOC );
		menu->addItem( CMD_SAVE );
		menu->addItem( CMD_SAVEAS );
		menu->addItem( CMD_PRINT );
		menu->addItem( CMD_QUICKPRINT );

#ifdef USE_PRINT_VIEW
		menu->addItem( CMD_NEW_PRINTPREVIEW );
#endif

	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getNewViewMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getNewViewMenu( ChartProperties *props )
{
	const int vedicitems[] = { 0, CMD_NEW_DASA_TREE, CMD_NEW_DASA_COMPOSITE, CMD_NEW_GRAPHICALDASA,
		CMD_NEW_TRANSIT, 0, CMD_NEW_YOGA, CMD_NEW_ASHTAKAVARGA,
		CMD_NEW_SHADBALA, CMD_NEW_SOLAR, -1 };

	const int westernitems[5] = { 0, CMD_NEW_URANIAN, CMD_NEW_TRANSIT, CMD_NEW_SOLAR, -1 };
	const int appitems[5] = { APP_EPHEMERIS, APP_ECLIPSE, APP_HORA, APP_PARTNER, -1 };

	MyMenu *menu = new MyMenu( _( "New View" ) );
	bool vedicmode = props->isVedic();
	if ( wxGetKeyState( WXK_SHIFT )) vedicmode = ! vedicmode;

	if ( props->hasDocument())
	{
		// Varga submenu or western chart
		if ( vedicmode ) menu->addVargaMenu();
		else menu->addItem( CMD_NEW_WCHART );

		if ( vedicmode )
		{
			menu->addItem( CMD_NEW_VARGA );
			menu->addItem( CMD_NEW_SBC );
		}
		menu->addItem( CMD_NEW_TEXT );
		menu->addArray( vedicmode ? vedicitems : westernitems );
	}
	else
	{
		menu->addArray( appitems );
	}
	return menu;
}

