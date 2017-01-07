/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/PartnerView.cpp
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

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "Aspect.h"
#include "BasicView.h"
#include "Commands.h"
#include "Conf.h"
#include "DataSet.h"
#include "Document.h"
#include "GraphicGrid.h"
#include "Lang.h"
#include "maitreya.h"
#include "Partner.h"
#include "ObjectFilterDialog.h"
#include "Sheet.h"
#include "SplitterWidget.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "ToolPanel.h"
#include "Uranian.h"
#include "UranianHelper.h"
#include "UranianParamPanel.h"
#include "Varga.h"

extern Config *config;

// PNB = Partner Notebook
enum { PNB_SYNASTRY = 0, PNB_ASTAKOOTA, PNB_COMPOSITE, PNB_SBC, PNB_SHADVARGA, PNB_VARGA2, PNB_VARGA3, PNB_URANIAN };
enum { PV_NOTEBOOK = wxID_HIGHEST + 3500, PV_UPANEL };
#define URANIAN_VIEW_WO WO_MENU_FULL_OBJECT | WO_EXPORT_PDF | WO_EXPORT_PLAINTEXT | WO_EXPORT_HTMLTEXT | WO_EXPORT_CSVTEXT

#define MAX_PARTNER_PAGES 8

/*************************************************//**
*
* \brief view for widgets related to partner charts
*
******************************************************/
class PartnerView : public BasicView, public DocumentListener
{
	DECLARE_CLASS( PartnerView )

public:

