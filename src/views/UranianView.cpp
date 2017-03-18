/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/UranianView.cpp
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

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "astrobase.h"
#include "BasicView.h"
#include "Conf.h"
#include "maitreya.h"
#include "DialogElements.h"
#include "Document.h"
#include "Lang.h"
#include "mathbase.h"
#include "ObjectFilterDialog.h"
#include "TextWidget.h"
#include "ToolPanel.h"
#include "Uranian.h"
#include "UranianHelper.h"

extern Config *config;

#define MAX_URANIAN_PAGES 8
enum { UNB_ERROR = -1, UNB_MIDPOINTS = 0, UNB_SUMS, UNB_DIFFERENCES, UNB_ANT_MIDPOINTS, UNB_REFLECTION_POINTS,
	UNB_CLUSTERS, UNB_EVENT_LIST, UNB_YEARLY_PREVIEW };

#define URANIAN_VIEW_WO WO_MENU_FULL_OBJECT | WO_EXPORT_PDF | WO_EXPORT_PLAINTEXT | WO_EXPORT_HTMLTEXT | WO_EXPORT_CSVTEXT

enum { UV_NOTEBOOK = wxID_HIGHEST + 3500 };

/*************************************************//**
*
*  view for Uranian astrology (midpoint analysis)
*
******************************************************/
class UranianView : public BasicView
{
	DECLARE_CLASS( UranianView )

public:

	UranianView( wxWindow *parent, ChildWindow *frame )
			: BasicView( parent, frame, VIEW_URANIAN, true, wxFULL_REPAINT_ON_RESIZE )
	{
		notebook = new wxNotebook( this, UV_NOTEBOOK );
		props->setFixedWestern();

		year = getCurrentYear();
		//printf( "YEAR %d\n", year );

		expert = new UranianExpert( doc, props );
		helper = new UranianHelper( expert );

		initToolItems();

		// UNB_MIDPOINTS
		midpointTable = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( midpointTable, _( "Midpoints" ));

		// UNB_SUMS
		sumTable = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( sumTable, _( "Sums" ));

		// UNB_DIFFERENCES
		differenceTable = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( differenceTable, _( "Differences" ));

		// UNB_ANT_MIDPOINTS
		antMidpointTable = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( antMidpointTable, _( "Midpoints of Antiscia" ));

		// UNB_REFLECTION_POINT
		reflectionPointTable = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( reflectionPointTable, _( "Reflection Points" ));

		// UNB_CLUSTERS
		clusterList = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( clusterList, _( "Clusters" ));

		// UNB_EVENT_LIST
		eventList = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( eventList, _( "Matchings" ));

		// UNB_YEARLY_PREVIEW
		yearlyPreviewList = new TextWidget( notebook, props, URANIAN_VIEW_WO );
		notebook->AddPage( yearlyPreviewList, _( "Yearly Preview" ));

		widget = notebook;
		int pageId = UNB_MIDPOINTS;
		if ( config->viewprefs->activePageUranian >= 0 && config->viewprefs->activePageUranian < (int)notebook->GetPageCount())
		{
			pageId = config->viewprefs->activePageUranian;
		}
		notebook->SetSelection( pageId );
		OnToolbarCommand(); // does it all, refresh and hide-and-seek for tool items

		Connect( wxEVT_IDLE, wxIdleEventHandler( UranianView::OnIdle ));
		Connect( UV_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( UranianView::OnNotebook ));
		Connect( CMD_FILTER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UranianView::OnFilter ));
	  Connect( TBS_YEAR, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler( UranianView::OnYearSpin ));
		Connect( APP_CONFIGURE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UranianView::OnConfigure ));
	}

	~UranianView()
	{

		config->viewprefs->activePageUranian = notebook->GetSelection();

		*config->uranian = props->getUranianConfig();

		delete helper;
		delete expert;
	}

	wxString getWindowLabel( const bool = false ) { return _( "Uranian" ); }

