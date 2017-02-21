/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BarDiagram.cpp
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

#include "BarDiagram.h"

#include "Ashtakavarga.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "Painter.h"
#include "PlanetList.h"
#include "SymbolProvider.h"
#include "VedicPlanet.h"

#include<math.h>

extern Config *config;

IMPLEMENT_CLASS( BarDiagramItem, SheetWidgetItem )
IMPLEMENT_CLASS( BarDiagramWidget, SheetWidgetItem )

#define BARDIAGRAM_BREAK_X 800
#define x3d 5
#define y3d 10

/*****************************************************
**
**   BarDiagramWriter   ---   Constructor
**
******************************************************/
BarDiagramWriter::BarDiagramWriter( Sheet *sheet, ChartProperties *chartprops, Horoscope *h )
 : sheet( sheet ),
 props( chartprops ),
 horoscope( h )
{
}

/*****************************************************
**
**   BarDiagramWriter   ---   writeVargaDiagrams
**
******************************************************/
void BarDiagramWriter::writeVargaDiagrams()
{
	const ObjectArray &obs = PlanetList().getVedicPlanetOnlyList();

	uint i;

	SheetWidgetGrid *grid = new SheetWidgetGrid( 3 );

	BarDiagramWidget *diagram1 = new BarDiagramWidget( props, _( "Shad Varga" ));
	for ( i = 0; i < obs.size(); i++ ) diagram1->addItem( new BarDiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 0 )));
	grid->addItem( diagram1 );

	BarDiagramWidget *diagram2 = new BarDiagramWidget( props, _( "Sapta Varga" ));
	for ( i = 0; i < obs.size(); i++ ) diagram2->addItem( new BarDiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 1 )));
	grid->addItem( diagram2 );

	BarDiagramWidget *diagram3 = new BarDiagramWidget( props, _( "Dasa Varga" ));
	for ( i = 0; i < obs.size(); i++ ) diagram3->addItem( new BarDiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 2 )));
	grid->addItem( diagram3 );

	BarDiagramWidget *diagram4 = new BarDiagramWidget( props, _( "Shodasa Varga" ));
	for ( i = 0; i < obs.size(); i++ ) diagram4->addItem( new BarDiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 3 )));
	grid->addItem( diagram4 );

	BarDiagramWidget *diagram5 = new BarDiagramWidget( props, _( "Average" ));
	for ( i = 0; i < obs.size(); i++ ) diagram5->addItem( new BarDiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getAverageVimsopakaBala()));
	grid->addItem( diagram5 );
	sheet->addItem( grid );
}

#define RASIDIAGRAM_XMIN 400
#define RASIDIAGRAM_XMAX 600
#define RASIDIAGRAM_RATIO .6

/*****************************************************
**
**   BarDiagramWriter   ---   writeAvRasiDiagrams
**
******************************************************/
void BarDiagramWriter::writeAvRasiDiagrams( AshtakavargaExpert *expert )
{
	Rasi rasi;
	//SheetWidgetGrid *grid = new SheetWidgetGrid( 3 );
	SheetWidgetGrid *grid = new SheetWidgetGrid( 3, .3, 400, 200, 1600, 800 );

	BarDiagramWidget *diagram1 = new BarDiagramWidget( props, _( "Rekha" ), false, RASIDIAGRAM_XMIN, RASIDIAGRAM_XMAX, RASIDIAGRAM_RATIO );
	for ( rasi = R_ARIES; rasi <= R_PISCES; rasi++ ) diagram1->addItem( new BarDiagramItem( (int)rasi, expert->getSarva( REKHA, rasi )));
	grid->addItem( diagram1 );

	BarDiagramWidget *diagram2 = new BarDiagramWidget( props, _( "Trikona Shodana" ), false, RASIDIAGRAM_XMIN, RASIDIAGRAM_XMAX, RASIDIAGRAM_RATIO  );
	for ( rasi = R_ARIES; rasi <= R_PISCES; rasi++ ) diagram2->addItem( new BarDiagramItem( (int)rasi, expert->getSarva( TRIKONA, rasi )));
	grid->addItem( diagram2 );

	BarDiagramWidget *diagram3 = new BarDiagramWidget( props, _( "Ekadhipatya Shodana" ), false, RASIDIAGRAM_XMIN, RASIDIAGRAM_XMAX, RASIDIAGRAM_RATIO );
	for ( rasi = R_ARIES; rasi <= R_PISCES; rasi++ ) diagram3->addItem( new BarDiagramItem( (int)rasi, expert->getSarva( EKADHI, rasi )));
	grid->addItem( diagram3 );
	sheet->addItem( grid );
}

