/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChildWindowListCtrl.cpp
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

#include "ChildWindowListCtrl.h"

#include "BasicView.h"
#include "ChildWindow.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "DataSet.h"
#include "Document.h"
#include "IdConverter.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "MenuProvider.h"

#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/string.h>

extern Config *config;

IMPLEMENT_CLASS2( ChildWindowListCtrl, wxWindow, KeyEventObserver )

/*****************************************************
**
**   CLASS ChildWindowListCtrlEvtHandler
**
******************************************************/
class ChildWindowListCtrlEvtHandler : public wxEvtHandler
{
	DECLARE_CLASS( ChildWindowListCtrlEvtHandler )
public:
	ChildWindowListCtrlEvtHandler( ChildWindowListCtrl *listctrl ) {
		this->listctrl = listctrl;
	}

private:
	virtual void OnContextMenuEvent( wxMouseEvent& event) {
		listctrl->OnRightDown( event );
	}
	ChildWindowListCtrl *listctrl;
	DECLARE_EVENT_TABLE()
};

IMPLEMENT_CLASS( ChildWindowListCtrlEvtHandler, wxEvtHandler )
BEGIN_EVENT_TABLE( ChildWindowListCtrlEvtHandler, wxEvtHandler)
	EVT_RIGHT_DOWN( ChildWindowListCtrlEvtHandler::OnContextMenuEvent )
END_EVENT_TABLE()

