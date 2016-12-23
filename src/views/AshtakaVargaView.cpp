/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/AshtakaVargaView.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

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

#define MAX_ASHTAKAVarga_PAGES 6

enum { ANB_REKHA, ANB_TRIKONA, ANB_EKADHIPATYA, ANB_RASIDIAGRAM, ANB_PINDADIAGRAM, ANB_TEXT };
enum { AV_NOTEBOOK = wxID_HIGHEST + 3500 };

/*************************************************//**
*
* \brief Chart grid for Ashtakavarga charts
*
******************************************************/
class AshtakaVargaChartGridWidget : public BasicWidget
{
public:
	/*****************************************************
	**
	**   AshtakaVargaChartGridWidget   ---   Constructor
	**
	******************************************************/
	AshtakaVargaChartGridWidget( wxWindow *parent, ChartProperties *chartprops, AshtakavargaExpert *expert, const int &type, const int &rows, const int &cols,
		const wxPoint& pos = wxDefaultPosition, const wxSize &size = wxDefaultSize )
			: BasicWidget( parent, chartprops, -1, pos, size ),
			expert( expert ),
			ashtakatype( type ),
			rows( rows ),
			cols( cols )
	{
		chart = new AshtakaVargaChart( chartprops );
	}

	/*****************************************************
	**
	**   AshtakaVargaChartGridWidget   ---   Destructor
	**
	******************************************************/
	~AshtakaVargaChartGridWidget()
	{
		delete chart;
	}

	/*****************************************************
	**
	**   AshtakaVargaChartGridWidget   ---   doPaint
	**
	******************************************************/
	virtual void doPaint( const wxRect&, const bool /* eraseBackground */ = true )
	{
		int nb = 0;
		SymbolProvider sp;
		Lang lang;

		assert( painter );
		wxSize size = GetVirtualSize();
		const int xstep = size.x / cols;
		const int ystep = size.y / rows;

		ObjectArray obs = PlanetList().getVedicPlanetOnlyList();
		obs.push_back( OASCENDANT );

		for ( int row = 0; row < rows; row++ )
		{
			for ( int col = 0; col < cols; col++ )
			{
				assert( nb < rows * cols );
				chart->setAscendant( expert->getRasi( OASCENDANT ));
				for ( Rasi rasi = R_ARIES; rasi <= R_PISCES; rasi++ )
				{
					if ( nb == 0 )
					{
						chart->setRasiValue( rasi, expert->getSarva( ashtakatype, rasi ));
						chart->setCenterString( _( "Sarva" ));
						chart->setGraphicSupport( false );
						chart->setCenterGraphicFormat( false );
					}
					else if ( nb > 0 && nb <= 8 )
					{
						chart->setRasiValue( rasi, expert->getItem( ashtakatype, obs[nb-1], rasi ));
						chart->setCenterGraphicFormat( config->writer->planetSymbols );
						chart->setCenterString( config->writer->planetSymbols ?  wxString( sp.getPlanetCode( obs[nb-1])) : lang.getObjectName( obs[nb-1], TF_LONG ));
						chart->setGraphicSupport( true );
					}
					else assert( false );
				}
				MRect rect( col * xstep, row * ystep, xstep, ystep );
				chart->OnDataChanged();
				chart->paint( painter, rect );
				nb++;
			}
		}
	}

protected:

	AshtakavargaExpert *expert;
	const int ashtakatype;
	const int rows, cols;

	AshtakaVargaChart *chart;

	DECLARE_CLASS( AshtakaVargaChartGridWidget )
};

IMPLEMENT_CLASS( AshtakaVargaChartGridWidget, BasicWidget )

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
		rekhagrid = new AshtakaVargaChartGridWidget( notebook, props, expert, REKHA, 3, 3 );
		notebook->AddPage( rekhagrid, _( "Rekha" ));

		// ANB_TRIKONA
		trikonagrid = new AshtakaVargaChartGridWidget( notebook, props, expert, TRIKONA, 3, 3 );
		notebook->AddPage( trikonagrid, _( "Trikona Shodana" ));

		// ANB_EKADHIPATYA
		ekadhigrid = new AshtakaVargaChartGridWidget( notebook, props, expert, EKADHI, 3, 3 );
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

		Connect( TBS_Varga, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AshtakaVargaView::OnChoiceCommand ));
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

	virtual wxString getWindowLabel( const bool shortname = false ) { return shortname ? _( "AV" ) : _( "Ashtaka Varga" ); }

	/*****************************************************
	**
	**   AshtakaVargaView   ---   supportsGraphicExport
	**
	******************************************************/
	bool supportsGraphicExport() const { return ( notebook->GetSelection() != ANB_TEXT ); }

	/*****************************************************
	**
	**   AshtakaVargaView   ---   supportsTextExport
	**
	******************************************************/
	bool supportsTextExport() const { return ( notebook->GetSelection() == ANB_TEXT ); }

	/*****************************************************
	**
	**   AshtakaVargaView   ---   supportsSkinToggle
	**
	******************************************************/
	bool supportsSkinToggle() const
	{
		switch ( notebook->GetSelection() )
		{
		case ANB_REKHA:
		case ANB_TRIKONA:
		case ANB_EKADHIPATYA:
			return true;
			break;
		case ANB_RASIDIAGRAM:
		case ANB_PINDADIAGRAM:
		case ANB_TEXT:
		default:
			return false;
		break;
		}
	}

	/*****************************************************
	**
	**   AshtakaVargaView   ---   supportsRestrictedGraphicStyleToggle
	**
	******************************************************/
	bool supportsRestrictedGraphicStyleToggle() const { return supportsSkinToggle(); }

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
		expert->write( twidget->getSheet(), config->view->showTextViewHeaders );
		twidget->OnDataChanged();

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
			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_Varga );
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
			wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_Varga );
			if ( choice_varga ) varga = (Varga)choice_varga->GetSelection();
		}
		OnDataChanged();
	}


	AshtakaVargaChartGridWidget *rekhagrid, *trikonagrid, *ekadhigrid;
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