protected:

	/**************************************************************
	***
	**   UranianView   ---   initToolItems
	***
	***************************************************************/
	void initToolItems()
	{
		UranianConfig &uconfig = props->getUranianConfig();
		//printf( "initToolItems\n" );
		if ( toolbar )
		{
			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) spin_orbis->SetValue( (int)(60 * uconfig.orbisRadix ));

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) choice_gk->SetSelection( (int)uconfig.gradkreis );

			//printf( "initToolItems YEAR %d\n", year );
			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) spin_year->SetValue( year );

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) choice_sort->SetSelection( uconfig.sortOrder );
		}
	}

	/*****************************************************
	**
	**   UranianView   ---   OnDataChanged
	**
	******************************************************/
	void OnDataChanged()
	{
		//printf( "OnDataChanged %d\n", year );

		BasicView::OnDataChanged();

		expert->OnDataChanged();
		expert->calculateYearlyPreview( year );
		setAllDirty( true );
		updateView = true;
	}

	/**************************************************************
	***
	**   UranianView   ---   OnIdle
	***
	***************************************************************/
	void OnIdle( wxIdleEvent& )
	{
		if ( ! updateView ) return;

		write();
		updateView = false;
	}

	/*****************************************************
	**
	**   UranianView   ---   OnNotebook
	**
	******************************************************/
	void OnNotebook( wxNotebookEvent& )
	{
		//printf( "OnNotebook PAGE %d\n", notebook->GetSelection() );
		activateControls();
		updateView = true;
	}

	/*****************************************************
	**
	**   UranianView   ---   setAllDirty
	**
	******************************************************/
	void setAllDirty( const bool b = true )
	{
		for ( int i = 0; i < MAX_URANIAN_PAGES; i++ ) dirty[i] = b;
	}

	/*****************************************************
	**
	**   UranianView   ---   write
	**
	******************************************************/
	void write()
	{

		const int pageId = notebook->GetSelection();
		assert( pageId >= 0 && pageId < MAX_URANIAN_PAGES );

		if ( dirty[pageId] )
		{
			switch( pageId )
			{
				case UNB_MIDPOINTS:
					midpointTable->clearSheet();
					helper->writeTupleTable( midpointTable->getSheet(), UTT_MIDPOINTS, PcRadix );
					midpointTable->OnDataChanged();
				break;
				case UNB_SUMS:
					sumTable->clearSheet();
					helper->writeTupleTable( sumTable->getSheet(), UTT_SUMS, PcRadix );
					sumTable->OnDataChanged();
				break;
				case UNB_DIFFERENCES:
					differenceTable->clearSheet();
					helper->writeTupleTable( differenceTable->getSheet(), UTT_DIFFERENCES, PcRadix );
					differenceTable->OnDataChanged();
				break;
				case UNB_REFLECTION_POINTS:
					reflectionPointTable->clearSheet();
					helper->writeTupleTable( reflectionPointTable->getSheet(), UTT_REFLECTION_POINTS, PcRadix );
					reflectionPointTable->OnDataChanged();
				break;
				case UNB_ANT_MIDPOINTS:
					antMidpointTable->clearSheet();
					helper->writeTupleTable( antMidpointTable->getSheet(), UTT_ANT_MIDPOINTS, PcRadix );
					antMidpointTable->OnDataChanged();
				break;
				case UNB_CLUSTERS:
					clusterList->clearSheet();
					helper->writeMatchingClusters( clusterList->getSheet(), PcRadix );
					clusterList->OnDataChanged();
				break;
				case UNB_EVENT_LIST:
					eventList->clearSheet();
					helper->writeUEvents( eventList->getSheet(), PcRadix );
					eventList->OnDataChanged();
				break;
				case UNB_YEARLY_PREVIEW:
					yearlyPreviewList->clearSheet();
					helper->writeYearlyPreview( yearlyPreviewList->getSheet());
					yearlyPreviewList->OnDataChanged();
				break;
				default:
					assert( false );
				break;
			}
			dirty[pageId] = false;
		}
		notebook->Refresh();
	}

	/*****************************************************
	**
	**   UranianView   ---   OnConfigure
	**
	******************************************************/
	void OnConfigure( wxCommandEvent& )
	{
		wxArrayString l;
		l.Add( wxT( "Midpoints" ));
		l.Add( wxT( "Reflection Points" ));
		l.Add( wxT( "Sums" ));
		l.Add( wxT( "Differences" ));
		//l.Add( wxT( "Antiscia" ));
		//l.Add( wxT( "Triples" ));
		wxMultiChoiceDialog dialog( this, _( "Included Event Configuration" ), GUI_APP_NAME, l );

		UranianConfig &ucfg = props->getUranianConfig();
		wxArrayInt a;
		if( ucfg.eventsIncludeMidpoints )        a.Add( 0 );
		if( ucfg.eventsIncludeReflectionPoints ) a.Add( 1 );
		if( ucfg.eventsIncludeSums )             a.Add( 2 );
		if( ucfg.eventsIncludeDifferences )      a.Add( 3 );
		//if( ucfg.eventsIncludeAntiscia )         a.Add( 4 );
		//if( ucfg.eventsIncludeTriples ) a.Add( 5 );
		dialog.SetSelections( a );
		if ( IS_VALID_SIZE( config->viewprefs->sizes.sUranianConfigDialog )) dialog.SetSize( config->viewprefs->sizes.sUranianConfigDialog );

		if ( dialog.ShowModal() == wxID_OK )
		{
			a = dialog.GetSelections();
			ucfg.eventsIncludeMidpoints        = ( a.Index( 0 ) != wxNOT_FOUND );
			ucfg.eventsIncludeReflectionPoints = ( a.Index( 1 ) != wxNOT_FOUND );
			ucfg.eventsIncludeSums             = ( a.Index( 2 ) != wxNOT_FOUND );
			ucfg.eventsIncludeDifferences      = ( a.Index( 3 ) != wxNOT_FOUND );
			//ucfg.eventsIncludeAntiscia         = ( a.Index( 4 ) != wxNOT_FOUND );
			//ucfg.eventsIncludeTriples          = ( a.Index( 5 ) != wxNOT_FOUND );
			config->viewprefs->sizes.sUranianConfigDialog = dialog.GetSize();
			OnDataChanged();
			updateView = true;
		}
	}

	/*****************************************************
	**
	**   UranianView   ---   OnFilter
	**
	******************************************************/
	void OnFilter( wxCommandEvent& )
	{

		//ObjectFilterDialog dialog( this, expert->getPlanetList( PcRadix ), props->getObjectFilter());
		ObjectFilterDialog dialog( this, props->getPlanetList(), props->getObjectFilter());
		if ( dialog.ShowModal() == wxID_OK )
		{
			props->setObjectFilter( dialog.getFilter());
			OnDataChanged();
			updateView = true;
		}
	}

	/*****************************************************
	**
	**   UranianView   ---   OnYearSpin
	**
	******************************************************/
	void OnYearSpin( wxCommandEvent& )
	{
		//printf( "YEAR SPIN\n" );

		OnToolbarCommand();
		expert->calculateYearlyPreview( year );
		dirty[UNB_YEARLY_PREVIEW] = true;

		updateView = true;
	}

	/*****************************************************
	**
	**   UranianView   ---   activateControls
	**
	******************************************************/
	void activateControls()
	{
		if ( toolbar )
		{
			const int pageId = notebook->GetSelection();
			//printf( "activateControls activateControls pageId %d\n", pageId );

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) choice_sort->Enable( pageId == UNB_EVENT_LIST );

			const bool needsFilter = pageId == UNB_EVENT_LIST || pageId == UNB_CLUSTERS ||  pageId == UNB_YEARLY_PREVIEW;
			toolbar->EnableTool( CMD_FILTER, needsFilter );
			toolbar->EnableTool( APP_CONFIGURE, needsFilter );

			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) spin_orbis->Enable( pageId == UNB_CLUSTERS || pageId == UNB_EVENT_LIST );

			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) spin_year->Enable( pageId == UNB_YEARLY_PREVIEW );
		}
	}

	/*****************************************************
	**
	**   UranianView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		UranianConfig &uconfig = props->getUranianConfig();
		if ( toolbar )
		{
			//printf( "OnToolbarCommand\n" );

			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis )
			{
				uconfig.orbisRadix = spin_orbis->GetValue();
				uconfig.orbisRadix /= 60;
			}

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) uconfig.sortOrder = (ASPECT_SORTORDER)choice_sort->GetSelection();

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) uconfig.gradkreis = choice_gk->getGradkreis();

			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) year = spin_year->GetValue();
		}
		activateControls();
		OnDataChanged();
		updateView = true;
	}

	bool updateView;
	bool dirty[MAX_URANIAN_PAGES];
	int year;
	UranianExpert *expert;
	UranianHelper *helper;
	wxNotebook *notebook;

	TextWidget *midpointTable, *sumTable, *differenceTable, *antMidpointTable, *reflectionPointTable,
		*clusterList, *eventList, *yearlyPreviewList;
};

IMPLEMENT_CLASS( UranianView, BasicView )

/**************************************************************
***
**   ViewFactory   ---   createUranianView
***
***************************************************************/
BasicView *ViewFactory::createUranianView( wxWindow *parent, ChildWindow *frame )
{
	return new UranianView( parent, frame );
}


