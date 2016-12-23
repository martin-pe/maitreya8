/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChildWindowListCtrl.h
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
#ifndef CHILDWINDOWLISTCTRL_H_
#define CHILDWINDOWLISTCTRL_H_

#include <wx/window.h>
#include <vector>

#include "Commands.h"
#include "guibase.h"

class ChildWindow;
class Document;
class wxListCtrl;
class wxListEvent;
class wxMouseEvent;
class wxSizeEvent;

using namespace std;

#define LISTCTRL_ID wxID_HIGHEST + 12000


/*************************************************//**
*
* \brief list control for main window supporting icons and several view types
*
******************************************************/
class ChildWindowListCtrl : public wxWindow, public KeyEventObserver
{
	DECLARE_CLASS( ChildWindowListCtrl )
public:

	ChildWindowListCtrl( wxWindow* parent, wxWindowID id );
	~ChildWindowListCtrl();
	void recreate();

	void addEntry( ChildWindow* );
	void deleteEntry( ChildWindow* );
	int getNumberOfEntries();

	ChildWindow *getChild( const int& );
	ChildWindow *getActiveChild();

	void selectItem( const int & );
	void setActiveItem( const int & );

	void OnRightDown( wxMouseEvent &event );
	void updateItemLabels();
	void onConfigChanged();

private:

	int activeitem;
	wxListCtrl *listctrl;
	vector<ChildWindow*> windowlist;

	void insertListItem( ChildWindow *item );
	void updateItemLabel( ChildWindow *item, const int &pos );

	void OnSize( wxSizeEvent& );
	void OnListItemActivated( wxListEvent& );
	void OnListItemSelected( wxListEvent& );
	void OnKeyDown( wxKeyEvent& );

};


#endif

