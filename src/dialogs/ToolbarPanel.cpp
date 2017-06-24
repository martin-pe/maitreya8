/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ToolbarPanel.cpp
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

#include "ToolbarPanel.h"

#include "Commands.h"
#include "Conf.h"
#include "ConfigDialog.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "mvalidator.h"
#include "ToolPanel.h"

#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

extern Config *config;
enum { CD_LEFT = wxID_HIGHEST + 1, CD_RIGHT, CD_UP, CD_DOWN, CD_LIST1, CD_LIST2 };

IMPLEMENT_CLASS( ToolbarPanel, ConfigPanel )

/*****************************************************
**
**   ToolbarPanel   ---   Constructor
**
******************************************************/
ToolbarPanel::ToolbarPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	tconfig = new ToolbarConfig;
	config2model();

    // begin wxGlade: ToolbarPanel::ToolbarPanel
    list1 = new wxListCtrl(this, CD_LIST1, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL);
    button_right = new wxBitmapButton(this, CD_RIGHT, wxNullBitmap);
    button_left = new wxBitmapButton(this, CD_LEFT, wxNullBitmap);
    button_up = new wxBitmapButton(this, CD_UP, wxNullBitmap);
    button_down = new wxBitmapButton(this, CD_DOWN, wxNullBitmap);
    list2 = new wxListCtrl(this, CD_LIST2, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL);
    label_toolstyle = new wxStaticText(this, wxID_ANY, _("Show Tools as"));
    const wxString choice_toolstyle_choices[] = {
        _("Text"),
        _("Icon"),
        _("Text and Icon"),
    };
    choice_toolstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_toolstyle_choices);

    set_properties();
    do_layout();
    // end wxGlade

	choice_toolstyle->SetValidator( MChoiceValidator( &tconfig->toolbarStyle ));

	initToolItems();
	initRightControl();
	updateUi();

	Connect( CD_LEFT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnLeft ));
	Connect( CD_RIGHT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnRight ));
	Connect( CD_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnUp ));
	Connect( CD_DOWN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnDown ));

	Connect( CD_LIST1, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ToolbarPanel::OnLeftSelected ));
	Connect( CD_LIST2, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ToolbarPanel::OnRightSelected ));

	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolbarPanel::OnChoice ));
}

/*****************************************************
**
**   ToolbarPanel   ---   Destructor
**
******************************************************/
ToolbarPanel::~ToolbarPanel()
{
	delete imageList;
	delete tconfig;
}

/*****************************************************
**
**   ToolbarPanel   ---   config2model
**
******************************************************/
void ToolbarPanel::config2model()
{
	*tconfig = *config->toolbar;
}

/*****************************************************
**
**   ToolbarPanel   ---   model2config
**
******************************************************/
void ToolbarPanel::model2config()
{
	*config->toolbar = *tconfig;
}

/*****************************************************
**
**   ToolbarPanel   ---   updateUi
**
******************************************************/
void ToolbarPanel::updateUi()
{
}

