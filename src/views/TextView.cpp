/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/TextView.cpp
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

#include <wx/choice.h>

#include "TextView.h"
#include "ChildWindow.h"
#include "Conf.h"
#include "DialogElements.h"
#include "Document.h"
#include "maitreya.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "ToolPanel.h"

extern Config *config;

IMPLEMENT_CLASS( TextView, BasicView )

/*************************************************//**
*
*     TextView   ---   Constructor
*
******************************************************/
TextView::TextView( wxWindow *parent, ChildWindow *frame, const VIEW_ID &viewId, const bool &showToolbar )
	: BasicView( parent, frame, viewId, showToolbar )
{
	twidget = new TextWidget( this, props );
	widget = twidget;
	updateView = true;
	Connect( wxEVT_IDLE, wxIdleEventHandler( TextView::OnIdle ));
}

/*************************************************//**
*
*     TextView   ---   OnDataChanged
*
******************************************************/
void TextView::OnDataChanged()
{
	OnToolbarCommand();
	updateView = true;
}

/*************************************************//**
*
*     TextView   ---   OnIdle
*
******************************************************/
void TextView::OnIdle( wxIdleEvent& )
{
	if ( ! updateView ) return;

	assert( twidget );

	write();
	//twidget->Scroll( 0, 0 );
	twidget->OnDataChanged();
	updateView = false;
}

/*************************************************//**
*
* \brief view for various kinds of text output including toolbar controls
*
******************************************************/
class SelectableTextView : public TextView
{
	DECLARE_CLASS( SelectableTextView )
public:
	SelectableTextView( wxWindow *parent, ChildWindow *frame, const bool &toolpanel, const int &mode )
		: TextView( parent, frame, VIEW_TEXT, toolpanel )
	{
		this->mode = mode;
		dasa = D_VIMSOTTARI;
		varga = V_RASI;
		twidget->addWidgetOption( WO_MENU_FULL_OBJECT );

		initToolItems();
		OnToolbarCommand();
	}

	virtual wxString getWindowLabel( const bool /* shortname */ = false )
	{
		return TextViewModeChoice::getItemName( mode );
	} 

protected:

	/**************************************************************
	***
	**   SelectableTextView   ---   initToolItems
	***
	***************************************************************/
	void initToolItems()
	{
		if ( toolbar )
		{
			wxChoice *choice_viewmode = (wxChoice*)toolbar->FindControl( TBS_TEXTMODE );
			if ( choice_viewmode ) choice_viewmode->SetSelection( mode );

			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
			if ( choice_varga )
			{
				choice_varga->SetSelection( varga );
				choice_varga->Enable( mode == TM_ASHTAKAVARGA || mode == TM_JAIMINI );
			}

			wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
			if ( choice_dasa )
			{
				choice_dasa->SetSelection( dasa );
				choice_dasa->Enable( mode == TM_DASA || mode == TM_DASACOMPACT || mode == TM_DASALONG || mode == TM_KP );
			}
		}
	}

	/**************************************************************
	***
	**   SelectableTextView   ---   write
	***
	***************************************************************/
	virtual void write()
	{
		twidget->clearSheet();
		TextHelper helper( doc, props, twidget->getSheet() );

		const int ret = helper.writeTextAnalysis( mode, varga, dasa );
		if ( ret )
		{
			wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
			event.SetId( ret );
			event.SetEventObject( (wxWindow*)this );
			wxPostEvent( GetParent(), event );
		}
	}

	/*****************************************************
	**
	**   SelectableTextView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_viewmode = (wxChoice*)toolbar->FindControl( TBS_TEXTMODE );
			if ( choice_viewmode )
			{
				mode = choice_viewmode->GetSelection();
				config->viewprefs->textWindowPage = mode;
				frame->setTitle();
				DocumentManager::get()->documentChanged( doc );
			}

			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
			if ( choice_varga )
			{
				varga = (Varga)choice_varga->GetSelection();
				choice_varga->Enable( mode == TM_ASHTAKAVARGA || mode == TM_JAIMINI );
			}

			wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
			if ( choice_dasa )
			{
				dasa = (DasaId)choice_dasa->GetSelection();
				choice_dasa->Enable( mode == TM_DASA || mode == TM_DASACOMPACT || mode == TM_DASALONG || mode == TM_KP );
			}
		}
		updateView = true;
	}

	int mode;
	Varga varga;
	DasaId dasa;
};

IMPLEMENT_CLASS( SelectableTextView, TextView )

/**************************************************************
***
**   ViewFactory   ---   createSelectableTextView
***
***************************************************************/
BasicView *ViewFactory::createSelectableTextView( wxWindow *parent, ChildWindow *frame,
	const bool &toolpanel, const int &mode )
{
	return new SelectableTextView( parent, frame, toolpanel, mode );
}

/*************************************************//**
*
* \brief this is the main view for a chart in single view mode
*
******************************************************/
class PlanetListView : public TextView
{
	DECLARE_CLASS( PlanetListView )

public:
	PlanetListView( wxWindow *parent, ChildWindow *frame )
			: TextView( parent, frame, VIEW_PLANETLIST, false )
	{
		twidget->addWidgetOption( WO_IS_MAIN_VIEW );
		twidget->addWidgetOption( WO_MENU_FULL_OBJECT );
		twidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );

		OnDataChanged();
	}

	virtual wxString getWindowLabel( const bool shortname = false ) { return shortname ? wxT( "" ) : _( "Main Window" ); }

protected:

	/*****************************************************
	**
	**   PlanetListView   ---   write
	**
	******************************************************/
	virtual void write()
	{
		twidget->clearSheet();
		TextHelper th( doc, props, twidget->getSheet() );
		const int ret = th.writePlanets();
		//printf( "RET %d\n", ret );
		if ( ret )
		{
			wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
			event.SetId( ret );
			event.SetEventObject( (wxWindow*)this );
			wxPostEvent( GetParent(), event );
		}
	}

};

IMPLEMENT_CLASS( PlanetListView, TextView )

/**************************************************************
***
**   ViewFactory   ---   createMainView
***
***************************************************************/
BasicView *ViewFactory::createMainView( wxWindow *parent, ChildWindow *frame )
{
	return new PlanetListView( parent, frame );
}



