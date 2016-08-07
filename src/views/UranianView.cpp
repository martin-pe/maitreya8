/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/UranianView.cpp
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

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "astrobase.h"
#include "BasicView.h"
#include "Conf.h"
#include "maitreya.h"
#include "DialogElements.h"
#include "Document.h"
#include "SheetWidget.h"
#include "Lang.h"
#include "mathbase.h"
#include "ObjectFilterDialog.h"
#include "ToolPanel.h"
#include "Uranian.h"
#include "UranianHelper.h"
#ifdef USE_URANIAN_CHART
#include "UranianWidget.h"
#endif

extern Config *config;

#define MAX_URANIAN_PAGES 8
enum { UNB_ERROR = -1, UNB_BASIC = 0, UNB_MIDPOINTS, UNB_SUMS, UNB_DIFFERENCES, UNB_CLUSTERS,
	UNB_EVENT_LIST, UNB_YEARLY_PREVIEW, UNB_PREVIEWWIDGET };

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
		printf( "YEAR %d\n", year );

		//supportObjectToggle = true;

		cfg = config->uranian;
		expert = new UranianExpert( doc, props, cfg );
		helper = new UranianHelper( expert );

		initToolItems();

		// UNB_BASIC
		basicList = new SheetWidget( notebook, props );
		notebook->AddPage( basicList, _( "Basic" ));

		// UNB_MIDPOINTS
		midpointTable = new SheetWidget( notebook, props );
		notebook->AddPage( midpointTable, _( "Midpoints" ));

		// UNB_SUMS
		sumTable = new SheetWidget( notebook, props );
		notebook->AddPage( sumTable, _( "Sums" ));

		// UNB_DIFFERENCES
		differenceTable = new SheetWidget( notebook, props );
		notebook->AddPage( differenceTable, _( "Differences" ));

		// UNB_CLUSTERS
		clusterWidget = new SheetWidget( notebook, props );
		notebook->AddPage( clusterWidget, _( "Clusters" ));

		// UNB_EVENT_LIST
		eventList = new SheetWidget( notebook, props );
		notebook->AddPage( eventList, _( "Events" ));

		// UNB_YEARLY_PREVIEW
		yearlyPreviewList = new SheetWidget( notebook, props );
		notebook->AddPage( yearlyPreviewList, _( "Yearly Preview" ));

#ifdef USE_URANIAN_CHART
		// UNB_PREVIEWWIDGET
		yearlyPreviewWidget = new SheetWidget( notebook, props );
		//yearlyPreviewWidget = new YearlyPreviewWidget( notebook, props, expert, cfg );
		notebook->AddPage( yearlyPreviewWidget, _( "Preview Graphic" ));
