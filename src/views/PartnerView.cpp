/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/PartnerView.cpp
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
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/toolbar.h>

#include "Aspect.h"
#include "BasicView.h"
#include "Commands.h"
#include "Conf.h"
#include "DataSet.h"
#include "DialogElements.h"
#include "Document.h"
#include "SheetWidget.h"
#include "GraphicGrid.h"
#include "guibase.h"
#include "Lang.h"
#include "maitreya.h"
#include "Partner.h"
#include "SplitterWidget.h"
#include "TextHelper.h"
#include "ToolPanel.h"

#ifdef USE_URANIAN_CHART
#include "Uranian.h"
#include "UranianHelper.h"
#endif

#include "Varga.h"

extern Config *config;

// PNB = Partner Notebook
enum { PNB_SYNASTRY = 0, PNB_ASTAKOOTA, PNB_COMPOSITE, PNB_SBC, PNB_SHADVARGA, PNB_VARGA2, PNB_VARGA3, PNB_URANIAN };
enum { PV_NOTEBOOK = wxID_HIGHEST + 3500 };

class PartnerView;

/*************************************************//**
*
* \brief manages documents for the PartnerView
*
******************************************************/
class PartnerViewDocListener : public DocumentListener
{
	DECLARE_CLASS( PartnerViewDocListener )

public:

	PartnerViewDocListener( DocumentManager *manager, PartnerView *view )
			:  DocumentListener( manager )
	{
		this->manager = manager;
		this->view = view;
		manager->addClient( this );
	}

	~PartnerViewDocListener() { manager->deleteClient( this ); }

protected:

	void updateAllDocs() {}
	void documentListChanged();

private:

	DocumentManager *manager;
	PartnerView *view;

};

IMPLEMENT_CLASS( PartnerViewDocListener, DocumentListener )

#define MAX_PARTNER_PAGES 8

/*************************************************//**
*
* \brief view for widgets related to partner charts
*
******************************************************/
class PartnerView : public BasicView
{
	DECLARE_CLASS( PartnerView )
	friend class PartnerViewDocListener;

public:

	/*****************************************************
	**
	**   PartnerView   ---   Constructor
	**
	******************************************************/
	PartnerView( wxWindow *parent, ChildWindow *frame, DocumentManager *manager )
			: BasicView( parent, frame, VIEW_PARTNER, true ),
			docmanager( manager )
	{
		h1 = h2 = (Horoscope*)NULL;
		activetext = (SheetWidget*)NULL;
		activewidget = (ChartGridWidget*)NULL;

#ifdef USE_URANIAN_CHART
		uconfig = new UranianConfig( *config->uranian );
		uexpert = new UranianExpert( props, config->uranian );
#endif

		listener = new PartnerViewDocListener( manager, this );
		notebook = new wxNotebook( this, PV_NOTEBOOK );

		// PNB_SYNASTRY
		synastrysplitter = new SplitterWidget( notebook );
		synastrywidget = new ChartGridWidget( synastrysplitter, CT_PARTNER, props, 1, 1 );
		synastrywidget->addVedicChart();
		synastrywidget->addWesternChart();
#ifdef USE_URANIAN_CHART
		synastrywidget->getAspectExpert()->setSortOrder( uconfig->sortOrder );
#endif
		swidget = new SheetWidget( synastrysplitter, props );
		swidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );

		synastrysplitter->SplitVertically( swidget, synastrywidget );
		notebook->AddPage( synastrysplitter, _( "Synastry" ));

		// PNB_ASTAKOOTA
		awidget = new SheetWidget( notebook, props );
		notebook->AddPage( awidget, _( "Ashtakoota" ));