/*****************************************************
**
**   BarDiagramWriter   ---   writeAvPindaDiagrams
**
******************************************************/
void BarDiagramWriter::writeAvPindaDiagrams( AshtakavargaExpert *expert )
{
	ObjectArray obs = PlanetList().getVedicPlanetOnlyList();
	obs.push_back( OASCENDANT );
	uint i;

	//SheetWidgetGrid *grid = new SheetWidgetGrid( 3 );
	SheetWidgetGrid *grid = new SheetWidgetGrid( 3, .3, 400, 200, 1600, 800 );
	BarDiagramWidget *diagram4 = new BarDiagramWidget( props, _( "Graha Pinda" ));
	for ( i = 0; i < obs.size(); i++ ) diagram4->addItem( new BarDiagramItem( obs[i], expert->getPinda( GRAHAPINDA, i )));
	grid->addItem( diagram4 );

	BarDiagramWidget *diagram5 = new BarDiagramWidget( props, _( "Rasi Pinda" ));
	for ( i = 0; i < obs.size(); i++ ) diagram5->addItem( new BarDiagramItem( obs[i], expert->getPinda( RASIPINDA, i )));
	grid->addItem( diagram5 );

	BarDiagramWidget *diagram6 = new BarDiagramWidget( props, _( "Sodhya Pinda" ));
	for ( i = 0; i < obs.size(); i++ ) diagram6->addItem( new BarDiagramItem( obs[i], expert->getPinda( YOGAPINDA, i )));
	grid->addItem( diagram6 );
	sheet->addItem( grid );
}

/*****************************************************
**
**   BarDiagramItem   ---   Constructor
**
******************************************************/
BarDiagramItem::BarDiagramItem( const int id, const double v, BarDiagramConfig* cfg )
	: SheetItem( WiBar ),
	objectId( id ),
	value(  v )
{
	if ( cfg ) barconfig = cfg;
	else barconfig = config->bardiagram;
}

/*****************************************************
**
**   BarDiagramItem   ---   cloneClean
**
******************************************************/
SheetItem *BarDiagramItem::cloneClean()
{
	BarDiagramItem *item = new BarDiagramItem( objectId, value );
	return item;
}

/*****************************************************
**
**   BarDiagramWidget   ---   Constructor
**
******************************************************/
BarDiagramWidget::BarDiagramWidget( ChartProperties *props, wxString title, const bool planetmode, const double /*xmin*/,
	const double /*xmax*/, const double /*ratio*/, BarDiagramConfig *bconfig )
 : SheetWidgetItem( props ),
	title( title ),
	planetmode( planetmode ),
	deltasteps( 4 ),
  xmargin( 5 ),
	xdmargin( 10 ),
	ymargin( 10 ),
	ydmargin( 10 )
{
	barconfig = bconfig ? bconfig : config->bardiagram;
	minvalue = maxvalue = 0;
}

/*****************************************************
**
**   BarDiagramWidget   ---   Destructor
**
******************************************************/
BarDiagramWidget::~BarDiagramWidget()
{
	clear();
}

/*****************************************************
**
**   BarDiagramWidget   ---   cloneClean
**
******************************************************/
SheetItem *BarDiagramWidget::cloneClean()
{
	SheetItem *item = new BarDiagramWidget( props, title, planetmode );
	for ( list<BarDiagramItem*>::iterator iter = subitems.begin(); iter != subitems.end(); iter++ )
	//for ( uint i = 0; i < data.size(); i++ )
	{
		// TODO copy constructor
		assert( false );
	}
	return item;
}

/*****************************************************
**
**   BarDiagramWidget   ---   clear
**
******************************************************/
void BarDiagramWidget::clear()
{
	for ( list<BarDiagramItem*>::iterator iter = subitems.begin(); iter != subitems.end(); iter++ )
	{
		delete *iter;
	}
	subitems.clear();
	maxvalue = minvalue = 0;
}