/*****************************************************
**
**   ToolbarPanel   ---   initToolItems
**
******************************************************/
void ToolbarPanel::initToolItems()
{
	uint i;
	ToolbarFactory *factory = ToolbarFactory::get();
	CommandHandler *handler = CommandHandler::get();
	ImageProvider *ip = ImageProvider::get();
	MCommand *command;

	vector<int> fullCommandIds = factory->getFullCommandIdList();

	const int size = ip->getDefaultBitmapSize();
	imageList = new wxImageList( size, size, true );
	imageList->Add( ip->getBitmap( BM_SEPARATOR ));

	list1->SetImageList( imageList, wxIMAGE_LIST_SMALL );
	list2->SetImageList( imageList, wxIMAGE_LIST_SMALL );
	imageListIds.clear();

	// setup left list
	list1->DeleteAllItems();

	// insert separator
	wxListItem item;
	item.SetId( 0 );
	item.SetImage( 0 );
	item.SetText( _( "Separator" ));
	list1->InsertItem( item );

	for ( i = 0; i < fullCommandIds.size(); i++ )
	{
		command = handler->getCommand( fullCommandIds[i] );
		imageList->Add( ip->getBitmap( command->getBitmapId() ));
		imageListIds[ command->getBitmapId() ] = i + 1;

		wxListItem item;
		item.SetId( i + 1 );
		item.SetData( fullCommandIds[i] );
		item.SetImage( i + 1 );
		item.SetText( command->getText() );
		list1->InsertItem( item );
	}

	list1->SetColumnWidth( 0, wxLIST_AUTOSIZE );
	list1->Arrange();
	left_sel = 0;
	list1->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

/*****************************************************
**
**   ToolbarPanel   ---   updateItemConfig
**
******************************************************/
void ToolbarPanel::updateItemConfig()
{
	long commandId;
	tconfig->mainToolbarItems.clear();
	for( long l = 0; l < list2->GetItemCount(); l++ )
	{
		commandId = list2->GetItemData( l );
		tconfig->mainToolbarItems.push_back( commandId );
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   initRightControl
**
******************************************************/
void ToolbarPanel::initRightControl()
{
	ToolbarFactory *factory = ToolbarFactory::get();

	if ( tconfig->mainToolbarItems.size() == 0 )
	{
		tconfig->mainToolbarItems = factory->getDefaultCommandIdList();
	}

	list2->DeleteAllItems();
	for( uint i = 0; i < tconfig->mainToolbarItems.size(); i++ )
	{
		insertRightItemById( tconfig->mainToolbarItems[i], i, false );
	}

	assert( list2->GetItemCount() > 0 );
	list2->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	right_sel = 0;
	list2->SetColumnWidth( 0, wxLIST_AUTOSIZE );
	list2->Arrange();
}

/*****************************************************
**
**   ToolbarPanel   ---   OnLeft
**
******************************************************/
void ToolbarPanel::OnLeft( wxCommandEvent& )
{
	if ( right_sel > -1 )
	{
		list2->DeleteItem( right_sel );
		const int new_sel = Max( 0, right_sel - 1 );
		list2->SetItemState( new_sel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list2->EnsureVisible( new_sel );
		list2->Arrange();
		updateItemConfig();
		setDirty();
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnRight
**
******************************************************/
void ToolbarPanel::OnRight( wxCommandEvent& )
{
	if ( left_sel < 0 || right_sel < 0 )
	{
		printf( "ToolbarPanel::OnRight ERROR: wrong selection left %d right %d\n", left_sel, right_sel );
		return;
	}
	const int commandId = list1->GetItemData( left_sel );

	for( long l = 0; l < list2->GetItemCount(); l++ )
	{
		if ( commandId && (int)list2->GetItemData( l ) == commandId )
		{
			doMessageBox( this, wxString::Format( _( "Item already in toolbar at position %ld" ), l + 1 ), wxICON_ERROR | wxOK );
			return;
		}
	}

	insertRightItemById( commandId, right_sel + 1 );
	updateItemConfig();
	setDirty();
}

/*****************************************************
**
**   ToolbarPanel   ---   insertRightItemById
**
******************************************************/
void ToolbarPanel::insertRightItemById( const int &commandId, const int &pos, const bool doarrange )
{
	CommandHandler *handler = CommandHandler::get();
	MCommand *command = handler->getCommand( commandId );

	wxListItem item;
	item.SetId( pos );
		item.SetData( commandId );

	if ( command )
	{
		item.SetImage( imageListIds[ command->getBitmapId() ] );
		item.SetText( command->getText() );
	}
	else
	{
		item.SetImage( 0 );
		item.SetText( _( "Separator" ));
	}
	list2->InsertItem( item );

	if ( doarrange )
	{
		list2->SetItemState( pos, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list2->EnsureVisible( pos );
		list2->Arrange();
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnUp
**
******************************************************/
void ToolbarPanel::OnUp( wxCommandEvent& )
{
	if ( right_sel > 0 )
	{
		const int commandId = list2->GetItemData( right_sel );
		list2->DeleteItem( right_sel );
		insertRightItemById( commandId, right_sel - 1 );
		updateItemConfig();
		setDirty();
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnDown
**
******************************************************/
void ToolbarPanel::OnDown( wxCommandEvent& )
{
	if ( right_sel < list2->GetItemCount() - 1 )
	{
		const int commandId = list2->GetItemData( right_sel );
		list2->DeleteItem( right_sel );
		insertRightItemById( commandId, right_sel + 1 );
		updateItemConfig();
		setDirty();
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnRightSelected
**
******************************************************/
void ToolbarPanel::OnRightSelected( wxListEvent& event )
{
	right_sel = event.GetIndex();
	long l = list2->GetItemCount();
	button_up->Enable( right_sel > 0 );
	button_down->Enable( right_sel < l - 1 );
}

/*****************************************************
**
**   ToolbarPanel   ---   OnLeftSelected
**
******************************************************/
void ToolbarPanel::OnLeftSelected( wxListEvent& event )
{
	left_sel = event.GetIndex();
}

/*****************************************************
**
**   ToolbarPanel   ---   OnChoice
**
******************************************************/
void ToolbarPanel::ToolbarPanel::OnChoice( wxCommandEvent & )
{
	setDirty();
	updateUi();
}

/*****************************************************
**
**   ToolbarPanel   ---   restoreDefaults
**
******************************************************/
void ToolbarPanel::restoreDefaults()
{
	ToolbarFactory *factory = ToolbarFactory::get();

	*tconfig = ToolbarConfig();
	tconfig->mainToolbarItems = factory->getDefaultCommandIdList();
	initRightControl();
}

/*****************************************************
**
**   ToolbarPanel   ---   set_properties
**
******************************************************/
void ToolbarPanel::set_properties()
{
	const int w = 150;
	ImageProvider *ip = ImageProvider::get();

	wxListItem itemCol;
	itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
	itemCol.m_text = _( "Type" );
	list1->InsertColumn(0, itemCol);
	list1->SetColumnWidth( 0, w );

	list2->InsertColumn(0, itemCol);
	list2->SetColumnWidth( 0, w );

	button_right->SetBitmapLabel( ip->getBitmap( BM_ARROW_RIGHT ));
	button_left->SetBitmapLabel( ip->getBitmap( BM_ARROW_LEFT ));
	button_up->SetBitmapLabel( ip->getBitmap( BM_ARROW_UP ));
	button_down->SetBitmapLabel( ip->getBitmap( BM_ARROW_DOWN ));

    // begin wxGlade: ToolbarPanel::set_properties
    list1->SetMinSize(wxSize(200,250));
    button_right->SetMinSize(wxSize(32,32));
    button_right->SetToolTip(_("Move selected item to toolbar"));
    button_left->SetMinSize(wxSize(32,32));
    button_left->SetToolTip(_("Remove selected item from toolbar"));
    button_up->SetMinSize(wxSize(32,32));
    button_up->SetToolTip(_("Move selected item up"));
    button_down->SetMinSize(wxSize(32,32));
    button_down->SetToolTip(_("Move selected item down"));
    list2->SetMinSize(wxSize(200,250));
    choice_toolstyle->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   ToolbarPanel   ---   do_layout
**
******************************************************/
void ToolbarPanel::do_layout()
{
    // begin wxGlade: ToolbarPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(2, 1, 3, 3);
    wxStaticBoxSizer* sizer_options = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Toolbar Options")), wxHORIZONTAL);
    wxFlexGridSizer* sizer_listbox = new wxFlexGridSizer(1, 3, 3, 3);
    wxStaticBoxSizer* sizer_selected = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Items in Toolbar")), wxVERTICAL);
    wxFlexGridSizer* sizer_selected2 = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* sizer_buttons = new wxFlexGridSizer(4, 1, 3, 3);
    wxStaticBoxSizer* sizer_available = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Available Items")), wxVERTICAL);
    wxFlexGridSizer* sizer_available2 = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_available2->Add(list1, 1, wxALL|wxEXPAND, 3);
    sizer_available2->AddGrowableRow(0);
    sizer_available2->AddGrowableCol(0);
    sizer_available->Add(sizer_available2, 1, wxALL|wxEXPAND, 3);
    sizer_listbox->Add(sizer_available, 1, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_right, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_left, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_up, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_down, 0, wxALL|wxEXPAND, 3);
    sizer_listbox->Add(sizer_buttons, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND|wxSHAPED, 3);
    sizer_selected2->Add(list2, 1, wxALL|wxEXPAND, 3);
    sizer_selected2->AddGrowableRow(0);
    sizer_selected2->AddGrowableCol(0);
    sizer_selected->Add(sizer_selected2, 1, wxALL|wxEXPAND, 3);
    sizer_listbox->Add(sizer_selected, 1, wxALL|wxEXPAND, 3);
    sizer_listbox->AddGrowableRow(0);
    sizer_listbox->AddGrowableCol(0);
    sizer_listbox->AddGrowableCol(2);
    sizer_main->Add(sizer_listbox, 1, wxALL|wxEXPAND, 3);
    sizer_options->Add(label_toolstyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    sizer_options->Add(choice_toolstyle, 0, wxALL, 3);
    sizer_main->Add(sizer_options, 1, wxEXPAND, 0);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createToolbarPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createToolbarPanel( wxWindow *parent )
{
	return new ToolbarPanel( parent );
}

