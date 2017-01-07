/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/YogaView.cpp
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
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "Conf.h"
#include "Document.h"
#include "Jaimini.h"
#include "TextView.h"
#include "TextWidget.h"
#include "ToolPanel.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Yoga.h"

extern Config *config;

/*************************************************//**
*
* \brief view for Yogas including text view and toolbar controls
*
******************************************************/
class YogaView : public TextView
{
	DECLARE_CLASS( YogaView )

public:

	/*****************************************************
	**
	**   YogaView   ---   Constructor
	**
	******************************************************/
	YogaView( wxWindow *parent, ChildWindow *frame )
			: TextView( parent, frame, VIEW_YOGA, true )
	{
		props->setFixedVedic();
		firsthouse = 0;
		varga = V_RASI;
		initToolItems();
		OnToolbarCommand();
	}

		virtual wxString getWindowLabel( const bool = false ) { return _( "Yogas" ); }

protected:

	/**************************************************************
	***
	**   YogaView   ---   initToolItems
	***
	***************************************************************/
	void initToolItems()
	{
		YogaExpert *expert = YogaExpert::get();
		set<wxString> v;
		set<wxString>::iterator iter;

		if ( toolbar )
		{
			expert->init();
			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_Varga );
			if ( choice_varga ) choice_varga->SetSelection( varga );

			wxChoice *choice_firsthouse = (wxChoice*)toolbar->FindControl( TBS_YOGAFIRSTHOUSE );
			if ( choice_firsthouse ) choice_firsthouse->SetSelection( firsthouse );

			wxChoice *choice_groupfilter = (wxChoice*)toolbar->FindControl( TBS_YOGAGROUPFILTER );
			if ( choice_groupfilter )
			{
				v = expert->getGroups();
				choice_groupfilter->Clear();

				choice_groupfilter->Append( _( "All Groups" ));
				for ( iter = v.begin(); iter != v.end(); iter++ ) choice_groupfilter->Append( *iter );

				choice_groupfilter->SetSelection( config->viewprefs->yogaGroupFilter );
			}

			wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
			if ( choice_sourcefilter )
			{
				v = expert->getSources();
				choice_sourcefilter->Clear();

				choice_sourcefilter->Append( _( "All Sources" ));
				for ( iter = v.begin(); iter != v.end(); iter++ ) choice_sourcefilter->Append( *iter );

				choice_sourcefilter->SetSelection( config->viewprefs->yogaSourceFilter );
			}
		}
	}

	/**************************************************************
	***
	**   YogaView   ---   write
	***
	***************************************************************/
	virtual void write()
	{
		int lagna;
		YogaExpert *expert = YogaExpert::get();
		VargaHoroscope chart( doc, varga );

		switch ( firsthouse )
		{
		case 0:
			lagna = chart.getRasi( OASCENDANT );
			break;
		case 1:
			lagna = chart.getRasi( OMOON );
			break;
		case 2:
		{
			JaiminiExpert jexpert( doc, varga );
			lagna = jexpert.calcPada( 0 );
			break;
		}
		default:
			lagna = firsthouse - 3;
			ASSERT_VALID_RASI( lagna );
			break;
		}

		twidget->clearSheet();
		expert->updateAndWrite( twidget->getSheet(), &chart, lagna, groupFilterString, sourceFilterString );
	}

	/*****************************************************
	**
	**   YogaView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_Varga );
			if ( choice_varga ) varga = (Varga)choice_varga->GetSelection();

			wxChoice *choice_firsthouse = (wxChoice*)toolbar->FindControl( TBS_YOGAFIRSTHOUSE );
			if ( choice_firsthouse ) firsthouse = choice_firsthouse->GetSelection();

			wxChoice *choice_groupfilter = (wxChoice*)toolbar->FindControl( TBS_YOGAGROUPFILTER );
			if ( choice_groupfilter )
			{
				if ( choice_groupfilter->GetSelection() > 0 ) groupFilterString = choice_groupfilter->GetStringSelection();
				else groupFilterString.Clear();

				config->viewprefs->yogaGroupFilter = choice_groupfilter->GetSelection();
			}

			wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
			if ( choice_sourcefilter )
			{
				if ( choice_sourcefilter->GetSelection() > 0 ) sourceFilterString = choice_sourcefilter->GetStringSelection();
				else sourceFilterString.Clear();

				config->viewprefs->yogaSourceFilter = choice_sourcefilter->GetSelection();
			}
		}
		updateView = true;
	}

	int firsthouse;
	wxString groupFilterString, sourceFilterString;
	Varga varga;

};

IMPLEMENT_CLASS( YogaView, TextView )

/**************************************************************
***
**   ViewFactory   ---   createYogaView
***
***************************************************************/
BasicView *ViewFactory::createYogaView( wxWindow *parent, ChildWindow *frame )
{
	return new YogaView( parent, frame );
}


