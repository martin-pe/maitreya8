/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/ShadbalaView.cpp
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

#include <wx/event.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/statbox.h>

#include "maitreya.h"
#include "BasicView.h"
#include "Conf.h"
#include "BarDiagram.h"
#include "Document.h"
#include "SheetWidget.h"
#include "GraphicGrid.h"
#include "maitreya.h"
#include "Painter.h"
#include "PlanetList.h"
#include "ShadBala.h"
#include "TextWidget.h"
#include "VedicChart.h"

extern Config *config;

enum { SB_OVERVIEW, SB_STHANA, SB_SAPTA, SB_KALA, SB_SUMMARY, SB_TEXT };

/*************************************************//**
*
* \brief view containing diagrams and text output for Shad Bala
*
******************************************************/
class ShadbalaView : public BasicView
{
	DECLARE_CLASS( ShadbalaView )

public:

	/*****************************************************
	**
	**   ShadbalaView   ---   Constructor
	**
	******************************************************/
	ShadbalaView( wxWindow *parent, ChildWindow *frame )
			: BasicView( parent, frame, VIEW_SHADBALA, false )
	{
		props->setFixedVedic();
		notebook = new wxNotebook( this, -1 );
		expert = new ShadBalaExpert( doc );

		overviewWidget = new SheetWidget( notebook, props );
		notebook->AddPage( overviewWidget, _( "6 Balas" ));

		stbWidget = new SheetWidget( notebook, props );
		notebook->AddPage( stbWidget, _( "Sthana Bala" ));

		saptaWidget = new SheetWidget( notebook, props );
		notebook->AddPage( saptaWidget, _( "Saptavargaja Bala" ));

		kalaWidget = new SheetWidget( notebook, props );
		notebook->AddPage( kalaWidget, _( "Kala Bala" ));

		summaryWidget = new SheetWidget( notebook, props );
		notebook->AddPage( summaryWidget, _( "Summary" ));

		theText = new TextWidget( notebook, props );
		notebook->AddPage( theText, _( "Text" ));

		notebook->SetSelection( config->viewprefs->activePageShadbala );
		widget = notebook;
		OnDataChanged();
	}

	/*****************************************************
	**
	**   ShadbalaView   ---   Destructor
	**
	******************************************************/
	~ShadbalaView()
	{
		delete expert;
		config->viewprefs->activePageShadbala = notebook->GetSelection();
	}

	/*****************************************************
	**
	**   ShadbalaView   ---   postCreate
	**
	******************************************************/
	void postCreate()
	{
		//notebook->SetSelection( config->viewprefs->activePageShadbala );
	}

	virtual wxString getWindowLabel( const bool = false ) { return _( "Shadbala" ); }


	bool supportsGraphicExport() const { return ( notebook->GetSelection() != SB_TEXT ); }

	bool supportsTextExport() const { return ( notebook->GetSelection() == SB_TEXT ); }

