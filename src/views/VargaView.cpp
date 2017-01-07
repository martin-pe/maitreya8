/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/VargaView.cpp
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

#include "BasicView.h"
#include "Commands.h"
#include "Conf.h"
#include "BarDiagram.h"
#include "Document.h"
#include "GraphicGrid.h"
#include "PlanetList.h"
#include "SheetWidget.h"
#include "TextWidget.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicPlanet.h"

#include <wx/notebook.h>

extern Config *config;

enum { VV_NOTEBOOK = wxID_HIGHEST + 6000 };

/*************************************************//**
*
*  view for Varga topics (charts, bar diagrams and text output)
*
******************************************************/
class VargaView : public BasicView
{
	DECLARE_CLASS( VargaView )

public:

	VargaView( wxWindow *parent, ChildWindow *frame )
			: BasicView( parent, frame, VIEW_VARGA, false, wxFULL_REPAINT_ON_RESIZE )
	{
		props->setFixedVedic();
		notebook = new wxNotebook( this, VV_NOTEBOOK );

		//  VNB_SHADVARGA
		varga1 = new ShadVargaChartGridWidget( notebook, CT_RADIX, props, doc );
		notebook->AddPage( varga1, _( "Shad Varga" ));

		// VNB_VARGA2
		varga2 = new VargaTab2ChartGridWidget( notebook, CT_RADIX, props, doc );
		notebook->AddPage( varga2, _( "Vargas (2)" ));

		//  VNB_VARGA3
		varga3 = new VargaTab3ChartGridWidget( notebook, CT_RADIX, props, doc );
		notebook->AddPage( varga3, _( "Vargas (3)" ));

		// VNB_DIAGRAM
		dwidget = new SheetWidget( notebook, props );
		notebook->AddPage( dwidget, _( "Diagram" ));

		// VNB_TEXT
		twidget = new TextWidget( notebook, props );
		notebook->AddPage( twidget, _( "Text" ));

		widget = notebook;
		notebook->SetSelection( config->viewprefs->activePageVargaOverview );
		OnDataChanged();
	}

	~VargaView()
	{
		config->viewprefs->activePageVargaOverview = notebook->GetSelection();
	}

	virtual wxString getWindowLabel( const bool = false ) { return _( "Vargas" ); }

	virtual void OnDataChanged()
	{
		varga1->OnDataChanged();
		varga2->OnDataChanged();
		varga3->OnDataChanged();

		VargaExpert expert( doc );
		twidget->clearSheet();
		expert.write( twidget->getSheet(), props->getVedicObjectStyle() );
		twidget->OnDataChanged();

		dwidget->clearSheet();
		Sheet *dsheet = dwidget->getSheet();

		BarDiagramWriter bdwriter( dsheet, props, doc );
		bdwriter.writeVargaDiagrams();

		dwidget->OnDataChanged();

		notebook->GetCurrentPage()->Refresh();
	}

protected:

	ChartGridWidget *varga1, *varga2, *varga3;

	TextWidget *twidget;
	SheetWidget *dwidget;

	wxNotebook *notebook;
};

IMPLEMENT_CLASS( VargaView, BasicView )

/*************************************************//**
*
*  \brief container for a grid of graphical charts
*
******************************************************/
class GraphicGridView : public BasicView
{
	DECLARE_CLASS( GraphicGridView )

public:

	GraphicGridView( wxWindow *parent, ChildWindow *frame, const int &rows, const int &cols, vector<Varga> vargas )
		: BasicView( parent, frame, VIEW_GRAPHICGRID, false )
	{
		props->setFixedVedic();
		sgw = new ChartGridWidget( this, CT_RADIX, props, rows, cols );

		for ( uint i = 0; i < vargas.size(); i++ )
		{
			sgw->addVedicChart( vargas[i], doc );
		}
		widget = sgw;
	}

	virtual wxString getWindowLabel( const bool = false ) { return _( "Vargas" ); }

	virtual void OnDataChanged()
	{
		sgw->OnDataChanged();
	}

protected:
	ChartGridWidget *sgw;

};
IMPLEMENT_CLASS( GraphicGridView, BasicView )

/*************************************************//**
*
* \brief Simple view for Vimsopaka Bala (used in Mulitple View)
*
******************************************************/
class VimsopakaBalaView : public BasicView
{
	DECLARE_CLASS( VimsopakaBalaView )

public:
	VimsopakaBalaView( wxWindow *parent, ChildWindow *frame ) : BasicView( parent, frame, VIEW_VIMSOPAKABALA, false )
	{
		props->setFixedVedic();
		dwidget = new SheetWidget( this, props );

		widget = dwidget;
		OnDataChanged();
	}

	virtual wxString getWindowLabel( const bool = false ) { return _( "Vimsopaka Bala" ); }

	virtual void OnDataChanged()
	{
		dwidget->clearSheet();
		Sheet *dsheet = dwidget->getSheet();

		BarDiagramWriter bdwriter( dsheet, props, doc );
		bdwriter.writeVargaDiagrams();

		dwidget->OnDataChanged();
	}

protected:

	SheetWidget *dwidget;

};

IMPLEMENT_CLASS( VimsopakaBalaView, BasicView )

/*****************************************************
**
**   ViewFactory  ---   createVimsopakaBalaView
**
******************************************************/
BasicView *ViewFactory::createVimsopakaBalaView( wxWindow *parent, ChildWindow *frame )
{
	return new VimsopakaBalaView( parent, frame );
}

/*****************************************************
**
**   ViewFactory  ---   createVargaView
**
******************************************************/
BasicView *ViewFactory::createVargaView( wxWindow *parent, ChildWindow *frame )
{
	return new VargaView( parent, frame );
}

/*****************************************************
**
**   ViewFactory  ---   createGraphicGridView
**
******************************************************/
BasicView *ViewFactory::createGraphicGridView( wxWindow *parent, ChildWindow *frame, const int &rows, const int &cols, vector<int> vargas )
{
	vector<Varga> v;
	for( uint i = 0; i < vargas.size(); i++ ) v.push_back( (Varga)(vargas[i] ));
	return new GraphicGridView( parent, frame, rows, cols, v );
}