/*****************************************************
**
**   BarDiagramWidget   ---   addItem
**
******************************************************/
void BarDiagramWidget::addItem( BarDiagramItem *item )
{
	subitems.push_back( item );
	maxvalue = Max( item->value, maxvalue );
	minvalue = Min( item->value, minvalue );
}

#define BAR_DIAGRAM_CONTENTRECT_TOL .01
#define BAR_DIAGRAM_CONTENTRECT_CORNER_RADIUS .01

/*****************************************************
**
**   BarDiagramWidget   ---   doPaint
**
******************************************************/
void BarDiagramWidget::doPaint( Painter *painter, const MRect& /*refreshRect*/ )
{
	//printf( "BarDiagramWidget::doPaint\n" );
	const int graphicfont_scalingfactor = 1.0;
	wxString s;
	Lang lang;
	SymbolProvider sp;
	double value, currenty;

	// Find a reasonable value for maxvalue
	double dmaxvalue =  maxvalue * 1.1;
	if ( dmaxvalue > 40 ) dmaxvalue = 20 * ceil( dmaxvalue / 20 );
	else dmaxvalue = 8 * ceil( dmaxvalue / 8 );

	// dmaxvalue = 0 may happen if all values are null (e.g. Yuddha Bala)
	if ( dmaxvalue == 0 ) dmaxvalue = 60;

	// Total y value
	const double valuespan = dmaxvalue - minvalue;
	//printf( "maxvalue %f minvalue %f span %f\n", dmaxvalue, minvalue, valuespan );

	// calculate content rect
	const double xm0 = rect.width * BAR_DIAGRAM_CONTENTRECT_TOL;
	const double ym0 = rect.height * BAR_DIAGRAM_CONTENTRECT_TOL;
	MRect contentRect = rect;
	contentRect.x += xm0;
	contentRect.y += ym0;
	contentRect.width -= 2 * xm0;
	contentRect.height -= 2 * ym0;
	const double cornerRadius = rect.width * BAR_DIAGRAM_CONTENTRECT_CORNER_RADIUS;

	// draw frame on content rect
	painter->setDefaultPen();
	painter->setTransparentBrush();
	painter->setDefaultTextColor();
	//printf( "drawRectangle %f %f %f %f\n", contentRect.x, contentRect.y, contentRect.width, contentRect.height );
	painter->drawRectangle( contentRect, cornerRadius );

	// font scaling
	painter->setGraphicFontZoom( graphicfont_scalingfactor );
	MPoint p = painter->getTextExtent( wxT( "0" ));
	text_height = p.imag();

	const MRect rulerRect(
		contentRect.x + deltasteps * xdmargin,
		contentRect.y + 2 * ymargin,
		contentRect.width - ( 1 + deltasteps ) * xdmargin,
		contentRect.height - 4 * ydmargin
	);

	const double xstep = rulerRect.width / subitems.size();
	const double xtol = xstep / 10;
	const double ystep = rulerRect.height / 4;

	// paint ruler
	painter->drawLine( rulerRect.x, rulerRect.y + rulerRect.height, rulerRect.x + rulerRect.width, rulerRect.y + rulerRect.height );
	painter->drawLine( rulerRect.x, rulerRect.y + rulerRect.height, rulerRect.x, rulerRect.y );

	// draw another horizontal line in case of negative y values
	if ( minvalue < 0 )
	{
		const double y0line = rulerRect.y + rulerRect.height * ( 1 + minvalue / valuespan );
		painter->drawLine( rulerRect.x, y0line, rulerRect.x + rulerRect.width, y0line );
	}

	// numbers at left side of ruler
	for ( int i = 0; i <= 4; i++ )
	{
		currenty = rulerRect.y + rulerRect.height - i * ystep;
		painter->drawLine( rulerRect.x - ydmargin, currenty, rulerRect.x, currenty );
		painter->drawTextFormatted( MRect( rulerRect.x - xstep - ydmargin, currenty - ymargin, xstep, 2 * ymargin ),
			wxString::Format( wxT( "%d" ), (int)( minvalue + .25 * i * valuespan )),
			Align::Right + Align::VCenter );
	}

	// draw title
	if ( ! title.IsEmpty())
	{
		painter->drawTextFormatted(
			MRect( contentRect.x, rulerRect.y, contentRect.width, contentRect.height - 2 * ymargin ),
			title, Align::Top + Align::HCenter );
	}

	// draw bar items
	int i = 0;
	for ( list<BarDiagramItem*>::iterator iter = subitems.begin(); iter != subitems.end(); iter++ )
	{
		value = fabs( rulerRect.height * (*iter)->value / valuespan );
		double ybarbottom = rulerRect.y + rulerRect.height;

		if ( minvalue < 0 )
		{
			if ( (*iter)->value >= 0 )
			{
				// shift bars for positive values
				ybarbottom += rulerRect.height * minvalue / valuespan;
			}
			else
			{
				ybarbottom += rulerRect.height * ( minvalue - (*iter)->value ) / valuespan;
			}
		}
		(*iter)->rect = MRect( rulerRect.x + i * xstep + xtol, ybarbottom - value, xstep - 2 * xtol, value );
		drawBar( painter, *iter );
		i++;
	}
	painter->setDefaults();

	const bool symbolmode = ( planetmode ? config->writer->planetSymbols : config->writer->signSymbols );
	if ( symbolmode ) painter->setSymbolFont();

	// draw signs resp planets below the ruler
	i = 0;
	for ( list<BarDiagramItem*>::iterator iter = subitems.begin(); iter != subitems.end(); iter++ )
	{
		if ( symbolmode )
		{
			if ( planetmode ) s = sp.getPlanetCode( (ObjectId)(*iter)->objectId );
			else s = sp.getSignCode( (Rasi)(*iter)->objectId );
		}
		else
		{
			if ( planetmode ) s = lang.getObjectName( (ObjectId)(*iter)->objectId, TF_MEDIUM );
			else s = lang.getSignName( (Rasi)(*iter)->objectId, TF_MEDIUM );
		}
		painter->drawTextFormatted( MRect( rulerRect.x + i * xstep, rulerRect.y + rulerRect.height, xstep, 2 * text_height ),
			s, Align::Top + Align::HCenter );
		i++;
	}
	painter->setGraphicFontZoom( graphicfont_scalingfactor );
}