		// PNB_COMPOSITE
		ch = new CompositHoroscope();
		compositesplitter = new SplitterWidget( notebook );
		cwidget = new SheetWidget( compositesplitter, props );
		cwidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );

		compositewidget = new ChartGridWidget( compositesplitter, CT_RADIX, props, 2, 2 );
		compositewidget->addVedicChart( V_RASI );
		compositewidget->addVedicChart( V_BHAVA );
		compositewidget->addVedicChart( V_NAVAMSA );
		compositewidget->addVedicChart( V_TRIMSAMSA );
		compositewidget->addWesternChart();
		compositesplitter->SplitVertically( cwidget, compositewidget );
		notebook->AddPage( compositesplitter, _( "Composite Chart" ));

		// PNB_SBC
		sbcwidget = new ChartGridWidget( notebook, CT_PARTNER, props );
		sbcwidget->addSarvatobhadraChart();
		notebook->AddPage( sbcwidget, _( "Sarvatobhadra" ));

		// PNB_SHADVARGA
		shadvargawidget = new ShadVargaChartGridWidget( notebook, CT_PARTNER, props );
		notebook->AddPage( shadvargawidget, _( "Shadvarga" ));

		// PNB_VARGA2
		varga2widget = new VargaTab2ChartGridWidget( notebook, CT_PARTNER, props );
		notebook->AddPage( varga2widget, _( "Varga (2)" ));

		// PNB_VARGA3
		varga3widget = new VargaTab3ChartGridWidget( notebook, CT_PARTNER, props );
		notebook->AddPage( varga3widget, _( "Varga (3)" ));

#ifdef USE_URANIAN_CHART
		// PNB_URANIAN
		uwidget = new SheetWidget( notebook, props );
		notebook->AddPage( uwidget, _( "Uranian" ));
#endif

		widget = notebook;
		notebook->SetSelection( config->viewprefs->activePagePartner );
		notebook->SetSelection( notebook->GetSelection() );
		setActiveItems();
		initToolItems();
		updatePartnerChoices();
		OnDataChanged();
		SetSize( config->viewprefs->sizes.sPartnerWindow );

		Connect( TBS_PARTNER1, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnPartnerChoice ));
		Connect( TBS_PARTNER2, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnPartnerChoice ));
#ifdef USE_URANIAN_CHART
		Connect( TBS_ORBIS, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( PartnerView::OnSpinCommand ));
		Connect( TBS_SORT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnChoiceCommand ));
		Connect( TBS_GRADKREIS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnChoiceCommand ));
		Connect( CMD_FILTER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PartnerView::OnFilter ));
#endif
		Connect( PV_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( PartnerView::OnNotebook ));
	}

	/*****************************************************
	**
	**   PartnerView   ---   Destructor
	**
	******************************************************/
	~PartnerView()
	{
		config->viewprefs->sashPartnerDouble = synastrysplitter->GetSashPosition();
		config->viewprefs->sashPartnerComposite = compositesplitter->GetSashPosition();
		config->viewprefs->activePagePartner = notebook->GetSelection();

#ifdef USE_URANIAN_CHART
		config->uranian->orbisTransit = uconfig->orbisTransit;
		config->uranian->gradkreis = uconfig->gradkreis;
		config->uranian->sortOrder = uconfig->sortOrder;

		delete uexpert;
		delete uconfig;
#endif

		delete ch;
		docmanager->deleteClient( listener );
	}

	wxString getWindowLabel( const bool shortname ) { return shortname ? _( "Partner" ) : _( "Partner View" ); }

	/*****************************************************
	**
	**   PartnerView   ---   postCreate
	**
	******************************************************/
	void postCreate()
	{
		assert( synastrysplitter );
		synastrysplitter->SetSashPosition( config->viewprefs->sashPartnerDouble );

		assert( compositesplitter );
		compositesplitter->SetSashPosition( config->viewprefs->sashPartnerComposite );
	}

protected:

	/*****************************************************
	**
	**   PartnerView   ---   initToolItems
	**
	******************************************************/
#ifdef USE_URANIAN_CHART
	virtual void initToolItems()
	{
		if ( toolbar )
		{
			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) spin_orbis->SetValue( 60 * uconfig->orbisPartner );

			wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) choice_sort->SetSelection( uconfig->sortOrder );

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) choice_gk->SetSelection( (int)uconfig->gradkreis );
		}
	}
