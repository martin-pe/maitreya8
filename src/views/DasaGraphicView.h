/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/DasaGraphicView.h
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

#ifndef _DASAGRAPHICVIEW_H_
#define _DASAGRAPHICVIEW_H_

#include "BasicWidget.h"
#include "BasicView.h"
#include "Dasa.h"

#include <vector>

using namespace std;

class wxMouseEvent;
class wxStatusBar;
class Horoscope;
class DasaGraphicWidget;
class Dasa;
class DasaExpert;
class ToolbarLabel;

/*************************************************//**
*
* \brief view for grahical Dasa
*
******************************************************/
class DasaGraphicView : public BasicView
{
	DECLARE_CLASS( DasaGraphicView )

public:

	DasaGraphicView( wxWindow*, ChildWindow* );
	~DasaGraphicView();

	virtual wxString getWindowLabel( const bool shortname = false );

private:

	virtual void OnDataChanged();

	virtual void postCreate();

	virtual void initToolItems();

	// set zoom variable and write into combo box
	void setZoom( const int& );

	// apply changes on widget
	void applyZoom();

	void OnNow( wxCommandEvent& );
	void OnMouseWheelEvent( wxMouseEvent& );
	void OnYear( wxCommandEvent& );
	void OnDasas( wxCommandEvent& );
	void OnChoiceAntardasa( wxCommandEvent& );
	void OnZoomCombo( wxCommandEvent& );
	void OnZoomIn( wxCommandEvent& );
	void OnZoomOut( wxCommandEvent& );
	void OnZoomOriginal( wxCommandEvent& );
	void OnHelp( wxCommandEvent& );

	DasaExpert *experts[MAX_DASAEXPERTS];
	DasaGraphicWidget *swidget;
	int zoom;
};

/*************************************************//**
*
* \brief represents a single item in graphical Dasa
*
******************************************************/
class GraphicalDasaItem
{
public:

	GraphicalDasaItem( const int &visualId, const DasaId &dasaId, Dasa *d = 0, wxRect *r = 0, const int graphicalObjectId = 0 )
	: visualId( visualId ),
		dasaId( dasaId ),
		dasa( d ),
		rect( r ),
		graphicalObjectId( graphicalObjectId )
	{}

	~GraphicalDasaItem();

	int getLevel() const { return dasa ? dasa->getLevel() : -1; }

	// the index 0..n of the Dasa in the diagram
	const int visualId;

	const DasaId dasaId;

	// jyotish class reference
	Dasa* dasa;

	// rectangle coordinates
	wxRect *rect;

	// for access to the config style stock
	int graphicalObjectId;

	// antardasas etc of this Dasa
	vector<GraphicalDasaItem*> depitems;
};

/*************************************************//**
*
* \brief widget for graphical Dasa
*
******************************************************/
class DasaGraphicWidget : public BasicWidget
{
	DECLARE_CLASS( DasaGraphicWidget )

public:
	DasaGraphicWidget( wxWindow *parent, ChartProperties*, Horoscope *doc, DasaExpert *experts[MAX_DASAEXPERTS] );

	~DasaGraphicWidget();

	void OnDataChanged();

	void setZoom( const int = 0 );

	void setDasas( const wxArrayInt& );

	const wxArrayInt &getDasas() { return dasas; }

	void setAntardasaLevel( const int& );

	void setYear( const int& );

	void setTextField( ToolbarLabel* sb ) { theText = sb; }

	void setStatusbar( wxStatusBar *sb ) { statusbar = sb; }

private:

	void calcDasaLevel( GraphicalDasaItem* );

	void calcItemRectangles( GraphicalDasaItem *item );

	virtual void doPaint( const wxRect&, const bool eraseBackground = true );

	void paintDasaItem( GraphicalDasaItem*, const wxRect&, const bool doIterate = true, const bool skipCurrentItem = true );

	void paintRuler( const wxRect& );

	void initViewport();

	GraphicalDasaItem *findItemForMousePosition( const int&, const int& );

	GraphicalDasaItem *findItemForMousePositionInDasa( GraphicalDasaItem *item, const int &x, const int &y );

	virtual void HandleMouseWheelEvent( wxMouseEvent& );

	void mouseHasMoved( const bool &outside );

	wxRect zoomRect( const wxRect *rect );

	int calcYTopForDasaItem( GraphicalDasaItem* );

	int jd2x( const double& );

	double x2jd( const int& );

	void echo( wxString = wxEmptyString, const int field = 0 );

	// in percent
	int zoom;

	//  depth of antardasa calculation
	int adlevel;

	// total view size
	int xviewport, yviewport;

	// coordinates of the sheet
	wxRect sheetRect;

	// number of year the sheet covers
	const int totalyears;

	// x position of birth
	double birthx;

	// x position of last date
	double endjdx;

	// last date of sheet
	double endjd;
	
	// days from birth to endjd
	const double jdtotalduration;

	// constant height of Dasa bar
	const int ybar;

	// hold the IDs of the correspoding Dasa experts
	wxArrayInt dasas;

	// active item under mouse cursor
	GraphicalDasaItem *currentItem;

	Horoscope *doc;

	vector <GraphicalDasaItem*> rootItems;

	DasaExpert **experts;

	ToolbarLabel *theText;

	WriterConfig *writerCfg;

	wxStatusBar *statusbar;
};

#endif