	/*****************************************************
	**
	**   ShadbalaView   ---   OnDataChanged
	**
	******************************************************/
	void OnDataChanged()
	{
		uint i;
		const ObjectArray &obs = PlanetList().getVedicPlanetOnlyList();

		expert->updateAllBalas();
		writeText();

		// Summary
		summaryWidget->clearSheet();
		{
			SheetWidgetGrid *summaryGrid = new SheetWidgetGrid( 2, .4, 400, 200, 1000, 800 );
			BarDiagramWidget *diagram1 = new BarDiagramWidget( props, _( "Shad Bala Summary" ));
			for ( i = 0; i < obs.size(); i++ ) diagram1->addItem( new BarDiagramItem( obs[i], expert->getShadBala( obs[i] )));
			summaryGrid->addItem( diagram1 );

			BarDiagramWidget *diagram2 = new BarDiagramWidget( props, _( "Ratio (Percent)" ));
			for ( i = 0; i < obs.size(); i++ ) diagram2->addItem( new BarDiagramItem( obs[i], expert->getPercentualShadBala( obs[i] )));
			summaryGrid->addItem( diagram2 );
			summaryWidget->getSheet()->addItem( summaryGrid );
		}
		summaryWidget->OnDataChanged();

		// Overview
		overviewWidget->clearSheet();
		{
			SheetWidgetGrid *overviewGrid = new SheetWidgetGrid( 3 );

			BarDiagramWidget *diagram3 = new BarDiagramWidget( props, _( "Sthaana" ));
			for ( i = 0; i < obs.size(); i++ ) diagram3->addItem( new BarDiagramItem( obs[i], expert->getSthaanaBala( obs[i] )));
			overviewGrid->addItem( diagram3 );

			BarDiagramWidget *diagram4 = new BarDiagramWidget( props, _( "Dig" ));
			for ( i = 0; i < obs.size(); i++ ) diagram4->addItem( new BarDiagramItem( obs[i], expert->getDigBala( obs[i] )));
			overviewGrid->addItem( diagram4 );

			BarDiagramWidget *diagram5 = new BarDiagramWidget( props, _( "Kala" ));
			for ( i = 0; i < obs.size(); i++ ) diagram5->addItem( new BarDiagramItem( obs[i], expert->getKalaBala( obs[i] )));
			overviewGrid->addItem( diagram5 );

			BarDiagramWidget *diagram6 = new BarDiagramWidget( props, _( "Cheshta" ));
			for ( i = 0; i < obs.size(); i++ ) diagram6->addItem( new BarDiagramItem( obs[i], expert->getCheshtaBala( obs[i] )));
			overviewGrid->addItem( diagram6 );

			BarDiagramWidget *diagram7 = new BarDiagramWidget( props, _( "Naisargika" ));
			for ( i = 0; i < obs.size(); i++ ) diagram7->addItem( new BarDiagramItem( obs[i], expert->getNaisargikaBala( obs[i] )));
			overviewGrid->addItem( diagram7 );

			BarDiagramWidget *diagram8 = new BarDiagramWidget( props, _( "Drik" ));
			for ( i = 0; i < obs.size(); i++ ) diagram8->addItem( new BarDiagramItem( obs[i], expert->getDrikBala( obs[i] )));
			overviewGrid->addItem( diagram8 );
			overviewWidget->getSheet()->addItem( overviewGrid );
		}
		overviewWidget->OnDataChanged();

		// Sthaana Bala
		stbWidget->clearSheet();
		{
			SheetWidgetGrid *stbGrid = new SheetWidgetGrid( 3 );

			BarDiagramWidget *diagram9 = new BarDiagramWidget( props, _( "Sthaana Bala Summary" ));
			for ( i = 0; i < obs.size(); i++ ) diagram9->addItem( new BarDiagramItem( obs[i], expert->getSthaanaBala( obs[i] )));
			stbGrid->addItem( diagram9 );

			BarDiagramWidget *diagram10 = new BarDiagramWidget( props, _( "Uchcha" ));
			for ( i = 0; i < obs.size(); i++ ) diagram10->addItem( new BarDiagramItem( obs[i], expert->getUchchaBala( obs[i] )));
			stbGrid->addItem( diagram10 );

			BarDiagramWidget *diagram11 = new BarDiagramWidget( props, _( "Saptavargaja" ));
			for ( i = 0; i < obs.size(); i++ ) diagram11->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i] )));
			stbGrid->addItem( diagram11 );

			BarDiagramWidget *diagram12 = new BarDiagramWidget( props, _( "Ojhajugma" ));
			for ( i = 0; i < obs.size(); i++ ) diagram12->addItem( new BarDiagramItem( obs[i], expert->getOjhajugmaBala( obs[i] )));
			stbGrid->addItem( diagram12 );

			BarDiagramWidget *diagram13 = new BarDiagramWidget( props, _( "Kendradi" ));
			for ( i = 0; i < obs.size(); i++ ) diagram13->addItem( new BarDiagramItem( obs[i], expert->getKendradiBala( obs[i] )));
			stbGrid->addItem( diagram13 );

			BarDiagramWidget *diagram14 = new BarDiagramWidget( props, _( "Drekkana" ));
			for ( i = 0; i < obs.size(); i++ ) diagram14->addItem( new BarDiagramItem( obs[i], expert->getDrekkanaBala( obs[i] )));
			stbGrid->addItem( diagram14 );
			stbWidget->getSheet()->addItem( stbGrid );
		}
		stbWidget->OnDataChanged();

		// Saptavargaja Bala
		saptaWidget->clearSheet();
		{
			SheetWidgetGrid *saptaGrid = new SheetWidgetGrid( 3 );
			BarDiagramWidget *diagram15 = new BarDiagramWidget( props, _( "Saptavargaja Summary" ));
			for ( i = 0; i < obs.size(); i++ ) diagram15->addItem( new BarDiagramItem( obs[i], expert->getSthaanaBala( obs[i] )));
			saptaGrid->addItem( diagram15 );

			BarDiagramWidget *diagram16 = new BarDiagramWidget( props, _( "D-1" ));
			for ( i = 0; i < obs.size(); i++ ) diagram16->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 0 )));
			saptaGrid->addItem( diagram16 );

			BarDiagramWidget *diagram17 = new BarDiagramWidget( props, _( "D-8" ));
			for ( i = 0; i < obs.size(); i++ ) diagram17->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 1 )));
			saptaGrid->addItem( diagram17 );

			BarDiagramWidget *diagram18 = new BarDiagramWidget( props, _( "D-2" ));
			for ( i = 0; i < obs.size(); i++ ) diagram18->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 2 )));
			saptaGrid->addItem( diagram18 );

			BarDiagramWidget *diagram19 = new BarDiagramWidget( props, _( "D-3" ));
			for ( i = 0; i < obs.size(); i++ ) diagram19->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 3 )));
			saptaGrid->addItem( diagram19 );

			BarDiagramWidget *diagram20 = new BarDiagramWidget( props, _( "D-7" ));
			for ( i = 0; i < obs.size(); i++ ) diagram20->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 4 )));
			saptaGrid->addItem( diagram20 );

			BarDiagramWidget *diagram21 = new BarDiagramWidget( props, _( "D-12" ));
			for ( i = 0; i < obs.size(); i++ ) diagram21->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 5 )));
			saptaGrid->addItem( diagram21 );

			BarDiagramWidget *diagram22 = new BarDiagramWidget( props, _( "D-30" ));
			for ( i = 0; i < obs.size(); i++ ) diagram22->addItem( new BarDiagramItem( obs[i], expert->getSaptavargajaBala( obs[i], 6 )));
			saptaGrid->addItem( diagram22 );
			saptaWidget->getSheet()->addItem( saptaGrid );
		}
		saptaWidget->OnDataChanged();

		// Kala Bala
		kalaWidget->clearSheet();
		{
			SheetWidgetGrid *kalaGrid = new SheetWidgetGrid( 3 );
			BarDiagramWidget *diagram23 = new BarDiagramWidget( props, _( "Kala Bala Summary" ));
			for ( i = 0; i < obs.size(); i++ ) diagram23->addItem( new BarDiagramItem( obs[i], expert->getKalaBala( obs[i] )));
			kalaGrid->addItem( diagram23 );

			BarDiagramWidget *diagram24 = new BarDiagramWidget( props, _( "Nathonatha" ));
			for ( i = 0; i < obs.size(); i++ ) diagram24->addItem( new BarDiagramItem( obs[i], expert->getNathonathaBala( obs[i] )));
			kalaGrid->addItem( diagram24 );

			BarDiagramWidget *diagram25 = new BarDiagramWidget( props, _( "Paksha" ));
			for ( i = 0; i < obs.size(); i++ ) diagram25->addItem( new BarDiagramItem( obs[i], expert->getPakshaBala( obs[i] )));
			kalaGrid->addItem( diagram25 );

			BarDiagramWidget *diagram26 = new BarDiagramWidget( props, _( "Tribhaga" ));
			for ( i = 0; i < obs.size(); i++ ) diagram26->addItem( new BarDiagramItem( obs[i], expert->getTribhagaBala( obs[i] )));
			kalaGrid->addItem( diagram26 );

			BarDiagramWidget *diagram27 = new BarDiagramWidget( props, _( "Varsha-Masa-Dina-Hora" ));
			for ( i = 0; i < obs.size(); i++ ) diagram27->addItem( new BarDiagramItem( obs[i], expert->getVarshaMasaDinaHoraBala( obs[i] )));
			kalaGrid->addItem( diagram27 );

			BarDiagramWidget *diagram28 = new BarDiagramWidget( props, _( "Ayana" ));
			for ( i = 0; i < obs.size(); i++ ) diagram28->addItem( new BarDiagramItem( obs[i], expert->getAyanaBala( obs[i] )));
			kalaGrid->addItem( diagram28 );

			BarDiagramWidget *diagram29 = new BarDiagramWidget( props, _( "Yudhdha" ));
			for ( i = 0; i < obs.size(); i++ ) diagram29->addItem( new BarDiagramItem( obs[i], expert->getYudhdhaBala( obs[i] )));
			kalaGrid->addItem( diagram29 );
			kalaWidget->getSheet()->addItem( kalaGrid );
		}
		kalaWidget->OnDataChanged();

		notebook->GetCurrentPage()->Refresh();
	}

protected:

	/*****************************************************
	**
	**   ShadbalaView   ---   OnSize
	**
	******************************************************/
	virtual void OnSize( wxSizeEvent &event )
	{
		//printf( "VargaGridView::OnSize x %d y %d\n", event.GetSize().x, event.GetSize().y );

		// TODO: no update without that line
		notebook->GetCurrentPage()->Refresh();
		BasicView::OnSize( event );
	}

	/*****************************************************
	**
	**   ShadbalaView   ---   writeText
	**
	******************************************************/
	void writeText()
	{
		theText->clearSheet();
		expert->write( theText->getSheet());
		theText->OnDataChanged();
	}


	SheetWidget *overviewWidget;
	SheetWidget *summaryWidget;
	SheetWidget *stbWidget;
	SheetWidget *saptaWidget;
	SheetWidget *kalaWidget;
	TextWidget *theText;

	ShadBalaExpert *expert;

	wxNotebook *notebook;

};


IMPLEMENT_CLASS( ShadbalaView, BasicView )

/*****************************************************
**
**   ViewFactory  ---   createShadbalaView
**
******************************************************/
BasicView *ViewFactory::createShadbalaView( wxWindow *parent, ChildWindow *frame )
{
	return new ShadbalaView( parent, frame );
}


