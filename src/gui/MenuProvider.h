/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/MenuProvider.h
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

#ifndef _MENUPROVIDER_H_
#define _MENUPROVIDER_H_

#include <wx/menu.h>
#include <wx/string.h>

class BasicView;
class BasicWidget;
class ChartProperties;
class ChildWindow;

/*************************************************//**
*
* \brief specific menu class, transporting View details
*
******************************************************/
class MyMenu : public wxMenu
{
public:

	MyMenu( const wxString title = wxEmptyString );

	wxMenuItem *addItem( const int &id );
	void addCheckItem( const int &id, const bool checked = false );
	void addCheckItem( const int &id, const wxString title, const bool checked = false );
	void addArray( const int a[] );
	void addWindowListMenu();
	void addVargaMenu();


private:
	class CommandHandler *handler;

	DECLARE_CLASS( MyMenu )
};


/*************************************************//**
*
* \brief main menu bar of the appliation
*
******************************************************/
class AppMenuBar : public wxMenuBar
{
public:
	AppMenuBar( int style );

	void updateMenus( ChildWindow* );

	void addToRecentFiles( const wxString& );
	wxString getRecentFilesEntry( const int& );
	void saveRecentFiles();

private:
	MyMenu *filemenu, *westernmenu, *vedicmenu, *extramenu, *windowmenu, *helpmenu, *recentmenu, *newmenu;
	wxArrayString recentfiles;
	wxMenuItem *newDefaultItem;
};

/*************************************************//**
*
* \brief creates the context menu according to view options
*
******************************************************/
class ContextMenuProvider : public wxObject
{
public:
	wxMenu *getWidgetMenu( BasicWidget* );
	wxMenu *getWidgetMenu( ChartProperties*, const int &widgetOptions );

	wxMenu *getChildWindowListCtrlMenu( ChildWindow* );
	wxMenu *getAppWindowContextMenu();

	wxMenu *getToolbarNewMenu();

private:
	wxMenu *getSkinMenu( ChartProperties* );
	wxMenu *getObjectSubMenu( const int &wo, ChartProperties* );
	wxMenu *getMainViewColumnMenu( ChartProperties* );
	wxMenu *getGraphicStyleMenu( const int &wo, ChartProperties* );
	wxMenu *getSarvatobhadraMenu( ChartProperties* );
	wxMenu *getExportMenu( const int &wo );

	wxMenu *getChartMenu( ChartProperties*, const bool &ismainview );
	wxMenu *getNewViewMenu( ChartProperties* );

};


#endif