#endif

		widget = notebook;
		int pageId = UNB_BASIC;
		if ( config->viewprefs->activePageUranian >= 0 && config->viewprefs->activePageUranian < (int)notebook->GetPageCount())
		{
			pageId = config->viewprefs->activePageUranian;
		}
		notebook->SetSelection( pageId );
		setActiveWidget( pageId );

		OnToolbarCommand(); // does it all, refresh and hide-and-seek for tool items

		Connect( wxEVT_IDLE, wxIdleEventHandler( UranianView::OnIdle ));
		Connect( UV_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( UranianView::OnNotebook ));
		Connect( CMD_FILTER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UranianView::OnFilter ));
	  Connect( TBS_YEAR, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler( UranianView::OnYearSpin ));
	}

	~UranianView()
	{
		config->viewprefs->activePageUranian = notebook->GetSelection();
		config->viewprefs->orbisRadix = 60 * cfg->orbisRadix;
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
		printf( "initToolItems\n" );
		if ( toolbar )
		{
			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) spin_orbis->SetValue( (int)(60 * cfg->orbisRadix ));

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) choice_gk->SetSelection( (int)cfg->gradkreis );

			printf( "initToolItems YEAR %d\n", year );
			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) spin_year->SetValue( year );

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) choice_sort->SetSelection( cfg->sortOrder );
		}
	}

	/*****************************************************
	**
	**   UranianView   ---   OnDataChanged
	**
	******************************************************/
	void OnDataChanged()
	{
		printf( "OnDataChanged %d\n", year );

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
		printf( "OnNotebook PAGE %d\n", notebook->GetSelection() );
		setActiveWidget( notebook->GetSelection());
		activateControls();
		updateView = true;
	}

	/*****************************************************
	**
	**   UranianView   ---   setActiveWidget
	**
	******************************************************/
	void setActiveWidget( const int &page )
	{
		switch( page )
		{
			case UNB_BASIC:
				activeWidget = basicList;
			break;
			case UNB_MIDPOINTS:
				activeWidget = midpointTable;
			break;
			case UNB_SUMS:
				activeWidget = sumTable;
			break;
			case UNB_DIFFERENCES:
				activeWidget = differenceTable;
			break;
			case UNB_CLUSTERS:
				activeWidget = clusterWidget;
			break;
			case UNB_EVENT_LIST:
				activeWidget = eventList;
			break;
			case UNB_YEARLY_PREVIEW:
				activeWidget = yearlyPreviewList;
			break;
#ifdef USE_URANIAN_CHART
			case UNB_PREVIEWWIDGET:
				activeWidget = yearlyPreviewWidget;
			break;
#endif
			default:
				assert( false );
			break;
		}

		/*
		assert( notebook->GetSelection() >= 0 && notebook->GetSelection() < MAX_URANIAN_PAGES );


		TODO
		1. DOEN alle UNB* sortieren wie auf der GUI
		2. DONE *Text umbenennen in Widget
		3. DONE activeWidget hier definieren
		4. PARTLY Preview Widget portieren nach WidgetItem
		5. Export bauen für alle Widgets -> geht auch nach BasicView oder Widget
		6. Subscriptum und Super implementieren

		switch ( notebook->GetSelection() )
		{
		}
		//if ( dirty[notebook->GetSelection()] ) updatePage( notebook->GetSelection() );
		*/
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

		const int sel = notebook->GetSelection();
		const int pageId = notebook->GetSelection();

		printf( "WRITE active page %d ID %d dirty is %d\n", sel, pageId, dirty[sel] );
		assert( pageId >= 0 && pageId < MAX_URANIAN_PAGES );

		if ( ! activeWidget )
		{
			doMessageBox( this, wxT( "UranianView: no active widget" ));
			return;
		}


		if ( dirty[pageId] )
		{
			assert( activeWidget );
			activeWidget->clearSheet();
			switch( pageId )
			{
				case UNB_BASIC:
					helper->writeRadixAnalysis( basicList->getSheet());
				break;
				case UNB_MIDPOINTS:
					helper->writeTupleTable( midpointTable->getSheet(), UTT_MIDPOINTS, PC_RADIX );
				break;
				case UNB_SUMS:
					helper->writeTupleTable( sumTable->getSheet(), UTT_SUMS, PC_RADIX );
				break;
				case UNB_DIFFERENCES:
					helper->writeTupleTable( differenceTable->getSheet(), UTT_DIFFERENCES, PC_RADIX );
				break;
				case UNB_CLUSTERS:
					helper->writeMatchingClusters( clusterWidget->getSheet(), PC_RADIX );
				break;
				case UNB_EVENT_LIST:
					helper->writeUEvents( eventList->getSheet(), PC_RADIX );
				break;
				case UNB_YEARLY_PREVIEW:
					helper->writeYearlyPreview( yearlyPreviewList->getSheet());
				break;
#ifdef USE_URANIAN_CHART
				case UNB_PREVIEWWIDGET:
					//yearlyPreviewWidget->Refresh();
					yearlyPreviewWidget->getSheet()->addItem( new YearlyPreviewWidgetItem( props, expert ));
				break;
#endif
				default:
					assert( false );
				break;
			}
			activeWidget->OnDataChanged();
			dirty[pageId] = false;
		}
		/*
		else
		{
			printf( "Active page %d (id %d) is not dirty\n", sel, pageId );
			switch( sel )
			{
#ifdef USE_URANIAN_CHART
				case UNB_PREVIEWWIDGET:
					yearlyPreviewWidget->Refresh();
				break;
#endif
				default:
				break;
			}
		}
		*/
		notebook->Refresh();
	}

	/*****************************************************
	**
	**   UranianView   ---   OnFilter
	**
	******************************************************/
	void OnFilter( wxCommandEvent& )
	{
		ObjectFilterDialog dialog( this, expert->getPlanetList( PC_RADIX ), cfg->objectFilter);
		if ( dialog.ShowModal() == wxID_OK )
		{
			cfg->objectFilter = dialog.getFilter();
			OnDataChanged();
			updateView = true;
		}
/*
		if ( configureObjectFilter( this, expert->getPlanetList( PC_RADIX ), cfg->objectFilter ))
		{
			OnDataChanged();
			updateView = true;
		}
	*/
	}

	/*****************************************************
	**
	**   UranianView   ---   OnYearSpin
	**
	******************************************************/
	void OnYearSpin( wxCommandEvent& )
	{
		printf( "YEAR SPIN\n" );

		OnToolbarCommand();
		expert->calculateYearlyPreview( year );
		dirty[UNB_YEARLY_PREVIEW] = true;

#ifdef USE_URANIAN_CHART
		dirty[UNB_PREVIEWWIDGET] = true;
#endif

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
			printf( "activateControls activateControls pageId %d\n", pageId );

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) choice_sort->Enable( pageId == UNB_BASIC || pageId == UNB_EVENT_LIST );

			toolbar->EnableTool( CMD_FILTER, pageId != UNB_MIDPOINTS && pageId != UNB_SUMS && pageId != UNB_DIFFERENCES );

			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) spin_orbis->Enable( pageId == UNB_BASIC || pageId == UNB_CLUSTERS || pageId == UNB_EVENT_LIST );

			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) spin_year->Enable( pageId == UNB_YEARLY_PREVIEW || pageId == UNB_PREVIEWWIDGET );
		}
	}

	/*****************************************************
	**
	**   UranianView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			printf( "OnToolbarCommand\n" );

			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis )
			{
				cfg->orbisRadix = spin_orbis->GetValue();
				cfg->orbisRadix /= 60;
			}

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) cfg->sortOrder = (ASPECT_SORTORDER)choice_sort->GetSelection();

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) cfg->gradkreis = choice_gk->getGradkreis();

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
	UranianConfig *cfg;
	UranianExpert *expert;
	UranianHelper *helper;
	wxNotebook *notebook;

	SheetWidget *basicList, *midpointTable, *sumTable, *differenceTable, *clusterWidget, *eventList,
		*yearlyPreviewList;
	SheetWidget *activeWidget;

#ifdef USE_URANIAN_CHART
	//YearlyPreviewWidget *yearlyPreviewWidget;
	SheetWidget *yearlyPreviewWidget;
#endif

};

IMPLEMENT_CLASS( UranianView, BasicView )

#ifdef USE_URANIAN_CHART
/**************************************************************
***
**   ViewFactory   ---   createUranianView
***
***************************************************************/
BasicView *ViewFactory::createUranianView( wxWindow *parent, ChildWindow *frame )
{
	return new UranianView( parent, frame );
}
#endif


