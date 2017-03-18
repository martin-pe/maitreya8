/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/AshtakaVargaView.cpp
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
#include <wx/notebook.h>
#include <wx/toolbar.h>

#include "Ashtakavarga.h"
#include "BasicView.h"
#include "Commands.h"
#include "Conf.h"
#include "BarDiagram.h"
#include "Document.h"
#include "GraphicGrid.h"
#include "Lang.h"
#include "Painter.h"
#include "PlanetList.h"
#include "SheetWidget.h"
#include "SymbolProvider.h"
#include "TextWidget.h"
#include "ToolPanel.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicChart.h"

extern Config *config;

#define WO_BUNDLE_AVGRID WO_MENU_RESTRICTED_GRAPHIC_STYLE | WO_MENU_SKIN | WO_EXPORT_GRAPHIC

enum { ANB_REKHA, ANB_TRIKONA, ANB_EKADHIPATYA, ANB_RASIDIAGRAM, ANB_PINDADIAGRAM, ANB_TEXT };
enum { AV_NOTEBOOK = wxID_HIGHEST + 3500 };

/*************************************************//**
*
* \brief Chart grid for Ashtakavarga charts
*
******************************************************/
class AshtakaVargaGridWidget : public ChartGridWidget
{
public:

	AshtakaVargaGridWidget( wxWindow *parent, ChartProperties *chartprops, AshtakavargaExpert *expert, const int &type )
		: ChartGridWidget( parent, CT_RADIX, chartprops, 3, 3 ),
		expert( expert ),
		ashtakatype( type )
	{
		for( int i = 0; i < 9; i++ ) addAshtakavargaChart();
		setWidgetOptions( WO_BUNDLE_AVGRID );

		obs = PlanetList().getVedicPlanetOnlyList();
		obs.push_back( OASCENDANT );
	}

	/*****************************************************
	**
	**   AshtakaVargaGridWidget   ---   OnDataChanged
	**
	******************************************************/
	void OnDataChanged()
	{
		//printf( "AshtakaVargaGridWidget::OnDataChanged\n" );
		SymbolProvider sp;
		Lang lang;

		for ( uint i = 0; i < charts.size(); i++ )
		{
			// dynamic cast  would be better
			AshtakaVargaChart *chart = (AshtakaVargaChart*)charts[i];

			chart->setAscendant( expert->getRasi( OASCENDANT ));
			for ( Rasi rasi = R_ARIES; rasi <= R_PISCES; rasi++ )
			{
				if ( i == 0 )
				{
					chart->setRasiValue( rasi, expert->getSarva( ashtakatype, rasi ));

					// Sarva can have high numbers, so the bindhu etc. mode is switched off here
					chart->setGraphicSupport( false );
					chart->setCenterString( _( "Sarva" ));
					chart->setCenterGraphicFormat( false );
				}
				else if ( i <= 8 )
				{
					chart->setRasiValue( rasi, expert->getItem( ashtakatype, obs[i-1], rasi ));

					// cosmetic
					chart->setCenterGraphicFormat( config->writer->planetSymbols );
					chart->setCenterString( config->writer->planetSymbols ?  wxString( sp.getPlanetCode( obs[i-1])) : lang.getObjectName( obs[i-1], TF_LONG ));
					chart->setGraphicSupport( true );
				}
				else assert( false );
				chart->OnDataChanged();
			}
		}
		Refresh();
	}


private:

	AshtakavargaExpert *expert;
	const int ashtakatype;
	ObjectArray obs;

};

/*************************************************//**
*
* \brief view containing Ashtaka Varga charts, diagrams and text view
*
******************************************************/
class AshtakaVargaView : public BasicView
{
	DECLARE_CLASS( AshtakaVargaView )

public:

