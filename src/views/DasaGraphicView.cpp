/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/DasaGraphicView.cpp
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

#include "DasaGraphicView.h"

#include <wx/choicdlg.h>
#include <wx/combobox.h>
#include <wx/event.h>
#include <wx/log.h>
#include <wx/statusbr.h>
#include <wx/textdlg.h>
#include <wx/toolbar.h>

#include "ColorConfig.h"
#include "Commands.h"
#include "Conf.h"
#include "Dasa.h"
#include "DasaTool.h"
#include "DialogElements.h"
#include "Document.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "Painter.h"
#include "ToolPanel.h"

#define ZOOM_FACTOR 1.5
#define MIN_ZOOM 10
#define MAX_ZOOM 50000
#define DASA_TITLE_STEP_WIDTH 1500
#define DASA_BAR_MIN_ITEM_WIDTH 5

extern Config *config;

IMPLEMENT_CLASS( DasaGraphicView, BasicView )
IMPLEMENT_CLASS( DasaGraphicWidget, BasicWidget )

/*****************************************************
**
**   DasaGraphicView   ---   Constructor
**
******************************************************/
DasaGraphicView::DasaGraphicView( wxWindow *parent, ChildWindow *frame )
		: BasicView( parent, frame, VIEW_GRAPHICALDASA, true )
{
	props->setFixedVedic();
	props->setVedic( true );

	//statusbar = new wxStatusBar( this, -1 );
	//statusbar->SetFieldsCount( 4 );

	DasaExpertFactory factory;
	for( int d = 0; d < MAX_DASAEXPERTS; d++ ) experts[d] = factory.getDasaExpert( (DasaId)d, doc );

	swidget = new DasaGraphicWidget( this, props, doc, experts );
	widget = swidget;
	//swidget->setStatusbar( statusbar );

	zoom = config->viewprefs->graphicalDasaWidgetZoom;
	if ( zoom <= 0 ) zoom = 100;
	initToolItems();

	Connect( CMD_SELECT_DASA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnDasas ));
	Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnNow ));
	Connect( CMD_GOTO_YEAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnYear ));
	Connect( CMD_ZOOM_IN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnZoomIn ));
	Connect( CMD_ZOOM_ORIGINAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnZoomOriginal ));
	Connect( CMD_ZOOM_OUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnZoomOut ));
	Connect( CMD_TIP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnHelp ));

	Connect( TBC_DASA_ZOOM, wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DasaGraphicView::OnZoomCombo ));
	Connect( TBC_DASA_ZOOM, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DasaGraphicView::OnZoomCombo ));
	Connect( TBS_ANTARDASALEVEL, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaGraphicView::OnChoiceAntardasa ));
  Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( DasaGraphicView::OnMouseWheelEvent ));

}

/*****************************************************
**
**   DasaGraphicView   ---   Destructor
**
******************************************************/
DasaGraphicView::~DasaGraphicView()
{
	for( int d = 0; d < MAX_DASAEXPERTS; d++ ) delete experts[d];
}