#endif

	/*****************************************************
	**
	**   PartnerView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			setupCharts();
#ifdef USE_URANIAN_CHART
			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) uconfig->orbisPartner = (double)(spin_orbis->GetValue()) / 60.0;

			wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort )
			{
				uconfig->sortOrder = (ASPECT_SORTORDER)choice_sort->GetSelection();
				synastrywidget->getAspectExpert()->setSortOrder( uconfig->sortOrder );
			}

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) uconfig->gradkreis = choice_gk->getGradkreis();
#endif
		}
		OnDataChanged();
	}

#ifdef USE_URANIAN_CHART
	/*****************************************************
	**
	**   PartnerView   ---   OnFilter
	**
	******************************************************/
	void OnFilter( wxCommandEvent& )
	{
		if ( setupObjectFilter( this, uexpert->getPlanetList( PC_RADIX ), uconfig->objectFilter ))
		{
			OnDataChanged();
		}
	}
#endif

	/*****************************************************
	**
	**   PartnerView   ---   updateToolItems
	**
	******************************************************/
	void updateToolItems()
	{
#ifdef USE_URANIAN_CHART
		if ( ! toolbar ) return;
		const bool showExtraItems = ( notebook->GetSelection() == PNB_URANIAN );
		const bool showSortChoice = ( notebook->GetSelection() == PNB_URANIAN || ( ! isVedic() && notebook->GetSelection() == PNB_SYNASTRY ));

		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis ) spin_orbis->Enable( showExtraItems );

		wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort ) choice_sort->Enable( showSortChoice );

		GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
		if ( choice_gk ) choice_gk->Enable( showExtraItems );

		toolbar->EnableTool( CMD_FILTER, showExtraItems );