	/*****************************************************
	**
	**   AshtakaVargaView   ---   Constructor
	**
	******************************************************/
	AshtakaVargaView( wxWindow *parent, ChildWindow *frame )
			: BasicView( parent, frame, VIEW_ASHTAKAVARGA, true )
	{
		props->setFixedVedic();
		varga = V_RASI;

		expert = new AshtakavargaExpert( doc, varga );

		notebook = new wxNotebook( this, AV_NOTEBOOK );

		//  ANB_REKHA
		rekhagrid = new AshtakaVargaGridWidget( notebook, props, expert, REKHA );
		notebook->AddPage( rekhagrid, _( "Rekha" ));

		// ANB_TRIKONA
		trikonagrid = new AshtakaVargaGridWidget( notebook, props, expert, TRIKONA );
		notebook->AddPage( trikonagrid, _( "Trikona Shodana" ));

		// ANB_EKADHIPATYA
		ekadhigrid = new AshtakaVargaGridWidget( notebook, props, expert, EKADHI );
		notebook->AddPage( ekadhigrid, _( "Ekadhipatya Shodana" ));
		
		// ANB_RASIDIAGRAM
		rasiwidget = new SheetWidget( notebook, props );
		notebook->AddPage( rasiwidget, _( "Rasi Diagram" ));

		// ANB_PINDADIAGRAM
		pindawidget = new SheetWidget( notebook, props );
		notebook->AddPage( pindawidget, _( "Pinda Diagram" ));

		// ANB_TEXT
		twidget = new TextWidget( notebook, props );
		notebook->AddPage( twidget, _( "Text" ));

		notebook->SetSelection( config->viewprefs->activePageAshtakaVarga );
		widget = notebook;
		initToolItems();
		OnDataChanged();

		Connect( TBS_VARGA, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AshtakaVargaView::OnChoiceCommand ));
	}

	/*****************************************************
	**
	**   AshtakaVargaView   ---   Destructor
	**
	******************************************************/
	~AshtakaVargaView()
	{
		config->viewprefs->activePageAshtakaVarga = notebook->GetSelection();
		delete expert;
	}

	virtual wxString getWindowLabel( const bool shortname = false ) { return shortname ? _( "AV" ) : _( "Ashtakavarga" ); }

	/*****************************************************
	**
	**   AshtakaVargaView   ---   OnDataChanged
	**
	******************************************************/
	void OnDataChanged()
	{
		expert->setVarga( varga );
		expert->update();
		twidget->clearSheet();

		rekhagrid->OnDataChanged();
		trikonagrid->OnDataChanged();
		ekadhigrid->OnDataChanged();

		// Rasi widget
		rasiwidget->clearSheet();
		Sheet *rasisheet = rasiwidget->getSheet();
		BarDiagramWriter rwriter( rasisheet, props, doc );
		rwriter.writeAvRasiDiagrams( expert );
		rasiwidget->OnDataChanged();

		// Pinda widget
		pindawidget->clearSheet();
		Sheet *pindasheet = pindawidget->getSheet();
		BarDiagramWriter pwriter( pindasheet, props, doc );
		pwriter.writeAvPindaDiagrams( expert );
		pindawidget->OnDataChanged();

		expert->write( twidget->getSheet(), config->view->showTextViewHeaders );
		twidget->OnDataChanged();

		notebook->GetCurrentPage()->Refresh();
	}

	/*****************************************************
	**
	**   AshtakaVargaView   ---   postCreate
	**
	******************************************************/
	void postCreate()
	{
		//notebook->SetSelection( config->viewprefs->activePageAshtakaVarga );
	}

protected:

	/**************************************************************
	***
	**   AshtakaVargaView   ---   initToolItems
	***
	***************************************************************/
	void initToolItems()
	{
		if ( toolbar )
		{
			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
			if ( choice_varga ) choice_varga->SetSelection( varga );
		}
	}

	/*****************************************************
	**
	**   AshtakaVargaView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
			if ( choice_varga ) varga = (Varga)choice_varga->GetSelection();
		}
		OnDataChanged();
	}


	AshtakaVargaGridWidget *rekhagrid, *trikonagrid, *ekadhigrid;
	TextWidget *twidget;
	SheetWidget *pindawidget;
	SheetWidget *rasiwidget;

	AshtakavargaExpert *expert;
	Varga varga;
	wxNotebook *notebook;
};

IMPLEMENT_CLASS( AshtakaVargaView, BasicView )


/*****************************************************
**
**   ViewFactory  ---   createAshtakaVargaView
**
******************************************************/
BasicView *ViewFactory::createAshtakaVargaView( wxWindow *parent, ChildWindow *frame )
{
	return new AshtakaVargaView( parent, frame );
}

