/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChildWindowListCtrl.h
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