	/*****************************************************
	**
	**   PartnerView   ---   Constructor
	**
	******************************************************/
	PartnerView( wxWindow *parent, ChildWindow *frame )
			: BasicView( parent, frame, VIEW_PARTNER, true )
	{
		h1 = h2 = (Horoscope*)NULL;
		activetext = (TextWidget*)NULL;
		activewidget = (ChartGridWidget*)NULL;
		DocumentManager::get()->subscribe( this );;

		uexpert = new UranianExpert( props );
		UranianConfig &uconfig = props->getUranianConfig();

		notebook = new wxNotebook( this, PV_NOTEBOOK );

		// PNB_SYNASTRY
		synastrysplitter = new SplitterWidget( notebook );
		synastrywidget = new ChartGridWidget( synastrysplitter, CT_PARTNER, props, 1, 1 );
		synastrywidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
		synastrywidget->addVedicChart();
		synastrywidget->addWesternChart();
		synastrywidget->getAspectExpert()->setSortOrder( uconfig.sortOrder );
		swidget = new TextWidget( synastrysplitter, props );
		swidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
		swidget->addWidgetOption( WO_MENU_FULL_OBJECT );

		synastrysplitter->SplitVertically( swidget, synastrywidget );
		notebook->AddPage( synastrysplitter, _( "Synastry" ));

		// PNB_ASTAKOOTA
		awidget = new TextWidget( notebook, props );
		notebook->AddPage( awidget, _( "Ashtakoota" ));

		// PNB_COMPOSITE
		ch = new CompositHoroscope();
		compositesplitter = new SplitterWidget( notebook );
		cwidget = new TextWidget( compositesplitter, props );
		cwidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
		cwidget->addWidgetOption( WO_MENU_FULL_OBJECT );

		compositewidget = new ChartGridWidget( compositesplitter, CT_RADIX, props, 2, 2 );
		compositewidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
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

		// PNB_URANIAN
		wxPanel *panel = new wxPanel( notebook );
		uwidget = new TextWidget( panel, props, URANIAN_VIEW_WO );
		printf( "ORBIS 1 %f\n", uconfig.orbisPartner );
		//upanel = new UranianParamPanel( panel, PV_UPANEL, props, &uconfig.orbisPartner );
		upanel = new UranianParamPanel( panel, PV_UPANEL, props, &uconfig.orbisPartner );
		wxBoxSizer* usizer = new wxBoxSizer( wxHORIZONTAL );
		usizer->Add( upanel, 0, wxALL, 3);
		usizer->Add( uwidget, 1, wxEXPAND | wxALL, 3);
		panel->SetSizer( usizer );
		usizer->Fit( panel );
		notebook->AddPage( panel, _( "Uranian" ));

		widget = notebook;
		notebook->SetSelection( config->viewprefs->activePagePartner );

		setActiveItems();
		initToolItems();
		updatePartnerChoices();
		OnDataChanged();

		if ( IS_VALID_SIZE( config->viewprefs->sizes.sPartnerWindow )) SetSize( config->viewprefs->sizes.sPartnerWindow );

		Connect( TBS_PARTNER1, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnPartnerChoice ));
		Connect( TBS_PARTNER2, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnPartnerChoice ));
		Connect( PV_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( PartnerView::OnNotebook ));
		Connect( PV_UPANEL, COMMAND_UPANEL_CHANGED, wxCommandEventHandler( PartnerView::OnGeneralCommand ));
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

		printf( "ORBIS 2 %f\n", props->getUranianConfig().orbisPartner );
		*config->uranian = props->getUranianConfig();

		delete uexpert;
		delete ch;
		DocumentManager::get()->unsubscribe( this );
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
	**   PartnerView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		//printf( "OnToolbarCommand\n" );
		OnDataChanged();
	}

	/*****************************************************
	**
	**   PartnerView   ---   OnNotebook
	**
	******************************************************/
	void OnNotebook( wxNotebookEvent& )
	{
		//printf( "ON NOTEBOOK PAGE %d\n", notebook->GetSelection() );
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
		activetext = (TextWidget*)NULL;
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
		case PNB_URANIAN:
			activetext = uwidget;
			break;
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
		//printf( "VOR UPDATE PAGE %d dirty %d\n", notebook->GetSelection(), dirty[notebook->GetSelection()] );
		if ( dirty[notebook->GetSelection()] ) updatePage( notebook->GetSelection() );
		else
		{
			if ( activewidget ) activewidget->Refresh();
		}
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
		//printf( "PartnerView::updatePage nummer %d h1 %ld h2 %ld page %ld widget %ld\n",
			//notebook->GetSelection(), (long)h1, (long)h2, (long)activetext, (long)activewidget );

		if ( activewidget )
		{
			if ( notebook->GetSelection() == PNB_COMPOSITE )
			{
				//printf( "updatePage set COMPOSITE\n" );
				assert( activewidget == compositewidget );
				activewidget->setHoroscopes( h1 && h2 && h1 != h2 ? ch : (Horoscope*)NULL, (Horoscope*)NULL );
			}
			else
			{
				//printf( "updatePage set BOTH\n" );
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
			ch->update( h1, h2 );
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
		//printf( "PartnerView   ---   OnEWToggle\n" );
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
		int sel1 = -1;
		int sel2 = -1;
		DocumentManager *docmanager = DocumentManager::get();

		wxChoice *choice_partner1 = (wxChoice*)toolbar->FindControl( TBS_PARTNER1 );
		if ( choice_partner1 ) sel1 = choice_partner1->GetSelection();

		wxChoice *choice_partner2 = (wxChoice*)toolbar->FindControl( TBS_PARTNER2 );
		if ( choice_partner2 ) sel2 = choice_partner2->GetSelection();
		if ( sel1 > -1 ) h1 = docmanager->getDocument( sel1 );
		else h1 = (Horoscope*)NULL;
		if ( sel2 > -1 ) h2 = docmanager->getDocument( sel2 );
		else h2 = (Horoscope*)NULL;
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
		int sel1 = -1;
		int sel2 = -1;
		wxString docname;
		DocumentManager *docmanager = DocumentManager::get();

		const int docnum = (int)docmanager->getDocumentCount();
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

		wxArrayString a = docmanager->getNamesArray();
		choice_partner1->Clear();
		choice_partner1->Append( a );
		choice_partner2->Clear();
		choice_partner2->Append( a );

		choice_partner1->SetSelection( sel1 );
		choice_partner2->SetSelection( sel2 );
		//printf( "sel1 %d sel2 %d\n", sel1, sel2 );
		if ( sel1 != -1 && docnum > 0 ) h1 = docmanager->getDocument( Min( sel1, docnum ));
		else h1 = (Horoscope*)NULL;
		if ( sel2 != -1 && docnum > 0 ) h2 = docmanager->getDocument( Min( sel2, docnum ));
		else h2 = (Horoscope*)NULL;
	}

	/*****************************************************
	**
	**   PartnerView   ---   notifyDocumentListChanged
	**
	******************************************************/
	virtual void notifyDocumentListChanged()
	{
		updatePartnerChoices();
		OnDataChanged();
	}

	/*****************************************************
	**
	**   PartnerView   ---   notifyDocumentChanged
	**
	******************************************************/
	virtual void notifyDocumentChanged( Document* )
	{
		OnDataChanged();
	}
	virtual void notifyDocumentSaved( Document* ) {}

	// synastry chart
	SplitterWidget *synastrysplitter;
	TextWidget *swidget;
	ChartGridWidget *synastrywidget;

	// Asta Koota
	TextWidget *awidget;

	// Composite
	SplitterWidget *compositesplitter;
	TextWidget *cwidget;
	ChartGridWidget *compositewidget;

	// Sbc
	ChartGridWidget *sbcwidget;

	// Vargas
	ShadVargaChartGridWidget *shadvargawidget;
	VargaTab2ChartGridWidget *varga2widget;
	VargaTab3ChartGridWidget *varga3widget;

	TextWidget *activetext;
	ChartGridWidget *activewidget;
	bool dirty[MAX_PARTNER_PAGES];

	Horoscope *h1, *h2;
	CompositHoroscope *ch;

	// Uranian, no splitter
	TextWidget *uwidget;

	UranianExpert *uexpert;
	UranianParamPanel *upanel;
	ObjectFilter filter;

	wxNotebook *notebook;
};

IMPLEMENT_CLASS( PartnerView, BasicView )

/**************************************************************
***
**    ViewFactory   ---   createPartnerView
***
***************************************************************/
BasicView *ViewFactory::createPartnerView( wxWindow *parent, ChildWindow *frame )
{
	return new PartnerView( parent, frame );
}