/*****************************************************
**
**   BarDiagramWidget   ---   drawBar
**
******************************************************/
void BarDiagramWidget::drawBar( Painter *painter, BarDiagramItem *item )
{
	wxString s;

	assert( item );

	if ( item->value != 0 )
	{
		//GraphicalDasaPaintHelper( painter ).setDasaColors( object, ! planetmode );
		int objectid = config->bardiagram->getConfigId( item->objectId, ! planetmode );
		const BarDiagramStyle &barstyle = barconfig->style[objectid];

		painter->setPen( barstyle.pen );
		painter->setTextColor( barstyle.pen.GetColour() );
		painter->setBrush( barstyle.brush );
	}
	else
	{
		painter->setDefaults();
	}

	MRect rect = item->rect;

	const double xright = rect.x + rect.width;
	const double ybottom = rect.y + rect.height;

	MPoint rightRhomb[4] = {
		MPoint( xright, ybottom ),
		MPoint( xright + x3d, ybottom - y3d ),
		MPoint( xright + x3d, rect.y - y3d ),
		MPoint( xright, rect.y  )
	};
	painter->drawPolygon( 4, rightRhomb );

	MPoint topRhomb[4] = {
		MPoint( rect.x, rect.y ),
		MPoint( rect.x + x3d, rect.y - y3d ),
		MPoint( xright + x3d, rect.y - y3d ),
		MPoint( xright, rect.y )
	};
	painter->drawPolygon( 4, topRhomb );

	painter->drawRectangle( rect );

	s.Printf( wxT( "%d" ), (int)( item->value > 0 ?  item->value + .5 : item->value - .5 ) ); // rounded value
	if ( rect.height > text_height ) painter->drawTextFormatted( rect, s, Align::Center );
	else
	{
		painter->setTextColor( config->colors->fgColor );
		painter->drawTextFormatted( MRect( rect.x, rect.y - 2 * text_height, rect.width, text_height ), s, Align::HCenter + Align::Bottom );
	}
}