#endif
	}

	/*****************************************************
	**
	**   PartnerView   ---   OnNotebook
	**
	******************************************************/
	void OnNotebook( wxNotebookEvent& )
	{
		printf( "ON NOTEBOOK PAGE %d\n", notebook->GetSelection() );
		assert( notebook->GetSelection() >= 0 && notebook->GetSelection() < MAX_PARTNER_PAGES );
		setActiveItems();
	}

	/*****************************************************
	**
	**   PartnerView   ---   setActiveItems
	**
	******************************************************/
	void setActiveItems()
	{
		activetext = (SheetWidget*)NULL;
		activewidget = (ChartGridWidget*)NULL;

		switch ( notebook->GetSelection() )
		{
		case PNB_SYNASTRY:
			activetext = swidget;
			activewidget = synastrywidget;
			break;
		case PNB_ASTAKOOTA:
			activetext = awidget;
			break;
		case PNB_COMPOSITE:
			activetext = cwidget;
			activewidget = compositewidget;
			break;
#ifdef USE_URANIAN_CHART
		case PNB_URANIAN:
			activetext = uwidget;
			break;
#endif
		case PNB_SBC:
			activewidget = sbcwidget;
			break;
		case PNB_SHADVARGA:
			activewidget = shadvargawidget;
			break;
		case PNB_VARGA2:
			activewidget = varga2widget;
			break;
		case PNB_VARGA3:
			activewidget = varga3widget;
			break;
		default:
			assert( false );
			break;
		}
		printf( "VOR UPDATE PAGE %d dirty %d\n", notebook->GetSelection(), dirty[notebook->GetSelection()] );
		if ( dirty[notebook->GetSelection()] ) updatePage( notebook->GetSelection() );
		else
		{
			if ( activewidget ) activewidget->Refresh();
		}
		updateToolItems();
	}

	/*****************************************************
	**
	**   PartnerView   ---   setAllDirty
	**
	******************************************************/
	void setAllDirty( const bool b = true )
	{
		for ( int i = 0; i < MAX_PARTNER_PAGES; i++ ) dirty[i] = b;
	}

	/*****************************************************
	**
	**   PartnerView   ---   updatePage
	**
	******************************************************/
	void updatePage( const int &page )
	{
		assert( page >= 0 && page < MAX_PARTNER_PAGES );
		printf( "PartnerView::updatePage nummer %d h1 %ld h2 %ld page %ld widget %ld\n",
			notebook->GetSelection(), (long)h1, (long)h2, (long)activetext, (long)activewidget );

		updateToolItems();

		if ( activewidget )
		{
			if ( notebook->GetSelection() == PNB_COMPOSITE )
			{
				printf( "updatePage set COMPOSITE\n" );
				assert( activewidget == compositewidget );
				activewidget->setHoroscopes( h1 && h2 && h1 != h2 ? ch : (Horoscope*)NULL, (Horoscope*)NULL );
			}
			else
			{
				printf( "updatePage set BOTH\n" );
				assert( activewidget != compositewidget );

				// set NULL horoscopes if there is no partner context
				if ( h1 == h2 ) activewidget->setHoroscopes();
				else activewidget->setHoroscopes( h1, h2 );
			}
			activewidget->OnDataChanged();
			activewidget->Refresh();
		}
		switch ( notebook->GetSelection() )
		{
		case PNB_SYNASTRY:
		{
			swidget->clearSheet();
			if ( ! h1 || ! h2 || h1 == h2 ) writeEmptyMessage( swidget->getSheet() );
			else
			{
				TextHelper th( h1, props, swidget->getSheet() );
				th.writeSynastryReport( h2, synastrywidget->getAspectExpert() );
			}
			swidget->OnDataChanged();
		}
		break;
		case PNB_ASTAKOOTA:
		{
			awidget->clearSheet();
			if ( ! h1 || ! h2 || h1 == h2 ) writeEmptyMessage( awidget->getSheet() );
			else
			{
				AshtakootaExpert expert( h1, h2 );
				expert.update();
				//expert.write( awidget->getSheet() );
				expert.writeComprehensive( awidget->getSheet() );
			}
			awidget->OnDataChanged();
		}
		break;
		case PNB_COMPOSITE:
		{
			cwidget->clearSheet();
			if ( ! h1 || ! h2 || h1 == h2 ) writeEmptyMessage( cwidget->getSheet() );
			else
			{
				ch->dump( cwidget->getSheet(), props, config->view->showTextViewHeaders );
			}
			cwidget->OnDataChanged();
		}
		break;
#ifdef USE_URANIAN_CHART
		case PNB_URANIAN:
		{
			uwidget->clearSheet();
			if ( ! h1 || ! h2 || h1 == h2 ) writeEmptyMessage( uwidget->getSheet() );
			else
			{
				uexpert->calculatePartner( h1, h2 );
				UranianHelper( uexpert ).writePartner( uwidget->getSheet() );
			}
			uwidget->OnDataChanged();
		}
		break;
#endif
		default:
				//assert( false );
		break;
		}
		dirty[page] = false;
	}

	/*****************************************************
	**
	**   PartnerView   ---   OnDataChanged
	**
	******************************************************/
	virtual void OnDataChanged()
	{
		setAllDirty();
		if ( h1 && h2 && h1 != h2 )
		{
			printf( "Fall 1 h1 %ld h2 %ld\n", (long)h1, (long)h2 );
			ch->update( h1, h2 );
		}
		else
		{
			printf( "Fall 2 h1 %ld h2 %ld\n", (long)h1, (long)h2 );
		}
		updatePage( notebook->GetSelection() );
	}

	/*****************************************************
	**
	**   PartnerView   ---   writeEmptyMessage
	**
	******************************************************/
	void writeEmptyMessage( Sheet *sheet )
	{
		if ( ! h1 && ! h2 ) 
		{
			sheet->addParagraph( _( "No chart opened" ));
		}
		else if ( ! h1 || ! h2 || h1 == h2 ) 
		{
			sheet->addParagraph( _( "Charts identical" ));
		}
	}

	/*****************************************************
	**
	**   PartnerView   ---   OnEWToggle
	**
	******************************************************/
	virtual void OnEWToggle( const bool& )
	{
		// maybe obsolet
		printf( "PartnerView   ---   OnEWToggle\n" );
		OnDataChanged();
		//setAllDirty();
	}

	/*****************************************************
	**
	**   PartnerView   ---   setupCharts
	**
	******************************************************/
	void setupCharts()
	{
		int sel1 = -1, sel2 = -1;

		wxChoice *choice_partner1 = (wxChoice*)toolbar->FindControl( TBS_PARTNER1 );
		if ( choice_partner1 ) sel1 = choice_partner1->GetSelection();

		wxChoice *choice_partner2 = (wxChoice*)toolbar->FindControl( TBS_PARTNER2 );
		if ( choice_partner2 ) sel2 = choice_partner2->GetSelection();
		if ( sel1 > -1 ) h1 = docmanager->getDocument( sel1 );
		else h1 = 0;
		if ( sel2 > -1 ) h2 = docmanager->getDocument( sel2 );
		else h2 = 0;
	}

	/*****************************************************
	**
	**   PartnerView   ---   OnPartnerChoice
	**
	******************************************************/
	void OnPartnerChoice( wxCommandEvent& )
	{
		setupCharts();
		OnDataChanged();
	}

	/*****************************************************
	**
	**   PartnerView   ---   updatePartnerChoices
	**
	******************************************************/
	void updatePartnerChoices()
	{
		if ( ! toolbar ) return;
		int sel1 = -1, sel2 = -1;
		wxString docname;

		int docnum = docmanager->getNbDocuments();
		wxChoice *choice_partner1 = (wxChoice*)toolbar->FindControl( TBS_PARTNER1 );
		wxChoice *choice_partner2 = (wxChoice*)toolbar->FindControl( TBS_PARTNER2 );

		if ( ! choice_partner1 || ! choice_partner2 )
		{
			printf( "WARN: updatePartnerChoices choices are NULL\n" );
			return;
		}
		if ( choice_partner1 ) sel1 = choice_partner1->GetSelection();

		if ( choice_partner2 ) sel2 = choice_partner2->GetSelection();
		if ( sel2 >= docnum ) sel2 = sel1;

		if ( sel1 == -1 && docnum > 0 ) sel1 = 0;
		if ( ( sel2 == sel1 || sel2 == -1 ) && docnum > 0 ) sel2 = docnum - 1;

		choice_partner1->Clear();
		choice_partner2->Clear();

		int i = 1;
		for ( i = 0; i < docnum; i++ )
		{
			docname = docmanager->getDocument( i )->getHName();
			choice_partner1->Append( docname );
			choice_partner2->Append( docname );
		}
		choice_partner1->SetSelection( sel1 );
		choice_partner2->SetSelection( sel2 );
		if ( sel1 != -1 && docnum > 0 ) h1 = docmanager->getDocument( Min( sel1, docnum ));
		else h1 = 0;
		if ( sel2 != -1 && docnum > 0 ) h2 = docmanager->getDocument( Min( sel2, docnum ));
		else h2 = 0;
	}

	/*****************************************************
	**
	**   PartnerView   ---   documentListChanged
	**
	******************************************************/
	void documentListChanged()
	{
		updatePartnerChoices();
		OnDataChanged();
	}

	// synastry chart
	SplitterWidget *synastrysplitter;
	SheetWidget *swidget;
	ChartGridWidget *synastrywidget;

	// Vedic
	SheetWidget *awidget;

	// Composite
	SplitterWidget *compositesplitter;
	SheetWidget *cwidget;
	ChartGridWidget *compositewidget;

	// Sbc
	ChartGridWidget *sbcwidget;

	// Vargas
	ShadVargaChartGridWidget *shadvargawidget;
	VargaTab2ChartGridWidget *varga2widget;
	VargaTab3ChartGridWidget *varga3widget;

	SheetWidget *activetext;
	//PartnerViewBaseTextWidget *activetext;
	//BasicWidget *activewidget;
	ChartGridWidget *activewidget;
	bool dirty[MAX_PARTNER_PAGES];

	Horoscope *h1, *h2;
	CompositHoroscope *ch;

#ifdef USE_URANIAN_CHART
	// Uranian, no splitter
	SheetWidget *uwidget;

	UranianConfig *uconfig;
	UranianExpert *uexpert;
#endif

	DocumentManager *docmanager;
	PartnerViewDocListener *listener;
	wxNotebook *notebook;
};

IMPLEMENT_CLASS( PartnerView, BasicView )

void PartnerViewDocListener::documentListChanged() { view->documentListChanged(); }

/**************************************************************
***
**    ViewFactory   ---   createPartnerView
***
***************************************************************/
BasicView *ViewFactory::createPartnerView( wxWindow *parent, ChildWindow *frame, DocumentManager *manager )
{
	return new PartnerView( parent, frame, manager );
}


