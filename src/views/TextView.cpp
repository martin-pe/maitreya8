/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/TextView.cpp
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

#include <wx/choice.h>

#include "SheetView.h"
#include "Conf.h"
#include "DialogElements.h"
#include "maitreya.h"
#include "Document.h"
#include "TextHelper.h"
#include "ToolPanel.h"

#include "ChildWindow.h"
extern Config *config;

//#define WO_BUNDLE_SELECTABLETEXTVIEW  ( WO_MENU_FULL_OBJECT ) | ( WO_EXPORT_ALL )
//#define WO_BUNDLE_PLANETLISTVIEW ( WO_IS_MAIN_VIEW ) | ( WO_MENU_FULL_OBJECT ) | ( WO_EXPORT_ALL ) | ( WO_SUPPORTS_EW_TOGGLE )
#define WO_BUNDLE_SELECTABLETEXTVIEW  ( WO_MENU_FULL_OBJECT )
#define WO_BUNDLE_PLANETLISTVIEW ( WO_IS_MAIN_VIEW ) | ( WO_MENU_FULL_OBJECT ) | ( WO_SUPPORTS_EW_TOGGLE )

/*************************************************//**
*
* \brief view for various kinds of text output including toolbar controls
*
******************************************************/
class SelectableTextView : public SheetView
{
	DECLARE_CLASS( SelectableTextView )
public:
	SelectableTextView( wxWindow *parent, ChildWindow *frame, const bool &toolpanel, const int &mode )
		: SheetView( parent, frame, VIEW_TEXT, toolpanel )
	{
		this->mode = mode;
		dasa = D_VIMSOTTARI;
		varga = V_RASI;
		twidget->setWidgetOptions( WO_BUNDLE_SELECTABLETEXTVIEW );

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

			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_Varga );
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
		twidget->enableFloatingLayout( mode != TM_DASALONG && mode != TM_JAIMINI && mode != TM_KP );
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
				doc->emitDocUpdatedEvent();
			}

			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_Varga );
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

IMPLEMENT_CLASS( SelectableTextView, SheetView )

/**************************************************************
***
**   ViewFactory   ---   createSelectableTextView
***
***************************************************************/
BasicView *ViewFactory::createSelectableTextView( wxWindow *parent, ChildWindow *frame, const bool &toolpanel, const int &mode )
{
	return new SelectableTextView( parent, frame, toolpanel, mode );
}

/*************************************************//**
*
* \brief this is the main view for a chart in single view mode
*
******************************************************/
class PlanetListView : public SheetView
{
	DECLARE_CLASS( PlanetListView )

public:
	PlanetListView( wxWindow *parent, ChildWindow *frame )
			: SheetView( parent, frame, VIEW_PLANETLIST, false )
	{
		twidget->setWidgetOptions( WO_BUNDLE_PLANETLISTVIEW );

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
		printf( "RET %d\n", ret );
		if ( ret )
		{
			wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
			event.SetId( ret );
			event.SetEventObject( (wxWindow*)this );
			wxPostEvent( GetParent(), event );
		}
	}

};

IMPLEMENT_CLASS( PlanetListView, SheetView )

/**************************************************************
***
**   ViewFactory   ---   createMainView
***
***************************************************************/
BasicView *ViewFactory::createMainView( wxWindow *parent, ChildWindow *frame )
{
	return new PlanetListView( parent, frame );
}