/*****************************************************
**
**   DasaGraphicView   ---   initToolItems
**
******************************************************/
void DasaGraphicView::initToolItems()
{
	if ( toolbar )
	{
		wxComboBox *combo_zoom = (wxComboBox*)toolbar->FindControl( TBC_DASA_ZOOM );
		if ( combo_zoom ) combo_zoom->SetValue( wxString::Format( wxT( "%d" ), zoom ));

		wxChoice *choice_antardasa = (wxChoice*)toolbar->FindControl( TBS_ANTARDASALEVEL );
		if ( choice_antardasa ) choice_antardasa->SetSelection( config->viewprefs->antarDasaLevelChoice );

		ToolbarLabel *tl = (ToolbarLabel*)toolbar->FindControl( TB_TEXT );
		if ( tl ) swidget->setTextField( tl );
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   postCreate
**
******************************************************/
void DasaGraphicView::postCreate()
{
	swidget->setZoom( zoom );
	swidget->setYear( 0 );
}

/*****************************************************
**
**   DasaGraphicView   ---   OnNow
**
******************************************************/
void DasaGraphicView::OnNow( wxCommandEvent& )
{
	swidget->setYear( 0 );
}

/*****************************************************
**
**   DasaGraphicView   ---   OnDataChanged
**
******************************************************/
void DasaGraphicView::OnDataChanged()
{
	swidget->OnDataChanged();
	swidget->Refresh();
}

/*****************************************************
**
**   DasaGraphicView   ---   OnZoomIn
**
******************************************************/
void DasaGraphicView::OnZoomIn( wxCommandEvent& )
{
	setZoom( zoom * ZOOM_FACTOR );
}

/*****************************************************
**
**   DasaGraphicView   ---   OnZoomOut
**
******************************************************/
void DasaGraphicView::OnZoomOut( wxCommandEvent& )
{
	setZoom( zoom / ZOOM_FACTOR );
}

#include <wx/button.h>

/*****************************************************
**
**   DasaGraphicView   ---   OnHelp
**
******************************************************/
void DasaGraphicView::OnHelp( wxCommandEvent& /*event*/ )
{
	wxString title, s;
		
	title << wxT( "Available mouse actions" ) << Endl;
	s << wxT( "Wheel: scroll widget" ) << Endl
		<< wxT( "Shift+Wheel: change Antardasa mode" ) << Endl
		<< wxT( "Ctrl+Wheel: increase/decrease zoom" );

	showHelpPopup( toolbar, title, s );
}

/*****************************************************
**
**   DasaGraphicView   ---   OnZoomOriginal
**
******************************************************/
void DasaGraphicView::OnZoomOriginal( wxCommandEvent& )
{
	setZoom( 100 );
}

/*****************************************************
**
**   DasaGraphicView   ---   setZoom
**
******************************************************/
void DasaGraphicView::setZoom( const int &z )
{
	if ( z < 0 ) zoom = 100;
	else if ( z < MIN_ZOOM ) zoom = MIN_ZOOM;
	else if ( z > MAX_ZOOM ) zoom = MAX_ZOOM;
	else zoom = z;

	if ( toolbar )
	{
		wxComboBox *combo_zoom = (wxComboBox*)toolbar->FindControl( TBC_DASA_ZOOM );
		assert( combo_zoom );
		combo_zoom->SetValue( wxString::Format( wxT( "%d" ), zoom ));
	}
	applyZoom();
}

/*****************************************************
**
**   DasaGraphicView   ---   applyZoom
**
******************************************************/
void DasaGraphicView::applyZoom()
{
	if ( zoom < 0 ) zoom = 100;
	config->viewprefs->graphicalDasaWidgetZoom = zoom;
	swidget->setZoom( zoom );
	swidget->Refresh();
}

/*****************************************************
**
**   DasaGraphicView   ---   OnZoomCombo
**
******************************************************/
void DasaGraphicView::OnZoomCombo( wxCommandEvent& )
{
	printf( "OnZoomCombo\n" );
	if ( toolbar )
	{
		wxComboBox *combo_zoom = (wxComboBox*)toolbar->FindControl( TBC_DASA_ZOOM );
		assert( combo_zoom );

		wxString s = combo_zoom->GetValue();
		long z2 = 100;
		s.ToLong( &z2 );

		// check for invalid values
		if ( z2 == 0 ) zoom = 100;
		else if ( z2 < MIN_ZOOM ) zoom = MIN_ZOOM;
		else if ( z2 > MAX_ZOOM ) zoom = MAX_ZOOM;
		else zoom = (int)z2;

		if ( z2 < MIN_ZOOM || z2 > MAX_ZOOM ) combo_zoom->SetValue( wxString::Format( wxT( "%d" ), zoom ));
		applyZoom();
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   OnMouseWheelEvent
**
******************************************************/
void DasaGraphicView::OnMouseWheelEvent( wxMouseEvent &event )
{
	printf( "DasaGraphicView::OnMouseWheelEvent\n" );
	const bool shiftpressed = event.m_shiftDown;

	if ( shiftpressed )
	{
		if ( toolbar )
		{
			wxChoice *choice_antardasa = (wxChoice*)toolbar->FindControl( TBS_ANTARDASALEVEL );
			if ( choice_antardasa )
			{
				const int oldsel = choice_antardasa->GetSelection();
				int sel = oldsel;

				if ( event.GetWheelRotation() < 0 )
				{
					if ( oldsel + 1 < (int)choice_antardasa->GetCount() ) sel++;
				}
				else sel = Max( 0, sel - 1 );
				
				if ( oldsel != sel )
				{
					choice_antardasa->SetSelection( sel );
					wxCommandEvent e;
					OnChoiceAntardasa( e );
				}
			}
		}
	}
	else
	{
		if ( event.GetWheelRotation() < 0 ) setZoom( zoom / ZOOM_FACTOR );
		else setZoom( zoom * ZOOM_FACTOR );
	}
	Refresh();
}

/*****************************************************
**
**   DasaGraphicView   ---   OnYear
**
******************************************************/
void DasaGraphicView::OnYear( wxCommandEvent& )
{
	wxString defvalue;
	MDate d;
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	int month, day, year;
	long y;

	formatter->calculateDateIntegers( d.getJD(), day, month, year );
	defvalue.Printf( wxT( "%d" ), year );

	wxTextEntryDialog dialog( this, _("Please enter a year"), GUI_APP_NAME, defvalue );
	if ( dialog.ShowModal() == wxID_OK )
	{
		wxString s = dialog.GetValue();
		s.ToLong( &y );
		if ( y <= 0 )
		{
			doMessageBox( this, wxString::Format( _("The value \"%s\" is not valid."), s.c_str() ), wxOK | wxICON_ERROR | wxCENTRE );
			return;
		}
		swidget->setYear( y );
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   OnDasas
**
******************************************************/
void DasaGraphicView::OnDasas( wxCommandEvent& )
{
	wxString l[MAX_DASAEXPERTS];
	DasaTool *tool = DasaTool::get();

	for ( unsigned int i = 0; i < MAX_DASAEXPERTS; i++ )
		l[i] = tool->getDasaName( (DasaId)i );

	wxMultiChoiceDialog dialog( this, _( "Choose Dasas" ), GUI_APP_NAME, MAX_DASAEXPERTS, l );

	dialog.SetSelections( swidget->getDasas() );
	if ( dialog.ShowModal() == wxID_OK )
	{
		swidget->setDasas( dialog.GetSelections() );
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   OnChoiceAntardasa
**
******************************************************/
void DasaGraphicView::OnChoiceAntardasa( wxCommandEvent& )
{
	if ( toolbar )
	{
		wxChoice *choice_antardasa = (wxChoice*)toolbar->FindControl( TBS_ANTARDASALEVEL );
		assert( choice_antardasa );
		config->viewprefs->antarDasaLevelChoice = choice_antardasa->GetSelection();
		swidget->setAntardasaLevel( config->viewprefs->antarDasaLevelChoice );
		swidget->Refresh();
	}
}

/**************************************************************
***
**   GraphicalDasaItem   ---   Destructor
***
***************************************************************/
GraphicalDasaItem::~GraphicalDasaItem()
{
	for ( unsigned int i = 0; i < depitems.size(); i++ ) delete depitems[i];
	depitems.clear();

	if ( dasa ) delete dasa;
	dasa = 0;

	if ( rect ) delete rect;
	rect = 0;
}


/*****************************************************
**
**   DasaGraphicWidget   ---   Constructor
**
******************************************************/
DasaGraphicWidget::DasaGraphicWidget( wxWindow *parent, ChartProperties *chartprops, Horoscope *doc, DasaExpert **experts )
	: BasicWidget( parent, chartprops ),
	totalyears( 100 ),
	jdtotalduration( totalyears * 365.25 ),
	ybar( 30 ),
	doc( doc ),
	experts( experts )
{
	wxString s;
	statusbar = 0;
	setWidgetOptions( WO_EXPORT_GRAFIC );

	writerCfg = new WriterConfig( *config->writer );
	writerCfg->signSymbols = writerCfg->planetSymbols = false;

	for( unsigned int i = 0; i <  config->viewprefs->graphicalDasaList.size(); i++ )
	{
		dasas.Add( config->viewprefs->graphicalDasaList[i] );
	}

	// add default Dasas if something went wrong
	if ( dasas.size() == 0 )
	{
		dasas.Add( 0 );
		dasas.Add( 1 );
		dasas.Add( 2 );
	}

	currentItem = (GraphicalDasaItem*)NULL;
	theText = 0;
	zoom = 100;
	adlevel = config->viewprefs->antarDasaLevelChoice;

	initViewport();
	OnDataChanged();
}

/*****************************************************
**
**   DasaGraphicWidget   ---   Destructor
**
******************************************************/
DasaGraphicWidget::~DasaGraphicWidget()
{
	config->viewprefs->graphicalDasaList.clear();
	for ( unsigned dasa = 0; dasa < dasas.Count(); dasa++ )
	{
		config->viewprefs->graphicalDasaList.push_back( dasas[dasa] );
	}
	for ( unsigned int i = 0; i < rootItems.size(); i++ ) delete rootItems[i];
	delete writerCfg;
}

/*****************************************************
**
**   DasaGraphicWidget   ---   OnDataChanged
**
******************************************************/
void DasaGraphicWidget::OnDataChanged()
{
	unsigned int dasa;
	GraphicalDasaItem *item;

	currentItem  = (GraphicalDasaItem*)NULL;
	echo();

	endjd = doc->getJD() + jdtotalduration;

	for ( unsigned int i = 0; i < rootItems.size(); i++ ) delete rootItems[i];
	rootItems.clear();

	for ( dasa = 0; dasa < dasas.Count(); dasa++ ) // First loop generates Dasas like Vimsottari, Yogini, Kalachakra
	{
		item = new GraphicalDasaItem( dasa, (DasaId)dasas.Item( dasa ));
		calcDasaLevel( item );
		calcItemRectangles( item );
		rootItems.push_back( item );
	}
}

/*****************************************************
**
**   DasaGraphicWidget   ---   doPaint
**
******************************************************/
void DasaGraphicWidget::doPaint( const wxRect &rect, const bool eraseBackground )
{
	const wxLongLong starttime = wxGetLocalTimeMillis();
	DasaTool *tool = DasaTool::get();

	wxString s;
	int x, y;
	wxRect titleRect;

	assert( painter );
	painter->setGraphicFont();

	// paint background
	const wxRect backgroundRect( 0, 0, xviewport, yviewport );
	painter->setPenColor( *wxLIGHT_GREY );
	painter->setBrush( *wxLIGHT_GREY );
	painter->drawRectangle( backgroundRect.Intersect( rect ));

	// paint sheet
	painter->setDefaultBrush();
	painter->setPen( wxPen( config->colors->bgColor ));
	wxRect r = sheetRect;
	r.Intersect( rect );
	printf( "DasaGraphicWidget::doPaint: background rect x %d y %d w %d h %d MODE %d\n", r.x, r.y, r.width, r.height, exportMode );
	painter->drawRectangle( r );

	paintRuler( rect );

	for ( unsigned int i = 0; i < rootItems.size(); i++ )
	{
		// first task: print Dasa name on sheet
		s = tool->getDasaName( rootItems[i]->dasaId );

		// repeat set font because it will be set during Dasa paint
		painter->setGraphicFont();
		painter->setTextColor( config->colors->fgColor );
		MPoint p = painter->getTextExtent( s );

		y = calcYTopForDasaItem( rootItems[i] );
		x = 100;
		while( x + (int)p.real() < sheetRect.x + sheetRect.width )
		{
			titleRect = wxRect( x, y - ybar, (int)p.real(), ybar );
			
			if ( rect.Intersects( titleRect ))
			{
				painter->drawTextFormatted( titleRect, s, Align::Bottom + Align::Left );
			}
			x += DASA_TITLE_STEP_WIDTH;
		}

		// second task: print Dasa
		for( unsigned j = 0; j < rootItems[i]->depitems.size(); j++ )
		{
			paintDasaItem( rootItems[i]->depitems[j], rect );
		}
	}

	// print current item last
	if ( currentItem && config->bardiagram->doItemZoom )
	{
		paintDasaItem( currentItem, rect, false, false );
	}

	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "DasaGraphicWidget::doPaint in %ld millisec eraseBackground %d" ), totaltime.ToLong(), eraseBackground ));
}

/*****************************************************
**
**   DasaGraphicWidget   ---   paintDasaItem
**
******************************************************/
void DasaGraphicWidget::paintDasaItem( GraphicalDasaItem *item, const wxRect& rect,
	const bool doIterate, const bool skipCurrentItem )
{
	assert( item );
	assert( item->rect );
	assert( item->dasa );

	const bool zoomItem = config->bardiagram->doItemZoom && item == currentItem;

	if (( item != currentItem || ! skipCurrentItem ) && rect.Intersects( *item->rect ))
	{
		if ( item->rect->width > DASA_BAR_MIN_ITEM_WIDTH )
		{
			TEXT_FORMAT format = TF_LONG;
			if ( item->rect->width < 80 ) format = TF_SHORT;
			else if ( item->rect->width < 150 ) format = TF_MEDIUM;

			assert( item->graphicalObjectId >= 0 && item->graphicalObjectId < MAX_BARDIAGRAM_COLORS );
			BarDiagramStyle &style = config->bardiagram->style[item->graphicalObjectId];

			painter->setPen( style.pen );
			painter->setTextColor( style.pen.GetColour() );
			painter->setBrush( style.brush );

			const wxRect rr = zoomItem ? zoomRect( item->rect ) : *item->rect;
			painter->drawRectangle( rr, config->bardiagram->cornerRadius );

			ASSERT_VALID_DASA_ID( item->dasaId );
			assert( item->dasa );
			MString ms = experts[item->dasaId]->getDasaDescriptionF( item->dasa, format, writerCfg );
			painter->setGraphicFont();
			painter->drawMString( MRect( rr ), ms, Align::Center );
		}
		else // item too small
		{
			painter->setPenColor( *wxLIGHT_GREY );
			painter->setTransparentBrush();
			painter->drawRectangle( *item->rect );
		}
	}

	const int level = item->getLevel() + 1;

	// attention: sublevel must be painted if rect contains a deeper antardasa
	// y is special: paint can be required even if y value of refresh rect does not intersect item rect -> item is in sublevel
	if ( doIterate && level <= adlevel
		&& item->rect->x + item->rect->width >= rect.x && item->rect->x <= rect.x + rect.width
		&& item->rect->y + item->rect->height >= rect.y - ( adlevel - level ) * ybar && item->rect->y <= rect.y + rect.height
		)
	{
		// calculate dependent items if not available (first time paint)
		if ( item->depitems.size() == 0 ) calcDasaLevel( item );

		for( unsigned int i = 0; i < item->depitems.size(); i++ ) paintDasaItem( item->depitems[i], rect );
	}
}

/*****************************************************
**
**   DasaGraphicWidget   ---   calcDasaLevel
**
******************************************************/
void DasaGraphicWidget::calcDasaLevel( GraphicalDasaItem *item )
{
	unsigned int antardasa;
	int graphicalObjectId;
	int x1 = -1, x2;
	vector<Dasa*> v;

	// calc y value;
	const int y = calcYTopForDasaItem( item );

	ASSERT_VALID_DASA_ID( item->dasaId );

	// Mahadasa
	//printf( "DasaGraphicWidget::calcDasaLevel level %d dasa %ld\n", item->getLevel(), (long)item->dasa );
	if ( ! item->dasa )
	{
		printf( "LEVEL OHNE %d\n", item->getLevel());
		assert( item->getLevel() == -1 );
		v = experts[item->dasaId]->getFirstLevel();
	}
	// Antardasa or deeper
	else
	{
		printf( "LEVEL %d lord %d\n", item->getLevel(), item->dasa->getDasaLord());
		assert( item->getLevel() >= 0 );
		v = experts[item->dasaId]->getNextLevel( item->dasa );
		printf( "Fertig\n" );
	}
	for ( antardasa = 0; antardasa < v.size(); antardasa++ )
	{
		if ( v[antardasa]->getEndJD() > doc->getJD() )
		{
			if ( x1 == -1 ) x1 = jd2x( Max( v[antardasa]->getStartJD(), doc->getJD() ));
			x2 = jd2x( Min( v[antardasa]->getEndJD(), endjd ));

			graphicalObjectId = BarDiagramConfig::getConfigId( v[antardasa]->getDasaLord(), item->dasaId == D_KALACHAKRA );
			item->depitems.push_back( new GraphicalDasaItem( item->visualId, item->dasaId,
				v[antardasa], new wxRect( x1, y, x2 - x1, ybar ), graphicalObjectId ));
			x1 = x2;
		}
	}
}

/*****************************************************
**
**   DasaGraphicWidget   ---   calcItemRectangles
**
******************************************************/
void DasaGraphicWidget::calcItemRectangles( GraphicalDasaItem *item )
{
	assert( item );
	int x1 = -1, x2;
	GraphicalDasaItem *subitem;
	Dasa *dasa;
	const int y = calcYTopForDasaItem( item );

	for( unsigned int i = 0; i < item->depitems.size(); i++ )
	{
		subitem = item->depitems[i];
		dasa = subitem->dasa;
		assert( dasa );

		if ( x1 == -1 ) x1 = jd2x( Max( dasa->getStartJD(), doc->getJD() ));
		x2 = jd2x( Min( dasa->getEndJD(), endjd ));

		if ( subitem->rect ) subitem->rect = new wxRect( x1, y, x2 - x1, ybar );
		else *subitem->rect = wxRect( x1, y, x2 - x1, ybar );

		x1 = x2;

		if ( subitem->depitems.size() > 0 ) calcItemRectangles( subitem );
	}
}

/*****************************************************
**
**   DasaGraphicWidget   ---   paintRuler
**
******************************************************/
void DasaGraphicWidget::paintRuler( const wxRect &rect )
{
	// y value of the ruler
	const int yruler = yviewport - 3 * ybar;

	// tolerances for centered text output
	const int xtext = 50;
	const int ytext = 10;

	// half length of the tickers
	const int yearticker = 20;
	const int quarteryearticker = 15;
	const int monthticker = 10;

	int ylength;
	double x, xm;
	wxString s;
	Lang lang;

	painter->setTransparentBrush();
	painter->setDefaultPen();
	painter->setTextColor( config->colors->fgColor );
	//painter->setGraphicFont();

	wxRect r(  birthx, sheetRect.y, (int)( endjdx - birthx ), sheetRect.height );
	r.Intersect( rect );

	painter->drawLine( r.x, yruler, r.x + r.width, yruler );

	int startyear, endyear, dummy1, dummy2;
	MDate d;

	DateTimeFormatter *formatter = DateTimeFormatter::get();
	formatter->calculateDateIntegers( x2jd( r.x ), dummy1, dummy2, startyear );
	d.setDate( 1, 1, startyear, 0 );
	const int startx = jd2x( d.getJD());

	formatter->calculateDateIntegers( x2jd( r.x + r.width ), dummy1, dummy2, endyear );
	d.setDate( 1, 1, endyear + 1, 0 );
	const int endx = jd2x( d.getJD());

	assert( endx > startx );
	assert( endyear >= startyear );

	const double steplen = ( endx - startx ) / ( endyear + 1 - startyear );
	const double xmonth = steplen / 12.0;

	for ( int i = 0; i <= endyear + 1 - startyear; i++ )
	{
		x = startx + i * steplen;

		if ( x > birthx && x <= endjdx && x >= r.x - xtext && x <= r.x + r.width + xtext )
		{
			painter->drawLine( x, (int)( sheetRect.y + 2 * hborder), x, yruler + yearticker );

			// print year if large enough, otherwise only 10 year steps
			if ( steplen > 40 || ! (( startyear + i ) % 10 ))
			{
				s.Printf( wxT( "%d" ), startyear + i );
				painter->drawTextFormatted( wxRect( x - xtext, yruler + yearticker + ytext, 2 * xtext, 2 * ytext ), s, Align::Center );
			}
		}

		// print month if month has a minimal x length
		if ( xmonth > 3 )
		{
			for ( int j = 1; j < 12; j++ )
			{
				xm = x + j * xmonth;
				if ( xm < birthx || xm > endjdx ) continue;

				if ( xm >= r.x - xtext && xm <= r.x + r.width + xtext )
				{
					ylength = j % 3 ? monthticker : quarteryearticker;
					painter->drawLine( xm, yruler - ylength, xm, yruler + ylength );
					if ( xmonth > 50 )
					{
						painter->drawTextFormatted( wxRect( xm-xtext, yruler + ytext, 2*xtext, 2*ytext ),
							lang.getMonthName( j ).Left( 3 ), Align::Center );
					}
					else if  ( xmonth > 25 )
					{
						s.Printf( wxT( "%d" ), j+1 );
						painter->drawTextFormatted( wxRect( xm-xtext, yruler + ytext, 2*xtext, 2*ytext ), s, Align::Center );
					}
				}
			}
		}

	}

/*
	printf( "Ruler rect %d - %d\n",  r.x, r.width );
	printf( "Refresh rect %d - %d\n",  refreshRect.x, refreshRect.width );
	printf( "Sheet rect %d - %d\n",  sheetRect.x, sheetRect.width );
*/

	// mark current date
	painter->setPen( wxPen( *wxBLUE, 2, wxSOLID ));
	x = jd2x( MDate().getJD());
	painter->drawLine( x, (int)( sheetRect.y + hborder ), x, yruler + 2 * yearticker );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   initViewport
**
******************************************************/
void DasaGraphicWidget::initViewport()
{
	xviewport = 100 * zoom;
	yviewport = 5 * ybar +  ybar * ( adlevel + 2 ) * dasas.Count();
	SetVirtualSize( xviewport, yviewport );

	sheetRect = wxRect( 2 * hborder, 2 * vborder, xviewport - 4 * hborder, yviewport - 4 * vborder );

	// x position of birth and the last date that will be displayed (100 years)
	birthx = 4 * hborder;
	endjdx = xviewport - 4 * hborder;
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setZoom
**
******************************************************/
void DasaGraphicWidget::setZoom( const int z )
{
	if ( z == zoom ) return;

	int xs, ys, oldx, oldy;

	GetViewStart( &oldx, &oldy );
	int xanchor = unscrolledMousePosition.x;// - oldx;

	// mouse outside view, use center as reference
	if ( xanchor == -1 )
	{
		GetSize( &xs, &ys );
		//printf( "xs %d ys %d xanchor %d\n", xs, ys, xanchor );
		xanchor = xs / 2;
	}

	// julian date on mouse position
	const double refjd = x2jd( xanchor + oldx );

	// change viewport and calculate rectangles according to new coordinates
	zoom = z;
	initViewport();
	for( unsigned int i = 0; i < rootItems.size(); i++ )
	{
		calcItemRectangles( rootItems[i] );
	}

	const int newx = jd2x( refjd );
	Scroll( newx - xanchor, oldy );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setDasas
**
******************************************************/
void DasaGraphicWidget::setDasas( const wxArrayInt &d )
{
	dasas = d;
	OnDataChanged();
	initViewport();
	Refresh();
}

/*****************************************************
**
**   DasaGraphicWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void DasaGraphicWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	const bool modpressed = event.m_controlDown;
	const bool shiftpressed = event.m_shiftDown;

	wxLogDebug( wxString::Format( wxT( "DasaGraphicWidget::HandleMouseWheelEvent shiftpressed %d modpressed %d" ), shiftpressed, modpressed ));

	if ( modpressed || shiftpressed )
	{
		event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
		event.Skip();
	}
	else
	{
		int x, y, x0, y0, xs, ys;
		const int offset = 60;

		GetViewStart( &x, &y );      // scroll position
		GetSize( &xs, &ys );         // size of visible widget only
		GetVirtualSize( &x0, &y0 );  // complete size of widget

		// Wheel scolls in vertical direction, but horizontal if nothing left to scroll
		if ( event.GetWheelRotation() < 0 )
		{
			if ( y + ys >= y0 ) x = Min( x0, x + offset );
			else y += offset;
		}
		else
		{
			if ( y > 0 ) y = Max ( 0, y - offset );
			else x -= offset;
		}
		Scroll( x, y );
	}
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setYear
**
******************************************************/
void DasaGraphicWidget::setYear( const int &year )
{
	assert( doc );
	const int scrolly = 1;

	// begin of year will be shifted by that value to the middle of the widget
	const int xoffset = 300;

	MDate d;
	if ( year > 0 )
	{
		d.setDate( 1, 1, year, 0 );
	}
	double portion = ( d.getJD() - doc->getJD() ) / ( endjd - doc->getJD());
	double y = portion * sheetRect.width;
	Scroll( (int)( y / scrolly - xoffset ), 0 );
}


/**************************************************************
***
**   DasaGraphicWidget   ---   x2jd
***
***************************************************************/
double DasaGraphicWidget::x2jd( const int &x )
{
	return doc->getJD() + jdtotalduration * ((double)x - birthx ) / ( endjdx - birthx );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   jd2x
**
******************************************************/
int DasaGraphicWidget::jd2x( const double &jd )
{
	double portion = ( jd - doc->getJD()) / ( totalyears * 365.25 );
	double x = birthx + portion * ( endjdx - birthx );
	return (int)x;
}

/*****************************************************
**
**   DasaGraphicWidget   ---   calcYTopForDasaItem
**
******************************************************/
int DasaGraphicWidget::calcYTopForDasaItem( GraphicalDasaItem *item )
{
	assert( item );
	const int step = item->visualId * ( adlevel + 2 )  + item->getLevel() + 2;
	return( sheetRect.y + ybar * step );
}

/**************************************************************
***
**   DasaGraphicWidget   ---   zoomRect
***
***************************************************************/
wxRect DasaGraphicWidget::zoomRect( const wxRect *rect )
{
	assert( rect );
	const int a = config->bardiagram->itemZoom;
	return wxRect( rect->x - a, rect->y - a, rect->width + 2 * a, rect->height + 2 * a );
}

/**************************************************************
***
**   DasaGraphicWidget   ---   mouseHasMoved
***
***************************************************************/
void DasaGraphicWidget::mouseHasMoved( const bool& /* outside */ )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	Lang lang;
	wxRect zr;
	DasaTool *tool = DasaTool::get();

	assert( theText );

	int x, y;
	CalcUnscrolledPosition( mousePosition.x, mousePosition.y, &x, &y );
	x = mousePosition.x;
	y = mousePosition.y;

	//printf( "DasaGraphicWidget::mouseHasMoved mousePosition.x %d mousePosition.y %d x %d, y %d\n", mousePosition.x, mousePosition.y, x, y );

	int vpx, vpy;
	GetViewStart( &vpx, &vpy );

	GraphicalDasaItem *item = findItemForMousePosition( x, y );

	if ( item && item != currentItem )
	{
		// refresh current item rect because it is not zoomed anymore
		if ( currentItem && config->bardiagram->doItemZoom )
		{
			// old item is not zoomed anymore but the whole area must be repainted
			assert( currentItem->rect );
			zr = zoomRect( currentItem->rect );
			zr.x -= vpx;
			zr.y -= vpy;

			// must be set to null because it requires repaint in unzoomed mode
			currentItem = (GraphicalDasaItem*)NULL;
			RefreshRect( zr, false );
		}

		// draw item in zoomed mode now
		if ( config->bardiagram->doItemZoom )
		{
			zr = zoomRect( item->rect );
			zr.x -= vpx;
			zr.y -= vpy;
			RefreshRect( zr, false );
		}
		echo( tool->getDasaLordChain( item->dasa, true ));
	}
	else if ( item && item == currentItem )
	{
		// nothing to do
	}
	else if ( ! item )
	{
		// repaint area of item because it is not zoomed anymore
		if ( currentItem && config->bardiagram->doItemZoom )
		{
			zr = zoomRect( currentItem->rect );
			zr.x -= vpx;
			zr.y -= vpy;

			// must be set to null because it requires repaint in unzoomed mode
			currentItem = (GraphicalDasaItem*)NULL;
			RefreshRect( zr, false );
		}
		if( config->bardiagram->showToolbarMessages && sheetRect.Contains( x, y ))
			//theText->SetLabel( formatter->getDateStringFromJD( x2jd( x )));
			echo( formatter->formatDateString( x2jd( x )));
		else
			//theText->SetLabel( wxEmptyString );
			echo();
	}
	currentItem = item;
}

/**************************************************************
***
**   DasaGraphicWidget   ---   findItemForMousePosition
***
***************************************************************/
GraphicalDasaItem *DasaGraphicWidget::findItemForMousePosition( const int &x, const int &y )
{
	GraphicalDasaItem *item = (GraphicalDasaItem*)NULL;
	int y0;

	if ( ! sheetRect.Contains( x, y )) return item;

	for ( unsigned i = 0; i < dasas.Count(); i++ )
	{
		// find a Dasa candidat for y value
		y0 = calcYTopForDasaItem( rootItems[i] );
		if ( y >= y0 && y <= y0 + ybar + adlevel * ybar )
		{
			item = findItemForMousePositionInDasa( rootItems[i], x, y );
			if ( item ) return item;
		}
	}
	return item;
}

/**************************************************************
***
**   DasaGraphicWidget   ---   findItemForMousePositionInDasa
***
***************************************************************/
GraphicalDasaItem *DasaGraphicWidget::findItemForMousePositionInDasa( GraphicalDasaItem *item, const int &x, const int &y )
{
	GraphicalDasaItem *di;
	//printf( "findItemForMousePositionInDasa x %d y %d level %d base x %d base width %d\n", x, y, item->level, item->rect ? item->rect->x : -1, item->rect ? item->rect->width : -1);

	for( unsigned int i = 0; i < item->depitems.size(); i++ )
	{
		di = item->depitems[i];
		assert( di );
		assert( di->rect );

		//printf( "Rect in loop level %dx %d y %d w %d h %d\n", item->level, di->rect->x, di->rect->y, di->rect->width, di->rect->height );
		if ( di->rect->Contains( x, y ))
		{
			return di;
		}

		// x coordiante matches but y value is deeper
		else if ( di->rect->x <= x && x <= di->rect->x + di->rect->width )
		{
			return findItemForMousePositionInDasa( di, x, y );
		}
	//printf( "ERROR findItemForMousePositionInDasa not found for x %d y %d\n", x, y );
	}
	wxLogDebug( wxString::Format( wxT( "findItemForMousePositionInDasa not found for x %d y %d" ), x, y ));
	return (GraphicalDasaItem*)NULL;
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setAntardasaLevel
**
******************************************************/
void DasaGraphicWidget::setAntardasaLevel( const int &a )
{
	adlevel = a;
	OnDataChanged();
	initViewport();
}

/*****************************************************
**
**   DasaGraphicWidget   ---   echo
**
******************************************************/
void DasaGraphicWidget::echo( wxString s, const int field )
{
	if ( theText ) theText->SetLabel( s );
	if ( statusbar )
	{
		statusbar->SetStatusText( s, field );
	}
}

/**************************************************************
***
**   DasaGraphicView   ---   getWindowLabel
***
***************************************************************/
wxString DasaGraphicView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Dasa" ) : _( "Graphical Dasa" );
}

/**************************************************************
***
**   DasaGraphicWidget   ---   GetExportSize
***
***************************************************************/
/*
void DasaGraphicWidget::GetExportSize( int* width, int *height) const
{
	*width = 100 * zoom;
	*height = 500;
}
*/

/**************************************************************
***
**   ViewFactory   ---   createDasaGraphicView
***
***************************************************************/
BasicView *ViewFactory::createGraphicalDasaView( wxWindow *parent, ChildWindow *frame )
{
	return new DasaGraphicView( parent, frame );
}