/*****************************************************
**
**   ChildWindowListCtrl   ---   Constructor
**
******************************************************/
ChildWindowListCtrl::ChildWindowListCtrl( wxWindow* parent, wxWindowID id )
	: wxWindow( parent, id ),
	KeyEventObserver()
{
	listctrl = (wxListCtrl*)NULL;
	activeitem = -1;

	Connect( wxEVT_SIZE, wxSizeEventHandler( ChildWindowListCtrl::OnSize ));
	Connect( LISTCTRL_ID, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ChildWindowListCtrl::OnListItemActivated ));

	recreate();
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   Destructor
**
******************************************************/
ChildWindowListCtrl::~ChildWindowListCtrl()
{
	listctrl->PopEventHandler( true );
	delete listctrl;
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   recreate
**
******************************************************/
void ChildWindowListCtrl::recreate()
{
	wxListItem itemCol;
	ImageProvider *ip = ImageProvider::get();

	// delete old listctrl after creating the new one
	// this is a bug of 7.0.1 on GTK (GTK event loop crash on style change)
	wxListCtrl *old_listctrl = listctrl;

	if ( false && listctrl )
	{
		listctrl->PopEventHandler( true );
		delete listctrl;
	}
	int width = GetParent()->GetSize().GetWidth() / 4 - 2;
	switch ( config->viewprefs->listStyle )
	{
	case 1:
		listctrl = new wxListCtrl( this, LISTCTRL_ID, wxDefaultPosition, wxDefaultSize,
			wxLC_SMALL_ICON|wxLC_SINGLE_SEL|wxSUNKEN_BORDER );
		listctrl->AssignImageList( ip->createViewImageList( true ), wxIMAGE_LIST_SMALL );
		break;
	case 2:
		listctrl = new wxListCtrl( this, LISTCTRL_ID, wxDefaultPosition, wxDefaultSize,
			wxLC_REPORT|wxLC_SINGLE_SEL|wxSUNKEN_BORDER );
		listctrl->AssignImageList( ip->createViewImageList( true ), wxIMAGE_LIST_SMALL );

		itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
		itemCol.m_text = _( "Type" );
		listctrl->InsertColumn(0, itemCol);
		listctrl->SetColumnWidth( 0, width );

		itemCol.m_text = _( "Name" );
		listctrl->InsertColumn(1, itemCol);
		listctrl->SetColumnWidth( 1, width );

		itemCol.m_text = _( "Date" );
		listctrl->InsertColumn(2, itemCol);
		listctrl->SetColumnWidth( 2, width );

		itemCol.m_text = _( "Location" );
		listctrl->InsertColumn(3, itemCol);
		listctrl->SetColumnWidth( 3, width );
		break;
	default:
		listctrl = new wxListCtrl( this, LISTCTRL_ID, wxDefaultPosition, wxDefaultSize,
			wxLC_ICON|wxLC_SINGLE_SEL|wxSUNKEN_BORDER );
		listctrl->AssignImageList( ip->createViewImageList( false ), wxIMAGE_LIST_NORMAL );
		break;
	}
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
		insertListItem( *iter );

	listctrl->SetSize( GetSize() );
	listctrl->Arrange();
	if ( activeitem != -1 ) listctrl->SetItemState( activeitem ,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	listctrl->PushEventHandler( new ChildWindowListCtrlEvtHandler( this ));
	listctrl->Show( true );

	listctrl->SetBackgroundColour( config->colors->bgColor );
	listctrl->SetForegroundColour( config->colors->fgColor );

	if ( old_listctrl )
	{
		old_listctrl->PopEventHandler( true );
		delete old_listctrl;
	}
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   getNumberOfEntries
**
******************************************************/
int ChildWindowListCtrl::getNumberOfEntries()
{
	return windowlist.size();
}

/***********************************************
***
**  ChildWindowListCtrl   ---   setActiveItem
***
***********************************************/
void ChildWindowListCtrl::setActiveItem( const int &i )
{
	activeitem = i;
}

/***********************************************
***
**  ChildWindowListCtrl   ---   getActiveChild
***
***********************************************/
ChildWindow *ChildWindowListCtrl::getActiveChild()
{
	if ( activeitem == -1 ) return 0;
	else return windowlist[activeitem];
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   updateItemLabel
**
******************************************************/
void ChildWindowListCtrl::updateItemLabel( ChildWindow *child, const int &pos )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();

	assert( child );
	wxString name = wxT( "NONAME" );

	Document *doc = child->getDoc();
	if ( doc ) name = doc->getHName();

	wxString label = wxT( "NONE" );
	if ( ! child->isMultiple())
	{
		SimpleChildWindow *sc = wxDynamicCast( child, SimpleChildWindow );
		assert( sc );

		BasicView *view = sc->getView();
		assert( view );
		label = view->getWindowLabel();
	}
	else label = _( "Multiple View" );
	printf( "LABEL is %s\n", str2char( label ));

	wxString title = wxT( "ERROR" );
	if ( config->viewprefs->listStyle == 2 )
	{
		listctrl->SetItem( pos, 0, label );
		if ( doc )
		{
			listctrl->SetItem( pos, 1, doc->getHName() );
			Location *location = doc->getLocation();
			listctrl->SetItem( pos, 2,
				formatter->formatFullDateString( doc->getJD() + ( location->getTimeZone() + location->getDST())/24 ) );
			listctrl->SetItem( pos, 3, doc->getLocation()->getLocName() );
		}
	}
	else
	{
		if( child->isMainWindow())
		{
			assert( doc );
			if ( name.Len() > 12 ) title.Printf( wxT( "%s..." ), name.Left( 12 ).c_str());
			else title = name;
		}
		else
		{
			if ( doc )
			{
				if ( name.Len() > 10 ) title.Printf( wxT( "%s...<%s>" ), name.Left( 10 ).c_str(), label.c_str() );
				else title.Printf( wxT( "%s <%s>" ), name.c_str(), label.c_str() );
			}
			else // Simplechild without doc
			{
				title = label;
			}
		}
		listctrl->SetItemText( pos, title );
		//listctrl->RefreshItem( pos );
	}
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   insertListItem
**
******************************************************/
void ChildWindowListCtrl::insertListItem( ChildWindow *item )
{
	int i = item->getViewListImageId();
	int pos = listctrl->InsertItem( listctrl->GetItemCount(), wxT( "--" ), i );
	updateItemLabel( item, pos );
	listctrl->SetItemState( pos, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   onConfigChanged
**
******************************************************/
// doc based views are update by DocManager in ApplcationWindows
void ChildWindowListCtrl::onConfigChanged()
{
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
	{
		ChildWindow *child = *iter;
		if ( ! child->getDoc())
		{
			child->OnDataChanged();
		}
	}

	// colors may have changed
	listctrl->SetBackgroundColour( config->colors->bgColor );
	listctrl->SetForegroundColour( config->colors->fgColor );
	recreate();
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   updateItemLabels
**
******************************************************/
void ChildWindowListCtrl::updateItemLabels()
{
	int i = 0;
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
	{
		updateItemLabel( *iter, i );
		i++;
	}
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   addEntry
**
******************************************************/
void ChildWindowListCtrl::addEntry( ChildWindow *child )
{
	windowlist.push_back( child );
	insertListItem( child );
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   deleteEntry
**
******************************************************/
void ChildWindowListCtrl::deleteEntry( ChildWindow *child )
{
	int i = 0;
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
	{
		ChildWindow *current = *iter;
		if ( current == child )
		{
			windowlist.erase( iter );
			listctrl->DeleteItem( i );
			//recreate();
			break;
		}
		i++;
	}
	if ( windowlist.size() == 0 )
	{
		activeitem = -1;
	}
	else
	{
		if ( activeitem == i ) selectItem( i == 0 ? 0 : i - 1 );
	}
	listctrl->Arrange();
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   OnSize
**
******************************************************/
void ChildWindowListCtrl::OnSize( wxSizeEvent &event )
{
	listctrl->SetSize( event.GetSize() );
	listctrl->Arrange();
}

/***********************************************
***
**  ChildWindowListCtrl   ---   getChild
***
***********************************************/
ChildWindow *ChildWindowListCtrl::getChild( const int &i )
{
	return windowlist[i];
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   selectItem
**
******************************************************/
void ChildWindowListCtrl::selectItem( const int &i )
{
	//printf( "ChildWindowListCtrl::selectItem -->  Active item %d -- i %d\n", activeitem, i );
	if ( activeitem == i ) return;
	activeitem = i;
	listctrl->SetItemState( i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   OnListItemActivated
**
******************************************************/
void ChildWindowListCtrl::OnListItemActivated( wxListEvent& event )
{
	ChildWindow *child = windowlist[event.GetIndex()];
	if ( ! child )
	{
		wxLogError( wxT( "Error: can't find child  %ld in list"),  event.GetIndex() );
		return;
	}
	child->Raise();
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   OnListItemSelected
**
******************************************************/
void ChildWindowListCtrl::OnListItemSelected( wxListEvent& event )
{
	ChildWindow *child = windowlist[event.GetIndex()];
	if ( ! child )
	{
		wxLogError( wxT( "Error: can't find child %ld in list" ), event.GetIndex() );
		return;
	}
}

/*****************************************************
**
**   ChildWindowListCtrl   ---   OnRightDown
**
******************************************************/
void ChildWindowListCtrl::OnRightDown( wxMouseEvent &event )
{
	int flag = wxLIST_HITTEST_ONITEM;
	wxMenu *menu = (wxMenu*)NULL;

	long int i = listctrl->HitTest( event.GetPosition(), flag );

	if ( i == -1 )
	{
		menu = ContextMenuProvider().getAppWindowContextMenu();
	}
	else
	{
		selectItem( i );
		//BasicView *view = (BasicView*)NULL;
		ChildWindow *child = windowlist[i];
		assert( child );

		menu = ContextMenuProvider().getChildWindowListCtrlMenu( child );
	}
	PopupMenu( menu, event.GetX(), event.GetY() );
	delete menu;
}



